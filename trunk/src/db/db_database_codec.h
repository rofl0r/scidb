// ======================================================================
// Author : $Author$
// Version: $Revision: 1437 $
// Date   : $Date: 2017-10-04 11:10:20 +0000 (Wed, 04 Oct 2017) $
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

#ifndef _db_database_codec_included
#define _db_database_codec_included

#include "db_database_content.h"
#include "db_time.h"
#include "db_move.h"

#include "u_crc.h"

#include "m_string.h"
#include "m_vector.h"

namespace sys
{
	namespace utf8 { class Codec; }
}

namespace mstl
{
	class fstream;
	class ifstream;
	class ofstream;
	class ostream;
	template <typename T, typename U> class map;
}

namespace util
{
	class Progress;
	class BlockFile;
	class BlockFileReader;
	class ByteStream;
}

namespace db {

class Board;
class Database;
class GameData;
class GameInfo;
class TagSet;
class Consumer;
class Producer;
class NamebaseEntry;
class Signature;
class Time;

class DatabaseCodec
{
public:

	enum Mode { Existing, New };

	typedef mstl::map<mstl::string,unsigned> TagMap;
	typedef mstl::vector<mstl::string> StringList;
	typedef tag::TagSet TagBits;

	class CustomFlags
	{
	public:

		CustomFlags();

		char const* get(unsigned n) const;
		void set(unsigned n, char const* text);
		void set(unsigned n, mstl::string const& text);

	private:

		typedef char String[9];
		String m_text[6];
	};

	typedef format::Type Format;

	enum Section		{ InfoTags, GameTags };
	enum Allocation	{ Hook, Alloc };

	DatabaseCodec();
	virtual ~DatabaseCodec() throw();

	bool isOpen() const;
	virtual bool isExpired() const;
	virtual bool isWritable() const = 0;
	virtual bool encodingFailed() const = 0;
	virtual bool usingAsyncReader() const;

	variant::Type variant() const;
	virtual Format format() const = 0;

	CustomFlags const& customFlags() const;
	CustomFlags& customFlags();

	virtual unsigned maxGameRecordLength() const = 0;
	virtual unsigned maxGameLength() const = 0;
	virtual unsigned maxGameCount() const = 0;
	virtual unsigned maxPlayerCount() const = 0;
	virtual unsigned maxEventCount() const = 0;
	virtual unsigned maxSiteCount() const = 0;
	virtual unsigned maxAnnotatorCount() const = 0;
	virtual unsigned minYear() const = 0;
	virtual unsigned maxYear() const = 0;
	virtual unsigned maxDescriptionLength() const = 0;
	virtual unsigned gameFlags() const = 0;
	virtual mstl::string const& defaultEncoding() const = 0;

	virtual tag::TagSet tagFilter(Section section, TagSet const& tags) const = 0;
	virtual mstl::string const& extension() const = 0;
	virtual mstl::string const& encoding() const = 0;
	virtual Time modified() const;
	uint32_t created() const;
	virtual util::crc::checksum_t computeChecksum(GameInfo const& info, unsigned crc) const;

	void updateHeader();
	virtual void setEncoding(mstl::string const& encoding) = 0;
	virtual void setWritable();
	virtual void reset() = 0;

	void open(DatabaseContent* db, mstl::string const& encoding);
	void open(DatabaseContent* db, mstl::string const& encoding, util::Progress& progress);
	void open(	DatabaseContent* db,
					mstl::string const& encoding,
					Producer& producer,
					util::Progress& progress);
	unsigned openProgressive(DatabaseContent* db, mstl::string const& encoding);
	void clear();
	void rename(mstl::string const& oldName, mstl::string const& newName);

	void save(unsigned start, util::Progress& progress);
	virtual void writeNamebases(mstl::ostream& os, util::Progress& progress);
	virtual void writeIndex(mstl::ostream& os, util::Progress& progress);
	virtual void writeGames(mstl::ostream& os, util::Progress& progress);
	void update(unsigned index, bool updateNamebase);
	void attach(util::Progress& progress);
	void reloadDescription();
	void reloadNamebases(util::Progress& progress);
	virtual void close() = 0;
	void removeAllFiles();
	virtual void readIndexProgressive(unsigned index);
	virtual bool stripMoveInformation(GameInfo const& info, unsigned types);
	virtual bool stripTags(GameInfo const& info, TagMap const& tags);
	virtual void findTags(GameInfo const& info, TagMap& tags) const;

	unsigned importGames(Producer& producer, util::Progress& progress, int startIndex = -1);

	unsigned decodeGame(	::util::BlockFileReader* asyncReader,
								GameInfo const& info,
								uint16_t* line,
								unsigned length,
								Board& startBoard,
								bool useStartBoard);
	void decodeGame(GameData& data, GameInfo& info, unsigned gameIndex, mstl::string* encoding = 0);
	void encodeGame(	util::ByteStream& strm,
							GameData const& data,
							Signature const& signature,
							unsigned langFlags);
	void encodeGame(	util::ByteStream& strm,
							GameData const& data,
							Signature const& signature,
							unsigned langFlags,
							TagBits const& allowedTags,
							bool allowExtraTags);

	save::State exportGame(Consumer& consumer, TagSet& tags, GameInfo const& info, unsigned gameIndex);
	save::State exportGame(Consumer& consumer, util::ByteStream& strm, TagSet& tags);

	virtual util::ByteStream getGame(GameInfo const& info);
	save::State addGame(util::ByteStream const& gameData, GameInfo const& info, Allocation allocation);
	save::State addGame(util::ByteStream& gameData, TagSet const& tags, Consumer& consumer);
	save::State saveGame(util::ByteStream const& gameData, TagSet const& tags, Provider const& provider);
	save::State updateCharacteristics(unsigned index, TagSet const& tags);
	save::State saveMoves(util::ByteStream const& gameData, Provider const& provider);
	virtual void sync();

	virtual ::util::BlockFileReader* getAsyncReader();
	virtual Consumer* getConsumer(format::Type srcFormat);
	virtual void closeAsyncReader(::util::BlockFileReader* reader);

	Move findExactPosition(GameInfo const& info, Board const& position, bool skipVariations);
	virtual Move findExactPosition(	GameInfo const& info,
												Board const& position,
												bool skipVariations,
												::util::BlockFileReader* reader);

	static bool hasCodecFor(mstl::string const& suffix);
	static bool upgradeIndexOnly();
	static DatabaseCodec* makeCodec(mstl::string const& name, Mode mode);
	static DatabaseCodec* makeCodec();

	static int getNumberOfGames(mstl::string const& filename);
	static bool getAttributes(	mstl::string const& filename,
										int& numGames,
										type::ID& type,
										variant::Type& variant,
										uint32_t& creationTime,
										mstl::string* description = 0);
	static void getSuffixes(mstl::string const& filename, StringList& result);

	// public for index recovering
	Namebases& namebases();

protected:

	enum { Readonly = 1, Truncate = 2 };

	class InfoData;

	typedef DatabaseContent::GameInfoList GameInfoList;

	virtual void updateHeader(mstl::string const& rootname);
	virtual void save(mstl::string const& rootname, unsigned start, util::Progress& progress);
	virtual void update(mstl::string const& rootname, unsigned index, bool updateNamebase);
	virtual void attach(mstl::string const& rootname, util::Progress& progress);
	virtual void reloadDescription(mstl::string const& rootname);
	virtual void reloadNamebases(	mstl::string const& rootname,
											mstl::string const& originalSuffix,
											util::Progress& progress);
	virtual void removeAllFiles(mstl::string const& rootname);

	virtual void doOpen(mstl::string const& encoding);
	virtual void doOpen(mstl::string const& rootname, mstl::string const& encoding);
	virtual void doOpen(	mstl::string const& rootname,
								mstl::string const& originalSuffix,
								mstl::string const& encoding,
								util::Progress& progress) = 0;
	virtual unsigned doOpenProgressive(mstl::string const& rootname, mstl::string const& encoding);
	virtual void doClear(mstl::string const& rootname);

	virtual unsigned doDecoding(	::util::BlockFileReader* asyncReader,
											GameInfo const& info,
											uint16_t* line,
											unsigned length,
											Board& startBoard,
											bool useStartBoard) = 0;
	virtual void doDecoding(GameData& data,
									GameInfo& info,
									unsigned gameIndex,
									mstl::string* encoding) = 0;
	virtual save::State doDecoding(Consumer& consumer, util::ByteStream& strm, TagSet& tags);
	virtual save::State doDecoding(	Consumer& consumer,
												TagSet& tags,
												GameInfo const& info,
												unsigned gameIndex) = 0;
	virtual void doEncoding(util::ByteStream& strm,
									GameData const& data,
									Signature const& signature,
									unsigned langFlags,
									TagBits const& allowedTags,
									bool allowExtraTags);
	virtual unsigned putGame(util::ByteStream const& data);
	virtual unsigned putGame(	util::ByteStream const& strm,
										unsigned prevOffset,
										unsigned prevRecordLength);
	virtual void writeIndexProgressively(	mstl::string const& rootname,
														GameInfo const& info,
														unsigned index);

	bool isReadonly() const;
	bool shouldCompact() const;
	GameInfoList& gameInfoList();
	mstl::string const& description() const;
	DatabaseContent::Type type() const;
	Namebase& namebase(Namebase::Type type);
	GameInfo& gameInfo(unsigned index);

	void setVariant(variant::Type variant);
	void setType(DatabaseContent::Type type);
	void setCreated(uint32_t time);
	void setDescription(char const* description);
	void shouldCompact(bool flag);
	void useEncoding(mstl::string const& encoding);

	void checkPermissions(mstl::string const& filename);

	void openFile(mstl::fstream& stream, mstl::string const& filename, unsigned mode = 0);
	void openFile(	mstl::fstream& stream,
						mstl::string const& filename,
						mstl::string const& magic,
						unsigned mode = 0);

	static void getGameRecord(GameInfo const& info, util::BlockFileReader& reader, util::ByteStream& src);

private:

	DatabaseContent*	m_db;
	CustomFlags*		m_customFlags;
	GameInfo*			m_storedInfo;
};

} // namespace db

#include "db_database_codec.ipp"

#endif // _db_database_codec_included

// vi:set ts=3 sw=3:
