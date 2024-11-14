// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2011-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _sys_base_included
#define _sys_base_included

#define TCL_PREREQ(maj, min) ((TCL_MAJOR_VERSION << 16) + TCL_MINOR_VERSION >= ((maj) << 16) + (min))

extern "C" { struct Tcl_Interp; }

namespace sys {
namespace tcl {

Tcl_Interp* interp();

} // namespace tcl
} // namespace sys

#include "sys_base.ipp"

#endif // _sys_base_included

// vi:set ts=3 sw=3:
