// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
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

#ifndef _db_game_data_included
#define _db_game_data_included

#include "db_board.h"
#include "db_tag_set.h"
#include "db_engine_list.h"
#include "db_time_table.h"

#include "m_utility.h"

namespace db {

class MoveNode;

class GameData : public mstl::noncopyable
{
public:

	GameData();
	virtual ~GameData() throw();

	MoveNode*		m_startNode;	///< Keeps the starting node of the game
	Board				m_startBoard;	///< Keeps the start position of the game
	TagSet			m_tags;
	variant::Type	m_variant;
	uint16_t			m_idn;
	EngineList		m_engines;
	TimeTable		m_timeTable;

};

} // namespace db

#endif // _db_game_data_included

// vi:set ts=3 sw=3:
