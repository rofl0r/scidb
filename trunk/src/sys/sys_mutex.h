// ======================================================================
// Author : $Author: gcramer $
// Version: $Revision: 1341 $
// Date   : $Date: 2017-08-01 14:21:38 +0000 (Tue, 01 Aug 2017) $
// Url    : $HeadURL: https://svn.code.sf.net/p/scidb/code/trunk/src/sys/sys_mutex.h $
// ======================================================================

// ======================================================================
// Copyright: (C) 2014 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _sys_mutex_included
#define _sys_mutex_included

#ifdef __WIN32__
# include <windows.h>
#else
# include <pthread.h>
#endif

namespace sys {

class Mutex
{
public:

	Mutex();

	bool isLocked() const;

	void lock();
	void release();

private:

#ifdef __WIN32__
	CRITICAL_SECTION mutex_t;
#else
	typedef pthread_mutex_t mutex_t;
#endif

	mutex_t	m_lock;
	bool		m_isLocked;
};

} // namespace sys

#include "sys_mutex.ipp"

#endif // _sys_mutex_included

// vi:set ts=3 sw=3:
