// ======================================================================
// Author : $Author$
// Version: $Revision: 957 $
// Date   : $Date: 2013-09-30 15:11:24 +0000 (Mon, 30 Sep 2013) $
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
// Copyright: (C) 2012-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "app_multi_cursor.h"
#include "app_cursor.h"
#include "app_application.h"

#include "db_multi_base.h"
#include "db_database.h"

#include "sys_utf8_codec.h"
#include "sys_fam.h"

#include "m_map.h"
#include "m_auto_ptr.h"
#include "m_assert.h"

#include <string.h>

using namespace app;
using namespace db;


namespace {

struct WriteGuard
{
	void release() { m_app.setIsWriting(); }

	WriteGuard(Application& app, Database const& base)
		:m_app(app)
	{
		if (!base.isMemoryOnly())
			m_app.setIsWriting(base.name());
	}

	WriteGuard(Application& app, MultiCursor const& cursor)
		:m_app(app)
	{
		if (!cursor.cursor().isMemoryOnly())
			m_app.setIsWriting(cursor.name());
	}

	~WriteGuard() { release(); }

	Application& m_app;
};

} // namespace



mstl::string MultiCursor::m_clipbaseName("Clipbase");
mstl::string MultiCursor::m_scratchbaseName("Scratchbase");


MultiCursor::MultiCursor(Application& app, Type type)
	:m_app(app)
	,m_base(0)
	,m_leader(0)
	,m_isScratchbase(type == Scratchbase)
	,m_isClipbase(type == Clipbase)
{
	m_base = new MultiBase(	type == Clipbase ? m_clipbaseName : m_scratchbaseName,
									sys::utf8::Codec::utf8(),
									variant::Undetermined,
									storage::MemoryOnly,
									type::Clipbase);

	for (unsigned v = 0; v < variant::NumberOfVariants; ++v)
	{
		M_ASSERT(m_base->database(v));
		m_cursor[v] = new Cursor(*this, m_base->database(v));
	}

	m_leader = m_cursor[variant::Index_Normal];
}


MultiCursor::MultiCursor(Application& app, MultiBase*	base)
	:m_app(app)
	,m_base(base)
	,m_leader(0)
	,m_isScratchbase(false)
	,m_isClipbase(false)
{
	M_REQUIRE(base);

	::memset(m_cursor, 0, sizeof(m_cursor));

	if (base->isSingleBase())
	{
		m_cursor[variant::toIndex(m_base->variant())] = m_leader = new Cursor(*this, m_base->database());
	}
	else
	{
		for (unsigned v = 0; v < variant::NumberOfVariants; ++v)
		{
			M_ASSERT(m_base->database(v));
			m_cursor[v] = new Cursor(*this, m_base->database(v));
		}

		m_leader = m_cursor[variant::Index_Normal];
	}
}


MultiCursor::MultiCursor(	Application& app,
									mstl::string const& name,
									db::type::ID type,
									db::Producer& producer,
									util::Progress& progress)
	:m_app(app)
	,m_base(0)
	,m_leader(0)
	,m_isScratchbase(false)
	,m_isClipbase(false)
{
	::memset(m_cursor, 0, sizeof(m_cursor));

	mstl::auto_ptr<MultiBase> base(new MultiBase(name,
																sys::utf8::Codec::utf8(),
																variant::Undetermined,
																storage::MemoryOnly,
																type));

	WriteGuard guard(m_app, *base->database());
	unsigned n = base->importGames(producer, progress);
	if (n)
		base->save(progress);
	base->resetInitialSize();
	guard.release();

	m_base = base.release();

	if (n)
	{
		for (unsigned v = 0; v < variant::NumberOfVariants; ++v)
		{
			if (!m_base->isEmpty(v))
			{
				(m_cursor[v] = new Cursor(*this, m_base->database(v)))->database().setReadonly();
				if (m_leader == 0)
					m_leader = m_cursor[v];
			}
		}
	}
	else
	{
		m_leader = m_cursor[variant::Index_Normal] =
			new Cursor(*this, m_base->database(variant::Index_Normal));
		m_leader->database().setReadonly();
	}
}


MultiCursor::~MultiCursor()
{
	for (unsigned v = 0; v < variant::NumberOfVariants; ++v)
		delete m_cursor[v];

	delete m_base;
}


bool
MultiCursor::isEmpty(unsigned variantIndex) const
{
	M_REQUIRE(isOpen());
	return m_base->isEmpty(variantIndex);
}


bool
MultiCursor::isEmpty() const
{
	M_REQUIRE(isOpen());

	for (unsigned v = 0; v < variant::NumberOfVariants; ++v)
	{
		if (!m_base->isEmpty(v))
			return false;
	}

	return true;
}


bool
MultiCursor::isSingleBase() const
{
	return m_base->isSingleBase();
}


bool
MultiCursor::isUnsaved() const
{
	return m_base->isUnsaved();
}


unsigned
MultiCursor::countGames() const
{
	M_REQUIRE(isOpen());
	return m_base->countGames();
}


bool
MultiCursor::setReadonly(bool flag)
{
	return m_leader->base().setReadonly(flag);
}


void
MultiCursor::close()
{
	if (isOpen())
	{
		for (unsigned v = 0; v < variant::NumberOfVariants; ++v)
		{
			if (m_cursor[v])
				m_cursor[v]->close();
		}

		m_base->close();
	}
}


unsigned
MultiCursor::copyGames(	MultiCursor& destination,
								GameCount& accepted,
								GameCount& rejected,
								TagBits const& allowedTags,
								bool allowExtraTags,
								unsigned& illegalRejected,
								db::Log& log,
								util::Progress& progress) const
{
	unsigned total = 0;

	WriteGuard guard(m_app, destination);

	for (unsigned v = 0; v < ::db::variant::NumberOfVariants; ++v)
	{
		if (m_cursor[v] && m_cursor[v]->count(table::Games) > 0)
		{
			unsigned count = 0;

			if (destination.m_cursor[v])
			{
				Cursor& cursor = *m_cursor[v];

				count = cursor.base().copyGames(
					destination.m_cursor[v]->database(),
					cursor.view(0).filter(table::Games),
					cursor.view(0).selector(table::Games),
					allowedTags,
					allowExtraTags,
					illegalRejected,
					log,
					progress);

				accepted[v] += count;
				total += count;
			}

			rejected[v] += m_cursor[v]->count(table::Games) - count;
		}
	}

	return total;
}


void
MultiCursor::changeVariant(::db::variant::Type variant)
{
	M_REQUIRE(isOpen());
	M_REQUIRE(isEmpty());
	M_REQUIRE(!isScratchbase());
	M_REQUIRE(!isClipbase());
	M_REQUIRE(!isReadonly());
	M_REQUIRE(isWritable());

	if (m_leader->variant() != variant)
	{
		m_base->changeVariant(variant);

		if (m_base->isSingleBase())
		{
			for (unsigned v = 0; v < variant::NumberOfVariants; ++v)
				delete m_cursor[v];
			::memset(m_cursor, 0, sizeof(m_cursor));
			m_cursor[variant::toIndex(variant)] = new Cursor(*this, m_base->database());
		}

		M_ASSERT(m_cursor[variant::toIndex(variant)]);

		m_leader = m_cursor[variant::toIndex(variant)];
	}
}


void
MultiCursor::replace(db::Database* database)
{
	m_base->replace(database);
}


void
MultiCursor::famChanged()
{
	// TODO
}


void
MultiCursor::famDeleted()
{
	// TODO
}

// vi:set ts=3 sw=3:
