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

#ifndef _sci_consumer_included
#define _sci_consumer_included

#include "sci_encoder.h"
#include "sci_common.h"

#include "db_consumer.h"
#include "db_move.h"

#include "u_byte_stream.h"

namespace mstl { class string; }

namespace db {

class TagSet;
class MarkSet;
class Annotation;

namespace sci {

class Codec;

class Consumer : private Encoder, public db::Consumer
{
public:

	Consumer(format::Type srcFormat, Codec& codec);

private:

	format::Type format() const;

	void start();
	void finish();

	bool beginGame(TagSet const& tags);
	save::State endGame(TagSet const& tags);

	void sendComment(Comment const& comment, Annotation const& annotation, MarkSet const& marks);
	bool sendMove(Move const& move);
	bool sendMove(	Move const& move,
						Annotation const& annotation,
						MarkSet const& marks,
						Comment const& comment);

	void beginMoveSection();
	void endMoveSection(result::ID result);
	void beginVariation();
	void endVariation();

	util::ByteStream	m_stream;
	Byte					m_buffer[Block_Size];
	Codec&				m_codec;
	unsigned				m_streamPos;
	Move					m_move;
	unsigned				m_runLength;
	bool					m_endOfRun;
	bool					m_danglingPop;
	bool					m_putComment;
};

} // namespace sci
} // namespace db

#endif // _sci_consumer_included

// vi:set ts=3 sw=3:
