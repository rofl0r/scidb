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

#include "db_tree_cache.h"
#include "db_tree.h"

using namespace db;

#ifndef NDEBUG

# include <stdlib.h>

static bool const useCache = getenv("SCIDB_NO_CACHE") == 0;

#endif

TreeCache::TreeCache() : m_inUse(0), m_mostRecentIndex(CacheSize - 1), m_lastIndex(0) {}


TreeCache::~TreeCache()
{
	for (unsigned i = 0; i < m_inUse; ++i)
	{
		if (m_cache[i]->release())
			delete m_cache[i];
	}
}


Tree*
TreeCache::lookup(uint64_t hash,
						Position const& position,
						tree::Method method,
						tree::Mode mode,
						rating::Type ratingType) const
{
	M_ASSERT(m_lastIndex <= m_inUse);

	for (unsigned i = m_lastIndex; i < m_inUse; ++i)
	{
		Tree* t = m_cache[i];

		if (t->match(method, mode, ratingType, hash, position))
		{
			m_lastIndex = i;
			return t;
		}
	}

	for (unsigned i = 0; i < m_lastIndex; ++i)
	{
		Tree* t = m_cache[i];

		if (t->match(method, mode, ratingType, hash, position))
		{
			m_lastIndex = i;
			return t;
		}
	}

	return 0;
}


void
TreeCache::add(Tree* tree)
{
#ifndef NDEBUG

	if (!::useCache)
		return;

#endif

	if (isCached(tree->hash(), tree->position(), tree->method(), tree->mode(), tree->ratingType()))
		return;

	if (++m_mostRecentIndex == CacheSize)
		m_mostRecentIndex = 0;

	if (m_mostRecentIndex >= m_inUse)
		++m_inUse;
	else if (m_cache[m_mostRecentIndex]->release())
		delete m_cache[m_mostRecentIndex];

	tree->ref();
	m_cache[m_lastIndex = m_mostRecentIndex] = tree;
}


void
TreeCache::clear()
{
	for (unsigned i = 0; i < m_inUse; ++i)
	{
		Tree* tree = m_cache[i];

		if (tree->release())
			delete tree;
	}

	m_inUse = 0;
	m_lastIndex = 0;
	m_mostRecentIndex = CacheSize - 1;
}


void
TreeCache::clear(tree::Mode mode)
{
	unsigned index = 0;

	for (unsigned i = 0; i < m_inUse; ++i)
	{
		Tree* tree = m_cache[i];

		if (tree->mode() != mode)
			m_cache[index++] = tree;
		else if (tree->release())
			delete tree;
	}

	m_inUse = index;
	m_lastIndex = 0;
	m_mostRecentIndex = 0;
}


void
TreeCache::setIncomplete()
{
	for (unsigned i = 0; i < m_inUse; ++i)
		m_cache[i]->setIncomplete();
}


void
TreeCache::setIncomplete(unsigned firstIndex, unsigned lastIndex)
{
	for (unsigned i = 0; i < m_inUse; ++i)
		m_cache[i]->setIncomplete(firstIndex, lastIndex);
}

// vi:set ts=3 sw=3:
