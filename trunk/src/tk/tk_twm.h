// ======================================================================
// Author : $Author$
// Version: $Revision: 1517 $
// Date   : $Date: 2018-09-06 08:47:10 +0000 (Thu, 06 Sep 2018) $
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
// Copyright: (C) 2010-2018 Gregor Cramer
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
