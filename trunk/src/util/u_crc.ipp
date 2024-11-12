// ======================================================================
// Author : $Author$
// Version: $Revision: 33 $
// Date   : $Date: 2011-05-29 12:27:45 +0000 (Sun, 29 May 2011) $
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

#undef Byte
#undef uInt
#undef uLong
#undef Bytef
#undef charf
#undef intf
#undef uIntf
#undef uLongf
#undef voidpc
#undef voidpf
#undef voidp

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
