// ======================================================================
// Author : $Author$
// Version: $Revision: 5 $
// Date   : $Date: 2011-05-05 07:51:24 +0000 (Thu, 05 May 2011) $
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

#include "sci_codec.h"
#include "sci_decoder.h"
#include "sci_encoder.h"
#include "sci_consumer.h"
#include "sci_common.h"

#include "db_game_info.h"
#include "db_consumer.h"
#include "db_producer.h"
#include "db_game_data.h"
#include "db_exception.h"

#include "u_byte_stream.h"
#include "u_block_file.h"
#include "u_progress.h"
#include "u_crc.h"

#include "m_string.h"
#include "m_fstream.h"
#include "m_assert.h"
#include "m_static_check.h"

#include "sys_utf8_codec.h"

#include <string.h>

//#define USE_LZO

#ifdef USE_LZO
# include "u_lzo_byte_stream.h"
#endif

using namespace db;
using namespace db::sci;
using namespace util;


typedef ByteStream::uint24_t uint24_t;
typedef ByteStream::uint48_t uint48_t;

namespace
{
	enum { IndexEntrySize = 61 };	// = sizeof(pointers) + sizeof(IndexBits) = 35 + 27

	struct IndexBits
	{
		uint32_t flags					:23;
		uint32_t castling				:4;
		uint32_t termination			:4;
		uint32_t underPromotion		:1;
		//----------------------------- 32 bit
		uint32_t ecoKey				:20;
		uint32_t plyCount				:12;
		//----------------------------- 32 bit
		uint32_t ecoOpening			:20;
		uint32_t whiteRating			:12;
		//----------------------------- 32 bit
		uint32_t blackElo				:12;
		uint32_t blackRating			:12;
		uint32_t dateMonth			:4;
		uint32_t annotationCount	:4;
		//----------------------------- 32 bit
		uint32_t whiteElo				:12;
		uint32_t dateDay				:5;
		uint32_t variationCount		:4;
		uint32_t commentCount		:4;
		uint32_t hpCount				:4;
		uint32_t result				:3;
		//----------------------------- 32 bit
		uint32_t positionId			:12;
		uint32_t dateYear				:10;
		uint32_t eco					:9;
		uint32_t promotion			:1;
		//----------------------------- 32 bit
		uint32_t round					:8;
		uint32_t subround				:8;
		uint32_t whiteRatingType	:3;
		uint32_t blackRatingType	:3;
		uint32_t _unused_				:2;
		//----------------------------- 24 bit
	};
}


static mstl::string const MagicIndexFile("Scidb.i\0", 8);
static mstl::string const MagicGameFile ("Scidb.g\0", 8);
static mstl::string const MagicNamebase ("Scidb.n\0", 8);
static mstl::string const Extension("sci");

static uint16_t const FileVersion = 99;

static char const* NamebaseTags[Namebase::Round];

namespace {

struct Init { Init(); };
Init m_init;

Init::Init()
{
	NamebaseTags[Namebase::Player		] = "player\0";
	NamebaseTags[Namebase::Site		] = "site\0\0\0";
	NamebaseTags[Namebase::Event		] = "event\0\0";
	NamebaseTags[Namebase::Annotator	] = "annota\0";
}

} // namespace


inline static NamebaseSite*
getSite(Namebase& base, unsigned index)
{
	if (index >= base.size())
		IO_RAISE(Index, Corrupted, "corrupted namebase index %u", index);

	return base.siteAt(index);
}


inline static NamebaseEvent*
getEvent(Namebase& base, unsigned index)
{
	if (index >= base.size())
		IO_RAISE(Index, Corrupted, "corrupted namebase index %u", index);

	return base.eventAt(index);
}


inline static NamebasePlayer*
getPlayer(Namebase& base, unsigned index)
{
	if (index >= base.size())
		IO_RAISE(Index, Corrupted, "corrupted namebase index %u", index);

	// XXX we need player with id()==index
	return base.playerAt(index);
}


inline static NamebaseEntry*
getRound(Namebase& base, unsigned index)
{
	if (index >= base.size())
		IO_RAISE(Index, Corrupted, "corrupted namebase index %u", index);

	return base.entryAt(index);
}


inline static NamebaseEntry*
getAnnotator(Namebase& base, unsigned index)
{
	if (index >= base.size())
		IO_RAISE(Index, Corrupted, "corrupted namebase index %u", index);

	return base.entryAt(index);
}


static unsigned
prefix(char const* s, char const* t)
{
	unsigned count = 0;

	for ( ; *s && *s == *t; ++s, ++t, ++count)
		;

	return count;
}

#ifndef USE_LZO

namespace {

struct ByteIStream : public ByteStream
{
	ByteIStream(mstl::fstream& strm);
	void underflow(unsigned size);
	mstl::istream& m_strm;
};


ByteIStream::ByteIStream(mstl::fstream& strm)
	:ByteStream(strm.bufsize())
	,m_strm(strm)
{
	skip(strm.bufsize());	// force underflow()
}


void
ByteIStream::underflow(unsigned size)
{
	M_ASSERT(size <= capacity());

	unsigned remaining = this->remaining();
	::memmove(m_base, m_getp, remaining);
	m_getp = m_base + remaining;
	m_endp = m_getp + m_strm.readsome(reinterpret_cast<char*>(m_getp), capacity() - remaining);
	m_getp -= remaining;

	if (__builtin_expect(m_getp >= m_endp, 0))
		IO_RAISE(Namebase, Corrupted, "unexpected end of stream");
}


struct ByteOStream : public ByteStream
{
	ByteOStream(mstl::ostream& strm, unsigned char* buf, unsigned size);
	void overflow(unsigned size);
	void flush();
	mstl::ostream& m_strm;
};


ByteOStream::ByteOStream(mstl::ostream& strm, unsigned char* buf, unsigned size)
	:ByteStream(buf, size)
	,m_strm(strm)
{
}


void
ByteOStream::overflow(unsigned size)
{
	if (__builtin_expect(!m_strm.write(m_base, m_putp - m_base), 0))
		IO_RAISE(Namebase, Write_Failed, "write failed");

	m_putp = m_base;
}


void
ByteOStream::flush()
{
	if (__builtin_expect(!m_strm.write(m_base, m_putp - m_base), 0))
		IO_RAISE(Namebase, Write_Failed, "write failed");
}

} // namespace

#endif // !USE_LZO


unsigned Codec::maxGameRecordLength() const	{ return (1 << 20) - 1; }
unsigned Codec::maxGameLength() const			{ return (1 << 12) - 1; }
unsigned Codec::maxGameCount() const			{ return (1 << 24) - 1; }
unsigned Codec::maxPlayerCount() const			{ return (1 << 24) - 1; }
unsigned Codec::maxSiteCount() const			{ return (1 << 24) - 1; }
unsigned Codec::maxEventCount() const			{ return (1 << 24) - 1; }
unsigned Codec::maxAnnotatorCount() const		{ return (1 << 24) - 1; }
unsigned Codec::minYear() const					{ return Date::MinYear; }
unsigned Codec::maxYear() const					{ return Date::MaxYear; }
unsigned Codec::maxDescriptionLength() const	{ return 111; }
mstl::string const& Codec::extension() const	{ return Extension; }
mstl::string const& Codec::encoding() const	{ return sys::utf8::Codec::utf8(); }
bool Codec::encodingFailed() const				{ return false; }
void Codec::reset()									{}


unsigned
Codec::gameFlags() const
{
	return	GameInfo::Flag_White_Opening
			 | GameInfo::Flag_Black_Opening
			 | GameInfo::Flag_Middle_Game
			 | GameInfo::Flag_End_Game
			 | GameInfo::Flag_Novelty
			 | GameInfo::Flag_Pawn_Structure
			 | GameInfo::Flag_Tactics
			 | GameInfo::Flag_King_Side
			 | GameInfo::Flag_Queen_Side
			 | GameInfo::Flag_Brilliancy
			 | GameInfo::Flag_Blunder
			 | GameInfo::Flag_User
			 | GameInfo::Flag_Best_Game
			 | GameInfo::Flag_Decided_Tournament
			 | GameInfo::Flag_Model_Game
			 | GameInfo::Flag_Strategy
			 | GameInfo::Flag_With_Attack
			 | GameInfo::Flag_Sacrifice
			 | GameInfo::Flag_Defense
			 | GameInfo::Flag_Material
			 | GameInfo::Flag_Piece_Play;
}


Codec::Codec()
	:m_gameData(0)
	,m_asyncReader(0)
{
	M_STATIC_CHECK(U_NUMBER_OF(m_lookup) <= Namebase::Round, Index_Out_Of_Range);

	m_magicGameFile = MagicGameFile;
	m_magicGameFile.resize(MagicGameFile.size() + 2);

	ByteStream strm(m_magicGameFile.data(), m_magicGameFile.size());
	strm.advance(MagicGameFile.size());
	strm << uint16_t(FileVersion);
}


Codec::~Codec() throw()
{
	if (m_asyncReader)
		m_gameData->closeAsyncReader(m_asyncReader);

	delete m_gameData;
}


Codec::Format
Codec::format() const
{
	return format::Scidb;
}


void
Codec::setEncoding(mstl::string const& encoding)
{
	M_REQUIRE(encoding == sys::utf8::Codec::utf8());
}


BlockFile*
Codec::newBlockFile() const
{
	return new BlockFile(Block_Size, BlockFile::ReadWriteLength);
}


void
Codec::filterTag(TagSet& tags, tag::ID tag) const
{
	if (!Encoder::skipTag(tag))
		tags.remove(tag);
}


void
Codec::recode(	GameInfo const& info,
					util::ByteStream& dst,
					sys::utf8::Codec& oldCodec,
					sys::utf8::Codec& newCodec)
{
	M_ASSERT(m_gameData);

	ByteStream src;
	getGameRecord(info, m_gameData->reader(), src);
	Decoder decoder(src, m_gameData->blockSize() - info.gameOffset());
	decoder.recode(dst, oldCodec, newCodec);
}


void
Codec::replaceBlockFile(util::BlockFile* blockFile)
{
	M_ASSERT(blockFile);
	M_ASSERT(!m_gameStream.is_open());

	if (m_gameData)
	{
		m_gameData->close();
		delete m_gameData;
	}

	m_gameData = blockFile;
}


void
Codec::close()
{
	m_gameData->close();
}


util::ByteStream
Codec::getGame(GameInfo const& info)
{
	M_ASSERT(m_gameData);

	ByteStream src;
	getGameRecord(info, m_gameData->reader(), src);
	return src;
}


unsigned
Codec::putGame(ByteStream const& strm)
{
	M_ASSERT(m_gameData);
	return m_gameData->put(strm);
}


void
Codec::save(mstl::string const& rootname, unsigned start, util::Progress& progress, bool attach)
{
	if (!(m_gameStream.mode() & mstl::ios_base::out))
		IO_RAISE(Game, Read_Only, "game file '%s' is read-only", (rootname + ".scg").c_str());

	mstl::string indexFilename(rootname + ".sci");
	if (!attach)
		checkPermissions(indexFilename);

	if (isReadOnly())
		IO_RAISE(Index, Read_Only, "index file '%s' is read-only", indexFilename.c_str());

	mstl::string namebaseFilename(rootname + ".scn");
	if (!attach)
		checkPermissions(namebaseFilename);

	if (isReadOnly())
		IO_RAISE(Index, Read_Only, "name-base file '%s' is read-only", namebaseFilename.c_str());

	mstl::fstream indexStream;
	mstl::fstream namebaseStream;

	m_gameData->sync();
	openFile(indexStream, indexFilename, MagicIndexFile, attach ? Truncate : 0);
	openFile(namebaseStream, namebaseFilename, MagicNamebase, attach ? Truncate : 0);
	writeNamebase(namebaseStream);
	writeIndex(indexStream, start, progress);
}


void
Codec::save(mstl::string const& rootname, unsigned start, util::Progress& progress)
{
	save(rootname, start, progress, false);
}


void
Codec::attach(mstl::string const& rootname, util::Progress& progress)
{
	static mstl::ios_base::openmode mode =
		mstl::ios_base::in | mstl::ios_base::out | mstl::ios_base::trunc | mstl::ios_base::binary;

	mstl::string gameFilename(rootname + ".scg");
	m_gameStream.set_unbuffered();
	m_gameStream.open(gameFilename, mode);
	m_gameData->attach(&m_gameStream);
	save(rootname, 0, progress, true);
}


void
Codec::update(mstl::string const& rootname)
{
	if (!(m_gameStream.mode() & mstl::ios_base::out))
		IO_RAISE(Game, Read_Only, "game file '%s' is read-only", (rootname + ".scg").c_str());

	mstl::string indexFilename(rootname + ".sci");
	checkPermissions(indexFilename);

	if (isReadOnly())
		IO_RAISE(Index, Read_Only, "index file '%s' is read-only", indexFilename.c_str());

	mstl::string namebaseFilename(rootname + ".scn");
	checkPermissions(namebaseFilename);

	if (isReadOnly())
		IO_RAISE(Namebase, Read_Only, "name-base file '%s' is read-only", namebaseFilename.c_str());

	mstl::fstream indexStream;
	mstl::fstream namebaseStream;

	m_gameData->sync();
	indexStream.open(indexFilename, mstl::ios_base::in | mstl::ios_base::out | mstl::ios_base::binary);
	openFile(namebaseStream, namebaseFilename, MagicNamebase);
	writeNamebase(namebaseStream);
	updateIndex(indexStream);
}


void
Codec::update(mstl::string const& rootname, unsigned index, bool updateNamebase)
{
	if (!(m_gameStream.mode() & mstl::ios_base::out))
		IO_RAISE(Game, Read_Only, "game file '%s' is read-only", (rootname + ".scg").c_str());

	mstl::string indexFilename(rootname + ".sci");
	checkPermissions(indexFilename);

	if (isReadOnly())
		IO_RAISE(Index, Read_Only, "index file '%s' is read-only", indexFilename.c_str());

	mstl::fstream indexStream;

	m_gameData->sync();
	indexStream.open(indexFilename, mstl::ios_base::in | mstl::ios_base::out | mstl::ios_base::binary);

	if (updateNamebase)
	{
		mstl::fstream namebaseStream;
		mstl::string namebaseFilename(rootname + ".scn");

		checkPermissions(namebaseFilename);
		openFile(namebaseStream, namebaseFilename, MagicNamebase);
		writeNamebase(namebaseStream);
	}

	GameInfo* info = gameInfoList()[index];

	unsigned char buf[IndexEntrySize];

	ByteStream bstrm(buf, IndexEntrySize);
	encodeIndex(*info, bstrm);

	if (!indexStream.seekp(index*IndexEntrySize + 128) || !indexStream.write(buf, IndexEntrySize))
		IO_RAISE(Index, Corrupted, "unexpected end of index file");

	info->setDirty(false);
}


void
Codec::updateHeader(mstl::string const& rootname)
{
	mstl::string indexFilename(rootname + ".sci");
	checkPermissions(indexFilename);

	if (isReadOnly())
		IO_RAISE(Index, Read_Only, "index file '%s' is read-only", indexFilename.c_str());

	mstl::fstream indexStream;
	indexStream.open(indexFilename, mstl::ios_base::in | mstl::ios_base::out | mstl::ios_base::binary);
	writeIndexHeader(indexStream);
}


void
Codec::doEncoding(util::ByteStream& strm, GameData const& data, Signature const& signature)
{
	M_ASSERT(gameInfoList().size() <= maxGameCount());
	M_ASSERT(namebase(Namebase::Player).size() <= maxPlayerCount());
	M_ASSERT(namebase(Namebase::Site).size() <= maxSiteCount());
	M_ASSERT(namebase(Namebase::Event).size() <= maxEventCount());

	Encoder encoder(strm);
	encoder.doEncoding(signature, data);
}


db::Consumer*
Codec::getConsumer(format::Type srcFormat)
{
	return new Consumer(srcFormat, *this);
}


save::State
Codec::doDecoding(db::Consumer& consumer, unsigned flags, TagSet& tags, GameInfo const& info)
{
	ByteStream strm;
	getGameRecord(info, m_gameData->reader(), strm);
	Decoder decoder(strm, m_gameData->blockSize() - info.gameOffset());
	return decoder.doDecoding(consumer, flags, tags);
}


save::State
Codec::doDecoding(db::Consumer& consumer, ByteStream& strm, unsigned flags, TagSet& tags)
{
	Decoder decoder(strm);
	return decoder.doDecoding(consumer, flags, tags);
}


void
Codec::doDecoding(unsigned flags, GameData& data, GameInfo& info)
{
	ByteStream strm;
	getGameRecord(info, m_gameData->reader(), strm);
	data.m_crc = crc::compute(0, strm.data(), strm.size());
	Decoder decoder(strm, m_gameData->blockSize() - info.gameOffset());
	decoder.doDecoding(flags, data);
}


void
Codec::doOpen(mstl::string const& encoding)
{
	M_REQUIRE(encoding == sys::utf8::Codec::utf8());

	if (m_gameData)
	{
		m_gameData->close();
		delete m_gameData;
	}

	if (m_gameStream.is_open())
		m_gameData = new BlockFile(&m_gameStream, Block_Size, BlockFile::ReadWriteLength, m_magicGameFile);
	else
		m_gameData = new BlockFile(Block_Size, BlockFile::ReadWriteLength, m_magicGameFile);
}


void
Codec::doOpen(mstl::string const& rootname, mstl::string const& encoding, Progress& progress)
{
	M_REQUIRE(encoding == sys::utf8::Codec::utf8());
	M_ASSERT(m_gameData == 0);

	mstl::string indexFilename(rootname + ".sci");
	mstl::string gameFilename(rootname + ".scg");
	mstl::string namebaseFilename(rootname + ".scn");

	checkPermissions(indexFilename);
	mstl::fstream indexStream;
	openFile(indexStream, indexFilename, MagicIndexFile);

	uint16_t fileVersion = readIndexHeader(indexStream);

	mstl::fstream namebaseStream;

	try
	{
		checkPermissions(gameFilename);
		checkPermissions(namebaseFilename);

		m_gameStream.set_unbuffered();
		namebaseStream.set_unbuffered();

		openFile(m_gameStream, gameFilename, MagicGameFile);
		checkFileVersion(m_gameStream, MagicGameFile, fileVersion);
		openFile(namebaseStream, namebaseFilename, MagicNamebase);
		checkFileVersion(namebaseStream, MagicNamebase, fileVersion);

		readNamebase(namebaseStream, progress);
		decodeIndex(indexStream, progress);
		m_gameData = new BlockFile(&m_gameStream, Block_Size, BlockFile::ReadWriteLength, m_magicGameFile);
	}
	catch (...)
	{
		indexStream.close();
		namebaseStream.close();
		m_gameStream.close();
		throw;
	}

	namebaseStream.close();
	indexStream.close();
}


void
Codec::checkFileVersion(mstl::fstream& fstrm, mstl::string const& magic, uint16_t fileVersion)
{
	char buf[2];

	fstrm.seekg(magic.size(), mstl::ios_base::beg);
	fstrm.read(buf, 2);

	ByteStream bstrm(buf, 2);
	uint16_t version = bstrm.uint16();

	if (version != fileVersion)
	{
		if (&fstrm == m_gameStream)
			IO_RAISE(Game, Unexpected_Version, "unexpected version (%u)", unsigned(version));
		else
			IO_RAISE(Namebase, Unexpected_Version, "unexpected version (%u)", unsigned(version));
	}
}


void
Codec::doOpen(mstl::string const& rootname, mstl::string const& encoding)
{
	M_REQUIRE(encoding == sys::utf8::Codec::utf8());

	mstl::string indexFilename(rootname + ".sci");
	mstl::string gameFilename(rootname + ".scg");
	mstl::string namebaseFilename(rootname + ".scn");

	mstl::fstream indexStream;
	mstl::fstream namebaseStream;

	m_gameStream.set_unbuffered();

	openFile(m_gameStream, gameFilename, Truncate);
	openFile(indexStream, indexFilename, MagicIndexFile, Truncate);
	openFile(namebaseStream, namebaseFilename, MagicNamebase, Truncate);
	writeNamebase(namebaseStream);
	writeIndexHeader(indexStream);
	doOpen(encoding);
}


void
Codec::doClear(mstl::string const& rootname)
{
	mstl::string indexFilename(rootname + ".sci");
	mstl::string gameFilename(rootname + ".scg");
	mstl::string namebaseFilename(rootname + ".scn");

	mstl::fstream indexStream;
	mstl::fstream namebaseStream;

	openFile(m_gameStream, gameFilename, Truncate);
	openFile(indexStream, indexFilename, MagicIndexFile, Truncate);
	openFile(namebaseStream, namebaseFilename, MagicNamebase, Truncate);
	writeNamebase(namebaseStream);
	writeIndexHeader(indexStream);

	doOpen(sys::utf8::Codec::utf8());
	m_gameData->attach(&m_gameStream);
}


uint16_t
Codec::readIndexHeader(mstl::fstream& fstrm)
{
	char header[120];

	if (!fstrm.read(header, sizeof(header)))
		IO_RAISE(Index, Corrupted, "unexpected end of index file");

	ByteStream bstrm(header, sizeof(header));

	unsigned version	= bstrm.uint16();
	unsigned numGames	= bstrm.uint24();
	unsigned baseType	= bstrm.uint8();

	if (version != ::FileVersion)
	{
		if (version < FileVersion)
			IO_RAISE(Index, Unexpected_Version, "old format scidb version (%d)", unsigned(version));
		else
			IO_RAISE(Index, Unknown_Version, "unknown Scidb version (%u)", unsigned(version));
	}

	bstrm.skip(2);	// skip unused bytes

	mstl::string description;
	bstrm.get(description);
	setDescription(description);

	setType(type::ID(baseType));

	GameInfoList& infoList = gameInfoList();

	infoList.resize(numGames);
	for (unsigned i = 0; i < numGames; ++i)
		infoList[i] = allocGameInfo();

	// go to first index
	if (!fstrm.seekg(sizeof(header) + 8, mstl::ios_base::beg))
		IO_RAISE(Index, Corrupted, "seek failed");

	return version;
}


void
Codec::decodeIndex(mstl::fstream &fstrm, Progress& progress)
{
	GameInfoList& infoList = gameInfoList();

	unsigned frequency	= progress.frequency(infoList.size(), 20000);
	unsigned reportAfter	= frequency;

	ProgressWatcher watcher(progress, infoList.size());

	for (unsigned i = 0; i < infoList.size(); ++i)
	{
		if (reportAfter == i)
		{
			progress.update(i);
			reportAfter += frequency;
		}

		char buf[IndexEntrySize];

		if (__builtin_expect(!fstrm.read(buf, IndexEntrySize), 0))
			IO_RAISE(Index, Corrupted, "unexpected end of index file");

		ByteStream bstrm(buf, IndexEntrySize);
		decodeIndex(bstrm, *infoList[i]);
	}
}


void
Codec::decodeIndex(ByteStream& strm, GameInfo& item)
{
#define GET(type) ::get##type(namebase(Namebase::type), m_lookup[Namebase::type][strm.uint24()])

	item.m_gameOffset = strm.uint32();

	NamebasePlayer* whitePlayer = GET(Player);
	NamebasePlayer* blackPlayer = GET(Player);

	whitePlayer->ref(); blackPlayer->ref();

	{
		NamebaseEvent* event			= GET(Event);
		NamebaseEntry* annotator	= GET(Annotator);

		event->ref(); annotator->ref();

		item.m_player[color::White]	= whitePlayer;
		item.m_player[color::Black]	= blackPlayer;
		item.m_event						= event;
		item.m_annotator					= annotator;
	}

#undef GET

	item.m_signature.m_homePawns.value = strm.uint64();
	item.m_signature.m_progress.side[color::White].rankValue = strm.uint16();
	item.m_signature.m_progress.side[color::Black].rankValue = strm.uint16();
	item.setMaterial(strm.uint24());

//	M_ASSERT(IndexEntrySize - sizeof(struct IndexBits) == strm.tellg());
//	M_ASSERT(sizeof(IndexBits) == strm.remaining());

	IndexBits const* bits = reinterpret_cast<IndexBits const*>(strm.data());

	whitePlayer->setElo(bits->whiteElo);
	blackPlayer->setElo(bits->blackElo);

	whitePlayer->setRating(rating::Type(bits->whiteRatingType), bits->whiteRating);
	blackPlayer->setRating(rating::Type(bits->blackRatingType), bits->blackRating);

	M_STATIC_CHECK(Eco::Bit_Size_Per_Subcode == 20, ReimplementationNeeded);

	item.m_round								= bits->round;
	item.m_subround							= bits->subround;
	item.m_eco									= bits->eco;
	item.m_ecoKey								= bits->ecoKey;
	item.m_ecoOpening							= bits->ecoOpening;
	item.m_pd[color::White].elo			= bits->whiteElo;
	item.m_pd[color::Black].elo			= bits->blackElo;
	item.m_pd[color::White].rating		= bits->whiteRating;
	item.m_pd[color::Black].rating		= bits->blackRating;
	item.m_pd[color::White].ratingType	= bits->whiteRatingType;
	item.m_pd[color::Black].ratingType	= bits->blackRatingType;
	item.m_gameFlags							= bits->flags;
	item.m_result								= bits->result;
	item.m_plyCount							= bits->plyCount;
	item.m_variationCount					= bits->variationCount;
	item.m_commentCount						= bits->commentCount;
	item.m_annotationCount					= bits->annotationCount;
	item.m_positionId							= bits->positionId;
	item.m_termination						= bits->termination;
	item.m_dateYear							= bits->dateYear;
	item.m_dateMonth							= bits->dateMonth;
	item.m_dateDay								= bits->dateDay;
	item.m_signature.m_castling			= bits->castling;
	item.m_signature.m_promotions			= bits->promotion;
	item.m_signature.m_underPromotions	= bits->underPromotion;
	item.m_signature.m_hpCount				= bits->hpCount;
}


void
Codec::writeIndexHeader(mstl::fstream& fstrm)
{
	// write header (w/o magic)
	unsigned char header[120];
	ByteStream strm(header, sizeof(header));

	::memset(header, 0, sizeof(header));

	strm << uint16_t(FileVersion);				// Scidb version
	strm << uint24_t(gameInfoList().size());	// number of games
	strm << uint8_t(type());						// base type
	strm << uint16_t(0);								// unused

	strm.put(description(), mstl::min(description().size(), sizeof(header) - strm.tellp() - 1));

	if (!fstrm.seekp(8, mstl::ios_base::beg))	// skip magic
		IO_RAISE(Index, Corrupted, "unexpected end of index file");

	if (!fstrm.write(header, sizeof(header)))
		IO_RAISE(Index, Write_Failed, "unexpected end of index file");
}


void
Codec::writeIndex(mstl::fstream& fstrm, unsigned start, util::Progress& progress)
{
	writeIndexHeader(fstrm);

	if (start > 0 && !fstrm.seekp(start*::IndexEntrySize + 128, mstl::ios_base::beg))
		IO_RAISE(Index, Corrupted, "cannot seek to end of file");

	GameInfoList& infoList = gameInfoList();

	unsigned frequency	= progress.frequency(infoList.size(), 100000);
	unsigned reportAfter	= frequency + start;

	ProgressWatcher watcher(progress, infoList.size());

	for (unsigned i = start; i < infoList.size(); ++i)
	{
		if (reportAfter == i)
		{
			progress.update(i);
			reportAfter += frequency;
		}

		char buf[IndexEntrySize];

		ByteStream bstrm(buf, IndexEntrySize);
		encodeIndex(*infoList[i], bstrm);

		if (__builtin_expect(!fstrm.write(buf, IndexEntrySize), 0))
			IO_RAISE(Index, Write_Failed, "error while writing index entry");
	}
}


void
Codec::updateIndex(mstl::fstream& fstrm)
{
	// update header
	{
		if (!fstrm.seekp(10))
			IO_RAISE(Index, Corrupted, "unexpected end of index file");

		unsigned char buf[3];
		ByteStream strm(buf, sizeof(buf));
		strm << uint24_t(gameInfoList().size());	// number of games

		if (!fstrm.write(buf, sizeof(buf)))
			IO_RAISE(Index, Write_Failed, "error while writing index entry");
	}

	GameInfoList& infoList = gameInfoList();

	for (unsigned i = 0; i < infoList.size(); ++i)
	{
		if (infoList[i]->isDirty())
		{
			unsigned char buf[IndexEntrySize];

			ByteStream bstrm(buf, IndexEntrySize);
			encodeIndex(*infoList[i], bstrm);

			if (!fstrm.seekp(i*IndexEntrySize + 128))
				IO_RAISE(Index, Corrupted, "unexpected end of index file");
			if (!fstrm.write(buf, IndexEntrySize))
				IO_RAISE(Index, Write_Failed, "error while writing index entry");

			infoList[i]->setDirty(false);
		}
	}
}


void
Codec::encodeIndex(GameInfo const& item, ByteStream& strm)
{
	strm << uint32_t(item.m_gameOffset);
	strm << uint24_t(item.m_player[color::White]->id());
	strm << uint24_t(item.m_player[color::Black]->id());
	strm << uint24_t(item.m_event->id());
	strm << uint24_t(item.m_annotator->id());
	strm << uint64_t(item.m_signature.m_homePawns.value);
	strm << uint16_t(item.m_signature.m_progress.side[color::White].rankValue);
	strm << uint16_t(item.m_signature.m_progress.side[color::Black].rankValue);
	strm << uint24_t(item.material().value);

//	M_ASSERT(IndexEntrySize - sizeof(struct IndexBits) == strm.tellp());
//	M_ASSERT(sizeof(IndexBits) == strm.free());

	IndexBits* bits = reinterpret_cast<IndexBits*>(strm.buffer());

	M_STATIC_CHECK(Eco::Bit_Size_Per_Subcode == 20, ReimplementationNeeded);

	bits->round					= item.m_round;
	bits->subround				= item.m_subround;
	bits->eco					= item.m_eco;
	bits->ecoKey				= item.m_ecoKey;
	bits->ecoOpening			= item.m_ecoOpening;
	bits->flags					= item.m_gameFlags;
	bits->promotion			= item.m_signature.hasPromotion();
	bits->underPromotion		= item.m_signature.hasUnderPromotion();
	bits->castling				= item.m_signature.m_castling;
	bits->hpCount				= item.m_signature.m_hpCount;
	bits->plyCount				= item.m_plyCount;
	bits->positionId			= item.m_positionId;
	bits->variationCount		= item.m_variationCount;
	bits->commentCount		= item.m_commentCount;
	bits->annotationCount	= item.m_annotationCount;
	bits->dateDay				= item.m_dateDay;
	bits->dateMonth			= item.m_dateMonth;
	bits->dateYear				= item.m_dateYear;
	bits->result				= item.m_result;
	bits->termination			= item.m_termination;
	bits->whiteElo				= item.m_pd[color::White].elo;
	bits->blackElo				= item.m_pd[color::Black].elo;
	bits->whiteRating			= item.m_pd[color::White].rating;
	bits->blackRating			= item.m_pd[color::Black].rating;
	bits->whiteRatingType	= item.m_pd[color::White].ratingType;
	bits->blackRatingType	= item.m_pd[color::Black].ratingType;
}


void
Codec::readNamebase(mstl::fstream& stream, Progress& progress)
{
#ifdef USE_LZO
	LzoByteStream bstrm(stream);
#else
	stream.set_bufsize(65536);
	ByteIStream bstrm(stream);
#endif

	ProgressWatcher watcher(progress, U_NUMBER_OF(::NamebaseTags));

	for (unsigned i = 0; i < U_NUMBER_OF(::NamebaseTags); ++i)
	{
		Namebase::Type type;

		char tag[8];
		bstrm.get(tag, 8);

		switch (tag[0])
		{
			case 's':	type = Namebase::Site; break;
			case 'p':	type = Namebase::Player; break;
			case 'e':	type = Namebase::Event; break;
			case 'a':	type = Namebase::Annotator; break;
			default:		IO_RAISE(Namebase, Corrupted, "unexpected tag entry");
		}

		Namebase& base = namebase(type);

		unsigned size = bstrm.uint24();
		unsigned maxFreq = bstrm.uint24();
		unsigned maxUsage = bstrm.uint24();

		m_lookup[i].resize(size);

		switch (i)
		{
			case Namebase::Event:	readEventbase(bstrm, namebase(type), size); break;
			case Namebase::Site:		readSitebase(bstrm, namebase(type), size); break;
			case Namebase::Player:	readPlayerbase(bstrm, namebase(type), size); break;
			default:						readNamebase(bstrm, namebase(type), size); break;
		}

		base.setPrepared(maxFreq, maxUsage);
		progress.update(i + 1);
	}
}


void
Codec::readNamebase(ByteStream& bstrm, Namebase& base, unsigned count)
{
	if (count == 0)
		return;

	mstl::string name;

	base.reserve(count, 1 << 24);

	unsigned	index		= bstrm.uint24();
	unsigned	length	= bstrm.get();
	char*		prev		= base.alloc(length);
	Lookup&	lookup	= m_lookup[base.type()];

	bstrm.get(prev, length);
	name.hook(prev, length);
	base.append(name, index);
	lookup[index] = 0;

	for (unsigned i = 1; i < count; ++i)
	{
		unsigned index 	= bstrm.uint24();
		unsigned prefix	= bstrm.get();
		unsigned length	= bstrm.get();

		if (prefix >= length)
			IO_RAISE(Namebase, Corrupted, "namebase file is broken");

		char* curr = base.alloc(length);
		::memcpy(curr, prev, prefix);
		bstrm.get(curr + prefix, length - prefix);
		name.hook(curr, length);
		prev = curr;
		base.append(name, index);
		lookup[index] = i;
	}
}


void
Codec::readSitebase(ByteStream& bstrm, Namebase& base, unsigned count)
{
	if (count == 0)
		return;

	mstl::string name;

	base.reserve(count, 1 << 24);

	unsigned	index		= bstrm.uint24();
	unsigned	length	= bstrm.get();
	char*		prev		= base.alloc(length);
	Lookup&	lookup	= m_lookup[Namebase::Site];

	bstrm.get(prev, length);
	name.hook(prev, length);

	base.appendSite(name, index, country::Code(bstrm.uint16()));
	lookup[index] = 0;

	for (unsigned i = 1; i < count; ++i)
	{
		unsigned	index		= bstrm.uint24();
		unsigned prefix	= bstrm.get();
		unsigned length	= bstrm.get();

		if (prefix > length)
			IO_RAISE(Namebase, Corrupted, "namebase file is broken");

		if (prefix < length)
		{
			char* curr = base.alloc(length);
			::memcpy(curr, prev, prefix);
			bstrm.get(curr + prefix, length - prefix);
			name.hook(curr, length);
			prev = curr;
		}

		base.appendSite(name, index, country::Code(bstrm.uint16()));
		lookup[index] = i;
	}
}


void
Codec::readEventbase(ByteStream& bstrm, Namebase& base, unsigned count)
{
	if (count == 0)
		return;

	mstl::string name;

	base.reserve(count, 1 << 24);

	char*		prev		= 0;
	unsigned	index		= bstrm.uint24();
	unsigned	length	= bstrm.get();
	Lookup&	lookup	= m_lookup[Namebase::Event];

	prev = base.alloc(length);
	bstrm.get(prev, length);
	name.hook(prev, length);

	NamebaseSite* site = ::getSite(namebase(Namebase::Site), m_lookup[Namebase::Site][bstrm.uint24()]);

	site->ref();

	if (uint16_t flags = bstrm.uint16())
	{
		// flags (16 bit)
		// -------------------------------
		// 0000 0000 0000 1111  type
		// 0000 0000 0111 0000  event mode
		// 0000 0011 1000 0000  time mode
		// 0111 1100 0000 0000	date day
		// 1000 0000 0000 0000  extranouos data flag
		//
		// extraneous data (16 bit)
		// -------------------------------
		// 0000 0011 1111 1111  date year
		// 0011 1100 0000 0000  date month
		// 1100 0000 0000 0000  <unused>

		unsigned dateYear;
		unsigned dateMonth;
		unsigned dateDay;

		if (flags & 0x8000)
		{
			uint16_t extraneous = bstrm.uint16();

			dateYear = Date::decodeYearFrom10Bits(extraneous & 0x03ff);
			dateMonth = (extraneous >> 10) & 0x000f;
			dateDay = (flags >> 10) & 0x001f;
		}
		else
		{
			dateYear = dateMonth = dateDay = 0;
		}

		base.appendEvent(	name,
								0,
								dateYear,
								dateMonth,
								dateDay,
								event::Type(flags & 0x000f),
								time::Mode((flags >> 7) & 0x0007),
								event::Mode((flags >> 4) & 0x0007),
								site);
	}
	else
	{
		base.appendEvent(name, index, site);
	}

	lookup[index] = 0;

	for (unsigned i = 1; i < count; ++i)
	{
		unsigned	index		= bstrm.uint24();
		unsigned prefix	= bstrm.get();
		unsigned length	= bstrm.get();

		if (prefix > length)
			IO_RAISE(Namebase, Corrupted, "namebase file is broken");

		if (prefix < length)
		{
			char* curr = base.alloc(length);
			M_ASSERT(prev || prefix == 0);
			::memcpy(curr, prev, prefix);
			bstrm.get(curr + prefix, length - prefix);
			name.hook(curr, length);
			prev = curr;
		}

		NamebaseSite* site = ::getSite(namebase(Namebase::Site), m_lookup[Namebase::Site][bstrm.uint24()]);

		site->ref();

		if (uint16_t flags = bstrm.uint16())
		{
			unsigned dateYear;
			unsigned dateMonth;
			unsigned dateDay;

			if (flags & 0x8000)
			{
				uint16_t extraneous = bstrm.uint16();

				dateYear = Date::decodeYearFrom10Bits(extraneous & 0x03ff);
				dateMonth = (extraneous >> 10) & 0x000f;
				dateDay = (flags >> 10) & 0x001f;
			}
			else
			{
				dateYear = dateMonth = dateDay = 0;
			}

			base.appendEvent(	name,
									index,
									dateYear,
									dateMonth,
									dateDay,
									event::Type(flags & 0x000f),
									time::Mode((flags >> 7) & 0x0007),
									event::Mode((flags >> 4) & 0x0007),
									site);
		}
		else
		{
			base.appendEvent(name, index, site);
		}

		lookup[index] = i;
	}
}


void
Codec::readPlayerbase(ByteStream& bstrm, Namebase& base, unsigned count)
{
	if (count == 0)
		return;

	mstl::string name;

	base.reserve(count, 1 << 24);

	unsigned	index		= bstrm.uint24();
	unsigned	length	= bstrm.get();
	char*		prev		= base.alloc(length);
	Lookup&	lookup	= m_lookup[Namebase::Player];

	bstrm.get(prev, length);
	name.hook(prev, length);

	if (Byte flags = bstrm.get())
	{
		// flags (8 bit)
		// -------------------------------
		// 0000 0111  species
		// 0011 1000  sex
		// 0100 0000  <unused>
		// 1000 0000  extranouos data flag
		//
		// extraneous data (16 bit)
		// -------------------------------
		// 0000 0000 0000 1111  title
		// 0001 1111 1111 0000  country
		// 1110 0000 0000 0000  <unused>

		country::Code	country;
		title::ID		title;

		if (flags & 0x80)
		{
			uint16_t extraneous = bstrm.uint16();

			country = country::Code(extraneous >> 4);
			title = title::ID(extraneous & 0x0f);
		}
		else
		{
			country = country::Unknown;
			title = title::None;
		}

		base.appendPlayer(name,
								index,
								country,
								title,
								species::ID(flags & 0x03),
								sex::ID((flags >> 3) & 0x03));
	}
	else
	{
		base.appendPlayer(name, index);
	}

	lookup[index] = 0;

	for (unsigned i = 1; i < count; ++i)
	{
		unsigned	index		= bstrm.uint24();
		unsigned prefix	= bstrm.get();
		unsigned length	= bstrm.get();

		if (prefix > length)
			IO_RAISE(Namebase, Corrupted, "namebase file is broken");

		if (prefix < length)
		{
			char* curr = base.alloc(length);
			M_ASSERT(prev || prefix == 0);
			::memcpy(curr, prev, prefix);
			bstrm.get(curr + prefix, length - prefix);
			name.hook(curr, length);
			prev = curr;
		}

		if (Byte flags = bstrm.get())
		{
			country::Code	country;
			title::ID		title;

			if (flags & 0x80)
			{
				uint16_t extraneous = bstrm.uint16();

				country = country::Code(extraneous >> 4);
				title = title::ID(extraneous & 0x0f);
			}
			else
			{
				country = country::Unknown;
				title = title::None;
			}

			base.appendPlayer(name,
									index,
									country,
									title,
									species::ID(flags & 0x03),
									sex::ID((flags >> 3) & 0x03));
		}
		else
		{
			base.appendPlayer(name, index);
		}

		lookup[index] = i;
	}
}


void
Codec::writeNamebase(mstl::fstream& stream)
{
#ifdef USE_LZO
	LzoByteStream bstrm(stream);
#else
	unsigned char	buf[32768];
	ByteOStream		bstrm(stream, buf, sizeof(buf));
#endif

	bstrm << uint16_t(FileVersion);

	for (unsigned i = 0; i < U_NUMBER_OF(::NamebaseTags); ++i)
	{
		Namebase& base = namebase(Namebase::Type(i));

		bstrm.put(::NamebaseTags[i], 8);
		bstrm << uint24_t(base.used());
		bstrm << uint24_t(base.maxFrequency());
		bstrm << uint24_t(base.maxUsage());

		if (base.used() > 0)
		{
			switch (i)
			{
				case Namebase::Site:		writeSitebase(bstrm, base); break;
				case Namebase::Event:	writeEventbase(bstrm, base); break;
				case Namebase::Player:	writePlayerbase(bstrm, base); break;
				default:						writeNamebase(bstrm, base); break;
			}
		}
	}

	bstrm.flush();
}


void
Codec::writeNamebase(ByteStream& bstrm, Namebase& base)
{
	M_ASSERT(base.used() > 0);

	NamebaseEntry* prev = base.entry(0);

	M_ASSERT(prev->name().size() <= 255);

	bstrm << uint24_t(prev->id());
	bstrm.put(prev->name().size());
	bstrm.put(prev->name(), prev->name().size());

	for (unsigned i = 1; i < base.used(); ++i)
	{
		NamebaseEntry* entry = base.entry(i);

		unsigned prefix = ::prefix(entry->name(), prev->name());
		unsigned length = entry->name().size();

		M_ASSERT(length <= 255);
		M_ASSERT(prefix < length);

		bstrm << uint24_t(entry->id());
		bstrm.put(prefix);
		bstrm.put(length);
		bstrm.put(entry->name().c_str() + prefix, length - prefix);

		prev = entry;
	}
}


void
Codec::writeSitebase(ByteStream& bstrm, Namebase& base)
{
	M_ASSERT(base.used() > 0);

	NamebaseSite* prev = base.site(0);

	M_ASSERT(prev->name().size() <= 255);

	bstrm << uint24_t(prev->id());
	bstrm.put(prev->name().size());
	bstrm.put(prev->name(), prev->name().size());
	bstrm << uint16_t(prev->country());

	for (unsigned i = 1; i < base.used(); ++i)
	{
		NamebaseSite* entry = base.site(i);

		unsigned prefix = ::prefix(entry->name(), prev->name());
		unsigned length = entry->name().size();

		M_ASSERT(length <= 255);
		M_ASSERT(prefix <= length);

		bstrm << uint24_t(entry->id());
		bstrm.put(prefix);
		bstrm.put(length);
		bstrm.put(entry->name().c_str() + prefix, length - prefix);
		bstrm << uint16_t(entry->country());

		prev = entry;
	}
}


void
Codec::writeEventbase(util::ByteStream& bstrm, Namebase& base)
{
	M_ASSERT(base.used() > 0);

	NamebaseEvent* prev = base.event(0);

	M_ASSERT(prev->name().size() <= 255);

	bstrm << uint24_t(prev->id());
	bstrm.put(prev->name().size());
	bstrm.put(prev->name(), prev->name().size());

	bstrm << uint24_t(prev->site()->id());

	uint16_t flags =	 (prev->type() & 0x000f)
						 | ((prev->eventMode() & 0x0007) << 4)
						 | ((prev->timeMode() & 0x0007) << 7);

	if (prev->hasDate())
	{
		flags |= ((prev->dateDay() & 0x001f) << 10) | 0x8000;

		bstrm << flags;
		bstrm << uint16_t(	Date::encodeYearTo10Bits(prev->dateYear())
								 | ((prev->dateMonth() & 0x000f) << 10));
	}
	else
	{
		bstrm << flags;
	}

	for (unsigned i = 1; i < base.used(); ++i)
	{
		NamebaseEvent* entry = base.event(i);

		unsigned prefix = ::prefix(entry->name(), prev->name());
		unsigned length = entry->name().size();

		M_ASSERT(length <= 255);
		M_ASSERT(prefix <= length);

		bstrm << uint24_t(entry->id());
		bstrm.put(prefix);
		bstrm.put(length);
		bstrm.put(entry->name().c_str() + prefix, length - prefix);

		bstrm << uint24_t(entry->site()->id());

		flags =	 (entry->type() & 0x000f)
				 | ((entry->eventMode() & 0x0007) << 4)
				 | ((entry->timeMode() & 0x0007) << 7);

		if (entry->hasDate())
		{
			flags |= ((entry->dateDay() & 0x001f) << 10) | 0x8000;

			bstrm << flags;
			bstrm << uint16_t(	Date::encodeYearTo10Bits(entry->dateYear())
									 | ((entry->dateMonth() & 0x000f) << 10));
		}
		else
		{
			bstrm << flags;
		}

		prev = entry;
	}
}


void
Codec::writePlayerbase(util::ByteStream& bstrm, Namebase& base)
{
	M_ASSERT(base.used() > 0);

	NamebasePlayer* prev = base.player(0);

	M_ASSERT(prev->name().size() <= 255);

	bstrm << uint24_t(prev->id());
	bstrm.put(prev->name().size());
	bstrm.put(prev->name(), prev->name().size());

	Byte flags = (prev->type() & 0x03) | ((prev->sex() & 0x03) << 3);

	if (uint16_t extranouos = ((prev->title() & 0x0f) | (prev->federation() << 4)))
	{
		bstrm.put(flags | 0x80);
		bstrm << extranouos;
	}
	else
	{
		bstrm.put(flags);
	}

	for (unsigned i = 1; i < base.used(); ++i)
	{
		NamebasePlayer* entry = base.player(i);

		unsigned prefix = ::prefix(entry->name(), prev->name());
		unsigned length = entry->name().size();

		M_ASSERT(length <= 255);
		M_ASSERT(prefix <= length);

		bstrm << uint24_t(entry->id());
		bstrm.put(prefix);
		bstrm.put(length);
		bstrm.put(entry->name().c_str() + prefix, length - prefix);

		flags = (entry->type() & 0x03) | ((entry->sex() & 0x03) << 3);

		if (uint16_t extranouos = ((entry->title() & 0x0f) | (entry->federation() << 4)))
		{
			bstrm.put(flags | 0x80);
			bstrm << extranouos;
		}
		else
		{
			bstrm.put(flags);
		}

		prev = entry;
	}
}


void
Codec::useAsyncReader(bool flag)
{
	M_ASSERT(m_gameData);

	if (flag)
	{
		if (m_asyncReader == 0)
			m_asyncReader = m_gameData->openAsyncReader();
	}
	else if (m_asyncReader)
	{
		m_gameData->closeAsyncReader(m_asyncReader);
		m_asyncReader = 0;
	}
}


Move
Codec::findExactPositionAsync(GameInfo const& info, Board const& position, bool skipVariations)
{
	M_ASSERT(m_asyncReader);

	ByteStream src;
	getGameRecord(info, *m_asyncReader, src);
	Decoder decoder(src, m_gameData->blockSize() - info.gameOffset());
	return decoder.findExactPosition(position, skipVariations);
}


uint32_t
Codec::computeChecksum(unsigned flags, GameInfo const& info, unsigned crc) const
{
	ByteStream strm;
	getGameRecord(info, m_gameData->reader(), strm);
	return crc::compute(crc, strm.data(), strm.size());
}

// vi:set ts=3 sw=3:
