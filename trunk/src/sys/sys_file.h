// ======================================================================
// Author : $Author$
// Version: $Revision: 130 $
// Date   : $Date: 2011-11-16 20:34:25 +0000 (Wed, 16 Nov 2011) $
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

#ifndef _sys_file_included
#define _sys_file_included

#include "sys_time.h"

#include "m_types.h"

namespace sys {
namespace file {

enum Mode
{
	Existence	= 0,
	Executable	= 1,
	Writeable	= 2,
	Readable		= 4,
};

bool access(char const* filename, Mode mode);
long size(char const* filename);
bool changed(char const* filename, uint32_t& time);

void rename(char const* oldFilename, char const* newFilename, bool preserveOldAttrs);
void deleteIt(char const* filename);

void* createMapping(char const* filename, Mode mode);
void closeMapping(void*& address);

bool lock(int fd);
void unlock(int fd);

} // namespace file
} // namespace sys

#endif // _sys_file_included

// vi:set ts=3 sw=3:
