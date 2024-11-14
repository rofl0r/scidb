// ======================================================================
// Author : $Author$
// Version: $Revision: 849 $
// Date   : $Date: 2013-06-20 15:03:29 +0000 (Thu, 20 Jun 2013) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2012-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "tk_init.h"

#include "tcl_base.h"

#include <tcl.h>
#include <tk.h>
#include <stdio.h>

using namespace tcl;

static int m_altModMask = Mod1Mask | Mod5Mask;


#if !defined(__WIN32__) && !defined(__MacOSX__)

# include <X11/keysym.h>

static void
initKeymapInfo(Tcl_Interp* ti)
{
	Display*				display			= Tk_Display(Tk_MainWindow(ti));
	XModifierKeymap*	modMap			= XGetModifierMapping(display);
	int					maxKeyPerMod	= modMap->max_keypermod;
	KeyCode*				keyCode			= modMap->modifiermap;

	m_altModMask = 0;

	for (int i = 0; i < 8*maxKeyPerMod; ++i, ++keyCode)
	{
		if (*keyCode)
		{
			KeySym keysym = XKeycodeToKeysym(display, *keyCode, 0);

			if (keysym == XK_Alt_L || keysym == XK_Alt_R)
				m_altModMask |= (ShiftMask << (i/maxKeyPerMod));
		}
	}

	if (m_altModMask == 0)
		fprintf(stderr, "initKeymapInfo(): no Alt key mapped");

	XFreeModifiermap(modMap);
}

#endif


static int
tkMisc(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	static char const* Subcommands[] = 
	{
		"setClass", "shiftMask?", "lockMask?", "controlMask?", "altMask?"
	};
	enum { Cmd_SetClass, Cmd_ShiftMask, Cmd_LockMask, Cmd_ControlMask, Cmd_AltMask };

	int index;
	int result = Tcl_GetIndexFromObj(ti, objv[1], Subcommands, "subcommand", TCL_EXACT, &index);

	if (result != TCL_OK)
		return TCL_ERROR;

	switch (index)
	{
		case Cmd_SetClass:
		{
			char const*	path	= stringFromObj(objc, objv, 2);
			char const*	cls	= stringFromObj(objc, objv, 3);
			Tk_Window	tkwin	= Tk_NameToWindow(ti, path, Tk_MainWindow(ti));

			if (!tkwin)
				return TCL_ERROR;

			Tk_SetClass(tkwin, cls);
			break;
		}

		case Cmd_ShiftMask:
			Tcl_SetObjResult(ti, Tcl_NewIntObj(ShiftMask));
			break;

		case Cmd_LockMask:
			Tcl_SetObjResult(ti, Tcl_NewIntObj(LockMask));
			break;

		case Cmd_ControlMask:
			Tcl_SetObjResult(ti, Tcl_NewIntObj(ControlMask));
			break;

		case Cmd_AltMask:
			Tcl_SetObjResult(ti, Tcl_NewIntObj(m_altModMask));
			break;
	}

	return TCL_OK;
}


void
tk::miscInit(Tcl_Interp* ti)
{
#if !defined(__WIN32__) && !defined(__MacOSX__)
	initKeymapInfo(ti);
#endif

	tcl::createCommand(ti, "::scidb::tk::misc", tkMisc);
}

// vi:set ts=3 sw=3:
