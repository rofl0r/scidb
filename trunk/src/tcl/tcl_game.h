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

#ifndef _tcl_game_included
#define _tcl_game_included

#include "db_common.h"

extern "C" { struct Tcl_Obj; }

namespace db { class TagSet; }

namespace tcl {
namespace game {

typedef ::db::tag::ID Ratings[2];

int convertTags(	::db::TagSet& tags,
						Tcl_Obj* taglist,
						::db::tag::ID wrt = ::db::tag::ExtraTag,
						::db::tag::ID brt = ::db::tag::ExtraTag,
						Ratings const* ratings = 0);

} // namespace game
} // namespace tcl

#endif // _tcl_game_included

// vi:set ts=3 sw=3:
