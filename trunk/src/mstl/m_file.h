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

#ifndef _mstl_file_included
#define _mstl_file_included

#ifndef M_INCLUDE_FILE
# error "m_file.h should not be included"
#endif
#undef M_INCLUDE_FILE

#include "m_ios.h"
#include "m_string.h"

namespace mstl {
namespace bits {

class file : virtual public ios_base
{
public:

	file();
	~file() throw();

	bool is_open() const;
	bool is_buffered() const;
	bool is_unbuffered() const;

	int64_t size() const;
	unsigned bufsize() const;
	char* buffer() const;
	uint64_t mtime();
	mstl::string const& filename() const;

	void open(char const* filename, char const* mode);
	void open(int fd, char const* mode);
	void open(struct _IO_FILE* fp);
	void reopen(char const* mode);
	void close();

	void truncate(unsigned length);

	void set_unbuffered();
	void set_line_buffered();
	void set_binary();
	void set_text();
	void set_bufsize(unsigned size);

private:

	void init();

	mstl::string m_filename;

	bool		m_open;
	bool		m_buffered;
	bool		m_line_buffered;
	bool		m_binary;
	unsigned	m_bufsize;
	char*		m_buffer;
};

} // namespace bits
} // namespace mstl

#include "m_file.ipp"

#endif // _mstl_file_included

// vi:set ts=3 sw=3:
