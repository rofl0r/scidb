// ======================================================================
// Author : $Author$
// Version: $Revision: 193 $
// Date   : $Date: 2012-01-16 09:55:54 +0000 (Mon, 16 Jan 2012) $
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
// Copyright: (C) 2009-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _app_uci_engine_included
#define _app_uci_engine_included

#include "app_engine.h"

#include "db_board.h"

#include "m_string.h"

namespace app {
namespace uci {

class Engine : public app::Engine::Concrete
{
public:

	Engine();

	bool startAnalysis(db::Board const& board) override;
	bool startAnalysis(db::Game const& game, bool isNewGame) override;
	bool stopAnalysis() override;

protected:

	void protocolStart(bool isProbing) override;
	void protocolEnd() override;
	void sendNumberOfVariations() override;
	void processMessage(mstl::string const& message) override;
	void doMove(db::Game const& game, db::Move const& lastMove) override;

	Result probeResult() const override;
	unsigned maxVariations() const override;

private:

	bool whiteToMove() const;

	void parseBestMove(char const* msg);
	void parseInfo(char const* msg);
	void parseOption(char const* msg);
	bool prepareStartAnalysis(db::Board const& board);
	void setupPosition(db::Board const& board);

	db::Board		m_board;
	mstl::string	m_position;
	mstl::string	m_waitingOn;
	unsigned			m_maxMultiPV;
	bool				m_needChess960;
	bool				m_uciok;
	bool				m_hasMultiPV;
	bool				m_hasAnalyseMode;
	bool				m_hasChess960;
	bool				m_hasLimitStrength;
	bool				m_hasOwnBook;
	bool				m_hasShowCurrLine;
	bool				m_hasShowRefutations;
	bool				m_hasPonder;
};

} // namespace uci
} // namespace app

#endif // _app_uci_engine_included

// vi:set ts=3 sw=3:
