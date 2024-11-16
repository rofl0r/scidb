// ======================================================================
// Author : $Author$
// Version: $Revision: 1339 $
// Date   : $Date: 2017-07-31 19:09:29 +0000 (Mon, 31 Jul 2017) $
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

#define USE_UINT128

#ifdef USE_UINT128
# include "m_uint128_t.h"
# else
# include "m_types.h"
#endif

namespace mstl {
namespace bf {
namespace bits {

template <int> struct remove_sign;

template <> struct remove_sign< 1> { typedef uint8_t  type; };
template <> struct remove_sign< 2> { typedef uint16_t type; };
template <> struct remove_sign< 4> { typedef uint32_t type; };
template <> struct remove_sign< 8> { typedef uint64_t type; };
#ifdef USE_UINT128
template <> struct remove_sign<16> { typedef uint128_t type; };
#endif

#if !__GNUC_PREREQ(3,4)
# error "at least compiler version 3.4 required"
#endif

inline unsigned clz(unsigned short x)		{ return __builtin_clz(x); }
inline unsigned clz(unsigned int x)			{ return __builtin_clz(x); }
inline unsigned clz(unsigned long x)		{ return __builtin_clzl(x); }
inline unsigned clz(unsigned long long x)	{ return __builtin_clzll(x); }

inline unsigned ctz(unsigned short x)		{ return __builtin_ctz(x); }
inline unsigned ctz(unsigned int x)			{ return __builtin_ctz(x); }
inline unsigned ctz(unsigned long x)		{ return __builtin_ctzl(x); }
#if defined(VALGRIND) && __WORDSIZE == 32
unsigned ctz(unsigned long long x);
#else
inline unsigned ctz(unsigned long long x)	{ return __builtin_ctzll(x); }
#endif

inline unsigned pc(unsigned short x)		{ return __builtin_popcount(x); }
inline unsigned pc(unsigned int x)			{ return __builtin_popcount(x); }
inline unsigned pc(unsigned long x)			{ return __builtin_popcountl(x); }
inline unsigned pc(unsigned long long x)	{ return __builtin_popcountll(x); }

inline unsigned msb(uint8_t x)				{ return  7 - clz(unsigned(x)); }
inline unsigned msb(uint16_t x)				{ return 15 - clz(unsigned(x)); }
inline unsigned msb(uint32_t x)				{ return 31 - clz(x); }
inline unsigned msb(uint64_t x)				{ return 63 - clz(x); }

inline unsigned lsb(uint8_t x)				{ return ctz(unsigned(x)); }
inline unsigned lsb(uint16_t x)				{ return ctz(unsigned(x)); }
inline unsigned lsb(uint32_t x)				{ return ctz(x); }
inline unsigned lsb(uint64_t x)				{ return ctz(x); }

inline unsigned popcount(uint8_t x)			{ return pc(unsigned(x)); }
inline unsigned popcount(uint16_t x)		{ return pc(unsigned(x)); }

inline unsigned
popcount(uint32_t x)
{
#if 1

	x -=  (x >> 1) & 0x55555555u;
	x  = ((x >> 2) & 0x33333333u) + (x & 0x33333333u);
	x  = ((x >> 4) + x) & 0x0f0f0f0fu;
	return (x*0x01010101u) >> 24;

#else

	// the builtin function of GCC is quite slow
	return pc(x);

#endif
}

inline unsigned
popcount(uint64_t x)
{
#if __WORDSIZE == 32

	return popcount(uint32_t(x)) + popcount(uint32_t(x >> 32));

#elif __WORDSIZE == 64

	x -=  (x >> 1) & UINT64_C(0x5555555555555555);
	x  = ((x >> 2) & UINT64_C(0x3333333333333333)) + (x & UINT64_C(0x3333333333333333));
	x  = ((x >> 4) + x) & UINT64_C(0x0f0f0f0f0f0f0f0f);
	return (x*UINT64_C(0x0101010101010101)) >> 56;

#else

	return pc(x);

#endif
}

#ifdef USE_UINT128
# if __WORDSIZE == 32 || !__GNUC_PREREQ(4,4)

inline
unsigned
msb(uint128_t const& x)
{
	return x.hi() ? 127 - clz(x.hi()) : 63 - clz(x.lo());
}

inline
unsigned
lsb(uint128_t const& x)
{
	return x.lo() ? ctz(x.lo()) : 64 + ctz(x.hi());
}

inline
unsigned
popcount(uint128_t const& x)
{
	return popcount(x.lo()) + popcount(x.hi());
}

# else // if __WORDSIZE == 64

inline
unsigned
msb(uint128_t x)
{
	uint64_t hi = x >> 64;
	return hi ? 127 - clz(hi) : 63 - clz(uint64_t(x));
}

inline
unsigned
lsb(uint128_t x)
{
	return uint64_t(x) ? ctz(uint64_t(x)) : 64 + ctz(uint64_t(x >> 64));
}

inline
unsigned
popcount(uint128_t x)
{
	return popcount(uint64_t(x)) + popcount(uint64_t(x >> 64));
}

# endif // __WORDSIZE
#endif

#define BF_USE_FLIP_ARR

#ifdef BF_USE_FLIP_ARR
extern uint8_t Flip[256];
#endif

inline
uint8_t
reverse(uint8_t x)
{
#ifdef BF_USE_FLIP_ARR
	return Flip[x];
#else
	x =		((x >> 1) & 0x55) | ((x << 1) & 0xAA);
	x =		((x >> 2) & 0x33) | ((x << 2) & 0xCC);
	return	((x >> 4) & 0x0F) | ((x << 4) & 0xF0);
#endif
}

inline
uint16_t
reverse(uint16_t x)
{
#ifdef BF_USE_FLIP_ARR
	return Flip[x >> 8] | (uint16_t(Flip[x]) << 8);
#else
	x =		((x >> 1) & 0x5555) | ((x << 1) & 0xAAAA);
	x =		((x >> 2) & 0x3333) | ((x << 2) & 0xCCCC);
	x =		((x >> 4) & 0x0F0F) | ((x << 4) & 0xF0F0);
	return	((x >> 8) & 0x00FF) | ((x << 8) & 0xFF00);
#endif
}

extern uint32_t reverse(uint32_t x);
extern uint64_t reverse(uint64_t x);

#ifdef USE_UINT128
# if __WORDSIZE == 32 || !__GNUC_PREREQ(4,4)

inline
uint128_t
reverse(uint128_t const& x)
{
	return uint128_t(reverse(x.lo()), reverse(x.hi()));
}

# else // if __WORDSIZE == 64

inline
uint128_t
reverse(uint128_t x)
{
	return uint128_t(reverse(uint64_t(x))) << 64 | uint128_t(reverse(uint64_t(x >> 64)));
}

# endif // __WORDSIZE
#endif

} // namespace bits

/// \brief Computes the number of 1 bits in a number.
/// \ingroup ConditionAlgorithms
template <typename T>
unsigned
count_bits(T x)
{
	return bits::popcount(static_cast<typename bits::remove_sign<sizeof(T)>::type>(x));
}


/// \brief Returns whether the number of 1 bits is greater than 1.
/// \ingroup BitAlgorithm
template <typename T>
inline
constexpr bool
more_than_one(T x)
{
	return x & (x - 1);
}


/// \brief Returns whether the number of 1 bits is less than or equal to 1.
/// \ingroup BitAlgorithm
template <typename T>
inline
constexpr bool
at_most_one(T x)
{
	return !more_than_one(x);
}


/// \brief Returns whether the number of 1 bits is equal to 1.
/// \ingroup BitAlgorithm
template <typename T>
inline
constexpr bool
exactly_one(T x)
{
	return x && at_most_one(x);
}


/// \brief Computes the index of the most significant bit in a number.
/// \ingroup ConditionAlgorithms
template <typename T>
unsigned
msb_index(T x)
{
	return bits::msb(static_cast<typename bits::remove_sign<sizeof(T)>::type>(x));
}


/// \brief Computes the index of the least significant bit in a number.
/// \ingroup ConditionAlgorithms
template <typename T>
unsigned
lsb_index(T x)
{
	return bits::lsb(static_cast<typename bits::remove_sign<sizeof(T)>::type>(x));
}


template <typename T>
T
reverse(T x)
{
	return bits::reverse(static_cast<typename bits::remove_sign<sizeof(T)>::type>(x));
}


template <typename T>
T
rotate_left(T x, unsigned shift)
{
	// NOTE:
	// Due to <http://chsc.wordpress.com/2010/01/13/compiler-optimization>
	// the GNU compiler knows that the C code only rotates the bits and that
	// this can be done with the x86 rol and ror instructions.
	// Also see <http://gcc.gnu.org/bugzilla/show_bug.cgi?id=17886>.
	return (x << shift) | (x >> (sizeof(T)*8 - shift));
}


template <typename T>
T
rotate_right(T x, unsigned shift)
{
	// NOTE:
	// Due to <http://chsc.wordpress.com/2010/01/13/compiler-optimization>
	// the GNU compiler knows that the C code only rotates the bits and that
	// this can be done with the x86 rol and ror instructions.
	// Also see <http://gcc.gnu.org/bugzilla/show_bug.cgi?id=17886>.
	return (x >> shift) | (x << (sizeof(T)*8 - shift));
}

} // namespace bf
} // namespace mstl

// vi:set ts=3 sw=3:
