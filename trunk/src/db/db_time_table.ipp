// ======================================================================
// Author : $Author$
// Version: $Revision: 602 $
// Date   : $Date: 2013-01-01 16:53:57 +0000 (Tue, 01 Jan 2013) $
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
// Copyright: (C) 2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

namespace db {

inline bool TimeTable::isEmpty() const					{ return m_table.empty(); }

inline unsigned TimeTable::size() const				{ return m_table.size(); }

inline void TimeTable::reserve(unsigned capacity)	{ m_table.reserve(capacity); }

inline MoveInfo const& TimeTable::operator[](unsigned index) const { return m_table[index]; }

} // namespace db

// vi:set ts=3 sw=3:
