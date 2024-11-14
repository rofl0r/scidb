// ======================================================================
// Author : $Author$
// Version: $Revision: 671 $
// Date   : $Date: 2013-03-13 09:49:26 +0000 (Wed, 13 Mar 2013) $
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
// Copyright: (C) 2011-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_move_info_set_included
#define _db_move_info_set_included

#include "db_move_info.h"

#include "u_crc.h"

#include "m_vector.h"

namespace mstl { class string; }

namespace db {

class EngineList;

class MoveInfoSet
{
public:

	bool operator==(MoveInfoSet const& info) const;
	bool operator!=(MoveInfoSet const& info) const;

	bool isEmpty() const;
	bool contains(MoveInfo const& info) const;

	unsigned count() const;
	int find(MoveInfo const& info) const;

	::util::crc::checksum_t computeChecksum(EngineList const& engines, util::crc::checksum_t crc) const;

	MoveInfo const& operator[](unsigned n) const;
	MoveInfo& operator[](unsigned n);

	MoveInfo& add();
	MoveInfo& add(MoveInfo const& info);

	void resize(unsigned n);
	void reserve(unsigned n);
	void swap(MoveInfoSet& row);
	void sort();
	void clear();

	bool extractFromComment(EngineList& engineList, mstl::string& comment);
	void print(	EngineList const& engines,
					mstl::string& result,
					MoveInfo::Format format = MoveInfo::Pgn) const;

private:

	typedef mstl::vector<MoveInfo> Row;

	Row m_row;
};

} // namespace db

#include "db_move_info_set.ipp"

#endif // _db_move_info_row_included

// vi:set ts=3 sw=3:
