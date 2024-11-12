// ======================================================================
// Author : $Author$
// Version: $Revision: 56 $
// Date   : $Date: 2011-06-28 14:04:22 +0000 (Tue, 28 Jun 2011) $
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

#ifndef _sci_codec_included
#define _sci_codec_included

#include "db_database_codec.h"
#include "db_common.h"

#include "m_fstream.h"
#include "m_string.h"
#include "m_vector.h"

namespace util
{
	class ByteStream;
	class BlockFile;
	class BlockFileReader;
	class Progress;
}

namespace db {

class GameData;
class GameInfo;
class Namebase;
class Consumer;
class TagSet;

namespace sci {

class Codec : public DatabaseCodec
{
public:

	Codec();
	~Codec() throw();

	bool encodingFailed() const;

	Format format() const;

	unsigned maxGameRecordLength() const;
	unsigned maxGameLength() const;
	unsigned maxGameCount() const;
	unsigned maxPlayerCount() const;
	unsigned maxEventCount() const;
	unsigned maxSiteCount() const;
	unsigned maxAnnotatorCount() const;
	unsigned minYear() const;
	unsigned maxYear() const;
	unsigned maxDescriptionLength() const;

	unsigned gameFlags() const;

	void filterTag(TagSet& tags, tag::ID tag) const;
	mstl::string const& extension() const;
	mstl::string const& encoding() const;

	void doOpen(mstl::string const& encoding);
	void doOpen(mstl::string const& rootname, mstl::string const& encoding);
	void doOpen(mstl::string const& rootname,
					mstl::string const& encoding,
					util::Progress& progress);
	void doClear(mstl::string const& rootname);

	unsigned putGame(util::ByteStream const& strm);
	unsigned putGame(util::ByteStream const& strm, unsigned prevOffset, unsigned prevRecordLength);
	util::ByteStream getGame(GameInfo const& info);
	void save(mstl::string const& rootname, unsigned start, util::Progress& progress);
	void attach(mstl::string const& rootname, util::Progress& progress);
	void update(mstl::string const& rootname);
	void update(mstl::string const& rootname, unsigned index, bool updateNamebase);
	void updateHeader(mstl::string const& rootname);
	void unlock(mstl::string const& rootname);
	void close();
	void sync();

	save::State doDecoding(db::Consumer& consumer, TagSet& tags, GameInfo const& info);
	save::State doDecoding(db::Consumer& consumer, util::ByteStream& strm, TagSet& tags);
	void doDecoding(GameData& data, GameInfo& info);

	void doEncoding(util::ByteStream& strm, GameData const& data, Signature const& signature);
	Consumer* getConsumer(format::Type srcFormat);

	void reset();
	void setEncoding(mstl::string const& encoding);

	void useAsyncReader(bool flag);
	Move findExactPositionAsync(GameInfo const& info, Board const& position, bool skipVariations);

private:

	typedef mstl::vector<unsigned> Lookup;

	void encodeIndex(GameInfo const& item, util::ByteStream& buf);

	void decodeIndex(mstl::fstream &fstrm, util::Progress& progress);
	void decodeIndex(util::ByteStream& strm, GameInfo& item);

	void readNamebases(mstl::fstream& stream, util::Progress& progress);
	void readNamebase(util::ByteStream& bstrm, Namebase& base, unsigned count, util::Progress& progress);
	void readSitebase(util::ByteStream& bstrm, Namebase& base, unsigned count, util::Progress& progress);
	void readEventbase(util::ByteStream& bstrm, Namebase& base, unsigned count, util::Progress& progress);
	void readPlayerbase(util::ByteStream& bstrm, Namebase& base, unsigned count,util::Progress& progress);

	void updateIndex(mstl::fstream& fstrm);
	void writeIndex(mstl::fstream& fstrm, unsigned start, util::Progress& progress);
	void writeIndexHeader(mstl::fstream& fstrm);
	void writeNamebases(mstl::fstream& stream);
	void writeNamebase(util::ByteStream& bstrm, Namebase& base);
	void writeSitebase(util::ByteStream& bstrm, Namebase& base);
	void writeEventbase(util::ByteStream& bstrm, Namebase& base);
	void writePlayerbase(util::ByteStream& bstrm, Namebase& base);

	void save(mstl::string const& rootname, unsigned start, util::Progress& progress, bool attach);
	uint16_t readIndexHeader(mstl::fstream& fstrm);
	void checkFileVersion(mstl::fstream& fstrm, mstl::string const& magic, uint16_t fileVersion);

	mstl::fstream				m_gameStream;
	util::BlockFile*			m_gameData;
	util::BlockFileReader*	m_asyncReader;
	mstl::string				m_magicGameFile;
	Lookup						m_lookup[4];
	unsigned						m_progressFrequency;
	unsigned						m_progressReportAfter;
	unsigned						m_progressCount;
};

} // namespace sci
} // namespace db

#endif // _sci_codec_included

// vi:set ts=3 sw=3:
