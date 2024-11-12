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

#include "m_assert.h"

namespace db {

inline Annotation::Annotation() : m_count(0) {}

inline bool Annotation::isEmpty() const								{ return m_count == 0; }
inline unsigned Annotation::count() const								{ return m_count; }
inline Annotation const* Annotation::defaultSet(nag::ID nag)	{ return &DefaultSets[nag]; }
inline uint8_t const* Annotation::data() const						{ return m_annotation; }


inline
void
Annotation::clear()
{
	M_REQUIRE(!isDefaultSet());
	m_count = 0;
}


inline
bool
Annotation::operator!=(Annotation const& annotation) const
{
	return !operator==(annotation);
}


inline
nag::ID
Annotation::operator[](unsigned n) const
{
	M_REQUIRE(n < count());
	return nag::ID(m_annotation[n]);
}

} // namespace db

// vi:set ts=3 sw=3:
