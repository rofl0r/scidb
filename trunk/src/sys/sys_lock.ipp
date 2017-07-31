// ======================================================================
// Author : $Author: gcramer $
// Version: $Revision: 1339 $
// Date   : $Date: 2017-07-31 19:09:29 +0000 (Mon, 31 Jul 2017) $
// Url    : $HeadURL: https://svn.code.sf.net/p/scidb/code/trunk/src/sys/sys_lock.ipp $
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

#include "sys_mutex.h"

namespace sys {

Lock::Lock(Mutex* mutex) :m_mutex(mutex) { if (m_mutex) m_mutex->lock(); }
Lock::~Lock() { if (m_mutex) m_mutex->release(); }

} // namespace sys

// vi:set ts=3 sw=3:
