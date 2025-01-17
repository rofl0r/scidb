// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
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
// Copyright: (C) 2009-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_assert.h"

namespace db {

inline bool Signature::hasPromotion() const			{ return m_promotions; }
inline bool Signature::hasUnderPromotion() const	{ return m_underPromotions; }

inline material::Signature Signature::material() const { return m_matSig; }
inline material::SigPart Signature::material(color::ID color) const { return m_matSig.part[color]; }
inline pawns::Side Signature::progress(color::ID color) const { return m_progress.side[color]; }
inline castling::Rights Signature::castling() const { return castling::Rights(m_castling); }
inline hp::Pawns Signature::homePawnsData() const { return m_homePawns; }

inline void Signature::addCastling(castling::Rights rights)		{ m_castling |= rights; }
inline void Signature::removeCastling(castling::Rights rights)	{ m_castling &= ~rights; }


inline
unsigned
Signature::hpCount() const
{
	return m_hpCount ? m_hpCount : (m_homePawns.value ? 16 : 0);
}


inline
void
Signature::setHomePawns(unsigned hpCount, hp::Pawns data)
{
	M_REQUIRE(hpCount <= 16);

	m_hpCount = hpCount == 16 ? 0 : hpCount;
	m_homePawns = data;
}


inline
void
Signature::clearHomePawns()
{
	m_hpCount = 0;
	m_homePawns.value = 0;
}


inline
bool
Signature::isReachableFinal(Signature const& target) const
{
	if (!isReachableFinalMaterial(target))
		return false;

	// check whether pawn structure is reachable
	return	isReachablePawnStructure(	m_progress.side[color::White],
													target.m_progress.side[color::White])
			&& isReachablePawnStructure(	m_progress.side[color::Black],
													target.m_progress.side[color::Black]);
}


inline
bool
Signature::isReachableFinal(Signature const& sig, uint16_t currentHpSig) const
{
	if (m_castling & ~sig.m_castling)
		return false;

	return isReachableFinalPosition(sig, currentHpSig);
}


inline
bool
Signature::isReachable(Signature const& current, Signature const& sig, uint16_t currentHpSig)
{
	return current.isReachableFinal(sig, currentHpSig);
}


inline
bool
Signature::isReachable(Signature const& current, Signature const& sig)
{
	return current.isReachableFinal(sig);
}


inline
bool
Signature::isReachablePosition(Signature const& current, Signature const& sig, uint16_t currentHpSig)
{
	return current.isReachableFinalPosition(sig, currentHpSig);
}


inline
bool
Signature::isReachablePawns(Signature const& sig) const
{
	if (sig.m_progress.side[0].rank[0] & ~m_progress.side[0].rank[0])
		return false;

	if (sig.m_progress.side[1].rank[0] & ~m_progress.side[1].rank[0])
		return false;

	return true;
}


inline
bool
Signature::isReachablePawnStructure(pawns::Progress lhs, pawns::Progress rhs)
{
	return	isReachablePawnStructure(lhs.side[0], rhs.side[0])
			&& isReachablePawnStructure(lhs.side[1], rhs.side[1]);
}

} // namespace db

// vi:set ts=3 sw=3:
