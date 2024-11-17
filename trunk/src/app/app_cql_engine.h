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
// Copyright: (C) 2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _app_cql_engine_included
#define _app_cql_engine_included

#include "cql_engine.h"

#include "app_engine.h"

#include "db_move.h"

#include "m_string.h"
#include "m_stack.h"

namespace sys { class Thread; }
namespace db  { class Game; }

namespace app {

class Engine;

namespace cql {

class Engine : public ::cql::Engine
{
public:

	typedef app::Engine::Command Command;

	class Creator : public ::cql::Engine::Creator
	{
	public:

		Creator(	app::Engine::Protocol protocol,
					Command const& command,
					mstl::string const& directory,
					unsigned hashSize,
					unsigned numThreads);

		Engine* createEngine() override;
		void destroyEngine(::cql::Engine* engine) override;

	private:

		typedef app::Engine::Protocol Protocol;

		Protocol			m_protocol;
		Command			m_command;
		mstl::string	m_directory;
		unsigned			m_hashSize;
		unsigned			m_numThreads;
	};

	Engine(::app::Engine* impl);
	~Engine();

	sys::Thread* thread() const;

	void setupThread(sys::Thread* currentThread);

	void newGame(db::variant::Type variant) override;
	void setup(::db::Board const& board) override;
	void move(::db::Move const& move) override;
	void enterVariation() override;
	void leaveVariation() override;

	bool searchMate(Mode mode, unsigned arg) override;

	float evaluate(Mode mode, unsigned arg) override;
	float evaluate(Mode mode, unsigned arg, ::db::Move const& move) override;

private:

	struct Variation
	{
		Variation();
		Variation(db::Move const& move);

		db::Move	move;
		unsigned length;
	};

	typedef mstl::stack<Variation> VariationStack;

	class MyEngine;
	friend class MyEngine;

	void setScore(float score, int mate);

	void setupMode(Mode mode, unsigned arg);

	app::Engine*	m_impl;
	sys::Thread*	m_currentThread;
	db::Game*		m_game;
	VariationStack	m_variationStack;
	float				m_score;
	int				m_mate;
};

} // namespace cql
} // namespace app

#endif // _app_cql_engine_included

// vi:set ts=3 sw=3:
