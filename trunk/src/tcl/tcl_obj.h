// ======================================================================
// Author : $Author$
// Version: $Revision: 44 $
// Date   : $Date: 2011-06-19 19:56:08 +0000 (Sun, 19 Jun 2011) $
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
// Copyright: (C) 2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _tcl_obj_included
#define _tcl_obj_included

#include <tcl.h>

namespace tcl {

class Obj
{
public:

	Obj(Tcl_Obj* obj);

	operator Tcl_Obj* () const;

	Tcl_Obj* operator()() const;

	bool operator==(Obj const& obj) const;

	void ref();
	void deref();

private:

	Tcl_Obj* m_obj;
};

} // namespace tcl

#include "tcl_obj.ipp"

#endif // _tcl_obj_included

// vi:set ts=3 sw=3:
