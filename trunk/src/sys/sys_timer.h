// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _sys_timer_included
#define _sys_timer_included

extern "C" { struct Tcl_TimerToken_; }

namespace sys {

class Timer
{
public:

	Timer(unsigned timeout);
	virtual ~Timer() throw();

	bool expired() const;

	void doNextEvent();

	virtual void timeout();

private:

	typedef struct Tcl_TimerToken_* Token;

	static void timerEvent(void* clientData);

	Token	m_token;
	bool	m_expired;
};

} // namespace sys

#endif // _sys_timer_included

// vi:set ts=3 sw=3:
