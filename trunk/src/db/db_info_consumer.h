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

#include "db_consumer.h"
#include "db_engine_list.h"

#ifndef _db_info_consumer_included
#define _db_info_consumer_included

namespace db {

class MoveInfoSet;

class InfoConsumer : public Consumer
{
public:

	InfoConsumer(	format::Type srcFormat,
						mstl::string const& encoding,
						TagBits const& allowedTags,
						bool allowExtraTags,
						LanguageList const* languages = nullptr,
						unsigned significantLanguages = 0);

	void sendComment(Comment const& comment);
	bool preparseComment(mstl::string& comment) override;
};

} // namespace db

#endif // _db_info_consumer_included

// vi:set ts=3 sw=3:
