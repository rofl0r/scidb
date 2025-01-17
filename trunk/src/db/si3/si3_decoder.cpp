// ======================================================================
// Author : $Author$
// Version: $Revision: 1522 $
// Date   : $Date: 2018-09-16 13:56:42 +0000 (Sun, 16 Sep 2018) $
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

#include "si3_decoder.h"
#include "si3_common.h"

#include "db_game_data.h"
#include "db_consumer.h"
#include "db_tag_set.h"
#include "db_mark_set.h"
#include "db_annotation.h"
#include "db_comment.h"
#include "db_move_node.h"
#include "db_database_codec.h"
#include "db_reader.h"
#include "db_exception.h"
#include "db_date.h"

#include "u_nul_string.h"
#include "u_byte_stream.h"

#include "m_string.h"

#include "sys_utf8.h"
#include "sys_utf8_codec.h"

using namespace db;
using namespace db::si3;
using namespace util;


enum { Max_Tag_Length = 240, };


static tag::ID CommonTags[255 - Max_Tag_Length] =
{
	// 241, 242
	tag::WhiteCountry, tag::BlackCountry,
	// 243
	tag::Annotator,
	// 244
	tag::PlyCount,
	// 245 (plain text encoding)
	tag::EventDate,
	// 246, 247
	tag::Opening, tag::Variation,
	// 248-250
	tag::SetUp, tag::Source, tag::SetUp,	// NOTE: Scid is using "Setup" for entry #248
	// 252-254: spare for future use
	tag::ExtraTag, tag::ExtraTag, tag::ExtraTag, tag::ExtraTag,
	// 255: Reserved for compact EventDate encoding (obsolete since 3.x)
	tag::ExtraTag
};


__attribute__((noreturn))
inline static void
throwCorruptData()
{
	IO_RAISE(Game, Corrupted, "error while decoding game data");
}


Decoder::Decoder(ByteStream& strm, sys::utf8::Codec& codec)
	:m_strm(strm)
	,m_givenCodec(&codec)
	,m_codec(&codec)
	,m_currentNode(nullptr)
	,m_hasVariantTag(false)
{
}


Decoder::~Decoder()
{
	if (m_codec != m_givenCodec)
		delete m_codec;
}


inline
void
Decoder::decodeKing(sq::ID from, Byte nybble)
{
	static int const Offset[16] = { 0, -9, -8, -7, -1, 1, 7, 8, 9, -2, 2 };

	if (__builtin_expect(nybble == 0, 0))
	{
		m_move = Move::null();
	}
	else
	{
		M_ASSERT(nybble < U_NUMBER_OF(Offset));

		int offset	= Offset[nybble];
		int to		= from + offset;

		if (abs(offset) == 2)
			m_move = m_position.makeCastlingMove(from, to & 63);
		else
			m_move = m_position.makeKingMove(from, to & 63);
	}
}


inline
void
Decoder::decodeQueen(sq::ID from, Byte nybble)
{
	if (nybble >= 8)
	{
		// rook-vertical move
		m_move = m_position.makeQueenMove(from, sq::make(sq::fyle(from), sq::Rank(nybble & 7)));
	}
	else if (nybble != sq::fyle(from))
	{
		// rook-horizontal move
		m_move = m_position.makeQueenMove(from, sq::make(sq::Fyle(nybble), sq::rank(from)));
	}
	else
	{
		// diagonal move: coded in two bytes
		m_move = m_position.makeQueenMove(from, (int(m_strm.get()) - 64) & 63);
	}
}


inline
void
Decoder::decodeRook(sq::ID from, Byte nybble)
{
	if (nybble >= 8)	// this is a move along a fyle, to a different rank
		m_move = m_position.makeRookMove(from, sq::make(sq::fyle(from), nybble & 7));
	else
		m_move = m_position.makeRookMove(from, sq::make(nybble, sq::rank(from)));
}


inline
void
Decoder::decodeBishop(sq::ID from, Byte nybble)
{
	int diff = int(nybble & 7) - int(sq::fyle(from));
	m_move = m_position.makeBishopMove(from, (nybble < 8 ? int(from) + 9*diff : int(from) - 7*diff) & 63);
}


inline
void
Decoder::decodeKnight(sq::ID from, Byte nybble)
{
	static int const Offset[16] = { 0, -17, -15, -10, -6, 6, 10, 15, 17, 0, 0, 0, 0, 0, 0, 0 };
	m_move = m_position.makeKnightMove(from, (from + Offset[nybble]) & 63);
}


void
Decoder::decodePawn(sq::ID from, Byte nybble)
{
	static int const Offset[16] = { 7,8,9, 7,8,9, 7,8,9, 7,8,9, 7,8,9, 16 };

	static piece::Type const PromotedPiece[16] =
	{
		piece::None,	piece::None,	piece::None,
		piece::Queen,	piece::Queen,	piece::Queen,
		piece::Rook,	piece::Rook,	piece::Rook,
		piece::Bishop,	piece::Bishop,	piece::Bishop,
		piece::Knight,	piece::Knight,	piece::Knight,
		piece::None,
	};

	int offset = Offset[nybble];

	if (m_position.whiteToMove())
	{
		Square to = (from + offset) & 63;

		switch (offset)
		{
			case 8:
				if (to >= sq::a8)
					m_move = Move::genPromote(from, to, PromotedPiece[nybble]);
				else
					m_move = Move::genOneForward(from, to);
				return;

			case 16:
				m_move = Move::genTwoForward(from, to);
				return;
		}

		if (to >= sq::a8)
			m_move = Move::genCapturePromote(from, to, PromotedPiece[nybble], m_position.piece(to));
		else if (to == m_position.board().enPassantSquare())
			m_move = Move::genEnPassant(from, to);
		else
			m_move = Move::genPawnCapture(from, to, m_position.piece(to));
	}
	else
	{
		// black to move

		Square to = (from - offset) & 63;

		switch (offset)
		{
			case 8:
				if (__builtin_expect(to <= sq::h1, 0))
					m_move = Move::genPromote(from, to, PromotedPiece[nybble]);
				else
					m_move = Move::genOneForward(from, to);
				return;

			case 16:
				m_move = Move::genTwoForward(from, to);
				return;
		}

		if (to <= sq::h1)
			m_move = Move::genCapturePromote(from, to, PromotedPiece[nybble], m_position.piece(to));
		else if (to == m_position.board().enPassantSquare())
			m_move = Move::genEnPassant(from, to);
		else
			m_move = Move::genPawnCapture(from, to, m_position.piece(to));
	}
}


Byte
Decoder::decodeMove(Byte value)
{
	unsigned	pieceNum = (value >> 4);

	if (m_position.blackToMove())
		pieceNum |= 0x10;

	sq::ID from = sq::ID(m_position[pieceNum]);

	switch (m_position.piece(from))
	{
		case piece::King:		decodeKing(from, value & 15); break;
		case piece::Queen:	decodeQueen(from, value & 15); break;
		case piece::Rook:		decodeRook(from, value & 15); break;
		case piece::Bishop:	decodeBishop(from, value & 15); break;
		case piece::Knight:	decodeKnight(from, value & 15); break;
		case piece::Pawn:		decodePawn(from, value & 15); break;
		case piece::None:		::throwCorruptData();
	}

	return pieceNum;
}


void
Decoder::handleInvalidMove(Byte value)
{
	// Scid (at least older versions of Scid) has a bug when parsing PGN files:
	// a move like "b1" will be encoded in something like "bxc1=N", instead into
	// the valid move "b1=Q". We try to fix this case. But note that this may lead
	// to succeeding illegal moves, so it's not sure that this fix helps to decode
	// the complete game.

	if (	m_move.moved() == piece::Pawn
		&& sq::rank(m_move.from()) == (m_position.blackToMove() ? sq::Rank2 : sq::Rank7))
	{
		sq::ID to = sq::make(sq::fyle(m_move.from()), m_position.blackToMove() ? sq::Rank1 : sq::Rank8);
		m_move = Move::genPromote(m_move.from(), to, piece::Queen);

		unsigned pieceNum = value >> 4;
		if (m_position.blackToMove())
			pieceNum |= 0x10;

		if (m_position.doMove(m_move, pieceNum))
			return;
	}

	// TODO: more clever error handling
	M_THROW(DecodingFailedException("Invalid move"));
}


void
Decoder::decodeVariation(unsigned level)
{
	bool preComment = false;

	while (true)
	{
		Byte b;

		while (__builtin_expect((b = m_strm.get()) > token::Last, 1))
		{
			if (!m_position.doMove(m_move, decodeMove(b)))
				handleInvalidMove(b);

			m_currentNode->setNext(new MoveNode(m_move));
			m_currentNode = m_currentNode->next();

			if (preComment)
			{
				m_currentNode->setComment(move::Ante);
				preComment = false;
			}
		}

		if (b < token::First)
		{
			if (!m_position.doMove(m_move, decodeMove(b)))
				handleInvalidMove(b);

			m_currentNode->setNext(new MoveNode(m_move));
			m_currentNode = m_currentNode->next();

			if (preComment)
			{
				m_currentNode->setComment(move::Ante);
				preComment = false;
			}
		}
		else
		{
			switch (b)
			{
				case token::End_Game:
					if (level > 0)
					{
						::throwCorruptData();
					}
					else
					{
						MoveNode* node = new MoveNode;
						m_currentNode->setNext(node);

						if (preComment)
							node->setComment(move::Post);
					}
					return;

				case token::End_Marker:
					if (level == 0)
					{
						::throwCorruptData();
					}
					else
					{
						MoveNode* node = new MoveNode;
						m_currentNode->setNext(node);

						if (preComment)
							node->setComment(move::Post);
					}
					return;

				case token::Start_Marker:
				{
					if (!m_currentNode->move())
						throwCorruptData();

					MoveNode* current = m_currentNode;

					m_position.push();
					m_position.undoMove(current->move());
					current->addVariation(m_currentNode = new MoveNode);
					decodeVariation(level + 1);

					if (m_currentNode->atLineStart())
					{
						// Scidb does not support empty variations...

						if (m_currentNode->hasNote() || m_currentNode->next()->hasNote())
						{
							// ...but we cannot delete the variation if an annotation/comment exists.

							if (	!m_currentNode->annotation().isEmpty()
								|| m_strm.peek() == token::Start_Marker)
							{
#ifndef ALLOW_EMPTY_VARS
								// As a workaround we insert a null move.

								MoveNode* node = new MoveNode(m_position.board().makeNullMove());
								node->setNext(m_currentNode->removeNext());
								m_currentNode->setNext(node);
#endif
							}
							else
							{
								// This is the last variation. Handle this comment as a pre-comment.
								current->deleteVariation(current->countVariations() - 1);
								preComment = true;
							}
						}
						else
						{
							current->deleteVariation(current->countVariations() - 1);
						}
					}

					m_currentNode = current;
					m_position.pop();
					break;
				}

				case token::Nag:
					static_assert(Annotation::Max_Nags >= 7, "Scid needs at least seven entries");
					m_currentNode->addAnnotation(nag::fromScid3(nag::ID(m_strm.get())));
					break;

				case token::Comment:
					m_currentNode->setComment(move::Post);
					break;
			}
		}
	}
}


void
Decoder::skipTags()
{
	Byte b;

	while ((b = m_strm.get()))
	{
		if (__builtin_expect(b == 255, 0))
		{
			// special binary 3-byte encoding of EventDate (obsolete since 3.x)
			m_strm.skip(3);
		}
		else
		{
			if (b <= ::Max_Tag_Length)
				m_strm.skip(b);
			m_strm.skip(m_strm.get());
		}
	}
}


void
Decoder::determineCharsetTags()
{
	Byte tagLen;

	while ((tagLen = m_strm.get()))
	{
		if (tagLen <= ::Max_Tag_Length)
		{
			char*	tag = const_cast<char*>(reinterpret_cast<char const*>(m_strm.data()));

			m_strm.skip(tagLen);
			Byte len = m_strm.get();

			// NOTE: we ignore invalid tags (Scid bug)
			if (Reader::validateTagName(tag, tagLen))
				HandleData(reinterpret_cast<char const*>(m_strm.data()), len);

			m_strm.skip(len);
		}
		else if (tagLen == 255)
		{
			// special binary 3-byte encoding of EventDate (obsolete since 3.x)
			m_strm.skip(3);
		}
		else
		{
			// a common tag name, not explicitly stored
			Byte len = m_strm.get();
			HandleData(reinterpret_cast<char const*>(m_strm.data()), len);
			m_strm.skip(len);
		}
	}
}


void
Decoder::decodeTags(TagSet& tags)
{
	Byte significance[2] = { 2, 2 };
	mstl::string out;
	Byte tagLen;

	while ((tagLen = m_strm.get()))
	{
		if (tagLen <= ::Max_Tag_Length)
		{
			char*	tag = const_cast<char*>(reinterpret_cast<char const*>(m_strm.data()));

			m_strm.skip(tagLen);
			Byte len = m_strm.get();

			// NOTE: we ignore invalid tags (Scid bug)
			if (Reader::validateTagName(tag, tagLen))
			{
				// look if this is a known tag with an unusual notation

				switch (tag::ID id = tag::fromName(tag, tagLen))
				{
					case tag::TimeControl:
						if (!tags.contains(tag::TimeMode))
						{
							util::NulString s(reinterpret_cast<char*>(m_strm.data()), len);
							time::Mode mode = Reader::getTimeModeFromTimeControl(s);

							if (mode != time::Unknown)
							{
								tags.set(tag::TimeMode, time::toString(mode));

								if (mode == time::Corr && !tags.contains(tag::EventType))
									tags.set(tag::EventType, event::toString(event::PaperMail));
							}
						}
						tags.set(tag::TimeControl, reinterpret_cast<char const*>(m_strm.data()), len);
						break;

					case tag::Variant:
						{
							char const* s = reinterpret_cast<char const*>(m_strm.data());

							tags.set(tag::Variant, variant::identifier(variant::fromString(s)));

							if (tags.contains(tag::Variant))
								m_hasVariantTag = true;
							else
								tags.setExtra(tag, tagLen, s, len);
						}
						break;

						case tag::EventCountry:
							{
								char const* s = reinterpret_cast<char const*>(m_strm.data());

								if (len == 3)
								{
									util::NulString v(reinterpret_cast<char*>(m_strm.data()), len);
									country::Code code = country::fromString(v);

									if (code != country::Unknown)
										tags.set(tag::EventCountry, country::toString(code));
								}

								if (!tags.contains(tag::EventCountry))
									tags.set(tag::EventCountry, s, len);
							}
							break;

					case tag::Termination:
						{
							util::NulString v(reinterpret_cast<char*>(m_strm.data()), len);
							termination::Reason reason = Reader::getTerminationReason(v);
							if (reason == termination::Unknown)
								tags.setExtra(tag, tagLen, v, len);
							else
								tags.set(tag::Termination, termination::toString(reason));
						}
						break;

					case tag::Fen: // should not happen
						break;

					case tag::ExtraTag:
						{
							mstl::string in;
							in.hook(reinterpret_cast<char*>(m_strm.data()), len);
							m_codec->convertToUtf8(in, out);
							if (!sys::utf8::validate(out))
								m_codec->forceValidUtf8(out);
							tags.setExtra(tag, tagLen, out, len);
						}
						break;

					case tag::EventDate:
						// this case happens if a tag like "Eventdate" is detected
						{
							Date date;
							date.parseFromString(reinterpret_cast<char const*>(m_strm.data()), len);
							if (date)
								tags.set(tag::EventDate, date.asString());
						}
						break;

					default:
						{
							util::NulString in(reinterpret_cast<char*>(m_strm.data()), len);
							m_codec->convertToUtf8(in, out);
							if (!sys::utf8::validate(out))
								m_codec->forceValidUtf8(out);

							tags.set(id, out);

							if (tag::isRatingTag(id))
							{
								color::ID color = tag::isWhiteRatingTag(id) ? color::White : color::Black;

								tags.setSignificance(id, significance[color]);
								significance[color] = 0;
							}
						}
						break;
				}
			}

			m_strm.skip(len);
		}
		else
		{
			Byte len = m_strm.get();

			switch (tagLen)
			{
				case 245:	// event date
					{
						char const*	data = reinterpret_cast<char const*>(m_strm.data());
						Date date;

						date.parseFromString(data, len);
						if (date)
							tags.set(tag::EventDate, date.asString());
					}
					break;

				case 255:	// special binary 3-byte encoding of EventDate (obsolete since 3.x)
					{
						unsigned value = (((len << 8) | m_strm.get()) << 8) | m_strm.get();
						Date		date;

						date.setYMD(value >> 9, (value >> 5) & 15, value & 31);
						if (date)
							tags.set(tag::EventDate, mstl::string(date.asString()));
						len = 0;
					}
					break;

				default:		// a common tag name, not explicitly stored
					{
						tag::ID tag = ::CommonTags[tagLen - ::Max_Tag_Length - 1];

						if (__builtin_expect(tag == tag::ExtraTag, 0))
							throwCorruptData();

						switch (unsigned(tag))
						{
							case tag::WhiteCountry:
							case tag::BlackCountry:
								if (len == 3)
								{
									util::NulString v(reinterpret_cast<char*>(m_strm.data()), len);
									country::Code code = country::fromString(v);

									if (code != country::Unknown)
									{
										tags.set(tag, country::toString(code));
										tag = tag::ExtraTag;
									}
								}
								break;

							case tag::Annotator:
							case tag::Source:
							case tag::Opening:
							case tag::Variation:
								{
									util::NulString in(reinterpret_cast<char*>(m_strm.data()), len);
									m_codec->convertToUtf8(in, out);
									if (!sys::utf8::validate(out))
										m_codec->forceValidUtf8(out);
									tags.set(tag, out);
									tag = tag::ExtraTag;
								}
								break;

							case tag::SetUp:	// not needed
								tag = tag::ExtraTag;
								break;
						}

						if (tag != tag::ExtraTag)
							tags.set(tag::toName(tag), reinterpret_cast<char const*>(m_strm.data()), len);
					}
					break;
			}

			m_strm.skip(len);
		}
	}
}


void
Decoder::determineCharsetComments(MoveNode* node)
{
	if (node->hasComment(move::Post))
	{
		mstl::string content;
		m_strm.get(content);
		HandleData(content, content.size());
	}

	for (node = node->next(); node; node = node->next())
	{
		if (node->hasSupplement())
		{
			if (node->hasComment(move::Ante))
			{
				mstl::string content;
				m_strm.get(content);
				HandleData(content, content.size());
			}

			if (node->hasComment(move::Post))
			{
				mstl::string content;
				m_strm.get(content);
				HandleData(content, content.size());
			}

			for (unsigned i = 0; i < node->variationCount(); ++i)
				determineCharsetComments(node->variation(i));
		}
	}
}


void
Decoder::decodeComments(MoveNode* node, Consumer* consumer)
{
	mstl::string buffer;

	if (node->hasComment(move::Post))
	{
		mstl::string	content;
		Comment			comment;
		MarkSet			marks;

		buffer.clear();
		m_strm.get(content);
		marks.extractFromComment(content);
		node->swapMarks(marks);
		m_codec->toUtf8(content, buffer);

		if (!content.empty())
		{
			if (!sys::utf8::validate(buffer))
				m_codec->forceValidUtf8(buffer);

			if (Comment::convertCommentToXml(buffer, comment, encoding::Utf8))
				node->addAnnotation(nag::Diagram);

			comment.normalize();
			if (!node->next()->atLineEnd())
				node->swapComment(comment, move::Ante);
			node->swapComment(comment, move::Post);
		}
	}

	for (node = node->next(); node; node = node->next())
	{
		if (node->hasSupplement())
		{
			if (node->hasComment(move::Ante))
			{
				mstl::string	content;
				Comment			comment;
				MarkSet			marks;

				m_strm.get(content);
				marks.extractFromComment(content);
				node->swapMarks(marks);

				if (!content.empty())
				{
					buffer.clear();
					m_codec->toUtf8(content, buffer);

					if (!sys::utf8::validate(buffer))
						m_codec->forceValidUtf8(buffer);

					if (Comment::convertCommentToXml(buffer, comment, encoding::Utf8))
						node->addAnnotation(nag::Diagram);

					if (	node->prev()->atLineStart()
						&& node->isBeforeLineEnd()
						&& node->prev()->hasComment(move::Ante))
					{
						Comment total;
						node->prev()->swapComment(total, move::Ante);
						total.append(comment, ' ');
						total.normalize();
						node->swapComment(total, move::Ante);
					}
					else
					{
						comment.normalize();
						node->swapComment(comment, move::Ante);
					}
				}
			}
			else if (	node->prev()->atLineStart()
						&& node->isBeforeLineEnd()
						&& node->prev()->hasComment(move::Ante))
			{
				Comment comment;
				node->prev()->swapComment(comment, move::Ante);
				node->swapComment(comment, move::Ante);
			}

			if (node->hasComment(move::Post))
			{
				mstl::string	content;
				Comment			comment;
				MarkSet			marks;
				MoveInfoSet		moveInfo;

				buffer.clear();
				m_strm.get(content);
				marks.extractFromComment(content);
				node->swapMarks(marks);

				moveInfo.extractFromComment(m_engines, content);
				if (!moveInfo.isEmpty())
					node->swapMoveInfo(moveInfo);

				if (!content.empty())
				{
					m_codec->toUtf8(content, buffer);

					if (!sys::utf8::validate(buffer))
						m_codec->forceValidUtf8(buffer);

					if (Comment::convertCommentToXml(buffer, comment, encoding::Utf8))
						node->addAnnotation(nag::Diagram);

					comment.normalize();
					node->swapComment(comment, move::Post);
				}
			}

			for (unsigned i = 0; i < node->variationCount(); ++i)
				decodeComments(node->variation(i), consumer);
		}
		else if (	node->prev()->atLineStart()
					&& node->isBeforeLineEnd()
					&& node->prev()->hasComment(move::Ante))
		{
			Comment comment;
			node->prev()->swapComment(comment, move::Ante);
			node->prev()->swapComment(comment, move::Post);
		}
	}
}


void
Decoder::checkVariant(TagSet& tags)
{
	Board const& startBoard = m_position.startBoard();

	if (!startBoard.isStandardPosition(variant::Normal))
	{
		if (startBoard.isChess960Position(variant::Normal))
		{
			tags.set(tag::Variant, chess960::identifier());
		}
		else if (startBoard.isShuffleChessPosition(variant::Normal))
		{
			tags.set(tag::Variant, shuffle::identifier());
		}
		else if (m_hasVariantTag)
		{
			if (!startBoard.notDerivableFromChess960())
				tags.set(tag::Variant, shuffle::identifier());
			else if (!startBoard.notDerivableFromStandardChess())
				tags.set(tag::Variant, chess960::identifier());
		}
	}
}


unsigned
Decoder::doDecoding(uint16_t* line, unsigned length, Board& startBoard, bool useStartBoard)
{
	skipTags();

	if (m_strm.get() & flags::Non_Standard_Start)
	{
		mstl::string fen;
		m_strm.get(fen);
		m_position.setup(fen);
	}
	else
	{
		m_position.setup();
	}

	if (!useStartBoard)
		startBoard = m_position.board();
	else if (startBoard.isEqualZHPosition(m_position.board()))
		useStartBoard = false;

	unsigned index = 0;

	while (index < length)
	{
		Byte b;

		if (__m_likely((b = m_strm.get()) > token::Last) || b < token::First)
		{
			if (!m_position.doMove(m_move, decodeMove(b)))
				handleInvalidMove(b);

			if (!useStartBoard)
			{
				line[index++] = m_move.index();
			}
			else if (startBoard.isEqualZHPosition(m_position.board()))
			{
				startBoard.setPlyNumber(m_position.board().plyNumber());
				useStartBoard = false;
			}
		}
		else
		{
			switch (b)
			{
				case token::End_Game:
				case token::End_Marker:
					return index;

				case token::Start_Marker:
					skipVariation();
					break;

				case token::Nag:
					m_strm.skip(1);
					break;
			}
		}
	}

	return index;
}


save::State
Decoder::doDecoding(db::Consumer& consumer, TagSet& tags)
{
	// determine character set
	Reset();
	{
		unsigned pos = m_strm.tellg();
		determineCharsetTags();
		m_strm.seekg(pos);
	}

	decodeTags(tags);

	if (	consumer.variant() == variant::ThreeCheck
		&& (	!tags.contains(tag::Variant)
			|| variant::fromString(tags.value(tag::Variant)) != variant::ThreeCheck))
	{
		return save::UnsupportedVariant;
	}

	if (	consumer.variant() == variant::Normal
		&& tags.contains(tag::Variant)
		&& variant::fromString(tags.value(tag::Variant)) == variant::ThreeCheck)
	{
		return save::UnsupportedVariant;
	}

	if (m_strm.get() & flags::Non_Standard_Start)
	{
		mstl::string fen;
		m_strm.get(fen);
		m_position.setup(fen);
		tags.set(tag::SetUp, "1");	// bad PGN design
		tags.set(tag::Fen, fen);
	}
	else
	{
		m_position.setup();
	}

	checkVariant(tags);

	if (!consumer.startGame(tags, m_position.board()))
		return save::UnsupportedVariant;

	consumer.startMoveSection();

	MoveNode start;
	m_currentNode = &start;

	decodeVariation();

	// determine character set
	{
		unsigned pos = m_strm.tellg();
		determineCharsetComments(&start);
		m_strm.seekg(pos);
	}
	DataEnd();

	decodeComments(&start, &consumer);
	m_engines.swap(consumer.engines());
	decodeVariation(consumer, &start);
	consumer.finishMoveSection(result::fromString(tags.value(tag::Result)));

	m_givenCodec->setError(m_codec->error());
	m_givenCodec->setUnknown(m_codec->unknown());

	return consumer.finishGame(tags);
}


unsigned
Decoder::doDecoding(GameData& data)
{
	// determine character set
	Reset();
	{
		unsigned pos = m_strm.tellg();
		determineCharsetTags();
		m_strm.seekg(pos);
	}

	decodeTags(data.m_tags);

	if (m_strm.get() & flags::Non_Standard_Start)
	{
		mstl::string fen;
		m_strm.get(fen);
		m_position.setup(fen);
	}
	else
	{
		m_position.setup();
	}

	checkVariant(data.m_tags);

	unsigned plyNumber = m_position.board().plyNumber();

	data.m_startBoard = m_position.board();
	m_currentNode = data.m_startNode;

	M_ASSERT(m_currentNode);

	decodeVariation();

	// determine character set
	{
		unsigned pos = m_strm.tellg();
		determineCharsetComments(data.m_startNode);
		m_strm.seekg(pos);
	}
	DataEnd();

	decodeComments(data.m_startNode);
	data.m_engines.swap(m_engines);

	m_givenCodec->setError(m_codec->error());
	m_givenCodec->setUnknown(m_codec->unknown());

	return m_position.board().plyNumber() - plyNumber;
}


void
Decoder::decodeVariation(Consumer& consumer, MoveNode const* node)
{
	M_ASSERT(node);

	for (node = node->next(); node; node = node->next())
	{
		if (node->atLineEnd())
		{
			if (node->hasComment(move::Post))
				consumer.putTrailingComment(node->comment(move::Post));
		}
		else
		{
			if (node->hasNote())
			{
				consumer.putMove(	node->move(),
										node->annotation(),
										node->comment(move::Ante),
										node->comment(move::Post),
										node->marks());
			}
			else
			{
				consumer.putMove(node->move());
			}

			if (node->hasMoveInfo())
				consumer.putMoveInfo(node->moveInfo());

			for (unsigned i = 0; i < node->variationCount(); ++i)
			{
				consumer.startVariation();
				decodeVariation(consumer, node->variation(i));
				consumer.finishVariation();
			}
		}
	}
}


void
Decoder::skipVariation()
{
	unsigned count = 1;

	while (true)
	{
		Byte b = m_strm.get();

		// We don't need to check 'token::First <= b', it is working anyway.
		if (__builtin_expect(b <= token::Last/* && token::First <= b*/, 0))
		{
			switch (b)
			{
				case token::End_Game:
					::throwCorruptData();
					// never reached

				case token::Start_Marker:
					++count;
					break;

				case token::End_Marker:
					if (--count == 0)
						return;
					break;

				case token::Nag:
					m_strm.skip(1);
					break;
			}
		}
	}
}


Move
Decoder::nextMove()
{
	while (true)
	{
		Byte b = m_strm.get();

		if (__builtin_expect(b > token::Last, 1) || token::First > b)
		{
			if (!m_position.doMove(m_move, decodeMove(b)))
				handleInvalidMove(b);
			return m_move;
		}

		switch (b)
		{
			case token::End_Game:
				return Move::empty();

			case token::End_Marker:
				::throwCorruptData();
				// not reached

			case token::Start_Marker:
				skipVariation();
				break;

			case token::Nag:
				m_strm.skip(1);
				break;
		}
	}

	return m_move;	// not reached
}


Move
Decoder::findExactPosition(Board const& position, bool skipVariations)
{
	skipTags();

	if (m_strm.get() & flags::Non_Standard_Start)
	{
		mstl::string fen;
		m_strm.get(fen);
		m_position.setup(fen);
	}
	else
	{
		m_position.setup();
	}

	if (m_position.board().isEqualPosition(position))
		return nextMove();

	while (true)
	{
		Byte b;

		while (__builtin_expect((b = m_strm.get()) > token::Last, 1))
		{
			if (!m_position.doMove(m_move, decodeMove(b)))
				handleInvalidMove(b);

			if (m_position.board().isEqualPosition(position))
				return nextMove();

			if (!m_position.board().signature().isReachablePawns(position.signature()))
				return Move::invalid();
		}

		if (b < token::First)
		{
			if (!m_position.doMove(m_move, decodeMove(b)))
				handleInvalidMove(b);

			if (m_position.board().isEqualPosition(position))
				return nextMove();

			if (!m_position.board().signature().isReachablePawns(position.signature()))
				return Move::invalid();
		}
		else
		{
			switch (b)
			{
				case token::End_Game:
					return Move::invalid();

				case token::End_Marker:
					::throwCorruptData();
					// not reached

				case token::Start_Marker:
					if (skipVariations)
					{
						this->skipVariation();
					}
					else if (m_move)
					{
						m_position.push();
						m_position.undoMove(m_move);
						m_move = findExactPosition(position, false);
						m_position.pop();

						if (m_move)
							return m_move;
					}
					else
					{
						throwCorruptData();
					}
					break;

				case token::Nag:
					m_strm.skip(1);
					break;
			}
		}
	}

	return m_move;	// not reached
}


type::ID
Decoder::decodeType(unsigned type)
{
	switch (type)
	{
		case  0: return type::Unspecific;				// Unspecific
		case  1: return type::Temporary;					// Temporary Database
		case  2: return type::Clipbase;					// Clipbase
		case  3: return type::PGNFile;					// PGN format file
		case  4: return type::My_Games;					// My games
		case  5: return type::Large_Database;			// Large database
		case  6: return type::Correspondence_Chess;	// Correspondence chess
		case  7: return type::Computer_Chess;			// Computer chess
		case  8: return type::Unspecific;				// Sorted index of games
		case  9: return type::Player_Collection;		// Player collection
		case 10: return type::Tournament;				// Tournament: All-play-all
		case 11: return type::Tournament_Swiss;		// Tournament: Swiss
		case 12: return type::GM_Games;					// Grandmaster games
		case 13: return type::IM_Games;					// International master games
		case 14: return type::Blitz_Games;				// Blitz (fast) games
		case 15: return type::Tactics;					// Tactics
		case 16: return type::Endgames;					// Endgames
		case 17: return type::Openings_White;			// Openings for White
		case 18: return type::Openings_Black;			// Openings for Black
		case 19: return type::Openings;					// Openings for either color
	};

	return type::Openings;									// Theory ...
}


void
Decoder::Report(char const* charset)
{
	if (sys::utf8::Codec::ascii() != charset && m_codec->encoding() != charset)
		m_codec = new sys::utf8::Codec(charset);
}


mstl::string const&
Decoder::encoding() const
{
	return m_codec->encoding();
}

// vi:set ts=3 sw=3:
