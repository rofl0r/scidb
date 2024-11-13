// ======================================================================
// Author : $Author$
// Version: $Revision: 420 $
// Date   : $Date: 2012-09-09 14:33:43 +0000 (Sun, 09 Sep 2012) $
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
// Copyright: (C) 2008-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "tcl_position.h"
#include "tcl_application.h"
#include "tcl_base.h"

#include "app_application.h"
#include "app_cursor.h"

#include "db_game.h"
#include "db_game_info.h"
#include "db_guess.h"
#include "db_board.h"
#include "db_move.h"
#include "db_move_list.h"

#include <tcl.h>
#include <ctype.h>
#include <string.h>

using namespace app;
using namespace db;
using namespace tcl;

using namespace tcl::pos;
using namespace tcl::app;

static char const* CmdBoard			= "::scidb::pos::board";
static char const* CmdFen				= "::scidb::pos::fen";
static char const* CmdGuess			= "::scidb::pos::guess";
static char const* CmdGuessNext		= "::scidb::pos::guessNext";
static char const* CmdIdn				= "::scidb::pos::idn";
static char const* CmdPromotion		= "::scidb::pos::promotion?";
static char const* CmdSan				= "::scidb::pos::san";
static char const* CmdSearchDepth	= "::scidb::pos::searchDepth";
static char const* CmdSetup			= "::scidb::pos::setup";
static char const* CmdStm				= "::scidb::pos::stm";
static char const* CmdValid			= "::scidb::pos::valid?";

static Square		m_bestMoveCache[64];
static unsigned	m_searchDepth	= 3;
static MoveList	m_bestMoveList;
static unsigned	m_bestMoveIndex = 0;


static Square
squareFromObj(int objc, Tcl_Obj* const objv[], unsigned index)
{
	char const* arg = stringFromObj(objc, objv, index);

	if (sq::isValid(arg))
		return sq::make(arg);

	if (::strcmp(arg, "null") == 0)
		return sq::Null;

	unsigned val = intFromObj(objc, objv, index);

	if (val < sq::a1 || sq::h8 < val)
		return sq::Null;

	return val;
}


static Byte
pieceFromObj(int objc, Tcl_Obj* const objv[], unsigned index)
{
	char const* arg	= stringFromObj(objc, objv, index);
	Byte			piece	= piece::None;

	if (isdigit(*arg))
	{
		piece = *arg - '0';

		if (piece < piece::King || piece::Pawn < piece)
			return piece::None;
	}
	else
	{
		piece = piece::fromLetter(*arg);
	}

	return piece;
}


void
pos::resetMoveCache()
{
	memset(m_bestMoveCache, sq::Null, sizeof(m_bestMoveCache));
	m_bestMoveList.clear();
}


void
pos::dumpBoard(::db::Board const& board, mstl::string& result)
{
	result.resize(64);

	for (unsigned i = 0; i < 64; ++i)
	{
		char c = piece::print(board.pieceAt(i));
		result[i] = isalpha(c) ? c : '.';
	}
}


void
pos::dumpFen(mstl::string const& position, mstl::string& result)
{
	Board board;
	board.setup(position);
	dumpBoard(board, result);
}


/// Takes a square and returns the best square that makes a move
/// with the given square. The square can be the from or to part of
/// a move. Used for smart move completion.
/// Returns -1 if no legal moves go to or from the square.
static int
cmdGuess(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	Square square = squareFromObj(objc, objv, 1);

	if (square == sq::Null)
		return error(::CmdGuess, nullptr, nullptr, "invalid square %s", Tcl_GetString(objv[1]));

	Board const& currentBoard = Scidb->game().currentBoard();

	if (m_bestMoveCache[square] != sq::Null)
	{
		setResult(m_bestMoveCache[square]);
	}
	else
	{
		// TODO: We have to distinguish between chess960 and standard chess.
		// Currently class Guess is designed for standard chess.
		Guess board(currentBoard, Scidb->gameInfoAt().idn());
		Move bestMove(board.bestMove(square, m_searchDepth));
		int bestSquare = -1;
		
		if (bestMove)
		{
			m_bestMoveList.clear();
			m_bestMoveList.append(bestMove);
			bestSquare = bestMove.from() == square ? bestMove.to() : bestMove.from();
		}

		m_bestMoveCache[square] = bestSquare;
		setResult(bestSquare);
	}

	return TCL_OK;
}


static int
cmdGuessNext(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	Square square = squareFromObj(objc, objv, 1);

	if (square == sq::Null)
		return error(::CmdGuess, nullptr, nullptr, "invalid square %s", Tcl_GetString(objv[1]));

	if (m_bestMoveCache[square] == sq::Null || m_bestMoveList.isEmpty())
	{
		setResult(-1);
	}
	else
	{
		Board const& currentBoard = Scidb->game().currentBoard();

		// TODO: We have to distinguish between chess960 and standard chess.
		// Currently class Guess is designed for standard chess.
		Guess board(currentBoard, Scidb->gameInfoAt().idn());
		Move bestMove(board.bestMove(square, m_bestMoveList, m_searchDepth));
		int bestSquare = -1;

		if (bestMove)
		{
			m_bestMoveList.append(bestMove);
			bestSquare = bestMove.from() == square ? bestMove.to() : bestMove.from();
			m_bestMoveIndex = 0;
		}
		else if (m_bestMoveList.size() > 1)
		{
			bestMove = m_bestMoveList[m_bestMoveIndex];
			bestSquare = bestMove.from() == square ? bestMove.to() : bestMove.from();

			if (++m_bestMoveIndex == m_bestMoveList.size())
				m_bestMoveIndex = 0;
		}

		setResult(bestSquare);
	}

	return TCL_OK;
}


static int
cmdSearchDepth(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	m_searchDepth = unsignedFromObj(objc, objv, 1);
	resetMoveCache();
	return TCL_OK;
}


static int
cmdStm(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	setResult(color::isWhite(Scidb->game().currentBoard().sideToMove()) ? "w" : "b");
	return TCL_OK;
}


static int
cmdBoard(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	mstl::string result;
	dumpBoard(Scidb->game().currentBoard(), result);
	setResult(result);
	return TCL_OK;
}


static int
cmdFen(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	setResult(Scidb->game().currentBoard().toFen());
	return TCL_OK;
}


static int
cmdSetup(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	scidb->game().setStartPosition(stringFromObj(objc, objv, 1));
	return TCL_OK;
}


static int
cmdIdn(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	setResult(Scidb->game().currentBoard().computeIdn());
	return TCL_OK;
}


static int
cmdValid(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	Square sq1 = squareFromObj(objc, objv, 1);
	Square sq2 = squareFromObj(objc, objv, 2);

	bool allowIllegalMove = boolFromObj(objc, objv, 3);

	if (sq1 == sq::Null || sq2 == sq::Null)
	{
		if (Scidb->game().currentBoard().checkState() & (Board::CheckMate | Board::StaleMate))
			setResult(0);	// null move not allowed
		else
			setResult(1);
	}
	else
	{
		Board	board	= Scidb->game().currentBoard();
		Move	move	= board.prepareMove(sq1, sq2);

		if (!move.isLegal() && allowIllegalMove)
		{
			color::ID side = board.sideToMove();

			board.tryCastleShort(side);
			board.tryCastleLong(side);

			move = board.prepareMove(sq1, sq2, move::AllowIllegalMove);
		}

#ifdef ALLOW_INVALID_MOVES
		setResult(bool(move));
#else
		setResult(bool(move) && board.checkMove(move));
#endif
	}

	return TCL_OK;
}


static int
cmdPromotion(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	Square sq1 = squareFromObj(objc, objv, 1);
	Square sq2 = squareFromObj(objc, objv, 2);

	bool allowIllegalMove = boolFromObj(objc, objv, 3);

	if (sq1 == sq::Null || sq2 == sq::Null)
	{
		setResult(false);
	}
	else
	{
		Board	board	= Scidb->game().currentBoard();
		Move	move	= board.prepareMove(sq1, sq2);

		if (!move.isLegal() && allowIllegalMove)
		{
			Board			board	= Scidb->game().currentBoard();
			color::ID	side	= board.sideToMove();

			board.tryCastleShort(side);
			board.tryCastleLong(side);

			move = board.prepareMove(sq1, sq2, move::AllowIllegalMove);
		}

#ifdef ALLOW_INVALID_MOVES
		setResult(move.isPromotion());
#else
		setResult(move.isPromotion() && board.checkMove(move));
#endif
	}

	return TCL_OK;
}


static int
cmdSan(ClientData, Tcl_Interp* ti, int objc, Tcl_Obj* const objv[])
{
	Square	sq1	= squareFromObj(objc, objv, 1);
	Square	sq2	= squareFromObj(objc, objv, 2);
	Byte		promo	= piece::Queen;

	if (objc >= 4)
		promo = pieceFromObj(objc, objv, 3);

	Move move;

	if (sq1 == sq::Null && sq2 == sq::Null)
		move = Scidb->game().currentBoard().makeNullMove();
	else if (sq1 == sq::Null)
		return error(::CmdSan, nullptr, nullptr, "invalid square %s", Tcl_GetString(objv[1]));
	else if (sq2 == sq::Null)
		return error(::CmdSan, nullptr, nullptr, "invalid square %s", Tcl_GetString(objv[2]));
	else
		move = Scidb->game().currentBoard().prepareMove(sq1, sq2);

	if (!move.isLegal())
	{
		Board			board	= Scidb->game().currentBoard();
		color::ID	side	= board.sideToMove();

		board.tryCastleShort(side);
		board.tryCastleLong(side);

		move = board.prepareMove(sq1, sq2, move::AllowIllegalMove);
	}

	if (!move)
	{
		setResult(mstl::string::empty_string);
	}
	else
	{
		if (move.isPromotion())
		{
			if (objc >= 4 && !piece::canPromoteTo(piece::Type(promo)))
			{
				return error(	::CmdSan,
									nullptr, nullptr,
									"invalid promotion piece %s",
									Tcl_GetString(objv[3]));
			}

			move.setPromotionPiece(piece::Type(promo));
		}

		mstl::string san;
		Scidb->game().currentBoard().prepareForPrint(move);
		move.printSan(san);
		setResult(san);
	}

	return TCL_OK;
}


namespace tcl {
namespace pos {

void
init(Tcl_Interp* ti)
{
	createCommand(ti, CmdBoard,			cmdBoard);
	createCommand(ti, CmdFen,				cmdFen);
	createCommand(ti, CmdGuess,			cmdGuess);
	createCommand(ti, CmdGuessNext,		cmdGuessNext);
	createCommand(ti, CmdIdn,				cmdIdn);
	createCommand(ti, CmdPromotion,		cmdPromotion);
	createCommand(ti, CmdSan,				cmdSan);
	createCommand(ti, CmdSearchDepth,	cmdSearchDepth);
	createCommand(ti, CmdSetup,			cmdSetup);
	createCommand(ti, CmdStm,				cmdStm);
	createCommand(ti, CmdValid,			cmdValid);
}

} // namespace game
} // namespace tcl

// vi:set ts=3 sw=3:
