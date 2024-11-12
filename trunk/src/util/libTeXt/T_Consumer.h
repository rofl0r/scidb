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

#ifndef _TeXt_Consumer_included
#define _TeXt_Consumer_included

#include "m_ref_counter.h"

namespace mstl { class string; }

namespace TeXt {

class Consumer : public mstl::ref_counter
{
public:

	virtual ~Consumer() = 0;

	virtual void put(unsigned char c) = 0;
	virtual void put(mstl::string const& s) = 0;

	virtual void out(mstl::string const& text) = 0;
	virtual void log(mstl::string const& text, bool copyToOut) = 0;
};

} // namespace TeXt

#endif //_TeXt_Consumer_included

// vi:set ts=3 sw=3:
