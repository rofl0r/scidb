// ======================================================================
// Author : $Author$
// Version: $Revision: 851 $
// Date   : $Date: 2013-06-24 15:15:00 +0000 (Mon, 24 Jun 2013) $
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
// Copyright: (C) 2011-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_assert.h"

namespace db {

inline MoveInfoSet::MoveInfoSet() {}

inline bool MoveInfoSet::isEmpty() const									{ return m_row.empty(); }
inline bool MoveInfoSet::contains(MoveInfo const& info) const		{ return find(info) >= 0; }

inline unsigned MoveInfoSet::count() const								{ return m_row.size(); }

inline MoveInfo const& MoveInfoSet::operator[](unsigned n) const	{ return m_row[n]; }
inline MoveInfo& MoveInfoSet::operator[](unsigned n)					{ return m_row[n]; }

inline void MoveInfoSet::resize(unsigned n)		{ m_row.resize(n); }
inline void MoveInfoSet::reserve(unsigned n)		{ m_row.reserve(n); }
inline void MoveInfoSet::swap(MoveInfoSet& row)	{ m_row.swap(row.m_row); }
inline void MoveInfoSet::clear()						{ m_row.clear(); }


inline
bool
MoveInfoSet::operator!=(MoveInfoSet const& info) const
{
	return !operator==(info);
}


inline
MoveInfo&
MoveInfoSet::add()
{
	m_row.push_back();
	return m_row.back();
}


inline
MoveInfo&
MoveInfoSet::add(MoveInfo const& info)
{
	M_REQUIRE(!info.isEmpty());
	m_row.push_back(info);
	return m_row.back();
}

} // namespace db

// vi:set ts=3 sw=3:
