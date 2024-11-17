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
// Copyright: (C) 2009-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_assert.h"

namespace db {

inline EcoTable::MoveOrder::MoveOrder() :m_buffer(0) {}
inline EcoTable::MoveOrder::~MoveOrder() { delete m_buffer; }
inline bool EcoTable::MoveOrder::isEmpty() const { return m_buffer == 0; }
inline Line const& EcoTable::MoveOrder::line() const { return m_line; }

inline EcoTable::Successors::Successor::Successor() : move(0) {}
inline EcoTable::Successors::Successors() : length(0) {}


inline EcoTable::Opening::Opening() {}


inline
EcoTable::EcoOpening::EcoOpening(unsigned ply, Eco eco, Opening const& opening)
	:ply(ply), eco(eco), opening(opening)
{
}


inline bool EcoTable::Opening::operator!=(const Opening& op) const { return !(*this == op); }


inline
EcoTable const&
EcoTable::specimen(variant::Type variant)
{
	return m_specimen[variant::toIndex(variant)];
}


inline
EcoTable const&
EcoTable::specimen(variant::Index variant)
{
	return m_specimen[variant];
}


inline
Line const&
EcoTable::getLine(Eco code) const
{
	return getEntry(code).line;
}


inline
variant::Type
EcoTable::variant() const
{
	return m_variant;
}

} // namespace db

// vi:set ts=3 sw=3:
