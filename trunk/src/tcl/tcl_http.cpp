// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
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

#include "tcl_http.h"
#include "tcl_exception.h"
#include "tcl_base.h"

#include <tcl.h>

using namespace tcl;

static int const PortNumber = 80;


Http::Http(char const* host)
{
	m_chan = Tcl_OpenTcpClient(interp(), ::PortNumber, host, 0, 0, true);

	if (!m_chan)
		TCL_RAISE("Tcl_OpenTcpClient() failed");
}

// vi:set ts=3 sw=3:
