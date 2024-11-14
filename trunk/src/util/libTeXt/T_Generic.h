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

#ifndef _TeXt_Generic_included
#define _TeXt_Generic_included

#include "T_Token.h"

#include "m_function.h"
#include "m_string.h"

namespace TeXt {

class Environment;

class Generic
{
public:

	typedef mstl::function<void (Environment&)> Func;
	typedef Token::Type Type;

	virtual ~Generic() = 0;

	Type type() const;
	mstl::string name() const;
	void perform(Environment& env);

	void setType(Type type);

protected:

	Generic(Type type, const mstl::string& name, Func func);

private:

	Type				m_type;
	mstl::string	m_name;
	Func				m_func;
};

} // namespace TeXt

#endif // _TeXt_Generic_included

// vi:set ts=3 sw=3:
