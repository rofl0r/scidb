// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2010-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

// We have to hide typedef's in zlib.h!
#define Byte	_ZLIB_Byte
#define uInt	_ZLIB_uInt
#define uLong	_ZLIB_uLong
#define Bytef	_ZLIB_Bytef
#define charf	_ZLIB_charf
#define intf	_ZLIB_intf
#define uIntf	_ZLIB_uIntf
#define uLongf	_ZLIB_uLongf
#define voidpc	_ZLIB_voidpc
#define voidpf	_ZLIB_voidpf
#define voidp	_ZLIB_voidp

#include <zlib.h>

#undef _ZLIB_Byte
#undef _ZLIB_uInt
#undef _ZLIB_uLong
#undef _ZLIB_Bytef
#undef _ZLIB_charf
#undef _ZLIB_intf
#undef _ZLIB_uIntf
#undef _ZLIB_uLongf
#undef _ZLIB_voidpc
#undef _ZLIB_voidpf
#undef _ZLIB_voidp

namespace util {
namespace crc {

inline
uint32_t
compute(uint32_t crc, char const* bytes, unsigned len)
{
	return ::crc32(crc, reinterpret_cast<unsigned char const*>(bytes), len);
}


inline
uint32_t
compute(uint32_t crc, unsigned char const* bytes, unsigned len)
{
	return ::crc32(crc, bytes, len);
}


inline
uint32_t
combine(uint32_t crc1, uint32_t crc2, unsigned len2)
{
	return ::crc32_combine(crc1, crc2, len2);
}

} // namespace crc
} // namespace util

// vi:set ts=3 sw=3:
