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

#include "T_Input.h"

using namespace TeXt;


Input::Input()
	:m_lineNo(0)
{
}


Input::~Input()
{
	// no action
}


bool
Input::nextLine(mstl::string& result)
{
	bool rc = readNextLine(result);

	if (rc)
		++m_lineNo;

	return rc;
}

// vi:set ts=3 sw=3:
