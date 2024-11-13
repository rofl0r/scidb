// ======================================================================
// Author : $Author$
// Version: $Revision: 569 $
// Date   : $Date: 2012-12-16 21:41:55 +0000 (Sun, 16 Dec 2012) $
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
// Copyright: (C) 2011-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_assert.h"

namespace db {

inline bool MoveInfo::isEmpty() const				{ return m_content == None; }
inline MoveInfo::Type MoveInfo::content() const	{ return m_content; }
inline unsigned MoveInfo::centiSeconds() const	{ return m_centiSeconds; }


inline
bool
MoveInfo::hasEvaluationInfo() const
{
	return m_content == Evaluation;
}


inline
Clock const&
MoveInfo::clock() const
{
	M_REQUIRE(hasTimeInfo());
	return m_time.m_clock;
}


inline
Date const&
MoveInfo::date() const
{
	M_REQUIRE(hasTimeInfo());
	return m_time.m_date;
}


inline
unsigned
MoveInfo::engine() const
{
	M_REQUIRE(hasTimeInfo() || hasEvaluationInfo());
	return m_engine;
}


inline
unsigned
MoveInfo::depth() const
{
	M_REQUIRE(hasEvaluationInfo());
	return m_analysis.m_depth;
}


inline
unsigned
MoveInfo::pawns() const
{
	M_REQUIRE(hasEvaluationInfo());
	return m_analysis.m_pawns;
}


inline
unsigned
MoveInfo::centipawns() const
{
	M_REQUIRE(hasEvaluationInfo());
	return m_analysis.m_centipawns;
}


inline
float
MoveInfo::value() const
{
	M_REQUIRE(hasEvaluationInfo());
	return float(m_analysis.m_pawns) + float(m_analysis.m_centipawns)/100.0f;
}


inline
char const*
MoveInfo::parsePlayersClock(char const* s)
{
	return parseTime(PlayersClock, s);
}


inline
char const*
MoveInfo::parseClockTime(char const* s)
{
	return parseTime(ClockTime, s);
}


inline
char const*
MoveInfo::parseElapsedGameTime(char const* s)
{
	return parseTime(ElapsedGameTime, s);
}


inline
char const*
MoveInfo::parseElapsedMoveTime(char const* s)
{
	return parseElapsedTime(s);
}


inline
char const*
MoveInfo::parseVideoTime(char const* s)
{
	return parseCentiSeconds(s);
}


inline
void
MoveInfo::clear()
{
	*this = MoveInfo();
}

} // namespace db

// vi:set ts=3 sw=3:
