// ======================================================================
// Author : $Author: gcramer $
// Version: $Revision: 1411 $
// Date   : $Date: 2017-08-12 11:08:17 +0000 (Sat, 12 Aug 2017) $
// Url    : $HeadURL: https://svn.code.sf.net/p/scidb/code/trunk/src/eco/eco_line_reader.h $
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
// ======================================================================

// ======================================================================
// Copyright: (C) 2014-2017 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _eco_line_reader_included
#define _eco_line_reader_included

#include "eco_id.h"
#include "eco_reader.h"

#include "db_board.h"

namespace mstl { class istream; }

namespace eco {

class LineReader : public Reader
{
public:

	static constexpr unsigned Ignore_ECO = 1 << 0;

	LineReader(mstl::istream& strm, unsigned flags = 0);

	auto readLine(
		db::MoveLine& line,
		Transitions& transitions,
		Name& name,
		mstl::string& prologue,
		mstl::string& epilogue,
		char& sign,
		Node* root) -> Id override;

private:

	mstl::istream&	m_strm;
	unsigned			m_flags;
	Id					m_lastId;
	unsigned			m_lineLength;
	mstl::string	m_comment;
	db::Board		m_board;
};

} // namespace eco

#endif // _eco_line_reader_included

// vi:set ts=3 sw=3: