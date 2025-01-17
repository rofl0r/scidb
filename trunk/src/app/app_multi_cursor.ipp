// ======================================================================
// Author : $Author$
// Version: $Revision: 1522 $
// Date   : $Date: 2018-09-16 13:56:42 +0000 (Sun, 16 Sep 2018) $
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
// Copyright: (C) 2012-2018 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "app_cursor.h"

#include "m_assert.h"

namespace app {

inline bool MultiCursor::isOpen() const			{ return m_leader->isOpen(); }
inline bool MultiCursor::isClosed() const			{ return m_leader->isClosed(); }
inline bool MultiCursor::isReadonly() const		{ return m_leader->isReadonly(); }
inline bool MultiCursor::isWritable() const		{ return m_leader->isWritable(); }
inline bool MultiCursor::isMemoryOnly() const	{ return m_leader->isMemoryOnly(); }
inline bool MultiCursor::isClipbase() const		{ return m_isClipbase; }
inline bool MultiCursor::isScratchbase() const	{ return m_isScratchbase; }

inline void MultiCursor::setClipbase()				{ m_isClipbase = true; }
inline void MultiCursor::setScratchbase()			{ m_isScratchbase = true; }

inline mstl::string const& MultiCursor::clipbaseName()		{ return m_clipbaseName; }
inline mstl::string const& MultiCursor::scratchbaseName()	{ return m_scratchbaseName; }

inline Cursor& MultiCursor::cursor() const				{ return *m_leader; }
inline mstl::string const& MultiCursor::name() const	{ return m_leader->name(); }
inline Application& MultiCursor::app() const				{ return m_app; }
inline db::MultiBase& MultiCursor::multiBase()			{ return *m_base; }

inline db::MultiBase const& MultiCursor::multiBase() const { return *m_base; }


inline
bool
MultiCursor::exists(unsigned variantIndex) const
{
	return !isClosed() && m_cursor[mapVariantIndex(variantIndex)];
}


inline
Cursor&
MultiCursor::cursor(unsigned variantIndex) const
{
	M_REQUIRE(exists(variantIndex));
	return *m_cursor[mapVariantIndex(variantIndex)];
}


inline
Cursor&
MultiCursor::cursor(db::variant::Type variant) const
{
	M_REQUIRE(variant == db::variant::Undetermined || db::variant::isMainVariant(variant));
	M_REQUIRE(variant == db::variant::Undetermined || exists(variant));

	if (variant == db::variant::Undetermined)
		return *m_leader;

	return *m_cursor[db::variant::toIndex(variant)];
}

} // namespace app

// vi:set ts=3 sw=3:
