// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _TeXt_TextProducer_included
#define _TeXt_TextProducer_included

#include "T_Producer.h"

#include "m_string.h"

namespace TeXt {

class Environment;

class TextProducer : public Producer
{
public:

	TextProducer(mstl::string const& text);

	bool finished() const;

	mstl::string currentDescription() const;
	Source source() const;

	TokenP next(Environment& env);
	bool reset();

private:

	mstl::string m_text;
	mstl::string::const_iterator m_pos;
};

} // namespace TeXt

#endif // _TeXt_TextProducer_included

// vi:set ts=3 sw=3:
