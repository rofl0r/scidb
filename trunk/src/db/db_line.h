// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
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

#ifndef _db_line_included
#define _db_line_included

#include "u_base.h"

namespace mstl { class string; }

namespace db {

struct Line
{
	Line();
	Line(uint16_t const* moves, unsigned length = 0);

	bool operator==(Line const& line) const;
	bool operator!=(Line const& line) const;
	bool operator<=(Line const& line) const;
	bool partialMatch(Line const& line) const;

	uint16_t operator[](unsigned n) const;

	mstl::string& print(mstl::string& result) const;
	mstl::string& dump(mstl::string& result) const;
	void dump() const;

	void copy(Line const& line);
	void copy(Line const& line, unsigned maxLength);
	Line& transpose(Line& dst) const;
	Line& transpose();

	uint16_t const*	moves;	// null terminated
	unsigned 			length;	// w/o null terminator
};

} // namespace db

namespace mstl {

template <typename T> struct is_pod;
template <> struct is_pod<db::Line> { enum { value = 1 }; };

} // namespace mstl

#include "db_line.ipp"

#endif // _db_line_included

// vi:set ts=3 sw=3:
