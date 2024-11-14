// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2008-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _jpeg_exception_defined
#define _jpeg_exception_defined

namespace JPEG {

class Exception
{
public:

	Exception(char const* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
	Exception(Exception const& exc);
	~Exception() throw();

	char const* what (void) const throw();

private:

	char* m_msg;
};

} // namespace JPEG

#endif // _jpeg_exception_defined

// vi:set ts=3 sw=3:
