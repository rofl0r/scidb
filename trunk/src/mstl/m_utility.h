// ======================================================================
// Author : $Author$
// Version: $Revision: 1453 $
// Date   : $Date: 2017-12-11 14:27:52 +0000 (Mon, 11 Dec 2017) $
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

#ifndef _mstl_utility_included
#define _mstl_utility_included

#include "m_types.h"
#include "m_type_traits.h"

namespace mstl {

namespace noncopyable_	// protection from unintended ADL
{
	class noncopyable
	{
	protected:

		noncopyable() {}
		~noncopyable() {}

#if HAVE_C11_EXPLICITLY_DEFAULTED_AND_DELETED_SPECIAL_MEMBER_FUNCTIONS

		noncopyable(noncopyable const&) = delete;
		noncopyable& operator=(noncopyable const&) = delete;

#else

	private:

		noncopyable(noncopyable const&);
		noncopyable& operator=(noncopyable const&);
#endif
	};
}

typedef noncopyable_::noncopyable noncopyable;

template <typename T> constexpr bool is_odd(T x);
template <typename T> constexpr bool is_even(T x);
template <typename T> constexpr bool is_pow_2(T x);
template <typename T> constexpr bool is_not_pow_2(T x);

template <typename T> constexpr T sqr(T x);
template <typename T> constexpr T abs(T x);
template <typename T> constexpr T min(T a, T b);
template <typename T> constexpr T max(T a, T b);
template <typename T> constexpr T min(T a, T b, T c);
template <typename T> constexpr T max(T a, T b, T c);
template <typename T> constexpr T signum(T x);

template <typename T> constexpr T div2(T x);
template <typename T> constexpr T div4(T x);
template <typename T> constexpr T div8(T x);
template <typename T> constexpr T mod2(T x);
template <typename T> constexpr T mod4(T x);
template <typename T> constexpr T mod8(T x);
template <typename T> constexpr T mul2(T x);
template <typename T> constexpr T mul4(T x);
template <typename T> constexpr T mul4(T x);

template <typename T> unsigned log2_floor(T x);
template <typename T> unsigned log2_ceil(T x);

template <typename T> constexpr bool is_between(T x, T a, T b);

template <typename T> void swap(T& a, T& b);
template <typename T, size_t N> void swap(T(& a)[N], T(& b)[N]);
template <typename T> constexpr T advance(T i, size_t offset);
template <typename T> T align(T n, size_t grain);
template <typename T> constexpr ptrdiff_t distance(T first, T last);

#if USE_C11_STANDARD

template<typename T> typename mstl::remove_reference<T>::type&& move(T&& t) noexcept;
template<typename T> T&& forward(typename mstl::remove_reference<T>::type& t) noexcept;

# define M_CXX_MOVE(x) 		::mstl::move(x)
# define M_CXX_FORWARD(c)	::mstl::forward(x)

#else

template<typename T> typename mstl::remove_reference<T>::type& move(T& t);
template<typename T> T& forward(typename mstl::remove_reference<T>::type& t);

# define M_CXX_MOVE(x)		(x)
# define M_CXX_FORWARD(x)	(x)

#endif

template <typename T> int compare(T const& lhs, T const& rhs);

int compare(int8_t lhs, int8_t rhs);
int compare(int16_t lhs, int16_t rhs);
int compare(int32_t lhs, int32_t rhs);
int compare(int64_t lhs, int64_t rhs);
int compare(uint8_t lhs, uint8_t rhs);
int compare(uint16_t lhs, uint16_t rhs);
int compare(uint32_t lhs, uint32_t rhs);
int compare(uint64_t lhs, uint64_t rhs);

int compare(bool lhs, bool rhs);

} // namespace mstl

#include "m_utility.ipp"

#endif // _mstl_utility_included

// vi:set ts=3 sw=3:
