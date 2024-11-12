// ======================================================================
// Author : $Author$
// Version: $Revision: 84 $
// Date   : $Date: 2011-07-18 18:02:11 +0000 (Mon, 18 Jul 2011) $
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

#ifndef _db_game_data_included
#define _db_game_data_included

#include "db_board.h"
#include "db_tag_set.h"

#include "m_utility.h"

namespace db {

class MoveNode;

class GameData : public mstl::noncopyable
{
public:

	GameData();
	virtual ~GameData() throw();

	MoveNode*	m_startNode;	/// Keeps the starting node of the game
	Board			m_startBoard;	/// Keeps the start position of the game
	TagSet		m_tags;
};

} // namespace db

#endif // _db_game_data_included

// vi:set ts=3 sw=3:
