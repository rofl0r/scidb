// ======================================================================
// Author : $Author$
// Version: $Revision: 1279 $
// Date   : $Date: 2017-07-09 09:41:39 +0000 (Sun, 09 Jul 2017) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _tcl_exception_included
#define _tcl_exception_included

#include "u_exception.h"

#define TCL_RAISE(fmt,args...) M_THROW(::tcl::Exception(fmt,##args))

extern "C" { struct Tcl_Obj; }

namespace tcl {

class Exception : public util::Exception
{
public:

	// structors
	explicit Exception(char const* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
	Exception(unsigned numArgs, Tcl_Obj* const objv[], char const* usage = nullptr);
	Exception(util::Exception& exc);
	~Exception() throw();
};


class Error : public util::Exception
{
public:

	Error();
	Error(util::Exception& exc);
	~Error() throw();
};


class InterruptException : public util::BasicException
{
public:

	InterruptException();
	InterruptException(unsigned count);

	int count() const;

private:

	int m_count;
};

} // namespace tcl

#endif // _tcl_exception_included

// vi:set ts=3 sw=3:
