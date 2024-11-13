// ======================================================================
// Author : $Author$
// Version: $Revision: 569 $
// Date   : $Date: 2012-12-16 21:41:55 +0000 (Sun, 16 Dec 2012) $
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
// Copyright: (C) 2009-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "db_producer.h"

using namespace db;


Producer::Producer(format::Type srcFormat, Consumer* consumer)
	:m_format(srcFormat)
	,m_variant(variant::Normal)
	,m_consumer(consumer)
{
	if (consumer)
		m_consumer->setProducer(this);
}


Producer::~Producer() {}


void
Producer::setConsumer(Consumer* consumer)
{
	if (m_consumer)
		m_consumer->setProducer(0);

	if ((m_consumer = consumer))
		m_consumer->setProducer(this);
}

// vi:set ts=3 sw=3:
