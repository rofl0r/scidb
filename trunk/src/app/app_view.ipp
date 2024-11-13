// ======================================================================
// Author : $Author$
// Version: $Revision: 373 $
// Date   : $Date: 2012-07-02 10:25:19 +0000 (Mon, 02 Jul 2012) $
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

namespace app {

inline unsigned View::countGames() const		{ return m_gameFilter.count(); }
inline unsigned View::totalGames() const		{ return m_gameFilter.size(); }
inline unsigned View::countPlayers() const	{ return m_playerFilter.count(); }
inline unsigned View::countEvents() const		{ return m_eventFilter.count(); }
inline unsigned View::countSites() const		{ return m_siteFilter.count(); }
inline unsigned View::totalPlayers() const	{ return m_playerFilter.size(); }
inline unsigned View::totalEvents() const		{ return m_eventFilter.size(); }

inline Application const& View::application() const	{ return m_app; }
inline db::Database const& View::database() const		{ return m_db; }

} // namespace db

// vi:set ts=3 sw=3:
