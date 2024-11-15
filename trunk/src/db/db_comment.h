// ======================================================================
// Author : $Author$
// Version: $Revision: 1080 $
// Date   : $Date: 2015-11-15 10:23:19 +0000 (Sun, 15 Nov 2015) $
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
// Copyright: (C) 2010-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_comment_included
#define _db_comment_included

#include "db_common.h"

#include "u_crc.h"

#include "m_string.h"
#include "m_map.h"

namespace db {

class Comment
{
public:

	struct Callback
	{
		enum Attribute
		{
			Bold,
			Italic,
			Underline,
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
		virtual void emoticon(mstl::string const& s) = 0;

		virtual void invalidXmlContent(mstl::string const& content) = 0;
	};

	enum Mode { PreserveEmoticons, ExpandEmoticons };

	typedef mstl::map<mstl::string,unsigned> LanguageSet;

	Comment();
	Comment(mstl::string const& content, unsigned langFlags);

#if HAVE_OX_EXPLICITLY_DEFAULTED_AND_DELETED_SPECIAL_MEMBER_FUNCTIONS
	Comment(Comment const&) = default;
	Comment& operator=(Comment const&) = default;
#endif

#if HAVE_0X_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR
	Comment(Comment&& comment);
	Comment& operator=(Comment&& comment);
#endif

	operator mstl::string const& () const;

	bool operator==(Comment const& comment) const;
	bool operator!=(Comment const& comment) const;

	bool isEmpty() const;
	bool isXml() const;
	bool containsLanguage(mstl::string const& lang) const;
	bool containsAnyLanguageOf(LanguageSet const& langSet) const;

	unsigned size() const;
	unsigned length() const;
	unsigned langFlags() const;
	mstl::string const& content() const;
	util::crc::checksum_t computeChecksum(util::crc::checksum_t crc) const;
	LanguageSet const& languageSet() const;

	void append(Comment const& comment, char delim = '\0');
	void appendCommonSuffix(mstl::string const& suffix);
	void merge(Comment const& comment, LanguageSet const& leadingLanguageSet);
	void remove(mstl::string const& lang);
	void remove(LanguageSet const& languageSet);
	void strip(LanguageSet const& set);
	void strip(mstl::string const& lang, unsigned langFlags);
	void detectEmoticons();
	bool fromHtml(mstl::string const& s);
	void swap(Comment& comment);
	void swap(mstl::string& content, unsigned langFlags);
	void copy(mstl::string const& fromLang, mstl::string const& toLang, bool stripOriginal = false);
	void normalize(Mode mode = ExpandEmoticons, char delim = '\n');
	void clear();

	void parse(Callback& cb) const;
	void collectLanguages(LanguageSet& result) const;
	void flatten(mstl::string& result, encoding::CharSet encoding, unsigned langFlags) const;
	void toHtml(mstl::string& result) const;

	unsigned countLength(mstl::string const& lang) const;
	unsigned countLength(LanguageSet const& set) const;

	static bool convertCommentToXml(	mstl::string const& comment,
												Comment& result,
												encoding::CharSet encoding);
	static void escapeString(mstl::string const& src, mstl::string& dst);

private:

	bool operator==(mstl::string const& comment) const; // avoid this usage
	bool operator!=(mstl::string const& comment) const; // avoid this usage

	void collect() const;

	mstl::string m_content;

	mutable unsigned		m_langFlags;
	mutable LanguageSet	m_languageSet;
};

} // namespace db

#include "db_comment.ipp"

#endif // _db_comment_included

// vi:set ts=3 sw=3:
