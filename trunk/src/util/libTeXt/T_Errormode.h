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

#ifndef _TeXt_Errormode_included
#define _TeXt_Errormode_included

#include "T_Package.h"

#include "m_string.h"

namespace TeXt {

class Errormode : public Package
{
public:

	static mstl::string setAbortmode(Environment& env);
	static mstl::string setBatchmode(Environment& env);
	static mstl::string setErrorstopmode(Environment& env);
	static mstl::string setScrollmode(Environment& env);
	static mstl::string setNonstopmode(Environment& env);

private:

	void doRegister(Environment& env) override;
};

} // namespace TeXt

#endif // _TeXt_Errormode_included

// vi:set ts=3 sw=3:
