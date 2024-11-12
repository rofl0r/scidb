// ======================================================================
// Author : $Author$
// Version: $Revision: 33 $
// Date   : $Date: 2011-05-29 12:27:45 +0000 (Sun, 29 May 2011) $
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

#include "m_exception.h"
#include "m_sstream.h"
#include "m_string.h"

#include <stdlib.h>
#include <stdarg.h>

using namespace mstl;


exception::exception() throw() : m_msg(new string) {}
exception::exception(string const& msg) : m_msg(new string(msg)) {}
exception::exception(exception const& exc) : m_msg(new mstl::string(*exc.m_msg)) {}
exception::~exception() throw() { delete m_msg; }

char const* exception::what() const throw()		{ return *m_msg; }
backtrace const& exception::backtrace() const	{ return m_backtrace; }


exception::exception(char const* fmt, va_list args)
	:m_msg(new string)
{
	m_msg->vformat(fmt, args);
}


exception::exception(char const* fmt, ...)
	:m_msg(new string)
{
	M_REQUIRE(fmt);

	va_list args;
	va_start(args, fmt);
	m_msg->vformat(fmt, args);
	va_end(args);
}


void
exception::set_message(char const* fmt, va_list args)
{
	m_msg->vformat(fmt, args);
}


#ifndef __OPTIMIZE__

#ifdef __GNUC__
# include <cxxabi.h>
#endif


static int
excbreak()
{
	return 0;
}


void
mstl::bits::prepare_msg(mstl::exception& exc,
								char const* file,
								unsigned line,
								char const* func,
								char const* exc_type)
{
	try
	{
		ostringstream strm;
		strm.format(	"(func) %s\n(file) %s:%u\n(what) %s\n(type) %s\n",
							func, file, line, exc.what(), exc_type);

#ifndef NDEBUG
		if (!exc.backtrace().empty())
		{
			strm.write("\n", 1);
			strm.format("=== Backtrace ============================================\n");
			exc.backtrace().text_write(strm, 3);
			strm.format("==========================================================\n");
		}
#endif

		excbreak();
		exc.m_msg->assign(strm.str());
	}
	catch (...)
	{
	}
}


void
mstl::bits::prepare_exc(mstl::exception& exc,
								char const* file,
								unsigned line,
								char const* func,
								char const* exc_type_id)
{
#ifdef __GNUC__

	int	status;
	char*	exc_type = ::abi::__cxa_demangle(exc_type_id, 0, 0, &status);

	prepare_msg(exc, file, line, func, status == -1 ? 0 : exc_type);

	if (status != -1)
		free(exc_type);

#else

	prepare_msg(exc, file, line, func, exc_type_id);

#endif
}

#endif // __OPTIMIZE__

// vi:set ts=3 sw=3:
