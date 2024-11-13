// ======================================================================
// Author : $Author$
// Version: $Revision: 430 $
// Date   : $Date: 2012-09-20 17:13:27 +0000 (Thu, 20 Sep 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
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

#ifndef _app_exception_included
#define _app_exception_included

#include "u_exception.h"

#define APP_RAISE(fmt, args...) M_THROW(::app::Exception(fmt, ##args))

namespace app {

class Exception : public util::Exception
{
public:

	Exception();
	explicit Exception(char const* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
	Exception(util::Exception& exc);
	~Exception() throw();
};

} // namespace app

#endif // _app_exception_included

// vi:set ts=3 sw=3:
