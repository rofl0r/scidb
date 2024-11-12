// ======================================================================
// Author : $Author$
// Version: $Revision: 33 $
// Date   : $Date: 2011-05-29 12:27:45 +0000 (Sun, 29 May 2011) $
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
// Copyright: (C) 2010-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_provider_included
#define _db_provider_included

#include "u_base.h"

namespace db {

class Board;
class Line;

class Provider
{
public:

	Provider();
	virtual ~Provider() throw() = 0;

	virtual Board const& getFinalBoard() const = 0;
	virtual Board const& getStartBoard() const = 0;

	virtual Line const& openingLine() const = 0;

	virtual unsigned countVariations() const = 0;
	virtual unsigned countComments() const = 0;
	virtual unsigned countAnnotations() const = 0;
	virtual unsigned countMarks() const = 0;
	virtual unsigned plyCount() const = 0;
	virtual uint32_t flags() const = 0;
	virtual bool commentEngFlag() const = 0;
	virtual bool commentOthFlag() const = 0;

	// data for receiver

	int index() const;
	void setIndex(int index);

private:

	int m_index;
};

} // namespace db

#include "db_provider.ipp"

#endif // _db_provider_included

// vi:set ts=3 sw=3:
