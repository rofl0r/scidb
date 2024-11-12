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

#ifndef _TeXt_Alignment_included
#define _TeXt_Alignment_included

#include "T_Package.h"

#include "m_ref_counted_ptr.h"

namespace TeXt {

class Alignment : public Package
{
public:

	Alignment();
	~Alignment();

private:

	class AlignmentFilter;

	typedef mstl::ref_counted_ptr<AlignmentFilter> FilterP;

	void doRegister(Environment& env);
	void doFinish(Environment& env);

	void performCrcr(Environment& env);
	void performSpace(Environment& env);
	void performTab(Environment& env);
	void performEps(Environment& env);
	void performEverycr(Environment& env);

	FilterP m_filter;
};

} // namespace TeXt

#endif // _TeXt_Alignment_included

// vi:set ts=3 sw=3:
