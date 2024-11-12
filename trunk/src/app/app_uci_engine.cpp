// ======================================================================
// Author : $Author$
// Version: $Revision: 96 $
// Date   : $Date: 2011-10-28 23:35:25 +0000 (Fri, 28 Oct 2011) $
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

#include "app_uci_engine.h"
#include "app_exception.h"

#include "db_game.h"

#include "m_list.h"
#include "m_stdio.h"

#include <string.h>
#include <ctype.h>

using namespace app;
using namespace app::uci;
using namespace db;


inline bool isLan(char const* s)					{ return ::isalpha(s[0]) && ::isdigit(s[1]); }
inline static mstl::string toStr(bool value)	{ return value ? "true" : "false"; }


static mstl::string
toStr(unsigned value)
{
	char buf[20];
	snprintf(buf, sizeof(buf), "%u", value);
	return buf;
}


static char const*
skipSpaces(char const*s)
{
	while (::isspace(*s))
		++s;

	return s;
}


static char const*
skipNonSpaces(char const*s)
{
	while (*s && !::isspace(*s))
		++s;

	return s;
}


static char const*
skipWords(char const* s, unsigned n)
{
	while (n--)
	{
		while (*s && !::isspace(*s))
			++s;

		s = skipSpaces(s);
	}

	return skipSpaces(s);
}


static bool
nextWord(mstl::string& result, char const*& p)
{
	if (!isalpha(*p))
		return false;

	result.clear();

	while (*p && !isspace(*p))
		result += *++p;

	while (isspace(*p))
		++p;

	return true;
}


static void
append(mstl::string& str, mstl::string const& val)
{
	if (!str.empty())
		str += " ";

	str += val;
}


static void
subst(mstl::string& str, char c)
{
	mstl::string clone;

	for (unsigned i = 0; i < str.size(); ++i)
	{
		clone += str[i];

		if (str[i] == c)
			clone += c;
	}

	str.swap(clone);
}


static bool
isNumeric(char const* s)
{
	char const* p = s;

	while (isdigit(*p))
		++p;

	return p > s && *p == '\0';
}


static bool
endsWithPath(mstl::string const& s)
{
	if (s.size() < 4)
		return false;

	return strncmp(s.c_str() + s.size() - 4, "Path", 4) == 0;
}


uci::Engine::Engine()
	:m_maxMultiPV(1)
	,m_needChess960(false)
	,m_uciok(false)
	,m_hasMultiPV(false)
	,m_hasAnalyseMode(false)
	,m_hasChess960(false)
	,m_hasLimitStrength(false)
	,m_hasOwnBook(false)
	,m_hasShowCurrLine(false)
	,m_hasPonder(false)
{
}


void
uci::Engine::doMove(db::Game const& game, db::Move const&)
{
	if (isAnalyzing())
	{
		startAnalysis(game, false);
	}
	else
	{
		// TODO
	}
}


bool
uci::Engine::whiteToMove() const
{
	return m_board.whiteToMove();
}


uci::Engine::Result
uci::Engine::probeResult() const
{
	return m_uciok ? app::Engine::Probe_Successfull : app::Engine::Probe_Failed;
}


unsigned
uci::Engine::maxVariations() const
{
	return m_maxMultiPV;
}


void
uci::Engine::sendNumberOfVariations()
{
	if (isActive() && m_hasMultiPV)
	{
		m_waitingOn = "multiPV";
		send("stop");
		send("isready");
	}
}


bool
uci::Engine::prepareStartAnalysis(Board const& board)
{
	if (!isActive())
		return false;

	if (board.notDerivableFromChess960())
	{
		// what should we do?
	}

	if ((m_needChess960 = board.notDerivableFromStandardChess()))
	{
		if (!hasFeature(app::Engine::Feature_Chess_960))
			APP_RAISE("Chess 960 not supported");
	}

	return true;
}


void
uci::Engine::setupPosition(Board const& board)
{
	if (board.isStandardPosition())
	{
		m_position = "startpos";
	}
	else
	{
		m_position = "fen";
		m_position.append(board.toFen(
			hasFeature(app::Engine::Feature_Chess_960) ? Board::Shredder : Board::XFen));
	}
}


bool
uci::Engine::startAnalysis(Board const& board)
{
	if (!prepareStartAnalysis(board))
		return false;

	m_board = board;
	m_waitingOn = "position";
	setupPosition(board);

	send("stop");
	send("ucinewgame"); // clear's all states
	send("isready");

	return true;
}


bool
uci::Engine::startAnalysis(Game const& game, bool isNewGame)
{
	if (!prepareStartAnalysis(game.startBoard()))
		return false;

	setupPosition(game.startBoard());
	m_position.append(" moves");
	game.dumpHistory(m_position);

	m_board = game.currentBoard();
	m_waitingOn = "position";

	send("stop");
	if (isNewGame)
		send("ucinewgame"); // clear's all states
	send("isready");

	return true;
}


bool
uci::Engine::stopAnalysis()
{
	if (isAnalyzing())
		send("stop");

	return true;
}


void
uci::Engine::protocolStart(bool isProbing)
{
	// tell the engine we are using the UCI protocol
	send("uci");
	// after that we wait for "uciok"
}


void
uci::Engine::protocolEnd()
{
	// Some engines in analyze mode may not react as expected
	// to "quit" so ensure the engine exits analyze mode first:
	stopAnalysis();
	send("quit");
}


void
uci::Engine::processMessage(mstl::string const& message)
{
	switch (message[0])
	{
		case 'u':
			// we expect "uciok" as a response to "uci"
			if (message == "uciok")
			{
				m_uciok = true;

				if (!isProbing())
				{
					m_waitingOn = "uciok";
					send("isready");	// is required once to finish initialising
					// now we wait for "readyok"
				}
			}
			break;

		case 'r':
			if (message == "readyok")
			{
				if (m_waitingOn == "uciok")
				{
					// engine is now initialised and ready to go
//					setReady(true);
					// now we can send our options
					if (m_hasMultiPV)
						send("setoption name MultiPV value " + ::toStr(numVariations()));
					if (m_hasOwnBook)
						send("setoption name OwnBook value false");
					if (m_hasChess960)
						send("setoption name UCI_Chess960 value " + ::toStr(m_needChess960));
					if (m_hasShowCurrLine)
						send("setoption name UCI_ShowCurrLine value false");
					if (m_hasShowRefutations)
						send("setoption name UCI_ShowRefutations value false");
					if (m_hasPonder)
						send("setoption name Ponder value true");

					if (m_hasLimitStrength)
					{
						if (limitedStrength())
						{
							send("setoption name UCI_LimitStrength value true");
							send("setoption name UCI_Elo value " + ::toStr(limitedStrength()));
						}
						else
						{
							send("setoption name UCI_LimitStrength value false");
						}
					}
				}
				else if (m_waitingOn == "position")
				{
					// engine is now ready to analyse a new position
					m_waitingOn = "";
					if (m_hasAnalyseMode)
						send("setoption name UCI_AnalyseMode value true");
					send("position " + m_position);

					if (searchMate() > 0)
						send("go mate=" + ::toStr(searchMate()));
					else
						send("go infinite");

					// NOTE: probably we like to use something like
					// "go wtime 55857 btime 58611 winc 1000 binc 1000"
				}
				else if (m_waitingOn == "multiPV")
				{
					send("setoption name MultiPV value " + ::toStr(numVariations()));
				}

				m_waitingOn = "";
			}
			break;

		case 'i':
			if (isAnalyzing())
			{
				if (::strncmp(message, "info ", 5) == 0)
					parseInfo(::skipSpaces(message.c_str() + 5));
			}
			else if (::strncmp(message, "id name ", 8) == 0)
			{
				setIdentifier(message.c_str() + 8);
			}
			break;

		case 'o':
			if (isProbing() && ::strncmp(message, "option name ", 12) == 0)
				parseOption(::skipSpaces(message.c_str() + 12));
			break;

		case 'b':
			if (isAnalyzing() && ::strncmp(message, "bestmove ", 9) == 0)
				parseBestMove(message.c_str() + 9);
			break;
	}
}


void
uci::Engine::parseBestMove(char const* msg)
{
	char const* s = ::skipSpaces(msg);

	if (Move move = m_board.parseMove(s))
	{
		s = ::skipNonSpaces(s);

		if (move.isLegal())
			setBestMove(move);

		if (::strncmp(s, "ponder ", 6) == 0)
		{
			s = ::skipSpaces(s + 6);

			m_board.doMove(move);
			Move ponder = m_board.parseMove(s);
			m_board.undoMove(move);

			if (ponder.isLegal())
				setPonder(ponder);
		}
	}
}


void
uci::Engine::parseInfo(char const* msg)
{
	int	multiPv		= 1;
	bool	havePv		= false;
	bool	haveScore	= false;

	resetInfo();

	while (true)
	{
		unsigned numWords = 2;
		unsigned value;

		switch (msg[0])
		{
			case 'd':
				if (::sscanf(msg, "depth %u ", &value) == 1)
					setDepth(value);
				break;

			case 's':
				if (::strncmp(msg, "score ", 6) == 0)
				{
					int value;

					msg = skipSpaces(msg);

					switch (msg[0])
					{
						case 'c':
							if (::sscanf(msg, "cp %d ", &value) == 1)
							{
								setScore(whiteToMove() ? value : -value);
								haveScore = true;
							}
							break;

						case 'm':
							if (::sscanf(msg, "mate %d ", &value) == 1)
							{
								setMate(whiteToMove() ? value : -value);
								haveScore = true;
							}
							break;
					}
				}
				break;

			case 't':
				if (::sscanf(msg, "time %u ", &value) == 1)
					setTime(value/1000.0);
				break;

			case 'n':
				if (::sscanf(msg, "nodes %u ", &value) == 1)
					setNodes(value);
				break;

			case 'p':
				if (::strncmp(msg, "pv ", 3) == 0)
				{
					bool		okSoFar(true);
					Board		board(m_board);
					MoveList	moves;

					for ( ; ::isLan(msg); msg = ::skipWords(msg, 1))
					{
						if (okSoFar)
						{
							Move move = board.parseMove(msg);

							if (move.isLegal() && moves.notFull())
							{
								board.doMove(move);
								moves.append(move);
							}
							else
							{
								okSoFar = false;
							}
						}
					}

					setVariation(moves, multiPv);
					havePv = true;
					numWords = 0;
				}
				break;

			case 'm':
				if (	::sscanf(msg, "multipv %u ", &value) == 1
					&& 1 <= value
					&& value <= numVariations())
				{
					multiPv = value;
				}
				break;
		}

		msg = ::skipWords(msg, numWords);
	}

	if (haveScore && havePv)
		updateInfo();
}


void
uci::Engine::parseOption(char const* msg)
{
	mstl::string	name;
	mstl::string	type;
	mstl::string	dflt;
	mstl::string	min;
	mstl::string	max;
	mstl::string	key;
	mstl::string*	value = 0;

	mstl::list<mstl::string> vars;

	while (::nextWord(key, msg))
	{
		if (key == "name")			value = &name;
		else if (key == "type")		value = &type;
		else if (key == "default")	value = &dflt;
		else if (key == "min")		value = &min;
		else if (key == "max")		value = &max;
		else if (key == "var")		value = vars.insert(vars.end(), mstl::string());
		else								::append(*value,  key);
	}

	if (name.empty())
		return;

	if (name == "UCI_Chess960")
	{
		addFeature(app::Engine::Feature_Chess_960);
		return;
	}

	if (::strncmp(name, "UCI_", 4) == 0)
	{
		switch (name[4])
		{
			case 'A':
				if (name == "UCI_AnalyseMode")
				{
					m_hasAnalyseMode = true;
					break;
				}
				// fallthru
			case 'C':
				if (name == "UCI_Chess960")
				{
					m_hasChess960 = true;
					break;
				}
				// fallthru
			case 'L':
				if (name == "UCI_LimitStrength")
				{
					m_hasLimitStrength = true;
					break;
				}
				// fallthru
			case 'E':
				if (name == "UCI_Elo")
					break;
				if (name == "UCI_EngineAbout")
					break;
				// fallthru
			case 'S':
				if (name == "UCI_ShredderbasesPath")
				{
					break;
				}
				if (name == "UCI_ShowCurrLine")
				{
					m_hasShowCurrLine = true;
					break;
				}
				if (name == "UCI_ShowRefutations")
				{
					m_hasShowRefutations = true;
					break;
				}
				// fallthru
			default:
				return;
		}
	}

	if (type == "check")
	{
		if (dflt != "true" && dflt != "false")
			return;

		if (name == "Ponder")
		{
			// this means that the engine is able to ponder
			// should be enabled by default?
			m_hasPonder = true;
			return;
		}
		if (name == "OwnBook")
		{
			// this means that the engine has its own book
			// if this is set, the engine takes care of the opening book and
			// the GUI will never execute a move out of its book for the engine
			m_hasOwnBook = true;
			return;
		}

		addOption(name, type, dflt);
	}
	else if (type == "spin")
	{
		if (!::isNumeric(dflt) || !::isNumeric(min) || !::isNumeric(max))
			return;

		addOption(name, type, dflt, min, max);

		switch (name[0])
		{
//			case 'H':
//				if (name == "Hash")
//				{
//				}
//				break;

			case 'M':
				if (name == "MultiPV")
				{
					m_hasMultiPV = true;
					m_maxMultiPV = mstl::max(1ul, ::strtoul(max, nullptr, 10));
				}
				break;
		}
	}
	else if (type == "combo")
	{
		mstl::string var;
		bool found = false;

		for (unsigned i = 0; i < vars.size(); ++i)
		{
			if (!vars[i].empty())
			{
				if (dflt == vars[i])
					found = true;

				if (!var.empty())
					var += ";";
				::subst(vars[i], ';');
				var += vars[i];
			}
		}

		if (found)
			addOption(name, type, dflt, var);
	}
	else if (type == "button")
	{
		// Possibly we should skip: "Clear Hash", "Clear PosLearning"
		addOption(name, type);
	}
	else if (type == "string")
	{
		if (::endsWithPath(name))
			addOption(name, "path");
		else
			addOption(name, type, dflt);
	}
}

// vi:set ts=3 sw=3:
