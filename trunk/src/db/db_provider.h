// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
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
// Copyright: (C) 2010-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_provider_included
#define _db_provider_included

#include "db_common.h"

#include "u_base.h"

namespace db {

class Board;
class Line;

class Provider
{
public:

	Provider(format::Type srcFormat);
	virtual ~Provider() throw() = 0;

	format::Type sourceFormat() const;

	virtual Board const& getFinalBoard() const = 0;
	virtual Board const& getStartBoard() const = 0;

	virtual Line const& openingLine() const = 0;

	virtual unsigned countVariations() const = 0;
	virtual unsigned countComments() const = 0;
	virtual unsigned countAnnotations() const = 0;
	virtual unsigned countMoveInfo() const = 0;
	virtual unsigned countMarks() const = 0;
	virtual unsigned plyCount() const = 0;
	virtual variant::Type variant() const = 0;
	virtual uint16_t idn() const = 0;
	virtual uint32_t flags() const = 0;
	virtual bool commentEngFlag() const = 0;
	virtual bool commentOthFlag() const = 0;

	bool setupBoard() const;

	// data for receiver

	int index() const;
	void setIndex(int index);

private:

	format::Type	m_format;
	int				m_index;
};

} // namespace db

#include "db_provider.ipp"

#endif // _db_provider_included

// vi:set ts=3 sw=3:
