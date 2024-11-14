// ======================================================================
// Author : $Author$
// Version: $Revision: 719 $
// Date   : $Date: 2013-04-19 16:40:59 +0000 (Fri, 19 Apr 2013) $
// Url    : $URL$
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

#include "m_utility.h"
#include "m_assert.h"

namespace mstl {
namespace bits {

//   +-------------+
//   |             V
// +-+-+  +---+  +---+
// | | |  |   |  |   |
// | * |<-+-* |<-+-* |
// |   |  |   |  |   |
// | *-+->| *-+->| * |
// |   |  |   |  | | |
// +---+  +---+  +-+-+
//   ^             |
//   +-------------+


inline
node_base::node_base()
	:m_next(this)
	,m_prev(this)
{
}


inline
void
node_base::swap(node_base& node)
{
	mstl::swap(m_prev, node.m_prev);
	mstl::swap(m_next, node.m_next);
}


inline
void
node_base::hook(node_base* succ)
{
	M_ASSERT(succ);

	m_next = succ;
	m_prev = succ->m_prev;

	m_prev->m_next = this;
	m_next->m_prev = this;
}


inline
void
node_base::unhook()
{
	M_ASSERT(m_next);
	M_ASSERT(m_prev);

	m_next->m_prev = m_prev;
	m_prev->m_next = m_next;
}

} // namespace bits
} // namespace mstl

// vi:set ts=3 sw=3:
