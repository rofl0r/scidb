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

#ifndef _mstl_type_traits_included
#define _mstl_type_traits_included

namespace mstl {

template <typename From, typename To> struct is_convertible;

template <typename T> struct is_reference;
template <typename T> struct is_pointer;

template <typename T> struct is_integral;
template <typename T> struct is_float;
template <typename T> struct is_void;
template <typename T> struct is_enum;
template <typename T> struct is_array;
template <typename T> struct is_member_pointer;
template <typename T> struct is_function;
template <typename T> struct is_class;

template <typename T> struct is_arithmetic;
template <typename T> struct is_scalar;
template <typename T> struct is_pod;

template <typename T> struct has_trivial_destructor;
template <typename T> struct is_movable;

} // namespace mstl

#include "m_type_traits.ipp"

#endif // _mstl_type_traits_included

// vi:set ts=3 sw=3:
