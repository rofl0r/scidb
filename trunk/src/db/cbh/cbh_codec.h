// ======================================================================
// Author : $Author$
// Version: $Revision: 1452 $
// Date   : $Date: 2017-12-08 13:37:59 +0000 (Fri, 08 Dec 2017) $
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

#ifndef _cbh_codec_included
#define _cbh_codec_included

#include "db_database_codec.h"
#include "db_move_node.h"
#include "db_date.h"

#include "sys_mutex.h"

#include "nsUniversalDetector.h"

#include "m_fstream.h"
#include "m_map.h"
#include "m_vector.h"
#include "m_chunk_allocator.h"
#include "m_fixed_size_allocator.h"

namespace sys  { namespace utf8 { class Codec; }; };
namespace mstl { class fstream; }
namespace util { class ByteStream; };

namespace db {

class NamebaseEntry;
class NamebaseEvent;
class NamebasePlayer;
class TagSet;
class GameInfo;
class Date;

namespace cbh {

class Codec : public DatabaseCodec, public nsUniversalDetector
{
public:

	Codec();
	~Codec() throw();

	bool isWritable() const override;
	bool encodingFailed() const override;

	Format format() const override;

	unsigned maxGameRecordLength() const override;
	unsigned maxGameLength() const override;
	unsigned maxGameCount() const override;
	unsigned maxPlayerCount() const override;
	unsigned maxEventCount() const override;
	unsigned maxSiteCount() const override;
	unsigned maxAnnotatorCount() const override;
	unsigned minYear() const override;
	unsigned maxYear() const override;
	unsigned maxDescriptionLength() const override;
	mstl::string const& defaultEncoding() const override;

	unsigned gameFlags() const override;

	mstl::string const& extension() const override;
	mstl::string const& encoding() const override;
	db::tag::TagSet tagFilter(Section section, TagSet const& tags) const override;

	void doOpen(mstl::string const& rootname,
					mstl::string const& originalSuffix,
					mstl::string const& encoding,
					util::Progress& progress) override;
	void reloadDescription(mstl::string const& rootname) override;
	void reloadNamebases(mstl::string const& rootname,
								mstl::string const& originalSuffix,
								util::Progress& progress) override;

	void close() override;

	unsigned doDecoding(	::util::BlockFileReader* reader,
								GameInfo const& info,
								uint16_t* line,
								unsigned length,
								Board& startBoard,
								bool useStartBoard) override;
	void doDecoding(GameData& data, GameInfo& info, unsigned gameIndex, mstl::string*) override;
	save::State doDecoding(	Consumer& consumer,
									TagSet& tags,
									GameInfo const& info,
									unsigned gameIndex) override;

	void reset() override;
	void setEncoding(mstl::string const& encoding) override;

	static bool getAttributes(	mstl::string const& filename,
										int& numGames,
										db::type::ID& type,
										mstl::string* description);
	static void getSuffixes(mstl::string const& filename, StringList& result);

	struct Tournament
	{
		Tournament();
		Tournament(Byte cat, Byte nrounds);

		Byte category;
		Byte rounds;
	}
	__attribute__((packed));

private:

	class Source;

	struct Team
	{
		mstl::string	title;
		country::Code	nation;
	};

	typedef mstl::map<uint32_t,NamebaseEntry*>			BaseMap;
	typedef mstl::map<uint32_t,uint32_t>					AnnotationMap;
	typedef mstl::vector<Source*>								SourceMap;
	typedef mstl::map<NamebaseEvent const*,Tournament>	TournamentMap;
	typedef mstl::chunk_allocator<NamebaseEvent>			Allocator;
	typedef mstl::vector<Team*>								TeamBase;
	typedef mstl::map<GameInfo const*,unsigned>			GameIndexLookup;
	typedef mstl::fixed_size_allocator<db::MoveNode>	MoveNodeAllocator;

	void startDecoding(	util::ByteStream& gameStream,
								util::ByteStream* annotationStream,
								GameInfo const& info,
								bool& isChess960);
	void decodeIndex(util::ByteStream& strm, GameInfo& info);
	void decodeGuidingText(util::ByteStream& strm);

	unsigned readHeader(mstl::string const& rootname);

	void readIniData(mstl::string const& rootname);
	void readPlayerData(mstl::string const& rootname, util::Progress& progress);
	void readTournamentData(mstl::string const& rootname, util::Progress& progress);
	void readAnnotatorData(mstl::string const& rootname, util::Progress& progress);
	void readSourceData(mstl::string const& rootname, util::Progress& progress);
	void readTeamData(mstl::string const& rootname, util::Progress& progress);
	void readIndexData(mstl::string const& rootname, util::Progress& progress);
	void reloadPlayerData(mstl::string const& rootname, util::Progress& progress);
	void reloadTournamentData(mstl::string const& rootname, util::Progress& progress);
	void reloadAnnotatorData(mstl::string const& rootname, util::Progress& progress);
	void reloadSourceData(mstl::string const& rootname, util::Progress& progress);
	void reloadTeamData(mstl::string const& rootname, util::Progress& progress);
	void reloadIndexData(mstl::string const& rootname, util::Progress& progress);
	void preloadPlayerData(mstl::string const& rootname, util::Progress& progress);
	void preloadTournamentData(mstl::string const& rootname, util::Progress& progress);
	void preloadAnnotatorData(mstl::string const& rootname, util::Progress& progress);

	void addSourceTags(TagSet& tags, unsigned index);
	void addEventTags(TagSet& tags, GameInfo const& info);

	NamebasePlayer* getPlayer(uint32_t ref);
	NamebaseEvent* getEvent(uint32_t ref);
	NamebaseEntry* getAnnotator(uint32_t ref);
	Source* getSource(uint32_t ref);

	void addTeamTags(TagSet& tags, GameInfo const& info);
	void mapPlayerName(mstl::string& str);
	void toUtf8(mstl::string& str);

	void Report(char const* charset) override;

	static void readIniData(mstl::fstream& strm,
									sys::utf8::Codec& codec,
									db::type::ID& type,
									mstl::string& title);

	sys::utf8::Codec*	m_codec;
	mstl::fstream		m_gameStream;
	mstl::fstream		m_annotationStream;
	mstl::fstream		m_teamStream;
	unsigned				m_teamRecords;
	unsigned				m_teamRecordSize;
	AnnotationMap		m_annotationMap;
	BaseMap				m_playerMap;
	BaseMap				m_eventMap;
	BaseMap				m_annotatorMap;
	SourceMap			m_sourceMap;
	BaseMap				m_sourceMap2;
	TournamentMap		m_tournamentMap;
	TeamBase				m_teamBase;
	GameIndexLookup	m_gameIndexLookup;
	mstl::string		m_encoding;
	Allocator			m_allocator;
	Namebase				m_sourceBase;
	NamebaseEvent*		m_illegalEvent;
	NamebasePlayer*	m_illegalPlayer;
	unsigned				m_numGames;
	bool					m_highQuality;
	MoveNodeAllocator	m_moveNodeAllocator;
	sys::Mutex			m_mutex;
};

} // namespace cbh
} // namespace db

namespace mstl {

template <typename T> struct is_movable;

template <>
struct is_movable<::db::cbh::Codec::Tournament> { enum { value = 1 }; };

}

#endif // _cbh_codec_included

// vi:set ts=3 sw=3:
