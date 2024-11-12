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

#ifndef _TeXt_FileInput_included
#define _TeXt_FileInput_included

#include "T_Input.h"

namespace mstl { class istream; }

namespace TeXt {


class FileInput : public Input
{
public:

	FileInput(mstl::istream* stream, bool owner = false);
	FileInput(FileInput const& stream);
	~FileInput();

	FileInput& operator=(FileInput const& stream);

	Source source() const;

	bool readNextLine(mstl::string& result);

private:

	mstl::istream*	m_stream;
	mutable bool	m_owner;
};

} // namespace TeXt

#endif // _TeXt_FileInput_included

// vi:set ts=3 sw=3:
