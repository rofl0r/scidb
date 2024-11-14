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

#ifndef _TeXt_Grouping_included
#define _TeXt_Grouping_included

#include "T_Package.h"

namespace TeXt {

class Grouping : public Package
{
	void doRegister(Environment& env) override;

	void performBegingroup(Environment& env);
	void performEndgroup(Environment& env);

	Token::Type m_begingroup;
};

} // namespace TeXt

#endif // _TeXt_Grouping_included

// vi:set ts=3 sw=3:
