// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2008-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "svg_exception.h"

#include "m_stdio.h"

#include <stdarg.h>

using namespace svg;

exception::exception(char const* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	set_message(fmt, args);
	va_end(args);
}


exception::exception(exception const& exc)
	:mstl::exception(exc)
{
}

// vi:set ts=3 sw=3:
