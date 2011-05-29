// ======================================================================
// Author : $Author$
// Version: $Revision: 33 $
// Date   : $Date: 2011-05-29 12:27:45 +0000 (Sun, 29 May 2011) $
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

#include "app_cursor.h"
#include "app_application.h"
#include "app_view.h"

#include "db_database.h"

#include "m_assert.h"

using namespace db;
using namespace app;


Cursor::Cursor(Application& app, Database* database)
	:m_app(app)
	,m_db(database)
	,m_treeView(-1)
	,m_isRefBase(false)
{
	M_REQUIRE(database);

	m_viewList.push_back(new View(app, *database));	// base view
	m_viewList.push_back(new View(app, *database));	// view 0
}


Cursor::~Cursor()
{
	clear();
	delete *m_viewList.begin();
}


void
Cursor::clear() throw()
{
	for (ViewList::iterator i = m_viewList.begin() + 1; i < m_viewList.end(); ++i)
		delete *i;

	m_viewList.resize(1);
	m_treeView = -1;
}


bool
Cursor::isViewOpen(unsigned view) const
{
	M_REQUIRE(view == BaseView || view <= maxViewNumber());
	return view == BaseView ? true : m_viewList[view + 1] != 0;
}


unsigned
Cursor::newView()
{
	M_REQUIRE(isOpen());

	unsigned view;

	if (m_freeSet.empty())
	{
		view = m_viewList.size() - 1;
		m_viewList.push_back(new View(m_app, *m_db));
	}
	else
	{
		view = *m_freeSet.begin();
		m_freeSet.erase(m_freeSet.begin());
		m_viewList[view + 1] = new View(m_app, *m_db);
	}

	return view;
}


unsigned
Cursor::newTreeView()
{
	M_REQUIRE(isReferenceBase());
	return m_treeView = newView();
}


void
Cursor::closeView(unsigned view)
{
	M_REQUIRE(view != BaseView);

	if (view != 0 && m_viewList[view])
	{
		m_freeSet.push_back(view);
		delete m_viewList[view + 1];
		m_viewList[view + 1] = 0;

		if (m_treeView == int(view))
			m_treeView = -1;
	}
}


void
Cursor::closeTreeView()
{
	if (hasTreeView())
		closeView(m_treeView);
}


mstl::string const&
Cursor::name() const
{
	M_REQUIRE(isOpen());
	return m_db->name();
}


type::ID
Cursor::type() const
{
	M_REQUIRE(isOpen());
	return m_db->type();
}


unsigned
Cursor::countGames() const
{
	M_REQUIRE(isOpen());
	return m_db->countGames();
}


unsigned
Cursor::countPlayers() const
{
	M_REQUIRE(isOpen());
	return m_db->countPlayers();
}


unsigned
Cursor::countEvents() const
{
	M_REQUIRE(isOpen());
	return m_db->countEvents();
}


unsigned
Cursor::countAnnotators() const
{
	M_REQUIRE(isOpen());
	return m_db->countAnnotators();
}


unsigned
Cursor::gameIndex() const
{
	M_REQUIRE(isOpen());
	M_RAISE("not yet implemented");
	// TODO
	return 0;
}


unsigned
Cursor::gameIndex(unsigned index, unsigned view) const
{
	M_REQUIRE(isOpen());
	M_REQUIRE(isViewOpen(view));

	return view == BaseView ? index : m_viewList[view + 1]->gameIndex(index);
}


unsigned
Cursor::playerIndex(unsigned index, unsigned view) const
{
	M_REQUIRE(isOpen());
	M_REQUIRE(isViewOpen(view));

	return view == BaseView ? index : m_viewList[view + 1]->playerIndex(index);
}


unsigned
Cursor::eventIndex(unsigned index, unsigned view) const
{
	M_REQUIRE(isOpen());
	M_REQUIRE(isViewOpen(view));

	return view == BaseView ? index : m_viewList[view + 1]->eventIndex(index);
}


unsigned
Cursor::annotatorIndex(unsigned index, unsigned view) const
{
	M_REQUIRE(isOpen());
	M_REQUIRE(isViewOpen(view));

	return view == BaseView ? index : m_viewList[view + 1]->annotatorIndex(index);
}


Database&
Cursor::database()
{
	if (m_isRefBase)
		Application::stopUpdateTree();

	return *m_db;
}


void
Cursor::close()
{
	if (m_db)
	{
		m_db->close();
		delete m_db;
		m_db = 0;
		clear();
		m_freeSet.clear();
	}
}


void
Cursor::save(util::Progress& progress, unsigned start)
{
	M_REQUIRE(isOpen());
	M_REQUIRE(start <= countGames());

	// TODO: handle return code!
	m_db->save(progress, start);
}


void
Cursor::updateCharacteristics(unsigned index, TagSet const& tags)
{
	M_REQUIRE(isOpen());
	M_REQUIRE(index < countGames());

	// TODO: handle return code!
	m_app.updateCharacteristics(*this, index, tags);
}


void
Cursor::updateMoves(unsigned index)
{
	M_REQUIRE(isOpen());
	M_REQUIRE(index < countGames());

	// TODO: handle return code!
	m_app.updateMoves(*this, index);
}


unsigned
Cursor::importGame(Producer& producer, unsigned index)
{
	M_REQUIRE(isOpen());
	M_REQUIRE(index < countGames());

	if (m_isRefBase)
		Application::cancelUpdateTree();

	return m_db->importGame(producer, index);
}


unsigned
Cursor::importGames(Producer& producer, util::Progress& progress)
{
	M_REQUIRE(isOpen());

	if (m_isRefBase)
		Application::cancelUpdateTree();

	unsigned res = m_db->importGames(producer, progress);

	if (res > 0)
	{
		for (unsigned i = 0; i < m_viewList.size(); ++i)
		{
			View* view = m_viewList[i];

			if (view)
				view->update(i <= 1 ? View::AddNewGames : View::LeaveEmpty);
		}
	}

	return res;
}


void
Cursor::clearBase()
{
	database().clear();

	for (unsigned i = 0; i < m_viewList.size(); ++i)
	{
		View* view = m_viewList[i];

		if (view)
			view->update(View::LeaveEmpty);
	}
}

// vi:set ts=3 sw=3:
