// ======================================================================
// Author : $Author$
// Version: $Revision: 193 $
// Date   : $Date: 2012-01-16 09:55:54 +0000 (Mon, 16 Jan 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C)2011-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "app_exception.h"

#include <stdarg.h>

using namespace app;


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

// vi:set ts=3 sw=3:
