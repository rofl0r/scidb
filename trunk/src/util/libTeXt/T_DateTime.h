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

#ifndef _TeXt_DateTime_included
#define _TeXt_DateTime_included

#include "T_Package.h"

namespace TeXt {

class DateTime : public Package
{
	void doRegister(Environment& env) override;
};

} // namespace TeXt

#endif // _TeXt_DateTime_included

// vi:set ts=3 sw=3:
