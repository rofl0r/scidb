// ======================================================================
// Author : $Author$
// Version: $Revision: 1372 $
// Date   : $Date: 2017-08-04 17:56:11 +0000 (Fri, 04 Aug 2017) $
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
// Copyright: (C) 2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _app_player_dictionary_included
#define _app_player_dictionary_included

#include "db_common.h"

#include "m_bitset.h"
#include "m_vector.h"

namespace util { class Pattern; }

namespace db { class Player; }
namespace db { class Query; }
namespace db { class Date; }

namespace app {

class PlayerDictionary
{
public:

	enum Mode
	{
		EnginesOnly,
		PlayersOnly,
		All,
	};

	enum Operator
	{
		Null,
		Not,
		And,
		Or,
		Reset,
		Remove,
	};

	enum Attribute
	{
		Name,
		FideID,
		DsbId,
		EcfId,
		IccfId,
		Type,
		Sex,
		DateOfBirth,
		DateOfDeath,
		Federation,
		Titles,
		NativeCountry,
		Frequency,
		LatestRating,
	};

	PlayerDictionary(Mode mode);

	unsigned count() const;

	::db::Player const& getPlayer(unsigned number) const;
	int search(util::Pattern const& namePattern, unsigned startIndex = 0) const;

	void finishOperation();

	void sort(	Attribute attr,
					::db::order::ID order,
					::db::rating::Type ratingType = ::db::rating::Elo);
	void reverseOrder();
	void cancelSort();

	void resetFilter();
	void negateFilter();
	void filterLetter(char letter);
	void filterName(Operator op, util::Pattern const& pattern);
	void filterFederation(Operator op, ::db::country::Code country);
	void filterNativeCountry(Operator op, ::db::country::Code country);
	void filterType(Operator op, ::db::species::ID type);
	void filterSex(Operator op, ::db::sex::ID sex);
	void filterTitles(Operator op, unsigned titles, uint16_t minYear, uint16_t maxYear);
	void filterOrganization(Operator op, ::db::organization::ID organization);
	void filterScore(Operator op, ::db::rating::Type rating, uint16_t min, uint16_t max);
	void filterBirthYear(Operator op, uint16_t minYear, uint16_t maxYear);
	void filterDeathYear(Operator op, uint16_t minYear, uint16_t maxYear);
	void filterFrequency(Operator op, unsigned minFrequency, unsigned maxFrequency);

private:

	typedef mstl::bitset Filter;
	typedef mstl::vector<unsigned> Selector;
	typedef void (mstl::bitset::*Setter)(mstl::bitset::size_type);

	bool prepareForOp(Operator op, Setter& setter);

	Filter	m_baseFilter;
	Filter	m_nameFilter;
	Filter	m_attrFilter;
	Filter	m_filter;
	Selector	m_selector;
	Selector	m_map;
	unsigned	m_count;
};

} // namespace app

#include "app_player_dictionary.ipp"

#endif // _app_player_dictionary_included

// vi:set ts=3 sw=3:
