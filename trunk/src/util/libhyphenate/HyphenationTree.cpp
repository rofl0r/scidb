// ======================================================================
// Author : $Author$
// Version: $Revision: 224 $
// Date   : $Date: 2012-01-31 21:02:29 +0000 (Tue, 31 Jan 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2012 Gregor Cramer
// ======================================================================

// ======================================================================
// libhyphenate: A TeX-like hyphenation algorithm.
// Copyright (C) 2007 Steve Wolter
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// If you have any questions, feel free to contact me:
// http://swolter.sdf1.org
// ======================================================================

// ------------- Implementation for HyphenationTree.h ----------------

#include "HyphenationTree.h"

#include "sys_utf8.h"

#include "m_auto_ptr.h"
#include "m_utility.h"
#include "m_istream.h"

#include <ctype.h>
#include <string.h>

using namespace Hyphenate;


// The HyphenationNode is a tree node for the hyphenation search tree. It
// represents the matching state after a single character; if there is a
// pattern that ends with that particular character, the hyphenation_pattern
// is set to non-NULL. The jump_table links to the children of that node,
// indexed by letters.
class Hyphenate::HyphenationNode
{
public:

	typedef HyphenationNode* JumpTable[256];

	// Table of children
	JumpTable jump_table;
	// Hyphenation pattern associated with the full path to this node.
	mstl::auto_ptr<HyphenationRule> hyphenation_pattern;

	inline HyphenationNode()
	{
		::memset(jump_table, 0, sizeof(jump_table));
	}

	~HyphenationNode()
	{
		// The destructor has to destroy all childrens.
		for (unsigned i = 0; i < 256; ++i)
			delete jump_table[i];
	}

	/// Find a particular jump table entry, or NULL if there is none for that letter.
	inline HyphenationNode const* find(unsigned char arg) const
	{
		return jump_table[arg];
	}

	/// Find a particular jump table entry, or NULL if there is none 
	/// for that letter.
	inline HyphenationNode* find(unsigned char arg)
	{
		return jump_table[arg];
	}

	/// Insert a particular hyphenation pattern into this 
	/// hyphenation subtree.
	/// \param pattern The character pattern to match in the input word.
	/// \param hp The digit-pattern for the hyphenation algorithm.
	void insert(char const* id, mstl::auto_ptr<HyphenationRule> pattern);

	/// Apply all patterns for that subtree.
	void apply_patterns(
		char* priority_buffer, 
		HyphenationRule const** rule_buffer, 
		char const* to_match) const;
};


Hyphenate::HyphenationTree::HyphenationTree()
	:root(new HyphenationNode())
	,start_safe(1)
	,end_safe(1)
{
}


Hyphenate::HyphenationTree::~HyphenationTree()
{
	delete root;
}


void
Hyphenate::HyphenationTree::insert(mstl::auto_ptr<HyphenationRule> pattern)
{
	// Convert our key to lower case to ease matching.
	char const* upperCaseKey = pattern->getKey().c_str();
	char const* e = upperCaseKey + pattern->getKey().size();

	mstl::string lowerCaseKey;

	while (upperCaseKey < e)
	{
		sys::utf8::uchar code;
		upperCaseKey = sys::utf8::nextChar(upperCaseKey, code);
		lowerCaseKey += sys::utf8::toLower(code);
	}

	root->insert(lowerCaseKey, pattern);
}


void
HyphenationNode::insert(char const* key_string, mstl::auto_ptr<HyphenationRule> pattern) 
{
	// Is this the terminal node for that pattern?
	if (key_string[0] == 0)
	{
		// If we descended the tree all the way to the last letter, we can now
		// write the pattern into this node.
		hyphenation_pattern.reset(pattern.release());
	}
	else
	{
		// If not, however, we make sure that the branch for our letter exists and descend.
		char key = key_string[0];

		// Ensure presence of a branch for that letter.
		HyphenationNode* p = find(key);

		if (!p)
			jump_table[static_cast<unsigned char>(key)] = new HyphenationNode();

		// Go to the next letter and descend.
		p->insert(key_string + 1, pattern);
	}
}


void
Hyphenate::HyphenationNode::apply_patterns(
	char* priority_buffer, 
	HyphenationRule const** rule_buffer, 
	char const* to_match) const
{
	// First of all, if we can descend further into the tree (that is,
	// there is an input char left and there is a branch in the tree),
	// do so.
	char key = to_match[0];

	if (key != 0)
	{
		if (HyphenationNode const* next = find(key))
			next->apply_patterns(priority_buffer, rule_buffer, to_match + 1);
	}

	// Now, if we have a pattern at this point in the tree, it must be a good
	// match. Apply the pattern.
	HyphenationRule const* hyp_pat = hyphenation_pattern.get();

	if (hyp_pat != nullptr)
	{
		for (int i = 0; hyp_pat->hasPriority(i); ++i)
		{
			if (priority_buffer[i] < hyp_pat->priority(i))
			{
				rule_buffer[i] = mstl::is_odd(hyp_pat->priority(i)) ? hyp_pat : nullptr;
				priority_buffer[i] = hyp_pat->priority(i);
			}
		}
	}
}


mstl::auto_ptr<mstl::vector<const HyphenationRule*> >
HyphenationTree::applyPatterns(mstl::string const& word) const
{
	return applyPatterns(word, mstl::string::npos);
}


mstl::auto_ptr<mstl::vector<const HyphenationRule*> >
HyphenationTree::applyPatterns(mstl::string const& word, size_t stop_at) const
{
	// Prepend and append a . to the string (word start and end), and convert
	// all characters to lower case to ease matching.
	mstl::string w = ".";
	{
		char const *s = word.begin();
		char const *e = word.end();

		while (s < e)
		{
			sys::utf8::uchar code;
			s = sys::utf8::nextChar(s, code);
			w += sys::utf8::toLower(code);
		}
	}
	w += ".";

	// Vectors for priorities and rules.
	mstl::vector<char> pri(w.size() + 2, 0);
	mstl::vector<const HyphenationRule*> rules(w.size() + 1, nullptr);

	// For each suffix of the expanded word, search all matching prefixes.
	// That way, each possible match is found. Note the pointer arithmetics
	// in the first and second argument.
	for (unsigned i = 0; i < w.size()-1 && i <= stop_at; ++i)
		root->apply_patterns((&pri[i]), &rules[i], w.c_str() + i);

	// Copy the results to a shorter vector.
	mstl::auto_ptr<mstl::vector<const HyphenationRule*> > output_rules(
		new mstl::vector<const HyphenationRule*>(word.size(), nullptr));

	// We honor the safe areas at the start and end of each word here.
	// Please note that the incongruence between start and end is due
	// to the fact that hyphenation happens _before_ each character.
	unsigned ind_start	= 1;
	unsigned ind_end		= w.size() - 1;

	for (unsigned skip = 0; skip < start_safe && ind_start < w.size(); ++ind_start)
	{
		if (sys::utf8::isFirst(w[ind_start]))
			++skip;
	}
	for (unsigned skip = 0; skip < end_safe && ind_end > 0; --ind_end)
	{
		if (sys::utf8::isFirst(w[ind_end]))
			++skip;
	}

	for (unsigned i = ind_start; i <= ind_end; ++i)
		(*output_rules)[i-1] = rules[i];

	return output_rules;
}


void
HyphenationTree::loadPatterns(mstl::istream &i)
{
	mstl::string pattern;

	// The input is a file with whitespace-separated words.
	// The first numerical-only word we encountered denotes the safe start,
	// the second the safe end area.

	char ch;
	bool numeric = true;
	int num_field = 0;

	while (i.get(ch))
	{
		if (::isspace(ch))
		{
			// The output operation.
			if (pattern.size() && numeric && num_field <= 1)
			{
				((num_field == 0) ? start_safe : end_safe) = ::atoi(pattern.c_str());
				++num_field;
			}
			else if (pattern.size())
			{
				M_ASSERT(sys::utf8::validate(pattern));
				insert(mstl::auto_ptr<HyphenationRule>(new HyphenationRule(pattern)));
			}

			// Reinitialize state.
			pattern.clear();
			numeric = true;
		}
		else
		{
			// This rule catches all other (mostly alpha, but probably UTF-8)
			// characters. It normalizes the previous letter and then appends
			// it to the pattern.
			pattern += ch;
			if (!::isdigit(ch))
				numeric = false;
		}
	}

	if (pattern.size()) 
		insert(mstl::auto_ptr<HyphenationRule>(new HyphenationRule(pattern)));
}

// vi:set ts=3 sw=3:
