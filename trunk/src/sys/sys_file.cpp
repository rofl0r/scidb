// ======================================================================
// Author : $Author$
// Version: $Revision: 132 $
// Date   : $Date: 2011-11-20 14:59:26 +0000 (Sun, 20 Nov 2011) $
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

#include "sys_file.h"
#include "sys_base.h"

#include "m_assert.h"

#include <tcl.h>
#include <time.h>

# include <sys/stat.h>
# include <sys/types.h>

#ifdef WIN32
# define stat	_stat
# define chmod	_chmod
#endif

#if !TCL_PREREQ(8,5)
# error  "unsupported TCL version"
#endif


bool
sys::file::access(char const* filename, Mode mode)
{
	M_REQUIRE(filename);
	return Tcl_Access(filename, mode) == 0;
}


long
sys::file::size(char const* filename)
{
	M_REQUIRE(filename);

	Tcl_StatBuf*	buf		= Tcl_AllocStatBuf();
	Tcl_Obj*			pathObj	= Tcl_NewStringObj(filename, -1);
	long				size		= -1;

	Tcl_IncrRefCount(pathObj);
	int ret = Tcl_FSStat(pathObj, buf);
	Tcl_DecrRefCount(pathObj);

	if (ret != -1)
	{
#if !TCL_PREREQ(8,6)
		size = buf->st_size;
#else
		size = long(Tcl_GetSizeFromStat(buf));
#endif
	}

	::ckfree(reinterpret_cast<char*>(buf));
	return size;
}


bool
sys::file::changed(char const* filename, uint32_t& time)
{
	M_REQUIRE(filename);

	Tcl_StatBuf*	buf		= Tcl_AllocStatBuf();
	Tcl_Obj*			pathObj	= Tcl_NewStringObj(filename, -1);

	Tcl_IncrRefCount(pathObj);
	int ret = Tcl_FSStat(pathObj, buf);
	Tcl_DecrRefCount(pathObj);

	if (ret != -1)
	{
#if !TCL_PREREQ(8,6)
		time = buf->st_ctime;
#else
		time = Tcl_GetChangeTimeFromStat(buf);
#endif
	}

	::ckfree(reinterpret_cast<char*>(buf));
	return ret != -1;
}


#ifdef WIN32

# include <windows.h>


void*
sys::file::createMapping(char const* filename, Mode mode)
{
	M_REQUIRE(filename);

	HANDLE file = CreateFileA(
							filename,
							(mode & Writeable ? GENERIC_WRITE : 0) | (mode & Readable ? GENERIC_READ : 0),
							0,
							0,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
							0);

	if (file == 0)
		return 0;

	HANDLE mapping = CreateFileMappingA(
								file,
								0,
								PAGE_READONLY,
								0,
								0,
								filename);

	if (mapping == 0)
	{
		CloseHandle(file);
		return 0;
	}

	void* address = MapViewOfFile(
							handle->mapping,
							(mode & Writeable ? FILE_MAP_WRITE : 0) | (mode & Readable ? FILE_MAP_READ : 0),
							0,
							0,
							0);

	CloseHandle(mapping);
	CloseHandle(file);

	return address;
}


void
sys::file::closeMapping(void*& address)
{
	if (address)
		UnmapViewOfFile(address);

	address = 0;
}

#else

#include "m_map.h"
#include "m_pair.h"

# include <fcntl.h>
# include <unistd.h>
# include <errno.h>

# include <sys/mman.h>


static mstl::map<void*,mstl::pair<int,int> > FileMap;


void*
sys::file::createMapping(char const* filename, Mode mode)
{
	M_REQUIRE(filename);

	int flags = 0;

	if (mode & (Writeable | Readable))
		flags = O_RDWR;
	else if (!(mode & Writeable))
		flags = O_WRONLY;
	else
		flags = O_RDONLY;

	int fildes = ::open(filename, flags);

	if (fildes == -1)
		return 0;

	struct ::stat st;
	if (::fstat(fildes, &st) == -1)
	{
		::close(fildes);
		return 0;
	}

	int length = st.st_size;

	flags = 0;
	if (flags & Readable)	flags |= PROT_READ;
	if (flags & Writeable)	flags |= PROT_WRITE;

	void* address = ::mmap(0, length, flags, MAP_PRIVATE, fildes, 0);

	if (address == MAP_FAILED)
	{
		::close(fildes);
		return 0;
	}

	FileMap[address] = mstl::make_pair(fildes, length);
	return address;
}


void
sys::file::closeMapping(void*& address)
{
	if (address)
	{
		mstl::pair<int,int> p = FileMap[address];

		::close(p.first);
		::munmap(address, p.second);

		FileMap.erase(address);
	}

	address = 0;
}


bool
sys::file::lock(int fd)
{
	return ::lockf(fd, F_TLOCK, 0) != -1;
}


void
sys::file::unlock(int fd)
{
	::lockf(fd, F_ULOCK, 0);
}

#endif


void
sys::file::rename(char const* oldFilename, char const* newFilename, bool preserveOldAttrs)
{
	M_REQUIRE(oldFilename);
	M_REQUIRE(newFilename);

	struct stat st;

	if (preserveOldAttrs)
	{
		if (stat(oldFilename, &st) == -1)
			preserveOldAttrs = false;
	}

	Tcl_Obj* src(Tcl_NewStringObj(oldFilename, -1));
	Tcl_Obj* dst(Tcl_NewStringObj(newFilename, -1));

	Tcl_IncrRefCount(src);
	Tcl_IncrRefCount(dst);
	Tcl_FSRenameFile(src, dst);
	Tcl_DecrRefCount(dst);
	Tcl_DecrRefCount(src);

	if (preserveOldAttrs)
	{
#if defined(WIN32)
		st.st_mode &= _S_IREAD | _S_IWRITE;
#else
		st.st_mode &= 0x07777;
#endif

		chmod(newFilename, st.st_mode);

#if defined(__unix__) || defined(__MacOSX__)
		chown(newFilename, st.st_uid, st.st_gid);
#endif
	}
}


bool
sys::file::isHardLinked(char const* filename1, char const* filename2)
{
	M_REQUIRE(filename1);
	M_REQUIRE(filename2);

	struct stat st1, st2;

	if (stat(filename1, &st1) == -1)
		return false;
	if (stat(filename2, &st2) == -1)
		return false;

	return st1.st_ino == st2.st_ino;
}


void
sys::file::deleteIt(char const* filename)
{
	M_REQUIRE(filename);

	Tcl_Obj* fn(Tcl_NewStringObj(filename, -1));

	Tcl_IncrRefCount(fn);
	Tcl_FSDeleteFile(fn);
	Tcl_DecrRefCount(fn);
}

// vi:set ts=3 sw=3:
