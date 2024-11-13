// ======================================================================
// Author : $Author$
// Version: $Revision: 430 $
// Date   : $Date: 2012-09-20 17:13:27 +0000 (Thu, 20 Sep 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2011-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _tk_ogl_defined
#define _tk_ogl_defined

#define OGL_VERSION "1.0"

extern "C" { struct Tcl_Interp; }

namespace tk {
namespace ogl {

struct Context;

int init(Tcl_Interp *interp);
void swapBuffers(Context const* ctx);

} // namespace ogl
} // namespace stk

#endif // _tk_ogl_defined

// vi:set ts=3 sw=3:
