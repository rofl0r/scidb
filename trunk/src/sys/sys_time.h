// ======================================================================
// Author : $Author$
// Version: $Revision: 36 $
// Date   : $Date: 2011-06-13 20:30:54 +0000 (Mon, 13 Jun 2011) $
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

#ifndef _sys_time_included
#define _sys_time_included

#include "u_base.h"

namespace sys {
namespace time {

struct Time
{
	Time();

	uint16_t	year;
	uint8_t  month;
	uint8_t  day;
	uint8_t  hour;
	uint8_t  minute;
	uint8_t  second;
};

/// Returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.
uint32_t time();

/// Returns a timestamp, measured in milliseconds.
uint64_t timestamp();

/// Convert UTC time to local time.
void localtime(uint32_t time, Time& tm);

} // namespace time
} // namespace sys

#endif // _sys_time_included

// vi:set ts=3 sw=3:
