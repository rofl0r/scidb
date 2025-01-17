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

#ifndef _db_tree_included
#define _db_tree_included

#include "db_tree_info.h"
#include "db_filter.h"
#include "db_eco.h"
#include "db_board.h"
#include "db_line.h"

#include "m_pvector.h"
#include "m_ref_counter.h"
#include "m_ref_counted_ptr.h"

namespace util { class Progress; }
namespace util { class ByteStream; }

namespace db {

class Database;
class Line;
class GameInfo;
class TreeCache;
class Signature;

class Tree : public mstl::ref_counter
{
public:

	typedef mstl::ref_counted_ptr<Tree> TreeP;
	typedef board::ExactZHPosition Position;

	class Key
	{
	public:

		Key();
		Key(	uint64_t hash,
				Position const& position,
				tree::Method method,
				tree::Mode mode,
				rating::Type ratingType);

		bool operator==(Key const& key) const;
		bool operator!=(Key const& key) const;

		bool match(	tree::Method method,
						tree::Mode mode,
						rating::Type ratingType,
						uint64_t hash,
						Position const& position) const;

		uint64_t hash() const;
		Position const& position() const;
		tree::Method method() const;
		tree::Mode mode() const;
		rating::Type ratingType() const;

		void set(tree::Method method,
					tree::Mode mode,
					rating::Type ratingType,
					uint64_t hash,
					Position const& position);
		void clear();

	private:

		uint64_t			m_hash;
		Position			m_position;
		tree::Method	m_method;
		tree::Mode		m_mode;
		rating::Type	m_ratingType;
	};

	bool isEmpty() const;
	bool isComplete() const;
	bool isCompressed() const;
	bool isTreeFor(Database const& base, Key const& key) const;
	bool isTreeFor(Database const& base, Board const& position) const;
	bool isTreeFor(Database const& base,
						Board const& position,
						tree::Method method,
						tree::Mode mode,
						rating::Type ratingType) const;
	bool match(	tree::Method method,
					tree::Mode mode,
					rating::Type ratingType,
					uint64_t hash,
					Position const& position) const;

	unsigned size() const;
	TreeInfo const& info(unsigned n) const;
	TreeInfo const& total() const;
	Database& database() const;
	Filter const& filter() const;
	unsigned countGames() const;
	unsigned prevGameCount() const;
#ifndef NDEBUG
	unsigned numGamesParsed() const;
#endif

	Key const& key() const;
	uint64_t hash() const;
	Position const& position() const;
	tree::Method method() const;
	tree::Mode mode() const;
	rating::Type ratingType() const;

	void sort(attribute::tree::ID column);
	void setIncomplete(unsigned firstIndex, unsigned lastIndex);
	void setIncomplete();

	void compressFilter();
	void uncompressFilter();

	static Tree* makeTree(	TreeP& tree,
									unsigned myIdn,
									Board startPosition,
									Board myPosition,
									Line myLine,
									uint16_t hpsig,
									Database& base,
									tree::Method method,
									tree::Mode mode,
									rating::Type ratingType,
									util::Progress& progress);

	static bool isCached(Database const& base,
								Board const& position,
								tree::Method method,
								tree::Mode mode,
								rating::Type ratingType);
	static Tree* lookup(	Database const& base,
								Board const& position,
								tree::Method method,
								tree::Mode mode,
								rating::Type ratingType);
	static void addToCache(Tree* tree);
	static void clearCache(Database& base);
	static void invalidateCache(Database& base, unsigned firstGameIndex, unsigned lastGameIndex);

private:

	typedef bool (*ReachableFunc)(Signature const&, Signature const&, uint16_t);
	typedef mstl::pvector<TreeInfo> InfoList;

	bool buildTree0(			unsigned myIdn,
									Board const& startPosition,
									Board const& myPosition,
									Line const& myLine,
									uint16_t hpsig,
									Database const& base,
									tree::Method method,
									tree::Mode mode,
									ReachableFunc reachableFunc,
									util::Progress& progress,
									unsigned frequency,
									unsigned numGames);
	bool buildTree518(		unsigned myIdn,
									Board const& startPosition,
									Board const& myPosition,
									Line const& myLine,
									uint16_t hpsig,
									Database const& base,
									tree::Method method,
									tree::Mode mode,
									ReachableFunc reachableFunc,
									util::Progress& progress,
									unsigned frequency,
									unsigned numGames);
	bool buildTree960(		unsigned myIdn,
									Board const& startPosition,
									Board const& myPosition,
									Line const& myLine,
									uint16_t hpsig,
									Database const& base,
									tree::Method method,
									tree::Mode mode,
									ReachableFunc reachableFunc,
									util::Progress& progress,
									unsigned frequency,
									unsigned numGames);
	bool buildTreeStandard(	unsigned myIdn,
									Board const& startPosition,
									Board const& myPosition,
									Line const& myLine,
									uint16_t hpsig,
									Database const& base,
									tree::Method method,
									tree::Mode mode,
									ReachableFunc reachableFunc,
									util::Progress& progress,
									unsigned frequency,
									unsigned numGames);
	bool buildTreeStart(		unsigned myIdn,
									Board const& startPosition,
									Board const& myPosition,
									Line const& myLine,
									uint16_t hpsig,
									Database const& base,
									tree::Method method,
									tree::Mode mode,
									ReachableFunc reachableFunc,
									util::Progress& progress,
									unsigned frequency,
									unsigned numGames);

	void possiblyAdd(	Database const& base,
							GameInfo const& info,
							tree::Mode mode,
							Eco eco,
							Board const& myPosition);
	void add(GameInfo const& info, Eco eco, uint16_t move, Board const& myPosition);

	static bool isCached(TreeCache& cache, uint64_t hash, Position const& position);
	static Tree* lookup(TreeCache& cache, uint64_t hash, Position const& position);

	Database*		m_base;
	variant::Type	m_variant;
	Key				m_key;
	unsigned			m_index;
	unsigned			m_last;
	unsigned			m_prevGameCount;
	bool				m_complete;
	InfoList			m_infoList;
	TreeInfo			m_total;
	Filter			m_filter;

#ifdef SHOW_TREE_INFO
	unsigned			m_numGamesParsed;
#endif
};

} // namespace db

namespace mstl {

template <typename T> struct is_pod;
template <> struct is_pod<db::Tree::Key> { enum { value = is_pod<db::board::ExactZHPosition>::value }; };

} // namespace mstl

#include "db_tree.ipp"

#endif // _db_tree_included

// vi:set ts=3 sw=3:
