// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
// Url    : $URL$
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

#ifndef _util_misc_included
#define _util_misc_included

#include "m_string.h"

namespace util {
namespace misc {

namespace file
{
	bool hasSuffix(mstl::string const& path);
	mstl::string dirname(mstl::string const& path);
	mstl::string basename(mstl::string const& path);
	mstl::string rootname(mstl::string const& path);
	mstl::string suffix(mstl::string const& path);

	char pathSeparator();
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
