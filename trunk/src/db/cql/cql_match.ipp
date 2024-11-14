// ======================================================================
// Author : $Author$
// Version: $Revision: 719 $
// Date   : $Date: 2013-04-19 16:40:59 +0000 (Fri, 19 Apr 2013) $
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

inline bool Match::isStandard() const		{ return m_ranges.empty(); }
inline bool Match::matchComments() const	{ return !m_matchCommentList.empty(); }

inline Match::Ranges const& Match::nonStandardRanges() const { return m_ranges; }

inline unsigned Match::sections() const { return m_sections; }

inline void Match::setInitial()	{ m_initialOnly = true; }
inline void Match::setFinal()		{ m_finalOnly = true; }

} // namespace cql

// vi:set ts=3 sw=3:
