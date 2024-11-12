// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
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

#ifndef _mstl_chunk_allocator_included
#define _mstl_chunk_allocator_included

#include "m_stack.h"
#include "m_types.h"

namespace mstl {

template <typename T>
class chunk_allocator
{
public:

	static unsigned const NotFound = unsigned(-1);

	typedef T value_type;

	chunk_allocator(size_t chunk_size = 0);
	~chunk_allocator();

	bool canRelease() const;
	bool canShrink(size_t size) const;
	bool empty() const;

	unsigned lookup(T const* p) const;

	size_t chunk_size() const;
	size_t elems_per_chunk() const;

	void set_zero();
	void clear();

	T* alloc();
	T* alloc(size_t length);

	void shrink(size_t allocatedLength, size_t newLength);
	void release();

private:

	struct chunk
	{
		T* base;
		T* curr;
	};

	typedef stack<chunk> chunk_list;

	chunk_allocator(chunk_allocator const& allocator);
	chunk_allocator& operator=(chunk_allocator const& allocator);

	chunk* new_chunk();

	size_t		m_chunk_size;
	size_t		m_num_elems;
	chunk_list	m_chunk_list;
	bool			m_zero;
};

} // namespace mstl

#include "m_chunk_allocator.ipp"

#endif // _mstl_chunk_allocator_included

// vi:set ts=3 sw=3:
