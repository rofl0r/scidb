// ======================================================================
// Author : $Author$
// Version: $Revision: 1493 $
// Date   : $Date: 2018-06-26 13:45:50 +0000 (Tue, 26 Jun 2018) $
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

#ifndef _mstl_ofstream_included
#define _mstl_ofstream_included

#include "m_ostream.h"
#define M_INCLUDE_FILE
#include "m_file.h"

namespace mstl {

class ofstream : public ostream, virtual protected bits::file
{
public:

	ofstream();
	explicit ofstream(char const* filename, openmode mode = out);
	explicit ofstream(int fd, openmode mode = out);
	explicit ofstream(struct _IO_FILE* fp, openmode mode = out);

	using file::is_open;
	using file::is_buffered;
	using file::is_unbuffered;

	using file::bufsize;
	using file::buffer;
	using file::mtime;
	using file::filename;

	virtual void open(char const* filename);
	virtual void open(char const* filename, openmode mode);
	void open(int fd, openmode mode = out);
	void open(struct _IO_FILE* fp, openmode mode = out);
	using file::close;

	using file::truncate;

	using file::set_unbuffered;
	using file::set_line_buffered;
	using file::set_binary;
	using file::set_text;
	using file::set_bufsize;
};

extern ofstream cout, cerr;

} // namespace mstl

#endif // _mstl_ofstream_included

// vi:set ts=3 sw=3:
