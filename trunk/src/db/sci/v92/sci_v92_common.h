// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
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

#ifndef _sci_v92_common_included
#define _sci_v92_common_included

namespace db {
namespace sci {
namespace v92 {

namespace token
{
	enum
	{
		Mark				= 0,
		Nag				= 1,
		Comment			= 2,
		Start_Marker	= 3,
		End_Marker		= 4,	Last = End_Marker,
	};
}

namespace comm
{
	enum
	{
		Ante		= 1 << 0,
		Post		= 1 << 1,
		Ante_Eng	= 1 << 2,
		Ante_Oth	= 1 << 3,
		Post_Eng	= 1 << 4,
		Post_Oth	= 1 << 5,
	};
}

enum { Block_Size = 131072 };

} // namespace v92
} // namespace sci
} // namespace db

#endif // _sci_v92_common_included

// vi:set ts=3 sw=3:
