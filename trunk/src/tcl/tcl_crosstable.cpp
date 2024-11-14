// ======================================================================
// Author : $Author$
// Version: $Revision: 625 $
// Date   : $Date: 2013-01-09 16:39:57 +0000 (Wed, 09 Jan 2013) $
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

#include "tcl_application.h"
#include "tcl_player.h"
#include "tcl_game.h"
#include "tcl_base.h"

#include "app_application.h"
#include "app_cursor.h"
#include "app_view.h"

#include "db_database.h"
#include "db_tournament_table.h"

#include "T_Controller.h"
#include "T_NumberToken.h"
#include "T_TextToken.h"

#include "m_sstream.h"
#include "m_hash.h"
#include "m_assert.h"

#include <tcl.h>
#include <string.h>
#include <stdio.h>

using namespace db;
using namespace app;
using namespace tcl;
using namespace tcl::app;


static char const* CmdGet		= "::scidb::crosstable::get";
static char const* CmdEmit		= "::scidb::crosstable::emit";
static char const* CmdMake		= "::scidb::crosstable::make";
static char const* CmdRelease	= "::scidb::crosstable::release";


namespace {

struct Key
{
	Key() :databaseId(0), viewId(0) {}
	Key(unsigned baseId, unsigned view) :databaseId(baseId), viewId(view) {}
	Key(Database const& db, unsigned view) :databaseId(db.id()), viewId(view) {}

	bool operator==(Key const& key) const
	{
		return databaseId == key.databaseId && viewId == key.viewId;
	}

	unsigned	databaseId;
	unsigned	viewId;
};

typedef mstl::hash<Key,TournamentTable*> TableHash;

} // namespace

namespace mstl {

template <>
struct hash_key<Key>
{
	static size_t hash(Key const& key) { return key.databaseId ^ key.viewId; }
};

} // namespace mstl

static TableHash tableHash;


static TournamentTable*
getTable(char const* cmd, unsigned dbId, unsigned view)
{
	if (TableHash::const_pointer ptr = tableHash.find(Key(dbId, view)))
		return *ptr;

	error(cmd, nullptr, nullptr, "crosstable not exisiting in database %u", dbId);
	return 0; // not reached
}


static int
cmdMake(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	char const* database = stringFromObj(objc, objv, 1);
	variant::Type variant(tcl::game::variantFromObj(objc, objv, 2));
	unsigned view = unsignedFromObj(objc, objv, 3);
	Cursor const& cursor = Scidb->cursor(database, variant);

	TableHash::reference ref = tableHash[Key(cursor.database(), view)];

	if (ref == 0)
		ref = cursor.view(view).makeTournamentTable();

	setResult(cursor.database().id());
	return TCL_OK;
}


static int
cmdGet(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	static char const* subcommands[] = {
		"bestMode", "playerName", "playerId", "playerInfo", "playerCount", 0
	};
	static char const* args[] =
	{
		"<baseId> <view>",
		"<baseId> <view> <ranking>",
		"<baseId> <view> <ranking>",
		"<baseId> <view> <ranking>",
		"<baseId> <view>",
		0
	};
	enum { Cmd_BestMode, Cmd_PlayerName, Cmd_PlayerId, Cmd_PlayerInfo, Cmd_PlayerCount };

	if (objc < 4)
		return usage(::CmdGet, nullptr, nullptr, subcommands, args);

	int		cmd		= tcl::uniqueMatchObj(objv[1], subcommands);
	unsigned	baseId	= unsignedFromObj(objc, objv, 2);
	unsigned	view		= longFromObj(objc, objv, 3);

	TournamentTable* table = getTable(CmdGet, baseId, view);

	if (!table)
		return TCL_ERROR;

	switch (cmd)
	{
		case Cmd_BestMode:
			switch (table->bestMode())
			{
				case TournamentTable::Crosstable:	setResult("Crosstable"); break;
				case TournamentTable::Scheveningen:	setResult("Scheveningen"); break;
				case TournamentTable::Swiss:			setResult("Swiss"); break;
				case TournamentTable::Match:			setResult("Match"); break;
				case TournamentTable::Knockout:		setResult("Knockout"); break;
				case TournamentTable::RankingList:	setResult("RankingList"); break;
				case TournamentTable::Auto:			M_ASSERT("unexpected result"); break;
			}
			break;

		case Cmd_PlayerName:
			setResult(table->getPlayer(unsignedFromObj(objc, objv, 4))->name());
			break;

		case Cmd_PlayerId:
			{
				color::ID side;
				unsigned ranking = unsignedFromObj(objc, objv, 4);
				int index = table->getPlayerId(ranking - 1, side);
				if (index < 0)
					return error(CmdGet, nullptr, nullptr, "invalid ranking number %u", ranking);
				Tcl_Obj* objs[2];
				objs[0] = Tcl_NewIntObj(index);
				objs[1] = Tcl_NewStringObj(color::printColor(side), -1);
				setResult(2, objs);
			}
			break;

		case Cmd_PlayerInfo:
			{
				unsigned ranking = unsignedFromObj(objc, objv, 4);
				tcl::player::Ratings ratings(rating::Any, rating::Any);
				NamebasePlayer const* player = table->getPlayer(ranking - 1);
				if (!player)
					return error(CmdGet, nullptr, nullptr, "invalid ranking number %u", ranking);
				return tcl::player::getInfo(*player, ratings, federation::Fide, true, true);
			}
			break;

		case Cmd_PlayerCount:
			setResult(table->countPlayers());
			break;

		default:
			return usage(::CmdGet, nullptr, nullptr, subcommands, args);
	}

	return TCL_OK;
}


static int
cmdRelease(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	unsigned databaseId(unsignedFromObj(objc, objv, 1));
	unsigned view(unsignedFromObj(objc, objv, 2));
	Key key(databaseId, view);

	if (TableHash::const_pointer p = tableHash.find(key))
	{
		delete *p;
		tableHash.remove(key);
	}

	return TCL_OK;
}


static int
cmdEmit(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	struct Log : public TeXt::Controller::Log
	{
		void error(mstl::string const& msg) { str = msg; }
		mstl::string str;
	};

	unsigned baseId	= unsignedFromObj(objc, objv, 1);
	unsigned view		= longFromObj(objc, objv, 2);

	TournamentTable* table = getTable(CmdGet, baseId, view);

	if (!table)
		return TCL_ERROR;

	TeXt::Controller::LogP myLog(new Log);
	TeXt::Controller controller(stringFromObj(objc, objv, 3), TeXt::Controller::AbortMode, myLog);
	TournamentTable::Order order = TournamentTable::Score;
	TournamentTable::KnockoutOrder knockoutOrder = TournamentTable::Triangle;
	TournamentTable::ScoringSystem scoringSystem = TournamentTable::Traditional;
	TournamentTable::Mode mode = TournamentTable::Auto;

	TournamentTable::TiebreakRules tiebreakRules =
	{
		TournamentTable::None, TournamentTable::None, TournamentTable::None,
		TournamentTable::None, TournamentTable::None, TournamentTable::None,
	};

	{
		char const* tableMode = stringFromObj(objc, objv, 5);

		if (::strcasecmp(tableMode, "Auto") == 0)
			mode = TournamentTable::Auto;
		else if (::strcasecmp(tableMode, "Crosstable") == 0)
			mode = TournamentTable::Crosstable;
		else if (::strcasecmp(tableMode, "Scheveningen") == 0)
			mode = TournamentTable::Scheveningen;
		else if (::strcasecmp(tableMode, "Swiss") == 0)
			mode = TournamentTable::Swiss;
		else if (::strcasecmp(tableMode, "Match") == 0)
			mode = TournamentTable::Match;
		else if (::strcasecmp(tableMode, "Knockout") == 0)
			mode = TournamentTable::Knockout;
		else if (::strcasecmp(tableMode, "RankingList") == 0)
			mode = TournamentTable::RankingList;
	}

	{
		char const* tableOrder = stringFromObj(objc, objv, 6);

		if (::strcasecmp(tableOrder, "Score") == 0)
			order = TournamentTable::Score;
		else if (::strcasecmp(tableOrder, "Alphabetical") == 0)
			order = TournamentTable::Alphabetical;
		else if (::strcasecmp(tableOrder, "Rating") == 0)
			order = TournamentTable::Rating;
		else if (::strcasecmp(tableOrder, "Federation") == 0)
			order = TournamentTable::Federation;
		else
			return error(CmdEmit, nullptr, nullptr, "unknown order '%s'", tableOrder);
	}

	{
		char const* koOrder = stringFromObj(objc, objv, 7);

		if (::strcasecmp(koOrder, "Pyramid") == 0)
			knockoutOrder = TournamentTable::Pyramid;
		else if (::strcasecmp(koOrder, "Triangle") == 0)
			knockoutOrder = TournamentTable::Triangle;
		else
			return error(CmdEmit, nullptr, nullptr, "unknown order '%s'", koOrder);
	}

	{
		char const* system = stringFromObj(objc, objv, 8);

		if (::strcasecmp(system, "Traditional") == 0)
			scoringSystem = TournamentTable::Traditional;
		else if (::strcasecmp(system, "Bilbao") == 0)
			scoringSystem = TournamentTable::Bilbao;
		else
			return error(CmdEmit, nullptr, nullptr, "unknown scoring system '%s'", system);
	}

	{
		int argc;
		Tcl_Obj** argv;

		if (Tcl_ListObjGetElements(ti, objectFromObj(objc, objv, 9), &argc, &argv) != TCL_OK)
			return error(CmdEmit, nullptr, nullptr, "list of tiebreak rules expected");

		if (size_t(argc) > U_NUMBER_OF(tiebreakRules))
			return error(CmdEmit, nullptr, nullptr, "too many rules");

		for (int i = 0; i < argc; ++i)
		{
			char const* rule = stringFromObj(argc, argv, i);

			if (::strcasecmp(rule, "None") == 0)
				tiebreakRules[i] = TournamentTable::None;
			else if (::strcasecmp(rule, "Buchholz") == 0)
				tiebreakRules[i] = TournamentTable::Buchholz;
			else if (::strcasecmp(rule, "MedianBuchholz") == 0)
				tiebreakRules[i] = TournamentTable::MedianBuchholz;
			else if (::strcasecmp(rule, "ModifiedMedianBuchholz") == 0)
				tiebreakRules[i] = TournamentTable::ModifiedMedianBuchholz;
			else if (::strcasecmp(rule, "SonnebornBerger") == 0)
				tiebreakRules[i] = TournamentTable::SonnebornBerger;
			else if (::strcasecmp(rule, "Progressive") == 0)
				tiebreakRules[i] = TournamentTable::Progressive;
			else if (::strcasecmp(rule, "KoyaSystem") == 0)
				tiebreakRules[i] = TournamentTable::KoyaSystem;
			else if (::strcasecmp(rule, "GamesWon") == 0)
				tiebreakRules[i] = TournamentTable::GamesWon;
			else if (::strcasecmp(rule, "GamesWonWithBlack") == 0)
				tiebreakRules[i] = TournamentTable::GamesWonWithBlack;
			else if (::strcasecmp(rule, "RefinedBuchholz") == 0)
				tiebreakRules[i] = TournamentTable::RefinedBuchholz;
			else if (::strcasecmp(rule, "ParticularResult") == 0)
				tiebreakRules[i] = TournamentTable::ParticularResult;
			else if (::strcasecmp(rule, "TraditionalScoring") == 0)
				tiebreakRules[i] = TournamentTable::TraditionalScoring;
			else
				return error(CmdEmit, nullptr, nullptr, "unknown tiebreak rule '%s'", rule);
		}
	}

	table->emit(controller.receptacle(), scoringSystem, tiebreakRules, order, knockoutOrder, mode);

	mstl::string preamble(stringFromObj(objc, objv, 10));

	mstl::istringstream src(preamble);
	mstl::ostringstream dst;
	mstl::ostringstream out;

	if (controller.processInput(src, dst, &out, &out) >= 0)
	{
		int rc = controller.processInput(stringFromObj(objc, objv, 4), dst, &out, &out);

		if (rc == TeXt::Controller::OpenInputFileFailed)
			out.write(static_cast<Log*>(myLog.get())->str);
	}

	mstl::string htm(dst.str());
	mstl::string log(out.str());

	if (!htm.empty() && htm.back() == '\n')
		htm.set_size(htm.size() - 1);
	if (!log.empty() && log.back() == '\n')
		log.set_size(log.size() - 1);

	Tcl_Obj* args[2];
	args[0] = Tcl_NewStringObj(htm, htm.size());
	args[1] = Tcl_NewStringObj(log, log.size());

	setResult(2, args);
	return TCL_OK;
}


namespace tcl {
namespace crosstable {

void
init(Tcl_Interp* ti)
{
	createCommand(ti, CmdGet,		cmdGet);
	createCommand(ti, CmdEmit,		cmdEmit);
	createCommand(ti, CmdMake,		cmdMake);
	createCommand(ti, CmdRelease,	cmdRelease);
}

} // namespace crosstable
} // namespace tcl

// vi:set ts=3 sw=3:
