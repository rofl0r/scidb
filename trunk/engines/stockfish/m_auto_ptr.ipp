// ======================================================================
// Author : $Author$
// Version: $Revision: 526 $
// Date   : $Date: 2012-11-13 13:26:07 +0000 (Tue, 13 Nov 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2012 Gregor Cramer
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
typename auto_ptr<T>::pointer
auto_ptr<T>::release()
{
	T* p = m_p;
	m_p = 0;
	return p;
}


template <typename T> inline void auto_ptr<T>::swap(auto_ptr<T>& ap) { mstl::swap(m_p, ap.m_p); }
template <typename T> inline void swap(auto_ptr<T>& lhs, auto_ptr<T>& rhs) { lhs.swap(rhs); }

template <typename T> inline auto_ptr<T>::by_ref::by_ref(pointer p) :m_p(p) {}
template <typename T> inline auto_ptr<T>::auto_ptr(pointer p) : m_p(p) {}
template <typename T> inline auto_ptr<T>::auto_ptr(auto_ptr<T>& ap) : m_p(ap.release()) {}
template <typename T> inline auto_ptr<T>::auto_ptr(by_ref ref) : m_p(ref.m_p) {}
template <typename T> inline auto_ptr<T>::~auto_ptr() { delete m_p; }
template <typename T> inline bool auto_ptr<T>::operator!() const { return m_p == 0; }
template <typename T> inline typename auto_ptr<T>::pointer auto_ptr<T>::get() { return m_p; }
template <typename T> inline typename auto_ptr<T>::const_pointer auto_ptr<T>::get() const { return m_p; }
template <typename T> inline auto_ptr<T>::operator void const* () const { return m_p; }


template <typename T>
inline
void
auto_ptr<T>::reset(pointer p)
{
	if (m_p != p)
	{
		delete m_p;
		m_p = p;
	}
}


template <typename T>
inline
auto_ptr<T>&
auto_ptr<T>::operator=(auto_ptr<T>& ap)
{
	if (m_p != ap.m_p)
		reset(ap.release());

	return *this;
}


template <typename T>
inline
auto_ptr<T>&
auto_ptr<T>::operator=(by_ref ref)
{
	reset(ref.m_p);
	return *this;
}


template <typename T>
inline
typename auto_ptr<T>::reference
auto_ptr<T>::operator*()
{
	return *m_p;
}


template <typename T>
inline
typename auto_ptr<T>::const_reference
auto_ptr<T>::operator*() const
{
	return *m_p;
}


template <typename T>
inline
typename auto_ptr<T>::pointer
auto_ptr<T>::operator->()
{
	return m_p;
}


template <typename T>
inline
typename auto_ptr<T>::const_pointer
auto_ptr<T>::operator->() const
{
	return m_p;
}


template <typename T>
inline
auto_ptr<T>::operator by_ref()
{
	pointer p = m_p;
	const_cast<pointer&>(m_p) = 0;
	return by_ref(p);
}

#if HAVE_0X_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR

template <typename T>
inline
auto_ptr<T>::auto_ptr(auto_ptr&& p)
	:m_p(p.m_p)
{
	p.m_p = 0;
}


template <typename T>
inline
auto_ptr<T>&
auto_ptr<T>::operator=(auto_ptr&& p)
{
	swap(m_p, p.m_p);
	return *this;
}

#endif

} // namespace mstl

// vi:set ts=3 sw=3:
