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

#include "db_eco.h"

#include <stdio.h>
#include <stdlib.h>

using namespace db;

int
main(int argc, char const* argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: eco2code <eco-value>\n");
		return -1;
	}

	unsigned value = strtoul(argv[1], 0, 10);

	printf("%u -> %s\n", value, Eco(value).asString().c_str());

	return 0;
}

// vi:set ts=3 sw=3:
