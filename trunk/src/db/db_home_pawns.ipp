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
// Copyright: (C) 2009-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_utility.h"

namespace db {

inline unsigned HomePawns::used() const		{ return mstl::div4(m_shift); }
inline uint16_t HomePawns::signature() const	{ return m_signature; }
inline hp::Pawns HomePawns::data() const		{ return m_data; }


inline
bool
HomePawns::isReachable(uint16_t currentSig, hp::Pawns targetData, unsigned count)
{
	// NOTE: only working if target is derived from start position

	if (currentSig == Start)
		return true;

	if (count == 0)
		return targetData.value == 0; // this means: we do not use home pawns

	if (currentSig == 0)
		return count == 16;

	return checkIfReachable(currentSig, targetData, count);
}

} // namespace db

// vi:set ts=3 sw=3:
