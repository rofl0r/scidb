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

#ifndef _sci_v92_decoder_position_included
#define _sci_v92_decoder_position_included

#include "db_board.h"
#include "db_move.h"

#include "m_stack.h"

namespace db {
namespace sci {
namespace v92 {
namespace decoder {

class Position
{
public:

	Position();

	void setup(char const* fen);
	void setup(uint16_t idn);

	void push();
	void pop();

	void doMove(Move& move, unsigned pieceNum);

	Board const& board() const;
	Board& board();

	bool whiteToMove() const;
	bool blackToMove() const;

	piece::Type piece(Square s) const;

	Square operator[](int n) const;

	Move makeShortCastlingMove(Square from);
	Move makeLongCastlingMove(Square from);
	Move makeKingMove(Square s, Square t) const;
	Move makeQueenMove(Square s, Square t) const;
	Move makeRookMove(Square s, Square t) const;
	Move makeBishopMove(Square s, Square t) const;
	Move makeKnightMove(Square s, Square t) const;

private:

	typedef Square Squares[32];

	struct Lookup
	{
		Board		board;
		Squares	squares;
	};

	typedef mstl::stack<Lookup> Stack;

	void setup(Board const& board);

	Stack	m_stack;
	Byte	m_rookNumbers[4];
};

} // namespace decoder
} // namespace v92
} // namespace sci
} // namespace db

namespace mstl {

template <typename> struct is_pod;

template <>
struct is_pod<db::sci::v92::decoder::Position::Lookup> { enum { value = is_pod<db::Board>::value }; };

} // namespace mstl

#include "sci_v92_decoder_position.ipp"

#endif // _sci_v92_decoder_position_included

// vi:set ts=3 sw=3:
