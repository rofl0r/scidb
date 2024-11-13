// ======================================================================
// Author : $Author$
// Version: $Revision: 193 $
// Date   : $Date: 2012-01-16 09:55:54 +0000 (Mon, 16 Jan 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _tcl_database_included
#define _tcl_database_included

#include "db_common.h"

#include "m_pair.h"

extern "C" { struct Tcl_Interp; }

namespace db { class GameInfo; }
namespace db { class NamebasePlayer; }
namespace db { class TagSet; }

namespace tcl
{
	namespace db
	{
		typedef mstl::pair< ::db::rating::Type, ::db::rating::Type> Ratings;

		int getGameInfo(	::db::GameInfo const& info,
								unsigned number,
								Ratings const& ratings,
								unsigned format);
		int getPlayerInfo(::db::NamebasePlayer const& player,
								Ratings const& ratings,
								bool info,
								bool idCard);
		int getTags(::db::TagSet const& tags, bool userSuppliedOnly);
	}
}

#endif // _tcl_database_included

// vi:set ts=3 sw=3:
