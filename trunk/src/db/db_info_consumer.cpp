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
// Copyright: (C) 2009-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "db_info_consumer.h"
#include "db_move_info_set.h"
#include "db_exception.h"

using namespace db;


InfoConsumer::InfoConsumer(format::Type srcFormat,
									mstl::string const& encoding,
									TagBits const& allowedTags,
									bool allowExtraTags,
									LanguageList const* languages,
									unsigned significantLanguages)
	:Consumer(srcFormat, encoding, allowedTags, allowExtraTags, languages, significantLanguages)
{
}


void
InfoConsumer::sendComment(Comment const&)
{
	M_ASSERT(!"shouldn't be called");
}


bool
InfoConsumer::preparseComment(mstl::string& comment)
{
	return m_moveInfoSet.extractFromComment(m_engines, comment);
}

// vi:set ts=3 sw=3:
