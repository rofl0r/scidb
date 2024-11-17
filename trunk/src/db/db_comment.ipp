// ======================================================================
// Author : $Author$
// Version: $Revision: 1453 $
// Date   : $Date: 2017-12-11 14:27:52 +0000 (Mon, 11 Dec 2017) $
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
// Copyright: (C) 2010-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_utility.h"

#include <string.h>

namespace db {

inline bool Comment::isXml() const							{ return ::strncmp(m_content, "<xml>", 5) == 0; }
inline bool Comment::isEmpty() const						{ return m_content.empty(); }
inline unsigned Comment::size() const						{ return m_content.size(); }
inline unsigned Comment::langFlags() const				{ return m_langFlags; }
inline mstl::string const& Comment::content() const	{ return m_content; }
inline Comment::operator mstl::string const& () const	{ return m_content; }

inline bool Comment::operator==(Comment const& comment) const { return m_content == comment.m_content; }
inline bool Comment::operator!=(Comment const& comment) const { return m_content != comment.m_content; }

#if HAVE_C11_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR

inline
Comment::Comment(Comment&& comment)
	:m_content(mstl::move(comment.m_content))
	,m_langFlags(comment.m_langFlags)
{
}


inline
Comment&
Comment::operator=(Comment&& comment)
{
	m_content = mstl::move(comment.m_content);
	m_langFlags = comment.m_langFlags;

	return *this;
}

#endif

} // naespace db

// vi:set ts=3 sw=3:
