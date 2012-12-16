// ======================================================================
// Author : $Author$
// Version: $Revision: 569 $
// Date   : $Date: 2012-12-16 21:41:55 +0000 (Sun, 16 Dec 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_bitfield.h"

# if __WORDSIZE == 32 || !__GNUC_PREREQ(4,4)

uint128_t const mstl::bitfield<uint128_t>::m_zero(0u);
uint128_t const mstl::bitfield<uint128_t>::m_one(1u);
uint128_t const mstl::bitfield<uint128_t>::m_inverse(~uint64_t(0), ~uint64_t(0));

#endif // __WORDSIZE == 32

// vi:set ts=3 sw=3:
