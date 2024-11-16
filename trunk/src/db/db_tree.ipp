// ======================================================================
// Author : $Author$
// Version: $Revision: 1339 $
// Date   : $Date: 2017-07-31 19:09:29 +0000 (Mon, 31 Jul 2017) $
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

#include "db_database.h"

#include "m_assert.h"

namespace db {

inline uint64_t Tree::Key::hash() const						{ return m_hash; }
inline Tree::Position const& Tree::Key::position() const	{ return m_position; }
inline tree::Method Tree::Key::method() const				{ return m_method; }
inline tree::Mode Tree::Key::mode() const						{ return m_mode; }
inline rating::Type Tree::Key::ratingType() const			{ return m_ratingType; }


inline
bool
Tree::Key::operator==(Key const& key) const
{
	return	m_method == key.m_method
			&& m_mode == key.m_mode
			&& m_ratingType == key.m_ratingType
			&& m_hash == key.m_hash
			&& m_position == key.m_position;
}


inline
bool
Tree::Key::operator!=(Key const& key) const
{
	return !operator==(key);
}


inline
bool
Tree::Key::match(	tree::Method method,
						tree::Mode mode,
						rating::Type ratingType,
						uint64_t hash,
						Position const& position) const
{
	return	m_method == method
			&& m_mode == mode
			&& m_ratingType == ratingType
			&& m_hash == hash
			&& m_position == position;
}


inline bool Tree::isEmpty() const							{ return m_infoList.empty(); }
inline bool Tree::isComplete() const						{ return m_complete; }
inline bool Tree::isCompressed() const						{ return m_filter.isCompressed(); }

inline unsigned Tree::size() const							{ return m_infoList.size(); }
inline TreeInfo const& Tree::total() const				{ return m_total; }
inline Database& Tree::database() const					{ return *m_base; }
inline Filter const& Tree::filter() const					{ return m_filter; }
inline unsigned Tree::countGames() const					{ return m_filter.count(); }
inline unsigned Tree::prevGameCount() const				{ return m_prevGameCount; }
inline Tree::Key const& Tree::key() const					{ return m_key; }
inline uint64_t Tree::hash() const							{ return m_key.hash(); }
inline Tree::Position const& Tree::position() const	{ return m_key.position(); }
inline tree::Method Tree::method() const					{ return m_key.method(); }
inline tree::Mode Tree::mode() const						{ return m_key.mode(); }
inline rating::Type Tree::ratingType() const				{ return m_key.ratingType(); }

#ifndef SUPPORT_TREE_INFO_FILTER
inline void Tree::compressFilter()		{ m_filter.compress(); }
inline void Tree::uncompressFilter()	{ m_filter.uncompress(); }
#endif


inline
bool
Tree::match(tree::Method method,
				tree::Mode mode,
				rating::Type ratingType,
				uint64_t hash,
				Position const& position) const
{
	return m_key.match(method, mode, ratingType,  hash, position);
}


inline
TreeInfo const&
Tree::info(unsigned n) const
{
	M_REQUIRE(n < size());
	return m_infoList[n];
}


inline
bool
Tree::isCached(Database const& base,
					Board const& position,
					tree::Method method,
					tree::Mode mode,
					rating::Type ratingType)
{
	return base.treeCache().isCached(position, method, mode, ratingType);
}


inline
Tree*
Tree::lookup(	Database const& base,
					Board const& position,
					tree::Method method,
					tree::Mode mode,
					rating::Type ratingType)
{
	return base.treeCache().lookup(position, method, mode, ratingType);
}


inline
void
Tree::addToCache(Tree* tree)
{
	M_REQUIRE(tree);
	tree->database().treeCache().add(tree);
}


inline
void
Tree::clearCache(Database& base)
{
	base.treeCache().clear();
}


inline
void
Tree::invalidateCache(Database& base, unsigned firstGameIndex, unsigned lastGameIndex)
{
	base.treeCache().setIncomplete(firstGameIndex, lastGameIndex);
}

} // namespace db

// vi:set ts=3 sw=3:
