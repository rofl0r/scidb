// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
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

#ifndef _util_misc_included
#define _util_misc_included

#include "m_string.h"

namespace util {
namespace misc {

namespace file
{
	bool hasSuffix(mstl::string const& path);
	mstl::string basename(mstl::string const& path);
	mstl::string rootname(mstl::string const& path);
	mstl::string suffix(mstl::string const& path);
}

namespace time
{
	struct tm
	{
		unsigned sec;
		unsigned min;
		unsigned hour;
		unsigned mday;
		unsigned mon;
		unsigned year;
	};

	bool getCurrentTime(struct tm& result);
}

} // namespace misc
} // namespace util

#endif // _util_misc_included

// vi:set ts=3 sw=3:
