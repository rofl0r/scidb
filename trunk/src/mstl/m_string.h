// ======================================================================
// Author : $Author$
// Version: $Revision: 1453 $
// Date   : $Date: 2017-12-11 14:27:52 +0000 (Mon, 11 Dec 2017) $
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

#ifndef _mstl_string_included
#define _mstl_string_included

#include "m_types.h"

#include <stdarg.h>

namespace mstl {

class string
{
public:

	typedef char					value_type;
	typedef value_type*			pointer;
	typedef value_type const*	const_pointer;
	typedef pointer				iterator;
	typedef const_pointer		const_iterator;
	typedef value_type			const_reference;
	typedef bits::size_t			size_type;

	class reference
	{
	public:

		operator value_type () const;

		value_type operator=(value_type c);

		template <typename T> value_type operator+=(T v);
		template <typename T> value_type operator-=(T v);
		template <typename T> value_type operator*=(T v);
		template <typename T> value_type operator/=(T v);
		template <typename T> value_type operator^=(T v);
		template <typename T> value_type operator|=(T v);
		template <typename T> value_type operator&=(T v);

	private:

		friend class string;

		reference(string& str, size_type pos);
		value_type& get();

		string&		m_str;
		size_type	m_pos;
	};

	static size_type const npos = size_type(-1);
	static value_type const c_Terminator = 0;
	static string const empty_string;

	string();
	string(const_pointer s);
	string(const_pointer s, size_type len);
	string(const_iterator i1, const_iterator i2);
	string(size_type n, value_type c);
	string(size_type n, mstl::string const& s);
	string(string const& s);
	string(string const& s, size_type len);
	string(string const& s, size_type pos, size_type len);
	explicit string(size_type n);
	~string() throw();

#if HAVE_C11_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR
	string(string&& str);
	string& operator=(string&& str);
#endif

	operator const_pointer () const;

	// NOTE: we have to use 'int' instead of 'size_type', otherwise
	// gcc complains a conflict with 'operator const_pointer () const'.
	value_type operator[](int pos) const;
	reference operator[](int pos);

	value_type operator*() const;

	string& operator=(const_reference c);
	string& operator=(const_pointer s);
	string& operator=(string const& s);

	string& operator+=(const_reference c);
	string& operator+=(const_pointer s);
	string& operator+=(string const& s);

	string operator+(const_reference c) const;
	string operator+(const_pointer s) const;
	string operator+(string const& s) const;

	bool empty() const;
	bool readonly() const;
	bool writable() const;
	bool is_7bit() const;

	size_type size() const;
	size_type capacity() const;

	const_pointer c_str() const;
	const_pointer data() const;
	pointer data();

	value_type at(size_type pos) const;
	reference at(size_type pos);

	value_type front() const;
	value_type& front();
	value_type back() const;
	value_type back(int offset) const;
	value_type& back();
	value_type& back(int offset);

	const_pointer begin() const;
	pointer begin();
	const_pointer end() const;
	pointer end();

	void push_back(const_reference c);
	void pop_back();

	string& append(const_reference c);
	string& append(size_type n, const_reference c);
	string& append(const_iterator i1, const_iterator i2);
	string& append(string const& s, size_type sp = 0, size_type slen = npos);
	string& append(const_pointer s, size_type len);
	string& append(const_pointer s);

	string& assign(size_type n, const_reference c);
	string& assign(size_type n, mstl::string const& s);
	string& assign(const_iterator i1, const_iterator i2);
	string& assign(const_pointer s);
	string& assign(const_pointer s, size_type len);
	string& assign(string const& s, size_type len);
	string& assign(string const& s, size_type pos, size_type len);
	string& assign(string const& s);

	string& insert(size_type ip, const_reference c);
	string& insert(size_type ip, string const& s, size_type sp, size_type slen);
	string& insert(size_type ip, const_pointer s, size_type slen);
	string& insert(size_type ip, const_pointer s);

	iterator insert(iterator ip, const_reference c);
	iterator insert(iterator ip, string const& s, size_type sp, size_type slen);
	iterator insert(iterator ip, const_pointer s, size_type slen);
	iterator insert(iterator ip, const_pointer s);

	string& replace(size_type rp, size_type n, const_reference c);
	string& replace(size_type rp, size_type n, const_pointer s);
	string& replace(size_type rp, size_type n, const_pointer s, size_type len);
	string& replace(size_type rp, size_type n, mstl::string const& s);

	iterator replace(iterator first, iterator last, const_reference c);
	iterator replace(iterator first, iterator last, const_pointer s);
	iterator replace(iterator first, iterator last, const_pointer s, size_type len);
	iterator replace(iterator first, iterator last, mstl::string const& s);

	void swap(string& s);

	void clear();
	void resize(size_type n);
	void reserve(size_type n);
	void reserve_exact(size_type n);
	void set_size(size_type n);
	void make_writable();

	string& tolower(size_type pos = 0, size_type len = npos);
	string& toupper(size_type pos = 0, size_type len = npos);

	iterator erase(const_iterator start, size_type n = 1);
	iterator erase(const_iterator first, const_iterator last);
	string& erase(size_type pos = 0, size_type n = npos);

	void strip();
	void strip(const_reference c);

	string substr(size_type pos = 0, size_type len = npos) const;
	string substr(const_iterator first, const_iterator last) const;

	int format(const_pointer fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
	int vformat(const_pointer fmt, va_list args);

	unsigned appendRomanNumber(unsigned n);
	unsigned appendSmallRomanNumber(unsigned n);
	int toArabic(size_type pos = 0, size_type len = npos) const;

	mstl::string& trim();
	mstl::string& ltrim();
	mstl::string& rtrim();

	size_type find(const_reference c, size_type pos = 0) const;
	size_type find(const_pointer s, size_type pos = 0) const;
	size_type find(string const& s, size_type pos = 0) const;

	size_type rfind(const_reference c, size_type pos = npos) const;

	size_type find_ignore_case(const_pointer s, size_type pos = 0) const;
	size_type find_ignore_case(string const& s, size_type pos = 0) const;

	size_type find_first_of(const_reference c, size_type pos = 0) const;
	size_type find_first_of(const_pointer s, size_type pos = 0) const;
	size_type find_first_of(string const& s, size_type pos = 0) const;

	size_type find_last_of(const_reference c, size_type pos = npos) const;
	size_type find_last_of(const_pointer s, size_type pos = npos) const;
	size_type find_last_of(string const& s, size_type pos = npos) const;

	size_type find_first_not_of(const_reference c, size_type pos = 0) const;
	size_type find_first_not_of(const_pointer s, size_type pos = 0) const;
	size_type find_first_not_of(string const& s, size_type pos = 0) const;

	size_type find_last_not_of(const_reference c, size_type pos = npos) const;
	size_type find_last_not_of(const_pointer s, size_type pos = npos) const;
	size_type find_last_not_of(string const& s, size_type pos = npos) const;

	size_type map(value_type mapping, value_type c, size_type occurrence = size_type(-1));

	bool equal(mstl::string const& s) const;
	bool equal(char const* s) const;
	bool equal(mstl::string const& s, size_type len) const;
	bool equal(char const* s, size_type len) const;

	bool not_equal(mstl::string const& s) const;
	bool not_equal(char const* s) const;
	bool not_equal(mstl::string const& s, size_type len) const;
	bool not_equal(char const* s, size_type len) const;

	bool case_equal(mstl::string const& s) const;
	bool case_equal(char const* s) const;
	bool case_equal(mstl::string const& s, size_type len) const;
	bool case_equal(char const* s, size_type len) const;

	bool not_case_equal(mstl::string const& s) const;
	bool not_case_equal(char const* s) const;
	bool not_case_equal(mstl::string const& s, size_type len) const;
	bool not_case_equal(char const* s, size_type len) const;

	unsigned levenshtein_distance_fast(mstl::string const& s) const;
	unsigned levenshtein_distance(mstl::string const& s,
											unsigned ins = 2,
											unsigned del = 2,
											unsigned sub = 1) const;

	void hook(pointer s);
	void hook(pointer s, size_type slen);
	void hook(string const& str);

	string& unhook();

	static string cast(int8_t  value);
	static string cast(int16_t value);
	static string cast(int32_t value);
	static string cast(int64_t value);

	static string cast(uint8_t  value);
	static string cast(uint16_t value);
	static string cast(uint32_t value);
	static string cast(uint64_t value);

	static bool is_7bit(char const* s);
	static bool is_7bit(char const* s, unsigned length);

	struct __EMPTY__ {};
	string(__EMPTY__); // don't use!

private:

	friend class reference;

	void clone();
	void copy();
	void init(const_pointer s, size_type len);
	unsigned appendRomanNumber(unsigned n, int (*caseconv)(int));

	size_type	m_size;
	size_type	m_capacity;
	pointer		m_data;

	static char const* m_empty;
};

bool operator==(string const& lhs, string const& rhs);
bool operator!=(string const& lhs, string const& rhs);
bool operator<=(string const& lhs, string const& rhs);
bool operator< (string const& lhs, string const& rhs);
bool operator>=(string const& lhs, string const& rhs);
bool operator> (string const& lhs, string const& rhs);

bool operator==(string const& lhs, char const* rhs);
bool operator!=(string const& lhs, char const* rhs);
bool operator<=(string const& lhs, char const* rhs);
bool operator< (string const& lhs, char const* rhs);
bool operator>=(string const& lhs, char const* rhs);
bool operator> (string const& lhs, char const* rhs);

bool operator==(char const* lhs, string const& rhs);
bool operator!=(char const* lhs, string const& rhs);
bool operator<=(char const* lhs, string const& rhs);
bool operator< (char const* lhs, string const& rhs);
bool operator>=(char const* lhs, string const& rhs);
bool operator> (char const* lhs, string const& rhs);

string operator+(char lhs, string const& rhs);
string operator+(char const* lhs, string const& rhs);

void swap(string& lhs, string& rhs);
void swap(string::reference lhs, string::reference rhs);

int compare(string const& lhs, string const& rhs);
int compare(string const& lhs, char const* rhs);
int compare(char const* lhs, string const& rhs);
int compare(char const* lhs, char const* rhs);

int compare(string const& lhs, string const& rhs, string::size_type len);
int compare(string const& lhs, char const* rhs, string::size_type len);
int compare(char const* lhs, string const& rhs, string::size_type len);
int compare(char const* lhs, char const* rhs, string::size_type len);

int case_compare(string const& lhs, string const& rhs);
int case_compare(string const& lhs, char const* rhs);
int case_compare(char const* lhs, string const& rhs);
int case_compare(char const* lhs, char const* rhs);

int case_compare(string const& lhs, string const& rhs, string::size_type len);
int case_compare(string const& lhs, char const* rhs, string::size_type len);
int case_compare(char const* lhs, string const& rhs, string::size_type len);
int case_compare(char const* lhs, char const* rhs, string::size_type len);

template <typename T> struct is_movable;
template <> struct is_movable<string> { enum { value = 1 }; };

} // namespace mstl

#include "m_string.ipp"

#endif // _mstl_string_included

// vi:set ts=3 sw=3:
