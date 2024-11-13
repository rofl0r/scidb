// ======================================================================
// Author : $Author$
// Version: $Revision: 452 $
// Date   : $Date: 2012-10-11 09:15:41 +0000 (Thu, 11 Oct 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _mstl_list_node_included
#define _mstl_list_node_included

namespace mstl {
namespace bits {

struct node_base
{
	node_base();

	void hook(node_base* succ);
	void swap(node_base& node);
	void unhook();

	node_base* m_next;
	node_base* m_prev;
};

} // namespace bits
} // namespace mstl

#include "m_list_node.ipp"

#endif // _mstl_list_node_included

// vi:set ts=3 sw=3:
