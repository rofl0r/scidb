// ======================================================================
// Author : $Author$
// Version: $Revision: 1080 $
// Date   : $Date: 2015-11-15 10:23:19 +0000 (Sun, 15 Nov 2015) $
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
// Copyright: (C) 2011-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_utility.h"

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

inline Key const& Key::emptyKey() { return m_emptyKey; }

inline bool Key::isValid() const { return isValid(m_id); }

#if HAVE_C11_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR

inline
Key::Key(Key&& key)
	:m_id(mstl::move(key.m_id))
{
}


inline
Key&
Key::operator=(Key&& key)
{
	m_id = mstl::move(key.m_id);
	return *this;
}

#endif

} // namespace edit
} // namespace db

// vi:set ts=3 sw=3:
