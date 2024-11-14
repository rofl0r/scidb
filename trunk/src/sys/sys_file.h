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

#ifndef _sys_file_included
#define _sys_file_included

#include "sys_time.h"

#include "m_string.h"

namespace sys {
namespace file {

enum Mode
{
	Existence	= 0,
	Executable	= 1,
	Writeable	= 2,
	Readable		= 4,
};

enum Type
{
	None,
	RegularFile,
	Directory,
	CharacterDevice,
	BlockDevice,
	NamedPipe,
	SymbolicLink,
	Socket,
	Unknown,
};

mstl::string internalName(char const* externalName);

bool access(char const* filename, Mode mode);

long size(char const* filename);
bool changed(char const* filename, uint32_t& time);
bool isHardLinked(char const* filename1, char const* filename2);
Type type(char const* filename);

void rename(char const* oldFilename, char const* newFilename, bool preserveOldAttrs = false);
void deleteIt(char const* filename);
bool setModificationTime(char const* filename, uint32_t time);

void* createMapping(char const* filename, Mode mode);
void closeMapping(void*& address);

bool lock(int fd);
void unlock(int fd);

} // namespace file
} // namespace sys

#endif // _sys_file_included

// vi:set ts=3 sw=3:
