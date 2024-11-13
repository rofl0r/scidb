// ======================================================================
// Author : $Author$
// Version: $Revision: 340 $
// Date   : $Date: 2012-06-14 19:06:13 +0000 (Thu, 14 Jun 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C)2011-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _TeXt_Unicode_included
#define _TeXt_Unicode_included

#include "T_Package.h"

#include "m_ref_counted_ptr.h"

namespace TeXt {

class Unicode : public Package
{
public:

	Unicode();
	~Unicode();

	class UnicodeFilter;

private:

	typedef mstl::ref_counted_ptr<UnicodeFilter> FilterP;

	void doRegister(Environment& env) override;

	void performUcPref(Environment& env);
	void performUcSuff(Environment& env);
	void performUcMap(Environment& env);

	FilterP m_filter;
};

} // namespace TeXt

#endif // _TeXt_Unicode_included

// vi:set ts=3 sw=3:
