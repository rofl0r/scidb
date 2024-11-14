// ======================================================================
// Author : $Author$
// Version: $Revision: 639 $
// Date   : $Date: 2013-01-23 20:50:00 +0000 (Wed, 23 Jan 2013) $
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

#include "db_database_content.h"
#include "db_game_info.h"

using namespace db;


DatabaseContent::~DatabaseContent() throw() {}


DatabaseContent::DatabaseContent(mstl::string const& encoding, Type type)
	:m_type(type)
	,m_variant(variant::Undetermined)
	,m_created(0)
	,m_readOnly(false)
	,m_writeable(true)
	,m_memoryOnly(false)
	,m_temporary(false)
	,m_shouldCompress(false)
	,m_encoding(encoding)
	,m_allocator(32768)
{
}


DatabaseContent::DatabaseContent(DatabaseContent const& content)
	:m_type(content.m_type)
	,m_variant(content.m_variant)
	,m_created(content.m_created)
	,m_readOnly(content.m_readOnly)
	,m_writeable(content.m_writeable)
	,m_memoryOnly(content.m_memoryOnly)
	,m_temporary(content.m_temporary)
	,m_shouldCompress(false)
	,m_description(content.m_description)
	,m_encoding(content.m_encoding)
	,m_allocator(32768)
{
}

// vi:set ts=3 sw=3:
