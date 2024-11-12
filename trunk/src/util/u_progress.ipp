// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2011 Gregor Cramer
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
	:m_progress(progress)
{
	m_progress.start(total);
}


inline ProgressWatcher::~ProgressWatcher() { m_progress.finish(); }

} // namespace util

// vi:set ts=3 sw=3:
