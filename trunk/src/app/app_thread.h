// ======================================================================
// Author : $Author: gcramer $
// Version: $Revision: 1339 $
// Date   : $Date: 2017-07-31 19:09:29 +0000 (Mon, 31 Jul 2017) $
// Url    : $URL: https://svn.code.sf.net/p/scidb/code/trunk/src/app/app_thread.h $
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
// ======================================================================

// ======================================================================
// Copyright: (C) 2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _app_thread_included
#define _app_thread_included

#include "sys_thread.h"

#include "m_utility.h"

namespace app {

class Cursor;

class Thread : public ::sys::Thread, private mstl::noncopyable
{
public:

	enum Signal { Stop, Cancel, Kill };

	Thread();
	virtual ~Thread() = 0;

	bool isWorkingOn(Cursor const& cursor) const;

	virtual void signal(Signal signal) = 0;

protected:

	void setWorkingOn(Cursor const* cursor = 0);

private:

	Cursor const* m_cursor;
};

} // namespace app

#include "app_thread.ipp"

#endif // _app_thread_included

// vi:set ts=3 sw=3:
