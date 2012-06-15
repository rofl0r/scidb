// ======================================================================
// Author : $Author$
// Version: $Revision: 343 $
// Date   : $Date: 2012-06-15 12:05:39 +0000 (Fri, 15 Jun 2012) $
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

#ifndef _TeXt_GenericToken_included
#define _TeXt_GenericToken_included

#include "T_Token.h"

#include "m_function.h"
#include "m_string.h"

namespace TeXt {

class Environment;


class GenericToken : public virtual Token
{
public:

	typedef mstl::function<void (Environment&)> Func;
	typedef Token::Type Type;

	virtual ~GenericToken() = 0;

	Type type() const override;
	mstl::string name() const override;
	void perform(Environment& env) const;

	void setType(Type type);

protected:

	GenericToken(Type type, mstl::string const& name, Func func);

private:

	Type				m_type;
	mstl::string	m_name;
	Func				m_func;
};

} // namespace TeXt

#endif // _TeXt_GenericToken_included

// vi:set ts=3 sw=3:
