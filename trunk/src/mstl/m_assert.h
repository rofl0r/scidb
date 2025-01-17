// ======================================================================
// Author : $Author$
// Version: $Revision: 648 $
// Date   : $Date: 2013-02-05 21:52:03 +0000 (Tue, 05 Feb 2013) $
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

#ifdef NREQ

# define M_REQUIRE(expr)
# define M_ASSERT(expr)
# define M_DEBUG(expr)
# define M_TEST(expr)

#else

#define M_CHECK(Exc,expr) ({ if (__builtin_expect(!(expr), 0)) M_THROW(Exc(#expr)); })

# define M_REQUIRE(expr)	M_CHECK(::mstl::precondition_violation_exception, expr)
# define M_ASSERT(expr)		M_CHECK(::mstl::assertion_failure_exception, expr)
# define M_DEBUG(expr)		M_CHECK(::mstl::assertion_failure_exception, expr)
# define M_TEST(expr)		expr;

#endif

} // namespace mstl

#endif // _mstl_assert_included

// vi:set ts=3 sw=3:
