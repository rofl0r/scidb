// ======================================================================
// Author : $Author$
// Version: $Revision: 1422 $
// Date   : $Date: 2017-08-18 10:27:34 +0000 (Fri, 18 Aug 2017) $
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
#include "m_limits.h"
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

static sys::utf8::Codec* g_latin1Codec = 0;

static char const GameFlagMap[24] =
{
	'-', // Flag_Deleted
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
	'C', // Flag_Illegal_Castling,
	'I', // Flag_Illegal_Move,
//	't', // Flag_Tactical_Blunder
//	's', // Flag_Strategical_Blunder
};


inline static util::crc::checksum_t
computeCRC(util::crc::checksum_t crc, mstl::string const& s)
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


static uint16_t
getRatingValue(TagSet const& tags, tag::ID tag)
{
	if (!tags.contains(tag))
		return 0;

	unsigned value = tags.asInt(tag);
	return rating::isValid(value) ? value : 0;
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
	,m_gameFlags(0)
	,m_dateDay(0)
	,m_setup(false)
	,m_positionId(variant::Standard)
	,m_round(0)
	,m_subround(0)
	,m_dateMonth(0)
	,m_ecoKey(1)
	,m_eco(Eco::root())
	,m_rest(0)
	,m_dateYear(Date::Zero10Bits)
	,m_plyCount(0)
	,m_result(result::Unknown)
{
#if defined(__i386__)
	static_assert(sizeof(GameInfo) == 64, "should be 64 bytes");
#endif

	m_pd[White].value = m_pd[Black].value = 0;
	m_player[White] = m_player[Black] = &g_player;
	::memset(&m_signature, 0, sizeof(m_signature));
}


GameInfo::GameInfo() { *this = m_initializer; }

bool GameInfo::isEmpty() const { return m_event == &g_event; }


Eco
GameInfo::userEco(variant::Type variant) const
{
	// TODO: variant still unused
	if (m_positionId != variant::Standard)
		return Eco();

	return m_eco ? Eco::fromShort(m_eco) : Eco(m_ecoKey);
}


void
GameInfo::setupOpening(bool startPosition, variant::Index variantIndex, unsigned idn, Line const& line)
{
	m_positionId = idn;
	m_setup = !startPosition;

	switch (idn)
	{
		case 0:
			m_ecoKey = 0;
			break;

		case variant::Standard:
			m_ecoKey = EcoTable::specimen(variantIndex).lookup(line);
			break;

		default:
			m_ecoKey = 0;
			switch (line.length)
			{
				case 0:
					break;

				case 1:
					m_ply[0] = line.moves[0];
					break;

				default:
					m_ply[0] = line.moves[0];
					m_ply[1] = line.moves[1];
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
	M_REQUIRE(annotator);
	M_REQUIRE((reinterpret_cast<long>(annotator) & 1) == 0);

	if (annotator != namebases(Namebase::Annotator).emptyAnnotator())
	{
		m_annotator = annotator;
		namebases(Namebase::Annotator).incrRef(m_annotator);
	}

	if (event->frequency() == 0)
		namebases(Namebase::Site).incrRef(event->site());

	namebases(Namebase::Player).incrRef(m_player[color::White] = whitePlayer);
	namebases(Namebase::Player).incrRef(m_player[color::Black] = blackPlayer);
	namebases(Namebase::Event).incrRef(m_event = event);

	m_dateYear		= Date::Zero10Bits;
	m_dateMonth		= 0;
	m_dateDay		= 0;
	m_result			= result::Unknown;
	m_eco				= Eco();
	m_round			= 0;
	m_subround		= 0;
	m_termination	= termination::Unknown;

	for (tag::ID tag = tags.findFirst(); tag <= tag::LastTag; tag = tags.findNext(tag))
	{
		switch (int(tag))
		{
			case tag::Round:
				{
					char* s = const_cast<char*>(tags.value(tag::Round).c_str());
					m_round = ::strtoul(s, &s, 10);
					if (*s == '.')
						m_subround = ::strtoul(s + 1, nullptr, 10);
				}
				break;

			case tag::Result:
				m_result = result::fromString(tags.value(tag::Result));
				break;

			case tag::Eco:
				m_eco = Eco::toShort(tags.value(tag::Eco));
				break;

			case tag::WhiteElo:
			{
				unsigned value = tags.asInt(tag::WhiteElo);
				if (rating::isValid(value))
					whitePlayer->setElo(m_pd[White].elo = value);
				break;
			}

			case tag::BlackElo:
			{
				unsigned value = tags.asInt(tag::BlackElo);
				if (rating::isValid(value))
					blackPlayer->setElo(m_pd[Black].elo = value);
				break;
			}

			case tag::WhiteRating:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					whitePlayer->setRating(rating::Rating, value);

					if (tags.significance(tag))
						setRating(White, rating::Rating, value);
				}
				break;

			case tag::WhiteRapid:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					whitePlayer->setRating(rating::Rapid, value);

					if (tags.significance(tag))
						setRating(White, rating::Rapid, value);
				}
				break;

			case tag::WhiteICCF:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					whitePlayer->setRating(rating::ICCF, value);

					if (tags.significance(tag))
						setRating(White, rating::ICCF, value);
				}
				break;

			case tag::WhiteUSCF:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					whitePlayer->setRating(rating::USCF, value);

					if (tags.significance(tag))
						setRating(White, rating::USCF, value);
				}
				break;

			case tag::WhiteDWZ:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					whitePlayer->setRating(rating::DWZ, value);

					if (tags.significance(tag))
						setRating(White, rating::DWZ, value);
				}
				break;

			case tag::WhiteECF:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					whitePlayer->setRating(rating::ECF, value);

					if (tags.significance(tag))
						setRating(White, rating::ECF, value);
				}
				break;

			case tag::WhiteIPS:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					whitePlayer->setRating(rating::IPS, value);

					if (tags.significance(tag))
						setRating(White, rating::IPS, value);
				}
				break;

			case tag::BlackRating:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					blackPlayer->setRating(rating::Rating, value);

					if (tags.significance(tag))
						setRating(Black, rating::Rating, value);
				}
				break;

			case tag::BlackRapid:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					blackPlayer->setRating(rating::Rapid, value);

					if (tags.significance(tag))
						setRating(Black, rating::Rapid, value);
				}
				break;

			case tag::BlackICCF:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					blackPlayer->setRating(rating::ICCF, value);

					if (tags.significance(tag))
						setRating(Black, rating::ICCF, value);
				}
				break;

			case tag::BlackUSCF:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					blackPlayer->setRating(rating::USCF, value);

					if (tags.significance(tag))
						setRating(Black, rating::USCF, value);
				}
				break;

			case tag::BlackDWZ:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					blackPlayer->setRating(rating::DWZ, value);

					if (tags.significance(tag))
						setRating(Black, rating::DWZ, value);
				}
				break;

			case tag::BlackECF:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					blackPlayer->setRating(rating::ECF, value);

					if (tags.significance(tag))
						setRating(Black, rating::ECF, value);
				}
				break;

			case tag::BlackIPS:
				if (uint16_t value = ::getRatingValue(tags, tag))
				{
					blackPlayer->setRating(rating::IPS, value);

					if (tags.significance(tag))
						setRating(Black, rating::IPS, value);
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

			case tag::Termination:
				m_termination = termination::fromString(tags.value(tag::Termination));
				break;
		}
	}
}


void
GameInfo::setup(uint32_t gameOffset, uint32_t gameRecordLength)
{
	m_gameOffset = gameOffset;

	if (hasGameRecordLength())
		setGameRecordLength(gameRecordLength);
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
	M_REQUIRE(annotator);
	M_REQUIRE((reinterpret_cast<long>(annotator) & 1) == 0);

	if (annotator == namebases(Namebase::Annotator).emptyAnnotator())
	{
		setGameRecordLength(gameRecordLength);
	}
	else
	{
		m_annotator = annotator;
		namebases(Namebase::Annotator).incrRef(m_annotator);
	}

	m_gameOffset		= gameOffset;
	m_player[White]	= whitePlayer;
	m_player[Black]	= blackPlayer;
	m_event				= event;

	if (event->frequency() == 0)
		namebases(Namebase::Site).incrRef(event->site());

	namebases(Namebase::Player).incrRef(whitePlayer);
	namebases(Namebase::Player).incrRef(blackPlayer);
	namebases(Namebase::Event).incrRef(event);
}


void
GameInfo::setupRating(TagSet const& tags, color::ID color, rating::Type rtType, tag::ID tag)
{
	if (uint16_t value = ::getRatingValue(tags, tag))
	{
		if (m_player[color]->rating(rtType) == 0)
		{
			m_player[color]->setRating(rtType, value);

			if (rtType == rating::Elo || tags.significance(tag))
				setRating(color, rtType, value);
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
						uint16_t whiteElo,
						uint16_t blackElo,
						TagSet const& tags,
						Provider const& provider,
						Namebases& namebases)
{
	M_REQUIRE(isEmpty());
	M_REQUIRE(rating::isValid(whiteElo));
	M_REQUIRE(rating::isValid(blackElo));

	setup(gameOffset, gameRecordLength, whitePlayer, blackPlayer, event, annotator, namebases);

	unsigned langFlags = provider.langFlags();

	m_gameOffset		= gameOffset;
	m_signature			= provider.getFinalBoard().signature();
	m_result				= result::fromString(tags.value(tag::Result));
	m_plyCount			= mstl::min(MaxPlyCount, provider.plyCount());
	m_pd[0].langFlag	= bool(langFlags & i18n::English);
	m_pd[1].langFlag	= bool(langFlags & i18n::Other_Lang);

	m_gameFlags = (provider.gameFlags() & ~Flag_Special) | (m_gameFlags & Flag_Special);

	char* s = const_cast<char*>(tags.value(tag::Round).c_str());
	m_round = ::strtoul(s, &s, 10);
	if (*s == '.')
		m_subround = ::strtoul(s + 1, nullptr, 10);

	m_variationCount  = ::encodeCount(provider.countVariations());
	m_commentCount    = ::encodeCount(provider.countComments());
	m_annotationCount = ::encodeCount(	provider.countAnnotations()
//												 + provider.countMoveInfo()
												 + provider.countMarks());

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

	if (whiteElo == 0 && tags.contains(tag::WhiteElo))
		whiteElo = rating::clip(tags.asInt(tag::WhiteElo));
	whitePlayer->setElo(m_pd[White].elo = whiteElo);

	if (blackElo == 0 && tags.contains(tag::BlackElo))
		blackElo = rating::clip(tags.asInt(tag::BlackElo));
	blackPlayer->setElo(m_pd[Black].elo = blackElo);

	static_assert(rating::Last == 8, "reimplementation required");

	setupRating(tags, White, rating::Rating,	tag::WhiteRating);
	setupRating(tags, White, rating::Rapid,	tag::WhiteRapid);
	setupRating(tags, White, rating::ICCF,		tag::WhiteICCF);
	setupRating(tags, White, rating::USCF,		tag::WhiteUSCF);
	setupRating(tags, White, rating::DWZ,		tag::WhiteDWZ);
	setupRating(tags, White, rating::ECF,		tag::WhiteECF);
	setupRating(tags, White, rating::IPS,		tag::WhiteIPS);

	setupRating(tags, Black, rating::Rating,	tag::BlackRating);
	setupRating(tags, Black, rating::Rapid,	tag::BlackRapid);
	setupRating(tags, Black, rating::ICCF,		tag::BlackICCF);
	setupRating(tags, Black, rating::USCF,		tag::BlackUSCF);
	setupRating(tags, Black, rating::DWZ,		tag::BlackDWZ);
	setupRating(tags, Black, rating::ECF,		tag::BlackECF);
	setupRating(tags, Black, rating::IPS,		tag::BlackIPS);

	M_REQUIRE(eventEntry()->country() == country::fromString(tags.value(tag::EventCountry)));
	M_REQUIRE(eventEntry()->eventMode() == event::modeFromString(tags.value(tag::Mode)));
	M_REQUIRE(eventEntry()->timeMode() == time::fromString(tags.value(tag::TimeMode)));
	M_REQUIRE(eventEntry()->date() == Date(tags.value(tag::EventDate)));

	if (tags.contains(tag::Termination))
		m_termination = termination::fromString(tags.value(tag::Termination));

	variant::Type	variant = provider.variant();
	variant::Index	variantIndex = variant::toIndex(variant::toMainVariant(variant));

	setupOpening(	provider.getStartBoard().isStartPosition(),
						variantIndex,
						provider.idn(),
						provider.openingLine());

	if (variant == variant::Giveaway)
		m_gameFlags |= Flag_Giveaway;

	if (m_positionId == variant::Standard)
	{
		if (tags.contains(tag::Eco))
			m_eco = Eco::toShort(tags.value(tag::Eco));
		else
			m_eco = EcoTable::specimen(variantIndex).getEco(provider.openingLine()).toShort();
	}

#if 0
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
#endif

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

	variant::Type variant = provider.variant();

	if (variant == variant::Giveaway)
		m_gameFlags |= Flag_Giveaway;
	else if (variant == variant::Suicide)
		m_gameFlags &= ~Flag_Giveaway;

	setupOpening(	provider.getStartBoard().isStartPosition(),
						variant::toIndex(variant::toMainVariant(variant)),
						provider.idn(),
						provider.openingLine());
}


void
GameInfo::reset(Namebases& namebases)
{
	if (!isEmpty())
	{
		unsigned specialFlags = m_gameFlags & Flag_Special;

		namebases(Namebase::Player).decrRef(m_player[White]);
		namebases(Namebase::Player).decrRef(m_player[Black]);
		namebases(Namebase::Event ).decrRef(m_event);

		if (m_event->frequency() == 0)
			namebases(Namebase::Site).decrRef(m_event->site());

		unsigned gameOffset = m_gameOffset;

		if (unsigned recordLength = gameRecordLength())
		{
			*this = m_initializer;
			setGameRecordLength(recordLength);
		}
		else
		{
			namebases(Namebase::Annotator).decrRef(m_annotator);
			*this = m_initializer;
		}

		m_gameFlags |= specialFlags;
		m_gameOffset = gameOffset;
	}
}


void
GameInfo::resetCharacteristics(Namebases& namebases)
{
	M_REQUIRE(!isEmpty());

	namebases(Namebase::Player).decrRef(m_player[White]);
	namebases(Namebase::Player).decrRef(m_player[Black]);
	namebases(Namebase::Event ).decrRef(m_event);

	if (m_event->frequency() == 0)
		namebases(Namebase::Site).decrRef(m_event->site());

	if (!hasGameRecordLength())
		namebases(Namebase::Annotator).decrRef(m_annotator);

	m_dateYear		= Date::Zero10Bits;
	m_dateMonth		= 0;
	m_dateDay		= 0;
	m_result			= result::Unknown;
	m_termination	= termination::Unknown;
	m_eco				= Eco();
	m_round			= 0;
	m_subround		= 0;
	m_pd[0].value	= 0;
	m_pd[1].value	= 0;
}


void
GameInfo::restore(GameInfo& oldInfo, Namebases& namebases)
{
	*this = oldInfo;

	if (m_event->frequency() == 0)
		namebases(Namebase::Site).incrRef(m_event->site());

	namebases(Namebase::Player).incrRef(m_player[White]);
	namebases(Namebase::Player).incrRef(m_player[Black]);
	namebases(Namebase::Event ).incrRef(m_event);

	if (!hasGameRecordLength())
		namebases(Namebase::Annotator).incrRef(m_annotator);
}


void
GameInfo::reallocate(Namebases& namebases) const
{
	static unsigned const Limit = mstl::numeric_limits<unsigned>::max();

	NamebaseSite* site;

	{
		Namebase& namebase = namebases(Namebase::Player);
		NamebasePlayer* entry = m_player[White];
		mstl::string name(entry->name());
		char* s = namebase.alloc(name.size());
		::memcpy(s, name, name.size());
		name.hook(s, name.size());

		namebase.insertPlayer(
			name, entry->federation(), entry->title(), entry->type(), entry->sex(), entry->fideID(), Limit);
	}
	{
		Namebase& namebase = namebases(Namebase::Player);
		NamebasePlayer* entry = m_player[Black];
		mstl::string name(entry->name());
		char* s = namebase.alloc(name.size());
		::memcpy(s, name, name.size());
		name.hook(s, name.size());

		namebase.insertPlayer(
			name, entry->federation(), entry->title(), entry->type(), entry->sex(), entry->fideID(), Limit);
	}
	{
		Namebase& namebase = namebases(Namebase::Site);
		NamebaseSite* entry = m_event->site();
		mstl::string name(entry->name());
		char* s = namebase.alloc(name.size());
		::memcpy(s, name, name.size());
		name.hook(s, name.size());

		site = namebase.insertSite(name, entry->country(), Limit);
	}
	{
		Namebase& namebase = namebases(Namebase::Event);
		NamebaseEvent* entry = m_event;
		mstl::string name(entry->name());
		char* s = namebase.alloc(name.size());
		::memcpy(s, name, name.size());
		name.hook(s, name.size());

		namebase.insertEvent(
			name, entry->date(), entry->type(), entry->timeMode(), entry->eventMode(), Limit, site);
	}
	if (!hasGameRecordLength())
	{
		Namebase& namebase = namebases(Namebase::Annotator);
		NamebaseEntry* entry = m_annotator;
		mstl::string name(entry->name());
		char* s = namebase.alloc(name.size());
		::memcpy(s, name, name.size());
		name.hook(s, name.size());

		namebase.insert(name, Limit);
	}
}


void
GameInfo::setupVariant(TagSet& tags, variant::Type variant, uint16_t idn)
{
	M_ASSERT(idn > 0);

	if (idn != variant::Standard)
	{
		tags.set(tag::SetUp, 1);

		if (variant::isChess960(idn))
		{
			tags.set(tag::Fen, chess960::fen(idn));
			tags.add(tag::Opening, chess960::position(idn));

//			Not required, because the FEN is decisive.
//			Furthermore it's a conflict in case of a chess variant.
//			tags.set(tag::Variant, chess960::identifier());
		}
		else if (variant::isShuffleChess(idn))
		{
			tags.set(tag::Fen, shuffle::fen(idn));
			tags.add(tag::Opening, shuffle::position(idn));

//			Not required, because the FEN is decisive.
//			Furthermore it's a conflict in case of a chess variant.
//			tags.set(tag::Variant, shuffle::identifier());
		}
		else
		{
			tags.set(tag::Fen, variant::fen(idn));
		}
	}

	if (variant != variant::Normal)
		tags.set(tag::Variant, variant::identifier(variant));
}


void
GameInfo::setupTags(TagSet& tags, variant::Type variant) const
{
	M_ASSERT(variant::isMainVariant(variant));

	tags.set(tag::Event, m_event->name());
	tags.set(tag::Site, m_event->site()->name());

	if (m_dateYear == Date::Zero10Bits)
	{
		tags.set(tag::Date, "????.??.??", 10);
	}
	else
	{
		tags.set(tag::Date,
					Date(Date::decodeYearFrom10Bits(m_dateYear), m_dateMonth, m_dateDay).asString());
	}

	if (m_round)
		tags.set(tag::Round, roundAsString());
	else
		tags.set(tag::Round, "?", 1);

	tags.set(tag::White, m_player[White]->name());
	tags.set(tag::Black, m_player[Black]->name());
	tags.set(tag::Result, result::toString(result::ID(m_result)));

	if (!hasGameRecordLength())
		tags.set(tag::Annotator, m_annotator->name());

	if (m_event->type() != event::Unknown)
		tags.set(tag::EventType, event::toString(m_event->type()));
	if (m_event->country() != country::Unknown)
		tags.set(tag::EventCountry, country::toString(m_event->country()));
	if (m_player[White]->federation() != country::Unknown)
		tags.set(tag::WhiteCountry, country::toString(country::Code(m_player[White]->federation())));
	if (m_player[Black]->federation() != country::Unknown)
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
	if (m_player[White]->haveFideId())
		tags.set(tag::WhiteFideId, m_player[White]->fideID());
	if (m_player[Black]->haveFideId())
		tags.set(tag::BlackFideId, m_player[Black]->fideID());

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

	tags.set(tag::Termination, termination::toString(termination::Reason(m_termination)));
	tags.set(tag::Mode, event::toString(m_event->eventMode()));
	tags.set(tag::TimeMode, time::toString(m_event->timeMode()));
	tags.set(tag::Idn, m_positionId);

	if (m_positionId)
	{
		setupVariant(
			tags,
			variant == variant::Antichess ? (isGiveaway() ? variant::Giveaway : variant::Suicide) : variant,
			m_positionId);

		if (m_positionId == variant::Standard)
		{
			tags.set(tag::Eco, Eco::fromShort(m_eco).asShortString());
			Eco eco = m_eco ? Eco::fromShort(m_eco) : Eco(m_ecoKey);
			addOpening(tags, variant, eco, eco == Eco(m_ecoKey));
		}
	}

#ifdef GAME_INFO_PLYCOUNT
	// IMPORTANT NOTE:
	// The ply count may be slightly incorrect if the source is .cbh or .cbf!
	// The tag value will be corrected afterwards (after loading).
	tags.set(tag::PlyCount, m_plyCount);
#endif
}


void
GameInfo::setupTags(TagSet& tags, Provider const& provider)
{
	variant::Type variant = provider.variant();
	unsigned idn = provider.getStartBoard().computeIdn(variant);

	tags.set(tag::Idn, idn);

	if (idn)
	{
		setupVariant(tags, variant, idn);

		if (idn == variant::Standard)
		{
			EcoTable const& ecoTable = EcoTable::specimen(variant);
			Eco eco = ecoTable.getEco(provider.openingLine());
			tags.add(tag::Eco, eco.asShortString());
			addOpening(tags, variant, eco, true);
		}
	}

#ifdef GAME_INFO_PLYCOUNT
	tags.set(tag::PlyCount, provider.plyCount());
#endif
}


void
GameInfo::addOpening(TagSet& tags, variant::Type variant, Eco const& eco, bool inclusiveVar)
{
	if (	eco
		&& !tags.isUserSupplied(tag::Opening)
		&& !tags.isUserSupplied(tag::Variation)
		&& !tags.isUserSupplied(tag::SubVariation))
	{
		if (g_latin1Codec == 0)
			g_latin1Codec = new sys::utf8::Codec(sys::utf8::Codec::latin1());

		EcoTable::Opening const& opening = EcoTable::specimen(variant).getOpening(eco);
		mstl::string mainvar(opening.part[0]);
		g_latin1Codec->toUtf8(mainvar);
		tags.add(tag::Opening, mainvar);

#ifdef GAME_INFO_VAR
		if (inclusiveVar)
		{
			static_assert(EcoTable::Num_Name_Parts >= 4, "index failure");
			mstl::string var(opening.part[2]);
			mstl::string subvar(opening.part[3]);
			g_latin1Codec->toUtf8(var);
			g_latin1Codec->toUtf8(subvar);
			tags.add(tag::Variation, var);
			tags.add(tag::SubVariation, subvar);
		}
#endif
	}
}


void
GameInfo::setRecord(uint32_t offset, uint32_t length)
{
	m_gameOffset = offset;

	if (hasGameRecordLength())
		setGameRecordLength(length);
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
	m_signature.m_matSig.part[White].queen   = (1 << sig.wq) - 1;
	m_signature.m_matSig.part[White].rook    = (1 << sig.wr) - 1;
	m_signature.m_matSig.part[White].bishop  = (1 << sig.wb) - 1;
	m_signature.m_matSig.part[White].knight  = (1 << sig.wn) - 1;
	m_signature.m_matSig.part[White].pawn    = (1 << sig.wp) - 1;

	m_signature.m_matSig.part[Black].queen   = (1 << sig.bq) - 1;
	m_signature.m_matSig.part[Black].rook    = (1 << sig.br) - 1;
	m_signature.m_matSig.part[Black].bishop  = (1 << sig.bb) - 1;
	m_signature.m_matSig.part[Black].knight  = (1 << sig.bn) - 1;
	m_signature.m_matSig.part[Black].pawn    = (1 << sig.bp) - 1;

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


util::crc::checksum_t
GameInfo::computeChecksum(util::crc::checksum_t crc) const
{
	crc = ::computeCRC(crc, m_event->name());
	crc = ::computeCRC(crc, m_event->site()->name());
	crc = ::computeCRC(crc, m_player[White]->name());
	crc = ::computeCRC(crc, m_player[Black]->name());

	if (!hasGameRecordLength())
		crc = ::computeCRC(crc, m_annotator->name());

	crc = ::util::crc::compute(crc, uint32_t(m_pd[White].ratingValue));
	crc = ::util::crc::compute(crc, uint32_t(m_pd[Black].ratingValue));
	crc = ::util::crc::compute(crc, uint32_t(date().hash()));
	crc = ::util::crc::compute(crc, uint32_t(eventDate().hash()));

	crc = ::util::crc::compute(crc, uint16_t(m_event->site()->country()));
	crc = ::util::crc::compute(crc, uint16_t(m_event->site()->country()));
	crc = ::util::crc::compute(crc, uint16_t(m_player[White]->federation()));
	crc = ::util::crc::compute(crc, uint16_t(m_player[Black]->federation()));
	crc = ::util::crc::compute(crc, uint16_t(m_eco));

	crc = ::util::crc::compute(crc, uint8_t(m_player[White]->title()));
	crc = ::util::crc::compute(crc, uint8_t(m_player[Black]->title()));
	crc = ::util::crc::compute(crc, uint8_t(m_player[White]->type()));
	crc = ::util::crc::compute(crc, uint8_t(m_player[Black]->type()));
	crc = ::util::crc::compute(crc, uint8_t(m_player[White]->sex()));
	crc = ::util::crc::compute(crc, uint8_t(m_player[Black]->sex()));
	crc = ::util::crc::compute(crc, uint8_t(eventType()));
	crc = ::util::crc::compute(crc, uint8_t(timeMode()));
	crc = ::util::crc::compute(crc, uint8_t(eventMode()));
	crc = ::util::crc::compute(crc, uint8_t(m_termination));
	crc = ::util::crc::compute(crc, uint8_t(m_result));

// belongs to move data
//	crc = ::computeCRC(crc, m_ecoKey);
//	crc = ::computeCRC(crc, m_signature);
//	crc = ::util::crc::compute(crc, uint16_t(m_positionId));
//	crc = ::util::crc::compute(crc, uint16_t(m_setup));
//	crc = ::util::crc::compute(crc, uint16_t(m_plyCount));
//	crc = ::util::crc::compute(crc, uint8_t(m_annotationCount));
//	crc = ::util::crc::compute(crc, uint8_t(m_commentCount));
//	crc = ::util::crc::compute(crc, uint8_t(m_variationCount));

// should not be included
//	crc = ::computeCRC(crc, uint32_t(m_gameFlags));

	return crc;
}


char
GameInfo::mapFlag(uint32_t flag)
{
	M_REQUIRE(flag <= Flag_Illegal_Move);
	M_ASSERT(mstl::bf::lsb_index(flag) <= U_NUMBER_OF(::GameFlagMap));

	return ::GameFlagMap[mstl::bf::lsb_index(flag)];
}


mstl::string&
GameInfo::flagsToString(uint32_t flags, mstl::string& result)
{
	if (flags & Flag_Illegal_Move)
		result += mapFlag(Flag_Illegal_Move);
	else if (flags & Flag_Illegal_Castling)
		result += mapFlag(Flag_Illegal_Castling);

	for (unsigned i = 0; i < U_NUMBER_OF(::GameFlagMap); ++i)
	{
		unsigned flag = (1u << i) & ~(Flag_Illegal_Castling | Flag_Illegal_Move);

		if (flags & flag)
			result += ::GameFlagMap[i];
	}

	return result;
}


unsigned
GameInfo::charToFlag(char c)
{
	for (unsigned i = 0; i < U_NUMBER_OF(::GameFlagMap); ++i)
	{
		if (c == ::GameFlagMap[i])
			return (1 << i);
	}

	return 0;
}


unsigned
GameInfo::stringToFlags(char const* str)
{
	unsigned result = 0;

	for ( ; *str; ++str)
	{
		if (!::isspace(*str))
			result |= charToFlag(*str);
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
	::printf(   "Eco:              %s\n", eco(variant::Normal).asString().c_str());
	if (idn() == variant::Standard)
	{
		::printf("Eco Key:          %s\n", Eco(m_ecoKey).asString().c_str());
	}
	else if (idn())
	{
		::printf("Ply 1:            %s-%s\n",
					sq::printAlgebraic(Move(m_ply[0]).from()), sq::printAlgebraic(Move(m_ply[0]).to()));
		::printf("Ply 2:            %s-%s\n",
					sq::printAlgebraic(Move(m_ply[1]).from()), sq::printAlgebraic(Move(m_ply[1]).to()));
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
