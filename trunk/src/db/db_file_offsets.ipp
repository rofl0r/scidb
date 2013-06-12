// ======================================================================
// Author : $Author$
// Version: $Revision: 832 $
// Date   : $Date: 2013-06-12 06:32:40 +0000 (Wed, 12 Jun 2013) $
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
// Copyright: (C) 2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

namespace db {

inline
void
FileOffsets::append(unsigned offset)
{
	m_offsets.push_back(offset);
}

} // namespace db

// vi:set ts=3 sw=3:
