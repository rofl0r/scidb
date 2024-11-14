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

#include "T_TextToken.h"
#include "T_NumberToken.h"

namespace TeXt {

template <class Iterator>
inline
ListToken::ListToken(Iterator const& first, Iterator const& last)
	:m_tokenList(first, last)
{
}


inline
Value
ListToken::length() const
{
	return m_tokenList.size();
}


inline
unsigned
ListToken::size() const
{
	return m_tokenList.size();
}


inline
void
ListToken::append(mstl::string const& text)
{
	append(new TextToken(text));
}


inline
void
ListToken::append(Value value)
{
	append(new NumberToken(value));
}

} // namespace TeXt

// vi:set ts=3 sw=3:
