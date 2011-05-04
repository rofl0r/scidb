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

#ifndef _db_database_codec_included
#define _db_database_codec_included

#include "db_database_content.h"
#include "db_time.h"
#include "db_move.h"

#include "m_string.h"

namespace sys
{
	namespace utf8 { class Codec; }
}

namespace mstl
{
	class fstream;
	class ifstream;
	class ofstream;
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
class GameData;
class GameInfo;
class TagSet;
class Consumer;
class Producer;
class NamebaseEntry;
class Signature;

class DatabaseCodec
{
public:

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

	static unsigned const Decode_Tags		= 1 << 0;
	static unsigned const Decode_Comments	= 1 << 1;
	static unsigned const Decode_All			= Decode_Tags | Decode_Comments;

	DatabaseCodec();
	virtual ~DatabaseCodec() throw();

	bool isOpen() const;
	virtual bool encodingFailed() const = 0;

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

	virtual void filterTag(TagSet& tags, tag::ID tag) const;
	virtual mstl::string const& extension() const = 0;
	virtual mstl::string const& encoding() const = 0;
	virtual Time modified(mstl::string const& rootname) const;
	virtual uint32_t computeChecksum(unsigned flags, GameInfo const& info, unsigned crc) const;
	virtual util::BlockFile* newBlockFile() const;

	virtual void updateHeader(mstl::string const& rootname);
	virtual void setEncoding(mstl::string const& encoding) = 0;
	virtual void reset() = 0;

	unsigned produce(Producer& producer, Consumer& consumer, util::Progress& progress);

	virtual void recode(	GameInfo const& info,
								util::ByteStream& dst,
								sys::utf8::Codec& oldCodec,
								sys::utf8::Codec& newCodec);

	void open(DatabaseContent* db, mstl::string const& encoding);
	void open(DatabaseContent* db, mstl::string const& rootname, mstl::string const& encoding);
	void open(	DatabaseContent* db,
					mstl::string const& rootname,
					mstl::string const& encoding,
					util::Progress& progress);
	void open(	DatabaseContent* db,
					mstl::string const& encoding,
					Producer& producer,
					util::Progress& progress);
	void clear(mstl::string const& rootname = mstl::string::empty_string);
	virtual void save(mstl::string const& rootname, unsigned start, util::Progress& progress);
	virtual void update(mstl::string const& rootname, unsigned index, bool updateNamebase);
	virtual void attach(mstl::string const& rootname, util::Progress& progress);
	virtual void close() = 0;

	unsigned importGames(Producer& producer, util::Progress& progress, int startIndex = -1);

	void decodeGame(unsigned flags, GameData& data, GameInfo& info);
	void encodeGame(util::ByteStream& strm, GameData const& data, Signature const& signature);

	save::State exportGame(	Consumer& consumer,
									unsigned flags,
									TagSet& tags,
									GameInfo const& info);
	save::State exportGame(	Consumer& consumer,
									util::ByteStream& strm,
									unsigned flags,
									TagSet& tags);

	virtual util::ByteStream getGame(GameInfo const& info);
	save::State addGame(util::ByteStream const& gameData, GameInfo const& info);
	save::State addGame(util::ByteStream& gameData, TagSet const& tags, Consumer& consumer);
	save::State saveGame(util::ByteStream const& gameData, TagSet const& tags, Provider const& provider);
	save::State updateCharacteristics(unsigned index, TagSet const& tags);
	virtual void replaceBlockFile(util::BlockFile* blockFile);

	virtual void useAsyncReader(bool flag);
	virtual Move findExactPositionAsync(GameInfo const& info,
													Board const& position,
													bool skipVariations) = 0;

	GameInfo* allocGameInfo();

	static bool hasCodecFor(mstl::string const& suffix);
	static DatabaseCodec* makeCodec(mstl::string const& suffix);
	static DatabaseCodec* makeCodec();

protected:

	enum Mode { Readonly = 1, Truncate = 2 };

	class InfoData;

	typedef DatabaseContent::GameInfoList GameInfoList;

	virtual Consumer* getConsumer(format::Type srcFormat);

	virtual void doOpen(mstl::string const& encoding);
	virtual void doOpen(mstl::string const& rootname, mstl::string const& encoding);
	virtual void doOpen(	mstl::string const& rootname,
								mstl::string const& encoding,
								util::Progress& progress) = 0;
	virtual void doClear(mstl::string const& rootname);

	virtual void doDecoding(unsigned flags, GameData& data, GameInfo& info) = 0;
	virtual save::State doDecoding(	Consumer& consumer,
												util::ByteStream& strm,
												unsigned flags,
												TagSet& tags);
	virtual save::State doDecoding(	Consumer& consumer,
												unsigned flags,
												TagSet& tags,
												GameInfo const& info) = 0;
	virtual void doEncoding(util::ByteStream& strm, GameData const& data, Signature const& signature);
	virtual unsigned putGame(util::ByteStream const& data);

	bool isReadOnly() const;
	GameInfoList& gameInfoList();
	GameInfo& gameInfo(unsigned index);
	mstl::string const& description() const;
	DatabaseContent::Type type() const;
	Namebase& namebase(Namebase::Type type);
	Namebases& namebases();

	void setType(DatabaseContent::Type type);
	void setDescription(char const* description);

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
