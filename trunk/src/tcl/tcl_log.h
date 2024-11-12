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

#ifndef _tcl_log_included
#define _tcl_log_included

#include "db_log.h"

extern "C" { struct Tcl_Obj; }

namespace tcl {

class Log : public ::db::Log
{
public:

	Log(Tcl_Obj* cmd, Tcl_Obj* arg);
	~Log() throw();

	bool error(::db::save::State code, unsigned gameNumber) override;

private:

	Tcl_Obj* m_cmd;
	Tcl_Obj* m_arg;
};

} // namespace tcl

#endif // _tcl_log_included

// vi:set ts=3 sw=3:
