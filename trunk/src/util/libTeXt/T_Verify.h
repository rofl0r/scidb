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

#ifndef _TeXt_Verify_included
#define _TeXt_Verify_included

#include "T_TokenP.h"

namespace TeXt {

class Environment;

namespace Verify {

bool controlSequenceToken(Environment& env, TokenP const& token);
bool finalToken(Environment& env, TokenP const& token);
bool unresolvedToken(Environment& env, TokenP const& token);
bool numberToken(Environment& env, TokenP const& token);
bool numericalToken(Environment& env, TokenP const& token);

} // namespace Verify
} // namespace TeXt

#endif // _TeXt_Verify_included

// vi:set ts=3 sw=3:
