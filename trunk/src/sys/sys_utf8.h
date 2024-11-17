// ======================================================================
// Author : $Author$
// Version: $Revision: 1372 $
// Date   : $Date: 2017-08-04 17:56:11 +0000 (Fri, 04 Aug 2017) $
// Url    : $URL$
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

#ifndef _sys_utf8_included
#define _sys_utf8_included

#include "m_types.h"
#include "m_string.h"

namespace sys {
namespace utf8 {

typedef uint16_t uchar;

bool isTail(char c);
bool isFirst(char c);

bool validate(mstl::string const& str);
bool validate(char const* str, unsigned nbytes);

bool isAlpha(uchar uc);
bool isAlnum(uchar uc);
bool isSpace(uchar uc);
bool isPunct(uchar uc);

bool isAlpha(char const* s);
bool isAlnum(char const* s);
bool isSpace(char const* s);
bool isPunct(char const* s);

bool isAscii(char uc);

bool isLower(uchar uc);
bool isUpper(uchar uc);

uchar toLower(uchar uc);
uchar toUpper(uchar uc);

char* toLower(char* s);
char* toUpper(char* s);

bool matchChar(char const* lhs, char const* rhs);
bool caseMatchChar(char const* lhs, char const* rhs);

bool isAscii(mstl::string const& str);

unsigned countChars(mstl::string const& str);
unsigned countChars(char const* str, unsigned byteLength);
unsigned charLength(char const* str);
unsigned charLength(uchar uc);
unsigned byteLength(mstl::string const& str, unsigned numChars);

uchar getChar(char const* str);
uchar getChar(char const* str, unsigned& len);
mstl::string& append(mstl::string& result, uchar uc);
unsigned copy(char* dst, uchar uc);

char const* nextChar(char const* str);
char const* nextChar(char const* str, uchar& code);
char const* prevChar(char const* str, char const* start);
char const* atIndex(char const* str, unsigned n);

char const* skipAlphas(char const* str, char const* end);
char const* skipNonAlphas(char const* str, char const* end);
char const* skipSpaces(char const* str, char const* end);
char const* skipNonSpaces(char const* str, char const* end);

unsigned makeValid(mstl::string& str, mstl::string const& replacement = mstl::string::empty_string);

int compare(mstl::string const& lhs, mstl::string const& rhs);
int casecmp(mstl::string const& lhs, mstl::string const& rhs);

bool caseMatch(mstl::string const& lhs, mstl::string const& rhs, unsigned size);

char const* findChar(char const* s, char const* e, uchar code);
char const* findCharNoCase(char const* s, char const* e, uchar code);

char const* findString(	char const* haystack,
								unsigned haystackLen,
								char const* needle,
								unsigned needleLen);
char const* findStringNoCase(	char const* haystack,
										unsigned haystackLen,
										char const* needle,
										unsigned needleLen);

int findFirst(char const* haystack, unsigned haystackLen, char const* needle, unsigned needleLen);
int findFirstNoCase(char const* haystack, unsigned haystackLen, char const* needle, unsigned needleLen);

unsigned levenshteinDistanceFast(mstl::string const& lhs, mstl::string const& rhs);
unsigned levenshteinDistance(	mstl::string const& lhs,
										mstl::string const& rhs,
										unsigned ins = 2,
										unsigned del = 2,
										unsigned sub = 1);
bool isSimilar(mstl::string const& lhs, mstl::string const& rhs, unsigned threshold = 2);

namespace latin1 {

void map(mstl::string const& name, mstl::string& result);
int compare(char const* lhs, char const* rhs, bool noCase = false, bool skipPunct = false);
int dictionaryCompare(char const* lhs, char const* rhs, bool skipPunct = false);

} // namespace latin1

namespace german {

void map(mstl::string const& name, mstl::string& result);
bool match(mstl::string const& utf8, mstl::string const& ascii, bool noCase);

} // namespace german

namespace ascii {

bool match(mstl::string const& utf8, mstl::string const& ascii, bool noCase);

} // namespace ascii
} // namespace utf8
} // namespace sys

#include "sys_utf8.ipp"

#endif // _sys_utf8_included

// vi:set ts=3 sw=3:
