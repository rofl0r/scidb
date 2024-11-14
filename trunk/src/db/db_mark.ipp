// ======================================================================
// Author : $Author$
// Version: $Revision: 643 $
// Date   : $Date: 2013-01-29 13:15:54 +0000 (Tue, 29 Jan 2013) $
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

#include "m_utility.h"
#include "m_assert.h"

namespace db {

inline bool Mark::isEmpty() const		{ return m_command == mark::None; }

inline mark::Type Mark::type() const	{ return m_type; }
inline char Mark::text() const			{ return m_text; }
inline mark::Color Mark::color() const	{ return m_color; }


inline
Square
Mark::square(unsigned index) const
{
	M_REQUIRE(index <= 1);
	return index ? m_square2 : m_square1;
}


inline
void
Mark::clear()
{
	*this = Mark();
}


inline
bool
Mark::isMark(Byte firstByte)
{
	return !(firstByte & 0x80);
}

} // namespace db

// vi:set ts=3 sw=3:
