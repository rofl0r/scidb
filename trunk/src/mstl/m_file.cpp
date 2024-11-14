// ======================================================================
// Author : $Author$
// Version: $Revision: 648 $
// Date   : $Date: 2013-02-05 21:52:03 +0000 (Tue, 05 Feb 2013) $
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

#include "m_file.h"
#include "m_stdio.h"
#include "m_assert.h"

#include <sys/stat.h>

#ifdef __WIN32__
# include <fcntl.h>
# include <io.h>
# define ftruncate _chsize
#else
# include <unistd.h>
#endif

using namespace mstl;
using namespace mstl::bits;


file::file()
	:m_open(false)
	,m_buffered(true)
	,m_line_buffered(false)
	,m_binary(false)
	,m_bufsize(0)
	,m_buffer(0)
{
}


file::~file() throw()
{
	if (m_open && fileno(m_fp) > 2)
	{
		::fclose(m_fp);
		m_fp = 0;
	}

	delete [] m_buffer;
}


int64_t
file::size() const
{
	M_REQUIRE(is_open());

	struct stat st;
	st.st_size = 0;

	if (::fstat(fileno(m_fp), &st) == -1)
		return -1;

	return st.st_size;
}


uint64_t
file::mtime()
{
	M_REQUIRE(is_open());

	struct stat st;
	st.st_size = 0;

	if (::fstat(fileno(m_fp), &st) == -1)
		setstate(m_open ? badbit | failbit : failbit);

	return st.st_mtime;
}


void
file::init()
{
	if (m_fp == 0)
		setstate(failbit);
	else
		m_open = true;

	if (m_line_buffered)
	{
		m_line_buffered = false;
		set_line_buffered();
	}
	else if (!m_buffered)
	{
		m_buffered = true;
		set_unbuffered();
	}

	if (m_binary)
		set_binary();
	if (m_bufsize)
		set_bufsize(m_bufsize);
}


void
file::open(char const* filename, char const* mode)
{
	M_ASSERT(filename);
	M_ASSERT(mode);
	M_ASSERT(!is_open());

	m_filename = filename;
	m_fp = ::fopen(filename, mode);

	init();
}


void
file::open(int fd, char const* mode)
{
	M_ASSERT(fd >= 0);
	M_ASSERT(mode);
	M_ASSERT(!is_open());

	m_filename.clear();
	m_fp = ::fdopen(fd, mode);

	init();
}


void
file::open(FILE* fp)
{
	M_ASSERT(fp);
	M_ASSERT(!is_open());

	m_filename.clear();
	m_fp = fp;
	m_open = true;

	init();
}


void
file::reopen(char const* mode)
{
	M_REQUIRE(!m_filename.empty());
	M_REQUIRE(is_open());

	if (fileno(m_fp) > 2)
		::fclose(m_fp);

	m_open = false;
	m_fp = ::fopen(m_filename, mode);
	init();
}


void
file::close()
{
	if (!m_open)
		return;

	int rc = 0;

	if (fileno(m_fp) > 2)
	{
		rc = ::fclose(m_fp);
		m_fp = 0;
	}

	m_open = false;
	m_filename.clear();
	delete [] m_buffer;
	m_buffer = 0;
	m_bufsize = 0;

	if (rc)
		setstate(failbit);
}


void
file::truncate(unsigned length)
{
	M_REQUIRE(is_open() && is_writable());

	if (ftruncate(fildes(), length) == -1)
		setstate(failbit);
}


void
file::set_bufsize(unsigned size)
{
	if (size == 0)
	{
		set_unbuffered();
	}
	else if (size != m_bufsize)
	{
		if (size > m_bufsize)
		{
			delete [] m_buffer;
			m_buffer = new char[size];
		}

		if (m_open)
		{
			if (::setvbuf(m_fp, m_buffer, _IOFBF, size))
				M_RAISE("setvbuf() can't be honoured (fd=%d)", fileno(m_fp));
		}

		m_bufsize = size;
		m_buffered = true;
		m_line_buffered = false;
	}
}


void
file::set_unbuffered()
{
	if (!m_buffered && !m_line_buffered)
		return;

	if (m_open)
	{
		if (::setvbuf(m_fp, 0, _IONBF, 0))
			M_RAISE("setvbuf() can't be honoured (fd=%d)", fileno(m_fp));
	}

	m_buffered = false;
	m_line_buffered = false;
	delete [] m_buffer;
	m_buffer = 0;
	m_bufsize = 0;
}


void
file::set_line_buffered()
{
	if (m_line_buffered)
		return;

	if (m_open)
	{
		if (::setvbuf(m_fp, 0, _IOLBF, 0))
			M_RAISE("setvbuf() can't be honoured (fd=%d)", fileno(m_fp));
	}

	if (m_buffered)
	{
		delete [] m_buffer;
		m_buffer = 0;
	}

	m_buffered = false;
	m_line_buffered = true;
	m_bufsize = 0;
}


void
file::set_binary()
{
	if (m_open && !(mode() & binary))
	{
#if defined(__WIN32__)

		if (::setmode(fileno(m_fp), O_BINARY) == -1)
			M_RAISE("setmode() failed (fd=%d)", fileno(m_fp));

#endif

		setmode(mode() | binary);
	}

	m_binary = true;
}


void
file::set_text()
{
	if (m_open && (mode() & binary))
	{
#if defined(__WIN32__)

		if (::setmode(fileno(m_fp), O_TEXT) == -1)
			M_RAISE("setmode() failed (fd=%d)", fileno(m_fp));

#endif

		setmode(mode() &~ binary);
	}

	m_binary = false;
}

// vi:set ts=3 sw=3:
