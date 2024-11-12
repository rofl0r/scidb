// ======================================================================
// Author : $Author$
// Version: $Revision: 84 $
// Date   : $Date: 2011-07-18 18:02:11 +0000 (Mon, 18 Jul 2011) $
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

#ifndef _TeXt_ListToken_included
#define _TeXt_ListToken_included

#include "T_FinalToken.h"

#include "m_string.h"
#include "m_vector.h"

namespace TeXt {

class Producer;

class ListToken : public FinalToken
{
public:

	ListToken();
	ListToken(TokenP const& token);
	ListToken(unsigned n, TokenP const& token);
	ListToken(Environment& env);
	ListToken(Environment& env, mstl::string const& text);
	template <class Iterator> ListToken(Iterator const& first, Iterator const& last);

	bool isEqualTo(Token const& token) const override;
	bool isEmpty() const override;

	Type type() const override;
	mstl::string name() const override;
	mstl::string meaning() const override;
	mstl::string description(Environment& env) const override;
	Producer* getProducer(TokenP const& self) const;
	Value length() const;
	TokenP performThe(Environment& env) const override;
	void perform(Environment& env) override;
	void traceCommand(Environment& env) const override;

	void append(Token* token);
	void append(TokenP const& token);
	void append(mstl::string const& text);
	void append(Value value);
	void append(Value value1, Value value2);
	void append(Value value1, Value value2, Value value3);
	void append(Value value1, Value value2, Value value3, Value value4);
	void append(Value const* first, Value const* last);

	TokenP front() const;
	TokenP back() const;
	TokenP index(Value n) const;
	void prepend(TokenP const& token);
	void popFront();
	void popBack();
	void reverse();
	void rotate(Value n);
	void flatten();
	Value find(TokenP const& token) const;
	void appendTo(ListToken& list) const;
	void set(Value index, TokenP const& token);
	void bind(Environment& env) override;

	unsigned size() const;
	void resize(unsigned n, TokenP value = TokenP());
	void set(unsigned at, TokenP value);
	void fill(unsigned from, unsigned to, TokenP value);

private:

	typedef mstl::vector<TokenP> TokenList;	// TODO: use deque (or list) instead?!

	class MyProducer;
	friend class MyProducer;

	mstl::string meaning(TokenList::const_iterator breakPoint) const;

	static void flatten(	TokenList::const_iterator first,
								TokenList::const_iterator last,
								TokenList& receiver);

	TokenList m_tokenList;
};

} // namespace TeXt

#include "T_ListToken.ipp"

#endif // _TeXt_ListToken_included

// vi:set ts=3 sw=3:
