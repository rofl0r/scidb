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

#include "m_utility.h"
#include "m_assert.h"

namespace util {

inline
unsigned
BitStream::bitsLeft() const
{
	return mstl::mul8(m_size) + m_bitsLeft;
}


inline
void
BitStream::fetchBits(unsigned n)
{
	if (m_bitsLeft < n)
		getBits();
}


inline
BitStream::Byte
BitStream::peek(unsigned n)
{
	M_REQUIRE(n <= U_BITS_OF(Byte));
	M_REQUIRE(n <= bitsLeft());

	fetchBits(n);

	return (m_bits >> (m_bitsLeft - n)) & ((1 << n) - 1);
}


inline
BitStream::Byte
BitStream::next(unsigned n)
{
	M_REQUIRE(n <= U_BITS_OF(Byte));
	M_REQUIRE(n <= bitsLeft());

	fetchBits(n);

	return (m_bits >> (m_bitsLeft -= n)) & ((1 << n) - 1);
}

#if HAVE_C11_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR

inline
BitStream::BitStream(BitStream&& strm)
	:m_buffer(strm.m_buffer)
	,m_size(strm.m_size)
	,m_bits(strm.m_bits)
	,m_bitsLeft(strm.m_bitsLeft)
{
	strm.m_buffer = 0;
}


inline
BitStream&
BitStream::operator=(BitStream&& strm)
{
	mstl::swap(m_buffer, strm.m_buffer);
	m_size = strm.m_size;
	m_bits = strm.m_bits;
	m_bitsLeft = strm.m_bitsLeft;

	return *this;
}

#endif

} // namespace util

// vi:set ts=3 sw=3:
