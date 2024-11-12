// ======================================================================
// $RCSfile: db_eco_table.cpp,v $
// $Revision: 1 $
// $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// $Author: gregor $
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

#include "db_eco_table.h"

db::EcoTable db::EcoTable::m_specimen;

using namespace db;


EcoTable::EcoTable()
	:m_branchBuffer(0)
	,m_nodeBuffer(0)
	,m_nameBuffer(0)
	,m_moveBuffer(0)
	,m_root(0)
{
}


EcoTable::~EcoTable() {}


void
EcoTable::getSuccessors(uint64_t, Successors&) const
{
	M_ASSERT(!"cannot execute stubs");
}


int
EcoTable::Successors::find(uint16_t) const
{
	M_ASSERT(!"cannot execute stubs");
	return -1;
}


uint8_t EcoTable::getStoredLine(Eco, Eco) const { return 0; }
Eco EcoTable::lookup(Line const&, Eco&, unsigned*, Successors*, EcoSet*) const { return Eco(); }

// vi:set ts=3 sw=3:
