// ======================================================================
// Author : $Author$
// Version: $Revision: 5 $
// Date   : $Date: 2011-05-05 07:51:24 +0000 (Thu, 05 May 2011) $
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

#ifndef _tk_twm_included
#define _tk_twm_included

extern "C" { struct Tcl_Interp; }

namespace tk {
namespace twm {

void init(Tcl_Interp* interp);

} // namespace twm
} // namespace tk

#endif // _tk_twm_included

// vi:set ts=3 sw=3:
