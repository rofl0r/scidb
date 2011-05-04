// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
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

#ifndef _TeXtController_included
#define _TeXtController_included

#include "T_Receptacle.h"
#include "T_TokenP.h"

#include "m_scoped_ptr.h"
#include "m_ref_counted_ptr.h"
#include "m_ref_counter.h"
#include "m_string.h"

namespace TeXt { class Environment; }
namespace TeXt { class Package; }
namespace mstl { class ifstream; }
namespace mstl { class istream; }
namespace mstl { class ostream; }

namespace TeXt {

class Controller
{
public:

	enum ErrorMode
	{
		ErrorStopMode,
		ScrollMode,
		NonStopMode,
		AbortMode,
		BatchMode,
	};

	struct Log : public mstl::ref_counter
	{
		virtual ~Log() throw();
		virtual void error(mstl::string const& msg) = 0;
	};

	typedef mstl::ref_counted_ptr<Log> LogP;

	static unsigned const UseLog		= 1 << 0;
	static unsigned const UseConsole	= 1 << 1;

	static int const OpenInputFileFailed	= -1;
	static int const OpenOutputFileFailed	= -2;
	static int const OpenLogFileFailed		= -3;

	typedef mstl::ref_counted_ptr<Package> PackageP;

	Controller(	mstl::string const& searchDirs = mstl::string::empty_string,
					ErrorMode errorMode = AbortMode,
					LogP log = LogP());
	~Controller();

	Receptacle& receptacle();

	void addPackage(PackageP package);

	int processInput(	mstl::istream& inp,
							mstl::ostream& dst,
							mstl::ostream* out = 0,
							mstl::ostream* log = 0);
	int processInput(	mstl::string const& inputPath,
							mstl::ostream& dst,
							mstl::ostream* out = 0,
							mstl::ostream* log = 0);
	int processInput(	mstl::string const& inputPath,
							mstl::string const& outputPath,
							unsigned flags = 0);

private:

	typedef mstl::scoped_ptr<Receptacle>	ReceptacleP;
	typedef mstl::scoped_ptr<Environment>	EnvironmentP;

	int processInput(	mstl::istream& inp,
							mstl::ostream& dst,
							mstl::string const& pathName,
							unsigned flags,
							mstl::string const& logHdr);
	mstl::string setupStream(mstl::string const& inputPath, mstl::ifstream& inStream);

	void addPackage(Package* package);
	void addPackages();

	EnvironmentP	m_env;
	ReceptacleP		m_receptacle;
	LogP				m_log;
	bool				m_continued;
};

} // namespace TeXt

#endif // _TeXtController_included

// vi:set ts=3 sw=3:
