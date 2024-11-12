// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
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

#include "tk_init.h"
#include "tcl_base.h"

#include "db_annotation.h"

#include "u_zstream.h"

#include "m_exception.h"

#include <tcl.h>
#include <tk.h>
#include <stdlib.h>


static int
init(Tcl_Interp* ti)
{
	db::Annotation::initialize();	// initializing before main() is not working for any reason

	try
	{
		util::ZStream::setZipFileSuffixes(util::ZStream::Strings(1, "pgn"));

		if (Tcl_Init(ti) == TCL_ERROR || Tk_Init(ti) == TCL_ERROR)
			return TCL_ERROR;

		Tcl_PkgProvide(ti, "tkscidb", "1.0");

		tcl::init(ti);
		tk::init(ti);
	}
	catch (mstl::exception const& exc)
	{
		fprintf(stderr, "exception catched: %s\n", exc.what());
	}

	return TCL_OK;	// satisfies the compiler
}


int
main(int argc, char* argv[])
{
	Tk_Main(argc, argv, init);
	return 0;
}

// vi:set ts=3 sw=3:
