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

namespace TeXt {

inline
bool
Package::isRegistered() const
{
	return m_isRegistered;
}


inline
bool
Package::isMandatory() const
{
	return m_category == Mandatory;
}


inline
bool
Package::isOptional() const
{
	return m_category == Optional;
}


inline
bool
Package::hasName() const
{
	return !m_name.empty();
}

} // namespace Private

// vi:set ts=3 sw=3:
