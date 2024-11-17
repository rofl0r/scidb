// ======================================================================
// Author : $Author$
// Version: $Revision: 1383 $
// Date   : $Date: 2017-08-06 17:18:29 +0000 (Sun, 06 Aug 2017) $
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
// Copyright: (C) 2009-2017 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "app_view.h"
#include "app_cursor.h"
#include "app_application.h"

#include "db_database.h"
#include "db_filter.h"
#include "db_game.h"
#include "db_pgn_writer.h"
#include "db_latex_writer.h"
#include "db_log.h"
#include "db_exception.h"

#include "sci_codec.h"
#include "sci_consumer.h"
#include "sci_encoder.h"

#include "si3_codec.h"
#include "si3_consumer.h"

#include "T_Controller.h"

#include "u_misc.h"
#include "u_zstream.h"
#include "u_progress.h"

#include "sys_utf8_codec.h"
#include "sys_file.h"
#include "sys_info.h"

#include "m_vector.h"
#include "m_assert.h"

#include <string.h>

using namespace db;
using namespace app;


enum { MaxRandom = 2097152 };


inline
static Filter::ResizeMode
map(View::UpdateMode mode)
{
	return mode == View::AddNewGames ? Filter::AddNewIndices : Filter::LeaveEmpty;
}


namespace {

struct WriteGuard
{
	void release() { m_app.setIsWriting(); }

	WriteGuard(Application& app, Database const& base)
		:m_app(app)
	{
		if (!base.isMemoryOnly())
			m_app.setIsWriting(base.name());
	}

	WriteGuard(Application& app, mstl::string const& name) :m_app(app) { m_app.setIsWriting(name); }

	~WriteGuard() { release(); }

	Application& m_app;
};

} // namespace


View::View(Application& app, Cursor& cursor)
	:m_app(app)
	,m_cursor(cursor)
{
	initialize();

	for (unsigned i = 0; i < table::LAST; ++i)
		m_updateMode[i] = NotNeeded;

	m_updateMode[table::Games  ] = AddNewGames;
	m_updateMode[table::Players] = AddNewGames;
	m_updateMode[table::Events ] = AddNewGames;
}


View::View(View& view)
	:m_app(view.m_app)
	,m_cursor(view.m_cursor)
{
	::memcpy(m_updateMode, view.m_updateMode, sizeof(m_updateMode));

	for (unsigned i = 0; i < table::LAST; ++i)
	{
		m_filter[i].swap(view.m_filter[i]);
		m_selector[i].swap(view.m_selector[i]);
	}
}


View::View(Application& app, Cursor& cursor, UpdateModeList const& updateMode)
	:m_app(app)
	,m_cursor(cursor)
{
	::memcpy(m_updateMode, updateMode, sizeof(updateMode));
	initialize();
}


Database const&
View::database() const
{
	return *m_cursor.m_db;
}


Database&
View::database()
{
	return *m_cursor.m_db;
}


void
View::initialize()
{
	for (unsigned i = 0; i < table::LAST; ++i)
	{
		switch (i)
		{
			case table::Annotators:
			case table::Positions:
				break; // nothing to do

			default:
				m_filter[i].resize(m_cursor.m_db->count(table::Type(i)), Filter::LeaveEmpty);
				m_filter[i].set();
				break;
		}
	}
}


void
View::update()
{
	for (unsigned i = 0; i < table::LAST; ++i)
	{
		if (m_updateMode[i] != NotNeeded)
		{
			switch (i)
			{
				case table::Annotators:
				case table::Positions:
					break; // nothing to do

				default:
					m_filter[i].resize(m_cursor.m_db->count(table::Type(i)), ::map(m_updateMode[i]));
					m_selector[i].update(m_cursor.m_db->count(table::Type(i)));
					break;
			}
		}
	}
}


unsigned
View::count(table::Type type) const
{
	M_REQUIRE(isUsed(type));

	switch (type)
	{
		case table::Annotators:			return m_cursor.m_db->countAnnotators();
		case table::Positions:			return m_cursor.m_db->countPositions();
		default:								return m_filter[type].count();
	}

	return 0; // never reached
}


unsigned
View::total(table::Type type) const
{
	M_REQUIRE(isUsed(type));

	switch (type)
	{
		case table::Annotators:			return m_cursor.m_db->countAnnotators();
		case table::Positions:			return m_cursor.m_db->countPositions();
		default:								return m_filter[type].size();
	}

	return 0; // never reached
}


unsigned
View::index(table::Type type, unsigned index) const
{
	M_REQUIRE(isUsed(type));
	M_REQUIRE(index < count(type));

	return m_selector[type].lookup(index);
}


int
View::lookupGameIndex(unsigned number) const
{
	M_REQUIRE(isUsed(table::Games));
	M_REQUIRE(number < total(table::Games));

	return m_filter[table::Games].contains(number) ? int(m_selector[table::Games].find(number)) : -1;
}


int
View::lookupPlayer(mstl::string const& name) const
{
	M_REQUIRE(isUsed(table::Players));
	return m_selector[table::Players].findPlayer(*m_cursor.m_db, name);
}


int
View::lookupPlayerIndex(unsigned number) const
{
	M_REQUIRE(isUsed(table::Players));
	return m_filter[table::Players].contains(number) ? int(m_selector[table::Players].find(number)) : -1;
}


int
View::lookupEvent(mstl::string const& name) const
{
	M_REQUIRE(isUsed(table::Events));
	return m_selector[table::Events].findEvent(*m_cursor.m_db, name);
}


int
View::lookupEventIndex(unsigned number) const
{
	M_REQUIRE(isUsed(table::Events));
	return m_filter[table::Events].contains(number) ? int(m_selector[table::Events].find(number)) : -1;
}


int
View::lookupSite(mstl::string const& name) const
{
	M_REQUIRE(isUsed(table::Sites));
	return m_selector[table::Sites].findSite(*m_cursor.m_db, name);
}


int
View::lookupSiteIndex(unsigned number) const
{
	M_REQUIRE(isUsed(table::Sites));
	return m_filter[table::Sites].contains(number) ? int(m_selector[table::Sites].find(number)) : -1;
}


int
View::lookupPositionIndex(unsigned number) const
{
	M_REQUIRE(isUsed(table::Positions));
	return m_selector[table::Positions].find(number);
}


int
View::lookupAnnotator(mstl::string const& name) const
{
	M_REQUIRE(isUsed(table::Annotators));
	return m_selector[table::Annotators].findAnnotator(*m_cursor.m_db, name);
}


int
View::lookupPosition(uint16_t idn) const
{
	M_REQUIRE(isUsed(table::Positions));
	return m_selector[table::Positions].findPosition(*m_cursor.m_db, idn);
}


int
View::findPlayer(util::Pattern const& pattern, unsigned startIndex) const
{
	M_REQUIRE(isUsed(table::Players));
	return m_selector[table::Players].searchPlayer(*m_cursor.m_db, pattern, startIndex);
}


int
View::findEvent(util::Pattern const& pattern, unsigned startIndex) const
{
	M_REQUIRE(isUsed(table::Events));
	return m_selector[table::Events].searchEvent(*m_cursor.m_db, pattern, startIndex);
}


int
View::findSite(util::Pattern const& pattern, unsigned startIndex) const
{
	M_REQUIRE(isUsed(table::Sites));
	return m_selector[table::Sites].searchSite(*m_cursor.m_db, pattern, startIndex);
}


int
View::findAnnotator(util::Pattern const& pattern, unsigned startIndex) const
{
	M_REQUIRE(isUsed(table::Annotators));
	return m_selector[table::Annotators].searchAnnotator(*m_cursor.m_db, pattern, startIndex);
}


void
View::sort(attribute::game::ID attr, order::ID order, rating::Type ratingType)
{
	M_REQUIRE(isUsed(table::Games));
	m_selector[table::Games].sort(*m_cursor.m_db, attr, order, ratingType);
}


void
View::sort(attribute::player::ID attr, order::ID order, rating::Type ratingType)
{
	M_REQUIRE(isUsed(table::Players));
	m_selector[table::Players].sort(*m_cursor.m_db, attr, order, ratingType);
}


void
View::sort(attribute::event::ID attr, order::ID order)
{
	M_REQUIRE(isUsed(table::Events));
	m_selector[table::Events].sort(*m_cursor.m_db, attr, order);
}


void
View::sort(attribute::site::ID attr, order::ID order)
{
	M_REQUIRE(isUsed(table::Sites));
	m_selector[table::Sites].sort(*m_cursor.m_db, attr, order);
}


void
View::sort(attribute::annotator::ID attr, order::ID order)
{
	M_REQUIRE(isUsed(table::Annotators));
	m_selector[table::Annotators].sort(*m_cursor.m_db, attr, order);
}


void
View::sort(attribute::position::ID attr, order::ID order)
{
	M_REQUIRE(isUsed(table::Positions));
	m_selector[table::Positions].sort(*m_cursor.m_db, attr, order);
}


void
View::reverseOrder(table::Type type)
{
	M_REQUIRE(isUsed(type));
	m_selector[type].reverse(*m_cursor.m_db);
}


void
View::resetOrder(table::Type type)
{
	M_REQUIRE(isUsed(type));
	m_selector[type].reset(*m_cursor.m_db);
}


void
View::updateSelector(table::Type type)
{
	M_REQUIRE(isUsed(type));

	switch (type)
	{
		case table::Annotators:
		case table::Positions:
			m_selector[type].update();
			break;

		default:
			m_selector[type].update(m_filter[type]);
			break;
	}
}


void
View::searchGames(Query const& query)
{
	M_REQUIRE(isUsed(table::Games));

	m_filter[table::Games].search(query, m_cursor.m_db->content());
	m_selector[table::Games].update(m_filter[table::Games]);
}


void
View::filterOnGames(table::Type type)
{
	M_REQUIRE(isUsed(type));

	switch (type)
	{
		case table::Players:
		{
			mstl::bitset f(m_cursor.m_db->namebase(Namebase::Player).nextId());
			{
				Filter& filter = m_filter[table::Games];

				for (int i = filter.next(); i != Filter::Invalid; i = filter.next(i))
				{
					f.set(m_cursor.m_db->gameInfo(i).playerEntry(color::White)->id());
					f.set(m_cursor.m_db->gameInfo(i).playerEntry(color::Black)->id());
				}
			}
			{
				Filter& filter = m_filter[table::Players];

				filter.reset();

				for (unsigned i = 0; i < m_cursor.m_db->countPlayers(); ++i)
				{
					if (f.test(m_cursor.m_db->player(i).id()))
						filter.add(i);
				}
			}
			break;
		}

		case table::Events:
		{
			mstl::bitset f(m_cursor.m_db->namebase(Namebase::Event).nextId());
			{
				Filter& filter = m_filter[table::Games];

				for (int i = filter.next(); i != Filter::Invalid; i = filter.next(i))
					f.set(m_cursor.m_db->gameInfo(i).eventEntry()->id());
			}
			{
				Filter& filter = m_filter[table::Events];

				filter.reset();

				for (unsigned i = 0; i < m_cursor.m_db->countEvents(); ++i)
				{
					if (f.test(m_cursor.m_db->event(i).id()))
						filter.add(i);
				}
			}
			break;
		}

		case table::Sites:
		{
			mstl::bitset f(m_cursor.m_db->namebase(Namebase::Site).nextId());
			{
				Filter& filter = m_filter[table::Games];

				for (int i = filter.next(); i != Filter::Invalid; i = filter.next(i))
					f.set(m_cursor.m_db->gameInfo(i).eventEntry()->site()->id());
			}
			{
				Filter& filter = m_filter[table::Sites];

				filter.reset();

				for (unsigned i = 0; i < m_cursor.m_db->countSites(); ++i)
				{
					if (f.test(m_cursor.m_db->site(i).id()))
						filter.add(i);
				}
			}
			break;
		}

		case table::Games:
			return;

		case table::Annotators:
		{
			mstl::bitset f(m_cursor.m_db->namebase(Namebase::Annotator).nextId());
			{
				Filter& filter = m_filter[table::Games];

				for (int i = filter.next(); i != Filter::Invalid; i = filter.next(i))
				{
					M_REQUIRE(m_cursor.m_db->gameInfo(i).annotatorEntry());
					f.set(m_cursor.m_db->gameInfo(i).annotatorEntry()->id());
				}
			}
			{
				Filter& filter = m_filter[table::Annotators];

				filter.reset();

				for (unsigned i = 0; i < m_cursor.m_db->countAnnotators(); ++i)
				{
					if (f.test(m_cursor.m_db->annotator(i).id()))
						filter.add(i);
				}
			}
			break;
		}

		case table::Positions:
		{
			Filter& filter	= m_filter[table::Positions];
			Filter& games	= m_filter[table::Games];

			filter.reset();

			for (int i = games.next(); i != Filter::Invalid; i = games.next(i))
				filter.add(m_cursor.m_db->gameInfo(i).idn());

			break;
		}
	}
}


void
View::setGameFilter(Filter const& filter)
{
	M_REQUIRE(isUsed(table::Games));
	M_REQUIRE(filter.size() == m_cursor.m_db->countGames());

	m_filter[table::Games] = filter;
	m_selector[table::Games].update(m_filter[table::Games]);
}


TournamentTable*
View::makeTournamentTable() const
{
	M_REQUIRE(isUsed(table::Games));
	return m_cursor.m_db->makeTournamentTable(m_filter[table::Games]);
}


unsigned
View::stripMoveInformation(unsigned types, util::Progress& progress)
{
	M_REQUIRE(isUsed(table::Games));
	return m_cursor.m_db->stripMoveInformation(m_filter[table::Games], types, progress);
}


unsigned
View::stripTags(TagMap const& tags, util::Progress& progress)
{
	M_REQUIRE(isUsed(table::Games));
	return m_cursor.m_db->stripTags(m_filter[table::Games], tags, progress);
}


void
View::findTags(TagMap& tags, util::Progress& progress) const
{
	M_REQUIRE(isUsed(table::Games));
	m_cursor.m_db->findTags(m_filter[table::Games], tags, progress);
}


View::Result
View::dumpGame(unsigned index, mstl::string const& fen, mstl::string& result) const
{
	M_REQUIRE(isUsed(table::Games));

	Game game;

	load::State state = m_cursor.m_db->loadGame(this->index(table::Games, index), game);

	if (state != load::Ok)
		return Result(state, 0);

	if (!fen.empty())
		game.goToPosition(fen);

	// NOTE: we like to use flag UseZeroWidthSpace, but Tk cannot handle this character.
	return Result(state, game.dumpMoves(result, Game::SuppressSpace | Game::WhiteNumbers));
}


View::Result
View::dumpGame(unsigned index,
					unsigned split,
					mstl::string const& fen,
					StringList& result,
					StringList& positions) const
{
	M_REQUIRE(isUsed(table::Games));

	typedef mstl::vector<unsigned> LengthList;

	Game game;
	mstl::string encoding;

	load::State state = m_cursor.m_db->loadGame(this->index(table::Games, index), game);

	if (state != load::Ok)
		return Result(state, 0);

	if (!fen.empty())
		game.goToPosition(fen);

	unsigned		moves = game.countHalfMoves();
	unsigned		count	= 0;
	LengthList	lengths;

	if (moves == 0)
	{
		lengths.push_back(0);
		result.resize(1);
	}
	else if (!fen.empty() || game.startBoard().isStandardPosition(game.variant()))
	{
		unsigned	size	= mstl::min(moves, split);
		double	delta	= double(moves)/double(size);

		result.resize(size);
		lengths.resize(size);

		for (unsigned i = 0; i < size; ++i)
		{
			// NOTE: we like to use flag UseZeroWidthSpace, but Tk cannot handle this character.
			count += (lengths[i] = game.dumpMoves(
												result[i],
												unsigned((i + 1)*delta + 0.5) - count,
												Game::SuppressSpace | Game::WhiteNumbers));
		}
	}
	else
	{
		unsigned	size	= mstl::min(moves + 1, split);
		double	delta	= double(moves)/double(mstl::min(moves, split - 1));

		result.resize(size);
		lengths.resize(size);

		lengths[0] = 0;

		for (unsigned i = 1; i < size; ++i)
		{
			count += (lengths[i] = game.dumpMoves(
												result[i],
												unsigned(i*delta + 0.5) - count,
												Game::SuppressSpace | Game::WhiteNumbers));
		}
	}

	if (fen.empty())
		game.moveToMainlineStart();
	else
		game.goToPosition(fen);

	for (unsigned i = 0; i < lengths.size(); ++i)
	{
		game.forward(lengths[i]);
		positions.push_back();
		game.currentBoard().toFen(positions.back(), game.variant());
	}

	return Result(state, count);
}


unsigned
View::copyGames(	Cursor& destination,
						TagBits const& allowedTags,
						bool allowExtraTags,
						unsigned* illegalRejected,
						Log& log,
						util::Progress& progress)
{
	M_REQUIRE(isUsed(table::Games));

	progress.message("copy-game");

	WriteGuard guard(m_app, destination.database());

	unsigned count = m_cursor.m_db->copyGames(destination.base(),
															m_filter[table::Games],
															m_selector[table::Games],
															allowedTags,
															allowExtraTags,
															illegalRejected,
															log,
															progress);

	guard.release();
	m_app.startUpdateTree(destination);
	return count;
}


unsigned
View::exportGames(Database& destination,
						unsigned* illegalRejected,
						Log& log,
						util::Progress& progress) const
{
	M_REQUIRE(isUsed(table::Games));

	return m_cursor.m_db->exportGames(	destination,
													m_filter[table::Games],
													m_selector[table::Games],
													illegalRejected,
													log,
													progress);
}


unsigned
View::exportGames(Consumer& destination,
						unsigned* illegalRejected,
						Log& log,
						util::Progress& progress) const
{
	M_REQUIRE(isUsed(table::Games));

	destination.setupVariant(m_cursor.m_db->variant());

	return m_cursor.m_db->exportGames(	destination,
													m_filter[table::Games],
													m_selector[table::Games],
													illegalRejected,
													log,
													progress);
}


unsigned
View::exportGames(mstl::string const& filename,
						mstl::string const& encoding,
						mstl::string const& description,
						uint32_t creationTime,
						type::ID type,
						unsigned flags,
						TagBits const& allowedTags,
						bool allowExtraTags,
						Languages const* languages,
						unsigned significantLanguages,
						unsigned* illegalRejected,
						Log& log,
						util::Progress& progress,
						FileMode fmode) const
{
	M_REQUIRE(isUsed(table::Games));
	M_REQUIRE(!application().contains(filename));

	if (m_cursor.m_db->size() == 0)
		return 0;

	mstl::string	ext	= util::misc::file::suffix(filename);
	unsigned			count	= 0;

	ext.tolower();

	if (ext == "sci")
	{
		Database destination(filename,
									sys::utf8::Codec::utf8(),
									storage::OnDisk,
									m_cursor.m_db->variant(),
									type);
		destination.setupDescription(description, creationTime);
		progress.message("write-game");

		WriteGuard guard(m_app, destination);

		if (	m_cursor.m_db->format() == format::Scidb
			&& !languages
			&& fmode != Upgrade
			&& allowExtraTags
			&& (allowedTags | sci::Encoder::infoTags()).any())
		{
			count = m_cursor.m_db->copyGames(destination,
														m_filter[table::Games],
														m_selector[table::Games],
														allowedTags,
														allowExtraTags,
														illegalRejected,
														log,
														progress);
		}
		else
		{
			sci::Consumer::Codecs codecs(&dynamic_cast<sci::Codec&>(destination.codec()));
			sci::Consumer consumer(	m_cursor.m_db->format(),
											codecs,
											allowedTags,
											allowExtraTags,
											languages,
											significantLanguages);
			count = exportGames(consumer, illegalRejected, log, progress);
		}

		progress.message("write-index");
		destination.save(progress);
		destination.close();
	}
	else if (ext == "si3" || ext == "si4")
	{
		// NOTE: we cannot use storage class Temporary because si3/si4 is using
		// a shadow namebase (index file must be written after namebase is written).
		Database destination(filename,
									sys::utf8::Codec::utf8(),
									storage::OnDisk,
									m_cursor.m_db->variant(),
									type);
		destination.setupDescription(description, creationTime);

		WriteGuard guard(m_app, destination);

		if (m_cursor.m_db->variant() == variant::Normal)
		{
//			We do not use speed up because the scid bases created with the Scid application
//			may contain some broken data. The exporting will fix the data.
//			if (	m_cursor.m_db->format() == format::Scid
//				&& !languages
//				&& (ext == "si4" || dynamic_cast<si3::Codec&>(m_cursor.m_db->codec()).isFormat3()))
//			{
//				count = exportGames(destination, illegalRejected, log, progress, "write-game");
//			}
//			else
			{
				format::Type format(ext == "si3" ? format::Scid3 : format::Scid4);
				si3::Consumer consumer(	format,
												dynamic_cast<si3::Codec&>(destination.codec()),
												encoding,
												allowedTags,
												allowExtraTags,
												languages,
												significantLanguages);
				progress.message("write-game");
				count = exportGames(consumer, illegalRejected, log, progress);
			}
		}

		progress.message("write-index");
		destination.save(progress);
		destination.close();
	}
	else if (ext == "pgn" || ext == "gz" || ext == "zip")
	{
		util::ZStream::Type type;

		if (ext == "gz")			type = util::ZStream::GZip;
		else if (ext == "zip")	type = util::ZStream::Zip;
		else							type = util::ZStream::Text;

		if (flags & (PgnWriter::Flag_Use_Scidb_Import_Format | PgnWriter::Flag_Use_ChessBase_Format))
			flags |= PgnWriter::Flag_Use_UTF8;

		mstl::ios_base::openmode mode = mstl::ios_base::out;
		mstl::string internalName(sys::file::internalName(filename));

		if (fmode == Append)
		{
			mode |= mstl::ios_base::app;

			if (type != util::ZStream::Zip)
			{
				flags |= PgnWriter::Flag_Append_Games;

				if (util::ZStream::testByteOrderMark(internalName))
					flags |= PgnWriter::Flag_Use_UTF8;
				else
					flags &= ~PgnWriter::Flag_Use_UTF8;
			}
		}
		else
		{
			mode |= mstl::ios_base::trunc;
		}

		mstl::string useEncoding;

		if (flags & PgnWriter::Flag_Use_UTF8)
			useEncoding = sys::utf8::Codec::utf8();
		else if (encoding == sys::utf8::Codec::utf8())
			useEncoding = sys::utf8::Codec::latin1();
		else
			useEncoding = encoding;

		PgnWriter::LineEnding lineEnding = PgnWriter::Unix;

		if (util::ZStream::isWindowsLineEnding(internalName))
			lineEnding = PgnWriter::Windows;

		util::ZStream strm(internalName, type, mode);
		strm.exceptions(mstl::ios_base::badbit | mstl::ios_base::failbit);

		try
		{
			PgnWriter writer(format::Pgn, strm, useEncoding, lineEnding, flags);
			progress.message("write-game");
			WriteGuard guard(m_app, filename);
			count = exportGames(writer, illegalRejected, log, progress);
		}
		catch (mstl::ios_base::failure const& exc)
		{
			static uint32_t const MaxFileSize = (uint32_t(2) << 31) - 1;
			IOException::ErrorType error = IOException::Write_Failed;
			if (unsigned(sys::file::size(filename)) == MaxFileSize)
				error = IOException::Max_File_Size_Exceeded;
			M_THROW(IOException(IOException::PgnFile, error, "write failed"));
		}
	}
	else
	{
		M_RAISE("unsupported extension: %s", ext.c_str());
	}

	return count;
}


unsigned
View::printGames(	TeXt::Environment& environment,
						format::Type format,
						unsigned flags,
						unsigned options,
						NagMap const& nagMap,
						Languages const* languages,
						unsigned significantLanguages,
						unsigned* illegalRejected,
						Log& log,
						util::Progress& progress) const
{
	M_REQUIRE(isUsed(table::Games));

	unsigned count = 0;

	switch (int(format))
	{
		case format::LaTeX:
			{
				LaTeXWriter	writer(	database().format(),
											flags,
											options,
											nagMap,
											languages,
											significantLanguages,
											environment);

				progress.message("print-game");
				count = exportGames(writer, illegalRejected, log, progress);
			}
			break;

		default:
			M_RAISE("unsupported format");
	}

	return count;
}


int
View::nextIndex(db::table::Type type, unsigned index) const
{
	M_REQUIRE(isUsed(type));

	if (index >= m_filter[type].size())
		return -1;

	int i = lookupGameIndex(index);

	if (i == -1 || i + 1 == int(m_filter[type].count()))
		return -1;

	return m_selector[type].lookup(i + 1);
}


int
View::prevIndex(db::table::Type type, unsigned index) const
{
	M_REQUIRE(isUsed(type));

	if (index >= m_filter[type].size())
		return -1;

	int i = lookupGameIndex(index);

	if (i <= 0)
		return -1;

	return m_selector[type].lookup(i - 1);
}


int
View::firstIndex(db::table::Type type) const
{
	M_REQUIRE(isUsed(type));

	if (m_filter[type].isEmpty())
		return -1;

	return m_selector[type].lookup(0);
}


int
View::lastIndex(db::table::Type type) const
{
	M_REQUIRE(isUsed(type));

	unsigned count = m_filter[type].count();

	if (count == 0)
		return -1;

	return m_selector[type].lookup(count - 1);
}


int
View::randomGameIndex() const
{
	M_REQUIRE(isUsed(table::Games));

	unsigned count = m_filter[table::Games].count();

	if (count == 0)
		return -1;

	if (count == 1)
		return 0;

	unsigned index;

	if (count > MaxRandom)
	{
		index = m_app.rand32(count);
	}
	else
	{
		if (m_used.size() != count)
			m_used.resize(count, true);

		unsigned free = m_used.count();

		if (free == 0)
		{
			m_used.set();
			index = m_app.rand32(count);
		}
		else
		{
			index = m_used.index(m_app.rand32(free));
		}

		m_used.reset(index);
	}

	return m_filter[table::Games].toIndex(index);
}


void
View::add(db::table::Type type, unsigned index)
{
	M_REQUIRE(isUsed(type));
	M_REQUIRE(index < total(type));

	m_filter[type].add(index);
	m_selector[type].update(m_filter[type]);
}

// vi:set ts=3 sw=3:
