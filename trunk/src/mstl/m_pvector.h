// ======================================================================
// Author : $Author$
// Version: $Revision: 1453 $
// Date   : $Date: 2017-12-11 14:27:52 +0000 (Mon, 11 Dec 2017) $
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

#ifndef _mstl_pvector_included
#define _mstl_pvector_included

#include "m_vector.h"
#include "m_pointer_iterator.h"
#include "m_iterator.h"

namespace mstl {

template <typename T>
class pvector
{
public:

	typedef T									value_type;
	typedef vector<T*>						vector_type;
	typedef value_type*						pointer;
	typedef value_type const*				const_pointer;
	typedef value_type&						reference;
	typedef value_type const&				const_reference;
	typedef ptrdiff_t							difference_type;
	typedef bits::size_t						size_type;

	typedef pointer_iterator<typename vector_type::iterator>			iterator;
	typedef pointer_const_iterator<typename vector_type::iterator>	const_iterator;

	typedef mstl::reverse_iterator<iterator>			reverse_iterator;
	typedef mstl::const_reverse_iterator<iterator>	const_reverse_iterator;

	pvector();
	explicit pvector(size_type n);
	pvector(size_type n, const_reference v);
	pvector(pvector const& v);
	~pvector() throw();

#if HAVE_C11_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR
	pvector(pvector&& v);
	pvector& operator=(pvector&& v);
#endif

	pvector& operator=(pvector const& v);

	reference operator[](size_type n);
	const_reference operator[](size_type n) const;
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

	void push_back(const_reference v);
	void push_back();
	void pop_back();

	iterator insert(iterator i, const_reference value);

	iterator erase(iterator i);
	iterator erase(reverse_iterator position);

	void reserve(size_type n);
	void reserve_exact(size_type n);
	void resize(size_type n);
	void resize(size_type n, const_reference v);
	void clear();
	void swap(pvector& v);
	void release();

	vector_type const& base() const;
	vector_type& base();	// use with care!

private:

	vector_type m_vec;
};

template <typename T> void swap(pvector<T>& lhs, pvector<T>& rhs);

} // namespace mstl

#include "m_pvector.ipp"

#endif // _mstl_pvector_included

// vi:set ts=3 sw=3:
