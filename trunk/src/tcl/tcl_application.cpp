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
// Copyright: (C) 2010-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "tcl_application.h"
#include "tcl_base.h"

#include "app_cursor.h"

#include "db_database.h"
#include "db_player.h"
#include "db_site.h"
#include "db_eco_table.h"

#include "u_zstream.h"

#include "m_ifstream.h"
#include "m_assert.h"

#include <tcl.h>
#include <string.h>

using namespace db;
using namespace app;
using namespace tcl;
using namespace tcl::app;

Application tcl::app::scidb;
Application const& tcl::app::Scidb = tcl::app::scidb;

static char const* CmdBases	= "::scidb::app::bases";
static char const* CmdCount	= "::scidb::app::count";
static char const* CmdGet		= "::scidb::app::get";
static char const* CmdLoad		= "::scidb::app::load";
static char const* CmdLookup	= "::scidb::app::lookup";


static int
cmdLoad(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	char const* type = stringFromObj(objc, objv, 1);

	if (::strcmp(type, "done") == 0)
	{
		db::Player::loadDone();
		db::Site::loadDone();
	}
	else
	{
		char const* path = stringFromObj(objc, objv, 2);

		if (::strcmp(type, "eco") == 0)
		{
			mstl::ifstream stream(path, mstl::ios_base::in | mstl::ios_base::binary);

			if (!stream)
			{
				appendResult("cannot open file '%s'", path);
				return TCL_ERROR;
			}

			db::EcoTable::specimen().load(stream);
		}
		else
		{
			util::ZStream::Strings oldSuffixes = util::ZStream::zipFileSuffixes();

			try
			{
				util::ZStream stream(path, mstl::ios_base::in | mstl::ios_base::binary);

				if (!stream)
				{
					appendResult("cannot open file '%s'", path);
					return TCL_ERROR;
				}

				if (::strcmp(type, "ssp") == 0)
				{
					util::ZStream::setZipFileSuffixes(util::ZStream::Strings(1, "ssp"));
					db::Player::parseSpellcheckFile(stream);
				}
				else if (::strcmp(type, "fide") == 0)
				{
					util::ZStream::setZipFileSuffixes(util::ZStream::Strings(1, "txt"));
					db::Player::parseFideRating(stream);
				}
				else if (::strcmp(type, "dwz") == 0)
				{
					util::ZStream::setZipFileSuffixes(util::ZStream::Strings(1, "txt"));
					db::Player::parseDwzRating(stream);
				}
				else if (::strcmp(type, "ecf") == 0)
				{
					util::ZStream::setZipFileSuffixes(util::ZStream::Strings(1, "txt"));
					db::Player::parseEcfRating(stream);
				}
				else if (::strcmp(type, "ips") == 0)
				{
					util::ZStream::setZipFileSuffixes(util::ZStream::Strings(1, "txt"));
					db::Player::parseIpsRatingList(stream);
				}
				else if (::strcmp(type, "iccf") == 0)
				{
					util::ZStream::setZipFileSuffixes(util::ZStream::Strings(1, "txt"));
					db::Player::parseIccfRating(stream);
				}
				else if (::strcmp(type, "wiki") == 0)
				{
					util::ZStream::setZipFileSuffixes(util::ZStream::Strings(1, "txt"));
					db::Player::parseWikipediaLinks(stream);
				}
				else if (::strcmp(type, "comp") == 0)
				{
					util::ZStream::setZipFileSuffixes(util::ZStream::Strings(1, "txt"));
					db::Player::parseComputerList(stream);
				}
				else if (::strcmp(type, "cgdc") == 0)
				{
					util::ZStream::setZipFileSuffixes(util::ZStream::Strings(1, "txt"));
					db::Player::parseChessgamesDotComLinks(stream);
				}
				else if (::strcmp(type, "site") == 0)
				{
					util::ZStream::setZipFileSuffixes(util::ZStream::Strings(1, "txt"));
					db::Site::parseFile(stream);
				}
				else
				{
					appendResult("cannot load type '%s'", type);
					return TCL_ERROR;
				}
			}
			catch (...)
			{
				util::ZStream::setZipFileSuffixes(oldSuffixes);
				throw;
			}

			util::ZStream::setZipFileSuffixes(oldSuffixes);
		}
	}

	return TCL_OK;
}


static int
cmdCount(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	static char const* subcommands[] = { "games", "bases", 0 };
	static char const* args[] = { "", "" };
	enum { Cmd_Games, Cmd_Bases };

	if (objc < 2)
		return usage(::CmdCount, 0, 0, subcommands, args);

	int index = tcl::uniqueMatchObj(objv[1], subcommands);

	switch (index)
	{
		case Cmd_Games:
			setResult(Scidb.countGames());
			break;

		case Cmd_Bases:
			setResult(Scidb.countBases());
			break;

		default:
			return usage(::CmdCount, 0, 0, subcommands, args);
	}

	return TCL_OK;
}


static int
cmdBases(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	Application::CursorList list;
	Scidb.enumCursors(list);
	objc = list.size();

	Tcl_Obj* objs[objc];

	for (int i = 0; i < objc; ++i)
	{
		mstl::string const& name = list[i]->database().name();
		objs[i] = Tcl_NewStringObj(name, name.size());
	}

	setResult(objc, objs);
	return TCL_OK;
}


static int
cmdLookup(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	static char const* subcommands[] = { "countryCode", "ecoCode", "playerAlias", "siteAlias", 0 };
	static char const* args[] = { "<country-code>", "<eco-code>", "<player-name>", "<site-name>", 0 };
	enum { Cmd_CountryCode, Cmd_EcoCode, Cmd_PlayerAlias, Cmd_SiteAlias };

	int index = tcl::uniqueMatchObj(objectFromObj(objc, objv, 1), subcommands);

	switch (index)
	{
		case Cmd_CountryCode:
			{
				char const* code = stringFromObj(objc, objv, 2);

				if (::strlen(code) == 3)
					setResult(country::toString(country::fromString(code)));
				else
					setResult(country::toString(country::Unknown));
			}
			break;

		case Cmd_EcoCode:
			{
				char const* code = stringFromObj(objc, objv, 2);
				mstl::string opening[4];
				Tcl_Obj* objs[4];

				EcoTable::specimen().getOpening(Eco(code), opening[0], opening[1], opening[2], opening[3]);
				for (unsigned i = 0; i < 4; ++i)
					objs[i] = Tcl_NewStringObj(opening[i], opening[i].size());
				setResult(U_NUMBER_OF(objs), objs);
			}
			break;

		case Cmd_PlayerAlias:
			if (Player const* player = Player::findPlayer(stringFromObj(objc, objv, 2)))
			{
				Player::StringList const& aliases = player->aliases();
				mstl::string const& name = player->name();

				Tcl_Obj* list = Tcl_NewListObj(0, 0);
				Tcl_ListObjAppendElement(interp(), list, Tcl_NewStringObj(name, name.size()));

				for (unsigned i = 0; i < aliases.size(); ++i)
				{
					mstl::string const& s = aliases[i];
					Tcl_ListObjAppendElement(interp(), list, Tcl_NewStringObj(s, s.size()));
				}

				setResult(list);
			}
			break;

		case Cmd_SiteAlias:
			{
				if (Site const* site = Site::findSite(stringFromObj(objc, objv, 2)))
				{
					country::Code countryCode = country::Unknown;
					char const* country = stringFromObj(objc, objv, 3);

					if (::strlen(country))
						countryCode = country::fromString(country);

					Site::StringList const& aliases = site->aliases(countryCode);
					mstl::string const& name = site->name();

					Tcl_Obj* list = Tcl_NewListObj(0, 0);

					Tcl_ListObjAppendElement(interp(), list, Tcl_NewStringObj(name, name.size()));

					for (unsigned i = 0; i < aliases.size(); ++i)
					{
						mstl::string const& s = aliases[i];
						Tcl_ListObjAppendElement(interp(), list, Tcl_NewStringObj(s, s.size()));
					}

					setResult(list);
				}
			}
			break;

		default:
			return usage(::CmdLookup, 0, 0, subcommands, args);
	}

	return TCL_OK;
}


static int
cmdGet(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	static char const* subcommands[] = { "countryCodes", 0 };
	static char const* args[] = { "", 0 };
	enum { Cmd_CountryCodes };

	if (objc != 2)
		return usage(::CmdGet, 0, 0, subcommands, args);

	int index = tcl::uniqueMatchObj(objv[1], subcommands);

	switch (index)
	{
		case Cmd_CountryCodes:
			{
				Tcl_Obj* objs[country::count()];

				for (unsigned i = 0; i < country::count(); ++i)
				{
					char const* code = country::toString(country::Code(i));
					if (*code == '\0')
						code = "UNK";
					M_ASSERT(::strlen(code) == 3);
					objs[i] = Tcl_NewStringObj(code, 3);
				}

				setResult(country::count(), objs);
			}
			break;

		default:
			return usage(::CmdGet, 0, 0, subcommands, args);
	}

	return TCL_OK;
}


void
tcl::app::init(Tcl_Interp* ti)
{
	createCommand(ti, CmdBases,		cmdBases);
	createCommand(ti, CmdCount,		cmdCount);
	createCommand(ti, CmdGet,		cmdGet);
	createCommand(ti, CmdLoad,		cmdLoad);
	createCommand(ti, CmdLookup,	cmdLookup);
}

// vi:set ts=3 sw=3:
