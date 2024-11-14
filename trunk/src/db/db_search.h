// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
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

#ifndef _db_search_included
#define _db_search_included

#include "db_date.h"

#include "m_string.h"
#include "m_ref_counter.h"
#include "m_ref_counted_ptr.h"

namespace db {

class GameInfo;
class NamebasePlayer;
class NamebaseEvent;
class NamebaseSite;
class NamebaseAnnotator;

class Search : public mstl::ref_counter
{
public:

	virtual ~Search() throw() = 0;
	virtual bool match(GameInfo const& info) const = 0;
};

class SearchOpAnd : public Search
{
public:

	bool match(GameInfo const& info) const override;
	void append(Search* search);
};

class SearchOpOr : public Search
{
public:

	bool match(GameInfo const& info) const override;
	void append(Search* search);
};

class SearchOpNot : public Search
{
public:

	typedef mstl::ref_counted_ptr<Search> SearchP;

	SearchOpNot(SearchP const& search);
	~SearchOpNot() throw();

#if HAVE_0X_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR
	SearchOpNot(SearchOpNot&& search);
	SearchOpNot& operator=(SearchOpNot&& search);
#endif

	bool match(GameInfo const& info) const override;

private:

	SearchP m_search;
};

class SearchPlayer : public Search
{
public:

	SearchPlayer(NamebasePlayer const* entry);

	bool match(GameInfo const& info) const override;

private:

	NamebasePlayer const* m_entry;
};

class SearchEvent : public Search
{
public:

	SearchEvent(NamebaseEvent const* entry);

	bool match(GameInfo const& info) const override;

private:

	NamebaseEvent const* m_entry;
};

class SearchGameEvent : public Search
{
public:

	SearchGameEvent(NamebaseEvent const* entry, Date const& gameDate);

	bool match(GameInfo const& info) const override;

private:

	NamebaseEvent const* m_entry;
	Date m_firstDate;
	Date m_lastDate;
};

class SearchSite : public Search
{
public:

	SearchSite(NamebaseSite const* entry);

	bool match(GameInfo const& info) const override;

private:

	NamebaseSite const* m_entry;
};

class SearchAnnotator : public Search
{
public:

	SearchAnnotator(mstl::string const& name);

	bool match(GameInfo const& info) const override;

private:

	mstl::string m_name;
};

#if 0
class SearchRating : public Search
{
public:

	Search(unsigned minRatingW, unsigned maxRatingW, unsigned minRatingB, unsigned maxRatingB);

	Type type() const;
	bool match(GameInfo const& info) override;

private:

	unsigned m_minW;
	unsigned m_maxW;
	unsigned m_minB;
	unsigned m_maxB;
};
#endif

} // namespace db

#endif // _db_search_included

// vi:set ts=3 sw=3:
