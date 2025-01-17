// ======================================================================
// Author : $Author$
// Version: $Revision: 1502 $
// Date   : $Date: 2018-07-16 12:55:14 +0000 (Mon, 16 Jul 2018) $
// Url    : $URL$
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
// ======================================================================

// ======================================================================
// Copyright: (C) 2010-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "db_tree.h"
#include "db_database.h"
#include "db_game_info.h"
#include "db_eco_table.h"
#include "db_signature.h"
#include "db_board.h"
#include "db_exception.h"

#include "u_byte_stream.h"
#include "u_progress.h"

#include "m_bitset.h"
#include "m_limits.h"
#include "m_ref_counted_ptr.h"

#include <string.h>

#ifdef SHOW_TREE_INFO
# include "m_stdio.h"
#endif

using namespace db;

typedef EcoTable::Successors Successors;

static TreeInfo* moveCache[1 << Move::Index_Bit_Length];


static uint16_t const Empty	= Move::Empty;
static uint16_t const Null		= sq::h8 | (sq::h8 << 6);


static Successors::Successor const*
findSuccessor(Successors const& successors, uint32_t eco)
{
	for (unsigned i = 0; i < successors.length; ++i)
	{
		M_ASSERT(!successors.list[i].reachable.empty());

		if (successors.list[i].reachable.test(eco))
			return &successors.list[i];
	}

	return 0;
}


inline static
uint16_t
index(Move const& m)
{
	return m.isNull() ? ::Null : m.index();
}


inline static
Move
move(Board const& position, uint16_t m)
{
	if (m == ::Null)	return Move::null();
	if (m == ::Empty)	return Move::empty();

	Move move = position.makeMove(m);
	move.setLegalMove();

	return move;
}


Tree::Key::Key()
	:m_hash(0)
	,m_position(Board::emptyBoard().exactZHPosition())
	,m_method(tree::Exact)
	,m_ratingType(rating::Any)
{
}


Tree::Key::Key(uint64_t hash,
					Position const& position,
					tree::Method method,
					tree::Mode mode,
					rating::Type ratingType)
	:m_hash(hash)
	,m_position(position)
	,m_method(method)
	,m_mode(mode)
	,m_ratingType(ratingType)
{
}


void
Tree::Key::clear()
{
	m_hash = 0;
	m_position = Board::emptyBoard().exactZHPosition();
}


void
Tree::Key::set(tree::Method method,
					tree::Mode mode,
					rating::Type ratingType,
					uint64_t hash,
					Position const& position)
{
	m_hash = hash;
	m_position = position;
	m_method = method;
	m_mode = mode;
	m_ratingType = ratingType;
}


void
Tree::add(GameInfo const& info, Eco eco, uint16_t move, Board const& myPosition)
{
	M_ASSERT(move < U_NUMBER_OF(::moveCache));

	m_filter.add(m_index);

	TreeInfo* tinfo = ::moveCache[move];

	if (tinfo == 0)
	{
		Move m = ::move(myPosition, move);

		m_infoList.reserve_exact(m_infoList.size() + 1);
#ifdef SUPPORT_TREE_INFO_FILTER
		m_infoList.push_back(TreeInfo(eco, m, m_filter.size()));
#else
		m_infoList.push_back(TreeInfo(eco, m, m_index));
#endif
		::moveCache[move] = tinfo = &m_infoList.back();
	}
	else
	{
		M_ASSERT(!eco || !tinfo->eco() || eco == tinfo->eco());
		M_ASSERT(::move(myPosition, move) == tinfo->move());
	}

	tinfo->add(info, myPosition.sideToMove(), m_key.ratingType());

#ifdef SUPPORT_TREE_INFO_FILTER
	tinfo->addGame(m_index);
#endif

#ifdef BUILD_VARIATION_LIST
	if (m_buildVariationList && eco.ecoKey())
		addToVariation(eco, info.ecoKey());
#endif
}


#ifdef SUPPORT_TREE_INFO_FILTER

void
Tree::compressFilter()
{
	m_filter.compress();

	for (unsigned i = 0; i < m_infoList.size(); ++i)
		m_infoList[i].uncompressFilter();
}


void
Tree::uncompressFilter()
{
	m_filter.uncompress();

	for (unsigned i = 0; i < m_infoList.size(); ++i)
		m_infoList[i].uncompressFilter();
}

#endif


void
Tree::possiblyAdd(Database const& base,
						GameInfo const& info,
						tree::Mode mode,
						Eco eco,
						Board const& myPosition)
{
	try
	{
		Move m = base.findExactPosition(m_index, myPosition, mode == tree::MainlineOnly);

		if (!m.isInvalid())
			add(info, eco, ::index(m), myPosition);
	}
	catch (IOException const& exc)
	{
		// ignore errors while decoding
	}
	catch (...)
	{
		throw;
	}

#ifdef SHOW_TREE_INFO
	++m_numGamesParsed;
#endif
}


bool
Tree::buildTree0(	unsigned myIdn,
						Board const& startPosition,
						Board const& myPosition,
						Line const& myLine,
						uint16_t hpSig,
						Database const& base,
						tree::Method method,
						tree::Mode mode,
						ReachableFunc reachableFunc,
						util::Progress& progress,
						unsigned frequency,
						unsigned numGames)
{
	typedef EcoTable::EcoSet EcoSet;

	M_ASSERT(myIdn == 0);
//	M_ASSERT(!myPosition.isStartPosition());

	unsigned	reportAfter = m_index + frequency;
	EcoSet	reachable;

	for (unsigned n = numGames; m_index < n; ++m_index)
	{
		if (reportAfter == m_index)
		{
			progress.update(m_index);
			reportAfter += frequency;
		}

		if (progress.interrupted())
			return false;

		GameInfo const& info = base.gameInfo(m_index);

		if (mode == tree::IncludeVariations && info.countVariations() > 0)
		{
			possiblyAdd(base, info, mode, Eco(), myPosition);
		}
		// XXX reachableFunc should not use home pawns
		else if (info.idn()
					? reachableFunc(myPosition.signature(), info.signature(), hpSig)
					: Signature::isReachable(myPosition.signature(), info.signature()))
		{
			possiblyAdd(base, info, mode, Eco(), myPosition);
		}
	}

	return true;
}


bool
Tree::buildTree518(	unsigned myIdn,
							Board const& startPosition,
							Board const& myPosition,
							Line const& myLine,
							uint16_t hpSig,
							Database const& base,
							tree::Method method,
							tree::Mode mode,
							ReachableFunc reachableFunc,
							util::Progress& progress,
							unsigned frequency,
							unsigned numGames)
{
	typedef EcoTable::EcoSet EcoSet;

	M_ASSERT(myIdn == variant::Standard);
	M_ASSERT(!myPosition.isStandardPosition(base.variant()));

	unsigned				reportAfter = m_index + frequency;
	Successors			successors;
	unsigned				myLength;
	EcoSet				reachable;
	pawns::Progress	myProgress;

//	Eco myEco = EcoTable::specimen().getEco(myLine);
	Eco myKey = EcoTable::specimen(base.variant()).lookup(myLine, &myLength, &successors, &reachable);

	myProgress.side[color::White] = myPosition.signature().progress(color::White);
	myProgress.side[color::Black] = myPosition.signature().progress(color::Black);

	for ( ; m_index < numGames; ++m_index)
	{
		if (reportAfter == m_index)
		{
			progress.update(m_index);
			reportAfter += frequency;
		}

		if (progress.interrupted())
			return false;

		GameInfo const& info = base.gameInfo(m_index);

		if (info.idn() == variant::Standard)
		{
			if (info.plyCount() && reachableFunc(myPosition.signature(), info.signature(), hpSig))
			{
				if (mode == tree::IncludeVariations && info.countVariations() > 0)
				{
					possiblyAdd(base, info, mode, Eco(), myPosition);
				}
//				else if (	myLength == info.plyCount()
//						&& myLine.length == myLength
//						&& info.ecoOpening() == myOpening)
//				{
//					add(info, myEco, ::Empty, myPosition);
//				}
//				else
				{
					Eco	otherKey		= Eco(info.ecoKey());
					bool	isReachable	= reachable.test(otherKey);

					Successors::Successor const* succ = 0;

					if (isReachable && otherKey != myKey)
						succ = findSuccessor(successors, otherKey);

					if (succ)
						add(info, succ->eco, succ->move, myPosition);
					else
						possiblyAdd(base, info, mode, Eco(), myPosition);
				}
			}
		}
		else
		{
			if (mode == tree::IncludeVariations && info.countVariations() > 0)
			{
				// nothing to do
			}
			// XXX reachableFunc should not use home pawns if info.idn() == 0
			else if (	method == tree::Exact
					&& info.idn()
							? reachableFunc(myPosition.signature(), info.signature(), hpSig)
							: Signature::isReachable(myPosition.signature(), info.signature()))
			{
				possiblyAdd(base, info, mode, Eco(), myPosition);
			}
		}
	}

	return true;
}


bool
Tree::buildTree960(	unsigned myIdn,
							Board const& startPosition,
							Board const& myPosition,
							Line const& myLine,
							uint16_t hpSig,
							Database const& base,
							tree::Method method,
							tree::Mode mode,
							ReachableFunc reachableFunc,
							util::Progress& progress,
							unsigned frequency,
							unsigned numGames)
{
	M_ASSERT(myIdn != 0);
	M_ASSERT(myIdn != variant::Standard);
	M_ASSERT(!myPosition.isStandardPosition(base.variant()));

	unsigned reportAfter = m_index + frequency;

	for ( ; m_index < numGames; ++m_index)
	{
		if (reportAfter == m_index)
		{
			progress.update(m_index);
			reportAfter += frequency;
		}

		if (progress.interrupted())
			return false;

		GameInfo const& info = base.gameInfo(m_index);

		if (mode == tree::IncludeVariations && info.countVariations() > 0)
		{
			possiblyAdd(base, info, mode, Eco(), myPosition);
		}
		else if (info.idn() == myIdn)
		{
			switch (myLine.length)
			{
				case 0:
					add(info, Eco(), info.plyCount() ? info.ply<0>() : ::Empty, myPosition);
					break;

				case 1:
					switch (info.plyCount())
					{
						case 0:
							break;

						case 1:
							if (myLine[0] == info.ply<0>())
								add(info, Eco(), ::Empty, myPosition);
							break;

						case 2:
							if (myLine[0] == info.ply<0>())
								add(info, Eco(), info.ply<1>(), myPosition);
							break;

						default:
							if (myLine[0] == info.ply<0>())
								add(info, Eco(), info.ply<1>(), myPosition);
							else if (reachableFunc(myPosition.signature(), info.signature(), hpSig))
								possiblyAdd(base, info, mode, Eco(), myPosition);
							break;
					}
					break;

				case 2:
					switch (info.plyCount())
					{
						case 0: break;
						case 1: break;

						case 2:
							if (myLine[0] == info.ply<0>() && myLine[1] == info.ply<1>())
							{
								add(info, Eco(), ::Empty, myPosition);
								break;
							}
							// fallthru

						default:
							if (reachableFunc(myPosition.signature(), info.signature(), hpSig))
								possiblyAdd(base, info, mode, Eco(), myPosition);
							break;
					}
					break;

				default:
					if (	info.plyCount() > 2
						&& reachableFunc(myPosition.signature(), info.signature(), hpSig))
					{
						possiblyAdd(base, info, mode, Eco(), myPosition);
					}
					break;
			}
		}
		else if (method == tree::Exact)
		{
			if (info.idn()
					? reachableFunc(myPosition.signature(), info.signature(), hpSig)
					: Signature::isReachable(myPosition.signature(), info.signature()))
			{
				possiblyAdd(base, info, mode, Eco(), myPosition);
			}
		}
	}

	return true;
}


bool
Tree::buildTreeStandard(unsigned myIdn,
								Board const& startPosition,
								Board const& myPosition,
								Line const& myLine,
								uint16_t hpSig,
								Database const& base,
								tree::Method method,
								tree::Mode mode,
								ReachableFunc reachableFunc,
								util::Progress& progress,
								unsigned frequency,
								unsigned numGames)
{
	M_ASSERT(myIdn == variant::Standard);
	M_ASSERT(myPosition.isStandardPosition(base.variant()));

	unsigned		reportAfter = m_index + frequency;
	Successors	successors;
	unsigned		myLength;	// unused

	Eco myEco = EcoTable::specimen(base.variant()).lookup(myLine, &myLength, &successors);

	for ( ; m_index < numGames; ++m_index)
	{
		if (reportAfter == m_index)
		{
			progress.update(m_index);
			reportAfter += frequency;
		}

		if (progress.interrupted())
			return false;

		GameInfo const& info = base.gameInfo(m_index);

		if (info.idn() == variant::Standard)
		{
			if (info.plyCount() == 0)
			{
				add(info, myEco, ::Empty, myPosition);
			}
			else
			{
				Successors::Successor const* succ = findSuccessor(successors, info.ecoKey());

				if (succ)	// should always be non-null
					add(info, succ->eco, succ->move, myPosition);
				else
					possiblyAdd(base, info, mode, Eco(), myPosition);
			}
		}
		else if (info.idn() == 0)
		{
			// NOTE: In Scid it is possible that a standard position
			// is declared as a non-standard position.
			possiblyAdd(base, info, mode, Eco(), myPosition);
		}
	}

	return true;
}


bool
Tree::buildTreeStart(unsigned myIdn,
							Board const& startPosition,
							Board const& myPosition,
							Line const& myLine,
							uint16_t hpSig,
							Database const& base,
							tree::Method method,
							tree::Mode mode,
							ReachableFunc reachableFunc,
							util::Progress& progress,
							unsigned frequency,
							unsigned numGames)
{
	M_ASSERT(myIdn != variant::Standard);
	M_ASSERT(myPosition.isStartPosition());

	unsigned reportAfter = m_index + mstl::max(frequency, 1000u);

	for ( ; m_index < numGames; ++m_index)
	{
		if (reportAfter == m_index)
		{
			progress.update(m_index);
			reportAfter += frequency;
		}

		if (progress.interrupted())
			return false;

		GameInfo const& info = base.gameInfo(m_index);

		static_assert(::Empty == 0, "reimplementation required");

		if (info.idn() == myIdn)
		{
			if (info.plyCount() == 0)
				add(info, Eco(), ::Empty, myPosition);
			else if (info.ply<0>() == 0)
				possiblyAdd(base, info, mode, Eco(), myPosition);
			else
				add(info, Eco(), info.ply<0>(), myPosition);
		}
		else if (info.idn() == 0)	// match is really possible?
		{
			possiblyAdd(base, info, mode, Eco(), myPosition);
		}
	}

	return true;
}


Tree*
Tree::makeTree(TreeP& tree,
					unsigned myIdn,
					Board startPosition,
					Board myPosition,
					Line myLine,
					uint16_t hpSig,
					Database& base,
					tree::Method method,
					tree::Mode mode,
					rating::Type ratingType,
					util::Progress& progress)
{
	M_REQUIRE(!format::isChessBaseFormat(base.format()));
	M_REQUIRE(base.variant() == variant::Normal);

	typedef bool (Tree::*BuildMeth)(	unsigned,
												Board const&,
												Board const&,
												Line const&,
												uint16_t,
												Database const&,
												tree::Method,
												tree::Mode,
												ReachableFunc,
												util::Progress&,
												unsigned,
												unsigned);

	BuildMeth buildMeth;

	if (myIdn == 0)
		buildMeth = &Tree::buildTree0;
	else if (myPosition.isStandardPosition(base.variant()))
		buildMeth = &Tree::buildTreeStandard;
	else if (myPosition.isStartPosition())
		buildMeth = &Tree::buildTreeStart;
	else if (myIdn == variant::Standard)
		buildMeth = &Tree::buildTree518;
	else
		buildMeth = &Tree::buildTree960;

	util::ProgressWatcher watcher(progress, base.countGames());

	unsigned frequency = progress.frequency(base.countGames());

	if (frequency == 0)
		frequency = mstl::min(1000u, mstl::max(base.countGames()/1000u, 1u));

	ReachableFunc reachableFunc;

	if (method == tree::Exact || base.format() != format::Scidb)
		reachableFunc = Signature::isReachablePosition;
	else
		reachableFunc = Signature::isReachable;

	::memset(::moveCache, 0, sizeof(::moveCache));

	if (tree)
	{
		// we have to rebuild the move cache
		for (unsigned i = 0; i < tree->m_infoList.size(); ++i)
		{
			TreeInfo& info = tree->m_infoList[i];
			::moveCache[::index(info.move())] = &info;
		}

		progress.update(tree->m_index);
		tree->m_prevGameCount = tree->m_filter.size();
	}
	else
	{
		tree.reset(new Tree);

		tree->m_key.set(method, mode, ratingType, myPosition.hash(), myPosition.exactZHPosition());
		tree->m_index = 0;
		tree->m_last = mstl::numeric_limits<unsigned>::max();
		tree->m_prevGameCount = 0;
		tree->m_complete = false;
		tree->m_base = &base;
		tree->m_variant = base.variant();

#ifdef SHOW_TREE_INFO
		tree->m_numGamesParsed = 0;
#endif
	}

	tree->m_filter.resize(base.countGames(), Filter::LeaveEmpty);

	if ((tree.get()->*buildMeth)(	myIdn,
											startPosition,
											myPosition,
											myLine,
											hpSig,
											base,
											method,
											mode,
											reachableFunc,
											progress,
											frequency,
											mstl::min(base.countGames(), tree->m_last)))
	{
		uint16_t buf[opening::Max_Line_Length];
		EcoTable const& ecoTable = EcoTable::specimen(base.variant());

		Line line(buf);
		line.copy(myLine);
		line.length++;

		for (unsigned i = 0; i < tree->m_infoList.size(); ++i)
		{
			TreeInfo& info = tree->m_infoList[i];

			if (info.move())
			{
				info.move().setColor(myPosition.sideToMove());
				myPosition.prepareForPrint(info.move(), base.variant(), Board::InternalRepresentation);
			}

			if (line.length <= opening::Max_Line_Length && !info.eco() && myIdn == variant::Standard)
			{
				buf[line.length - 1] = ::index(info.move());
				info.setEco(ecoTable.getEco(line));
			}

			tree->m_total.add(info, tree->m_key.ratingType());
		}

		tree->m_complete = true;

#ifdef SHOW_TREE_INFO
		fprintf(stderr, "games parsed: %u\n", tree->m_numGamesParsed);
#endif
	}

	return tree.release();
}


#ifdef VARIATIONS
Tree*
Tree::makeTree(TreeP& tree,
					Board myPosition,
					Line myLine,
					uint16_t hpSig,
					Database& base,
					tree::Method method,
					tree::Mode mode,
					rating::Type ratingType,
					util::Progress& progress)
{
	M_REQUIRE(base::format() == format::Scidb);
	M_REQUIRE(base.variant() == variant::Normal);

	typedef bool (Tree::*BuildMeth)(	unsigned,
												Board const&,
												Board const&,
												Line const&,
												uint16_t,
												Database const&,
												tree::Method,
												tree::Mode,
												ReachableFunc,
												util::Progress&,
												unsigned,
												unsigned);

	util::ProgressWatcher watcher(progress, base.countGames());

	unsigned frequency = progress.frequency(base.countGames());

	if (frequency == 0)
		frequency = mstl::min(1000u, mstl::max(base.countGames()/1000u, 1u));

	ReachableFunc reachableFunc;

	::memset(::moveCache, 0, sizeof(::moveCache));

	if (tree)
	{
		// we have to rebuild the move cache
		for (unsigned i = 0; i < tree->m_infoList.size(); ++i)
		{
			TreeInfo& info = tree->m_infoList[i];
			::moveCache[::index(info.move())] = &info;
		}

		progress.update(tree->m_index);
		tree->m_prevGameCount = tree->m_filter.size();
	}
	else
	{
		tree.reset(new Tree);

		tree->m_key.set(mode, ratingType, myPosition.hash(), myPosition.exactZHPosition());
		tree->m_index = 0;
		tree->m_last = mstl::numeric_limits<unsigned>::max();
		tree->m_prevGameCount = 0;
		tree->m_complete = false;
		tree->m_base = &base;
		tree->m_variant = base.variant();

#ifdef SHOW_TREE_INFO
		tree->m_numGamesParsed = 0;
#endif
	}

	tree->m_filter.resize(base.countGames(), Filter::LeaveEmpty);

	if ((tree.get()->*buildMeth)(	myIdn,
											startPosition,
											myPosition,
											myLine,
											hpSig,
											base,
											mode,
											reachableFunc,
											progress,
											frequency,
											mstl::min(base.countGames(), tree->m_last)))
	{
		uint16_t buf[opening::Max_Line_Length];
		EcoTable const& ecoTable = EcoTable::specimen(base.variant());

		Line line(buf);
		line.copy(myLine);
		line.length++;

		for (unsigned i = 0; i < tree->m_infoList.size(); ++i)
		{
			TreeInfo& info = tree->m_infoList[i];

			if (info.move())
			{
				info.move().setColor(myPosition.sideToMove());
				myPosition.prepareForPrint(info.move(), base.variant(), Board::InternalRepresentation);
			}

			if (line.length <= opening::Max_Line_Length && !info.eco() && myIdn == variant::Standard)
			{
				buf[line.length - 1] = ::index(info.move());
				info.setEco(ecoTable.getEco(line));
			}

			tree->m_total.add(info, tree->m_key.ratingType());
		}

		tree->m_complete = true;

#ifdef SHOW_TREE_INFO
		fprintf(stderr, "games parsed: %u\n", tree->m_numGamesParsed);
#endif
	}

	return tree.release();
}
#endif


void
Tree::setIncomplete()
{
	m_complete = false;
	m_last = mstl::numeric_limits<unsigned>::max();
}


void
Tree::setIncomplete(unsigned firstIndex, unsigned lastIndex)
{
	M_REQUIRE(firstIndex < lastIndex);

	if (m_complete)
	{
		m_index = firstIndex;
		m_last = lastIndex;
		m_complete = false;
	}
	else if (firstIndex < m_index)
	{
		m_index = firstIndex;
	}

	if (m_last < lastIndex)
		m_last = lastIndex;
}


bool
Tree::isTreeFor(Database const& base, Key const& key) const
{
	return m_complete && m_base->id() == base.id() && m_key == key;
}


bool
Tree::isTreeFor(Database const& base, Board const& position) const
{
	return	m_complete
			&& m_base->id() == base.id()
			&& m_key.hash() == position.hash()
			&& m_key.position() == position.exactZHPosition();
}


bool
Tree::isTreeFor(	Database const& base,
						Board const& position,
						tree::Method method,
						tree::Mode mode,
						rating::Type ratingType) const
{
	return	m_complete
			&& m_base->id() == base.id()
			&& m_key.match(method, mode, ratingType, position.hash(), position.exactZHPosition());
}


void
Tree::sort(attribute::tree::ID column)
{
	if (m_infoList.size() <= 1)
		return;

	for (unsigned k = 0, n = m_infoList.size() - 1; k < n; ++k)
	{
		unsigned index = k;

		TreeInfo* info = &m_infoList[index];

		for (unsigned i = k + 1; i <= n; ++i)
		{
			if (m_infoList[i].isLessThan(*info, m_key.ratingType(), column))
				info = &m_infoList[index = i];
		}

		if (index > k)
			mstl::swap(m_infoList[k], m_infoList[index]);
	}
}

// vi:set ts=3 sw=3:
