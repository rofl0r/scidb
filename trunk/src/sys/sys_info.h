// ======================================================================
// Author : $Author$
// Version: $Revision: 668 $
// Date   : $Date: 2013-03-10 18:15:28 +0000 (Sun, 10 Mar 2013) $
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

#ifndef _sys_info_included
#define _sys_info_included

#include "m_types.h"

namespace sys {
namespace info {

unsigned numberOfProcessors();

int64_t memFree();
int64_t memTotal();

bool isWindows();

} // namespace info
} // namespace sys

#endif // _sys_info_included

// vi:set ts=3 sw=3:
