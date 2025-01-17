// ======================================================================
// Author : $Author$
// Version: $Revision: 1522 $
// Date   : $Date: 2018-09-16 13:56:42 +0000 (Sun, 16 Sep 2018) $
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

#ifndef _app_view_included
#define _app_view_included

#include "db_filter.h"
#include "db_selector.h"
#include "db_log.h"
#include "db_common.h"

#include "m_pvector.h"
#include "m_vector.h"
#include "m_pair.h"
#include "m_string.h"
#include "m_utility.h"

namespace util { class Progress; }
namespace util { class Pattern; }
namespace TeXt { class Environment; }
namespace mstl { template <typename T, typename U> class map; }

namespace db {

class TournamentTable;
class Database;
class Consumer;
class Query;
class Log;

} // namespace db

namespace app {

class Application;
class Cursor;

class View : private mstl::noncopyable
{
public:

	enum FileMode		{ Create, Append, Upgrade };
	enum UpdateMode	{ NotNeeded, AddNewGames, LeaveEmpty };

	typedef mstl::pvector<mstl::string>	StringList;
	typedef mstl::vector<unsigned> LengthList;
	typedef ::db::tag::TagSet TagBits;
	typedef db::Byte NagMap[db::nag::Scidb_Last];
	typedef mstl::map<mstl::string,unsigned> TagMap;
	typedef mstl::vector<mstl::string> Languages;
	typedef mstl::pair<db::load::State,unsigned> Result;
	typedef UpdateMode UpdateModeList[db::table::LAST];

	static unsigned const DefaultView = 0;

	View(Application& app, Cursor& cursor);
	View(View& view);
	View(Application& app, Cursor& cursor, UpdateModeList const& updateMode);

	/// Return application.
	Application const& application() const;
	/// Return cursor.
	Cursor const& cursor() const;
	/// Return database.
	db::Database& database();
	/// Return database.
	db::Database const& database() const;

	/// Return whether this table type is used in this view.
	bool isUsed(db::table::Type type) const;

	/// Return the update mode of the specified table.
	UpdateMode updateMode(db::table::Type type) const;

	/// Return number of items in filter.
	unsigned count(db::table::Type type) const;
	/// Return size of filter.
	unsigned total(db::table::Type type) const;
	/// Return index according to current selector.
	unsigned index(db::table::Type type, unsigned index) const;

	/// Return index in current selector of given player name.
	int lookupPlayer(mstl::string const& name) const;
	/// Return index in current selector of given event title.
	int lookupEvent(mstl::string const& name) const;
	/// Return index in current selector of given site.
	int lookupSite(mstl::string const& name) const;
	/// Return index in current selector of given annotator name.
	int lookupAnnotator(mstl::string const& name) const;
	/// Return index in current selector of given position number.
	int lookupPosition(uint16_t idn) const;

	/// Return index in current selector of given game number.
	int lookupGameIndex(unsigned number) const;
	/// Return index in current selector of given game number.
	int lookupPlayerIndex(unsigned number) const;
	/// Return index in current selector of given game number.
	int lookupEventIndex(unsigned number) const;
	/// Return index in current selector of given game number.
	int lookupSiteIndex(unsigned number) const;
	/// Return index in current selector of given game number.
	int lookupPositionIndex(unsigned number) const;

	/// Return index of first matching player.
	int findPlayer(util::Pattern const& pattern, unsigned startIndex = 0) const;
	/// Return index of first matching event.
	int findEvent(util::Pattern const& pattern, unsigned startIndex = 0) const;
	/// Return index of first matching site.
	int findSite(util::Pattern const& pattern, unsigned startIndex = 0) const;
	/// Return index of first matching annotator.
	int findAnnotator(util::Pattern const& pattern, unsigned startIndex = 0) const;

	/// Return current table filter.
	db::Filter const& filter(db::table::Type type) const;
	/// Return current table selector.
	db::Selector const& selector(db::table::Type type) const;

	/// Compute next index in filter according to ordering.
	int nextIndex(db::table::Type type, unsigned index) const;
	/// Compute previous index in filter according to ordering.
	int prevIndex(db::table::Type type, unsigned index) const;
	/// Compute first index in filter according to ordering.
	int firstIndex(db::table::Type type) const;
	/// Compute last index in filter according to ordering.
	int lastIndex(db::table::Type type) const;
	/// Compute random game index in filter.
	int randomGameIndex() const;

	/// Get PGN (without variations) of given game index.
	Result dumpGame(unsigned index, mstl::string const& fen, mstl::string& result) const;
	/// Get PGN (without variations) of given game index.
	Result dumpGame(	unsigned index,
							unsigned split,
							mstl::string const& fen,
							StringList& result,
							StringList& positions) const;

	/// Add item to this view.
	void add(db::table::Type type, unsigned index);
	/// Sort database (using a selector).
	void sort(	db::attribute::game::ID attr,
					db::order::ID order,
					db::rating::Type ratingType = db::rating::Any);
	/// Sort database (using a selector).
	void sort(	db::attribute::player::ID attr,
					db::order::ID order,
					db::rating::Type ratingType = db::rating::Any);
	/// Sort database (using a selector).
	void sort(db::attribute::event::ID attr, db::order::ID order);
	/// Sort database (using a selector).
	void sort(db::attribute::site::ID attr, db::order::ID order);
	/// Sort database (using a selector).
	void sort(db::attribute::annotator::ID attr, db::order::ID order);
	/// Sort database (using a selector).
	void sort(db::attribute::position::ID attr, db::order::ID order);
	/// Reverse order of table (using a selector).
	void reverseOrder(db::table::Type type);
	/// Reset order of table (using a selector).
	void resetOrder(db::table::Type type);
	/// Update the modified selector. Must be used after any filter or sorting functions.
	void updateSelector(db::table::Type type);
	/// Do a search for games (modifies the filter).
	void searchGames(db::Query const& query);
	/// Set table filter according to game filter.
	void filterOnGames(db::table::Type type);

	/// Maintenance: strip move information from all games in view.
	unsigned stripMoveInformation(unsigned types, util::Progress& progress);
	/// Maintenance: strip PGN tags from all games in view.
	unsigned stripTags(TagMap const& tags, util::Progress& progress);
	/// Maintenance: find all additional tags in database.
	void findTags(TagMap& tags, util::Progress& progress) const;

	/// Reflect database changes (number of games),
	void update();
	/// Set game filter.
	void setGameFilter(db::Filter const& filter);

	/// Build tournament table for all games in current view.
	db::TournamentTable* makeTournamentTable() const;

	/// Copy games from database to database.
	unsigned copyGames(	Cursor& destination,
								TagBits const& allowedTags,
								bool allowExtraTags,
								unsigned* illegalRejected,
								db::Log& log,
								util::Progress& progress);

	/// Export games in view.
	unsigned exportGames(	mstl::string const& filename,
									mstl::string const& encoding,
									mstl::string const& description,
									uint32_t creationTime,
									db::type::ID type,
									unsigned flags,
									TagBits const& allowedTags,
									bool allowExtraTags,
									Languages const* languages,
									unsigned significantLanguages,
									unsigned* illegalRejected,
									db::Log& log,
									util::Progress& progress,
									FileMode fmode = Create) const;

	/// Print games in view.
	unsigned printGames(	TeXt::Environment& environment,
								db::format::Type format,
								unsigned flags,
								unsigned options,
								NagMap const& nagMap,
								Languages const* languages,
								unsigned significantLanguages,
								unsigned* illegalRejected,
								db::Log& log,
								util::Progress& progress) const;

private:

	unsigned exportGames(db::Consumer& destination,
								db::variant::Type variant,
								unsigned* illegalRejected,
								db::Log& log,
								util::Progress& progress) const;
	unsigned exportGames(db::Database& destination,
								unsigned* illegalRejected,
								db::Log& log,
								util::Progress& progress) const;

	void initialize();

	Application&	m_app;
	Cursor&			m_cursor;
	UpdateModeList	m_updateMode;
	db::Filter		m_filter[db::table::LAST];
	db::Selector	m_selector[db::table::LAST];

	mutable mstl::bitset m_used;
};

} // namespace app

#include "app_view.ipp"

#endif // _app_view_included

// vi:set ts=3 sw=3:
