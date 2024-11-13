// ======================================================================
// Author : $Author$
// Version: $Revision: 223 $
// Date   : $Date: 2012-01-31 18:16:26 +0000 (Tue, 31 Jan 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_string.h"

inline bool sys::utf8::isFirst(char c)	{ return (c & 0xc0) != 0x80; }
inline bool sys::utf8::isTail(char c)	{ return (c & 0xc0) == 0x80; }

inline bool sys::utf8::validate(mstl::string const& str) { return validate(str, str.size()); }

// vi:set ts=3 sw=3:
