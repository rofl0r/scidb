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

#ifndef _TeXt_Miscellaneous_included
#define _TeXt_Miscellaneous_included

#include "T_Package.h"
#include "T_Token.h"

namespace TeXt {

class TextToken;

class Miscellaneous : public Package
{
public:

	typedef mstl::ref_counted_ptr<TextToken> TextTokenP;

	static TextTokenP getTextToken(Environment& env);

private:

	void doRegister(Environment& env) override;
};

} // namespace TeXt

#endif // _TeXt_Miscellaneous_included

// vi:set ts=3 sw=3:
