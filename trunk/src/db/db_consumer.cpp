// ======================================================================
// Author : $Author$
// Version: $Revision: 1500 $
// Date   : $Date: 2018-07-13 10:00:25 +0000 (Fri, 13 Jul 2018) $
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
// Copyright: (C) 2009-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "db_consumer.h"
#include "db_tag_set.h"
#include "db_annotation.h"
#include "db_mark_set.h"
#include "db_move_info_set.h"
#include "db_game_info.h"
#include "db_comment.h"
#include "db_producer.h"

#include "sys_utf8_codec.h"

#include "m_assert.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

using namespace db;


Consumer::Consumer(	format::Type srcFormat,
							mstl::string const& encoding,
							TagBits const& allowedTags,
							bool allowExtraTags,
							LanguageList const* languages,
							unsigned significantLanguages)
	:Provider(srcFormat)
	,m_allowedTags(allowedTags)
	,m_allowExtraTags(allowExtraTags)
	,m_stack(1)
	,m_variationCount(0)
	,m_commentCount(0)
	,m_annotationCount(0)
	,m_moveInfoCount(0)
	,m_markCount(0)
	,m_langFlags(0)
	,m_terminated(false)
	,m_mainVariant(variant::Normal)
	,m_variant(variant::Normal)
	,m_useVariant(variant::Normal)
	,m_idn(0)
	,m_gameFlags(0)
	,m_updateFlags(0)
	,m_line(m_moveBuffer)
	,m_encoding(encoding)
	,m_codec(new sys::utf8::Codec(encoding))
	,m_consumer(0)
	,m_producer(0)
	,m_setupBoard(true)
	,m_haveUsedLangs(false)
	,m_noComments(languages && languages->empty())
	,m_significantLangs(significantLanguages)
{
	M_REQUIRE(!languages || significantLanguages <= languages->size());

	if (languages)
		m_wantedLanguages = *languages;
}


Consumer::~Consumer() throw()
{
	delete m_codec;
}


void
Consumer::variantHasChanged(variant::Type)
{
	// no action
}


bool
Consumer::supportsVariant(variant::Type) const
{
	return true;
}


void
Consumer::setVariant(variant::Type variant)
{
	M_REQUIRE(variant == variant::Undetermined || supportsVariant(variant));

	m_useVariant = m_variant = variant;

	if (m_producer && m_producer->variant() == variant::Undetermined)
	{
		m_producer->setVariant(
			variant == variant::Undetermined ? variant : variant::toMainVariant(variant));
	}

	variantHasChanged(variant);
}


void
Consumer::setUsedLanguages(LanguageSet languages)
{
	if (m_noComments)
		return;

	m_usedLanguages.swap(languages);
	m_haveUsedLangs = true;

	if (m_wantedLanguages.empty())
		return; // we want it all

	m_langFlags = 0;

	LanguageSet::const_iterator e = m_usedLanguages.end();
	unsigned i;

	for (i = 0; i < m_significantLangs; ++i)
	{
		mstl::string const& lang = m_wantedLanguages[i];

		if (m_usedLanguages.find(lang) != e)
		{
			m_relevantLangs[lang] = 0;
			m_langFlags |= (lang[0] == 'e' && lang[1] == 'n') ? i18n::English : i18n::Other_Lang;
		}
	}

	if (m_relevantLangs.empty())
	{
		while (i < m_wantedLanguages.empty() && m_usedLanguages.find(m_wantedLanguages[i]) == e)
			++i;

		if (i < m_wantedLanguages.empty())
		{
			mstl::string const& lang = m_wantedLanguages[i];
			m_relevantLangs[lang] = 0;
			m_langFlags |= (lang[0] == 'e' && lang[1] == 'n') ? i18n::English : i18n::Other_Lang;
		}
	}

	if (m_relevantLangs.size() > 1)
		m_langFlags |= i18n::Multilingual;
}


Board const&
Consumer::getFinalBoard() const
{
	return board();
}


Board const&
Consumer::getStartBoard() const
{
	return startBoard();
}


unsigned
Consumer::plyCount() const
{
	return m_stack.top().board.plyNumber() - m_stack.bottom().board.plyNumber();
}


variant::Type
Consumer::variant() const
{
	return m_variant;
}


uint16_t
Consumer::idn() const
{
	return m_idn;
}


void
Consumer::setGameFlags(uint32_t flags)
{
	m_updateFlags = flags & ~(GameInfo::Flag_Dirty | GameInfo::Flag_Changed);
}


void
Consumer::setup(Board const& startPosition)
{
	m_stack.bottom().board = startPosition;
}


void
Consumer::setup(unsigned idn)
{
	m_stack.bottom().board.setup(idn, m_useVariant);
}


void
Consumer::setup(mstl::string const& fen)
{
	// XXX possibly we should allow:
	// 1. handicap games
	// 2. illegal positions (king in check)
	M_ASSERT(Board::isValidFen(fen, m_useVariant));

	m_stack.bottom().board.setup(fen, m_useVariant);
}


void
Consumer::swapMoveInfo(MoveInfoSet& moveInfo)
{
	moveInfo.swap(m_moveInfoSet);
}


bool
Consumer::startGame(TagSet const& tags, Board const* board, uint16_t* idn)
{
	M_ASSERT(finalized());

	m_variationCount = 0;
	m_commentCount = 0;
	m_annotationCount = 0;
	m_moveInfoCount = 0;
	m_markCount = 0;
	m_terminated = false;
	m_line.length = 0;
	m_gameFlags = m_updateFlags;
	m_variant = m_mainVariant;
	m_useVariant = m_mainVariant;
	m_moveInfoSet.clear();
	m_engines.clear();
	m_homePawns.clear();
	m_sendTimeTable.clear();
	m_updateFlags = 0;

	if (board)
	{
		if (board->notDerivableFromChess960())
			return false;

		m_stack.bottom().board = *board;
		m_idn = startBoard().computeIdn(m_useVariant);
	}
	else if (tags.contains(tag::Fen))
	{
		setup(tags.value(tag::Fen));

		if (startBoard().notDerivableFromChess960())
			return false;

		m_idn = startBoard().computeIdn(m_useVariant);
	}
	else if (idn)
	{
		M_ASSERT(*idn > 0);
		setup(m_idn = *idn);
	}
	else if (m_setupBoard)
	{
		// In this case we have to use Crazyhouse setup,
		// because we don't know yet whether its's standard
		// chess or Crazyhouse. The hash code of the board
		// doesn't matter here.
		setup(Board::standardBoard(variant::Crazyhouse));
		m_idn = variant::Standard;
	}

	m_stack.dup();
	m_stack.top().empty = true;
	m_stack.top().move.clear();

	return beginGame(tags);
}


void
Consumer::finalizeGame()
{
	while (m_stack.size() > 1)
		m_stack.pop();
	m_setupBoard = true;
	m_haveUsedLangs = false;
	m_langFlags = i18n::None;
	m_usedLanguages.clear();
	m_relevantLangs.clear();
}


save::State
Consumer::finishGame(TagSet const& tags)
{
	M_REQUIRE(variationLevel() == 0);

	if (startBoard().isStartPosition())
		m_stack.top().board.signature().setHomePawns(m_homePawns.used(), m_homePawns.data());
	else
		m_stack.top().board.signature().setHomePawns(0, hp::Pawns());

	return endGame(tags);
}


save::State
Consumer::skipGame(TagSet const& tags)
{
	return finishGame(tags);
}


void
Consumer::finishMoveSection(result::ID result)
{
	if (m_terminated)
	{
		while (variationLevel() > 0)
		{
			endVariation(m_stack.top().move.isEmpty());
			m_stack.pop();
		}
	}

	endMoveSection(result);

	// we don't like to have null moves in the opening line
	unsigned i = 0;
	while (i < m_line.length && m_line[i])
		++i;
	m_line.length = i;
}


Comment const&
Consumer::prepareComment(Comment& dst, Comment const& src)
{
	M_ASSERT(!src.isEmpty());
	M_ASSERT(dst.isEmpty());

	if (!m_noComments)
	{
		if (m_wantedLanguages.empty() || !m_haveUsedLangs)
			dst = src;
		else if (!m_relevantLangs.empty())
			(dst = src).strip(m_relevantLangs);
	}

	return dst;
}


Comment const&
Consumer::preparePhrase(Comment& dst, Comment const& src)
{
	M_ASSERT(!src.isEmpty());
	M_ASSERT(dst.isEmpty());
	M_ASSERT(src.langFlags() & i18n::English);

	if (!m_noComments)
	{
		if (m_wantedLanguages.empty() || src.containsAnyLanguageOf(m_relevantLangs))
			prepareComment(dst, src);
		else
			(dst = src).strip("en", m_langFlags);
	}

	return dst;
}


void
Consumer::putPrecedingComment(Comment const& comment, Annotation const& annotation, MarkSet const& marks)
{
	M_REQUIRE(!comment.isEmpty() || !annotation.isEmpty() || !marks.isEmpty());

	if (m_terminated)
		return;

	Entry&	entry = m_stack.top();
	Comment	myComment;

	if (!comment.isEmpty())
	{
		prepareComment(myComment, comment);

		if (myComment.isEmpty() && annotation.isEmpty() && marks.isEmpty())
			return;
	}

	if (entry.empty)
	{
		if (!isMainline())
		{
			++m_variationCount;
			beginVariation();
		}

		entry.empty = false;
	}

	++m_commentCount;
	m_langFlags |= comment.langFlags();
	m_annotationCount += annotation.count();
	m_markCount += marks.count();

	sendPrecedingComment(comment, annotation, marks);
}


void
Consumer::putTrailingComment(Comment const& comment)
{
	M_REQUIRE(!comment.isEmpty());

	if (m_terminated)
		return;

	Comment myComment;

	if (prepareComment(myComment, comment).isEmpty())
		return;

	++m_commentCount;
	m_langFlags |= comment.langFlags();

	sendTrailingComment(comment, m_stack.top().empty);
}


void
Consumer::putMove(Move const& move,
						Annotation const& annotation,
						Comment const& preComment,
						Comment const& comment,
						MarkSet const& marks)
{
	M_REQUIRE(terminated() || board().isValidMove(move, m_useVariant));

	if (m_terminated)
		return;

	Entry& entry = m_stack.top();

	Comment myPreComment, myComment;

	if (entry.empty)
	{
		if (!isMainline())
		{
			++m_variationCount;
			beginVariation();
		}

		entry.empty = false;
	}

	if (!preComment.isEmpty() && !prepareComment(myPreComment, preComment).isEmpty())
	{
		m_langFlags |= myPreComment.langFlags();
		++m_commentCount;
	}

	if (!comment.isEmpty() && !prepareComment(myComment, comment).isEmpty())
	{
		m_langFlags |= myComment.langFlags();
		++m_commentCount;
	}

	m_annotationCount += annotation.count();
	m_markCount += marks.count();

	entry.move = move;
	entry.board.prepareUndo(entry.move);

	if (sendMove(entry.move, annotation, marks, myPreComment, myComment))
		afterSendMove(entry);
	else
		m_terminated = true;
}


void
Consumer::putMove(Move const& move)
{
	M_REQUIRE(terminated() || board().isValidMove(move, m_useVariant));

	if (m_terminated)
		return;

	Entry& entry = m_stack.top();

	if (entry.empty)
	{
		if (!isMainline())
		{
			++m_variationCount;
			beginVariation();
		}

		entry.empty = false;
	}

	entry.move = move;
	entry.board.prepareUndo(entry.move);

	if (sendMove(entry.move))
		afterSendMove(entry);
	else
		m_terminated = true;
}


void
Consumer::afterSendMove(Entry& entry)
{
	Move const& move = entry.move;

	if (!m_moveInfoSet.isEmpty())
	{
		sendMoveInfo(m_moveInfoSet);
		m_moveInfoCount += m_moveInfoSet.count();
		m_moveInfoSet.clear();
	}

	if (isMainline())
	{
		unsigned plyCount = this->plyCount();

		if (plyCount < m_sendTimeTable.size())
		{
			MoveInfoSet const& m_moveInfoSet = m_sendTimeTable[plyCount];
			sendMoveInfo(m_moveInfoSet);
			m_moveInfoCount += m_moveInfoSet.count();
		}

		if (!move.isLegal())
		{
			if (move.isCastling() && !entry.board.isInCheck())
				m_gameFlags |= GameInfo::Flag_Illegal_Castling;
			else
				m_gameFlags |= GameInfo::Flag_Illegal_Move;
		}

		m_homePawns.move(move);

		if (m_line.length < opening::Max_Line_Length)
			m_moveBuffer[m_line.length++] = move.index();
	}

	entry.board.doMove(move, m_useVariant);
}


void
Consumer::startVariation()
{
	M_REQUIRE(!variationIsEmpty());

	if (m_terminated)
		return;

	m_stack.dup();
	Entry& entry = m_stack.top();
	entry.board.undoMove(entry.move, m_useVariant);
	entry.empty = true;
	entry.move.clear();
}


void
Consumer::finishVariation()
{
	M_REQUIRE(terminated() || variationLevel() > 0);

	if (m_terminated)
		return;

	if (!m_stack.top().empty)
		endVariation(m_stack.top().move.isEmpty());

	m_stack.pop();
}


void
Consumer::setStartBoard(Board const& board)
{
	m_stack.bottom().board = board;
	m_setupBoard = false;
}


bool
Consumer::preparseComment(mstl::string&)
{
	return false;
}


void
Consumer::setEngines(EngineList const& engines)
{
	m_engines = engines;
}


void
Consumer::swapEngines(EngineList& engines)
{
	m_engines.swap(engines);
}


void
Consumer::sendMoveInfo(MoveInfoSet const& moveInfoSet)
{
	mstl::string info;
	moveInfoSet.print(m_engines, info);
	sendComment(Comment(info, i18n::None));
	++m_commentCount;
}

// vi:set ts=3 sw=3:
