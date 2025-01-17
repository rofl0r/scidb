// ======================================================================
// $RCSfile: tk_image.cpp,v $
// $Revision: 1452 $
// $Date: 2017-12-08 13:37:59 +0000 (Fri, 08 Dec 2017) $
// $Author: gregor $
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
// ======================================================================

// ======================================================================
// Copyright: (C) 2012-2017 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "db_eco_table.h"
#include "db_database.h"
#include "db_pgn_reader.h"
#include "db_exception.h"
#include "db_log.h"

#include "sci/sci_consumer.h"
#include "sci/sci_encoder.h"
#include "sci/sci_codec.h"

#include "u_progress.h"
#include "u_zstream.h"
#include "u_misc.h"

#include "sys_utf8_codec.h"
#include "sys_file.h"

#include "tcl_base.h"

#include "m_ifstream.h"
#include "m_set.h"
#include "m_string.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <tcl.h>

#ifdef BROKEN_LINKER_HACK
# include "db_board.h"
# include "db_board_base.h"
# include "db_home_pawns.h"
# include "db_signature.h"
# include "sci_encoder.h"
# include "si3_encoder.h"
#endif

using namespace db;


static unsigned rejected	= 0;
static unsigned corrupted	= 0;

static bool newNewline = false;

static mstl::string cdbPath;


namespace tcl { namespace bits { Tcl_Interp* interp; } }


static void
newline()
{
	if (newNewline)
	{
		printf("\n");
		newNewline = false;
	}
	fflush(stdout);
}


struct TclInterpreter
{
	TclInterpreter()	{ tcl::bits::interp = Tcl_CreateInterp(); }
	~TclInterpreter()	{ Tcl_DeleteInterp(tcl::bits::interp); }
};


class MyPgnReader : public PgnReader
{
public:

	MyPgnReader(mstl::string const& path, mstl::string const& convertfrom)
		:PgnReader(	*(m_stream = new util::ZStream(sys::file::internalName(path))),
						variant::Normal,
						detectEncoding(path, convertfrom),
						PgnReader::File,
						PgnReader::Normalize,
						PgnReader::UseResultTag)
		,m_tooManyRoundNames(false)
	{
		if (!*m_stream)
		{
			::newline();
			::fprintf(stderr, "cannot open file '%s'\n", path.c_str());
			exit(1);
		}
	}

	~MyPgnReader() throw()
	{
		delete m_stream;
	}

	static mstl::string detectEncoding(mstl::string const& path, mstl::string convertfrom)
	{
		if (convertfrom == sys::utf8::Codec::automatic())
		{
			if (util::ZStream::testByteOrderMark(sys::file::internalName(path)))
				convertfrom = sys::utf8::Codec::utf8();
			else
				convertfrom = sys::utf8::Codec::latin1();
		}
		return convertfrom;
	}

	void warning(	Warning code,
						unsigned lineNo,
						unsigned column,
						unsigned gameNo,
						::db::variant::Type variant,
						mstl::string const& info,
						mstl::string const& item)
	{
		char const* msg;

		switch (code)
		{
			case MissingWhitePlayerTag:			msg = "missing white player tag"; break;
			case MissingBlackPlayerTag:			msg = "missing black player tag"; break;
			case MissingPlayerTags:					msg = "missing player tags"; break;
			case MissingResult:						msg = "missing result"; break;
			case MissingResultTag:					msg = "missing result Tag"; break;
			case InvalidRoundTag:					msg = "invalid round tag"; break;
			case InvalidResultTag:					msg = "invalid result tag"; break;
			case InvalidDateTag:						msg = "invalid date tag"; break;
			case InvalidEventDateTag:				msg = "invalid event date tag"; break;
			case InvalidTimeModeTag:				msg = "invalid time mode tag"; break;
			case InvalidEcoTag:						msg = "invalid eco tag"; break;
			case InvalidTagName:						msg = "invalid tagName"; break;
			case InvalidCountryCode:				msg = "invalid country code"; break;
			case InvalidRating:						msg = "invalid rating"; break;
			case InvalidNag:							msg = "invalid nag"; break;
			case BraceSeenOutsideComment:			msg = "brace seen outside comment"; break;
			case MissingFen:							msg = "missing fen"; break;
			case FixedInvalidFen:					msg = "fixed invalid fen"; break;
			case UnknownEventType:					msg = "unknown event type"; break;
			case UnknownTitle:						msg = "unknown title"; break;
			case UnknownPlayerType:					msg = "unknown player type"; break;
			case UnknownSex:							msg = "unknown sex"; break;
			case UnknownTermination:				msg = "unknown termination"; break;
			case UnknownMode:							msg = "unknown mode"; break;
			case RatingTooHigh:						msg = "rating too high"; break;
			case EncodingFailed:						msg = "encoding failed"; break;
			case TooManyNags:							msg = "too many nags"; break;
			case IllegalCastling:					msg = "illegal castling"; break;
			case IllegalMove:							msg = "illegal move"; break;
			case CastlingCorrection:				msg = "castling correction"; break;
			case ResultDidNotMatchHeaderResult: msg = "result did not match header result"; break;
			case ValueTooLong:						msg = "value too long"; break;
			case NotSuicideNotGiveaway:			msg = "not suicide, not giveaway"; break;
			case VariantChangedToGiveaway:		msg = "variant changed to giveaway"; break;
			case VariantChangedToSuicide:			msg = "variant changed to suicide"; break;
			case ResultCorrection:					msg = "result correction"; break;
			case MaximalErrorCountExceeded:		msg = "maximal error count exceeded"; break;
			case MaximalWarningCountExceeded:	msg = "maximal warning count exceeded"; break;
		}

		::newline();
		::fprintf(stderr, "*** Warning(game=%u,line=%u.%u): %s", gameNo, lineNo, column, msg);
		if (!item.empty())
			::fprintf(stderr, " (%s)", item.c_str());
		::fprintf(stderr, "\n");
	}

	void error(	Error code,
					unsigned lineNo,
					unsigned column,
					unsigned gameNo,
					::db::variant::Type variant,
					mstl::string const& message,
					mstl::string const& info,
					mstl::string const& item)
	{
		char const* msg;

		switch (code)
		{
			case InvalidToken:					msg = "invalid token"; break;
			case UnexpectedSymbol:				msg = "unexpected symbol"; break;
			case UnexpectedEndOfInput:			msg = "unexpected end of input"; break;
			case UnexpectedTag:					msg = "unexpected tag"; break;
			case UnexpectedEndOfGame:			msg = "unexpected end of game"; break;
			case TagNameExpected:				msg = "tag name expected"; break;
			case TagValueExpected:				msg = "tag value expected"; break;
			case InvalidFen:						msg = "invalid FEN"; break;
			case UnterminatedString:			msg = "unterminated string"; break;
			case UnterminatedVariation:		msg = "unterminated variation"; break;
			case InvalidMove:						msg = "invalid move"; break;
			case UnsupportedVariant:			msg = "unsupported variant"; break;
			case SeemsNotToBePgnText:			msg = "seems not to be PGN text"; break;
			case UnexpectedResultToken:		msg = "unexpected result token"; break;
			case UnexpectedCastling:			msg = "unexpected castling"; break;
			case ContinuationsNotSupported:	msg = "continuations not supported"; break;
		}

		::newline();
		::fprintf(stderr, "*** Error(game=%u,line=%u.%u): %s", gameNo, lineNo, column, msg);
		if (!message.empty())
			::fprintf(stderr, " -- %s", message.c_str());
		if (!item.empty())
			::fprintf(stderr, " (%s)", item.c_str());
		::fprintf(stderr, "\n");
	}

	void error(	::db::save::State state,
					unsigned lineNo,
					unsigned gameNo,
					::db::variant::Type variant)
	{
		char const* msg;

		switch (state)
		{
			case db::save::TooManyGames:				msg = "too many games"; break;
			case db::save::FileSizeExeeded:			msg = "file size exeeded"; break;
			case db::save::GameTooLong:				msg = "game too long"; break;
			case db::save::TooManyPlayerNames:		msg = "too many player names"; break;
			case db::save::TooManyEventNames:		msg = "too many event names"; break;
			case db::save::TooManySiteNames:			msg = "too many site names"; break;
			case db::save::TooManyAnnotatorNames:	msg = "too many annotator names"; break;
			case db::save::UnsupportedVariant:		msg = "unsupported variant"; break;

			case db::save::TooManyRoundNames:
				if (m_tooManyRoundNames)
					return;
				m_tooManyRoundNames = true;
				msg = "too many round names"; break;
				break;

			case db::save::Ok:
			case db::save::DecodingFailed:
				return; // should not happen
		}

		::newline();
		::fprintf(stderr, "*** Fatal error while reading game %u: %s\n", gameNo, msg);
	}

private:

	util::ZStream* m_stream;
	bool m_tooManyRoundNames;
};


struct Progress : public util::Progress
{
	void update(unsigned progress) override
	{
		::printf(".");
		::fflush(stdout);
	}

	void finish() throw() override {}
};


struct Log : public db::Log
{
	bool error(save::State code, unsigned gameNumber)
	{
		char const* msg = 0;

		switch (code)
		{
			case save::Ok:								return true;
			case save::UnsupportedVariant:		++rejected; return true;
			case save::DecodingFailed:				++corrupted; return true;
			case save::GameTooLong:					msg = "Game too long"; break; // should not happen
			case save::FileSizeExeeded:			msg = "File size exeeded"; break;
			case save::TooManyGames:				msg = "Too many games"; break;
			case save::TooManyPlayerNames:		msg = "Too many player names"; break;
			case save::TooManyEventNames:			msg = "Too many event names"; break;
			case save::TooManySiteNames:			msg = "Too many site names"; break;
			case save::TooManyRoundNames:			msg = "Too many round names"; break;
			case save::TooManyAnnotatorNames:	msg = "Too many annotator names"; break;
		}

		::fflush(stdout);
		::fprintf(	stderr,
						"%s: %s (#%u) [%s]\n",
						code == db::save::GameTooLong ? "Warning" : "Error",
						msg,
						gameNumber,
						cdbPath.c_str());

		return code == save::GameTooLong;
	}

	void warning(Warning code, unsigned gameNumber) {}
};


typedef sci::Consumer::TagBits TagBits;

static char const* ConvertFrom = "auto";


static TagBits
getDefaultTags()
{
	TagBits defaultTags;

	defaultTags.set(tag::Board);
	defaultTags.set(tag::EventCategory);
	defaultTags.set(tag::EventRounds);
	defaultTags.set(tag::Remark);
	defaultTags.set(tag::Source);
	defaultTags.set(tag::SourceDate);
	defaultTags.set(tag::TimeControl);
	defaultTags.set(tag::WhiteClock);
	defaultTags.set(tag::BlackClock);
	defaultTags.set(tag::WhiteTeam);
	defaultTags.set(tag::BlackTeam);

	return defaultTags;
}


static format::Type
getFormat(mstl::string& path)
{
	mstl::string ext(::util::misc::file::suffix(path));
	format::Type fmt = format::Invalid;

	if (ext == "gz")
	{
		ext = ::util::misc::file::suffix(::util::misc::file::basename(path));
		if (ext == "pgn" || ext == "PGN")
			fmt = format::Pgn;
	}
	else
	{
		if			(ext == "si4") fmt = format::Scid4;
		else if	(ext == "si3") fmt = format::Scid3;
		else if	(ext == "sci") fmt = format::Scidb;
		else if	(ext == "cbh") fmt = format::ChessBase;
		else if	(ext == "cbf") fmt = format::ChessBaseDOS;
		else if	(ext == "CBF") fmt = format::ChessBaseDOS;
		else if	(ext == "pgn") fmt = format::Pgn;
		else if	(ext == "PGN") fmt = format::Pgn;
		else if	(ext == "zip") fmt = format::Pgn;
		else if	(ext == "ZIP") fmt = format::Pgn;
	}

	return fmt;
}


static void
stripSuffix(mstl::string& path)
{
	if (path.size() >= 7 && strcmp(path.c_str() + path.size() - 7, ".pgn.gz") == 0)
	{
		path.erase(path.size() - 7);
	}
	else if (path.size() >= 4)
	{
		char const* s = path.c_str() + path.size() - 4;

		if (	strcmp(s, ".si4") == 0
			|| strcmp(s, ".si3") == 0
			|| strcmp(s, ".sci") == 0
			|| strcmp(s, ".cbh") == 0
			|| strcmp(s, ".cbf") == 0
			|| strcmp(s, ".CBF") == 0
			|| strcmp(s, ".pgn") == 0
			|| strcmp(s, ".PGN") == 0
			|| strcmp(s, ".zip") == 0
			|| strcmp(s, ".ZIP") == 0)
		{
			path.erase(path.size() - 4);
		}
	}
}


static void
loadEcoFile()
{
	mstl::string path(SHAREDIR "/data/eco.bin");
	mstl::ifstream stream(path, mstl::ios_base::in | mstl::ios_base::binary);

	if (!stream)
	{
		fprintf(stderr, "cannot open ECO file '%s'", path.c_str());
		exit(1);
	}

	EcoTable::specimen(variant::Normal).load(stream, variant::Normal);
}


static void
printHelpAndExit(int rc)
{
	printf("Usage: cdb2sci [options ...] <source database> ... [destination database]\n");
	printf("\n");
	printf("Options:\n");
	printf("  --              Only file names after this\n");
	printf("  --help          Print Help (this message) and exit\n");
	printf("  --force         Overwrite existing destination files\n");
	printf("  --convertfrom <encoding>\n");
	printf("                  The encoding of the source database\n");
	printf("                  (default is '%s')\n", ::ConvertFrom);
	printf("  --tags <comma-separated-tag-list>\n");
	printf("                  Export only the tags given with this list\n");
	printf("                  (but mandatory tags will be always exported)\n");
	printf("  --unusual-tags  Export unusual tags (otherwise ignore these tags)\n");
	printf("  --all-tags      Export all tags (overrules option --tags)\n");
	printf("                  (but ignore unusual tags except --unusual-tags is given)\n");
	printf("  --no-tags       Do not export any tag (except mandatory tags)\n");
	printf("                  (overrules --tags, --all-tags, but not --unusual-tags)\n");
	printf("  --list-encodings\n");
	printf("                  List all known encodings\n");
	printf("  --list-mandatory-tags\n");
	printf("                  List all mandatory tags\n");
	printf("  --list-default-tags\n");
	printf("                  List all default tags\n");
	printf("  --list-usual-tags\n");
	printf("                  List all usual tags (includes mandatory tags)\n");
	printf("\n");
	exit(rc);
}


static bool
isForbiddenEncoding(mstl::string const& encoding)
{
	return	encoding == "ascii"
			|| encoding == "identity"
			|| encoding == "dingbats"
			|| encoding == "ebcdic"
			|| encoding == "symbol"
			|| encoding == "unicode";
}


static void
printEncodingsAndExit(int rc)
{
	sys::utf8::Codec::EncodingList encodings;
	sys::utf8::Codec::getEncodingList(encodings);

	sys::utf8::Codec::EncodingList::const_iterator i = encodings.begin();
	sys::utf8::Codec::EncodingList::const_iterator e = encodings.end();

	while (i != e)
	{
		sys::utf8::Codec::EncodingList::const_iterator next = i + 1;

		if (!isForbiddenEncoding(*i))
		{
			printf(i->c_str());

			if (next != e)
				printf(", ");
		}

		i = next;
	}

	printf("\n");
	exit(rc);
}


static void
checkEncoding(mstl::string const& encoding)
{
	if (isForbiddenEncoding(encoding))
	{
		fprintf(stderr, "Encoding '%s' is not allowed\n", encoding.c_str());
		exit(1);
	}

	sys::utf8::Codec::EncodingList encodings;
	sys::utf8::Codec::getEncodingList(encodings);

	sys::utf8::Codec::EncodingList::const_iterator i = encodings.begin();
	sys::utf8::Codec::EncodingList::const_iterator e = encodings.end();

	for ( ; i != e; ++i)
	{
		if (*i == encoding)
			return;
	}

	fprintf(stderr, "Unknown encoding '%s'\n", encoding.c_str());
	exit(1);
}


static void
printDefaultTagsAndExit(int rc)
{
	char const* comma = "";
	TagBits defaultTags = getDefaultTags();

	for (unsigned i = 0; i < 64; ++i)
	{
		if (defaultTags.test(i))
		{
			printf("%s%s", comma, tag::toName(tag::ID(i)).c_str());
			comma = ",";
		}
	}

	printf("\n");
	exit(rc);
}


static void
printUsualTagsAndExit(int rc)
{
	for (unsigned i = 0; i < tag::BughouseTag; ++i)
	{
		if (i > 0)
			printf(",");
		printf("%s", tag::toName(tag::ID(i)).c_str());
	}

	printf("\n");
	exit(rc);
}


static void
printMandatoryTagsAndExit(int rc)
{
	mstl::string	tagList;
	tag::TagSet		mandatoryTags(sci::Encoder::infoTags());

	for (unsigned i = mandatoryTags.find_first(); i != tag::TagSet::npos; i = mandatoryTags.find_next(i))
	{
		if (!tagList.empty())
			tagList += ',';
		tagList += tag::toName(tag::ID(i));
	}

	printf("%s\n", tagList.c_str());
	::exit(rc);
}


static void
checkTagList(mstl::string& tagList)
{
	char* s = tagList.data();
	char* e = s;

	while (true)
	{
		while (*e && *e != ',')
			++e;

		if (!PgnReader::validateTagName(s, e - s))
		{
			fprintf(stderr, "Invalid tag name '%s'.\n", mstl::string(s, e).c_str());
			exit(1);
		}

		if (tag::fromName(s, e - s) == tag::ExtraTag)
		{
			fprintf(stderr, "'%s' is an unusual tag and cannot be included.\n", mstl::string(s, e).c_str());
			fprintf(stderr, "Probably you might use option --unusual-tags.\n");
			exit(1);
		}

		if (*e == '\0')
			return;

		s = ++e;
	}
}


static void
setTagList(TagBits& result, mstl::string const& tagList)
{
	char const* s = tagList;
	char const* e = s;

	while (true)
	{
		while (*e && *e != ',')
			++e;

		result.set(tag::fromName(s, e - s));

		if (*e == '\0')
			return;

		s = ++e;
	}
}


static void
logIOError(IOException const& exc, unsigned gameNumber = 0)
{
	char const* error	= 0;
	char const* file	= 0;

	switch (exc.errorType())
	{
		case IOException::Create_Failed:			error = "no permissions to create files"; break;
		case IOException::Open_Failed:			error = "open failed"; break;
		case IOException::Unknown_Error_Type:	error = "unknown error type"; break;
		case IOException::Unknown_Version:		error = "unknown file version"; break;
		case IOException::Unexpected_Version:	error = "unexpected file version"; break;
		case IOException::Corrupted:				error = "corrupted data"; break;
		case IOException::Invalid_Data:			error = "invalid data (file possibly corrupted)"; break;
		case IOException::Read_Error:				error = "read error"; break;
		case IOException::Load_Failed:			error = "load failed (too many event entries)"; break;

		case IOException::Read_Only:
		case IOException::Write_Failed:
		case IOException::Encoding_Failed:
		case IOException::Max_File_Size_Exceeded:
		case IOException::Not_Original_Version:
		case IOException::Cannot_Create_Thread:
			return; // cannot happen
	}

	switch (exc.fileType())
	{
		case IOException::Unspecified:	break;
		case IOException::Index:			file = "index"; break;
		case IOException::Game:				file = "game"; break;
		case IOException::Namebase:		file = "namebase"; break;
		case IOException::Annotation:		file = "annotation"; break;
		case IOException::PgnFile:			/* cannot happen */ break;
		case IOException::BookFile:		/* cannot happen */ break;
	}

	mstl::string msg("Error");

	if (file)
	{
		msg.append(" in ");
		msg.append(file);
		msg.append(" file");
	}

	msg.append(": ");
	msg.append(error);

	if (gameNumber > 0)
		msg.format(" (#%d)", gameNumber);

	newline();
	fprintf(stderr, "%s\n", msg.c_str());

	switch (exc.errorType())
	{
		case IOException::Unknown_Error_Type:
		case IOException::Create_Failed:
		case IOException::Open_Failed:
		case IOException::Unknown_Version:
		case IOException::Unexpected_Version:
		case IOException::Write_Failed:
		case IOException::Max_File_Size_Exceeded:
		case IOException::Not_Original_Version:
		case IOException::Read_Only:
			fprintf(stderr, "Abort.\n");
			exit(1);

		case IOException::Read_Error:
		case IOException::Corrupted:
		case IOException::Invalid_Data:
		case IOException::Encoding_Failed:
		case IOException::Load_Failed:
		case IOException::Cannot_Create_Thread:
			break;
	}
}


static unsigned
exportGames(Database& src, Consumer& dst, ::util::Progress& progress)
{
	unsigned numGames = src.countGames();

	util::ProgressWatcher watcher(progress, numGames);
	progress.setFrequency(mstl::min(200u, mstl::max(numGames/1000, 50u)));

	unsigned reportAfter	= progress.frequency();
	unsigned count			= 0;
	unsigned countGames	= 0;

	::Log log;

	for (unsigned i = 0; i < numGames; ++i)
	{
		if (reportAfter == count)
		{
			progress.update(count);
			reportAfter += progress.frequency();
			::newNewline = true;
		}
		count += 1;

		try
		{
			save::State state = src.exportGame(i, dst);

			if (state == save::Ok)
				++countGames;
			else if (!log.error(state, i + 1))
				return countGames;
		}
		catch (IOException const& exc)
		{
			logIOError(exc, i + 1);
		}
		catch (...)
		{
			printf("\n");
			fflush(stdout);
			fprintf(stderr, "*** Exception catched while decoding game #%u\n", count);
			throw;
		}
	}

	return countGames;
}


int
main(int argc, char* argv[])
{
#ifdef BROKEN_LINKER_HACK

		// HACK!
		// This hack is required because the linker is not working
		// properly anymore.
		db::tag::initialize();
		db::castling::initialize();
		db::board::base::initialize();
		db::Board::initialize();
		db::HomePawns::initialize();
		db::Signature::initialize();
		db::sci::Encoder::initialize();
		db::si3::Encoder::initialize();

#endif

	mstl::backtrace::disable();

	typedef mstl::set<mstl::string> BaseList;

	TclInterpreter	tclInterpreter;
	mstl::string	convertfrom(::ConvertFrom);
	bool				force(false);
	bool				allTags(false);
	bool				noTags(false);
	bool				defaultTags(true);
	bool				extraTags(false);
	mstl::string	givenTags;
	BaseList			baseList;

	int i = 1;

	for ( ; i < argc && argv[i][0] == '-'; ++i)
	{
		if (strcmp(argv[i], "--") == 0)
			break;

		if (strcmp(argv[i], "--help") == 0)
		{
			printHelpAndExit(0);
		}
		else if (strcmp(argv[i], "--convertfrom") == 0)
		{
			if (++i == argc)
				printHelpAndExit(1);
			checkEncoding(argv[i]);
			convertfrom.assign(argv[i]);
		}
		else if (strcmp(argv[i], "--tags") == 0)
		{
			if (++i == argc)
				printHelpAndExit(1);
			givenTags = argv[i];
			checkTagList(givenTags);
			defaultTags = false;
		}
		else if (strcmp(argv[i], "--all-tags") == 0)
		{
			allTags = true;
		}
		else if (strcmp(argv[i], "--no-tags") == 0)
		{
			noTags = true;
		}
		else if (strcmp(argv[i], "--unusual-tags") == 0)
		{
			extraTags = true;
		}
		else if (strcmp(argv[i], "--list-encodings") == 0)
		{
			printEncodingsAndExit(0);
		}
		else if (strcmp(argv[i], "--list-default-tags") == 0)
		{
			printDefaultTagsAndExit(0);
		}
		else if (strcmp(argv[i], "--list-mandatory-tags") == 0)
		{
			printMandatoryTagsAndExit(0);
		}
		else if (strcmp(argv[i], "--list-usual-tags") == 0)
		{
			printUsualTagsAndExit(0);
		}
		else if (strcmp(argv[i], "--force") == 0)
		{
			force = true;
		}
		else
		{
			fprintf(stderr, "Unrecognized option '%s'.\n\n", argv[i]);
			printHelpAndExit(1);
		}
	}

	if (convertfrom != "auto" && !sys::utf8::Codec::checkEncoding(convertfrom))
	{
		fprintf(stderr, "Unknown encoding '%s'.\n\n", convertfrom.c_str());
		printEncodingsAndExit(1);
	}

	mstl::string sciPath;
	mstl::string cdbPath;

	for ( ; i < argc - 1; i++)
	{
		mstl::string path(sys::file::normalizedName(argv[i]));

		if (getFormat(path) == format::Invalid)
		{
			fprintf(stderr, "'%s' is not a Scidb database.\n", path.c_str());
			exit(1);
		}

		if (access(sys::file::internalName(path), R_OK) != 0)
		{
			fprintf(stderr, "Cannot open file '%s'.\n", path.c_str());
			exit(1);
		}

		baseList.push_back(path);

		if (cdbPath.empty())
			cdbPath = path;
	}

	if (i == argc - 1)
	{
		mstl::string path(sys::file::normalizedName(argv[i]));

		if (getFormat(path) == format::Scidb)
		{
			sciPath = path;
		}
		else if (getFormat(path) == format::Invalid)
		{
			sciPath = path;
			mstl::string::size_type n = sciPath.rfind('/');

			if (n != mstl::string::npos)
				sciPath.erase(mstl::string::size_type(0), n + 1);

			stripSuffix(sciPath);
		}
		else if (access(sys::file::internalName(path), R_OK) != 0)
		{
			fprintf(stderr, "Cannot open file '%s'.\n", path.c_str());
			exit(1);
		}
		else
		{
			baseList.push_back(path);
		}
	}

	if (baseList.empty())
	{
		fprintf(stderr, "No input database specified.\n\n");
		printHelpAndExit(1);
	}

	if (sciPath.empty() && !cdbPath.empty())
		sciPath = cdbPath;

	if (sciPath.empty())
	{
		fprintf(stderr, "No output database specified.\n\n");
		printHelpAndExit(1);
	}

	if (sciPath.size() < 4 || strcmp(sciPath.c_str() + sciPath.size() - 4, ".sci") != 0)
		sciPath.append(".sci");

	if (!force && access(sys::file::internalName(sciPath), F_OK) == 0)
	{
		fprintf(stderr, "Database '%s' already exists.\n", sciPath.c_str());
		exit(1);
	}

	if (baseList.contains(sciPath))
	{
		fprintf(stderr, "Database '%s' cannot be used as input and output.\n", sciPath.c_str());
		exit(1);
	}

	try
	{
		loadEcoFile();

		Progress	progress;
		Database	dst(sciPath, sys::utf8::Codec::utf8(), storage::OnDisk, variant::Normal);
		TagBits	tagList;

		if (!noTags)
		{
			if (allTags)
				tagList.set();
			else if (defaultTags)
				tagList = getDefaultTags();
			else
				setTagList(tagList, givenTags);
		}

		for (BaseList::const_iterator i = baseList.begin(); i != baseList.end(); ++i)
		{
			cdbPath.assign(*i);

			mstl::string	ext(::util::misc::file::suffix(cdbPath));
			format::Type	format(getFormat(cdbPath));
			Database*		src(nullptr);
			unsigned			numGames;

			::newline();
			printf("Open '%s' ", cdbPath.c_str());
			printf("\nAppend to '%s' ", sciPath.c_str());
			fflush(stdout);
			::newNewline = true;

			if (format == format::Pgn)
			{
				MyPgnReader reader(cdbPath, convertfrom);
				dst.setType(type::PGNFile);
				dst.importGames(reader, progress);
				numGames = reader.accepted(variant::Normal);
				rejected = 0;

				for (unsigned i = 0; i < variant::NumberOfVariants; ++i)
					rejected += reader.rejected(i);
			}
			else
			{
				sci::Consumer::Codecs codecs(&dynamic_cast<sci::Codec&>(dst.codec()));
				src = new Database(cdbPath, convertfrom, permission::ReadOnly, progress);
				sci::Consumer consumer(format, codecs, tagList, extraTags);
				dst.setType(src->type());
				numGames = exportGames(*src, consumer, progress);
			}

			dst.save(progress);
			printf("\n*** %u game(s) written.", numGames);
			if (rejected > 0)
				printf("\n*** %u game(s) rejected.", rejected);
			if (corrupted > 0)
				printf("\n*** %u game(s) corrupted.", corrupted);
			printf("\n");
			fflush(stdout);
			corrupted = 0;
			delete src;
		}

		dst.close();
	}
	catch (IOException const& exc)
	{
		logIOError(exc);
		exit(1);
	}
	catch (mstl::exception const& exc)
	{
		fflush(stdout);
		fprintf(stderr, "\n%s\n", exc.what());
		exit(1);
	}
	catch (...)
	{
		fflush(stdout);
		fprintf(stderr, "\nunknown exception catched\n");
		exit(1);
	}

	return 0;
}

// vi:set ts=3 sw=3:
