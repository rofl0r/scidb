// ======================================================================
// Author : $Author$
// Version: $Revision: 969 $
// Date   : $Date: 2013-10-13 15:33:12 +0000 (Sun, 13 Oct 2013) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _mstl_vector_included
#define _mstl_vector_included

#include "m_type_traits.h"
#include "m_memblock.h"
#include "m_pointer_iterator.h"
#include "m_iterator.h"

#include <stddef.h>

namespace mstl {

template <typename T>
class vector : protected memblock<T>
{
public:

	typedef T						value_type;
	typedef vector<value_type>	vector_type;
	typedef value_type*			pointer;
	typedef value_type const*	const_pointer;
	typedef value_type&			reference;
	typedef value_type const&	const_reference;
	typedef ptrdiff_t				difference_type;
	typedef bits::size_t			size_type;

	typedef pointer_iterator<T>			iterator;
	typedef pointer_const_iterator<T>	const_iterator;

	typedef mstl::reverse_iterator<iterator>					reverse_iterator;
	typedef mstl::const_reverse_iterator<const_iterator>	const_reverse_iterator;

	vector();
	explicit vector(size_type n);
	vector(size_type n, const_reference v);
	vector(vector const& v);
	vector(vector const& v, size_type n);
	template <typename Iterator> vector(Iterator first, Iterator last);
	~vector() throw();

#if HAVE_0X_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR
	vector(vector&& v);
	vector& operator=(vector&& v);
#endif

	vector& operator=(vector const& v);

	bool operator==(vector const& v) const;
	bool operator!=(vector const& v) const;

	reference operator[](size_type n);
	const_reference operator[](size_type n) const;
	reference at(size_type n);
	const_reference at(size_type n) const;
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;

	bool empty() const;

	size_type size() const;
	size_type capacity() const;

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	reverse_iterator rbegin();
	reverse_iterator rend();
	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;

	const_pointer data() const;

	void push_back(const_reference v);
	void push_back();
	void push_front(const_reference v);
	void push_front();
	void pop_back();
	void pop_front();

	bool equal(vector_type const& v) const;
	bool equal(vector_type const& v, size_type n) const;

	template <typename Iterator> void assign(Iterator first, Iterator last);

	iterator insert(iterator position, const_reference value);
	void insert(iterator position, size_type n, const_reference value);
	template <typename Iterator>
	void insert(iterator position, Iterator first, Iterator last);

	iterator erase(iterator position);
	iterator erase(reverse_iterator position);
	iterator erase(iterator first, iterator last);
	iterator erase(reverse_iterator first, reverse_iterator last);

	void fill(value_type const& value);

	void reserve(size_type n);
	void reserve_exact(size_type n);
	void resize(size_type n);
	void resize(size_type n, const_reference v);
	void clear();
	void swap(vector& v);
	void release();

private:

	void fill_insert(iterator position, size_type n, const_reference value);
	void insert_aux(iterator position, const_reference value);
};

template <typename T> void swap(vector<T>& lhs, vector<T>& rhs);

} // namespace mstl

#include "m_vector.ipp"

#endif // _mstl_vector_included

// vi:set ts=3 sw=3:
