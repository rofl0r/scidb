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

#ifndef _tcl_http_included
#define _tcl_http_included

extern "C" { struct Tcl_Channel_; }

namespace tcl {

class Http
{
public:

	explicit Http(char const* host);

private:

	struct Tcl_Channel_* m_chan;
};

} // namespace tcl

#endif // _tcl_http_included

// vi:set ts=3 sw=3:
