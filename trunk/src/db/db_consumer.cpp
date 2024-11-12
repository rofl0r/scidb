// ======================================================================
// Author : $Author$
// Version: $Revision: 36 $
// Date   : $Date: 2011-06-13 20:30:54 +0000 (Mon, 13 Jun 2011) $
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
// Copyright: (C) 2009-2011 Gregor Cramer
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
#include "db_game_info.h"

#include "sys_utf8_codec.h"

#include "m_assert.h"

#include <stdlib.h>
#include <ctype.h>

using namespace db;


Consumer::Consumer(format::Type srcFormat, mstl::string const& encoding)
	:m_format(srcFormat)
	,m_stack(1)
	,m_variationCount(0)
	,m_commentCount(0)
	,m_annotationCount(0)
	,m_markCount(0)
	,m_terminated(false)
	,m_flags(0)
	,m_line(m_moveBuffer)
	,m_encoding(encoding)
	,m_codec(new sys::utf8::Codec(encoding))
	,m_consumer(0)
	,m_setupBoard(true)
	,m_hasComment(false)
	,m_commentEngFlag(false)
	,m_commentOthFlag(false)
{
}


Consumer::~Consumer() throw()
{
	delete m_codec;
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


void
Consumer::setup(Board const& startPosition)
{
	m_stack.bottom().board = startPosition;
}


void
Consumer::setup(unsigned idn)
{
	m_stack.bottom().board.setup(idn);
}


void
Consumer::setup(mstl::string const& fen)
{
	// XXX possibly we should allow:
	// 1. handicap games
	// 2. illegal positions (king in check)
	M_ASSERT(Board::isValidFen(fen, variant::Unknown));

	m_stack.bottom().board.setup(fen);
}


bool
Consumer::startGame(TagSet const& tags, Board const* board)
{
	while (m_stack.size() > 1)
		m_stack.pop();

	m_variationCount = 0;
	m_commentCount = 0;
	m_annotationCount = 0;
	m_markCount = 0;
	m_terminated = false;
	m_line.length = 0;
	m_hasComment = false;
	m_commentEngFlag = false;
	m_commentOthFlag = false;
	m_homePawns.clear();

	if (board)
		m_stack.bottom().board = *board;
	else if (tags.contains(tag::Fen))
		setup(tags.value(tag::Fen));
	else if (tags.contains(tag::Idn))
		setup(::strtoul(tags.value(tag::Idn), 0, 10));
	else if (m_setupBoard)
		setup(Board::standardBoard());

	if (getStartBoard().notDerivableFromChess960())
		return false;

	m_stack.dup();
	m_stack.top().empty = true;

	return beginGame(tags);
}


save::State
Consumer::finishGame(TagSet const& tags)
{
	M_REQUIRE(variationLevel() == 0);

	if (startBoard().isStartPosition())
		m_stack.top().board.signature().setHomePawns(m_homePawns.used(), m_homePawns.data());

	save::State state = endGame(tags);
	m_stack.pop();
	return state;
}


void
Consumer::finishMoveSection(result::ID result)
{
	sendComment();	// send dangling pre-comment

	if (m_terminated)
	{
		while (variationLevel() > 0)
		{
			endVariation();
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


void
Consumer::putComment(Comment const& comment)
{
	m_comment.append(comment, '\n');
	m_hasComment = !m_comment.isEmpty();
}


void
Consumer::putComment(Comment const& comment, Annotation const& annotation, MarkSet const& marks)
{
	m_comment.append(comment, ' ');
	m_preAnnotation.add(annotation);
	m_preMarks.add(marks);
	m_hasComment = !comment.isEmpty() || !annotation.isEmpty() || !marks.isEmpty();

}


void
Consumer::sendComment()
{
	if (m_hasComment)
	{
		if (!m_terminated)
		{
			if (!m_comment.isEmpty())
				++m_commentCount;
			m_annotationCount += m_preAnnotation.count();
			m_markCount += m_preMarks.count();

			if (m_comment.engFlag())
				m_commentEngFlag = true;
			if (m_comment.othFlag())
				m_commentOthFlag = true;

			sendComment(m_comment, m_preAnnotation, m_preMarks);

			m_comment.clear();
			m_preAnnotation.clear();
			m_preMarks.clear();
		}

		m_hasComment = false;
	}
}


void
Consumer::putMove(Move const& move,
						Annotation const& annotation,
						Comment const& preComment,
						Comment const& comment,
						MarkSet const& marks)
{
	M_REQUIRE(terminated() || board().isValidMove(move));

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

		sendComment();
		entry.empty = false;
	}

	if (!comment.isEmpty())
	{
		if (comment.engFlag())
			m_commentEngFlag = true;
		if (comment.othFlag())
			m_commentOthFlag = true;
		++m_commentCount;
	}

	if (!preComment.isEmpty())
	{
		if (preComment.engFlag())
			m_commentEngFlag = true;
		if (preComment.othFlag())
			m_commentOthFlag = true;
		++m_commentCount;
	}

	m_annotationCount += annotation.count();
	m_markCount += marks.count();

	entry.move = move;
	entry.board.prepareUndo(entry.move);

	if (sendMove(entry.move, annotation, marks, preComment, comment))
	{
		entry.board.doMove(entry.move);

		if (!move.isLegal())
			m_flags |= GameInfo::Flag_Illegal_Move;

		if (isMainline())
		{
			m_homePawns.move(move);

			if (m_line.length < opening::Max_Line_Length)
				m_moveBuffer[m_line.length++] = move.index();
		}
	}
	else
	{
		m_terminated = true;
	}
}


void
Consumer::putMove(Move const& move)
{
	M_REQUIRE(terminated() || board().isValidMove(move));

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

		sendComment();
		entry.empty = false;
	}

	entry.move = move;
	entry.board.prepareUndo(entry.move);

	if (sendMove(entry.move))
	{
		entry.board.doMove(entry.move);

		if (!move.isLegal())
			m_flags |= GameInfo::Flag_Illegal_Move;

		if (isMainline())
		{
			m_homePawns.move(move);

			if (m_line.length < opening::Max_Line_Length)
				m_moveBuffer[m_line.length++] = move.index();
		}
	}
	else
	{
		m_terminated = true;
	}
}


void
Consumer::startVariation()
{
	M_REQUIRE(!variationIsEmpty());

	if (m_terminated)
		return;

	m_stack.dup();
	Entry& entry = m_stack.top();
	entry.empty = true;
	entry.board.undoMove(entry.move);
}


void
Consumer::finishVariation()
{
	M_REQUIRE(terminated() || variationLevel() > 0);

	if (m_terminated)
		return;

	if (!m_stack.top().empty)
		endVariation();

	m_stack.pop();
}


void
Consumer::setStartBoard(Board const& board)
{
	m_stack.bottom().board = board;
	m_setupBoard = false;
}

// vi:set ts=3 sw=3:
