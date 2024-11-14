// ======================================================================
// Author : $Author$
// Version: $Revision: 631 $
// Date   : $Date: 2013-01-11 16:16:29 +0000 (Fri, 11 Jan 2013) $
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
// Copyright: (C) 2012-2013 Gregor Cramer
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
inline void TimeTable::clear()							{ m_table.clear(); }

inline MoveInfo const& TimeTable::operator[](unsigned index) const { return m_table[index]; }

} // namespace db

// vi:set ts=3 sw=3:
