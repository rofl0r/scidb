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

#ifndef _TeXt_ReadAgainProducer_included
#define _TeXt_ReadAgainProducer_included

#include "T_Producer.h"

namespace TeXt {

class ReadAgainProducer : public Producer
{
public:

	ReadAgainProducer(TokenP const& token);

	bool finished() const;

	mstl::string currentDescription() const;
	Source source() const;

	TokenP next(Environment& env);

private:

	TokenP	m_token;
	bool		m_finished;
	bool		m_unused;
};

} // namespace TeXt

#endif // _TeXt_ReadAgainProducer_included

// vi:set ts=3 sw=3:
