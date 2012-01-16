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

#include "T_GenericConditionalToken.h"

using namespace TeXt;


GenericConditionalToken::GenericConditionalToken(mstl::string const& name, Func func)
	:Generic(T_Generic, name, func)
{
}


GenericConditionalToken::Type
GenericConditionalToken::type() const
{
	return Generic::type();
}


mstl::string
GenericConditionalToken::name() const
{
	return Generic::name();
}


void
GenericConditionalToken::perform(Environment& env)
{
	Generic::perform(env);
}

// vi:set ts=3 sw=3:
