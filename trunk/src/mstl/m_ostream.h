// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
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

#ifndef _mstl_ostream_included
#define _mstl_ostream_included

#include "m_ios.h"

#include <stdarg.h>

namespace mstl {

class string;

class ostream : virtual public ios_base
{
public:

	~ostream() throw();

	ostream& operator<<(char c);

	ostream& operator<<(int16_t n);
	ostream& operator<<(int32_t n);
	ostream& operator<<(int64_t n);

	ostream& operator<<(uint16_t n);
	ostream& operator<<(uint32_t n);
	ostream& operator<<(uint64_t n);

	ostream& operator<<(void const* p);

	ostream& put(char c);
	ostream& write(char const* buffer, size_t size);
	ostream& write(unsigned char const* buffer, size_t size);
	ostream& write(string const& str);
	ostream& writenl(string const& str);
	ostream& seek_and_write(size_t pos, unsigned char const* buffer, size_t size);
	ostream& flush() throw();

	int vformat(char const* fmt, va_list args);
	int format(char const* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));

	unsigned long tellp();
	ostream& seekp(size_t pos);
	ostream& seekp(long pos, seekdir dir);
};

} // namespace mstl

#include "m_ostream.ipp"

#endif // _mstl_ostream_included

// vi:set ts=3 sw=3:
