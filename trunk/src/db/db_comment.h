// ======================================================================
// Author : $Author$
// Version: $Revision: 25 $
// Date   : $Date: 2011-05-19 14:05:57 +0000 (Thu, 19 May 2011) $
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
// Copyright: (C) 2010-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_comment_included
#define _db_comment_included

#include "m_string.h"
#include "m_map.h"

namespace db {

class Comment
{
public:

	enum Encoding { Unicode, Latin1 };

	struct Callback
	{
		enum Attribute
		{
			Bold			= 'b',
			Italic		= 'i',
			Underline	= 'u',
		};

		virtual ~Callback() throw();

		virtual void start() = 0;
		virtual void finish() = 0;

		virtual void startLanguage(mstl::string const& lang) = 0;
		virtual void endLanguage(mstl::string const& lang) = 0;

		virtual void startAttribute(Attribute attr) = 0;
		virtual void endAttribute(Attribute attr) = 0;

		virtual void content(mstl::string const& s) = 0;
		virtual void nag(mstl::string const& s) = 0;
		virtual void symbol(char s) = 0;

		virtual void invalidXmlContent(mstl::string const& content) = 0;
	};

	typedef mstl::map<mstl::string,unsigned> LanguageSet;

	Comment();
	Comment(mstl::string const& content);

	operator mstl::string const& () const;

	bool operator==(Comment const& comment) const;
	bool operator!=(Comment const& comment) const;

	bool isEmpty() const;
	bool isXml() const;
	bool containsLanguage(mstl::string const& lang) const;

	unsigned size() const;
	mstl::string const& content() const;

	mstl::string& content();

	void remove(mstl::string const& lang);
	void strip(LanguageSet const& set);
	void setContent(mstl::string const& s);
	bool fromHtml(mstl::string const& s);
	void swap(Comment& comment);
	void swap(mstl::string& content);
	void normalize();
	void clear();

	void parse(Callback& cb) const;
	void collectLanguages(LanguageSet& result) const;
	void flatten(mstl::string& result, Encoding encoding) const;
	void toHtml(mstl::string& result) const;

	unsigned countLength(mstl::string const& lang) const;
	unsigned countLength(LanguageSet const& set) const;

	static bool convertCommentToXml(mstl::string const& comment, mstl::string& result, Encoding encoding);

private:

	void collect() const;

	mstl::string m_content;

	mutable LanguageSet m_languageSet;
};

} // namespace db

#include "db_comment.ipp"

#endif // _db_comment_included

// vi:set ts=3 sw=3:
