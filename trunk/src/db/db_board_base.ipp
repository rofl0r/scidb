// ======================================================================
// Author : $Author$
// Version: $Revision: 1010 $
// Date   : $Date: 2014-10-18 15:12:33 +0000 (Sat, 18 Oct 2014) $
// Url    : $URL$
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
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

#include "m_bit_functions.h"

inline constexpr uint64_t db::board::set1Bit(int s)	{ return uint64_t(1) << s; }
inline constexpr uint64_t db::board::set2Bit(int s)	{ return uint64_t(3) << s; }

inline uint64_t db::board::shiftDown(uint64_t m)		{ return m >> 8; }
inline uint64_t db::board::shift2Down(uint64_t m)		{ return m >> 16; }
inline uint64_t db::board::shiftUp(uint64_t m)			{ return m << 8; }
inline uint64_t db::board::shift2Up(uint64_t m)			{ return m << 16; }
inline uint64_t db::board::shiftUpLeft(uint64_t m)		{ return (m << 7) & ~FyleMaskH; }
inline uint64_t db::board::shiftUpRight(uint64_t m)	{ return (m << 9) & ~FyleMaskA; }
inline uint64_t db::board::shiftDownLeft(uint64_t m)	{ return (m >> 9) & ~FyleMaskH; }
inline uint64_t db::board::shiftDownRight(uint64_t m)	{ return (m >> 7) & ~FyleMaskA; }
inline uint64_t db::board::shiftLeft(uint64_t m)		{ return (m >> 1) & ~FyleMaskH; }
inline uint64_t db::board::shiftRight(uint64_t m)		{ return (m << 1) & ~FyleMaskA; }
inline uint64_t db::board::shift2Left(uint64_t m)		{ return (m >> 2) & ~(FyleMaskG | FyleMaskH); }
inline uint64_t db::board::shift2Right(uint64_t m)		{ return (m << 2) & ~(FyleMaskA | FyleMaskB); }

inline int db::board::lsb(uint8_t n)	{ return mstl::bf::lsb_index(n); }
inline int db::board::lsb(uint64_t n)	{ return mstl::bf::lsb_index(n); }

inline int db::board::msb(uint8_t n)	{ return mstl::bf::msb_index(n); }
inline int db::board::msb(uint64_t n)	{ return mstl::bf::msb_index(n); }

template <typename T> inline int db::board::count(T n) { return mstl::bf::count_bits(n); }


inline
int
db::board::lsbClear(uint8_t& n)
{
	int r = lsb(n);
	n &= ~(1 << r);
	return r;
}


inline
int
db::board::lsbClear(uint64_t& n)
{
	int r = lsb(n);
	n &= ~(uint64_t(1) << r);
	return r;
}


inline
int
db::board::msbClear(uint8_t& n)
{
	int r = msb(n);
	n &= ~(1 << r);
	return r;
}


inline
int
db::board::msbClear(uint64_t& n)
{
	int r = msb(n);
	n &= ~(uint64_t(1) << r);
	return r;
}


inline
uint64_t
db::board::allSquaresBetween(Square from, Square to)
{
	M_REQUIRE(from <= sq::h8);
	M_REQUIRE(to <= sq::h8);

	return Obstructed[from][to];
}

// vi:set ts=3 sw=3:
