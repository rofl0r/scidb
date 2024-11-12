// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
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
// Copyright: (C) 2010-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_namebases_included
#define _db_namebases_included

#include "db_namebase.h"

namespace db {

class Namebases
{
public:

	typedef Namebase::Type Type;

	Namebases();

	Namebase& operator()(Type type);
	Namebase const& operator()(Type type) const;

	void update();
	void clear();

private:

	Namebase m_player;
	Namebase m_site;
	Namebase m_event;
	Namebase m_annotator;
	Namebase m_round;
};

} // namespace db

#include "db_namebases.ipp"

#endif // _db_namebases_included

// vi:set ts=3 sw=3:
