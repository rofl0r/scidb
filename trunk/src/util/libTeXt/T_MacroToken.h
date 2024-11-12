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

#ifndef _TeXt_MacroToken_included
#define _TeXt_MacroToken_included

#include "T_ExpandableToken.h"

#include "m_ref_counted_ptr.h"
#include "m_ref_counter.h"
#include "m_vector.h"

namespace TeXt {

class Producer;


class MacroToken : public ExpandableToken
{
public:

	typedef mstl::vector<TokenP> TokenList;

	MacroToken(	mstl::string const& name,
					TokenList const& paramList,
					TokenP const& body,
					int nestingLevel);

	bool isEqualTo(Token const& token) const override;
	bool isEmpty() const override;

	Type type() const override;
	mstl::string name() const override;
	mstl::string meaning() const override;
	Value value() const override;

	mstl::string parameterDescription() const;
	Producer* getProducer() const;

	void traceCommand(Environment& env) const override;
	void perform(Environment& env) override;

private:

	struct Data : protected mstl::ref_counter
	{
		Data(TokenList const& paramList, TokenP const& body, int nestingLevel);

		TokenList	m_parameters;
		TokenP		m_body;
		unsigned		m_nestingLevel;

		friend class mstl::ref_counted_traits<Data>;
	};

	typedef mstl::ref_counted_ptr<Data> DataP;
	typedef mstl::vector<bool> ListMarkers;

	MacroToken(mstl::string const& name, DataP const& data);

	void bindParameter(	Environment& env,
								TokenP const& parameter,
								TokenList::iterator first,
								TokenList::iterator last,
								ListMarkers::iterator firstMarker,
								ListMarkers::iterator lastMarker);

	mstl::string	m_ident;
	DataP				m_data;
};

} // namespace TeXt

#endif // _TeXt_MacroToken_included

// vi:set ts=3 sw=3:
