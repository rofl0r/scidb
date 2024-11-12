// ======================================================================
// Author : $Author$
// Version: $Revision: 84 $
// Date   : $Date: 2011-07-18 18:02:11 +0000 (Mon, 18 Jul 2011) $
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
// Copyright: (C) 2009-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_utility.h"

namespace db {

inline unsigned Selector::size() const { return m_map.size(); }


inline
unsigned
Selector::map(unsigned index) const
{
	return index < m_map.size() ? m_map[index] : index;
}


inline
unsigned
Selector::lookup(unsigned index) const
{
	return index < m_list.size() ? m_list[index] : index;
}


#if HAVE_0X_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR

inline
Selector::Selector(Selector&& sel)
	:m_map(mstl::move(sel.m_map))
	,m_list(mstl::move(sel.m_list))
{
}


inline
Selector&
Selector::operator=(Selector&& sel)
{
	m_map = mstl::move(sel.m_map);
	m_list = mstl::move(sel.m_list);

	return *this;
}

#endif

} // namespace db

// vi:set ts=3 sw=3:
