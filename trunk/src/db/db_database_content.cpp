// ======================================================================
// Author : $Author$
// Version: $Revision: 36 $
// Date   : $Date: 2011-06-13 20:30:54 +0000 (Mon, 13 Jun 2011) $
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

#include "db_database_content.h"
#include "db_game_info.h"

#include "m_static_check.h"

using namespace db;


DatabaseContent::~DatabaseContent() throw() {}


DatabaseContent::DatabaseContent(Type type)
	:m_type(type)
	,m_created(0)
	,m_readOnly(false)
	,m_memoryOnly(false)
	,m_allocator(32768)
{
}

// vi:set ts=3 sw=3:
