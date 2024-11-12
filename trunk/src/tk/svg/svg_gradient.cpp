// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2008-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "svg_gradient.h"

using namespace svg;


gradient::attr::attr()
	:offset(0.0)
	,opacity(1.0)
	,color(255, 255, 255)
{
}


gradient::gradient()
	:x1(0.0)
	,y1(0.0)
	,x2(100.0)
	,y2(0.0)
	,userSpaceOnUse(false)
{
}

// vi:set ts=3 sw=3:
