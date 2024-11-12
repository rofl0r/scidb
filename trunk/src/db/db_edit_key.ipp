// ======================================================================
// Author : $Author$
// Version: $Revision: 20 $
// Date   : $Date: 2011-05-15 12:32:40 +0000 (Sun, 15 May 2011) $
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
// Copyright: (C) 2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

namespace db {
namespace edit {

inline bool Key::operator==(Key const& key) const { return m_id == key.m_id; }
inline bool Key::operator!=(Key const& key) const { return m_id != key.m_id; }
inline bool Key::operator> (Key const& key) const { return key < *this; }

inline char Key::prefix() const { return m_id[0]; }
inline mstl::string const& Key::id() const { return m_id; }

inline bool operator==(mstl::string const& lhs, Key const& rhs) { return lhs == rhs.id(); }
inline bool operator!=(mstl::string const& lhs, Key const& rhs) { return lhs != rhs.id(); }
inline bool operator==(Key const& lhs, mstl::string const& rhs) { return lhs.id() == rhs; }
inline bool operator!=(Key const& lhs, mstl::string const& rhs) { return lhs.id() != rhs; }

} // namespace edit
} // namespace db

// vi:set ts=3 sw=3:
