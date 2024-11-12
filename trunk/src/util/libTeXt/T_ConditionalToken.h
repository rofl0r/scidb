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

#ifndef _TeXt_ConditionalToken_included
#define _TeXt_ConditionalToken_included

#include "T_ExpandableToken.h"

namespace TeXt {

class ConditionalToken : public ExpandableToken
{
public:

	bool isConditional() const;

protected:

	ConditionalToken();
};

} // namespace TeXt

#endif // _TeXt_ConditionalToken_included

// vi:set ts=3 sw=3:
