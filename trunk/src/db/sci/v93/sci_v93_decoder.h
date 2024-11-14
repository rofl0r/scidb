// ======================================================================
// Author : $Author$
// Version: $Revision: 773 $
// Date   : $Date: 2013-05-12 16:51:25 +0000 (Sun, 12 May 2013) $
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

#ifndef _sci_v93_decoder_included
#define _sci_v93_decoder_included

#include "sci_decoder_position.h"

#include "db_common.h"
#include "db_move.h"
#include "db_eco.h"

namespace mstl { template <typename T, typename U> class map; }

namespace sys { namespace utf8 { class Codec; } }

namespace util { class ByteStream; }

namespace db {

class Board;
class GameData;
class MoveNode;
class Consumer;
class TagSet;
class EngineList;
class TimeTable;

namespace sci {
namespace v93 {

class Decoder
{
public:

	typedef mstl::map<mstl::string,unsigned> TagMap;

	Decoder(util::ByteStream& strm, variant::Type variant);
	Decoder(util::ByteStream& strm, unsigned guaranteedStreamSize, variant::Type variant);

	Move findExactPosition(Board const& position, bool skipVariations);
	void findTags(TagMap& tags);

	void doDecoding(GameData& data);
	save::State doDecoding(db::Consumer& consumer, TagSet& tags);

	bool stripMoveInformation(unsigned halfMoveCount, unsigned types);
	bool stripTags(TagMap const& tags);

private:

	void decodeRun(unsigned count);
	void decodeRun(unsigned count, Consumer& consumer);
	void decodeVariation(util::ByteStream& data);
	void decodeVariation(Consumer& consumer, util::ByteStream& data, util::ByteStream& text);
	void decodeTextSection(MoveNode* node, util::ByteStream& text);
	void decodeMark();

	Byte const* skipTags(Byte const* p);
	Byte const* skipEngines(Byte const* p);
	Byte const* skipMoveInfo(Byte const* p, Byte const* eos);

	void skipVariations();

	unsigned decodeMove(Byte value, Move& move);
	unsigned decodeZhouseMove(Move& move);
	Move nextMove(unsigned runLength = 0);

	Move decodeKing(sq::ID from, Byte nybble);
	Move decodeQueen(sq::ID from, Byte nybble);
	Move decodeRook(sq::ID from, Byte nybble);
	Move decodeBishop(sq::ID from, Byte nybble);
	Move decodeKnight(sq::ID from, Byte nybble);
	Move decodePawn(sq::ID from, Byte nybble);

	Move searchForPosition(Board const& position, bool skipVariations);

	static void decodeEngines(util::ByteStream& strm, EngineList& engines);
	static void decodeTags(util::ByteStream& strm, TagSet& tags);
	static void decodeTimeTable(util::ByteStream& strm, TimeTable& timeTable);

	Decoder(Decoder const&);
	Decoder& operator=(Decoder const&);

	util::ByteStream&	m_strm;
	unsigned				m_guaranteedStreamSize;
	decoder::Position	m_position;
	MoveNode*			m_currentNode;
	variant::Type		m_variant;
};

} // namespace v93
} // namespace sci
} // namespace db

#endif // _sci_v93_decoder_included

// vi:set ts=3 sw=3:
