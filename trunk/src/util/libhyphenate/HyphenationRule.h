// ======================================================================
// Author : $Author$
// Version: $Revision: 226 $
// Date   : $Date: 2012-02-05 22:00:47 +0000 (Sun, 05 Feb 2012) $
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

#ifndef _hyphenate_hyphenation_rule_h
#define _hyphenate_hyphenation_rule_h

#include "m_string.h"
#include "m_vector.h"

namespace hyphenate
{
	/// The HyphenationRule class represents a single Hyphenation Rule, that
	/// is, a pattern that has a number assigned to each letter and will,
	/// if applied, hyphenate a word at the given point. The number assigned
	/// to each letter and accessed by priority() is odd when hyphenation
	/// should occur before the letter, and only the rule with the highest
	/// number will be applied to any letter.

	class HyphenationRule
	{
	private:

		int del_pre, skip_post;
		mstl::string key, insert_pre, insert_post;
		mstl::vector<char> priorities;

		mstl::string replacement;

	public:

		// HyphenationRule is constructed from a string consisting of
		// letters with numbers strewn in. The numbers are the priorities.
		// In addition, a / will start a non-standard hyphenization.
		HyphenationRule(mstl::string const& source_string);

		/// Call this method once an hyphen would, according to its base rule,
		///  be placed. Returns the number of bytes that should not be  
		///  printed afterwards.
		///
		///  For example, when applying the rules to "example", you should
		///  call the rules returned by HyphenationTree or Hyphenator as
		///  follows:
		///  string word = "ex";
		///  rule1.apply(word, "-");
		///  word += "am" ;
		///  rule2.apply(word, "-");
		///  word += "ple";
		///
		///  Watch out for non-standard rules, though. Example: "Schiffahrt"
		///  string word = "Schif";
		///  int skip = rule1.apply(word, "-");
		///  char *rest = "fahrt";
		///  word += rest+skip;
		int apply(mstl::string& word, mstl::string const& hyphen) const;

		/// Only apply the first part, that is, up to and including the hyphen.
		void apply_first(mstl::string& word, mstl::string const& hyphen) const;

		/// Only apply the second part, after the hyphen.
		int apply_second(mstl::string& word) const;

		/// Returns true iff there is a priority value != 0 for this offset or a larger one.
		inline bool hasPriority(unsigned offset) const { return priorities.size() > offset; }

		/// Returns the hyphenation priority for a hyphen preceding the byte
		/// at the given offset.
		inline char priority(unsigned offset) const { return priorities[offset]; }

		/// Returns the pattern to match for this rule to apply.
		inline mstl::string& getKey() { return key; }

		/// Returns the amount of bytes that will additionally be needed
		/// in front of the hyphen if this rule is applied. 0 for standard
		/// hyphenation, 1 for Schiff-fahrt.
		inline int spaceNeededPreHyphen() const { return insert_pre.size() - del_pre; }

		/// Returns true iff this rule is not a standard hyphenation rule.
		inline bool isNonStandard() const
		{
			return del_pre != 0 || skip_post != 0 || !insert_pre.empty() || !insert_post.empty();
		}
	};
}

#endif // _hyphenate_hyphenation_rule_h

// vi:set ts=3 sw=3:
