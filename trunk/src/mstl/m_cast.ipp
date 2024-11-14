// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
// Url    : $URL$
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

#include "m_exception.h"

#include "m_type_traits.h"

namespace mstl {
namespace cast_ {

template <class, int> struct cast_helper;

template <class T>
struct cast_helper<T,0>
{
	template <class U> static T* cast_ptr(U* p)
	{
		T* t = dynamic_cast<T*>(p);

		if (t == 0)
			M_RAISE("bad cast");

		return t;
	}

	template <class U> static T& cast_ref(U& p) { return dynamic_cast<T&>(p); }
};


template <class T>
struct cast_helper<T,1>
{
	template <class U> static T* cast_ptr(U* p) { return static_cast<T*>(p); }
	template <class U> static T& cast_ref(U& p) { return static_cast<T&>(p); }
};

} // namespace cast_


template <class T, class U>
inline
T&
safe_cast_ref(U& p)
{
	return cast_::cast_helper<T,(mstl::is_convertible<U,T>::value)>::cast_ref(p);
}


template <class T, class U>
inline
T*
safe_cast_ptr(U* p)
{
	return p ? cast_::cast_helper<T,(mstl::is_convertible<U*,T*>::value)>::cast_ptr(p) : 0;
}

} // namespace mstl

// vi:set ts=3 sw=3:
