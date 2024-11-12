// ======================================================================
// Author : $Author$
// Version: $Revision: 79 $
// Date   : $Date: 2011-07-14 13:14:44 +0000 (Thu, 14 Jul 2011) $
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

#include "tcl_exception.h"

#include <stdarg.h>

using namespace tcl;


Exception::Exception()
	:util::Exception()
{
}


Exception::Exception(char const* fmt, ...)
	:util::Exception()
{
	va_list args;
	va_start(args, fmt);
	set_message(fmt, args);
	va_end(args);
}


Exception::Exception(util::Exception& exc)
	:util::Exception(exc)
{
}


Exception::~Exception() throw()
{
}


Error::Error()
	:util::Exception()
{
}


Error::Error(util::Exception& exc)
	:util::Exception(exc)
{
}


Error::~Error() throw()
{
}

// vi:set ts=3 sw=3:
