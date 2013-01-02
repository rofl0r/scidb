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

#ifndef _TeXt_GenericConditionalToken_included
#define _TeXt_GenericConditionalToken_included

#include "T_ConditionalToken.h"
#include "T_Generic.h"

namespace TeXt {

class GenericConditionalToken : public ConditionalToken, public Generic
{
public:

	typedef Token::Type Type;

	GenericConditionalToken(mstl::string const& name, Func func);

	Type type() const override;
	mstl::string name() const override;
	void perform(Environment& env) override;
};

} // namespace TeXt

#endif // _TeXt_GenericConditionalToken_included

// vi:set ts=3 sw=3:
