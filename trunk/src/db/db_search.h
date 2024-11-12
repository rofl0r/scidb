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
// Copyright: (C) 2009-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_search_included
#define _db_search_included

#include "m_string.h"

namespace db {

class GameInfo;
class NamebasePlayer;
class NamebaseEvent;
class NamebaseAnnotator;

class Search
{
public:

	virtual ~Search() throw() = 0;
	virtual bool match(GameInfo const& info) const = 0;
};

class SearchOpAnd : public Search
{
public:

	bool match(GameInfo const& info) const;
	void append(Search* search);
};

class SearchOpOr : public Search
{
public:

	bool match(GameInfo const& info) const;
	void append(Search* search);
};

class SearchOpNot : public Search
{
public:

	SearchOpNot(Search* search);
	~SearchOpNot() throw();

	bool match(GameInfo const& info) const;

private:

	Search* m_search;
};

class SearchPlayer : public Search
{
public:

	SearchPlayer(NamebasePlayer const* entry);

	bool match(GameInfo const& info) const;

private:

	NamebasePlayer const* m_entry;
};

class SearchEvent : public Search
{
public:

	SearchEvent(NamebaseEvent const* entry);

	bool match(GameInfo const& info) const;

private:

	NamebaseEvent const* m_entry;
};

class SearchAnnotator : public Search
{
public:

	SearchAnnotator(mstl::string const& name);

	bool match(GameInfo const& info) const;

private:

	mstl::string m_name;
};

#if 0
class SearchRating : public Search
{
public:

	Search(unsigned minRatingW, unsigned maxRatingW, unsigned minRatingB, unsigned maxRatingB);

	Type type() const;
	bool match(GameInfo const& info);

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
