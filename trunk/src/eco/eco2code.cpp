// ======================================================================
// Author : $Author: gcramer $
// Version: $Revision: 1411 $
// Date   : $Date: 2017-08-12 11:08:17 +0000 (Sat, 12 Aug 2017) $
// Url    : $URL: https://svn.code.sf.net/p/scidb/code/trunk/src/eco/eco2code.cpp $
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2017 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "eco_id.h"

#include <stdio.h>

auto main(int argc, char const* argv[]) -> int
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: eco2code A00.001\n");
		return -1;
	}

	printf("%s -> %u\n", argv[1], unsigned(eco::Id(argv[1])));

	return 0;
}

// vi:set ts=3 sw=3:
