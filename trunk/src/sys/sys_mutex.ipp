// ======================================================================
// Author : $Author: gcramer $
// Version: $Revision: 1339 $
// Date   : $Date: 2017-07-31 19:09:29 +0000 (Mon, 31 Jul 2017) $
// Url    : $HeadURL: https://svn.code.sf.net/p/scidb/code/trunk/src/sys/sys_mutex.ipp $
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

namespace sys {

#ifdef __WIN32__

inline Mutex::Mutex()			{ InitializeCriticalSection(&m_lock); }
inline void Mutex::lock()		{ EnterCriticalSection(&m_lock); }
inline void Mutex::release()	{ LeaveCriticalSection(&m_lock); }

#else // !__WIN32__

inline Mutex::Mutex()			{ pthread_mutex_init(&m_lock, 0); }
inline void Mutex::lock()		{ pthread_mutex_lock(&m_lock); }
inline void Mutex::release()	{ pthread_mutex_unlock(&m_lock); }

#endif // __WIN32__

} // namespace sys

// vi:set ts=3 sw=3:
