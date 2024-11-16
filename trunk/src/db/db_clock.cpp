// ======================================================================
// Author : $Author$
// Version: $Revision: 1362 $
// Date   : $Date: 2017-08-03 10:35:52 +0000 (Thu, 03 Aug 2017) $
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
// Copyright: (C) 2011-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "db_clock.h"

#include "m_stdio.h"
#include "m_assert.h"

#include <stdlib.h>

using namespace db;


inline
static bool
isdelim(char c)
{
	return c == '\0' || c == ')' || c == ']';
}


inline
static char*
skipSpaces(char* s)
{
	while (*s == ' ')
		++s;

	return s;
}


inline
static char const*
skipSpaces(char const* s)
{
	while (*s == ' ')
		++s;

	return s;
}


char const*
Clock::parse(char const* s)
{
	M_REQUIRE(s);

	char* e = nullptr;

	m_second = m_minute = m_hour = 0;

	s = ::skipSpaces(s);
	m_hour = strtoul(s, &e, 10);
	e = skipSpaces(e);

	if (*e != ':' || m_hour >= 24)
	{
		m_hour = 0;
		return nullptr;
	}

	e = skipSpaces(e + 1);
	m_minute = strtoul(s = e, &e, 10);
	e = skipSpaces(e);

	if (m_minute >= 60)
	{
		m_hour = m_minute = 0;
		return nullptr;
	}

	if (::isdelim(*e))
		return e;

	if (*e != ':')
		return nullptr;

	e = skipSpaces(e + 1);
	m_second = strtoul(s = e, &e, 10);

	if (m_second > 60) // take leap second into account
	{
		m_hour = m_minute = m_second = 0;
		return nullptr;
	}

	return skipSpaces(e);
}


::util::crc::checksum_t
Clock::computeChecksum(util::crc::checksum_t crc) const
{
	crc = ::util::crc::compute(crc, m_hour);
	crc = ::util::crc::compute(crc, m_minute);
	crc = ::util::crc::compute(crc, m_second);

	return crc;
}


void
Clock::dump() const
{
	printf("%02u:%02u:%02u\n", m_hour, m_minute, m_second);
}

// vi:set ts=3 sw=3:
