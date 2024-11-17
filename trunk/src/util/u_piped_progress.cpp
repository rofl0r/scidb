// ======================================================================
// Author : $Author$
// Version: $Revision: 1502 $
// Date   : $Date: 2018-07-16 12:55:14 +0000 (Mon, 16 Jul 2018) $
// Url    : $URL$
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

#include "u_piped_progress.h"

#include "sys_thread.h"

#include "m_utility.h"
#include "m_assert.h"

using namespace util;


bool PipedProgress::interruptReceived() const { return m_interrupted; }


PipedProgress::PipedProgress(sys::Thread& thread)
	:m_thread(thread)
	,m_total(0)
	,m_interrupted(false)
	,m_prevValue(-1)
{
}


bool
PipedProgress::interrupted()
{
	if (!m_thread.testCancel() || !m_thread.testRunning())
		return false;

	if (!m_interrupted)
	{
		send(0);
		m_interrupted = true;
	}

	return true;
}


void
PipedProgress::start(unsigned total)
{
	m_total = total;
	setFrequency(mstl::max(1u, total/254));
	m_interrupted = false;
	m_prevValue = -1;
}


void
PipedProgress::update(unsigned progress)
{
	M_REQUIRE(!interruptReceived());

	if (progress < m_total)
	{
		unsigned c = unsigned((progress/m_total)*254.0 + 0.5);

		if (0 < c && c < 255 && int(c) != m_prevValue)
		{
			send(c);
			m_prevValue = c;
		}
	}
}


void
PipedProgress::finish() throw()
{
	if (!m_interrupted)
		send(255);
}

// vi:set ts=3 sw=3:
