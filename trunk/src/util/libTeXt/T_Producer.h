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

#ifndef _TeXt_Producer_included
#define _TeXt_Producer_included

#include "T_TokenP.h"
#include "T_Object.h"

#include "m_string.h"

namespace TeXt {

class Environment;

class Producer : public Object
{
public:

	enum Source
	{
		File,
		Macro,
		Parameter,
		List,
		Text,
		Insert,
		InsertedText,
		ReadAgain,
	};

	virtual ~Producer() = 0;

	virtual bool finished() const;

	virtual unsigned lineno() const;
	virtual mstl::string currentDescription() const = 0;
	virtual Source source() const = 0;

	virtual TokenP next(Environment& env) = 0;
	virtual bool reset();
};

} // namespace TeXt

#endif // _TeXt_Producer_included

// vi:set ts=3 sw=3:
