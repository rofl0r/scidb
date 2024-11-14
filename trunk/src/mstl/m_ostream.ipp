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

namespace mstl {

inline
ostream&
ostream::operator<<(char c)
{
	return put(c);
}


inline
ostream&
ostream::write(unsigned char const* buffer, size_t size)
{
	return write(reinterpret_cast<char const*>(buffer), size);
}


inline
ostream&
ostream::put(char c)
{
	return write(&c, 1);
}

} // namespace mstl

// vi:set ts=3 sw=3:
