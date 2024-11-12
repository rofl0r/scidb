// ======================================================================
// Author : $Author$
// Version: $Revision: 84 $
// Date   : $Date: 2011-07-18 18:02:11 +0000 (Mon, 18 Jul 2011) $
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
	bool stopAnalysis() override;

protected:

	void protocolStart(bool isProbing) override;
	void protocolEnd() override;
	void processMessage(mstl::string const& message) override;

	Result probeResult() const override;

private:

	bool whiteToMove() const;

	void parseBestMove(char const* msg);
	void parseInfo(char const* msg);
	void parseOption(char const* msg);

	db::Board		m_board;
	mstl::string	m_fen;
	mstl::string	m_waitingOn;
	bool				m_needChess960;
	bool				m_uciok;
};

} // namespace uci
} // namespace app

#endif // _app_uci_engine_included

// vi:set ts=3 sw=3:
