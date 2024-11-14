// ======================================================================
// Author : $Author$
// Version: $Revision: 794 $
// Date   : $Date: 2013-05-22 20:19:59 +0000 (Wed, 22 May 2013) $
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

#include "cql_engine.h"

#include "m_assert.h"

using namespace cql;

Engine::Creator* Engine::m_creator;


Engine::Creator::~Creator()
{
	// no action
}


Engine::~Engine()
{
	// no action
}


bool
Engine::hasCreator()
{
	return m_creator != 0;
}


void
Engine::hookCreator(Creator* creator)
{
	m_creator = creator;
}

// vi:set ts=3 sw=3:
