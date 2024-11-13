// ======================================================================
// Author : $Author$
// Version: $Revision: 569 $
// Date   : $Date: 2012-12-16 21:41:55 +0000 (Sun, 16 Dec 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _m_exception_included
#define _m_exception_included

namespace std { bool uncaught_exception() throw(); }

#ifndef __OPTIMIZE__

#ifdef __clang__
class type_info; // because of a cyclic bug in gcc headers
#endif

#include <typeinfo>

namespace mstl {

class exception;

namespace bits {

void
prepare_msg(exception& exc, char const* file, unsigned line, char const* func, char const* exc_type_id);

void
prepare_exc(exception& exc, char const* file, unsigned line, char const* func, char const* exc_type_id);

template <class Exc>
__attribute__((noreturn))
inline
static void
throw_exc(Exc const& exc, char const* file, int line, char const* func)
{
	Exc e(exc);
	prepare_exc(e, file, line, func, typeid(Exc).name());
	throw e;
}

} // namespace bits

} // namespace mstl

#endif // __OPTIMIZE__

namespace mstl { inline bool uncaught_exception() throw() { return ::std::uncaught_exception(); } }

#endif // _m_exception_included

// vi:set ts=3 sw=3:
