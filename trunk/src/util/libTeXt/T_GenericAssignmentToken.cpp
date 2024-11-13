// ======================================================================
// Author : $Author$
// Version: $Revision: 193 $
// Date   : $Date: 2012-01-16 09:55:54 +0000 (Mon, 16 Jan 2012) $
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

#include "T_GenericAssignmentToken.h"

using namespace TeXt;


GenericAssignmentToken::GenericAssignmentToken(mstl::string const& name, Func func, Type type)
	:GenericFinalToken(name, func, type)
	,m_uplevel(0)
{
}


int
GenericAssignmentToken::uplevel() const
{
	return m_uplevel;
}


bool
GenericAssignmentToken::setUplevel(int level)
{
	m_uplevel = level;
	return true;
}

// vi:set ts=3 sw=3:
