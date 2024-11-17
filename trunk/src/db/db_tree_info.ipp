// ======================================================================
// Author : $Author$
// Version: $Revision: 1500 $
// Date   : $Date: 2018-07-13 10:00:25 +0000 (Fri, 13 Jul 2018) $
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
// Copyright: (C) 2010-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_assert.h"

namespace db {

inline TreeInfo::Pair::Pair() : count(0), sum(0.0) {}

inline TreeInfo::Pair::operator bool () const	{ return count > 0; }

inline double TreeInfo::Pair::average() const	{ return sum/count; }
inline bool TreeInfo::hasLine() const				{ return m_line; }


inline
bool
TreeInfo::Pair::operator>(Pair const& p) const
{
	return sum/count > p.sum/p.count;
}


inline
void
TreeInfo::Pair::add(unsigned value)
{
	++count;
	sum += value;
}


inline Move const& TreeInfo::move() const									{ return m_move; }
inline Move& TreeInfo::move()													{ return m_move; }
inline Eco TreeInfo::eco() const												{ return m_eco; }
inline unsigned TreeInfo::frequency() const								{ return m_frequency; }
inline uint16_t TreeInfo::bestRating() const								{ return m_bestRating; }
inline uint16_t TreeInfo::lastYear() const								{ return m_lastYear; }
inline NamebasePlayer const& TreeInfo::bestPlayer() const			{ return *m_bestPlayer; }
inline NamebasePlayer const& TreeInfo::mostFrequentPlayer() const	{ return *m_mostFrequentPlayer; }
inline unsigned TreeInfo::result(result::ID type) const				{ return m_scoreCount[type]; }
#ifndef SUPPORT_TREE_INFO_FILTER
inline unsigned TreeInfo::firstGameIndex() const						{ return m_firstGameIndex; }
#endif

inline void TreeInfo::setEco(Eco code) { m_eco = code; }

#ifdef SUPPORT_TREE_INFO_FILTER
inline void TreeInfo::addGame(unsigned index) { m_filter.add(index); }
#endif


inline
Line const&
TreeInfo::line() const
{
	M_REQUIRE(hasLine());
	return *m_line;
}


inline
Line&
TreeInfo::line()
{
	M_REQUIRE(hasLine());
	return *m_line;
}


inline
unsigned
TreeInfo::perMille(unsigned value) const
{
	return m_frequency ? (1000*value)/m_frequency : 0u;
}


inline
uint16_t
TreeInfo::averageYear() const
{
	return uint16_t(m_averageYear.average() + 0.5);
}


inline
unsigned
TreeInfo::averageRating() const
{
	return m_averageRating.count >= 10 ? int(m_averageRating.average() + 0.5) : 0;
}


inline
unsigned
TreeInfo::score() const
{
	if (m_frequency == 0)
		return 0;

	unsigned sum =	mstl::mul2(m_scoreCount[result::White])
					 + m_scoreCount[result::Draw]
					 + m_scoreCount[result::Unknown];

	return (500*sum)/m_frequency;
}


inline
unsigned
TreeInfo::draws() const
{
	return m_scoreCount[result::Draw] ? perMille(m_scoreCount[result::Draw]) : 0;
}

} // namespace db

// vi:set ts=3 sw=3:
