// ======================================================================
// Author : $Author$
// Version: $Revision: 580 $
// Date   : $Date: 2012-12-19 10:39:49 +0000 (Wed, 19 Dec 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "sys_signal.h"

#ifdef __WIN32__

bool sys::signal::sendInterrupt(long pid) { return false; }

#else

// avoid warning "Attempt to use kernel headers from user space", what a nonsense!
# define __KERNEL__
# include <signal.h>

bool
sys::signal::sendInterrupt(long pid)
{
	return ::kill(pid, SIGINT) == 0;
}


bool
sys::signal::sendTerminate(long pid)
{
	return ::kill(pid, SIGTERM) == 0;
}

#endif

// vi:set ts=3 sw=3:
