// ======================================================================
// Author : $Author$
// Version: $Revision: 1395 $
// Date   : $Date: 2017-08-08 13:59:49 +0000 (Tue, 08 Aug 2017) $
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
// Copyright: (C) 2009-2017 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _app_winboard_engine_included
#define _app_winboard_engine_included

#include "app_engine.h"

#include "db_board.h"
#include "db_move.h"

#include "m_vector.h"
#include "m_string.h"
#include "m_auto_ptr.h"

namespace mstl { class string; }

namespace app {
namespace winboard {

class Engine : public app::Engine::Concrete
{
public:

	Engine();
	~Engine() throw();	// gcc complains w/o explicit destructor

	bool startAnalysis(bool isNewGame) override;
	bool stopAnalysis(bool restartIsPending) override;
	bool continueAnalysis() override;

	bool isReady() const override;
	bool isAnalyzing() const override;

	void timeout();
	void sendConfiguration(mstl::string const& script);

protected:

	void setupBoard(db::Board const& board);
	void reset();

	void protocolStart(bool isProbing) override;
	void protocolEnd() override;
	void stimulate() override;
	void sendOptions() override;
	void invokeOption(mstl::string const& name) override;
	void sendHashSize() override;
	void sendCores() override;
	void sendPondering() override;
	void processMessage(mstl::string const& message) override;
	void doMove(db::Move const& lastMove) override;

	bool pause() override;
	bool resume() override;

	Result probeResult() const override;
	Result probeAnalyzeFeature() const override;
	unsigned probeTimeout() const override;

private:

	enum State { None, Start, Stop, Pause };

	class Timer;

	typedef mstl::auto_ptr<Timer>		TimerP;
	typedef mstl::vector<db::Move>	History;

	void sendStartAnalysis();
	void sendStopAnalysis();
	void featureDone(bool done);
	void parseAnalysis(mstl::string const& msg);
	void parseInfo(mstl::string const& msg);
	bool parseCurrentMove(char const* s);
	void parseOption(mstl::string const& option);
	void parseFeatures(char const* msg);
	void detectFeatures(char const* identifier);
	void pongReceived();

	TimerP			m_timer;
	State				m_state;
	mstl::string	m_chess960Variant;
	uint64_t			m_startTime;
	unsigned			m_pingCount;
	unsigned			m_pongCount;
	unsigned			m_craftyVersion;

	bool m_isAnalyzing;
	bool m_response;
	bool m_waitForDone;
	bool m_waitForPong;
	bool m_analyzeResponse;
	bool m_identifierDetected;
	bool m_shortNameDetected;
	bool m_mustUseChess960;
	bool m_editSent;
	bool m_dontInvertScore;
	bool m_wholeSeconds;
	bool m_wholeSecondsDetected;
	bool m_featureUsermove;
	bool m_featureColors;
	bool m_featureSetboard;
	bool m_featureSigint;
	bool m_featureSAN;
	bool m_featureVariant;
	bool m_featurePing;
	bool m_parsingFeatures;
	bool m_startAnalyzeIsPending;
	bool m_stopAnalyzeIsPending;
	bool m_xboardAlreadySent;
};

} // namespace winboard
} // namespace app

#endif // _app_winboard_engine_included

// vi:set ts=3 sw=3:
