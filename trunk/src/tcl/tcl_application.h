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

#ifndef _tcl_application_included
#define _tcl_application_included

#include "app_application.h"

extern "C" { struct Tcl_Interp; }

namespace tcl {
namespace app {

extern ::app::Application scidb;
extern ::app::Application const& Scidb;

void init(Tcl_Interp* interp);

} // namespace app
} // namespace tcl

#endif // _tcl_application_included

// vi:set ts=3 sw=3:
