// ======================================================================
// Author : $Author$
// Version: $Revision: 36 $
// Date   : $Date: 2011-06-13 20:30:54 +0000 (Mon, 13 Jun 2011) $
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

#ifndef _app_cursor_included
#define _app_cursor_included

#include "db_common.h"

#include "m_vector.h"

namespace mstl { class string; }
namespace util { class Progress; }

namespace db {

class Database;
class Producer;
class TagSet;

} // namespace db

namespace app {

class Application;
class View;

class Cursor
{
public:

	static unsigned const BaseView = unsigned(-1);

	Cursor(Application& app, db::Database* database);
	~Cursor();

	bool isOpen() const;
	bool isClosed() const;
	bool isViewOpen(unsigned view) const;
	bool isReferenceBase() const;
	bool hasTreeView() const;

	/// Count number of loaded games.
	unsigned countGames() const;
	/// Return maximal view number
	unsigned maxViewNumber() const;
	/// Count number of players in database.
	unsigned countPlayers() const;
	/// Count number of events in database.
	unsigned countEvents() const;
	/// Count number of annotators in database.
	unsigned countAnnotators() const;
	/// Return database index of current game.
	unsigned gameIndex() const;
	/// Return database index of specified game in given view.
	unsigned gameIndex(unsigned index, unsigned view) const;
	/// Return player index of specified player in given view.
	unsigned playerIndex(unsigned index, unsigned view) const;
	/// Return event index of specified event in given view.
	unsigned eventIndex(unsigned index, unsigned view) const;
	/// Return annotator index of specified player in given view.
	unsigned annotatorIndex(unsigned index, unsigned view) const;
	/// Return name of database (may be a file name)
	mstl::string const& name() const;
	/// Return type of database
	db::type::ID type() const;
	/// Return view identifier of tree view (-1 if not exists)
	int treeViewIdentifier() const;

	/// Create new view and return the identifier.
	unsigned newView();
	/// Create new view for tree and return the identifier.
	unsigned newTreeView();
	/// Close an existing view.
	void closeView(unsigned view);
	/// Close an existing tree view.
	void closeTreeView();

	/// Return database object.
	db::Database const& database() const;
	/// Return database object.
	db::Database& database();
	/// Return default view,
	View const& view() const;
	/// Return default view,
	View& view();
	/// Return specified view.
	View const& view(unsigned id) const;
	/// Return specified view.
	View& view(unsigned id);
	/// Return tree view.
	View const& treeView() const;
	/// Return tree view.
	View& treeView();

	/// Import one game.
	unsigned importGame(db::Producer& producer, unsigned index);
	/// Import whole database.
	unsigned importGames(db::Producer& producer, util::Progress& progress);

	/// Update underlying database.
	void save(util::Progress& progress, unsigned start = 0);
	/// Close underlying database
	void close();
	/// Set whether this database is a reference database
	void setReferenceBase(bool flag);
	/// Removes all games from the underlying database.
	void clearBase();
	/// Update the characteristics of a game.
	void updateCharacteristics(unsigned index, db::TagSet const& tags);

private:

	friend class Application;

	typedef mstl::vector<View*>		ViewList;
	typedef mstl::vector<unsigned>	IndexSet;

	db::Database& base();

	void clear() throw();

	Application&	m_app;
	db::Database*	m_db;
	ViewList			m_viewList;
	IndexSet			m_freeSet;
	int				m_treeView;
	bool				m_isRefBase;
};

} // namespace app

#include "app_cursor.ipp"

#endif // _app_cursor_included

// vi:set ts=3 sw=3:
