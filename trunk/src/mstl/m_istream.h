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

#ifndef _mstl_istream_included
#define _mstl_istream_included

#include "m_ios.h"

namespace mstl {

class string;

class istream : virtual public ios_base
{
public:

	struct traits { static const int eof = -1; };

	istream();
	~istream() throw();

	istream& get(char& c);
	istream& get(string& buf);
	istream& getline(char* buf, size_t n);
	istream& getline(string& buf);
	istream& read(char* buf, size_t n);
	istream& read(unsigned char* buf, size_t n);
	istream& ignore(unsigned long n, int delim = traits::eof);

	size_t readsome(char* buf, size_t n);
	size_t readsome(unsigned char* buf, size_t n);

	virtual int64_t size() const;

	bool eof();

	int get();
	int peek();

	istream& putback(char c);
	istream& unget();

	unsigned long tellg();
	istream& seekg(unsigned long offset);
	istream& seekg(long offset, seekdir dir);

private:

	istream(istream const&);
	istream& operator=(istream const&);

	char*		m_data;
	size_t	m_size;
};

} // namespace mstl

#include "m_istream.ipp"

#endif // _mstl_istream_included

// vi:set ts=3 sw=3:
