// ======================================================================
// Author : $Author$
// Version: $Revision: 466 $
// Date   : $Date: 2012-10-14 23:03:57 +0000 (Sun, 14 Oct 2012) $
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

#include "app_engine.h"
#include "app_uci_engine.h"
#include "app_winboard_engine.h"

#include "db_game.h"
#include "db_player.h"

#include "sys_process.h"
#include "sys_timer.h"
#include "sys_info.h"

#include "u_misc.h"

#include "m_ostream.h"
#include "m_vector.h"
#include "m_algorithm.h"

#include <string.h>
#include <ctype.h>

using namespace app;
using namespace db;
using namespace util::misc::file;


namespace {

struct Range
{
	Range(char const* s, char const* e) :start(s), end(e) {}
	Range() :start(0), end(0) {}

	operator bool() const { return start < end; }


	bool set(char const* s, char const* e)
	{
		while (e > s && e[-1] == ' ')
			--e;
		while (s < e && s[0] == ' ')
			++s;

		start = s;
		end = e;

		return start < end;
	}

	char const* start;
	char const* end;
};

typedef mstl::vector<Range> Ranges;

} // namespace

static Range findShortName(Range const& range);


static void
compactSpaces(mstl::string& str)
{
	str.trim();

	M_ASSERT(str.empty() || !::isspace(str.front()));

	char const* s = str.begin();
	char const* e = str.end();

	char* p = str.begin();

	for ( ; s < e; s++)
	{
		if (!::isspace(*s))
			*p++ = *s;
		else if (p[-1] != ' ')
			*p++ = ' ';
	}

	str.set_size(p - str.begin());
}


static unsigned
split(Ranges& result, Range range, char delim)
{
	result.clear();

	while (range)
	{
		Range r;

		char const* p = strchr(range.start + 1, delim);

		if (p == 0 || p >= range.end)
		{
			if (r.set(range.start, range.end))
				result.push_back(r);
			return result.size();
		}

		if (delim == '(')
		{
			char const* q = strchr(p + 1, ')');

			if (q == 0 || q >= range.end)
			{
				if (r.set(range.start, range.end))
					result.push_back(r);
				return result.size();
			}

			Range r;

			if (r.set(range.start, p))
				result.push_back(r);

			if (r.set(p + 1, q))
				result.push_back(r);

			range.set(q + 1, range.end);
		}
		else
		{
			Range r;

			if (r.set(range.start, p))
				result.push_back(Range(range.start, p));

			range.start = p + 1;

			while (range && *range.start == delim)
				++range.start;

			range.set(range.start, range.end);
		}
	}

	return result.size();
}


static Range
findShortName(Ranges::const_iterator s, Ranges::const_iterator e)
{
	for (Ranges::const_iterator i = s; i != e; ++i)
	{
		for (Ranges::const_iterator k = e - 1; k >= i; --k)
		{
			Range r(i->start, k->end);

			if (db::Player::findEngine(mstl::string(r.start, r.end)))
				return r;
		}

		if (Range r = findShortName(*i))
			return r;
	}

	return Range();
}


static Range
findShortName(Range const& range, char const* delim)
{
	if (!range)
		return range;

	Ranges ranges;

	if (split(ranges, range, *delim) == 1)
	{
		if (db::Player::findEngine(mstl::string(range.start, range.end)))
			return range;
	}
	else
	{
		if (Range r = findShortName(ranges.begin(), ranges.end()))
			return r;
	}

	if (*++delim == '\0')
		return Range();

	return findShortName(range, delim);
}


static Range
findShortName(Range const& range)
{
	return findShortName(range, "(,. -");
}


static char const*
findVersionNumber(char const* s)
{
	if (*s == 'v' || *s == '.')
		++s;

	if (!isdigit(*s))
		return 0;

	++s;

	while (isdigit(*s) || *s == '.')
		++s;

	while (s[-1] == '.')
		--s;

	return s;
}


static char const*
findRomanNumber(char const* s)
{
	if (::strchr("XIV", ::toupper(*s)) == 0)
		return 0;

	++s;

	char const* numbers = ::strchr("XIV", *s) ? "XIV" : "xiv";

	while (::strchr(numbers, *s))
		++s;

	if (isalnum(*s))
		return 0;

	return s;
}


struct Engine::Process : public sys::Process
{
	Process(Engine* engine, mstl::string const& command, mstl::string const& directory);

	bool isConnected() const { return m_connected; }

	void readyRead() override;
	void exited() override;
	void stopped() override;
	void resumed() override;

	Engine*	m_engine;
	bool		m_connected;
};


Engine::Process::Process(Engine* engine, mstl::string const& command, mstl::string const& directory)
	:sys::Process(command, directory)
	,m_engine(engine)
	,m_connected(false)
{
}


void
Engine::Process::readyRead()
{
	m_engine->readyRead();

	if (!m_connected)
	{
		if (!m_engine->isActive())
			m_engine->activate();

		m_connected = true;
	}
}


void
Engine::Process::exited()
{
	m_engine->exited();
	m_connected = false;
}


void
Engine::Process::stopped()
{
	m_engine->stopped();
}


void
Engine::Process::resumed()
{
	m_engine->resumed();
}


Engine::Concrete::~Concrete() {}


unsigned
Engine::Concrete::maxVariations() const
{
	return 1;
}


Engine::Result
Engine::Concrete::probeAnalyzeFeature() const
{
	return Probe_Successfull;
}


void Engine::Concrete::sendNumberOfVariations() {}
void Engine::Concrete::sendHashSize() {}
void Engine::Concrete::sendCores() {}
void Engine::Concrete::sendThreads() {}
void Engine::Concrete::sendStrength() {}
void Engine::Concrete::sendSkillLevel() {}
void Engine::Concrete::sendPlayOther() {}
void Engine::Concrete::sendPondering() {}
void Engine::Concrete::sendPlayingStyle() {}
void Engine::Concrete::clearHash() {}


Engine::Engine(Protocol protocol, mstl::string const& command, mstl::string const& directory)
	:m_engine(0)
	,m_game(0)
	,m_gameId(unsigned(-1))
	,m_command(command)
	,m_directory(directory)
	,m_identifier(rootname(basename(command)))
	,m_ordering(Unordered)
	,m_currentVariant(Variant_Standard)
	,m_elo(0)
	,m_minElo(0)
	,m_maxElo(0)
	,m_skillLevel(0)
	,m_minSkillLevel(0)
	,m_maxSkillLevel(0)
	,m_maxMultiPV(1)
	,m_variations(1, MoveList())
	,m_numVariations(1)
	,m_hashFullness(0)
	,m_hashSize(0)
	,m_minHashSize(0)
	,m_maxHashSize(0)
	,m_numCores(1)
	,m_numThreads(0)
	,m_minThreads(0)
	,m_maxThreads(0)
	,m_playOther(false)
	,m_pondering(false)
	,m_searchMate(0)
	,m_strength(0)
	,m_features(0)
	,m_variants(0)
	,m_currMoveNumber(0)
	,m_currMoveCount(0)
	,m_bestIndex(0)
	,m_bestScore(0)
	,m_shortestMate(0)
	,m_depth(0)
	,m_selDepth(0)
	,m_time(0.0)
	,m_nodes(0)
	,m_active(false)
	,m_probe(false)
	,m_probeAnalyze(false)
	,m_identifierSet(false)
	,m_useLimitedStrength(false)
	,m_bestInfoHasChanged(false)
	,m_process(0)
	,m_exitStatus(0)
	,m_logStream(0)
{
	switch (protocol)
	{
		case Uci:		m_engine = new uci::Engine; break;
		case WinBoard:	m_engine = new winboard::Engine; break;
	}

	m_engine->m_engine = this;
}


Engine::~Engine()
{
	if (m_process)
	{
		// deactivate(); cannot call in destructor
		delete m_process;
	}

	delete m_engine;
}


void Engine::updateCurrMove()			{}
void Engine::updateCurrLine()			{}
void Engine::updateBestMove()			{}
void Engine::updateDepthInfo()		{}
void Engine::updateTimeInfo()			{}
void Engine::updateHashFullInfo()	{}


Engine::Protocol
Engine::protocol() const
{
	return dynamic_cast<uci::Engine const*>(m_engine) ? Uci : WinBoard;
}


::sys::Process&
Engine::process()
{
	M_REQUIRE(isConnected());
	return *m_process;
}


long
Engine::pid() const
{
	M_REQUIRE(isConnected());
	return m_process->pid();
}


void
Engine::kill()
{
	if (m_process)
	{
		m_active = false;
		m_process->close();
	}
}


void
Engine::invokeOption(mstl::string const& name)
{
	if (isActive())
		m_engine->invokeOption(name);
}


void
Engine::setHashRange(unsigned minSize, unsigned maxSize)
{
	minSize = mstl::max(4u, minSize);

	if (maxSize > minSize)
	{
		m_minHashSize = minSize;
		m_maxHashSize = maxSize;
		addFeature(Feature_Hash_Size);
	}
}


void
Engine::setThreadRange(unsigned minThreads, unsigned maxThreads)
{
	if (maxThreads > minThreads)
	{
		m_minThreads = minThreads;
		m_maxThreads = maxThreads;
		addFeature(Feature_Threads);
	}
}


void
Engine::setPlayingStyles(mstl::string const& styles)
{
	if (styles.find(',') != mstl::string::npos)
	{
		m_playingStyles = styles;
		addFeature(Feature_Playing_Styles);
	}
}


void
Engine::setMaxMultiPV(unsigned n)
{
	m_maxMultiPV = mstl::max(n, 1u);

	if (m_maxMultiPV > 1)
		addFeature(Feature_Multi_PV);
}


void
Engine::setEloRange(unsigned minElo, unsigned maxElo)
{
	if (minElo < maxElo)
	{
		if (m_useLimitedStrength)
			addFeature(Feature_Limit_Strength);

		m_minElo = minElo;
		m_maxElo = maxElo;
	}
}


void
Engine::setSkillLevelRange(unsigned minLevel, unsigned maxLevel)
{
	if (minLevel < maxLevel)
	{
		m_minSkillLevel = minLevel;
		m_maxSkillLevel = maxLevel;
		addFeature(Feature_Skill_Level);
	}
}


void
Engine::setScore(unsigned no, int score)
{
	M_ASSERT(no < numVariations());

	int bestScore = m_bestScore;

	m_scores[no] = score;
	m_mates[no] = 0;

	if (color::isWhite(currentBoard().sideToMove()))
	{
		m_bestScore = *mstl::max_element(m_scores, m_scores + m_numVariations);

		if (m_shortestMate < 0 || *mstl::max_element(m_mates, m_mates + m_numVariations) == 0)
			m_shortestMate = 0;

		m_sortScores[no] = score;
	}
	else
	{
		m_bestScore = *mstl::min_element(m_scores, m_scores + m_numVariations);

		if (m_shortestMate > 0 || *mstl::min_element(m_mates, m_mates + m_numVariations) == 0)
			m_shortestMate = 0;

		m_sortScores[no] = -score;
	}

	if (bestScore != m_bestScore)
		m_bestInfoHasChanged = true;

	if (m_ordering == BestFirst)
		reorderBestFirst(no);

	if (m_shortestMate == 0)
	{
		Selection selection = m_selection;

		m_selection.reset();

		for (unsigned i = 0; i < m_numVariations; ++i)
		{
			if (m_scores[i] == m_bestScore)
				m_selection.set(i);
		}

		if (m_selection != selection)
			m_bestInfoHasChanged = true;
	}
}


void
Engine::setMate(unsigned no, int numMoves)
{
	M_ASSERT(no < numVariations());

	int shortestMate = m_shortestMate;

	m_mates[no] = numMoves;

	int maxNegative	= INT_MIN;
	int minNegative	= INT_MAX;
	int maxPositive	= INT_MIN;
	int minPositive	= INT_MAX;
	int zeroIndex		= -1;

	for (unsigned i = 0; i < m_numVariations; ++i)
	{
		int mate		= m_mates[i];
		int score	= m_scores[i];

		if (i == no || (score != INT_MIN && score != INT_MAX))
		{
			if (mate < 0)
			{
				maxNegative = mstl::max(maxNegative, mate);
				minNegative = mstl::min(minNegative, mate);
			}
			else if (mate > 0)
			{
				maxPositive = mstl::max(maxPositive, mate);
				minPositive = mstl::min(minPositive, mate);
			}
			else
			{
				zeroIndex = i;
			}
		}
	}

	if (color::isWhite(currentBoard().sideToMove()))
	{
		if (minPositive != INT_MAX)
			m_shortestMate = minPositive;
		else if (zeroIndex >= 0)
			m_shortestMate = 0;
		else if (minNegative != INT_MAX)
			m_shortestMate = minNegative;

		if (numMoves > 0)
			m_sortScores[no] = INT_MAX - numMoves;
		else
			m_sortScores[no] = INT_MIN - numMoves;

		m_scores[no] = m_sortScores[no];
	}
	else
	{
		if (maxNegative != INT_MIN)
			m_shortestMate = maxNegative;
		else if (zeroIndex >= 0)
			m_shortestMate = 0;
		else if (maxPositive != INT_MIN)
			m_shortestMate = maxPositive;

		if (numMoves < 0)
			m_sortScores[no] = INT_MAX + numMoves;
		else
			m_sortScores[no] = INT_MIN + numMoves;
	}

	if (numMoves < 0)
		m_scores[no] = INT_MIN - numMoves;
	else
		m_scores[no] = INT_MAX - numMoves;

	if (shortestMate != m_shortestMate)
		m_bestInfoHasChanged = true;

	if (m_ordering == BestFirst)
		reorderBestFirst(no);

	if (m_shortestMate != 0)
	{
		Selection selection = m_selection;

		m_selection.reset();

		for (unsigned i = 0; i < m_numVariations; ++i)
		{
			if (m_mates[i] == m_shortestMate)
				m_selection.set(i);
		}

		if (m_selection != selection)
			m_bestInfoHasChanged = true;
	}
}


void
Engine::addFeature(unsigned feature)
{
	if (feature == Feature_Limit_Strength && m_maxElo > 0)
		m_useLimitedStrength = true;
	else
		m_features |= feature;
}


void
Engine::removeFeature(unsigned feature)
{
	if (feature & Feature_Limit_Strength)
		m_useLimitedStrength = false;

	m_features &= ~feature;
}


bool
Engine::isAlive()
{
	return m_active && m_process->isAlive();
}


bool
Engine::isActive() const
{
	return m_active;
}


unsigned
Engine::numThreads() const
{
	return mstl::min(m_maxThreads, mstl::max(m_minThreads, m_numThreads));
}


void
Engine::activate()
{
	if (!m_process)
	{
		m_process = new Process(this, m_command, m_directory);
	}
	else if (!m_active)
	{
		m_engine->protocolStart(false);

		if (!m_script.empty())
		{
			dynamic_cast<winboard::Engine*>(m_engine)->sendConfiguration(m_script);
			m_script.clear();
		}

		m_active = true;
	}
}


void
Engine::deactivate()
{
	if (m_active)
		m_engine->protocolEnd();

	m_active = false;
	m_script.clear();
}


void
Engine::log(mstl::string const& msg)
{
	if (m_logStream && !msg.empty())
	{
		m_buffer.assign("< ", 2);
		m_buffer.append(msg);
		m_buffer.append('\n');
		m_logStream->write(m_buffer);
	}
}


void
Engine::error(mstl::string const& msg)
{
	if (m_logStream)
	{
		m_buffer.assign("! ", 2);
		m_buffer.append(msg);
		m_buffer.append('\n');
		m_logStream->write(m_buffer);
	}
}


void
Engine::fatal(mstl::string const& msg)
{
	if (m_logStream)
	{
		m_buffer.assign("@ ", 2);
		m_buffer.append(msg);
		m_buffer.append('\n');
		m_logStream->write(m_buffer);
	}
}


void
Engine::readyRead()
{
	M_REQUIRE(isConnected());

	mstl::string line;

	while (m_process->gets(line) > 0)
	{
		line.trim();

		if (!line.empty())
		{
			log(line);
			m_engine->processMessage(line);
		}
	}
}


void
Engine::exited()
{
	m_active = false;

	if (m_process->wasCrashed())
	{
		fatal("Engine crashed");
		engineSignal(Crashed);
	}
	else if (m_process->wasKilled())
	{
		fatal("Engine killed");
		engineSignal(Killed);
	}
	else if (m_process->pipeWasClosed())
	{
		fatal("Engine closed pipe");
		engineSignal(PipeClosed);
	}
	else
	{
		mstl::string msg;
		msg.format("Engine terminated with exit status %d", m_exitStatus = m_process->exitStatus());
		fatal(msg);
		engineSignal(Terminated);
	}

	delete m_process;
	m_process = 0;
}


void
Engine::stopped()
{
	fatal("Engine stopped");
	engineSignal(Stopped);
}


void
Engine::resumed()
{
	fatal("Engine resumed");
	engineSignal(Resumed);
}


void
Engine::send(mstl::string const& msg)
{
	M_REQUIRE(isConnected());

	if (m_logStream)
	{
		m_buffer.assign("> ", 2);
		m_buffer.append(msg);
		m_buffer.append('\n');
		m_logStream->write(m_buffer);
	}
	m_process->puts(msg);
}


void
Engine::send(char const* message)
{
	mstl::string str;
	str.hook(const_cast<char*>(message), ::strlen(message));
	send(str);
}


bool
Engine::pause()
{
	M_REQUIRE(isActive());

	if (!isAnalyzing())
		return false;

	m_engine->pause();
	return true;
}


bool
Engine::resume()
{
	M_REQUIRE(isActive());

	if (currentGame() == 0)
		return false;

	m_engine->resume();
	return true;
}


Engine::Result
Engine::probe(unsigned timeout)
{
	Result result = Probe_Failed;

	sys::Timer timer(1000);

	m_probe = true;

	try
	{
		activate();
		m_active = true;

		while (m_process && !m_process->isConnected() && !timer.expired())
			timer.doNextEvent();

		if (!m_process)
			return Probe_Failed;

		m_engine->protocolStart(true);

		timer.restart(m_engine->probeTimeout());

		while (result != Probe_Successfull && !timer.expired())
		{
			timer.doNextEvent();
			result = m_engine->probeResult();
		}
	}
	catch (mstl::exception const& exc)
	{
		deactivate();
		m_probe = false;
		throw exc;
	}

	m_probe = false;

	if (result == Probe_Successfull && !hasFeature(Feature_Analyze))
	{
		Result analyzeResult = Probe_Failed;
		Game game;

		m_probeAnalyze = true;
		m_game = &game;
		addFeature(Feature_Analyze);
		m_engine->startAnalysis(true);
		removeFeature(Feature_Analyze);
		timer.restart(1000);

		try
		{
			while (!hasFeature(Feature_Analyze) && !timer.expired())
			{
				timer.doNextEvent();
				analyzeResult = m_engine->probeAnalyzeFeature();
			}
		}
		catch (mstl::exception const& exc)
		{
			deactivate();
			m_probe = false;
			throw exc;
		}

		m_probeAnalyze = false;
		m_game = 0;

		if (analyzeResult)
			addFeature(Feature_Analyze);
	}

	deactivate();

	return result;
}


bool
Engine::startAnalysis(db::Game const* game)
{
	M_REQUIRE(game);
	M_REQUIRE(isActive());

	if (!hasFeature(Feature_Analyze))
	{
		error(No_Analyze_Mode);
		return false;
	}

	bool isNew = m_game ? game->id() != m_game->id() : true;

	if (isNew)
	{
		m_currentVariant = Variant_Standard;

		if (!variant::isStandardChess(game->idn()))
		{
			if (	variant::isShuffleChess(game->idn())
				|| variant::isChess960(game->idn())
				|| game->startBoard().notDerivableFromStandardChess())
			{
				if (!hasVariant(Variant_Chess_960))
				{
					error(Chess_960_Not_Supported);
					return false;
				}

				m_currentVariant = Variant_Chess_960;
			}
		}

		if (m_currentVariant == Variant_Standard && !hasVariant(Variant_Standard))
		{
			error(Standard_Chess_Not_Supported);
			return false;
		}
	}
	else if (isAnalyzing())
	{
		if (m_engine->currentBoard().isEqualPosition(game->currentBoard()))
			return true;
	}

	m_game = game;
	m_gameId = game->id();

	if (m_engine->isReady())
	{
		if (isAnalyzing() && !m_engine->stopAnalysis())
			return false;

		int score = color::isWhite(game->currentBoard().sideToMove()) ? INT_MIN : INT_MAX;

		for (unsigned i = 0; i < m_numVariations; ++i)
		{
			m_variations[i].clear();
			m_sortScores[i] = INT_MIN;
			m_scores[i] = score;
			m_mates[i] = 0;
			m_map[i] = i;
		}

		resetInfo();
		m_bestIndex = 0;
		m_bestInfoHasChanged = false;
		m_selection.reset();
		m_currMove.clear();
		m_bestMove.clear();
		m_ponder.clear();
		// clear currline

		clearInfo();

		if (!m_engine->startAnalysis(isNew))
			return false;
	}

	return true;
}


bool
Engine::stopAnalysis()
{
	if (!isAnalyzing())
		return false;

	m_engine->stopAnalysis();
	return true;
}


bool
Engine::doMove(db::Move const& lastMove)
{
	if (!m_engine->isReady())
		return false;

	m_engine->doMove(lastMove);
	return true;
}


unsigned
Engine::changeNumberOfVariations(unsigned n)
{
	n = mstl::min(n, MaxNumVariations);

	if (n != m_numVariations)
	{
		if (n < m_numVariations)
		{
			Selection used;

			for (unsigned i = 0; i < n; ++i)
			{
				if (m_map[i] < n)
					used.set(i);
			}

			for (unsigned i = 0; i < n; ++i)
			{
				if (m_map[i] >= n)
				{
					unsigned firstFree = used.find_first_not();
					used.set(firstFree);
					m_map[i] = firstFree;
				}
			}
		}
		else
		{
			for (unsigned i = m_numVariations; i < n; ++i)
				m_map[i] = i;
		}

		m_variations.resize(n);
		m_numVariations = n;

		if (isActive())
			m_engine->sendNumberOfVariations();
	}

	return n;
}


unsigned
Engine::changeHashSize(unsigned size)
{
	size = mstl::max(4u, size);

	if (size != m_hashSize)
	{
		m_hashSize = size;

		if (isActive() && hasFeature(Feature_Hash_Size))
			m_engine->sendHashSize();
	}

	return size;
}


unsigned
Engine::changeCores(unsigned n)
{
	if (n != m_numCores)
	{
		m_numCores = n;
		m_numThreads = n - 1;

		if (isActive())
		{
			if (hasFeature(Feature_SMP))
				m_engine->sendCores();
			else if (hasFeature(Feature_Threads))
				m_engine->sendThreads();
		}
	}

	return n;
}


unsigned
Engine::changeThreads(unsigned n)
{
	if (n != m_numThreads)
	{
		m_numThreads = n;
		m_numCores = mstl::min(n + 1, sys::info::numberOfProcessors());

		if (isActive())
		{
			if (hasFeature(Feature_Threads))
				m_engine->sendThreads();
			else if (hasFeature(Feature_SMP))
				m_engine->sendCores();
		}
	}

	return n;
}


unsigned
Engine::changeStrength(unsigned elo)
{
	if (elo != m_strength)
	{
		m_strength = elo;

		if (isActive() && hasFeature(Feature_Limit_Strength))
			m_engine->sendStrength();
	}

	return elo;
}


unsigned
Engine::changeSkillLevel(unsigned level)
{
	if (level != m_skillLevel)
	{
		m_skillLevel = level;

		if (isActive() && hasFeature(Feature_Skill_Level))
			m_engine->sendSkillLevel();
	}

	return level;
}


mstl::string const&
Engine::changePlayingStyle(mstl::string const& style)
{
	m_playingStyle = style;

	if (isActive() && hasFeature(Feature_Playing_Styles))
		m_engine->sendPlayingStyle();

	return m_playingStyle;
}


bool
Engine::playOther(bool flag)
{
	m_playOther = flag;

	if (isActive())
		m_engine->sendPlayOther();

	return flag;
}


bool
Engine::pondering(bool flag)
{
	m_pondering = flag;

	if (isActive())
		m_engine->sendPondering();

	return flag;
}


void
Engine::setBestMove(db::Move const& move)
{
	m_bestMove = move;
	updateBestMove();
}


void
Engine::reorderBestFirst(unsigned currentNo)
{
	if (m_numVariations == 1)
		return;

	Map old;
	::memcpy(old, m_map, sizeof(old[0])*m_numVariations);

	for (unsigned k = 0; k < m_numVariations; ++k)
		m_map[k] = k;

	for (unsigned k = 0, n = m_numVariations - 1; k < n; ++k)
	{
		unsigned index = k;

		int score = m_sortScores[m_map[index]];

		for (unsigned i = k + 1; i < m_numVariations; ++i)
		{
			int score2 = m_sortScores[m_map[i]];

			if (score < score2)
			{
				score = score2;
				index = i;
			}
		}

		if (index > k)
			mstl::swap(m_map[index], m_map[k]);
	}

	for (unsigned i = 0; i < m_numVariations; ++i)
	{
		if (i != currentNo && m_map[i] != old[i])
			updatePvInfo(i);
	}
}


void
Engine::setOrdering(Ordering method)
{
	m_ordering = method;
}


void
Engine::setVariation(unsigned no, db::MoveList const& moves)
{
	M_REQUIRE(no < numVariations());

	int matchIndex = -1;

	if (m_numVariations > 1 && m_ordering == KeepStable && !isBestLine(no))
	{
		unsigned matchLength = 0;

		for (unsigned i = 0; i < m_numVariations; ++i)
		{
			unsigned length = moves.match(m_variations[i]);

			if (length > matchLength)
			{
				matchIndex = i;
				matchLength = length;
			}
		}

		if (matchIndex >= 0 && matchIndex != int(no) && isBestLine(matchIndex))
			mstl::swap(m_map[no], m_map[matchIndex]);
	}

	m_variations[no] = moves;

	if (matchIndex >= 0)
		updatePvInfo(matchIndex);
}


void
Engine::resetInfo()
{
	m_depth = 0;
	m_selDepth = 0;
	m_time = 0.0;
	m_nodes = 0;
}


void
Engine::addOption(mstl::string const& name,
						mstl::string const& type,
						mstl::string const& dflt,
						mstl::string const& var,
						mstl::string const& max)
{
	m_options.push_back();

	Option& opt = m_options.back();

	opt.name	= name;
	opt.type	= type;
	opt.val  = dflt;
	opt.dflt	= dflt;
	opt.var	= var;
	opt.max	= max;
}


void
Engine::setOption(mstl::string const& name, mstl::string const& value)
{
	for (Options::iterator i = m_options.begin(); i != m_options.end(); ++i)
	{
		if (i->name == name)
		{
			i->val = value;
			return;
		}
	}
}


void
Engine::updateOptions()
{
	if (isActive())
		m_engine->sendOptions();
}


void
Engine::updateConfiguration(mstl::string const& script)
{
	M_REQUIRE(protocol() == WinBoard);

	if (isActive())
		dynamic_cast<winboard::Engine*>(m_engine)->sendConfiguration(script);
	else
		m_script.assign(script);
}


void
Engine::clearHash()
{
	if (isActive() && hasFeature(Feature_Clear_Hash))
		m_engine->clearHash();
}


void
Engine::setIdentifier(mstl::string const& name)
{
	m_identifierSet = true;
	m_identifier = name;
	::compactSpaces(m_identifier);
}


void
Engine::setShortName(mstl::string const& name)
{
	m_shortName = name;
	::compactSpaces(m_shortName);

	if (!m_identifierSet)
		m_identifier = m_shortName;
}


void
Engine::setEmail(mstl::string const& address)
{
	for (mstl::string::const_iterator i = address.begin(); i != address.end(); ++i)
	{
		if (!::isalnum(*i) && ::strchr("_-.@", *i) == 0)
			return; // invalid character in Email address
	}

	mstl::string::size_type i = address.find_first_of('@');
	mstl::string::size_type k = address.find_last_of('.');

	if (	i != mstl::string::npos
		&& k != mstl::string::npos
		&& k > i + 2
		&& i > 5
		&& (k + 2 == address.size() || k + 3 == address.size()))
	{
		m_email.assign(address); // ok, seems to be a plausible Email address
	}
}


void
Engine::setUrl(mstl::string const& address)
{
	mstl::string url;

	if (::strncmp(address, "www.", 4) == 0)
		url.append("http://");

	url.append(address);

	if ((::strncmp(url, "http://", 7) == 0 || ::strncmp(url, "ftp://", 6)))
	{
		for (mstl::string::const_iterator i = url.begin(); i != url.end(); ++i)
		{
			if (!::isalnum(*i) && ::strchr("_/-.", *i) == 0)
				return; // invalid character in URL
		}

		mstl::string::size_type k = url.find_last_of('.');

		if (	k != mstl::string::npos
			&& k > 5
			&& (k + 2 == url.size() || k + 3 == url.size()))
		{
			m_url.swap(url); // ok, seems to be a plausible URL
		}
	}
}


bool
Engine::detectShortName(mstl::string const& s, bool setId)
{
	mstl::string str(s);
	::compactSpaces(str);

	bool detected = false;

	if (::Range range = ::findShortName(Range(str.begin(), str.end())))
	{
		setShortName(mstl::string(range.start, range.end));
		detected = true;

		if (setId)
		{
			char const* q = range.end + 1;
			char const* p = ::findVersionNumber(q);

			if (!p)
				p = ::findRomanNumber(q);

			if (!p)
				p = range.end;

			setIdentifier(mstl::string(range.start, p));
		}
	}

	return detected;
}


bool
Engine::detectUrl(mstl::string const& str)
{
	mstl::string::size_type n = str.find("http://");

	if (n == mstl::string::npos)
		n = str.find("ftp://");
	if (n == mstl::string::npos)
		n = str.find("www.");

	if (n != mstl::string::npos)
	{
		char const* t = str.begin() + n;
		char const* u = str;
		char const* e = str.end();

		while (u < e && (::isalnum(*u) || ::strchr("_/-.", *u)))
			++u;

		mstl::string url(m_url);
		setUrl(mstl::string(t, u));

		if (url != m_url)
			return true;
	}

	return false;
}


bool
Engine::detectEmail(mstl::string const& str)
{
	mstl::string::size_type n = str.find("@");

	char const* s = str.begin();
	char const* t = s + n;
	char const* u = s + n;
	char const* e = str.end();

	while (t > s && (::isalnum(*t) || ::strchr("_-.", *t)))
		--t;
	while (u < e && (::isalnum(*u) || ::strchr("_-.", *u)))
		++u;

	if (s + n <= u - 5 && t + 4 <= s + n)
	{
		mstl::string email(m_email);
		setEmail(mstl::string(t, u));

		if (email != m_email)
			return true;
	}

	return false;
}

// vi:set ts=3 sw=3:
