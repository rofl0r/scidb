// ======================================================================
// Author : $Author: gcramer $
// Version: $Revision: 1212 $
// Date   : $Date: 2017-06-24 12:47:56 +0000 (Sat, 24 Jun 2017) $
// Url    : $URL: https://svn.code.sf.net/p/scidb/code/trunk/src/tk/tk_base.ipp $
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
// ======================================================================

// ======================================================================
// Copyright: (C) 2017 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "tcl_base.h"

#include "m_assert.h"


inline Tk_Window tk::window(Tcl_Obj* path) { return window(tcl::asString(path)); }


inline
Tk_Window
tk::parent(Tk_Window window)
{
	M_REQUIRE(window);
	return Tk_Parent(window);
}


inline
int
tk::width(Tk_Window window)
{
	M_REQUIRE(window);
	return Tk_Width(window);
}


inline
int
tk::height(Tk_Window window)
{
	M_REQUIRE(window);
	return Tk_Height(window);
}


inline
void
tk::makeExists(Tk_Window window)
{
	M_REQUIRE(window);
	Tk_MakeWindowExist(window);
}


inline
bool
tk::exists(char const* path)
{
	M_REQUIRE(path);
	return Tk_NameToWindow(::tcl::interp(), path, mainWindow());
}


inline bool tk::exists(Tcl_Obj* obj) { return exists(tcl::asString(obj)); }


inline
bool
tk::isTopLevel(Tk_Window window)
{
	M_REQUIRE(window);
	return Tk_IsTopLevel(window);
}


inline
void
tk::unmap(Tk_Window window)
{
	M_REQUIRE(window);
	Tk_UnmapWindow(window);
}


inline
void
tk::unmanage(Tk_Window window)
{
	M_REQUIRE(window);
	Tk_ManageGeometry(window, nullptr, nullptr);
}


inline
void
tk::resize(Tk_Window window, int width, int height)
{
	M_REQUIRE(window);
	M_REQUIRE(width >= 0);
	M_REQUIRE(height >= 0);

	Tk_ResizeWindow(window, width > 0 ? width : 1, height > 0 ? height : 1);
}


inline
void
tk::createEventHandler(Tk_Window window, unsigned long mask, Tk_EventProc* proc, void* clientData)
{
	M_REQUIRE(window);
	M_REQUIRE(mask);
	M_REQUIRE(proc);

	Tk_CreateEventHandler(window, mask, proc, clientData);
}


inline
void
tk::deleteEventHandler(Tk_Window window, unsigned long mask, Tk_EventProc* proc, void* clientData)
{
	M_REQUIRE(window);
	M_REQUIRE(mask);
	M_REQUIRE(proc);

	Tk_DeleteEventHandler(window, mask, proc, clientData);
}

// vi:set ts=3 sw=3:
