// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
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
// Copyright: (C) 2010-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "db_namebases.h"

#include "m_assert.h"

using namespace db;


Namebases::Namebases()
	:m_player(Namebase::Player)
	,m_site(Namebase::Site)
	,m_event(Namebase::Event)
	,m_annotator(Namebase::Annotator)
	,m_round(Namebase::Round)
{
	M_ASSERT(&m_player + Namebase::Player    == &m_player   );
	M_ASSERT(&m_player + Namebase::Site      == &m_site     );
	M_ASSERT(&m_player + Namebase::Event     == &m_event    );
	M_ASSERT(&m_player + Namebase::Annotator == &m_annotator);
	M_ASSERT(&m_player + Namebase::Round     == &m_round);
}


void
Namebases::clear()
{
	m_player.clear();
	m_site.clear();
	m_event.clear();
	m_annotator.clear();
	m_round.clear();
}


void
Namebases::update()
{
	if (!m_player.isConsistent())
		m_player.update();
	if (!m_site.isConsistent())
		m_site.update();
	if (!m_event.isConsistent())
		m_event.update();
	if (!m_annotator.isConsistent())
		m_annotator.update();
	if (!m_round.isConsistent())
		m_round.update();
}

// vi:set ts=3 sw=3:
