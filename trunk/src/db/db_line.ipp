// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
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

#include "m_utility.h"
#include "m_assert.h"
#include "m_static_check.h"

#include <string.h>

namespace db {

inline Line::Line() : moves(0), length(0) {}


inline Line::Line(uint16_t const* moves, unsigned length)
	:moves(moves)
	,length(length)
{
	M_REQUIRE(moves);
}


inline
void
Line::copy(Line const& line)
{
	M_REQUIRE(moves);

	length = line.length;
	M_STATIC_CHECK(sizeof(moves[0]) == 2, Memcmp_Cannot_Work);
	::memcpy(const_cast<uint16_t*>(moves), line.moves, length << 1);
}


inline
void
Line::copy(Line const& line, unsigned maxLength)
{
	M_REQUIRE(moves);

	length = mstl::min(line.length, maxLength);
	M_STATIC_CHECK(sizeof(moves[0]) == 2, Memcmp_Cannot_Work);
	::memcpy(const_cast<uint16_t*>(moves), line.moves, length << 1);
}


inline
Line&
Line::transpose()
{
	return transpose(*this);
}


inline
bool
Line::operator==(Line const& line) const
{
	M_STATIC_CHECK(sizeof(moves[0]) == 2, Memcmp_Cannot_Work);
	return length == line.length && ::memcmp(moves, line.moves, length << 1) == 0;
}


inline
bool
Line::operator!=(Line const& line) const
{
	M_STATIC_CHECK(sizeof(moves[0]) == 2, Memcmp_Cannot_Work);
	return length != line.length || ::memcmp(moves, line.moves, length << 1) != 0;
}


inline
bool
Line::operator<=(Line const& line) const
{
	M_STATIC_CHECK(sizeof(moves[0]) == 2, Memcmp_Cannot_Work);
	return length <= line.length && ::memcmp(moves, line.moves, length << 1) <= 0;
}


inline
bool
Line::partialMatch(Line const& line) const
{
	M_REQUIRE(length <= line.length);
	M_STATIC_CHECK(sizeof(moves[0]) == 2, Memcmp_Cannot_Work);
	return ::memcmp(moves, line.moves, length << 1) == 0;
}


inline
uint16_t
Line::operator[](unsigned n) const
{
	M_REQUIRE(n < length);
	return moves[n];
}

} // namespace db

// vi:set ts=3 sw=3:
