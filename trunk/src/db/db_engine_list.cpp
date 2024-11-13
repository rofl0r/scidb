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
// Copyright: (C) 2011-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "db_engine_list.h"

using namespace db;


mstl::string const&
EngineList::engine(unsigned n) const
{
	for (unsigned i = 0; i < m_map.size(); ++i)
	{
		if (m_map.container()[i].second == n)
			return m_map.container()[i].first;
	}

	return mstl::string::empty_string;
}


unsigned
EngineList::addEngine(mstl::string const& engine)
{
	if (engine.empty() || count() == MaxEngines)
		return 0;

	return m_map.insert(Map::value_type(engine, m_map.size() + 1)).first->second;
}

// vi:set ts=3 sw=3:
