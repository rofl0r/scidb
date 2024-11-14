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

#ifndef _db_move_list_included
#define _db_move_list_included

#include "db_move.h"

#include "m_iterator.h"

namespace db {

class MoveList
{
public:

	// According to
	// http://en.wikipedia.org/wiki/World_records_in_chess
	// the maximum number of moves is possibly 218.
	// We have to add maximal 304 for piece drops (Zhouse),
	// TODO: Make a sharper calculation.
	enum { Maximum_Moves = 522 };

	typedef Move*			iterator;
	typedef Move const*	const_iterator;

	typedef mstl::reverse_iterator<Move>			reverse_iterator;
	typedef mstl::const_reverse_iterator<Move>	const_reverse_iterator;

	MoveList();

	MoveList& operator=(MoveList const& list);

	Move const& operator[](unsigned n) const;
	Move& operator[](unsigned n);

	Move const& front() const;
	Move const& back() const;

	bool isEmpty() const;
	bool isFull() const;
	bool notFull() const;

	unsigned size() const;
	int find(uint16_t move) const;
	unsigned match(MoveList const& list) const;

	const_iterator begin() const;
	const_iterator end() const;
	iterator begin();
	iterator end();

	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;
	reverse_iterator rbegin();
	reverse_iterator rend();

	void append(Move const& m);
	void push(Move const& m);
	Move& pop();
	void cut(unsigned size);
	void clear();

	void sort(int scores[]);
	void sort(unsigned startIndex, int scores[]);

	void print(mstl::string& result, unsigned halfMoveNo) const;
	void dump();

private:

	unsigned	m_size;
	Move		m_buffer[Maximum_Moves];
};

} // namespace db

#include "db_move_list.ipp"

#endif // _db_move_list_included

// vi:set ts=3 sw=3:
