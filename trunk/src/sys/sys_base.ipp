// ======================================================================
// Author : $Author$
// Version: $Revision: 60 $
// Date   : $Date: 2011-06-29 21:26:40 +0000 (Wed, 29 Jun 2011) $
// Url    : $URL$
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

#include "tcl_base.h"

namespace sys {
namespace tcl {

inline Tcl_Interp* interp() { return ::tcl::bits::interp; }

} // namespace tcl
} // namespace sys

// vi:set ts=3 sw=3:
