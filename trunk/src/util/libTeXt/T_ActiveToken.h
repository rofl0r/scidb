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

#ifndef _TeXt_ActiveToken_included
#define _TeXt_ActiveToken_included

#include "T_Token.h"

#include "m_function.h"

namespace TeXt {

class ActiveToken : public Token
{
public:

	typedef mstl::function<void (Environment&)> Func;

	ActiveToken(unsigned char c, Func const& func);
	ActiveToken(unsigned char c, TokenP const& macro);

	bool isBound() const;
	bool isEqualTo(Token const& token) const;

	RefID refID() const;
	Type type() const;
	mstl::string name() const;

	void bind(Environment& env);
	void resolve(Environment& env);
	void expand(Environment& env);
	void execute(Environment& env);

private:

	unsigned char	m_value;
	Func				m_func;
	TokenP			m_macro;
};

} // namespace TeXt

#endif // _TeXt_ActiveToken_included

// vi:set ts=3 sw=3:
