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

#ifndef _TeXt_ConditionalToken_included
#define _TeXt_ConditionalToken_included

#include "T_ExpandableToken.h"

namespace TeXt {

class ConditionalToken : public ExpandableToken
{
public:

	bool isConditional() const override;

protected:

	ConditionalToken();
};

} // namespace TeXt

#endif // _TeXt_ConditionalToken_included

// vi:set ts=3 sw=3:
