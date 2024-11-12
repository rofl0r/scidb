// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
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

#ifndef _mstl_exception_included
#define _mstl_exception_included

#include "m_backtrace.h"

#include <stdarg.h>

#define M_RAISE(fmt,args...) M_THROW(::mstl::exception(fmt,##args))

#ifdef __OPTIMIZE__
# define M_THROW(exc) ({ throw exc; })
#else
# define M_THROW(exc) ({ ::mstl::bits::throw_exc(exc, __FILE__, __LINE__, __func__); })
#endif

#include "m_exception.ipp"

namespace mstl {

class string;

class exception
{
public:

	exception() throw();
	exception(string const& msg);
	explicit exception(char const* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
	exception(char const* fmt, va_list args);
	exception(exception const& exc);
	virtual ~exception () throw();

	virtual char const* what() const throw();
	::mstl::backtrace const& backtrace() const;

protected:

	void set_message(char const* fmt, va_list args);

private:

#ifndef __OPTIMIZE__
	friend void bits::prepare_msg(exception& exc, char const*, unsigned, char const*, char const*);
#endif

	exception& operator=(exception const&);

	string* m_msg;
	::mstl::backtrace	m_backtrace;
};

} // namespace mstl

#endif // _mstl_exception_included

// vi:set ts=3 sw=3:
