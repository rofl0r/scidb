// ======================================================================
// Author : $Author$
// Version: $Revision: 617 $
// Date   : $Date: 2013-01-08 11:41:26 +0000 (Tue, 08 Jan 2013) $
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

#ifndef _si3_encoder_included
#define _si3_encoder_included

#include "si3_encoder_position.h"

#include "db_consumer.h"
#include "db_common.h"

#include "m_bitfield.h"

namespace util { class ByteStream; }
namespace mstl { class string; }
namespace sys { namespace utf8 { class Codec; } }

namespace db {

class Move;
class Eco;
class MoveNode;
class TagSet;
class GameData;
class Board;
class Signature;

namespace si3 {

class Encoder
{
public:

	Encoder(util::ByteStream& strm, sys::utf8::Codec& codec);

	void doEncoding(	Signature const& signature,
							GameData const& data,
							db::Consumer::TagBits const& allowedTags,
							bool allowExtraTags);

	static unsigned encodeType(db::type::ID type);
	static db::tag::TagSet const& infoTags();
	static bool isExtraTag(tag::ID tag);

	static void initialize();

protected:

	typedef encoder::Position Position;

	void doEncoding(Move const& move);

	void encodeVariation(MoveNode const* node, unsigned level = 0);
	void encodeComments(MoveNode* node, encoding::CharSet encoding);
	void encodeTag(TagSet const& tags, tag::ID tagID);
	void encodeTags(TagSet const& tags, db::Consumer::TagBits allowedTags, bool allowExtraTags);
	void encodeNullMove(Move const& move);
	void encodeKing(Move const& move);
	void encodeQueen(Move const& move);
	void encodeRook(Move const& move);
	void encodeBishop(Move const& move);
	void encodeKnight(Move const& move);
	void encodePawn(Move const& move);
	void encodeMove(Move const& move);

	void putComment(mstl::string& buf);
	void setup(Board const& board);

	util::ByteStream&	m_strm;
	sys::utf8::Codec&	m_codec;
	Position				m_position;
};


} // namespace si3
} // namespace db

#include "si3_encoder.ipp"

#endif // _si3_encoder_included

// vi:set ts=3 sw=3:
