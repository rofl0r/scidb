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

#ifndef _mstl_assert_included
#define _mstl_assert_included

#include "m_exception.h"

namespace mstl {

struct precondition_violation_exception : public exception
{
	precondition_violation_exception(char const* what);
};

struct assertion_failure_exception : public exception
{
	assertion_failure_exception(char const* what);
};

#define __M_CHECK(Exc,expr)	({ if (__builtin_expect(!(expr), 0)) M_THROW(Exc(#expr)); })

#ifndef NREQ
# define M_REQUIRE(expr)	__M_CHECK(::mstl::precondition_violation_exception, expr)
#else
# define M_REQUIRE(expr)
#endif

#ifndef NASSERT
# define M_ASSERT(expr)	__M_CHECK(::mstl::assertion_failure_exception, expr)
#else
# define M_ASSERT(expr)
#endif

#ifndef NDEBUG
# define M_DEBUG(expr)		__M_CHECK(::mstl::assertion_failure_exception, expr)
#else
# define M_DEBUG(expr)
#endif

} // namespace mstl

#endif // _mstl_assert_included

// vi:set ts=3 sw=3:
