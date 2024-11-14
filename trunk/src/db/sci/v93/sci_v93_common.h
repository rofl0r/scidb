// ======================================================================
// Author : $Author$
// Version: $Revision: 880 $
// Date   : $Date: 2013-07-08 21:37:41 +0000 (Mon, 08 Jul 2013) $
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

#ifndef _sci_v93_common_included
#define _sci_v93_common_included

namespace db {
namespace sci {
namespace v93 {

namespace token
{
	enum
	{
		Mark				= 0,
		Nag				= 1,
		Comment			= 2,
		Start_Marker	= 3,
		End_Marker		= 4,
		Special_Move	= 5,
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

namespace flags
{
	enum
	{
		TextSection			= 1 << 15,
		TagSection			= 1 << 14,
		EngineSection		= 1 << 13,
		TimeTableSection	= 1 << 12,
	};
};

namespace maintenance
{
	enum
	{
		Compact = 1,
	};
}

enum { Block_Size = 131072 };

} // namespace v93
} // namespace sci
} // namespace db

#endif // _sci_v93_common_included

// vi:set ts=3 sw=3:
