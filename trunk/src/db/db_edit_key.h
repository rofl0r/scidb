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
// Copyright: (C) 2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_edit_key_included
#define _db_edit_key_included

#include "m_string.h"

namespace db {

class Board;
class Game;
class MoveNode;

namespace edit {

class Key
{
public:

	Key();
	explicit Key(unsigned firstPly);
	Key(mstl::string const& key);
	Key(Key const& key, char prefix);

	bool operator==(Key const& key) const;
	bool operator!=(Key const& key) const;
	bool operator< (Key const& key) const;
	bool operator> (Key const& key) const;

	bool isVariationId() const;
	bool isMainlineId() const;
	bool isPrefixed() const;

	mstl::string const& id() const;
	char prefix() const;
	unsigned level() const;
	unsigned plyNumber() const;

	int computeDistance(Key const& key) const;

	void addPly(unsigned ply);
	void exchangePly(unsigned ply);
	void removePly();

	void addVariation(unsigned varno);
	void exchangeVariation(unsigned varno);
	void exchangePrefix(char prefix);
	void removeVariation();

	void clear();
	void reset(unsigned firstPly);
	Key& strip();

	bool setPosition(Game& game) const;
	bool setBoard(MoveNode const* root, Board& board) const;

	static bool isValid(mstl::string const& key);

private:

	mstl::string m_id;
};


bool operator==(mstl::string const& lhs, Key const& rhs);
bool operator!=(mstl::string const& lhs, Key const& rhs);
bool operator==(Key const& lhs, mstl::string const& rhs);
bool operator!=(Key const& lhs, mstl::string const& rhs);

} // namespace edit
} // namespace db

#include "db_edit_key.ipp"

#endif // _db_edit_key_included

// vi:set ts=3 sw=3:
