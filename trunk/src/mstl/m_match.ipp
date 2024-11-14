// ======================================================================
// Author : $Author$
// Version: $Revision: 719 $
// Date   : $Date: 2013-04-19 16:40:59 +0000 (Fri, 19 Apr 2013) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

namespace mstl {

inline bool pattern::is_ascii() const		{ return m_is_ascii; }
inline bool pattern::is_utf8() const		{ return !m_is_ascii; }
inline bool pattern::match_none() const	{ return m_pattern.empty(); }
inline bool pattern::match_any() const		{ return m_pattern.size() == 1 && m_pattern.back() == '*'; }

} // namespace mstl

// vi:set ts=3 sw=3:
