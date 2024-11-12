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

#ifndef _mstl_utility_included
#define _mstl_utility_included

#include <stddef.h>

namespace mstl {

namespace noncopyable_	// protection from unintended ADL
{
	class noncopyable
	{
		protected:

			noncopyable() {}
			~noncopyable() {}

		private:

			noncopyable( const noncopyable& );
			const noncopyable& operator=( const noncopyable& );
	};
}

typedef noncopyable_::noncopyable noncopyable;

template <typename T> bool is_odd(T x);
template <typename T> bool is_even(T x);
template <typename T> bool is_pow_2(T x);
template <typename T> bool is_not_pow_2(T x);

template <typename T> T sqr(T x);
template <typename T> T abs(T x);
template <typename T> T min(T a, T b);
template <typename T> T max(T a, T b);
template <typename T> T min(T a, T b, T c);
template <typename T> T max(T a, T b, T c);
template <typename T> T signum(T x);

template <typename T> T div2(T x);
template <typename T> T div4(T x);
template <typename T> T mod2(T x);
template <typename T> T mod4(T x);
template <typename T> T mul2(T x);
template <typename T> T mul4(T x);

template <typename T> unsigned log2_floor(T x);
template <typename T> unsigned log2_ceil(T x);

template <typename T> bool is_between(T x, T a, T b);

template <typename T> void swap(T& a, T& b);
template <typename T> T advance(T i, size_t offset);
template <typename T> T align(T n, size_t grain);
template <typename T> ptrdiff_t distance(T first, T last);

} // namespace mstl

#include "m_utility.ipp"

#endif // _mstl_utility_included

// vi:set ts=3 sw=3:
