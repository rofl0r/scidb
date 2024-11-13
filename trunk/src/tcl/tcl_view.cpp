// ======================================================================
// Author : $Author$
// Version: $Revision: 193 $
// Date   : $Date: 2012-01-16 09:55:54 +0000 (Mon, 16 Jan 2012) $
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
// Copyright: (C) 2009-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "tcl_progress.h"
#include "tcl_application.h"
#include "tcl_log.h"
#include "tcl_obj.h"
#include "tcl_base.h"

#include "app_application.h"
#include "app_cursor.h"
#include "app_view.h"

#include "db_query.h"
#include "db_search.h"
#include "db_database.h"

#include "T_Controller.h"

#include "sys_utf8_codec.h"
#include "sys_file.h"

#include "m_ofstream.h"
#include "m_sstream.h"
#include "m_vector.h"
#include "m_algorithm.h"
#include "m_tuple.h"
#include "m_map.h"
#include "m_ref_counted_ptr.h"
#include "m_assert.h"

#include <tcl.h>

using namespace db;
using namespace app;
using namespace tcl;
using namespace tcl::app;

static char const* CmdClose			= "::scidb::view::close";
static char const* CmdCount			= "::scidb::view::count";
static char const* CmdExport			= "::scidb::view::export";
static char const* CmdFind				= "::scidb::view::find";
static char const* CmdMap				= "::scidb::view::map";
static char const* CmdNew				= "::scidb::view::new";
static char const* CmdPrint			= "::scidb::view::print";
static char const* CmdSearch			= "::scidb::view::search";
static char const* CmdSubscribe		= "::scidb::view::subscribe";
static char const* CmdUnsubscribe	= "::scidb::view::unsubscribe";


namespace {

struct Subscriber : public Cursor::Subscriber
{
	typedef mstl::tuple<Obj,Obj,Obj> Tuple;

	void addProc(Tcl_Obj* base, Tcl_Obj* proc, Tcl_Obj* arg)
	{
		M_ASSERT(base);
		M_ASSERT(proc);

		m_list.push_back(Tuple(base, proc, arg));
		m_list.back().get<0>().ref();
		m_list.back().get<1>().ref();
		m_list.back().get<2>().ref();
	}

	void removeProc(Tcl_Obj* base, Tcl_Obj* proc, Tcl_Obj* arg)
	{
		List::iterator i = mstl::find(m_list.begin(), m_list.end(), Tuple(base, proc, arg));

		if (i == m_list.end())
		{
			fprintf(stderr, "Warning: view::unsubscribe failed\n");
		}
		else
		{
			i->get<0>().deref();
			i->get<1>().deref();
			i->get<2>().deref();
			m_list.erase(i);
		}
	}

	void close(unsigned view) override
	{
		Tcl_Obj* v = Tcl_NewIntObj(view);
		Tcl_IncrRefCount(v);

		for (unsigned i = 0; i < m_list.size(); ++i)
		{
			Tuple const& data = m_list[i];

			if (data.get<2>())
				invoke(__func__, data.get<0>()(), data.get<2>()(), data.get<1>()(), v, nullptr);
			else
				invoke(__func__, data.get<0>()(), data.get<1>()(), v, nullptr);
		}

		Tcl_DecrRefCount(v);
	}

	typedef mstl::vector<Tuple> List;

	List m_list;
};


typedef mstl::ref_counted_ptr<Subscriber> SubscriberP;
typedef mstl::map<mstl::string, SubscriberP> SubscriberMap;
typedef mstl::ref_counted_ptr<Search> SearchP;

static SubscriberMap subscriberMap;

} // namespace


static SearchP
buildSearch(Database const& db, Tcl_Interp* ti, Tcl_Obj* query)
{
	int objc;
	Tcl_Obj** objv;

	Tcl_ListObjGetElements(ti, query, &objc, &objv);

	if (objc < 2)
	{
		error(CmdSearch, 0, 0, "invalid query");
		return SearchP();
	}

	static char const* subcommands[] =
	{
		"OR", "AND", "NOT", "player", "event", "gameevent", "annotator", 0
	};
	static char const* args[] =
	{
		"<query>+", "<query>+", "<query>+", "<string>", "<string>", "<string>"
	};
	enum
	{
		OR, AND, NOT, Player, Event, GameEvent, Annotator
	};

	SearchP search;

	switch (tcl::uniqueMatchObj(objv[0], subcommands))
	{
		case OR:
		case AND:
			// TODO
			return search;

		case NOT:
			if (objc != 2)
			{
				error(CmdSearch, "NOT", 0, "invalid query");
				return search;
			}
			search = new SearchOpNot(buildSearch(db, ti, objv[1]));
			break;

		case Player:
			if (objc != 2)
			{
				error(CmdSearch, "player", 0, "invalid query");
				return search;
			}
			search = new SearchPlayer(&db.player(unsignedFromObj(2, objv, 1)));
			break;

		case Event:
			if (objc != 2)
			{
				error(CmdSearch, "event", 0, "invalid query");
				return search;
			}
			search = new SearchEvent(&db.event(unsignedFromObj(2, objv, 1)));
			break;

		case GameEvent:
			if (objc != 2)
			{
				error(CmdSearch, "gameevent", 0, "invalid query");
				return search;
			}
			search = new SearchEvent(db.gameInfo(unsignedFromObj(2, objv, 1)).eventEntry());
			break;

		case Annotator:
			if (objc != 2)
			{
				error(CmdSearch, "annotator", 0, "invalid query");
				return search;
			}
			search = new SearchAnnotator(Tcl_GetString(objv[1]));
			break;

		default:
			usage(CmdSearch, Tcl_GetString(objv[0]), nullptr, subcommands, args);
			return search;
	}

	return search;
}


static int
cmdNew(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	char const*			base		= stringFromObj(objc, objv, 1);
	View::UpdateMode	mode[4];

	for (unsigned i = 0; i < U_NUMBER_OF(mode); ++i)
	{
		char const* arg = stringFromObj(objc, objv, i + 2);

		if (::strcmp(arg, "master") == 0)
			mode[i] = View::AddNewGames;
		else if (::strcmp(arg, "slave") == 0)
			mode[i] = View::LeaveEmpty;
		else
			error(CmdNew, 0, 0, "unknown resize mode '%s'", arg);
	}

	appendResult("%u", scidb->cursor(base).newView(mode[0], mode[1], mode[2], mode[3]));
	return TCL_OK;
}


static int
cmdClose(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	if (objc != 3)
	{
		Tcl_WrongNumArgs(ti, 1, objv, "<database> <view>");
		return TCL_ERROR;
	}

	int view;

	if (Tcl_GetIntFromObj(ti, objv[2], &view) != TCL_OK)
		return error(CmdClose, 0, 0, "unsigned integer expected for view");

	scidb->cursor(Tcl_GetString(objv[1])).closeView(view);
	return TCL_OK;
}


static int
cmdCount(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	static char const* subcommands[] = { "games", "players", "annotators", "events", 0 };
	static char const* args[] = { "<database> <view>" };
	enum { Cmd_Games, Cmd_Players, Cmd_Annotators, Cmd_Events };

	if (objc != 4)
		return usage(CmdCount, nullptr, nullptr, subcommands, args);

	char const* database = stringFromObj(objc, objv, 2);

	int index	= tcl::uniqueMatchObj(objv[1], subcommands);
	int view		= intFromObj(objc, objv, 3);

	switch (index)
	{
		case Cmd_Games:
			appendResult("%u", Scidb->cursor(database).view(view).countGames());
			return TCL_OK;

		case Cmd_Players:
			appendResult("%u", Scidb->cursor(database).view(view).countPlayers());
			return TCL_OK;

		case Cmd_Annotators:
			appendResult("%u", Scidb->cursor(database).view(view).countAnnotators());
			return TCL_OK;

		case Cmd_Events:
			appendResult("%u", Scidb->cursor(database).view(view).countEvents());
			return TCL_OK;
	}

	return usage(CmdCount, nullptr, nullptr, subcommands, args);
}


static int
cmdFind(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	static char const* subcommands[] = { "player", "event", "annotator", 0 };
	static char const* args[] = { "<database> <view>" };
	enum { Cmd_Player, Cmd_Event, Cmd_Annotator };

	if (objc != 5)
	{
		Tcl_WrongNumArgs(ti, 1, objv, "player|event|annotator <database> <view> <string>");
		return TCL_ERROR;
	}

	int index = tcl::uniqueMatchObj(objv[1], subcommands);

	char const*	database	= Tcl_GetString(objv[2]);
	char const* name		= Tcl_GetString(objv[4]);

	int view;

	if (Tcl_GetIntFromObj(ti, objv[3], &view) != TCL_OK)
		return error(CmdFind, 0, 0, "unsigned integer expected for view");

	View const& v = Scidb->cursor(database).view(view);

	switch (index)
	{
		case Cmd_Player:
			appendResult("%d", v.findPlayer(name));
			break;

		case Cmd_Event:
			appendResult("%d", v.findEvent(name));
			break;

		case Cmd_Annotator:
			appendResult("%d", v.findAnnotator(name));
			break;

		default:
			return usage(CmdCount, nullptr, nullptr, subcommands, args);
	}

	return TCL_OK;
}


static int
cmdSearch(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	if (objc < 5 || 6 < objc)
	{
		Tcl_WrongNumArgs(ti, 1, objv, "<database> <view> <operator> <none|events|players> ?<query>?");
		return TCL_ERROR;
	}

	char const*	database	= Tcl_GetString(objv[1]);
	char const*	ops		= Tcl_GetString(objv[3]);

	Query::Operator op;

	if (strcmp(ops, "null") == 0)
		op = Query::Null;
	else if (strcmp(ops, "or") == 0)
		op = Query::Or;
	else if (strcmp(ops, "and") == 0)
		op = Query::And;
	else if (strcmp(ops, "not") == 0)
		op = Query::Not;
	else if (strcmp(ops, "reset") == 0)
		op = Query::Reset;
	else if (strcmp(ops, "remove") == 0)
		op = Query::Remove;
	else
		return error(CmdSearch, 0, 0, "invalid operator %s", ops);

	char const* f = Tcl_GetString(objv[4]);
	unsigned filter = Application::None;

	switch (f[0])
	{
		case 'n': break;												// none
		case 'e': filter |= Application::Events; break;		// events
		case 'p': filter |= Application::Players; break;	// players

		default:  return error(CmdSearch, 0, 0, "invalid filter argument %s", f);
	}

	int view;

	if (Tcl_GetIntFromObj(ti, objv[2], &view) != TCL_OK)
		return error(CmdSearch, 0, 0, "unsigned integer expected for view");

	// TODO: we don't like to interrupt tree search!
	Cursor& cursor = scidb->cursor(database);

	SearchP search;

	if (objc >= 6)
		search = buildSearch(cursor.database(), ti, objv[5]);

	if (!search && objc == 6)
		return TCL_ERROR;

	scidb->searchGames(cursor, Query(mstl::ref_counted_ptr<Search>(search), op), view, filter);

	return TCL_OK;
}


static int
cmdExport(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	if (objc != 13)
	{
		Tcl_WrongNumArgs(
			ti, 1, objv,
			"<database> <view> <file> <flags> <mode> <encoding> <exclude-illegal-games-flag> "
			"<exported-tags> <progress-cmd> <progress-arg> <log-cmd> <log-arg>");
		return TCL_ERROR;
	}

	char const*		database			= stringFromObj(objc, objv, 1);
	unsigned			view				= unsignedFromObj(objc, objv, 2);
	char const*		filename			= stringFromObj(objc, objv, 3);
	unsigned			flags				= unsignedFromObj(objc, objv, 4);
	View::FileMode	mode				= boolFromObj(objc, objv, 5) ? View::Append : View::Create;
	mstl::string	encoding			= stringFromObj(objc, objv, 6);
	bool				excludeIllegal	= boolFromObj(objc, objv, 7);
	Tcl_Obj*			allowedTags		= objv[8];
	bool				extraTags		= false;

	Progress			progress(objv[9], objv[10]);
	tcl::Log			log(objv[11], objv[12]);
	Cursor&			cursor(scidb->cursor(database));
	Database&		db(cursor.database());
	View&				v(cursor.view(view));
	type::ID			type(db.type());
	View::TagBits	tagBits;
	Tcl_Obj**		tags;

	if (type == type::Temporary)
		type = type::Unspecific;

	if (Tcl_ListObjGetElements(ti, allowedTags, &objc, &tags) != TCL_OK)
		error(CmdExport, 0, 0, "invalid tag list");

	if (objc == 0)
	{
		tagBits.set();
		extraTags = true;
	}
	else
	{
		for (int i = 0; i < objc; ++i)
		{
			tag::ID tag = tag::fromName(Tcl_GetString(tags[i]));

			if (tag == tag::ExtraTag)
				extraTags = true;
			else
				tagBits.set(tag);
		}
	}

	setResult(v.exportGames(sys::file::internalName(filename),
									encoding,
									db.description(),
									type,
									flags,
									excludeIllegal ? View::ExcludeIllegal : View::AllGames,
									tagBits,
									extraTags,
									log,
									progress,
									mode));

	return TCL_OK;
}


static int
cmdPrint(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	struct Log : public TeXt::Controller::Log
	{
		void error(mstl::string const& msg) { str = msg; }
		mstl::string str;
	};

	if (objc != 16)
	{
		Tcl_WrongNumArgs(
			ti, 1, objv,
			"<database> <view> <file> <search-path> <script-path> <preamble> <flags> <options> <nag-map> "
			"<languages> <significant> <progress-cmd> <progress-arg> <log-cmd> <log-arg>");
		return TCL_ERROR;
	}

	char const*		database			= stringFromObj(objc, objv, 1);
	unsigned			view				= unsignedFromObj(objc, objv, 2);
	char const*		filename			= stringFromObj(objc, objv, 3);
	char const* 	searchPath		= stringFromObj(objc, objv, 4);
	mstl::string 	scriptPath		= stringFromObj(objc, objv, 5);
	mstl::string 	preamble			= stringFromObj(objc, objv, 6);
	unsigned			flags				= unsignedFromObj(objc, objv, 7);
	unsigned			options			= unsignedFromObj(objc, objv, 8);
	Tcl_Obj*			mapObj			= objectFromObj(objc, objv, 9);
	Tcl_Obj*			languageList	= objectFromObj(objc, objv, 10);
	unsigned			significant		= unsignedFromObj(objc, objv, 11);

	Progress				progress(objv[12], objv[13]);
	tcl::Log				log(objv[14], objv[15]);
	Cursor&				cursor(scidb->cursor(database));
	View&					v(cursor.view(view));
	Tcl_Obj**			objs;
	View::NagMap		nagMap;
	View::Languages	languages;

	::memset(nagMap, 0, sizeof(nagMap));

	if (Tcl_ListObjGetElements(ti, mapObj, &objc, &objs) != TCL_OK)
		error(CmdExport, 0, 0, "invalid nag map");

	for (int i = 0; i < objc; ++i)
	{
		Tcl_Obj** pair;
		int nelems;

		if (Tcl_ListObjGetElements(ti, objs[i], &nelems, &pair) != TCL_OK || nelems != 2)
			error(CmdExport, 0, 0, "invalid nag map");

		int lhs = intFromObj(2, pair, 0);
		int rhs = intFromObj(2, pair, 1);

		if (lhs >= nag::Scidb_Last || rhs >= nag::Scidb_Last)
			error(CmdExport, 0, 0, "invalid nag map values");

		nagMap[lhs] = rhs;
	}

	if (	Tcl_ListObjGetElements(ti, languageList, &objc, &objs) != TCL_OK
		|| objc >= int(U_NUMBER_OF(languages)))
	{
		error(CmdExport, 0, 0, "invalid language list");
	}

	for (int i = 0; i < objc; ++i)
		languages[i] = stringFromObj(objc, objs, i);

	TeXt::Controller::LogP myLog(new Log);
	TeXt::Controller controller(searchPath, TeXt::Controller::AbortMode, myLog);
	mstl::istringstream src(preamble);
	mstl::ofstream dst(sys::file::internalName(filename));
	mstl::ostringstream out;

	if (controller.processInput(src, dst, &out, &out) >= 0)
	{
		int rc = controller.processInput(scriptPath, dst, &out, &out);

		if (rc == TeXt::Controller::OpenInputFileFailed)
			out.write(static_cast<Log*>(myLog.get())->str);
	}

	setResult(v.printGames(	controller.environment(),
									format::LaTeX,
									flags,
									options,
									nagMap,
									languages,
									significant,
									log,
									progress));

//	{
//		mstl::string log(out.str());
//
//		if (!log.empty() && log.back() == '\n')
//			log.set_size(log.size() - 1);
//
//		setResult(Tcl_NewStringObj(log, log.size()));
//	}

	return TCL_OK;
}


static int
cmdMap(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	char const* attr = stringFromObj(objc, objv, 1);
	Cursor const& cursor(Scidb->cursor(stringFromObj(objc, objv, 2)));
	View const& view = cursor.view(unsignedFromObj(objc, objv, 3));
	unsigned index = unsignedFromObj(objc, objv, 4);

	if (::strcmp(attr, "player") == 0)
		setResult(view.lookupPlayer(index));
	else if (::strcmp(attr, "event") == 0)
		setResult(view.lookupEvent(index));
	else if (::strcmp(attr, "game") == 0)
		setResult(view.lookupGame(index));
	else
		error(CmdMap, 0, 0, "unexpected attribute '%s'", attr);

	return TCL_OK;
}


static int
cmdSubscribe(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	Tcl_Obj* proc	= objectFromObj(objc, objv, 1);
	Tcl_Obj* base	= objectFromObj(objc, objv, 2);
	Tcl_Obj* arg = objc > 3 ? objv[3] : 0;

	// XXX we don't want cancelling tree search
	mstl::string basename(Tcl_GetString(base));
	Cursor& cursor = scidb->cursor(basename);

	SubscriberP& subscriber = ::subscriberMap[basename];

	if (!subscriber)
	{
		subscriber = new Subscriber;
		cursor.setSubscriber(subscriber);
	}

	subscriber->addProc(proc, base, arg);

	return TCL_OK;
}


static int
cmdUnsubscribe(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	Tcl_Obj* proc	= objectFromObj(objc, objv, 1);
	Tcl_Obj* base	= objectFromObj(objc, objv, 2);
	Tcl_Obj* arg = objc > 3 ? objv[3] : 0;

	mstl::string basename(Tcl_GetString(base));
	SubscriberMap::iterator i = ::subscriberMap.find(basename);

	if (i != subscriberMap.end())
	{
		i->second->removeProc(proc, base, arg);

		if (i->second->m_list.empty())
			::subscriberMap.erase(i);
	}

	return TCL_OK;
}


namespace tcl {
namespace view {

void
init(Tcl_Interp* ti)
{
	createCommand(ti, CmdClose,			cmdClose);
	createCommand(ti, CmdCount,			cmdCount);
	createCommand(ti, CmdExport,			cmdExport);
	createCommand(ti, CmdFind,				cmdFind);
	createCommand(ti, CmdMap,				cmdMap);
	createCommand(ti, CmdNew,				cmdNew);
	createCommand(ti, CmdPrint,			cmdPrint);
	createCommand(ti, CmdSearch,			cmdSearch);
	createCommand(ti, CmdSubscribe,		cmdSubscribe);
	createCommand(ti, CmdUnsubscribe,	cmdUnsubscribe);
}

} // namespace view
} // namespace tcl

// vi:set ts=3 sw=3:
