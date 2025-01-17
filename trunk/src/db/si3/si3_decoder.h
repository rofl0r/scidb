// ======================================================================
// Author : $Author$
// Version: $Revision: 1350 $
// Date   : $Date: 2017-08-02 10:42:46 +0000 (Wed, 02 Aug 2017) $
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

#ifndef _si3_decoder_included
#define _si3_decoder_included

#include "si3_decoder_position.h"

#include "db_common.h"
#include "db_move.h"
#include "db_engine_list.h"
#include "db_eco.h"

#ifdef DEBUG_SI4
# include "db_home_pawns.h"
#endif

#include "nsUniversalDetector.h"

namespace util { class ByteStream; }
namespace sys { namespace utf8 { class Codec; } }

namespace db {

class GameData;
class Consumer;
class TagSet;
class MoveNode;

namespace si3 {

class Decoder : public nsUniversalDetector
{
public:

	Decoder(util::ByteStream& strm, sys::utf8::Codec& codec);
	~Decoder();

	mstl::string const& encoding() const;

	Move findExactPosition(Board const& position, bool skipVariations);

	unsigned doDecoding(GameData& data);
	save::State doDecoding(db::Consumer& consumer, TagSet& tags);
	unsigned doDecoding(uint16_t* line, unsigned length, Board& startBoard, bool useStartBoard);

	static type::ID decodeType(unsigned type);

private:

	void determineCharsetComments(MoveNode* node);
	void determineCharsetTags();

	void decodeVariation(unsigned level = 0);
	Byte decodeMove(Byte value);
	void decodeComments(MoveNode* node, Consumer* consumer = 0);
	void decodeTags(TagSet& tags);
	void skipTags();

	void decodeKing(sq::ID from, Byte nybble);
	void decodeQueen(sq::ID from, Byte nybble);
	void decodeRook(sq::ID from, Byte nybble);
	void decodeBishop(sq::ID from, Byte nybble);
	void decodeKnight(sq::ID from, Byte nybble);
	void decodePawn(sq::ID from, Byte nybble);

	void decodeVariation(Consumer& consumer, MoveNode const* node);
	void handleInvalidMove(Byte value);

	void Report(char const* charset);

	Move nextMove();
	void skipVariation();
	void checkVariant(TagSet& tags);

	Decoder(Decoder const&);
	Decoder& operator=(Decoder const&);

	util::ByteStream&	m_strm;
	sys::utf8::Codec*	m_givenCodec;
	sys::utf8::Codec*	m_codec;
	decoder::Position	m_position;
	MoveNode*			m_currentNode;
	Move					m_move;
	bool					m_hasVariantTag;
	EngineList			m_engines;

#ifdef DEBUG_SI4
	HomePawns m_homePawns;
#endif
};

} // namespace si3
} // namespace db

#endif // _si3_decoder_included

// vi:set ts=3 sw=3:
