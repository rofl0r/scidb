// ======================================================================
// Author : $Author$
// Version: $Revision: 84 $
// Date   : $Date: 2011-07-18 18:02:11 +0000 (Mon, 18 Jul 2011) $
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

#include "tcl_progress.h"
#include "tcl_exception.h"
#include "tcl_base.h"

#include <tcl.h>

using namespace tcl;


Progress::Initializer Progress::m_initializer;

Tcl_Obj* Progress::m_open			= 0;
Tcl_Obj* Progress::m_close			= 0;
Tcl_Obj* Progress::m_start			= 0;
Tcl_Obj* Progress::m_update		= 0;
Tcl_Obj* Progress::m_finish		= 0;
Tcl_Obj* Progress::m_interrupted	= 0;
Tcl_Obj* Progress::m_ticks			= 0;


inline
void
checkResult(int rc, Tcl_Obj* cmd, Tcl_Obj* subcmd, Tcl_Obj* arg)
{
//	if (rc != TCL_OK)
//	{
//		TCL_RAISE(	"'%s %s %s' failed",
//						Tcl_GetStringFromObj(cmd, 0),
//						Tcl_GetStringFromObj(subcmd, 0),
//						Tcl_GetStringFromObj(arg, 0));
//	}
}


Progress::Initializer::Initializer()
{
	Tcl_IncrRefCount(m_open				= Tcl_NewStringObj("open",				-1));
	Tcl_IncrRefCount(m_close			= Tcl_NewStringObj("close",			-1));
	Tcl_IncrRefCount(m_start			= Tcl_NewStringObj("start",			-1));
	Tcl_IncrRefCount(m_update			= Tcl_NewStringObj("update",			-1));
	Tcl_IncrRefCount(m_finish			= Tcl_NewStringObj("finish",			-1));
	Tcl_IncrRefCount(m_interrupted	= Tcl_NewStringObj("interrupted?",	-1));
	Tcl_IncrRefCount(m_ticks			= Tcl_NewStringObj("ticks",			-1));
}


Progress::Progress(Tcl_Obj* cmd, Tcl_Obj* arg)
	:m_cmd(cmd)
	,m_arg(arg)
	,m_maximum(0)
	,m_numTicks(-1)
	,m_sendFinish(false)
	,m_firstStart(true)
{
	invoke(__func__, m_cmd, m_open, m_arg, nullptr);
}


Progress::~Progress() throw()
{
	if (m_sendFinish)
		sendFinish();

	invoke(__func__, m_cmd, m_close, m_arg, nullptr);
}


unsigned
Progress::ticks() const
{
	if (m_numTicks == -1)
	{
		m_numTicks = 0;

		Tcl_Obj* result = call(__func__, m_cmd, m_ticks, m_arg, 0);

		if (result)
		{
			Tcl_GetIntFromObj(interp(), result, &m_numTicks);
			Tcl_DecrRefCount(result);

			if (m_numTicks < 0)
				m_numTicks = 0;
		}
	}

	return m_numTicks;
}


bool
Progress::interrupted()
{
	Tcl_Obj* result = call(__func__, m_cmd, m_interrupted, m_arg, 0);

	int rc;
	bool ok = result && (Tcl_GetIntFromObj(interp(), result, &rc) == TCL_OK) && rc;
	Tcl_DecrRefCount(result);

	return ok;
}


void
Progress::start(unsigned total)
{
	if (!m_firstStart)
	{
		sendFinish();
		m_firstStart = false;
	}

	Tcl_Obj* maximum = Tcl_NewLongObj(m_maximum = total);
	Tcl_IncrRefCount(maximum);
	int rc = invoke(__func__, m_cmd, m_start, m_arg, maximum, nullptr);
	Tcl_DecrRefCount(maximum);
	m_sendFinish = rc == TCL_OK;
	checkResult(rc, m_cmd, m_start, m_arg);
}


void
Progress::update(unsigned progress)
{
	Tcl_Obj* value = Tcl_NewLongObj(progress);
	Tcl_IncrRefCount(value);
	int rc = invoke(__func__, m_cmd, m_update, m_arg, value, nullptr);
	Tcl_DecrRefCount(value);
	checkResult(rc, m_cmd, m_update, m_arg);
}


int
Progress::sendFinish() throw()
{
	Tcl_Obj* maximum = Tcl_NewLongObj(m_maximum);
	Tcl_IncrRefCount(maximum);
	int rc = invoke(__func__, m_cmd, m_finish, m_arg, maximum, nullptr);
	Tcl_DecrRefCount(maximum);
	return rc;
}


void
Progress::finish()
{
	int rc = sendFinish();
	m_sendFinish = rc != TCL_OK;
	checkResult(rc, m_cmd, m_finish, m_arg);
}

// vi:set ts=3 sw=3:
