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

#include "u_path.h"

#include "m_utility.h"

using namespace util;


path::path(mstl::string const& name)
	:m_name(name)
{
}

#if HAVE_0X_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR

path::path(path&& p) : m_name(mstl::move(m_name)) {}


path&
path::operator=(path&& p)
{
	m_name = mstl::move(p.m_name);
	return *this;
}

#endif

// vi:set ts=3 sw=3:
