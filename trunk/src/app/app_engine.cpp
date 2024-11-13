// ======================================================================
// Author : $Author$
// Version: $Revision: 427 $
// Date   : $Date: 2012-09-17 12:16:36 +0000 (Mon, 17 Sep 2012) $
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

#include "sys_process.h"
#include "sys_timer.h"

#include "u_misc.h"

#include "m_ostream.h"

using namespace app;
using namespace db;
using namespace util::misc::file;


struct Engine::Process : public sys::Process
{
	Process(Engine* engine, mstl::string const& command, mstl::string const& directory);

	bool isConnected() const { return m_connected; }

	void readyRead();
	void exited();

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


Engine::Concrete::~Concrete() throw() {}


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
	,m_maxMultiPV(1)
	,m_variations(1, MoveList())
	,m_numVariations(1)
	,m_hashSize(0)
	,m_searchMate(0)
	,m_limitedStrength(0)
	,m_features(0)
	,m_score(0)
	,m_mate(0)
	,m_depth(0)
	,m_time(0.0)
	,m_nodes(0)
	,m_active(false)
	,m_analyzing(false)
	,m_probe(false)
	,m_protocol(false)
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


Engine::~Engine() throw()
{
	if (m_process)
	{
		deactivate();
		delete m_process;
	}

	delete m_engine;
}


long
Engine::pid() const
{
	return m_process->pid();
}


void
Engine::kill()
{
	m_active = m_analyzing = false;
	m_process->kill();
}


void
Engine::setLog(mstl::ostream* stream)
{
	m_logStream = stream;
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
				m_engine->processMessage(line);
				log(line);
			}

			s = p + 1;
		}
	}
}


void
Engine::exited()
{
	if (m_active)
	{
		if (m_analyzing)
		{
			m_analyzing = false;
			stopAnalysis();
		}

		m_active = false;
	}

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

	bool result	= false;
	bool isNew	= m_game ? game->id() != m_game->id() : false;

	m_game = game;
	m_gameId = game->id();

	if (m_engine->isReady())
	{
		if ((result = m_engine->startAnalysis(isNew)))
			result = m_analyzing = true;
	}

	return result;
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

	return n;
}


unsigned
Engine::changeHashSize(unsigned size)
{
	if (!isActive())
		return 0;

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
	M_REQUIRE(no >= 1);
	M_REQUIRE(no <= numVariations());

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

// vi:set ts=3 sw=3:
