// ======================================================================
// Author : $Author$
// Version: $Revision: 1453 $
// Date   : $Date: 2017-12-11 14:27:52 +0000 (Mon, 11 Dec 2017) $
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

#include "u_path.h"

#include "m_utility.h"

using namespace util;


Path::Path(mstl::string const& name)
	:m_name(name)
{
}

#if HAVE_C11_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR

Path::Path(Path&& p) : m_name(mstl::move(m_name)) {}


Path&
Path::operator=(Path&& p)
{
	m_name = mstl::move(p.m_name);
	return *this;
}

#endif

// vi:set ts=3 sw=3:
