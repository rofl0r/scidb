// ======================================================================
// Author : $Author$
// Version: $Revision: 769 $
// Date   : $Date: 2013-05-10 22:26:18 +0000 (Fri, 10 May 2013) $
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

namespace cql {

inline bool PieceTypeDesignator::test(piece::ID piece) const { return m_pieces.test(piece); }
inline PieceTypeDesignator::Pieces const& PieceTypeDesignator::pieces() const { return m_pieces; }

} // namespace cql

// vi:set ts=3 sw=3:
