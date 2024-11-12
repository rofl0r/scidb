// ======================================================================
// $RCSfile: tk_image.cpp,v $
// $Revision: 102 $
// $Date: 2011-11-10 14:04:49 +0000 (Thu, 10 Nov 2011) $
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
// Copyright: (C) 2011 Gregor Cramer
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
#include "db_log.h"

#include "si3/si3_consumer.h"
#include "si3/si3_encoder.h"
#include "si3/si3_codec.h"

#include "u_progress.h"

#include "sys_utf8_codec.h"

#include "tcl_base.h"

#include "m_ifstream.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <tcl.h>

using namespace db;


static unsigned rejected = 0;


namespace tcl { namespace bits { Tcl_Interp* interp; } }


struct TclInterpreter
{
	TclInterpreter()	{ tcl::bits::interp = Tcl_CreateInterp(); }
	~TclInterpreter()	{ Tcl_DeleteInterp(tcl::bits::interp); }
};


struct Progress : public util::Progress
{
	void update(unsigned progress)
	{
		::printf(".");
		::fflush(stdout);
	}

	void finish() { }
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
			case save::DecodingFailed:				++rejected; return true;
			case save::GameTooLong:					msg = "Game too long"; break;
			case save::FileSizeExeeded:			msg = "File size exeeded"; break;
			case save::TooManyGames:				msg = "Too many games"; break;
			case save::TooManyPlayerNames:		msg = "Too many player names"; break;
			case save::TooManyEventNames:			msg = "Too many event names"; break;
			case save::TooManySiteNames:			msg = "Too many site names"; break;
			case save::TooManyRoundNames:			msg = "Too many round names"; break;
			case save::TooManyAnnotatorNames:	msg = "Too many annotator names"; break;
		}

		::fprintf(	stderr,
						"%s: %s (#%u)\n",
						code == db::save::GameTooLong ? "Warning" : "Error",
						msg,
						gameNumber);

		return code == save::GameTooLong;
	}
};


typedef si3::Consumer::TagBits TagBits;


static TagBits
getDefaultTags()
{
	TagBits defaultTags;

	defaultTags.set(tag::Board);
	defaultTags.set(tag::EventCountry);
	defaultTags.set(tag::EventType);
	defaultTags.set(tag::Mode);
	defaultTags.set(tag::Remark);
	defaultTags.set(tag::TimeControl);
	defaultTags.set(tag::TimeMode);
	defaultTags.set(tag::WhiteClock);
	defaultTags.set(tag::WhiteFideId);
	defaultTags.set(tag::WhiteTeam);
	defaultTags.set(tag::WhiteTitle);
	defaultTags.set(tag::BlackClock);
	defaultTags.set(tag::BlackFideId);
	defaultTags.set(tag::BlackTeam);
	defaultTags.set(tag::BlackTitle);

	return defaultTags;
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

	EcoTable::specimen().load(stream);
}


static void
printHelpAndExit(int rc)
{
	printf("Usage: cbh2si4 [options ...] <ChessBase database> [destination database]\n");
	printf("\n");
	printf("Options:\n");
	printf("  --            Only file names after this\n");
	printf("  --help        Print Help (this message) and exit\n");
	printf("  --force       Overwrite existing destination files\n");
	printf("  --convertto <encoding>\n");
	printf("                Use this encoding for output database\n");
	printf("                (default is iso8859-1)\n");
	printf("  --convertfrom <encoding>\n");
	printf("                The encoding of the ChessBase database\n");
	printf("                (default is cp1252)\n");
	printf("  --tags <comma-separated-tag-list>\n");
	printf("                Export only the tags given with this list\n");
	printf("                (but mandatory tags are always exported)\n");
	printf("  --unusual-tags\n");
	printf("                Export unusual tags (otherwise ignore these tags)\n");
	printf("  --all-tags\n");
	printf("                Export all tags (overrules option --tags)\n");
	printf("                (but ignore unusual tags except --unusual-tags is given)\n");
	printf("  --no-tags\n");
	printf("                Do not export any tag (except mandatory tags)\n");
	printf("                (overrules --tags, --all-tags, but not --unusual-tags)\n");
	printf("  --list-encodings\n");
	printf("                List all known encodings\n");
	printf("  --list-mandatory-tags\n");
	printf("                List all mandatory tags\n");
	printf("  --list-default-tags\n");
	printf("                List all default tags\n");
	printf("  --list-usual-tags\n");
	printf("                List all usual tags (includes mandatory tags)\n");
	printf("\n");
	exit(rc);
}


static void
printEncodingsAndExit(int rc)
{
	sys::utf8::Codec::EncodingList encodings;
	unsigned n = sys::utf8::Codec::getEncodingList(encodings);

	printf("Existing encodings: ");

	for (unsigned i = 0; i < n; ++i)
	{
		printf(encodings[i].c_str());
		if (i + 1 < n)
			printf(", ");
	}

	printf("\n");
	exit(rc);
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
	for (unsigned i = 0; i < tag::ExtraTag; ++i)
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
	mstl::string tagList;

	for (unsigned i = 0; i < tag::ExtraTag; ++i)
	{
		if (si3::Encoder::skipTag(tag::ID(i)))
		{
			if (!tagList.empty())
				tagList += ',';
			tagList += tag::toName(tag::ID(i));
		}
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


static unsigned
exportGames(Database& src, Consumer& dst, Progress& progress)
{
	unsigned numGames		= src.countGames();

	util::ProgressWatcher watcher(progress, numGames);
	progress.setFrequency(mstl::min(5000u, mstl::max(numGames/100, 1u)));

	unsigned reportAfter	= progress.frequency();
	unsigned count			= 0;
	unsigned countGames	= 0;

	::Log log;

	for (unsigned i = 0; i < numGames; ++i)
	{
		if (reportAfter == count++)
		{
			progress.update(count);
			reportAfter += progress.frequency();
		}

		save::State state = src.exportGame(i, dst);

		if (state == save::Ok)
			++countGames;
		else if (!log.error(state, i))
			return countGames;
	}

	return countGames;
}


int
main(int argc, char* argv[])
{
	TclInterpreter	tclInterpreter;
	mstl::string	convertto("iso8859-1");
	mstl::string	convertfrom("cp1252");
	bool				force(false);
	bool				allTags(false);
	bool				noTags(false);
	bool				defaultTags(true);
	bool				extraTags(false);
	mstl::string	givenTags;

	int i = 1;

	for ( ; i < argc && argv[i][0] == '-'; ++i)
	{
		if (strcmp(argv[i], "--") == 0)
			break;

		if (strcmp(argv[i], "--help") == 0)
		{
			printHelpAndExit(0);
		}
		else if (strcmp(argv[i], "--convertto") == 0)
		{
			if (++i == argc)
				printHelpAndExit(1);
			convertto.assign(argv[i]);

		}
		else if (strcmp(argv[i], "--convertfrom") == 0)
		{
			if (++i == argc)
				printHelpAndExit(1);
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

	if (!sys::utf8::Codec::checkEncoding(convertto))
	{
		fprintf(stderr, "Unknown encoding '%s'.\n\n", convertto.c_str());
		printEncodingsAndExit(1);
	}

	if (!sys::utf8::Codec::checkEncoding(convertfrom))
	{
		fprintf(stderr, "Unknown encoding '%s'.\n\n", convertfrom.c_str());
		printEncodingsAndExit(1);
	}

	if (i == argc)
	{
		fprintf(stderr, "No input database specified.\n\n");
		printHelpAndExit(1);
	}

	mstl::string cbhPath(argv[i++]);

	if (cbhPath.size() < 4 || strcmp(cbhPath.c_str() + cbhPath.size() - 4, ".cbh") != 0)
		cbhPath.append(".cbh");

	mstl::ifstream	stream(cbhPath);

	if (!stream)
	{
		fprintf(stderr, "Cannot open file '%s'.\n", cbhPath.c_str());
		exit(1);
	}

	mstl::string si4Path;

	if (i < argc)
	{
		si4Path.append(argv[i]);
	}
	else
	{
		si4Path.append(cbhPath);
		if (si4Path.size() >= 4 && strcmp(si4Path.c_str() + si4Path.size() - 4, ".cbh") == 0)
			si4Path.erase(si4Path.size() - 4);
	}

	if (si4Path.size() < 4 || strcmp(si4Path.c_str() + si4Path.size() - 4, ".si4") != 0)
		si4Path.append(".si4");
	
	if (!force && access(si4Path.c_str(), R_OK) == 0)
	{
		fprintf(stderr, "Database '%s' already exists.\n", si4Path.c_str());
		exit(1);
	}

	TagBits tagList;

	if (!noTags)
	{
		if (allTags)
			tagList.set();
		else if (defaultTags)
			tagList = getDefaultTags();
		else
			setTagList(tagList, givenTags);
	}

	try
	{
		loadEcoFile();

		Progress	progress;

		Database	src(cbhPath, convertfrom, Database::ReadOnly, progress);
		Database	dst(si4Path, convertto, Database::OnDisk);
		si3::Consumer consumer(	format::Scid4,
										dynamic_cast<si3::Codec&>(dst.codec()),
										convertto,
										tagList,
										extraTags);

		dst.setType(src.type());
		unsigned numGames = exportGames(src, consumer, progress);
		dst.save(progress);
		printf("\n%u game(s) written.\n", numGames);
		if (rejected > 0)
			printf("%u game(s) rejected.\n", rejected);
		fflush(stdout);
		dst.close();
		src.close();
	}
	catch (mstl::exception const& exc)
	{
		fflush(stdout);
		fprintf(stderr, "\n%s\n", exc.what());
		exit(1);
	}

	return 0;
}

// vi:set ts=3 sw=3:
