// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
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
// Copyright: (C) 2009-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_selector_included
#define _db_selector_included

#include "db_common.h"

#include "m_vector.h"

namespace db {

class Database;
class GameInfo;
class Filter;

class Selector
{
public:

	unsigned size() const;

	unsigned map(unsigned index) const;
	unsigned lookup(unsigned index) const;
	unsigned find(unsigned number) const;

	int findPlayer(Database const& db, mstl::string const& name) const;
	int findEvent(Database const& db, mstl::string const& name) const;
	int findAnnotator(Database const& db, mstl::string const& name) const;
	int searchPlayer(Database const& db, mstl::string const& name) const;
	int searchEvent(Database const& db, mstl::string const& name) const;
	int searchAnnotator(Database const& db, mstl::string const& name) const;

	void sort(	Database const& db,
					attribute::game::ID attr,
					order::ID order = order::Ascending,
					rating::Type ratingType = rating::Any);
	void sort(	Database const& db,
					attribute::player::ID attr,
					order::ID order = order::Ascending,
					rating::Type ratingType = rating::Any);
	void sort(	Database const& db,
					attribute::event::ID attr,
					order::ID order = order::Ascending);
	void sort(	Database const& db,
					attribute::annotator::ID attr,
					order::ID order = order::Ascending);

	void reverse(Database const& db);
	void update(Filter const& filter);
	void update(unsigned newSize);
	void update();

private:

	typedef mstl::vector<unsigned> Map;

	Map m_map;
	Map m_list;
};

} // namespace db

#include "db_selector.ipp"

#endif // _db_selector_included

// vi:set ts=3 sw=3:
