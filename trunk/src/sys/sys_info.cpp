// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
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

#include "sys_info.h"

#ifdef __unix__
# include <unistd.h>
#endif

#ifdef __hpux
# include <sys/pstat.h>
#endif

#if defined(__WIN32__) || defined(_WIN64__)
# include <windows.h>
#endif


unsigned
sys::info::numberOfProcessors()
{
#if defined(__WIN32__) || defined(_WIN64__)

	SYSTEM_INFO s;
	GetSystemInfo(&s);
	return s.dwNumberOfProcessors;

#elif defined(_SC_NPROCESSORS_ONLN)

	return ::sysconf(_SC_NPROCESSORS_ONLN);

#elif defined (__hpux)

	struct ::pst_dynamic psd;
	if (::pstat_getdynamic(&psd, sizeof(psd), 1, 0) == -1)
		return 1;
	return psd.psd_proc_cnt;

#else

	return 1;

#endif
}

// vi:set ts=3 sw=3:
