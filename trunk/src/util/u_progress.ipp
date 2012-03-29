// ======================================================================
// Author : $Author$
// Version: $Revision: 283 $
// Date   : $Date: 2012-03-29 18:05:34 +0000 (Thu, 29 Mar 2012) $
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

namespace util {

inline unsigned Progress::frequency() const { return m_freq; }
inline void Progress::setFrequency(unsigned frequency) { m_freq = frequency; }


inline
ProgressWatcher::ProgressWatcher(Progress& progress, unsigned total)
	:m_progress(&progress)
{
	m_progress->start(total);
}


inline
ProgressWatcher::ProgressWatcher(Progress* progress, unsigned total)
	:m_progress(progress)
{
	if (m_progress)
		m_progress->start(total);
}


inline ProgressWatcher::~ProgressWatcher()
{
	if (m_progress)
		m_progress->finish();
}

} // namespace util

// vi:set ts=3 sw=3:
