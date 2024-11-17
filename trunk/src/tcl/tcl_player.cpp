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
// Copyright: (C) 2012-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "tcl_player.h"
#include "tcl_base.h"

#include "app_player_dictionary.h"

#include "db_player.h"
#include "db_namebase_entry.h"

#include "u_match.h"

#include "m_utility.h"

#include <tcl.h>
#include <string.h>
#include <ctype.h>

using namespace db;
using namespace tcl;


static char const* CmdCount	= "::scidb::player::count";
static char const* CmdDict		= "::scidb::player::dict";
static char const* CmdFilter	= "::scidb::player::filter";
static char const* CmdLetter	= "::scidb::player::letter";
static char const* CmdInfo		= "::scidb::player::info";
static char const* CmdSearch	= "::scidb::player::search";
static char const* CmdSort		= "::scidb::player::sort";

static app::PlayerDictionary* m_dictionary = 0;


__attribute__((unused))
static bool
checkNonZero(Tcl_Obj** objv, unsigned size)
{
	for (unsigned i = 0; i < size; ++i)
	{
		if (!objv[i])
			return false;
	}

	return true;
}


static tcl::player::Ratings
convRatings(char const* s)
{
	M_ASSERT(s);

	char const* t = s;

	while (*t && !::isspace(*t))
		++t;

	while (::isspace(*t))
		++t;

	return tcl::player::Ratings(rating::fromString(s), rating::fromString(t));
}


static Tcl_Obj*
wikiLinkList(Player const* player)
{
	typedef Player::AssocList AssocList;

	AssocList wikiLinks;

	if (player->wikipediaLinks(wikiLinks))
	{
		Tcl_Obj* objs[mstl::mul2(wikiLinks.size())];
		unsigned n = 0;

		Player::AssocList::const_iterator i = wikiLinks.begin();
		Player::AssocList::const_iterator e = wikiLinks.end();

		for ( ; i != e; ++i)
		{
			Player::Assoc const& assoc = *i;

			objs[n++] = Tcl_NewStringObj(assoc.first, -1);
			objs[n++] = Tcl_NewStringObj(assoc.second, -1);
		}

		return Tcl_NewListObj(n, objs);
	}

	return Tcl_NewListObj(0, 0);
}


static void
playerRatings(	NamebasePlayer const& player,
					rating::Type& type,
					int16_t* ratings,
					bool idCard,
					bool usePlayerBase)
{
	if (type == rating::Any)
		type = player.findRatingType();

	ratings[0] = player.playerHighestRating(type);
	ratings[1] = player.playerLatestRating(type);

	if (usePlayerBase)
	{
		if (player.isPlayerRating(type))
		{
			if (idCard)
			{
				if (uint16_t elo = player.findElo())
				{
					type = rating::Elo;
					ratings[0] = elo;
					ratings[1] = player.playerLatestRating(type);
				}
			}
		}
		else
		{
			ratings[0] = -ratings[0];
			ratings[1] = -ratings[1];
		}
	}
}


static bool
getMinMax(Tcl_Interp* ti, Tcl_Obj* val, int& min, int& max, char const* msg)
{
	Tcl_Obj** objs;
	int len;
	if (	Tcl_ListObjGetElements(ti, val, &len, &objs) != TCL_OK
		|| len != 2
		|| Tcl_GetIntFromObj(ti, objs[0], &min) != TCL_OK
		|| Tcl_GetIntFromObj(ti, objs[1], &max) != TCL_OK)
	{
		error(CmdFilter, nullptr, nullptr, msg);
		return false;
	}
	return true;
}


static int
getInfo(	NamebasePlayer const* player,
			Player const* lookup,
			tcl::player::Ratings& ratings,
			organization::ID organization,
			bool info,
			bool idCard,
			bool usePlayerBase)
{
	M_ASSERT(!idCard || info);
	M_ASSERT(!idCard || player);
	M_ASSERT(player || lookup);

	Tcl_Obj* objv[info ? attribute::player::LastInfo : attribute::player::LastColumn];

	M_ASSERT(::memset(objv, 0, sizeof(objv)));

	mstl::string			title;
	char const*				federationCode(mstl::string::empty_string.c_str());
	mstl::string const*	name;
	mstl::string			sex;
	mstl::string			species;
	mstl::string			fideID;
	bool						haveInfo;

	if (player)
	{
		name = &player->name();
		sex = sex::toString(player->sex());
		species = species::toString(usePlayerBase ? player->findType() : player->type());
		if (organization == organization::Fide && player->fideID())
			fideID.format("%u", player->fideID());
		haveInfo = player->havePlayerInfo();
	}
	else
	{
		name = &lookup->name();
		sex = sex::toString(lookup->sex());
		species = species::toString(lookup->type());
		fideID = lookup->organization(organization);
		haveInfo = true;
	}

	if (lookup)
	{
		if (idCard || !player || player->title() == title::None)
		{
			unsigned titles = lookup->titles();

			while (titles)
			{
				unsigned t = 1u << mstl::bf::lsb_index(titles);

				if (!title.empty())
					title += ' ';

				title += title::toString(title::toID(t));
				titles &= ~t;
			}
		}

		if (idCard || !player || player->federation() == country::Unknown)
			federationCode = country::toString(lookup->federation());

		if (idCard || (player && player->sex() == sex::Unspecified))
			sex = sex::toString(lookup->sex());

		if (idCard || (player && player->type() == species::Unspecified))
			species = species::toString(lookup->type());

		if (idCard)
			name = &lookup->name();

		if (fideID.empty())
		{
			fideID = lookup->organization(organization);

			if (!fideID.empty() && organization == organization::Fide && !idCard)
				fideID.replace(size_t(0), size_t(0), "*", size_t(1));
		}
	}

	mstl::string ratingType;
	int16_t rating1[2];
	int16_t rating2[2];

	if (player)
	{
		if (title.empty())
			title = title::toString(player->title());

		if (!*federationCode)
			federationCode = country::toString(player->federation());

		::playerRatings(*player, ratings.first,  rating1, idCard, usePlayerBase);
		::playerRatings(*player, ratings.second, rating2, idCard, usePlayerBase);

		ratingType = rating::toString(player->playerRatingType());
	}
	else
	{
		ratingType = rating::toString(ratings.second);
		rating1[0] = lookup->highestRating(ratings.first);
		rating2[0] = lookup->highestRating(ratings.second);
		rating1[1] = lookup->latestRating(ratings.first);
		rating2[1] = lookup->latestRating(ratings.second);
	}

	Tcl_Obj* ratingObj1[3] =
	{
		Tcl_NewIntObj(rating1[0]),
		Tcl_NewIntObj(rating1[1]),
		Tcl_NewStringObj(rating::toString(ratings.first), -1),
	};
	Tcl_Obj* ratingObj2[3] =
	{
		Tcl_NewIntObj(rating2[0]),
		Tcl_NewIntObj(rating2[1]),
		Tcl_NewStringObj(rating::toString(ratings.second), -1),
	};

	objv[attribute::player::Name      ] = Tcl_NewStringObj(*name, name->size());
	objv[attribute::player::FideID    ] = Tcl_NewStringObj(fideID, fideID.size());
	objv[attribute::player::Sex       ] = Tcl_NewStringObj(sex, sex.size());
	objv[attribute::player::Rating1   ] = Tcl_NewListObj(3, ratingObj1);
	objv[attribute::player::Rating2   ] = Tcl_NewListObj(3, ratingObj2);
	objv[attribute::player::RatingType] = Tcl_NewStringObj(ratingType, ratingType.size());
	objv[attribute::player::Country   ] = Tcl_NewStringObj(federationCode, -1);
	objv[attribute::player::Title     ] = Tcl_NewStringObj(title, title.size());
	objv[attribute::player::Type      ] = Tcl_NewStringObj(species, species.size());
	objv[attribute::player::PlayerInfo] = Tcl_NewBooleanObj(haveInfo);
	objv[attribute::player::Frequency ] = Tcl_NewIntObj(player ? player->frequency() : 0);

	M_ASSERT(::checkNonZero(objv, attribute::player::LastColumn));

	if (info)
	{
		unsigned			viafID(0);
		unsigned			iccfID(0);
		mstl::string	dsbID;
		mstl::string	ecfID;
		mstl::string	pndID;
		mstl::string	chessgamesID;
		Date				dateOfBirth;
		Date				dateOfDeath;
		Tcl_Obj*			wikiLinkList(0);
		Tcl_Obj*			aliasList(0);

		if (lookup)
		{
			typedef Player::StringList	StringList;

			dateOfBirth = lookup->dateOfBirth();
			dateOfDeath = lookup->dateOfDeath();
			iccfID = lookup->iccfID();
			dsbID = lookup->dsbID().asString();
			ecfID = lookup->ecfID().asString();
			viafID = lookup->viafID();
			pndID = lookup->pndID();
			chessgamesID = lookup->chessgamesID();
			wikiLinkList = ::wikiLinkList(lookup);

			StringList const& aliases = lookup->aliases();

			if (!aliases.empty())
			{
				Tcl_Obj* objs[aliases.size()];

				for (unsigned i = 0; i < aliases.size(); ++i)
					objs[i] = Tcl_NewStringObj(aliases[i], aliases[i].size());

				aliasList = Tcl_NewListObj(aliases.size(), objs);
			}
		}

		objv[attribute::player::DateOfBirth  ] = Tcl_NewStringObj(dateOfBirth.asShortString(), -1);
		objv[attribute::player::DateOfDeath  ] = Tcl_NewStringObj(dateOfDeath.asShortString(), -1);
		objv[attribute::player::DsbID        ] = Tcl_NewStringObj(dsbID, dsbID.size());
		objv[attribute::player::EcfID        ] = Tcl_NewStringObj(ecfID, ecfID.size());
		objv[attribute::player::IccfID       ] = iccfID ? Tcl_NewIntObj(iccfID) : Tcl_NewStringObj(0, 0);
		objv[attribute::player::ViafID       ] = viafID ? Tcl_NewIntObj(viafID) : Tcl_NewStringObj(0, 0);
		objv[attribute::player::PndID        ] = Tcl_NewStringObj(pndID, pndID.size());
		objv[attribute::player::ChessgComLink] = Tcl_NewStringObj(chessgamesID, chessgamesID.size());
		objv[attribute::player::WikiLink     ] = wikiLinkList ? wikiLinkList : Tcl_NewListObj(0, 0);
		objv[attribute::player::Aliases      ] = aliasList ? aliasList : Tcl_NewListObj(0, 0);

		M_ASSERT(::checkNonZero(objv, attribute::player::LastInfo));
	}

	setResult(info ? attribute::player::LastInfo : attribute::player::LastColumn, objv);
	return TCL_OK;
}


int
tcl::player::getInfo(NamebasePlayer const& player,
							Ratings& ratings,
							organization::ID organization,
							bool info,
							bool idCard,
							bool usePlayerBase)
{
	M_ASSERT(!idCard || info);

	Player const* lookup = usePlayerBase ? player.player() : 0;
	return ::getInfo(&player, lookup, ratings, organization, info, idCard, usePlayerBase);
}


static int
cmdCount(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	setResult(Tcl_NewIntObj(m_dictionary ? m_dictionary->count() : 0));
	return TCL_OK;
}


static int
cmdDict(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	char const* cmd = stringFromObj(objc, objv, 1);

	if (strcmp(cmd, "open") == 0)
	{
		if (m_dictionary)
			return error(CmdDict, nullptr, nullptr, "player dictionary is already open");

		::app::PlayerDictionary::Mode mode;
		char const* modeStr = stringFromObj(objc, objv, 2);

		if (strcmp(modeStr, "all") == 0)
			mode = ::app::PlayerDictionary::All;
		else if (strcmp(modeStr, "player") == 0)
			mode = ::app::PlayerDictionary::PlayersOnly;
		else if (strcmp(modeStr, "engine") == 0)
			mode = ::app::PlayerDictionary::EnginesOnly;
		else
			return error(CmdDict, nullptr, nullptr, "unknown dictionary mode '%s'", modeStr);

		if (m_dictionary == 0)
		{
			m_dictionary = new ::app::PlayerDictionary(mode);
			m_dictionary->sort(::app::PlayerDictionary::Name, order::Ascending);
			m_dictionary->finishOperation();
		}
	}
	else if (strcmp(cmd, "close") == 0)
	{
		if (!m_dictionary)
			return error(CmdInfo, nullptr, nullptr, "player dictionary is already closed");

		delete m_dictionary;
		m_dictionary = 0;
	}
	else
	{
		return error(CmdDict, nullptr, nullptr, "unknown command '%s'", cmd);
	}

	return TCL_OK;
}


static int
cmdInfo(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	if (!m_dictionary)
		return error(CmdInfo, nullptr, nullptr, "player dictionary is closed");

	tcl::player::Ratings ratings(rating::Elo, rating::DWZ);
	organization::ID organization = organization::Fide; // organization::Scidb;
#if 0
	unsigned champFlags = 0;
	bool titleYear = false;
	bool allTrophies = false;
	db::player::Champion::AgeSet ageSet(true);
	db::player::Champion::YearRange yearRange(true);
#endif

	while (objc > 2)
	{
		char const* arg = stringFromObj(objc, objv, objc - 2);

		if (::strcmp(arg, "-ratings") == 0)
		{
			ratings = ::convRatings(stringFromObj(objc, objv, objc - 1));
		}
		else if (::strcmp(arg, "-organization") == 0)
		{
			organization = organization::fromString(stringFromObj(objc, objv, objc - 1));
		}
#if 0
		else if (::strcmp(arg, "-titleyear") == 0)
		{
			titleYear = boolFromObj(objc, objv, objc - 1);
		}
		else if (::strcmp(arg, "-trophyflags") == 0)
		{
			char const* flags = stringFromObj(objc, objv, objc - 1);

			for ( ; *flags; ++flags)
			{
				switch (::tolower(*flags))
				{
					case 'o': champFlags |= db::player::Champion::OverTheBoard; break;
					case 'c': champFlags |= db::player::Champion::Correspondence; break;
					case 'u': champFlags |= db::player::Champion::Unrestricted; break;
					case 'j': champFlags |= db::player::Champion::Junior; break;
					case 's': champFlags |= db::player::Champion::Senior; break;
				}
			}
		}
		else if (::strcmp(arg, "-trophyageset") == 0)
		{
			Tcl_Obj** objs;
			int len;

			if (Tcl_ListObjGetElements(ti, objectFromObj(objc, objv, objc - 1), &len, &objs) != TCL_OK)
				return error(::CmdInfo, nullptr, nullptr, "list of ages expected");

			for (int i = 0; i < len; ++i)
			{
				int age;

				if (Tcl_GetIntFromObj(ti, objs[i], &age) != TCL_OK || 8 > age || age > 20)
					return error(::CmdInfo, nullptr, nullptr, "list of ages in 8...20 expected");

				ageSet.set(age);
			}
		}
		else if (::strcmp(arg, "-alltrophies") == 0)
		{
			allTrophies = true;
		}
#endif
		else
		{
			return error(::CmdInfo, nullptr, nullptr, "invalid argument '%s'", arg);
		}

		objc -= 2;
	}

	Player const& player = m_dictionary->getPlayer(unsignedFromObj(objc, objv, 1));

	Tcl_Obj* objs[11]; // [14];
	Tcl_Obj* titles[2];
	int		numTitles = 0;
	char		sex;

	if (player.type() == species::Program)
		sex = 'c';
	else if (player.sex() == sex::Male)
		sex = 'm';
	else if (player.sex() == sex::Female)
		sex = 'f';
	else
		sex = ' ';

	if (player.title() != title::None)
	{
		mstl::string s(title::toString(player.title()));
#if 0
		if (titleYear && player.titleYear()) s.format(":%u", unsigned(player.titleYear()));
#endif
		titles[numTitles++] = Tcl_NewStringObj(s, s.size());
	}
#if 0
	if (player.title2() != title::None)
	{
		mstl::string s(title::toString(player.title2()));
		if (titleYear && player.title2Year()) s.format(":%u", unsigned(player.title2Year()));
		titles[numTitles++] = Tcl_NewStringObj(s, s.size());
	}

	mstl::string specialTitle;
	Tcl_Obj* specialTitles;

	Player::ChampionIter ci = player.firstChampionRecord();
	Player::ChampionIter ce = player.lastChampionRecord();

	for ( ; ci != ce; ++ci)
	{
		if (ci->match(champFlags, region::World, yearRange, ageSet))
			{ specialTitle.assign(ci->asString()); break; }
		else if (ci->match(champFlags, region::Europe, yearRange, ageSet))
			specialTitle.assign(ci->asString());
		else if (specialTitle.empty() && ci->match(champFlags, region::Unknown, yearRange, ageSet))
			specialTitle.assign(ci->asString());
	}

	if (allTrophies)
	{
		specialTitles = Tcl_NewListObj(0, 0);

		for (ci = player.firstChampionRecord(); ci != ce; ++ci)
		{
			if (ci->match(champFlags, region::Unknown, yearRange, ageSet))
				Tcl_ListObjAppendElement(ti, specialTitles, Tcl_NewStringObj(ci->asString(), -1));
		}
	}
	else
	{
		specialTitles = Tcl_NewStringObj(specialTitle, specialTitle.size());
	}
#endif

	objs[ 0] = Tcl_NewStringObj(country::toString(player.nativeCountry()), -1);
	objs[ 1] = Tcl_NewStringObj(player.name(), player.name().size());
	objs[ 2] = Tcl_NewStringObj(player.organization(organization), -1);
	objs[ 3] = Tcl_NewStringObj(country::toString(player.federation()), -1);
	objs[ 4] = Tcl_NewStringObj(&sex, 1);
	objs[ 5] = Tcl_NewIntObj(mstl::max(int16_t(0), player.latestRating(ratings.first)));
#if 0
	objs[ 6] = Tcl_NewIntObj(player.ranking(ratings.first));
#endif
	objs[ 6] = Tcl_NewIntObj(mstl::max(int16_t(0), player.latestRating(ratings.second)));
#if 0
	objs[ 8] = Tcl_NewIntObj(player.ranking(ratings.second));
#endif
	objs[ 7] = Tcl_NewListObj(numTitles, titles);
#if 0
	objs[10] = specialTitles;
#endif
	objs[ 8] = Tcl_NewStringObj(player.dateOfBirth().asShortString(), -1);
	objs[ 9] = Tcl_NewStringObj(player.dateOfDeath().asShortString(), -1);
	objs[10] = Tcl_NewIntObj(player.frequency());

	setResult(U_NUMBER_OF(objs), objs);
	return TCL_OK;
}


static int
cmdLetter(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	if (!m_dictionary)
		return error(CmdInfo, nullptr, nullptr, "player dictionary is closed");

	m_dictionary->filterLetter(toupper(*stringFromObj(objc, objv, 1)));
	m_dictionary->finishOperation();
	return TCL_OK;
}


static int
cmdFilter(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	if (!m_dictionary)
		return error(CmdInfo, nullptr, nullptr, "player dictionary is closed");

	char const*	opStr		= stringFromObj(objc, objv, 1);
	Tcl_Obj*		filter	= objectFromObj(objc, objv, 2);
	int			length;

	if (Tcl_ListObjLength(ti, filter, &length) != TCL_OK || mstl::is_odd(length))
		return error(CmdInfo, nullptr, nullptr, "list of name/value pairs expected");

	::app::PlayerDictionary::Operator op = ::app::PlayerDictionary::Reset;

	switch (tolower(*opStr))
	{
		case 'n':
			switch (tolower(opStr[1]))
			{
				case 'u': op = ::app::PlayerDictionary::Null; break;
				case 'o': op = ::app::PlayerDictionary::Not; break;
			}
			break;

		case 'r':
			if (tolower(opStr[1]) == 'e')
			{
				switch (tolower(opStr[2]))
				{
					case 's': op = ::app::PlayerDictionary::Reset; break;
					case 'm': op = ::app::PlayerDictionary::Remove; break;
				}
			}
			break;

		case 'a': op = ::app::PlayerDictionary::And; break;
		case 'o': op = ::app::PlayerDictionary::Or; break;
	}

	if (length > 0)
	{
		for (int i = 0; i < length; i += 2)
		{
			Tcl_Obj* key;
			Tcl_Obj* val;

			Tcl_ListObjIndex(nullptr, filter, i, &key);
			Tcl_ListObjIndex(nullptr, filter, i + 1, &val);

			char const* attr = Tcl_GetString(key);

			switch (tolower(*attr))
			{
				case 'c': // country
					m_dictionary->filterNativeCountry(op, ::country::fromString(Tcl_GetString(val)));
					break;

				case 'f':
					if (strcasecmp(attr, "organization") == 0)
					{
						organization::ID organization = organization::fromString(Tcl_GetString(val));
						m_dictionary->filterOrganization(op, organization);
					}
					if (strcasecmp(attr, "federation") == 0)
					{
						m_dictionary->filterFederation(op, ::country::fromString(Tcl_GetString(val)));
					}
					else // frequency
					{
						int min, max;
						if (::getMinMax(ti, val, min, max, "'min-freq max-freq' expected"))
							m_dictionary->filterFrequency(op, min, max);
					}
					break;

				case 'n': // name
					m_dictionary->filterName(op, util::Pattern(Tcl_GetString(val)));
					break;

				case 's': // sex
					m_dictionary->filterSex(op, sex::fromString(Tcl_GetString(val)));
					break;

				case 'r': // rating
				{
					Tcl_Obj** objs;
					int len, min, max;
					rating::Type type;
					if (	Tcl_ListObjGetElements(ti, val, &len, &objs) != TCL_OK
						|| len != 3
						|| (type = rating::fromString(Tcl_GetString(objs[0]))) == rating::Any
						|| Tcl_GetIntFromObj(ti, objs[1], &min) != TCL_OK
						|| Tcl_GetIntFromObj(ti, objs[2], &max) != TCL_OK)
					{
						return error(CmdFilter, nullptr, nullptr, "'rating min-score max-score' expected");
					}
					m_dictionary->filterScore(op, type, min, max);
					break;
				}

				case 't':
				{
					switch (::tolower(attr[1]))
					{
						case 'i':	// titles
						{
							Tcl_Obj** objs;
							int len, minYear, maxYear;
							if (	Tcl_ListObjGetElements(ti, val, &len, &objs) != TCL_OK
								|| len != 3
								|| Tcl_GetIntFromObj(ti, objs[1], &minYear) != TCL_OK
								|| Tcl_GetIntFromObj(ti, objs[2], &maxYear) != TCL_OK)
							{
								return error(CmdFilter, nullptr, nullptr, "'titles min-year max-year' expected");
							}
							unsigned titles = 0;
							if (Tcl_ListObjGetElements(ti, objs[0], &len, &objs) != TCL_OK)
								return error(CmdFilter, nullptr, nullptr, "list of titles expected");
							for (int i = 0; i < len; ++i)
							{
								char const* s = Tcl_GetString(objs[i]);
								title::ID title = title::fromString(s);
								if (title == title::None)
									return error(CmdFilter, nullptr, nullptr, "invalid title '%s'", s);
								titles |= title::fromID(title);
							}
							m_dictionary->filterTitles(op, titles, minYear, maxYear);
							break;
						}

#if 0
						case 'r':	// trophy
						{
							Tcl_Obj** objs;
							int len;
							if (Tcl_ListObjGetElements(ti, val, &len, &objs) != TCL_OK || len != 2)
							{
								return error(	CmdFilter,
													nullptr,
													nullptr,
													"'trophy region-code list-of-flags' expected");
							}
							unsigned flags = 0;
							if (Tcl_ListObjGetElements(ti, objs[1], &len, &objs) != TCL_OK)
								return error(CmdFilter, nullptr, nullptr, "list of flags expected");
							for (int i = 0; i < len; ++i)
							{
								char const* flag = Tcl_GetString(objs[i]);
								switch (::tolower(*flag))
								{
									case 'j': flags |= db::player::Champion::Junior; break;
									case 's': flags |= db::player::Champion::Senior; break;
									case 'c': flags |= db::player::Champion::Correspondence; break;
									case 'f': flags |= db::player::Champion::Fide; break;
									case 'p': flags |= db::player::Champion::PCA; break;
									case 'u': flags |= db::player::Champion::Unofficial; break;
								}
							}
							char const* str = Tcl_GetString(objs[0]);
							unsigned code = region::fromString(str);
							if (code == region::Unknown)
								code = country::fromString(str);
							m_dictionary->filterTrophy(op, code, flags);
							break;
						}
#endif
					}
				}

				case 'b': // birthYear
				case 'd': // birthYear
				{
					int min, max;
					if (::getMinMax(ti, val, min, max, "'min-year max-year' expected"))
					{
						if (tolower(*attr) == 'b')
							m_dictionary->filterBirthYear(op, min, max);
						else
							m_dictionary->filterDeathYear(op, min, max);
					}
					break;
				}
			}

			if (op == ::app::PlayerDictionary::Reset)
				op = ::app::PlayerDictionary::And;
			else if (op == ::app::PlayerDictionary::Null)
				op = ::app::PlayerDictionary::Not;
		}
	}
	else
	{
		m_dictionary->resetFilter();
	}

	m_dictionary->finishOperation();
	return TCL_OK;
}


static int
cmdSearch(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	if (!m_dictionary)
		return error(CmdInfo, nullptr, nullptr, "player dictionary is closed");

	util::Pattern pattern(stringFromObj(objc, objv, 1));
	unsigned startIndex(intFromObj(objc, objv, 2) + 1);

	setResult(m_dictionary->search(pattern, startIndex));
	return TCL_OK;
}


static int
cmdSort(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	if (!m_dictionary)
		return error(CmdInfo, nullptr, nullptr, "player dictionary is closed");

	char const* order	= stringFromObj(objc, objv, 1);

	if (strcmp(order, "reverse") == 0)
	{
		m_dictionary->reverseOrder();
	}
	else if (strcmp(order, "cancel") == 0)
	{
		m_dictionary->cancelSort();
		m_dictionary->sort(::app::PlayerDictionary::Name, order::Ascending);
	}
	else
	{
		char const* attr = stringFromObj(objc, objv, 2);
		rating::Type ratingType = rating::fromString(stringFromObj(objc, objv, 3));

		if (ratingType == rating::Any)
			return error(CmdSort, nullptr, nullptr, "invalid rating type");

		// g++ complains w/o initialization
		::app::PlayerDictionary::Attribute attribute = ::app::PlayerDictionary::Name;
		order::ID ordering = order::Ascending;

		switch (*attr)
		{
			case 'c': attribute = ::app::PlayerDictionary::NativeCountry; break;
			case 'n': attribute = ::app::PlayerDictionary::Name; break;
			case 'b': attribute = ::app::PlayerDictionary::DateOfBirth; break;
			case 'd': attribute = ::app::PlayerDictionary::DateOfDeath; break;
			case 's': attribute = ::app::PlayerDictionary::Sex; break;
#if 0
			case 'A': attribute = ::app::PlayerDictionary::AcfId;
#endif
			case 'D': attribute = ::app::PlayerDictionary::DsbId; break;
			case 'E': attribute = ::app::PlayerDictionary::EcfId;
			case 'F': attribute = ::app::PlayerDictionary::FideID; break;
			case 'I': attribute = ::app::PlayerDictionary::IccfId; break;

			case 'f':
				if (attr[1] == 'e')
					attribute = ::app::PlayerDictionary::Federation;
				else
					attribute = ::app::PlayerDictionary::Frequency;
				break;

			case 't':
				if (attr[1] == 'y')
					attribute = ::app::PlayerDictionary::Type;
				else
					attribute = ::app::PlayerDictionary::Titles;
				break;

			case 'R':
				if (attr[1] == 'a')
				{
					if (attr[2] == 't')
						attribute = ::app::PlayerDictionary::LatestRating;
#if 0
					else
						attribute = ::app::PlayerDictionary::Ranking;
#endif
				}
				break;
		}

		switch (tolower(*order))
		{
			case 'a': ordering = order::Ascending; break;
			case 'd': ordering = order::Descending; break;
		}

		m_dictionary->sort(attribute, ordering, ratingType);
	}

	m_dictionary->finishOperation();
	return TCL_OK;
}


namespace tcl {
namespace player {

void
init(Tcl_Interp* ti)
{
	createCommand(ti, CmdCount,		cmdCount);
	createCommand(ti, CmdDict,		cmdDict);
	createCommand(ti, CmdInfo,		cmdInfo);
	createCommand(ti, CmdFilter,	cmdFilter);
	createCommand(ti, CmdLetter,	cmdLetter);
	createCommand(ti, CmdSearch,	cmdSearch);
	createCommand(ti, CmdSort,		cmdSort);
}

} // namespace player
} // namespace tcl

// vi:set ts=3 sw=3:
