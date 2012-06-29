// ======================================================================
// Author : $Author$
// Version: $Revision: 367 $
// Date   : $Date: 2012-06-29 17:33:57 +0000 (Fri, 29 Jun 2012) $
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

namespace util {

inline NulString::NulString(char* s, size_t len) :m_pos(s + len), m_nulChar(*m_pos) { *m_pos = '\0'; }
inline NulString::~NulString() { *m_pos = m_nulChar; }
inline NulString::operator NulString::const_pointer() const { return c_str(); }

} // namespace util

// vi:set ts=3 sw=3:
