// ======================================================================
// Author : $Author$
// Version: $Revision: 985 $
// Date   : $Date: 2013-10-29 14:52:42 +0000 (Tue, 29 Oct 2013) $
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

#ifndef _db_exception_included
#define _db_exception_included

#include "u_exception.h"

#define DB_RAISE(fmt, args...) M_THROW(::db::Exception(fmt, ##args))

#define IO_RAISE(file, error, fmt, args...) \
	M_THROW(::db::IOException(::db::IOException::file, ::db::IOException::error, fmt, ##args))
#define IO_RAISE_BT(file, error, backtrace, fmt, args...) \
	M_THROW(::db::IOException(::db::IOException::file, ::db::IOException::error, backtrace, fmt, ##args))


namespace db {

class Exception : public util::Exception
{
public:

	Exception();
	explicit Exception(char const* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
	Exception(char const* fmt, va_list args);
	Exception(util::Exception& exc);
	~Exception() throw();
};


class IOException : public Exception
{
public:

	enum FileType
	{
		Unspecified,
		Index,
		Game,
		Namebase,
		Annotation,
		PgnFile,
		BookFile,
	};

	enum ErrorType
	{
		Unknown_Error_Type,
		Create_Failed,
		Open_Failed,
		Read_Only,
		Unknown_Version,
		Unexpected_Version,
		Corrupted,
		Invalid_Data,
		Write_Failed,
		Read_Error,
		Encoding_Failed,
		Max_File_Size_Exceeded,
		Load_Failed,
		Not_Original_Version,
		Cannot_Create_Thread,
	};

	IOException(FileType fileType,
					ErrorType errorType,
					char const* fmt, ...) __attribute__((__format__(__printf__, 4, 5)));
	IOException(FileType fileType,
					ErrorType errorType,
					mstl::backtrace const& backtrace,
					char const* fmt, ...) __attribute__((__format__(__printf__, 5, 6)));

	FileType fileType() const;
	ErrorType errorType() const;

private:

	FileType		m_fileType;
	ErrorType	m_errorType;
};


class DecodingFailedException : public Exception
{
public:

	DecodingFailedException(char const* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
};

} // namespace db

#endif // _db_exception_included

// vi:set ts=3 sw=3:
