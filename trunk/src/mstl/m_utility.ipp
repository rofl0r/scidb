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

#include "m_type_traits.h"
#include "m_limits.h"
#include "m_bit_functions.h"
#include "m_assert.h"

namespace mstl {
namespace bits {

template <size_t N> struct signed_arithmetic;

template <>
struct signed_arithmetic<1>
{
	template <typename T> inline static constexpr T div2(T x) { return x/2; }
	template <typename T> inline static constexpr T div4(T x) { return x/4; }
	template <typename T> inline static constexpr T div8(T x) { return x/8; }
	template <typename T> inline static constexpr T mod2(T x) { return x%2; }
	template <typename T> inline static constexpr T mod4(T x) { return x%4; }
	template <typename T> inline static constexpr T mod8(T x) { return x%8; }
	template <typename T> inline static constexpr T mul2(T x) { return x*2; }
	template <typename T> inline static constexpr T mul4(T x) { return x*4; }
	template <typename T> inline static constexpr T mul8(T x) { return x*8; }

	template <typename T>
	inline
	static T
	abs(T x)
	{
#ifdef WITHOUT_BRANCHING
		return a - ((a + a) & (a >> (sizeof(T)*8 - 1)));
#else
		return x < 0 ? -x : x;
#endif
	}
};

template <>
struct signed_arithmetic<0>
{
	template <typename T> inline static T constexpr div2(T x) { return x >> 1; }
	template <typename T> inline static T constexpr div4(T x) { return x >> 2; }
	template <typename T> inline static T constexpr div8(T x) { return x >> 3; }
	template <typename T> inline static T constexpr mod2(T x) { return x & 1; }
	template <typename T> inline static T constexpr mod4(T x) { return x & 3; }
	template <typename T> inline static T constexpr mod8(T x) { return x & 7; }
	template <typename T> inline static T constexpr mul2(T x) { return x << 1; }
	template <typename T> inline static T constexpr mul4(T x) { return x << 2; }
	template <typename T> inline static T constexpr mul8(T x) { return x << 3; }
	template <typename T> inline static T constexpr abs(T x)  { return x; }
};

} // namespace bits


#if USE_C11_STANDARD

template<typename T>
inline
typename mstl::remove_reference<T>::type&&
move(T&& t) noexcept
{
	return static_cast<typename mstl::remove_reference<T>::type&&>(t);
}


template<typename T>
inline
T&&
forward(typename mstl::remove_reference<T>::type& t) noexcept
{
	return static_cast<T&&>(t);
}

#else

template<typename T> typename mstl::remove_reference<T>::type& move(T& t)		{ return t; }
template<typename T> T& forward(typename mstl::remove_reference<T>::type& t)	{ return t; }

#endif


template <typename T>
inline
constexpr T
min(T a, T b)
{
#ifdef WITHOUT_BRANCHING
	b -= a;
	return a = b & (b >> (sizeof(T)*8 - 1));
#else
	return a < b ? a : b;
#endif
}


template <typename T>
inline
constexpr T
max(T a, T b)
{
#ifdef WITHOUT_BRANCHING
	b = a - b;
	return a - b & (b >> (sizeof(T)*8 - 1));
#else
	return a < b ? b : a;
#endif
}


template <typename T>
inline
constexpr T
min(T a, T b, T c)
{
	return min(a, min(b, c));
}


template <typename T>
inline
constexpr T
max(T a, T b, T c)
{
	return max(a, max(b, c));
}


template <typename T> inline constexpr T sqr(T x) { return x*x; }


template <typename T> inline constexpr T advance(T i, size_t offset)			{ return i + offset; }
template <typename T> inline constexpr ptrdiff_t distance(T first, T last)	{ return last - first; }


template <typename T>
inline
void
swap(T& a, T& b)
{
	T x = M_CXX_MOVE(a);
	a = M_CXX_MOVE(b);
	b = M_CXX_MOVE(x);
}


template <typename T, size_t N>
inline
void
swap(T(& a)[N], T(& b)[N])
{
	for (size_t n = 0; n < N; ++n)
		swap(a[n], b[n]);
}


inline
void*
advance(void* i, size_t offset)
{
	return static_cast<char*>(i) + offset;
}


template <>
inline
constexpr ptrdiff_t
distance(void* first, void* last)
{
	return static_cast<char*>(last) - static_cast<char*>(first);
}


template <typename T>
inline
T
align(T n, size_t grain)
{
    T r = n % grain;
    return r ? n + (grain - r) : n;
}


template <typename T>
inline
constexpr bool
is_odd(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return bits::signed_arithmetic<numeric_limits<T>::is_signed>::mod2(x) != 0;
}


template <typename T>
inline
constexpr bool
is_even(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return bits::signed_arithmetic<numeric_limits<T>::is_signed>::mod2(x) == 0;
}


template <typename T>
inline
constexpr bool
is_pow_2(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return x && !(x & (x - 1));
}


template <typename T>
inline
constexpr bool
is_not_pow_2(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return x & (x - 1);
}


template <typename T>
inline
constexpr T
abs(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return bits::signed_arithmetic<numeric_limits<T>::is_signed>::abs(x);
}


template <typename T>
inline
constexpr T
signum(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return (0 < x) - (x < 0);
}


template <typename T>
inline
constexpr T
div2(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return bits::signed_arithmetic<numeric_limits<T>::is_signed>::div2(x);
}


template <typename T>
inline
constexpr T
div4(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return bits::signed_arithmetic<numeric_limits<T>::is_signed>::div4(x);
}


template <typename T>
inline
constexpr T
div8(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return bits::signed_arithmetic<numeric_limits<T>::is_signed>::div8(x);
}


template <typename T>
inline
constexpr T
mod2(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return bits::signed_arithmetic<numeric_limits<T>::is_signed>::mod2(x);
}


template <typename T>
inline
constexpr T
mod4(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return bits::signed_arithmetic<numeric_limits<T>::is_signed>::mod4(x);
}


template <typename T>
inline
constexpr T
mod8(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return bits::signed_arithmetic<numeric_limits<T>::is_signed>::mod8(x);
}


template <typename T>
inline
constexpr T
mul2(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return bits::signed_arithmetic<numeric_limits<T>::is_signed>::mul2(x);
}


template <typename T>
inline
constexpr T
mul4(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return bits::signed_arithmetic<numeric_limits<T>::is_signed>::mul4(x);
}


template <typename T>
inline
constexpr T
mul8(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	return bits::signed_arithmetic<numeric_limits<T>::is_signed>::mul8(x);
}


template <typename T>
inline
unsigned
log2_floor(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	M_REQUIRE(x > 0);

	return bf::msb_index(x);
}


template <typename T>
inline
unsigned
log2_ceil(T x)
{
	static_assert(numeric_limits<T>::is_integer, "template parameter not integer");
	M_REQUIRE(x > 0);

	T n = bf::msb_index(x);
	return is_not_pow_2(x) ? n + 1 : n;
}


template <typename T>
inline
constexpr bool
is_between(T x, T a, T b)
{
	return a <= x && x <= b;
}


template <typename T>
inline
int
compare(T const& lhs, T const& rhs)
{
	if (lhs < rhs) return -1;
	if (rhs < lhs) return +1;
	return 0;
}


inline int compare(int8_t lhs, int8_t rhs)		{ return lhs - rhs; }
inline int compare(int16_t lhs, int16_t rhs)		{ return lhs - rhs; }
inline int compare(int32_t lhs, int32_t rhs)		{ return lhs - rhs; }
inline int compare(uint8_t lhs, uint8_t rhs)		{ return int(lhs) - int(rhs); }
inline int compare(uint16_t lhs, uint16_t rhs)	{ return int(lhs) - int(rhs); }
inline int compare(uint64_t lhs, uint64_t rhs)	{ return lhs < rhs ? -1 : (lhs == rhs ? 0 : +1); }

#if UINTPTR_MAX > UINT_MAX
inline int compare(uint32_t lhs, uint32_t rhs)	{ return int(lhs) - int(rhs); }
#else
inline int compare(uint32_t lhs, uint32_t rhs)	{ return lhs < rhs ? -1 : (lhs == rhs ? 0 : +1); }
#endif

#if UINTPTR_MAX == UINT_MAX
inline int compare(int64_t lhs, int64_t rhs)		{ return lhs - rhs; }
#else
inline int compare(int64_t lhs, int64_t rhs)		{ return lhs < rhs ? -1 : (lhs == rhs ? 0 : +1); }
#endif

inline int compare(bool lhs, bool rhs)				{ return int(lhs) - int(rhs); }

} // namespace mstl

// vi:set ts=3 sw=3:
