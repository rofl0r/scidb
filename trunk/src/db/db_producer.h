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
// Copyright: (C) 2009-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_producer_included
#define _db_producer_included

#include "db_common.h"

namespace util { class Progress; }
namespace mstl { class string; }

namespace db {

class Consumer;
class Board;

class Producer
{
public:

	Producer(format::Type srcFormat, Consumer* consumer = 0);
	virtual ~Producer() = 0;

	bool hasConsumer() const;
	virtual bool encodingFailed() const = 0;
	bool whiteToMove() const;
	bool blackToMove() const;

	format::Type format() const;
	variant::Type variant() const;
	Board const& board() const;
	virtual mstl::string const& encoding() const = 0;
	virtual uint16_t idn() const = 0;

	Consumer& consumer();
	Consumer const& consumer() const;

	void setConsumer(Consumer* consumer);
	void setVariant(variant::Type variant);

	virtual unsigned process(util::Progress& progress) = 0;

protected:

	Board& board();

private:

	format::Type	m_format;
	variant::Type	m_variant;
	Consumer*		m_consumer;
};

} // namespace db

#include "db_producer.ipp"

#endif // _db_producer_included

// vi:set ts=3 sw=3:
