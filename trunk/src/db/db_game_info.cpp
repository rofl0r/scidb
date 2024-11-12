// ======================================================================
// Author : $Author$
// Version: $Revision: 28 $
// Date   : $Date: 2011-05-21 14:57:26 +0000 (Sat, 21 May 2011) $
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

#include "db_game_info.h"
#include "db_provider.h"
#include "db_board.h"
#include "db_tag_set.h"
#include "db_eco_table.h"
#include "db_player.h"
#include "db_namebases.h"

#include "u_crc.h"

#include "sys_utf8_codec.h"

#include "m_bit_functions.h"
#include "m_assert.h"
#include "m_utility.h"
#include "m_stdio.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define GAME_INFO_VAR

using namespace db;
using namespace db::color;


static NamebaseEntry		g_empty;
static NamebaseEvent		g_event;
static NamebasePlayer	g_player;

static char const GameFlagMap[22] =
{
	'w', // Flag_White_Opening
	'b', // Flag_Black_Opening
	'm', // Flag_Middle_Game
	'e', // Flag_End_Game
	'N', // Flag_Novelty
	'p', // Flag_Pawn_Structure
	'T', // Flag_Tactics
	'K', // Flag_King_Side
	'Q', // Flag_Queen_Side
	'!', // Flag_Brilliancy
	'?', // Flag_Blunder
	'U', // Flag_User
	'*', // Flag_Best_Game
	'D', // Flag_Decided_Tournament
	'G', // Flag_Model_Game
	'S', // Flag_Strategy
	'^', // Flag_With_Attack
	'~', // Flag_Sacrifice
	'=', // Flag_Defense
	'M', // Flag_Material
	'P', // Flag_Piece_Play
	'I', // Flag_Illegal_Move,
//	't', // Flag_Tactical_Blunder
//	's', // Flag_Strategical_Blunder
};


template <typename T>
inline static uint32_t
computeCRC(uint32_t crc, T const& v)
{
	return util::crc::compute(crc, reinterpret_cast<unsigned char const*>(&v), sizeof(T));
}


inline static uint32_t
computeCRC(uint32_t crc, mstl::string const& s)
{
	return util::crc::compute(crc, s, s.size());
}


// NOTE: compatible to Scid 3.x
static uint8_t
encodeCount(unsigned count)
{
	uint16_t const Lookup[] =
	{
		 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,	// exact
		10, 10, 10,											// 10-12
		11, 11, 11, 11, 11,								// 13-17
		12, 12, 12, 12, 12, 12, 12,					// 18-24
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13,	// 25-34
		14, 14, 14, 14, 14, 14, 14, 14, 14, 14,	// 35-44
		15,													// >= 45
	};

	return Lookup[mstl::min(size_t(count), U_NUMBER_OF(Lookup) - 1)];
}


template <typename Int>
inline static
Int
setFlag(Int flags, unsigned at, bool value)
{
	return value ? flags | at : flags & ~at;
}


static void
recode(	NamebaseEntry* entry,
			sys::utf8::Codec& oldCodec,
			sys::utf8::Codec& newCodec,
			mstl::string& buf)
{
	M_ASSERT(entry);

	if (sys::utf8::Codec::is7BitAscii(entry->name()))
		return;

	oldCodec.convertFromUtf8(entry->name(), buf);
	newCodec.convertToUtf8(buf, buf);
	entry->setName(buf);
}


static uint16_t
getRatingValue(TagSet const& tags, tag::ID tag)
{
	if (!tags.contains(tag))
		return 0;

	int value = tags.asInt(tag);

	if (value > rating::Max_Value)
		return 0;

	return value;
}


GameInfo const GameInfo::m_initializer((Initializer()));


GameInfo::GameInfo(Initializer const&)
	:m_event(&g_event)
	,m_annotator(&g_empty)
	,m_variationCount(0)
	,m_annotationCount(0)
	,m_commentCount(0)
	,m_termination(termination::Unknown)
	,m_gameOffset(0)
	,m_plyCount(0)
	,m_ecoKey(0)
	,m_ecoOpening(0)
	,m_eco(Eco::root())
	,m_result(result::Unknown)
	,m_gameFlags(0)
	,m_positionId(chess960::StandardIdn)
	,m_dateYear(Date::Zero10Bits)
	,m_dateDay(0)
	,m_dateMonth(0)
	,m_round(0)
	,m_subround(0)
{
	m_pd[White].value = m_pd[Black].value = 0;
	m_player[White] = m_player[Black] = &g_player;
	::memset(&m_signature, 0, sizeof(m_signature));
}


GameInfo::GameInfo() { *this = m_initializer; }

bool GameInfo::isEmpty() const { return m_event == &g_event; }


Eco
GameInfo::ecoFromOpening() const
{
	if (m_positionId != chess960::StandardIdn)
		return Eco();

	return m_ecoOpening ? Eco(m_ecoOpening) : Eco::root();
}


Eco
GameInfo::userEco() const
{
	if (m_positionId != chess960::StandardIdn)
		return Eco();

	return m_eco ? Eco::fromShort(m_eco) : ecoFromOpening();
}


void
GameInfo::setupOpening(unsigned idn, Line const& line)
{
	m_positionId = idn;

	switch (idn)
	{
		case 0:
			break;

		case chess960::StandardIdn:
			{
				Eco opening;
				m_ecoKey = EcoTable::specimen().lookup(line, opening);
				m_ecoOpening = opening;
			}
			break;

		default:
			switch (line.length)
			{
				case 0:
					break;

				case 1:
					m_ply1 = line.moves[0];
					break;

				case 2:
					m_ply1 = line.moves[0];
					m_ply2 = line.moves[1];
					break;

				case 3:
					m_ply1 = line.moves[0];
					m_ply2 = line.moves[1];
					m_ply3 = line.moves[2];
					break;

				default:
					m_ply1 = line.moves[0];
					m_ply2 = line.moves[1];
					m_ply3 = line.moves[2];
					m_ply4 = line.moves[3];
					break;
			}
			break;
	}
}


void
GameInfo::update(	NamebasePlayer* whitePlayer,
						NamebasePlayer* blackPlayer,
						NamebaseEvent* event,
						NamebaseEntry* annotator,
						TagSet const& tags,
						Namebases& namebases)
{
	M_REQUIRE(!isEmpty());
	M_REQUIRE(whitePlayer);
	M_REQUIRE(blackPlayer);
	M_REQUIRE(event);
	M_REQUIRE((reinterpret_cast<long>(annotator) & 1) == 0);

	if (annotator)
	{
		m_annotator = annotator;
		namebases(Namebase::Annotator).ref(m_annotator);
	}

	namebases(Namebase::Player).ref(m_player[color::White] = whitePlayer);
	namebases(Namebase::Player).ref(m_player[color::Black] = blackPlayer);
	namebases(Namebase::Site).ref(event->site());
	namebases(Namebase::Event).ref(m_event = event);

	m_dateYear	= Date::Zero10Bits;
	m_dateMonth	= 0;
	m_dateDay	= 0;
	m_result		= result::Unknown;
	m_eco			= Eco();
	m_round		= 0;
	m_subround	= 0;

	for (unsigned i = 0; i < tag::ExtraTag; ++i)
	{
		if (tags.contains(tag::ID(i)))
		{
			switch (int(tag::ID(i)))
			{
				case tag::Round:
					{
						char* s = const_cast<char*>(tags.value(tag::Round).c_str());
						m_round = ::strtoul(s, &s, 10);
						if (*s == '.')
							m_subround = ::strtoul(s + 1, 0, 10);
					}
					break;

				case tag::Result:
					m_result = result::fromString(tags.value(tag::Result));
					break;

				case tag::Eco:
					m_eco = Eco::toShort(tags.value(tag::Eco));
					break;

				case tag::WhiteElo:
					whitePlayer->setElo(m_pd[White].elo = tags.asInt(tag::WhiteElo));
					break;

				case tag::BlackElo:
					blackPlayer->setElo(m_pd[Black].elo = tags.asInt(tag::BlackElo));
					break;

				case tag::WhiteRating:	break;
				case tag::WhiteRapid:	break;
				case tag::WhiteICCF:		break;
				case tag::WhiteUSCF:		break;
				case tag::WhiteDWZ:		break;
				case tag::WhiteECF:		break;
				case tag::WhiteIPS:		break;
					{
						if (uint16_t value = ::getRatingValue(tags, tag::ID(i)))
						{
							rating::Type rt = rating::fromTag(tag::ID(i));

							whitePlayer->setRating(rt, value);

							if (tags.significance(tag::ID(i)))
								setRating(White, rt, value);
						}
					}
					break;

				case tag::BlackRating:	break;
				case tag::BlackRapid:	break;
				case tag::BlackICCF:		break;
				case tag::BlackUSCF:		break;
				case tag::BlackDWZ:		break;
				case tag::BlackECF:		break;
				case tag::BlackIPS:		break;
					{
						if (uint16_t value = ::getRatingValue(tags, tag::ID(i)))
						{
							rating::Type rt = rating::fromTag(tag::ID(i));

							blackPlayer->setRating(rt, value);

							if (tags.significance(tag::ID(i)))
								setRating(Black, rt, value);
						}
					}
					break;

				case tag::Date:
					{
						Date date(tags.value(tag::Date));
						m_dateYear = Date::encodeYearTo10Bits(date.year());
						m_dateMonth = date.month();
						m_dateDay = date.day();
					}
					break;
			}
		}
	}
}


void
GameInfo::setup(	uint32_t gameOffset,
						uint32_t gameRecordLength,
						NamebasePlayer* whitePlayer,
						NamebasePlayer* blackPlayer,
						NamebaseEvent* event,
						NamebaseEntry* annotator,
						Namebases& namebases)
{
	M_REQUIRE(whitePlayer);
	M_REQUIRE(blackPlayer);
	M_REQUIRE(event);
	M_REQUIRE((reinterpret_cast<long>(annotator) & 1) == 0);

	if (annotator)
	{
		m_annotator = annotator;
		namebases(Namebase::Annotator).ref(m_annotator);
	}
	else
	{
		setGameRecordLength(gameRecordLength);
	}

	m_gameOffset		= gameOffset;
	m_player[White]	= whitePlayer;
	m_player[Black]	= blackPlayer;
	m_event				= event;

	namebases(Namebase::Player).ref(whitePlayer);
	namebases(Namebase::Player).ref(blackPlayer);
	namebases(Namebase::Site).ref(event->site());
	namebases(Namebase::Event).ref(event);
}


void
GameInfo::setup(	uint32_t gameOffset,
						uint32_t gameRecordLength,
						NamebasePlayer* whitePlayer,
						NamebasePlayer* blackPlayer,
						NamebaseEvent* event,
						NamebaseEntry* annotator,
						TagSet const& tags,
						Provider const& provider,
						Namebases& namebases)
{
	M_REQUIRE(isEmpty());

	setup(gameOffset, gameRecordLength, whitePlayer, blackPlayer, event, annotator, namebases);

	m_gameOffset	= gameOffset;
	m_gameFlags		= provider.flags();
	m_signature		= provider.getFinalBoard().signature();
	m_result			= result::fromString(tags.value(tag::Result));
	m_plyCount		= mstl::min(MaxPlyCount, provider.plyCount());

	char* s = const_cast<char*>(tags.value(tag::Round).c_str());
	m_round = ::strtoul(s, &s, 10);
	if (*s == '.')
		m_subround = ::strtoul(s + 1, 0, 10);

	m_variationCount  = ::encodeCount(provider.countVariations());
	m_commentCount    = ::encodeCount(provider.countComments());
	m_annotationCount = ::encodeCount(provider.countAnnotations() + provider.countMarks());

	if (tags.contains(tag::EventType))
		m_event->setType(event::typeFromString(tags.value(tag::EventType)));

	{
		material::Count matCount;

		matCount = provider.getFinalBoard().materialCount(White);

		m_pd[0].matQ = matCount.queen  >= 3;
		m_pd[0].matR = matCount.rook   >= 3;
		m_pd[0].matB = matCount.bishop >= 3;
		m_pd[0].matN = matCount.knight >= 3;

		matCount = provider.getFinalBoard().materialCount(Black);

		m_pd[1].matQ = matCount.queen  >= 3;
		m_pd[1].matR = matCount.rook   >= 3;
		m_pd[1].matB = matCount.bishop >= 3;
		m_pd[1].matN = matCount.knight >= 3;
	}

	if (tags.contains(tag::WhiteElo))
		whitePlayer->setElo(m_pd[White].elo = tags.asInt(tag::WhiteElo));
	if (tags.contains(tag::BlackElo))
		blackPlayer->setElo(m_pd[Black].elo = tags.asInt(tag::BlackElo));

	M_STATIC_CHECK(rating::Last == 8, Reimplementation_Needed);

	for (unsigned i = rating::Elo + 1; i < rating::Any; ++i)
	{
		tag::ID tag = tag::fromRating(White, rating::Type(i));

		if (uint16_t value = ::getRatingValue(tags, tag))
		{
			whitePlayer->setRating(rating::Type(i), value);

			if (tags.significance(tag))
				setRating(White, rating::Type(i), value);
		}

		tag = tag::fromRating(Black, rating::Type(i));

		if (uint16_t value = ::getRatingValue(tags, tag))
		{
			blackPlayer->setRating(rating::Type(i), value);

			if (tags.significance(tag))
				setRating(Black, rating::Type(i), value);
		}
	}

	M_REQUIRE(eventEntry()->country() == country::fromString(tags.value(tag::EventCountry)));
	M_REQUIRE(eventEntry()->eventMode() == event::modeFromString(tags.value(tag::Mode)));
	M_REQUIRE(eventEntry()->timeMode() == time::fromString(tags.value(tag::TimeMode)));
	M_REQUIRE(eventEntry()->date() == Date(tags.value(tag::EventDate)));

//	if (tags.contains(tag::EventCountry))
//		m_event->setCountry(country::fromString(tags.value(tag::EventCountry)));
//	if (tags.contains(tag::Mode))
//		m_event->setEventMode(event::modeFromString(tags.value(tag::Mode)));
//	if (tags.contains(tag::TimeMode))
//		m_event->setTimeMode(time::fromString(tags.value(tag::TimeMode)));
//	if (tags.contains(tag::EventDate))
//		m_event->setDate(Date(tags.value(tag::EventDate)));

	if (tags.contains(tag::Termination))
		m_termination = termination::fromString(tags.value(tag::Termination));

	setupOpening(provider.getStartBoard().computeIdn(), provider.openingLine());

	if (m_positionId == chess960::StandardIdn)
	{
		if (tags.contains(tag::Eco))
			m_eco = Eco::toShort(tags.value(tag::Eco));
		else
			m_eco = EcoTable::specimen().getEco(provider.openingLine()).toShort();
	}

	M_REQUIRE(
			!tags.contains(tag::WhiteCountry)
		|| playerEntry(color::White)->federation() == country::fromString(tags.value(tag::WhiteCountry)));
	M_REQUIRE(
			!tags.contains(tag::BlackCountry)
		|| playerEntry(color::Black)->federation() == country::fromString(tags.value(tag::BlackCountry)));
	M_REQUIRE(
			!tags.contains(tag::WhiteTitle)
		|| playerEntry(color::White)->title() == title::fromString(tags.value(tag::WhiteTitle)));
	M_REQUIRE(
			!tags.contains(tag::BlackTitle)
		|| playerEntry(color::Black)->title() == title::fromString(tags.value(tag::BlackTitle)));
	M_REQUIRE(
			!tags.contains(tag::WhiteType)
		|| playerEntry(color::White)->type() == species::fromString(tags.value(tag::WhiteType)));
	M_REQUIRE(
			!tags.contains(tag::BlackType)
		|| playerEntry(color::Black)->type() == species::fromString(tags.value(tag::BlackType)));
	M_REQUIRE(
			!tags.contains(tag::WhiteSex)
		|| playerEntry(color::White)->sex() == sex::fromString(tags.value(tag::WhiteSex)));
	M_REQUIRE(
			!tags.contains(tag::BlackSex)
		|| playerEntry(color::Black)->sex() == sex::fromString(tags.value(tag::BlackSex)));

//	if (tags.contains(tag::WhiteCountry))
//		whitePlayer->setFederation(country::Code(country::fromString(tags.value(tag::WhiteCountry))));
//	if (tags.contains(tag::BlackCountry))
//		blackPlayer->setFederation(country::Code(country::fromString(tags.value(tag::BlackCountry))));
//
//	if (tags.contains(tag::WhiteTitle))
//		whitePlayer->setTitle(title::fromString(tags.value(tag::WhiteTitle)));
//	if (tags.contains(tag::BlackTitle))
//		blackPlayer->setTitle(title::fromString(tags.value(tag::BlackTitle)));
//
//	if (tags.contains(tag::WhiteType))
//		whitePlayer->setPlayerType(species::fromString(tags.value(tag::WhiteType)));
//	if (tags.contains(tag::BlackType))
//		blackPlayer->setPlayerType(species::fromString(tags.value(tag::BlackType)));
//
//	if (tags.contains(tag::WhiteSex))
//		whitePlayer->setSex(sex::fromChar(*tags.value(tag::WhiteSex)));
//	if (tags.contains(tag::BlackSex))
//		blackPlayer->setSex(sex::fromChar(*tags.value(tag::BlackSex)));

	if (tags.contains(tag::Date))
	{
		Date date(tags.value(tag::Date));
		m_dateYear = Date::encodeYearTo10Bits(date.year());
		m_dateMonth = date.month();
		m_dateDay = date.day();
	}
}


void
GameInfo::update(Provider const& provider)
{
	m_plyCount			= mstl::min(GameInfo::MaxPlyCount, provider.plyCount());
	m_variationCount	= ::encodeCount(provider.countVariations());
	m_commentCount		= ::encodeCount(provider.countComments());
	m_annotationCount	= ::encodeCount(provider.countAnnotations() + provider.countMarks());

	// XXX: m_eco = game.ecoCode() ???
	setupOpening(provider.getStartBoard().computeIdn(), provider.openingLine());
}


void
GameInfo::reset(Namebases& namebases)
{
	if (!isEmpty())
	{
		namebases(Namebase::Player).deref(m_player[White]);
		namebases(Namebase::Player).deref(m_player[Black]);
		namebases(Namebase::Site  ).deref(m_event->site());
		namebases(Namebase::Event ).deref(m_event);

		if (!m_recordLengthFlag)
			namebases(Namebase::Annotator).deref(m_annotator);

		*this = m_initializer;
	}
}


void
GameInfo::resetCharacteristics(Namebases& namebases)
{
	M_REQUIRE(!isEmpty());

	namebases(Namebase::Player).deref(m_player[White]);
	namebases(Namebase::Player).deref(m_player[Black]);
	namebases(Namebase::Site  ).deref(m_event->site());
	namebases(Namebase::Event ).deref(m_event);

	if (!m_recordLengthFlag)
		namebases(Namebase::Annotator).deref(m_annotator);

	m_dateYear	= Date::Zero10Bits;
	m_dateMonth	= 0;
	m_dateDay	= 0;
	m_result		= result::Unknown;
	m_eco			= Eco();
	m_round		= 0;
	m_subround	= 0;
}


void
GameInfo::restore(GameInfo& oldInfo, Namebases& namebases)
{
	*this = oldInfo;

	namebases(Namebase::Player).ref(m_player[White]);
	namebases(Namebase::Player).ref(m_player[Black]);
	namebases(Namebase::Site  ).ref(m_event->site());
	namebases(Namebase::Event ).ref(m_event);
}


void
GameInfo::setupIdn(TagSet& tags, uint16_t idn)
{
	M_ASSERT(idn <= 4*960);

#ifdef GAME_INFO_IDN
	tags.remove(tag::Idn);	// it's too dangerous to keep a user supplied value
#endif

	if (idn == 0)
		return;

	if (idn != chess960::StandardIdn)
	{
		tags.set(tag::SetUp, 1);
#ifdef GAME_INFO_IDN
		tags.add(tag::Idn, idn);
#endif

		if (idn > 960)
		{
			tags.set(tag::Fen, shuffle::fen(idn));
			tags.set(tag::Variant, shuffle::identifier());
			tags.add(tag::Opening, shuffle::position(idn));
		}
		else
		{
			tags.set(tag::Fen, chess960::fen(idn));
			tags.set(tag::Variant, chess960::identifier());
			tags.add(tag::Opening, chess960::position(idn));
		}
	}
}


void
GameInfo::setupTags(TagSet& tags) const
{
	tags.set(tag::Event,		m_event->name());
	tags.set(tag::Site,		m_event->site()->name());
	tags.set(tag::White,		m_player[White]->name());
	tags.set(tag::Black,		m_player[Black]->name());
	tags.set(tag::Result,	result::toString(result::ID(m_result)));

	if (m_round)
		tags.set(tag::Round, roundAsString());

	if (!m_recordLengthFlag)
		tags.set(tag::Annotator, m_annotator->name());

	if (m_event->type() != event::Unknown)
		tags.set(tag::EventType, event::toString(m_event->type()));
	if (m_event->country() != country::Unknown)
		tags.set(tag::EventCountry, country::toString(m_event->country()));
	if (m_player[White]->federation())
		tags.set(tag::WhiteCountry, country::toString(country::Code(m_player[White]->federation())));
	if (m_player[Black]->federation())
		tags.set(tag::BlackCountry, country::toString(country::Code(m_player[Black]->federation())));
	if (m_player[White]->title())
		tags.set(tag::WhiteTitle, title::toString(m_player[White]->title()));
	if (m_player[Black]->title())
		tags.set(tag::BlackTitle, title::toString(m_player[Black]->title()));
	if (m_player[White]->type() != species::Unspecified)
		tags.set(tag::WhiteType, species::toString(m_player[White]->type()));
	if (m_player[Black]->type() != species::Unspecified)
		tags.set(tag::BlackType, species::toString(m_player[Black]->type()));
	if (m_player[White]->sex() != sex::Unspecified)
		tags.set(tag::WhiteSex, sex::toString(m_player[White]->sex()));
	if (m_player[Black]->sex() != sex::Unspecified)
		tags.set(tag::BlackSex, sex::toString(m_player[Black]->sex()));

	if (m_dateYear != Date::Zero10Bits)
	{
		tags.set(tag::Date,
					Date(Date::decodeYearFrom10Bits(m_dateYear), m_dateMonth, m_dateDay).asString());
	}

	if (m_event->hasDate())
		tags.set(tag::EventDate, m_event->date().asString());

	if (m_pd[White].elo)
	{
		tags.set(tag::WhiteElo, m_pd[White].elo);
		tags.setSignificance(tag::WhiteElo, 1);
	}
	if (m_pd[Black].elo)
	{
		tags.set(tag::BlackElo, m_pd[Black].elo);
		tags.setSignificance(tag::BlackElo, 1);
	}

	if (uint16_t score = m_pd[White].rating)
	{
		tag::ID t = rating::toWhiteTag(rating::Type(m_pd[White].ratingType));

		tags.set(t, score);
		tags.setSignificance(t, m_pd[White].elo ? 2 : 1);
	}
	if (uint16_t score = m_pd[Black].rating)
	{
		tag::ID t = rating::toBlackTag(rating::Type(m_pd[Black].ratingType));

		tags.set(t, score);
		tags.setSignificance(t, m_pd[Black].elo ? 2 : 1);
	}

	tags.set(tag::Termination,	termination::toString(termination::Reason(m_termination)));
	tags.set(tag::Mode,			event::toString(m_event->eventMode()));
	tags.set(tag::TimeMode,		time::toString(m_event->timeMode()));

	setupIdn(tags, m_positionId);

	if (m_positionId == chess960::StandardIdn)
	{
		tags.set(tag::Eco, Eco::fromShort(m_eco).asShortString());

		Eco eco = m_eco ? Eco::fromShort(m_eco) : ecoFromOpening();

		if (	eco
			&& !tags.isUserSupplied(tag::Opening)
			&& !tags.isUserSupplied(tag::Variation)
			&& !tags.isUserSupplied(tag::SubVariation))
		{
			mstl::string opening, variation, subvariation;
			EcoTable::specimen().getOpening(eco, opening, variation, subvariation);
			tags.add(tag::Opening,			opening);
#ifdef GAME_INFO_VAR
			tags.add(tag::Variation,		variation);
			tags.add(tag::SubVariation,	subvariation);
#endif
		}
	}

#ifdef GAME_INFO_PLYCOUNT
	// IMPORTANT NOTE:
	// The ply count may be slightly incorrect if the source is .cbh!
	// The tag value will be corrected afterwards (after loading).
	tags.set(tag::PlyCount, m_plyCount);
#endif
}


void
GameInfo::setupTags(TagSet& tags, Provider const& provider)
{
	mstl::string opening, variation, subvariation;
	unsigned idn = provider.getStartBoard().computeIdn();

	setupIdn(tags, idn);

	if (idn == chess960::StandardIdn)
	{
		Eco eco = EcoTable::specimen().getEco(provider.openingLine());
		EcoTable::specimen().getOpening(eco, opening, variation, subvariation);
		tags.add(tag::Eco, eco.asShortString());
	}

	if (	!tags.isUserSupplied(tag::Opening)
		&& !tags.isUserSupplied(tag::Variation)
		&& !tags.isUserSupplied(tag::SubVariation))
	{
		tags.add(tag::Opening,			opening);
#ifdef GAME_INFO_VAR
		tags.add(tag::Variation,		variation);
		tags.add(tag::SubVariation,	subvariation);
#endif
	}

#ifdef GAME_INFO_PLYCOUNT
	tags.set(tag::PlyCount, provider.plyCount());
#endif
}


void
GameInfo::setRecord(uint32_t offset, uint32_t length)
{
	m_gameOffset = offset;

	if (m_recordLengthFlag)
		setGameRecordLength(length);
}


void
GameInfo::recode(sys::utf8::Codec& oldCodec, sys::utf8::Codec& newCodec)
{
	mstl::string buf;

	// XXX wrong! namebase should be recoded!
	::recode(m_event,				oldCodec, newCodec, buf);
	::recode(m_event->site(),	oldCodec, newCodec, buf);
	::recode(m_player[White],	oldCodec, newCodec, buf);
	::recode(m_player[Black],	oldCodec, newCodec, buf);

	if (!m_recordLengthFlag)
		::recode(m_annotator, oldCodec, newCodec, buf);
}


void
GameInfo::setDeleted(bool flag)
{
	m_gameFlags = ::setFlag(m_gameFlags, Flag_Deleted, flag);
}


void
GameInfo::setChanged(bool flag)
{
	m_gameFlags = ::setFlag(m_gameFlags, Flag_Changed, flag);
}


void
GameInfo::setDirty(bool flag)
{
	m_gameFlags = ::setFlag(m_gameFlags, Flag_Dirty, flag);
}


material::si3::Signature
GameInfo::material() const
{
	material::si3::Signature res;
	material::SigPart sig;

	sig = m_signature.material(White);
	res.wq = mstl::bf::count_bits(sig.queen)  + m_pd[0].matQ;
	res.wr = mstl::bf::count_bits(sig.rook)   + m_pd[0].matR;
	res.wb = mstl::bf::count_bits(sig.bishop) + m_pd[0].matB;
	res.wn = mstl::bf::count_bits(sig.knight) + m_pd[0].matN;
	res.wp = mstl::bf::count_bits(sig.pawn);

	sig = m_signature.material(Black);
	res.bq = mstl::bf::count_bits(sig.queen)  + m_pd[1].matQ;
	res.br = mstl::bf::count_bits(sig.rook)   + m_pd[1].matR;
	res.bb = mstl::bf::count_bits(sig.bishop) + m_pd[1].matB;
	res.bn = mstl::bf::count_bits(sig.knight) + m_pd[1].matN;
	res.bp = mstl::bf::count_bits(sig.pawn);

	return res;
}


void
GameInfo::setMaterial(material::si3::Signature sig)
{
	m_signature.m_material.part[White].queen   = (1 << sig.wq) - 1;
	m_signature.m_material.part[White].rook    = (1 << sig.wr) - 1;
	m_signature.m_material.part[White].bishop  = (1 << sig.wb) - 1;
	m_signature.m_material.part[White].knight  = (1 << sig.wn) - 1;
	m_signature.m_material.part[White].pawn    = (1 << sig.wp) - 1;

	m_signature.m_material.part[Black].queen   = (1 << sig.bq) - 1;
	m_signature.m_material.part[Black].rook    = (1 << sig.br) - 1;
	m_signature.m_material.part[Black].bishop  = (1 << sig.bb) - 1;
	m_signature.m_material.part[Black].knight  = (1 << sig.bn) - 1;
	m_signature.m_material.part[Black].pawn    = (1 << sig.bp) - 1;

	m_pd[0].matQ = ((1 << sig.wq) - 1) >> 2;
	m_pd[0].matR = ((1 << sig.wr) - 1) >> 2;
	m_pd[0].matB = ((1 << sig.wb) - 1) >> 2;
	m_pd[0].matN = ((1 << sig.wn) - 1) >> 2;

	m_pd[1].matQ = ((1 << sig.bq) - 1) >> 2;
	m_pd[1].matR = ((1 << sig.br) - 1) >> 2;
	m_pd[1].matB = ((1 << sig.bb) - 1) >> 2;
	m_pd[1].matN = ((1 << sig.bn) - 1) >> 2;
}


uint16_t
GameInfo::playerRating(color::ID color, rating::Type type) const
{
	if (type == rating::Elo)
		return playerElo(color);

	if (uint16_t rating = m_player[color]->playerHighestRating(type))
		return rating;

	PlayerData const& pd = m_pd[color];

	if (pd.rating && ((1 << type) & ((1 << rating::Any) | (1 << pd.ratingType))))
		return pd.rating;

	return 0;
}


uint16_t
GameInfo::findRating(color::ID color, rating::Type type) const
{
	if (type == rating::Elo)
		return findElo(color);

	PlayerData const& pd = m_pd[color];

	if (type == rating::Any)
	{
		if (pd.rating)
			return pd.rating;

		if (pd.elo)
			return pd.elo;
	}

	if (pd.rating && type == rating::Type(pd.ratingType))
		return pd.rating;

	return m_player[color]->findRating(type);
}


rating::Type
GameInfo::findRatingType(color::ID color) const
{
	PlayerData const& pd = m_pd[color];

	if (pd.rating)
		return rating::Type(pd.ratingType);

	if (pd.elo)
		return rating::Elo;

	return m_player[color]->findRatingType();
}


bool
GameInfo::isGameRating(color::ID color, rating::Type type) const
{
	PlayerData const& pd = m_pd[color];

	if (type == rating::Elo)
		return pd.elo > 0;

	M_ASSERT((pd.rating == 0) == (pd.ratingType == rating::Elo));

	if (pd.elo && type == rating::Any)
		return true;

	if (pd.rating == 0)
		return false;

	return type == rating::Any || rating::Type(pd.ratingType) == type;
}


//uint16_t
//GameInfo::playerHighestRating(color::ID color, rating::Type type) const
//{
//	if (type == rating::Elo)
//		return playerHighestElo(color);
//
//	uint16_t rating = m_player[color]->playerHighestRating(type);
//
//	if (rating)
//		return rating;
//
//	PlayerData const& pd = m_pd[color];
//
//	if (pd.rating && ((1 << type) & ((1 << rating::Any) | (1 << pd.ratingType))))
//		return uint16_t(pd.rating);
//
//	return 0;
//}
//
//
//uint16_t
//GameInfo::playerLatestRating(color::ID color, rating::Type type) const
//{
//	if (type == rating::Elo)
//		return playerLatestElo(color);
//
//	uint16_t rating = m_player[color]->playerLatestRating(type);
//
//	if (rating)
//		return rating;
//
//	PlayerData const& pd = m_pd[color];
//
//	if (pd.rating && ((1 << type) & ((1 << rating::Any) | (1 << pd.ratingType))))
//		return uint16_t(pd.rating);
//
//	return 0;
//}
//
//
//rating::Type
//GameInfo::playerRatingType(color::ID color) const
//{
//	rating::Type type = m_player[color]->playerRatingType();
//
//	if (type != rating::Any)
//		return type;
//
//	return ratingType(color);
//}


uint32_t
GameInfo::computeChecksum() const
{
	unsigned crc = 0;

	crc = ::computeCRC(crc, m_event->name());
	crc = ::computeCRC(crc, m_event->site()->name());
	crc = ::computeCRC(crc, uint16_t(m_event->site()->country()));
	crc = ::computeCRC(crc, annotator());
	crc = ::computeCRC(crc, m_player[White]->name());
	crc = ::computeCRC(crc, m_player[Black]->name());
	crc = ::computeCRC(crc, uint16_t(m_player[White]->federation()));
	crc = ::computeCRC(crc, uint16_t(m_player[Black]->federation()));
	crc = ::computeCRC(crc, uint8_t(m_player[White]->title()));
	crc = ::computeCRC(crc, uint8_t(m_player[Black]->title()));
	crc = ::computeCRC(crc, uint8_t(m_player[White]->type()));
	crc = ::computeCRC(crc, uint8_t(m_player[Black]->type()));
	crc = ::computeCRC(crc, uint8_t(m_player[White]->sex()));
	crc = ::computeCRC(crc, uint8_t(m_player[Black]->sex()));
	crc = ::computeCRC(crc, uint8_t(date()));
	crc = ::computeCRC(crc, uint16_t(eventDate()));
	crc = ::computeCRC(crc, uint8_t(eventType()));
	crc = ::computeCRC(crc, uint8_t(timeMode()));
	crc = ::computeCRC(crc, uint8_t(eventMode()));
	crc = ::computeCRC(crc, m_signature);
	crc = ::computeCRC(crc, material().value);
	crc = ::computeCRC(crc, uint16_t(m_eco));
	crc = ::computeCRC(crc, m_ecoKey);
	crc = ::computeCRC(crc, m_ecoOpening);
	crc = ::computeCRC(crc, uint8_t(m_annotationCount));
	crc = ::computeCRC(crc, uint8_t(m_commentCount));
	crc = ::computeCRC(crc, uint8_t(m_variationCount));
	crc = ::computeCRC(crc, uint8_t(m_termination));
	crc = ::computeCRC(crc, uint32_t(m_gameFlags));
	crc = ::computeCRC(crc, uint8_t(m_result));
	crc = ::computeCRC(crc, uint16_t(m_plyCount));
	crc = ::computeCRC(crc, uint16_t(m_positionId));
	crc = ::computeCRC(crc, uint8_t(m_pd[White].value));
	crc = ::computeCRC(crc, uint8_t(m_pd[Black].value));

	return crc;
}


char
GameInfo::mapFlag(uint32_t flag)
{
	M_REQUIRE(flag <= Flag_Illegal_Move);
	M_REQUIRE(flag > Flag_Deleted);

	return GameFlagMap[mstl::bf::lsb_index(flag) - 1];
}


mstl::string&
GameInfo::flagsToString(uint32_t flags, mstl::string& result)
{
	unsigned size = result.size();

	for (unsigned i = 0; i < U_NUMBER_OF(::GameFlagMap); ++i)
	{
		if (flags & (1 << (i + 1)))
		{
			result += ::GameFlagMap[i];
			result += ' ';
		}
	}

	if (result.size() > size)
		result.resize(result.size() - 1);

	return result;
}


unsigned
GameInfo::stringToFlags(char const* str)
{
	unsigned result = 0;

	for ( ; *str; ++str)
	{
		if (!::isspace(*str))
		{
			for (unsigned i = 0; i < U_NUMBER_OF(::GameFlagMap); ++i)
			{
				if (*str == GameFlagMap[i])
					result |= (1 << (i + 1));
			}
		}
	}

	return result;
}


mstl::string
GameInfo::roundAsString() const
{
	mstl::string s;

	if (m_round == 0)
		s.append('?');
	else if (m_subround)
		s.format("%u.%u", unsigned(m_round), unsigned(m_subround));
	else
		s.format("%u", unsigned(m_round));

	return s;
}


void
GameInfo::debug() const
{
	mstl::string s;

	::printf(   "Event:            %s\n", event().c_str());
	::printf(   "Site:             %s\n", site().c_str());
	::printf(   "Date:             %s\n", date().asString().c_str());
	::printf("   Round:            %s\n", roundAsString().c_str());
	::printf(   "White:            %s\n", playerName(White).c_str());
	::printf(   "Black:            %s\n", playerName(Black).c_str());
	::printf(   "White Elo:        %u\n", unsigned(elo(White)));
	::printf(   "Black Elo:        %u\n", unsigned(elo(Black)));
	::printf(	"White Type:       %s\n", species::toString(playerType(color::White)).c_str());
	::printf(	"Black Type:       %s\n", species::toString(playerType(color::Black)).c_str());
	::printf(	"White Sex:        %c\n", sex::toChar(sex(color::White)));
	::printf(	"Black Sex:        %c\n", sex::toChar(sex(color::Black)));
	::printf(   "White Rating:     %u (%s)\n",
					unsigned(rating(White)), rating::toString(ratingType(White)).c_str());
	::printf(   "Black Rating:     %u (%s)\n",
					unsigned(rating(Black)), rating::toString(ratingType(Black)).c_str());
	::printf(   "White Country:    %s\n", country::toString(m_player[White]->federation()));
	::printf(   "Black Country:    %s\n", country::toString(m_player[Black]->federation()));
	::printf(   "White Title:      %s\n", title::toString(m_player[White]->title()).c_str());
	::printf(   "Black Title:      %s\n", title::toString(m_player[White]->title()).c_str());
	::printf(   "Result:           %s\n", result::toString(result()).c_str());
	::printf(   "Annotator:        %s\n", annotator().c_str());
	::printf(	"EventCountry:     %s\n", country::toString(eventCountry()));
	::printf(   "EventDate:        %s\n", eventDate().asString().c_str());
	::printf(	"EventType:        %s\n", event::toString(eventType()).c_str());
	::printf(   "Termination:      %s\n", termination::toString(terminationReason()).c_str());
	::printf(   "Mode:             %s\n", event::toString(eventMode()).c_str());
	::printf(	"Time Mode:        %s\n", time::toString(timeMode()).c_str());
	::printf(   "IDN:              %u\n", unsigned(idn()));
	::printf(   "Eco:              %s\n", eco().asString().c_str());
	if (idn() == chess960::StandardIdn)
	{
		::printf("Eco Key:          %s\n", Eco(m_ecoKey).asString().c_str());
		::printf("Eco Opening:      %s\n", Eco(m_ecoOpening).asString().c_str());
	}
	else if (idn())
	{
		::printf("Ply 1:            %s-%s\n",
					sq::printAlgebraic(Move(m_ply1).from()), sq::printAlgebraic(Move(m_ply1).to()));
		::printf("Ply 2:            %s-%s\n",
					sq::printAlgebraic(Move(m_ply2).from()), sq::printAlgebraic(Move(m_ply2).to()));
		::printf("Ply 3:            %s-%s\n",
					sq::printAlgebraic(Move(m_ply3).from()), sq::printAlgebraic(Move(m_ply3).to()));
		::printf("Ply 4:            %s-%s\n",
					sq::printAlgebraic(Move(m_ply4).from()), sq::printAlgebraic(Move(m_ply4).to()));
	}
	::printf(   "Ply Count:        %u\n", unsigned(plyCount()));
	::printf(   "Deleted:          %s\n", isDeleted() ? "yes" : "no");
	::printf(   "Flags:            %s\n", flagsToString(flags(), s).c_str());
	::printf(   "Annotations       %u\n", unsigned(countAnnotations()));
	::printf(   "Comments:         %u\n", unsigned(countComments()));
	::printf(   "Variations:       %u\n", unsigned(countVariations()));
	::printf(   "File Offset:      %u\n", unsigned(gameOffset()));
	::printf(   "Record Length:    %u\n", unsigned(gameRecordLength()));
	::printf(   "Dirty:            %s\n", isDirty() ? "yes" : "no");
	::printf(   "Modified:         %s\n", isChanged() ? "yes" : "no");

	m_signature.debug(2);
	::fflush(stdout);
}

// vi:set ts=3 sw=3:
