// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _sys_dl_included
#define _sys_dl_included

namespace mstl { class string; }

namespace sys {
namespace dl {

struct Handle;

Handle* open(char const* path, mstl::string* error);
void close(Handle*& handle);
void* lookup(Handle* handle, char const* symbol);

} // namespace dl
} // namespace sys

#endif // _sys_dl_included

// vi:set ts=3 sw=3:
