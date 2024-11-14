// ======================================================================
// Author : $Author$
// Version: $Revision: 743 $
// Date   : $Date: 2013-04-26 15:55:35 +0000 (Fri, 26 Apr 2013) $
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
// Copyright: (C) 2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _cql_match_move_included
#define _cql_match_move_included

#include "cql_designator.h"

namespace db { class Move; }
namespace db { class Board; }

namespace cql {

class Designator;

namespace move {

struct Match
{
	typedef db::Board Board;
	typedef db::Move Move;
	typedef db::variant::Type Variant;

	virtual ~Match() = 0;
	virtual bool match(Board const& board, Move const& move, Variant variant) = 0;
};


struct EnPassant : public Match
{
	bool match(Board const& board, Move const& move, Variant variant) override;
};


struct NoEnPassant : public Match
{
	bool match(Board const& board, Move const& move, Variant variant) override;
};


struct IsCastling : public Match
{
	bool match(Board const& board, Move const& move, Variant variant) override;
};


struct NoCastling : public Match
{
	bool match(Board const& board, Move const& move, Variant variant) override;
};


class ExchangeEvaluation : public Match
{
public:

	ExchangeEvaluation(int min, int max);

	bool match(Board const& board, Move const& move, Variant variant) override;

private:

	int m_minScore;
	int m_maxScore;
};


class MoveFrom : public Match
{
public:

	MoveFrom(Designator const& designator);

	bool match(Board const& board, Move const& move, Variant variant) override;

private:

	Designator m_designator;
};


class MoveTo : public Match
{
public:

	MoveTo(Designator const& designator);

	bool match(Board const& board, Move const& move, Variant variant) override;

private:

	Designator m_designator;
};


class PieceDrop : public Match
{
public:

	PieceDrop(Designator const& designator);

	bool match(Board const& board, Move const& move, Variant variant) override;

private:

	Designator m_designator;
};


class Promote : public Match
{
public:

	Promote(Designator const& designator);

	bool match(Board const& board, Move const& move, Variant variant) override;

private:

	Designator m_designator;
};

} // namespace move
} // namespace cql

#include "cql_match_move.ipp"

#endif // _cql_match_move_included

// vi:set ts=3 sw=3:
