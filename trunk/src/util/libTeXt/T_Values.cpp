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

#include "T_Values.h"
#include "T_GenericExpandableToken.h"
#include "T_GenericValueToken.h"
#include "T_Environment.h"
#include "T_TextToken.h"
#include "T_TextProducer.h"


using namespace TeXt;


static void
performMeaning(Environment& env)
{
	env.pushProducer(
		Environment::ProducerP(new TextProducer(env.getExpandableToken()->meaning())));
}


static void
performString(Environment& env)
{
	TokenP token = env.getUndefinedToken();

	if (token->type() == Token::T_List)
		env.putUnboundToken(TokenP(new TextToken(token->name(env))));
	else
		env.pushProducer(Environment::ProducerP(new TextProducer(token->name(env))));
}


void
Values::doRegister(Environment& env)
{
	env.bindMacro(new GenericExpandableToken("\\meaning", ::performMeaning));
	env.bindMacro(new GenericExpandableToken("\\string", ::performString));
	env.bindMacro(new GenericValueToken("\\escapechar", Token::T_Escapechar), Token::EscapeChar);
}

// vi:set ts=3 sw=3:
