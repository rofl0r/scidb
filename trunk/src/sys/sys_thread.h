// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
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

#ifndef _sys_thread_included
#define _sys_thread_included

#include "m_function.h"

namespace sys {
namespace thread {

typedef mstl::function<void ()> Runnable;

void start(Runnable runnable);
bool stop();
bool testCancel();
void yield();

} // namespace thread
} // namespace sys

#endif // _sys_thread_included

// vi:set ts=3 sw=3:
