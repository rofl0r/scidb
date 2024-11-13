// ======================================================================
// Author : $Author$
// Version: $Revision: 433 $
// Date   : $Date: 2012-09-21 17:19:40 +0000 (Fri, 21 Sep 2012) $
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

#include "u_misc.h"

#include "m_ostream.h"
#include "m_vector.h"

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
		if (!m_engine->protocolAlreadyStarted())
			m_engine->concrete()->protocolStart(m_engine->isProbing());

		m_connected = true;
	}
}


void
Engine::Process::exited()
{
	m_engine->exited();
	m_connected = false;
}


Engine::Concrete::~Concrete() {}


unsigned
Engine::Concrete::maxVariations() const
{
	return 1;
}


Engine::Engine(Protocol protocol, mstl::string const& command, mstl::string const& directory)
	:m_engine(0)
	,m_game(0)
	,m_gameId(unsigned(-1))
	,m_command(command)
	,m_directory(directory)
	,m_identifier(rootname(basename(command)))
	,m_elo(0)
	,m_minElo(0)
	,m_maxElo(0)
	,m_skillLevel(0)
	,m_minSkillLevel(0)
	,m_maxSkillLevel(0)
	,m_maxMultiPV(1)
	,m_variations(1, MoveList())
	,m_numVariations(1)
	,m_hashSize(0)
	,m_minHashSize(0)
	,m_maxHashSize(0)
	,m_numThreads(0)
	,m_minThreads(0)
	,m_maxThreads(0)
	,m_searchMate(0)
	,m_limitedStrength(0)
	,m_features(0)
	,m_currMoveNumber(0)
	,m_score(0)
	,m_mate(0)
	,m_depth(0)
	,m_time(0.0)
	,m_nodes(0)
	,m_active(false)
	,m_analyzing(false)
	,m_probe(false)
	,m_protocol(false)
	,m_identifierSet(false)
	,m_useLimitedStrength(false)
	,m_process(0)
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
		deactivate();
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


long
Engine::pid() const
{
	return m_process->pid();
}


void
Engine::kill()
{
	m_active = m_analyzing = false;
	m_process->close();
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
	minThreads = mstl::max(4u, minThreads);

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
Engine::setLimitedStrength(unsigned elo)
{
	m_limitedStrength = mstl::max(m_maxElo, mstl::max(m_minElo, elo));
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
Engine::setSkillLevel(unsigned level)
{
	m_skillLevel = mstl::max(m_minSkillLevel, mstl::min(m_maxSkillLevel, level));
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
Engine::addFeature(unsigned feature)
{
	if (feature == Feature_Limit_Strength && m_maxElo > 0)
		m_useLimitedStrength = true;
	else
		m_features |= feature;
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


bool
Engine::isAnalyzing() const
{
	return m_analyzing;
}


bool
Engine::isProbing() const
{
	return m_probe;
}


void
Engine::activate()
{
	if (!m_process)
		m_process = new Process(this, m_command, m_directory);
	else if (!m_active)
		m_engine->protocolStart(false);

	m_active = true;
}


void
Engine::deactivate()
{
	if (m_active)
		m_engine->protocolEnd();

	m_active = false;
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

	engineTerminated(msg);
}


void
Engine::readyRead()
{
	mstl::string lines;
	mstl::string line;

	while (m_process->gets(lines) > 0)
	{
		char const* s = lines.begin();
		char const* e = lines.end();

		while (s < e)
		{
			char const *p = s;

			while (p < e && *p != '\n')
				++p;

			line.assign(s, p - s);
			line.trim();

			if (!line.empty())
			{
				log(line);
				m_engine->processMessage(line);
			}

			s = p + 1;
		}
	}
}


void
Engine::exited()
{
	if (m_process->wasCrashed())
	{
		fatal("Engine crashed");
	}
	else if (m_process->wasKilled())
	{
		fatal("Engine killed");
	}
	else if (m_process->pipeWasClosed())
	{
		fatal("Engine closed pipe");
	}
	else
	{
		mstl::string msg;
		msg.format("Engine terminated with exit status %d", m_process->exitStatus());
		fatal(msg);
	}

	m_analyzing = false;
	m_active = false;
	delete m_process;
	m_process = 0;
}


void
Engine::send(mstl::string const& msg)
{
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


Engine::Result
Engine::probe(unsigned timeout)
{
	Result result = Probe_Failed;

	sys::Timer timer(1000);

	m_probe = true;

	try
	{
		activate();

		while (m_process->isConnected() && !timer.expired())
			timer.doNextEvent();
	}
	catch (mstl::exception const& exc)
	{
		deactivate();
		m_probe = false;
		throw exc;
	}

	result = m_engine->probeResult();

	if (result != Probe_Successfull)
	{
		if (!m_process->isConnected())
		{
			// Seems to be a quiet engine. Start the protocol.
			m_protocol = true;
			m_engine->protocolStart(true);
		}

		timer.restart(mstl::max(timeout, m_engine->probeTimeout()));

		try
		{
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
	}

	deactivate();
	m_probe = false;

	return result;
}


bool
Engine::startAnalysis(db::Game const* game)
{
	M_REQUIRE(game);

	bool isNew = m_game ? game->id() != m_game->id() : true;

	m_game = game;
	m_gameId = game->id();

	if (m_engine->isReady())
	{
		if (m_analyzing)
		{
			m_analyzing = false;

			if (!m_engine->stopAnalysis())
				return false;
		}

		if (!m_engine->startAnalysis(isNew))
			return false;
		m_analyzing = true;
	}

	return true;
}


bool
Engine::stopAnalysis()
{
	bool result = true;

	if (m_analyzing)
	{
		result = m_engine->stopAnalysis();
		m_analyzing = false;
	}

	return result;
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
	if (!isActive())
		return 0;

	n = mstl::max(1u, mstl::min(n, m_engine->maxVariations()));

	if (n != m_numVariations)
	{
		m_numVariations = n;
		m_engine->sendNumberOfVariations();
	}

	m_variations.resize(n);
	return n;
}


unsigned
Engine::changeHashSize(unsigned size)
{
	if (!isActive())
		return 0;

	if (m_minHashSize > 0)
		size = mstl::min(m_maxHashSize, mstl::max(m_minHashSize, size));
	else
		size = mstl::max(4u, size);

	if (size != m_hashSize)
	{
		m_hashSize = size;

		if (hasFeature(Feature_Hash_Size))
			m_engine->sendHashSize();
	}

	return size;
}


void
Engine::setBestMove(db::Move const& move)
{
	m_bestMove = move;
	updateBestMove();
}


void
Engine::setVariation(db::MoveList const& moves, unsigned no)
{
	M_REQUIRE(no < numVariations());
	m_variations[no] = moves;
}


void
Engine::resetInfo()
{
	m_score = 0;
	m_mate = 0;
	m_depth = 0;
	m_time = 0.0;
	m_nodes = 0;

	for (unsigned i = 0; i < m_variations.size(); ++i)
		m_variations[i].clear();
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
Engine::changeOptions(Options const& options)
{
	m_options = options;
	m_engine->sendOptions();
}


void
Engine::clearHash()
{
	if (hasFeature(Feature_Clear_Hash))
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
