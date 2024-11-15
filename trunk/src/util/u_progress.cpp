// ======================================================================
// Author : $Author$
// Version: $Revision: 906 $
// Date   : $Date: 2013-07-22 20:44:36 +0000 (Mon, 22 Jul 2013) $
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

#include "u_progress.h"

#include "m_utility.h"
#include "m_limits.h"

using namespace util;


namespace {

struct Null : public Progress
{
	bool interrupted() override { return false; }
	unsigned ticks() const override { return 0; }
	void start(unsigned) override {}
	void message(mstl::string const&) override {}
	void tick(unsigned) override {}
	void update(unsigned) override {}
	void finish() throw() override {};
};

static Null m_null;

}

Progress::Progress() : m_freq(0) {}
Progress::~Progress() throw() {}

void Progress::start(unsigned) {}
void Progress::message(mstl::string const&) {}
void Progress::tick(unsigned) {}
void Progress::update(unsigned) {}
void Progress::finish() throw() {}
bool Progress::interrupted() { return false; }
unsigned Progress::ticks() const { return 0; }


unsigned
Progress::frequency(unsigned count, unsigned maximum) const
{
	unsigned ticks = this->ticks();

	if (ticks > 0)
		return mstl::max(1u, maximum ? mstl::min(maximum, count/ticks) : count/ticks);

	return m_freq ? mstl::min(maximum, m_freq) : maximum;
}


void
Progress::setCount(unsigned count)
{
	unsigned ticks = this->ticks();

	if (ticks > 0)
		setFrequency(mstl::max(1u, count/ticks));
}


Progress&
Progress::null()
{
	return m_null;
}

// vi:set ts=3 sw=3:
