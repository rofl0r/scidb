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

#ifndef _cbh_decoder_included
#define _cbh_decoder_included

#include "cbh_decoder_position.h"

#include "db_move.h"

namespace util { class ByteStream; }
namespace sys { namespace utf8 { class Codec; } }

namespace db {

class GameData;
class Consumer;
class TagSet;
class MoveNode;
class GameInfo;

namespace cbh {

class Decoder
{
public:

	Decoder(util::ByteStream& gStrm, util::ByteStream& aStrm, sys::utf8::Codec& codec, bool isChess960);

	Move findExactPosition(Board const& position, bool skipVariations);

	unsigned doDecoding(unsigned flags, GameData& data);
	save::State doDecoding(db::Consumer& consumer, unsigned flags, TagSet& tags, GameInfo const& info);

private:

	Decoder(Decoder const&);
	Decoder& operator=(Decoder const&);

	void startDecoding(TagSet* tags = 0);
	unsigned decodeMove(Move& move, unsigned& count);
	void decodeMoves(Consumer& consumer);
	void decodeMoves(MoveNode* root, unsigned flags);
	void decodeMoves(MoveNode* root, unsigned flags, unsigned& count);
	void traverse(Consumer& consumer, MoveNode const* root);
	void getAnnotation(MoveNode* node, int position, unsigned flags);
	void decodeComment(MoveNode* node, unsigned length, unsigned flags);
	void decodeSymbols(MoveNode* node, unsigned length);
	void decodeSquares(MoveNode* node, unsigned length);
	void decodeArrows(MoveNode* node, unsigned length);

	util::ByteStream&	m_gStrm;
	util::ByteStream&	m_aStrm;
	sys::utf8::Codec&	m_codec;
	int					m_moveNo;
	bool					m_isChess960;
	decoder::Position	m_position;
	Byte const*			m_lookup;
};

} // namespace cbh
} // namespace db

#endif // _cbh_decoder_included

// vi:set ts=3 sw=3:
