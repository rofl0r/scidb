// ======================================================================
// Author : $Author$
// Version: $Revision: 193 $
// Date   : $Date: 2012-01-16 09:55:54 +0000 (Mon, 16 Jan 2012) $
// Url    : $URL$
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

#include "T_OutputFilter.h"
#include "T_Environment.h"

#include "m_assert.h"


using namespace TeXt;


OutputFilter::OutputFilter(unsigned priority)
	:m_priority(priority)
{
}


OutputFilter::~OutputFilter()
{
	// no action
}


OutputFilter&
OutputFilter::next()
{
	M_ASSERT(m_next);
	return *m_next;
}


void
OutputFilter::put(Environment& env, unsigned char c)
{
	next().put(env, c);
}


void
OutputFilter::put(Environment& env, mstl::string const& s)
{
	next().put(env, s);
}


void
OutputFilter::put(Environment& env, Value number)
{
	next().put(env, number);
}

// vi:set ts=3 sw=3:
