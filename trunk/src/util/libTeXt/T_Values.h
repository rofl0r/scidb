// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
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

#ifndef _TeXt_Values_included
#define _TeXt_Values_included

#include "T_Package.h"

namespace TeXt {

class Values : public Package
{
	void doRegister(Environment& env);
};

} // namespace TeXt

#endif // _TeXt_Values_included

// vi:set ts=3 sw=3:
