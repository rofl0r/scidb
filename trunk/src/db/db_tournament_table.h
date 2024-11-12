// ======================================================================
// Author : $Author$
// Version: $Revision: 30 $
// Date   : $Date: 2011-05-23 14:49:04 +0000 (Mon, 23 May 2011) $
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
// Copyright: (C) 2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_tournament_table_included
#define _db_tournament_table_included

#include "db_date.h"
#include "db_common.h"

#include "T_Base.h"

#include "m_map.h"
#include "m_chunk_allocator.h"

namespace TeXt { class Receptacle; }

namespace db {

class Database;
class NamebaseEvent;
class NamebasePlayer;
class Filter;

class TournamentTable
{
public:

	enum Mode
	{
		Auto,
		Crosstable,
		Scheveningen,
		Swiss,
		Match,
		Knockout,
		RankingList,
	};

	enum Order
	{
		Score,
		Alphabetical,
		Rating,
		Federation,
	};

	enum KnockoutOrder
	{
		Triangle,
		Pyramid,
	};

	enum Tiebreak
	{
		Buchholz,					// swiss
		MedianBuchholz,			// swiss
		ModifiedMedianBuchholz,	// swiss
		SonnebornBerger,			// all-play-all, swiss
		Progressive,				// swiss
		KoyaSystem,					// all-play-all
		GamesWon,					// all-play-all
		RefinedBuchholz,			// swiss
		None,

		LastBuchholz = SonnebornBerger,
		LastTiebreak = RefinedBuchholz,
	};

	typedef Tiebreak TiebreakRules[4];

	TournamentTable(Database const& db, NamebaseEvent const& event, Filter const& gameFilter);
	~TournamentTable() throw();

	Mode mode() const;
	Mode bestMode() const;
	unsigned averageElo() const;
	unsigned fideCategory() const;
	unsigned countPlayers() const;
	NamebasePlayer const* getPlayer(unsigned ranking) const;

	void emit(	TeXt::Receptacle& receptacle,
					TiebreakRules const& tiebreakRules,
					Order order,
					KnockoutOrder koOrder = Triangle,
					Mode mode = Auto);

	static unsigned fideCategory(unsigned elo);
	static unsigned computeEloPerformance(unsigned opponentAverage, unsigned percentage);
	static unsigned computeDWZPerformance(unsigned opponentAverage, unsigned percentage);
	static int ratingChange(int elo, int oppAvg, int percentage, int numGames);

	// private, although must be public due to technical reasons:

	class Player;
	friend class Player;

	struct Clash
	{
		Player*		player;
		Clash*		opponent;
		result::ID	result;
		unsigned		progress;
		unsigned		gameIndex;
		color::ID	color;
		unsigned		round;
		unsigned		subround;
		Date			date;
		bool			deleted;

		termination::Reason termination;
	};

private:

	typedef mstl::map<unsigned,Player*> PlayerMap;
	typedef mstl::vector<unsigned> Map;
	typedef TeXt::Value Value;
	typedef mstl::chunk_allocator<Clash> Allocator;

	void buildList(Database const& db, Filter const& gameFilter);
	void eliminateDuplicates();
	void computeScores();
	void computePerformance();
	void guessBestMode();
	void computeTiebreaks();
	void sort(TiebreakRules const& tiebreakRules, Order order, Mode mode);

	void emitCrossTable(TeXt::Receptacle& receptacle, bool isScheveningen);
	void emitSwissTable(TeXt::Receptacle& receptacle);
	void emitMatchTable(TeXt::Receptacle& receptacle);
	void emitKnockoutTable(TeXt::Receptacle& receptacle, KnockoutOrder order);

	NamebaseEvent const& m_event;

	Date			m_startDate;
	Date			m_endDate;
	Mode			m_bestMode;
	Mode			m_mode;
	Order			m_order;
	PlayerMap	m_playerMap;
	Value			m_resultCount[5];
	unsigned		m_avgElo;
	unsigned		m_numGames;
	unsigned		m_numRounds;
	unsigned		m_lastRound;
	unsigned		m_parity;
	unsigned		m_maxRound;
	unsigned		m_maxSubround;
	bool			m_excludeKnockout;
	Map			m_orderMap;
	Allocator	m_allocator;
};

} // namespace db

#include "db_tournament_table.ipp"

#endif // _db_tournament_table_included

// vi:set ts=3 sw=3:
