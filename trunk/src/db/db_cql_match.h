// ======================================================================
// Author : $Author$
// Version: $Revision: 704 $
// Date   : $Date: 2013-04-04 22:19:12 +0000 (Thu, 04 Apr 2013) $
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

#include "db_board.h"
#include "db_board_base.h"
#include "db_common.h"

using namespace db;
using namespace color;


namespace {
namespace my {

struct Pieces
{
	Pieces() :pawns(0), knights(0), bishops(0), rooks(0), queens(0), kings(0), any(0) {}

	void complete() { any = pawns | knights | bishops | rooks | queens | kings; }

	uint64_t pawns;
	uint64_t knights;
	uint64_t bishops;
	uint64_t rooks;
	uint64_t queens;
	uint64_t kings;
	uint64_t any;
};


struct Position
{
	Position() :empty(0) {}

	void complete() { pieces[0].complete(); pieces[1].complete(); }

	Pieces	pieces[2];
	uint64_t	empty;
};

} // namespace my
} // namespace


enum
{
	WK = 1 << 0,	// white king
	WQ = 1 << 1,	// white queen
	WR = 1 << 2,	// white rook
	WB = 1 << 3,	// white bishop
	WN = 1 << 4,	// white knight
	WP = 1 << 5,	// white pawn
	WI = 1 << 6,	// white minor piece
	WM = 1 << 7,	// white major piece
	WA = 1 << 8,	// any white piece

	BK = 1 << 10,	// black king
	BQ = 1 << 11,	// black queen
	BR = 1 << 12,	// black rook
	BB = 1 << 13,	// black bishop
	BN = 1 << 14,	// black knight
	BP = 1 << 15,	// black pawn
	BI = 1 << 16,	// black minor piece
	BM = 1 << 17,	// black major piece
	BA = 1 << 18,	// any black piece

	E  = 1 << 20,	// empty
};


inline static bool matchAny(uint64_t lhs, uint64_t rhs) { return bool(lhs & rhs); }


static bool
match(my::Position const& pos, Board const& board)
{
	return	matchAny(pos.pieces[White].any, board.pieces(White))
			&& matchAny(pos.pieces[Black].any, board.pieces(Black))
			&& (	matchAny(pos.pieces[White].pawns, board.pawns(White))
				|| matchAny(pos.pieces[White].knights, board.knights(White))
				|| matchAny(pos.pieces[White].bishops, board.bishops(White))
				|| matchAny(pos.pieces[White].rooks, board.rooks(White))
				|| matchAny(pos.pieces[White].queens, board.queens(White))
				|| matchAny(pos.pieces[White].kings, board.kings(White)))
			&& (	matchAny(pos.pieces[Black].pawns, board.pawns(Black))
				|| matchAny(pos.pieces[Black].knights, board.knights(Black))
				|| matchAny(pos.pieces[Black].bishops, board.bishops(Black))
				|| matchAny(pos.pieces[Black].rooks, board.rooks(Black))
				|| matchAny(pos.pieces[Black].queens, board.queens(Black))
				|| matchAny(pos.pieces[Black].kings, board.kings(Black)));
}


#if 0
template <unsigned> bool match(my::Position const&, Board const&);


template <>
inline bool
match<WK>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.pieces[White].kings, board.kings(White));
}

template <>
inline bool
match<BK>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.pieces[White].kings, board.kings(White));
}

template <>
inline bool
match<WQ>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.pieces[White].queens, board.queens(White));
}

template <>
inline bool
match<BQ>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.pieces[Black].queens, board.queens(Black));
}

template <>
inline bool
match<WR>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.pieces[White].rooks, board.rooks(White));
}

template <>
inline bool
match<BR>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.pieces[Black].rooks, board.rooks(Black));
}

template <>
inline bool
match<WB>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.pieces[White].bishops, board.bishops(White));
}

template <>
inline bool
match<BB>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.pieces[Black].bishops, board.bishops(Black));
}

template <>
inline bool
match<WN>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.pieces[White].knights, board.knights(White));
}

template <>
inline bool
match<BN>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.pieces[Black].knights, board.knights(Black));
}

template <>
inline bool
match<WP>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.pieces[White].pawns, board.pawns(White));
}

template <>
inline bool
match<BP>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.pieces[Black].pawns, board.pawns(Black));
}

template <>
inline bool
match<WI>(my::Position const& pos, Board const& board)
{
	return match<WB>(pos, board) || match<WN>(pos, board);
}

template <>
inline bool
match<BI>(my::Position const& pos, Board const& board)
{
	return match<BB>(pos, board) || match<BN>(pos, board);
}

template <>
inline bool
match<WM>(my::Position const& pos, Board const& board)
{
	return match<WQ>(pos, board) || match<WR>(pos, board);
}

template <>
inline bool
match<BM>(my::Position const& pos, Board const& board)
{
	return match<BQ>(pos, board) || match<BR>(pos, board);
}

template <>
inline bool
match<WA>(my::Position const& pos, Board const& board)
{
	return match<WM>(pos, board) && match<WI>(pos, board) && match<WP>(pos, board);
}

template <>
inline bool
match<BA>(my::Position const& pos, Board const& board)
{
	return match<BM>(pos, board) && match<BI>(pos, board) && match<BP>(pos, board);
}

template <>
inline bool
match<E>(my::Position const& pos, Board const& board)
{
	return matchAny(pos.empty, board.empty());
}

#define MATCH(P1,P2)                                         \
	template <>                                               \
	inline bool                                        \
	match<P1|P2>(my::Position const& pos, Board const& board) \
	{                                                         \
		return match<P1>(pos, board) && match<P2>(pos, board); \
	}

MATCH(WK,WQ)
MATCH(WK,WR)
MATCH(WK,WB)
MATCH(WK,WN)
MATCH(WK,WP)
MATCH(WK,WI)
MATCH(WK,WM)
MATCH(WQ,WB)
MATCH(WQ,WN)
MATCH(WQ,WP)
MATCH(WQ,WI)
MATCH(WR,WB)
MATCH(WR,WN)
MATCH(WR,WP)
MATCH(WR,WI)
MATCH(WR,WM)
MATCH(WB,WP)
MATCH(WB,WI)
MATCH(WB,WM)
MATCH(WN,WP)
MATCH(WN,WI)
MATCH(WN,WM)
MATCH(WP,WI)
MATCH(WP,WM)

MATCH(BK,BQ)
MATCH(BK,BR)
MATCH(BK,BB)
MATCH(BK,BN)
MATCH(BK,BP)
MATCH(BK,BI)
MATCH(BK,BM)
MATCH(BQ,BB)
MATCH(BQ,BN)
MATCH(BQ,BP)
MATCH(BQ,BI)
MATCH(BR,BB)
MATCH(BR,BN)
MATCH(BR,BP)
MATCH(BR,BI)
MATCH(BR,BM)
MATCH(BB,BP)
MATCH(BB,BI)
MATCH(BB,BM)
MATCH(BN,BP)
MATCH(BN,BI)
MATCH(BN,BM)
MATCH(BP,BI)
MATCH(BP,BM)

MATCH(WK,BK)
MATCH(WQ,BQ)
MATCH(WR,BR)
MATCH(WB,BB)
MATCH(WN,BN)
MATCH(WP,BP)
MATCH(WI,BI)
MATCH(WM,BM)
MATCH(WA,BA)

MATCH(WK,WQ|WB)
MATCH(WK,WQ|WN)
MATCH(WK,WQ|WP)
MATCH(WK,WQ|WI)
MATCH(WK,WR|WB)
MATCH(WK,WR|WN)
MATCH(WK,WR|WP)
MATCH(WK,WR|WI)
MATCH(WK,WB|WP)
MATCH(WK,WB|WM)
MATCH(WK,WN|WP)
MATCH(WK,WN|WM)
MATCH(WK,WP|WI)
MATCH(WK,WP|WM)
MATCH(WQ,WB|WP)
MATCH(WQ,WB|WI)
MATCH(WQ,WN|WP)
MATCH(WQ,WN|WI)
MATCH(WR,WB|WP)
MATCH(WR,WN|WP)
MATCH(WR,WI|WP)
MATCH(WB,WP|WM)
MATCH(WN,WP|WM)

MATCH(BK,BQ|BB)
MATCH(BK,BQ|BN)
MATCH(BK,BQ|BP)
MATCH(BK,BQ|BI)
MATCH(BK,BR|BB)
MATCH(BK,BR|BN)
MATCH(BK,BR|BP)
MATCH(BK,BR|BI)
MATCH(BK,BB|BP)
MATCH(BK,BB|BM)
MATCH(BK,BN|BP)
MATCH(BK,BN|BM)
MATCH(BK,BP|BI)
MATCH(BK,BP|BM)
MATCH(BQ,BB|BP)
MATCH(BQ,BB|BI)
MATCH(BQ,BN|BP)
MATCH(BQ,BN|BI)
MATCH(BR,BB|BP)
MATCH(BR,BN|BP)
MATCH(BR,BI|BP)
MATCH(BB,BP|BM)
MATCH(BN,BP|BM)

MATCH(WK,WQ|WB|WP)
MATCH(BK,BQ|BB|BP)
#endif


inline static bool countAny(uint64_t lhs, uint64_t rhs) { return db::board::count(lhs & rhs); }


static bool
count(my::Position const& pos, Board const& board)
{
	return	countAny(pos.pieces[White].pawns, board.pawns(White))
			 + countAny(pos.pieces[White].knights, board.knights(White))
			 + countAny(pos.pieces[White].bishops, board.bishops(White))
			 + countAny(pos.pieces[White].rooks, board.rooks(White))
			 + countAny(pos.pieces[White].queens, board.queens(White))
			 + countAny(pos.pieces[White].kings, board.kings(White))
			 + countAny(pos.pieces[Black].pawns, board.pawns(Black))
			 + countAny(pos.pieces[Black].knights, board.knights(Black))
			 + countAny(pos.pieces[Black].bishops, board.bishops(Black))
			 + countAny(pos.pieces[Black].rooks, board.rooks(Black))
			 + countAny(pos.pieces[Black].queens, board.queens(Black))
			 + countAny(pos.pieces[Black].kings, board.kings(Black));
}


#if 0
template <unsigned> unsigned count(my::Position const&, Board const&);


template <>
inline unsigned
count<WK>(my::Position const& pos, Board const& board)
{
	return countAny(pos.pieces[White].kings, board.kings(White));
}

template <>
inline unsigned
count<BK>(my::Position const& pos, Board const& board)
{
	return countAny(pos.pieces[White].kings, board.kings(White));
}

template <>
inline unsigned
count<WQ>(my::Position const& pos, Board const& board)
{
	return countAny(pos.pieces[White].queens, board.queens(White));
}

template <>
inline unsigned
count<BQ>(my::Position const& pos, Board const& board)
{
	return countAny(pos.pieces[Black].queens, board.queens(Black));
}

template <>
inline unsigned
count<WR>(my::Position const& pos, Board const& board)
{
	return countAny(pos.pieces[White].rooks, board.rooks(White));
}

template <>
inline unsigned
count<BR>(my::Position const& pos, Board const& board)
{
	return countAny(pos.pieces[Black].rooks, board.rooks(Black));
}

template <>
inline unsigned
count<WB>(my::Position const& pos, Board const& board)
{
	return countAny(pos.pieces[White].bishops, board.bishops(White));
}

template <>
inline unsigned
count<BB>(my::Position const& pos, Board const& board)
{
	return countAny(pos.pieces[Black].bishops, board.bishops(Black));
}

template <>
inline unsigned
count<WN>(my::Position const& pos, Board const& board)
{
	return countAny(pos.pieces[White].knights, board.knights(White));
}

template <>
inline unsigned
count<WP>(my::Position const& pos, Board const& board)
{
	return countAny(pos.pieces[White].pawns, board.pawns(White));
}

template <>
inline unsigned
count<BP>(my::Position const& pos, Board const& board)
{
	return countAny(pos.pieces[Black].pawns, board.pawns(Black));
}

template <>
inline unsigned
count<WI>(my::Position const& pos, Board const& board)
{
	return count<WB>(pos, board) + count<WN>(pos, board);
}

template <>
inline unsigned
count<BI>(my::Position const& pos, Board const& board)
{
	return count<BB>(pos, board) + count<BN>(pos, board);
}

template <>
inline unsigned
count<WM>(my::Position const& pos, Board const& board)
{
	return count<WQ>(pos, board) + count<WR>(pos, board);
}

template <>
inline unsigned
count<BM>(my::Position const& pos, Board const& board)
{
	return count<BQ>(pos, board) + count<BR>(pos, board);
}

template <>
inline unsigned
count<WA>(my::Position const& pos, Board const& board)
{
	return count<WM>(pos, board) + count<WI>(pos, board) + count<WP>(pos, board);
}

template <>
inline unsigned
count<BA>(my::Position const& pos, Board const& board)
{
	return count<BM>(pos, board) + count<BI>(pos, board) + count<BP>(pos, board);
}

template <>
inline unsigned
count<E>(my::Position const& pos, Board const& board)
{
	return countAny(pos.empty, board.empty());
}

#define COUNT(P1,P2)                                         \
	template <>                                               \
	inline unsigned                                           \
	count<P1|P2>(my::Position const& pos, Board const& board) \
	{                                                         \
		return count<P1>(pos, board) + count<P2>(pos, board);  \
	}

COUNT(WK,WQ)
COUNT(WK,WR)
COUNT(WK,WB)
COUNT(WK,WN)
COUNT(WK,WP)
COUNT(WK,WI)
COUNT(WK,WM)
COUNT(WQ,WB)
COUNT(WQ,WN)
COUNT(WQ,WP)
COUNT(WQ,WI)
COUNT(WR,WB)
COUNT(WR,WN)
COUNT(WR,WP)
COUNT(WR,WI)
COUNT(WR,WM)
COUNT(WB,WP)
COUNT(WB,WI)
COUNT(WB,WM)
COUNT(WN,WP)
COUNT(WN,WI)
COUNT(WN,WM)
COUNT(WP,WI)
COUNT(WP,WM)

COUNT(BK,BQ)
COUNT(BK,BR)
COUNT(BK,BB)
COUNT(BK,BN)
COUNT(BK,BP)
COUNT(BK,BI)
COUNT(BK,BM)
COUNT(BQ,BB)
COUNT(BQ,BN)
COUNT(BQ,BP)
COUNT(BQ,BI)
COUNT(BR,BB)
COUNT(BR,BN)
COUNT(BR,BP)
COUNT(BR,BI)
COUNT(BR,BM)
COUNT(BB,BP)
COUNT(BB,BI)
COUNT(BB,BM)
COUNT(BN,BP)
COUNT(BN,BI)
COUNT(BN,BM)
COUNT(BP,BI)
COUNT(BP,BM)

COUNT(WK,BK)
COUNT(WQ,BQ)
COUNT(WR,BR)
COUNT(WB,BB)
COUNT(WN,BN)
COUNT(WP,BP)
COUNT(WI,BI)
COUNT(WM,BM)
COUNT(WA,BA)

COUNT(WK,WQ|WB)
COUNT(WK,WQ|WN)
COUNT(WK,WQ|WP)
COUNT(WK,WQ|WI)
COUNT(WK,WR|WB)
COUNT(WK,WR|WN)
COUNT(WK,WR|WP)
COUNT(WK,WR|WI)
COUNT(WK,WB|WP)
COUNT(WK,WB|WM)
COUNT(WK,WN|WP)
COUNT(WK,WN|WM)
COUNT(WK,WP|WI)
COUNT(WK,WP|WM)
COUNT(WQ,WB|WP)
COUNT(WQ,WB|WI)
COUNT(WQ,WN|WP)
COUNT(WQ,WN|WI)
COUNT(WR,WB|WP)
COUNT(WR,WN|WP)
COUNT(WR,WI|WP)
COUNT(WB,WP|WM)
COUNT(WN,WP|WM)

COUNT(BK,BQ|BB)
COUNT(BK,BQ|BN)
COUNT(BK,BQ|BP)
COUNT(BK,BQ|BI)
COUNT(BK,BR|BB)
COUNT(BK,BR|BN)
COUNT(BK,BR|BP)
COUNT(BK,BR|BI)
COUNT(BK,BB|BP)
COUNT(BK,BB|BM)
COUNT(BK,BN|BP)
COUNT(BK,BN|BM)
COUNT(BK,BP|BI)
COUNT(BK,BP|BM)
COUNT(BQ,BB|BP)
COUNT(BQ,BB|BI)
COUNT(BQ,BN|BP)
COUNT(BQ,BN|BI)
COUNT(BR,BB|BP)
COUNT(BR,BN|BP)
COUNT(BR,BI|BP)
COUNT(BB,BP|BM)
COUNT(BN,BP|BM)

COUNT(WK,WQ|WB|WP)
COUNT(BK,BQ|BB|BP)

#undef MATCH
#endif

// vi:set ts=3 sw=3:
