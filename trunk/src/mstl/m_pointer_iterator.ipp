// ======================================================================
// Author : $Author$
// Version: $Revision: 1085 $
// Date   : $Date: 2016-02-29 17:11:08 +0000 (Mon, 29 Feb 2016) $
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

namespace mstl {

template <typename T>
inline
pointer_iterator<T>::pointer_iterator(T* elems)
	:m_elems(elems)
{
}


template <typename T>
inline
bool
pointer_iterator<T>::operator==(pointer_iterator const& it) const
{
	return it.m_elems == m_elems;
}


template <typename T>
inline
bool
pointer_iterator<T>::operator!=(pointer_iterator const& it) const
{
	return it.m_elems != m_elems;
}


template <typename T>
inline
pointer_iterator<T>&
pointer_iterator<T>::operator++()
{
	++m_elems;
	return *this;
}


template <typename T>
inline
pointer_iterator<T> const
pointer_iterator<T>::operator++(int)
{
	return pointer_iterator(m_elems++);
}


template <typename T>
inline
pointer_iterator<T>&
pointer_iterator<T>::operator--()
{
	--m_elems;
	return *this;
}


template <typename T>
inline
pointer_iterator<T> const
pointer_iterator<T>::operator--(int)
{
	return pointer_iterator(m_elems--);
}


template <typename T>
inline
pointer_iterator<T>&
pointer_iterator<T>::operator+=(difference_type n)
{
	m_elems += n;
	return *this;
}


template <typename T>
inline
pointer_iterator<T>
pointer_iterator<T>::operator+(difference_type n) const
{
	return pointer_iterator(m_elems + n);
}


template <typename T>
inline
pointer_iterator<T>&
pointer_iterator<T>::operator-=(difference_type n)
{
	m_elems -= n;
	return *this;
}


template <typename T>
inline
pointer_iterator<T>
pointer_iterator<T>::operator-(difference_type n) const
{
	return pointer_iterator(m_elems - n);
}


template <typename T>
inline
typename pointer_iterator<T>::difference_type
pointer_iterator<T>::operator-(pointer_iterator<T> const& i) const
{
	return m_elems > i.m_elems ? m_elems - i.m_elems : i.m_elems - m_elems;
}


template <typename T>
inline
typename pointer_iterator<T>::reference
pointer_iterator<T>::operator[](size_t n) const
{
	return m_elems[n];
}


template <typename T>
inline
typename pointer_iterator<T>::reference
pointer_iterator<T>::operator*() const
{
	return *m_elems;
}


template <typename T>
inline
typename pointer_iterator<T>::pointer
pointer_iterator<T>::operator->() const
{
	return m_elems;
}


template <typename T>
inline
pointer_iterator<T>::operator pointer () const
{
	return m_elems;
}


template <typename T>
inline
void
pointer_iterator<T>::swap(pointer_iterator i)
{
	T* p = *m_elems;
	*m_elems = *i.m_elems;
	*i.m_elems = p;
}


template <typename T>
inline
typename pointer_iterator<T>::pointer&
pointer_iterator<T>::ref()
{
	return m_elems;
}


template <typename T>
inline
pointer_const_iterator<T>::pointer_const_iterator(T const* elems)
	:m_elems(elems)
{
}


template <typename T>
inline
pointer_const_iterator<T>::pointer_const_iterator(iterator const& it)
	:m_elems(it.m_elems)
{
}


template <typename T>
inline
pointer_const_iterator<T>&
pointer_const_iterator<T>::operator=(iterator const& it)
{
	m_elems = it.m_elems;
	return *this;
}


template <typename T>
inline
bool
pointer_const_iterator<T>::operator==(pointer_const_iterator const& it) const
{
	return it.m_elems == m_elems;
}


template <typename T>
inline
bool
pointer_const_iterator<T>::operator!=(pointer_const_iterator const& it) const
{
	return it.m_elems != m_elems;
}


template <typename T>
inline
pointer_const_iterator<T>&
pointer_const_iterator<T>::operator++()
{
	++m_elems;
	return *this;
}


template <typename T>
inline
pointer_const_iterator<T>
pointer_const_iterator<T>::operator++(int)
{
	return pointer_const_iterator(m_elems++);
}


template <typename T>
inline
pointer_const_iterator<T>&
pointer_const_iterator<T>::operator--()
{
	--m_elems;
	return *this;
}


template <typename T>
inline
pointer_const_iterator<T>
pointer_const_iterator<T>::operator--(int)
{
	return pointer_const_iterator(m_elems--);
}


template <typename T>
inline
pointer_const_iterator<T>&
pointer_const_iterator<T>::operator+=(difference_type n)
{
	m_elems += n;
	return *this;
}


template <typename T>
inline
pointer_const_iterator<T>
pointer_const_iterator<T>::operator+(difference_type n) const
{
	return pointer_const_iterator(m_elems + n);
}


template <typename T>
inline
pointer_const_iterator<T>&
pointer_const_iterator<T>::operator-=(difference_type n)
{
	m_elems -= n;
	return *this;
}


template <typename T>
inline
pointer_const_iterator<T>
pointer_const_iterator<T>::operator-(difference_type n) const
{
	return pointer_const_iterator(m_elems - n);
}


template <typename T>
inline
typename pointer_const_iterator<T>::difference_type
pointer_const_iterator<T>::operator-(pointer_const_iterator<T> const& i) const
{
	return m_elems > i.m_elems ? m_elems - i.m_elems : i.m_elems - m_elems;
}


template <typename T>
inline
typename pointer_const_iterator<T>::reference
pointer_const_iterator<T>::operator[](size_t n) const
{
	return m_elems[n];
}


template <typename T>
inline
typename pointer_const_iterator<T>::reference
pointer_const_iterator<T>::operator*() const
{
	return *m_elems;
}


template <typename T>
inline
typename pointer_const_iterator<T>::pointer
pointer_const_iterator<T>::operator->() const
{
	return m_elems;
}


template <typename T>
inline
pointer_const_iterator<T>::operator pointer () const
{
	return m_elems;
}


template <typename T>
inline
typename pointer_const_iterator<T>::pointer
pointer_const_iterator<T>::ref() const
{
	return m_elems;
}


template <typename T>
inline
bool
operator==(pointer_const_iterator<T> const& lhs, pointer_iterator<T> const& rhs)
{
	return lhs.ref() == const_cast<pointer_iterator<T>&>(rhs).ref();
}


template <typename T>
inline
bool
operator!=(pointer_iterator<T> const& lhs, pointer_const_iterator<T> const& rhs)
{
	return const_cast<pointer_iterator<T>&>(lhs).ref() != rhs.ref();
}

} // namespace mstl

// vi:set ts=3 sw=3:
