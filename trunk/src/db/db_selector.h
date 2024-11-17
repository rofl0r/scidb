// ======================================================================
// Author : $Author$
// Version: $Revision: 1383 $
// Date   : $Date: 2017-08-06 17:18:29 +0000 (Sun, 06 Aug 2017) $
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

#ifndef _db_selector_included
#define _db_selector_included

#include "db_common.h"

#include "m_vector.h"

namespace util { class Pattern; }

namespace db {

class Database;
class GameInfo;
class Namebase;
class Filter;

class Selector
{
public:

	Selector();

#if HAVE_OX_EXPLICITLY_DEFAULTED_AND_DELETED_SPECIAL_MEMBER_FUNCTIONS
	Selector(Selector const&) = default;
	Selector& operator=(Selector const&) = default;
#endif

#if HAVE_0X_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR
	Selector(Selector&& sel);
	Selector& operator=(Selector&& sel);
#endif

	bool isUnsorted() const;
	bool isUnfiltered() const;

	// returns the size of the sort index, is zero if unsorted
	unsigned size() const;

	// map from sort index to real index
	unsigned map(unsigned index) const;
	// map from filtered sort index to real index
	unsigned lookup(unsigned index) const;
	// map from real index to filtered sort index
	unsigned find(unsigned number) const;

	// returns the filtered sort index, or -1 if not found
	int findPlayer(Database const& db, mstl::string const& name) const;
	int findEvent(Database const& db, mstl::string const& name) const;
	int findSite(Database const& db, mstl::string const& name) const;
	int findAnnotator(Database const& db, mstl::string const& name) const;
	int findPosition(Database const& db, uint16_t idn) const;

	// 'startIndex' will be given as filtered sort index
	// returns the filtered sort index, or -1 if not found
	int searchPlayer(Database const& db, util::Pattern const& pattern, unsigned startIndex = 0) const;
	int searchEvent(Database const& db, util::Pattern const& pattern, unsigned startIndex = 0) const;
	int searchSite(Database const& db, util::Pattern const& pattern, unsigned startIndex = 0) const;
	int searchAnnotator(Database const& db, util::Pattern const& pattern, unsigned startIndex = 0) const;

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
					attribute::site::ID attr,
					order::ID order = order::Ascending);
	void sort(	Database const& db,
					attribute::annotator::ID attr,
					order::ID order = order::Ascending);
	void sort(	Database const& db,
					attribute::position::ID attr,
					order::ID order = order::Ascending);

	void reverse(Database const& db);
	void reset(Database const& db);
	void swap(Selector& selector);
	void update(Filter const& filter);
	void update(unsigned newSize);
	void update();

private:

	typedef mstl::vector<unsigned> Map;
	typedef int (*Compar)(unsigned, unsigned, Database const&);

	void reserve(Database const& db, unsigned numEntries);
	void finish(Database const& db, unsigned numEntries, order::ID order, Compar compFunc);
	void reset();

	int search(Namebase const& namebase, util::Pattern const& pattern, unsigned startIndex) const;
	int search(	Namebase const& namebase,
					mstl::string const& prefix,
					util::Pattern const& pattern,
					unsigned startIndex) const;
	int find(Namebase const& namebase, mstl::string const& name) const;

	Map		m_map;
	Map		m_lookup;
	Map		m_find;
	unsigned	m_sizeOfMap;
	unsigned	m_sizeOfList;
};

} // namespace db

#include "db_selector.ipp"

#endif // _db_selector_included

// vi:set ts=3 sw=3:
