// ======================================================================
// Author : $Author$
// Version: $Revision: 64 $
// Date   : $Date: 2011-07-01 23:42:38 +0000 (Fri, 01 Jul 2011) $
// Url    : $URL$
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
// ======================================================================

// ======================================================================
// Copyright: (C) 2010-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "db_provider.h"

using namespace db;

Provider::Provider(format::Type srcFormat) :m_format(srcFormat), m_index(-1) {}
Provider::~Provider() throw() {}

// vi:set ts=3 sw=3:
