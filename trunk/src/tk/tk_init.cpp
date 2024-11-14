// ======================================================================
// Author : $Author$
// Version: $Revision: 813 $
// Date   : $Date: 2013-05-31 22:23:38 +0000 (Fri, 31 May 2013) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2008-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "tk_init.h"
#include "tk_session_manager.h"

#include <tcl.h>

extern "C"
{
	extern int Tkhtml_Init(Tcl_Interp*);
	extern int Tkhtml_SafeInit(Tcl_Interp*);
	extern int Tkdnd_Init(Tcl_Interp*);
}


void
tk::init(Tcl_Interp* ti)
{
	Tcl_Eval(ti, "namespace eval ::scidb {}");
	Tcl_Eval(ti, "namespace eval ::scidb::tk {}");

	fixes_init(ti);
	selection_init(ti);
	x11_init(ti);
	session_manager_init(ti, "::scidb::tk::sm");
	miscInit(ti);
	window_manager_init(ti);
	twm_init(ti);
	png_init(ti);
	image_init(ti);
	jpeg_init(ti);
	xcursor_init(ti);
	busy_init(ti);
	multiwindow_init(ti);
	notebook_init(ti);

	Tkhtml_Init(ti);
	Tkdnd_Init(ti);
//	Tcl_SetVar(ti, "tcl_rcFileName", "~/.scidb", TCL_GLOBAL_ONLY)
}

// vi:set ts=3 sw=3:
