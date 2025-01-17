// ======================================================================
// Author : $Author$
// Version: $Revision: 1213 $
// Date   : $Date: 2017-06-24 13:30:42 +0000 (Sat, 24 Jun 2017) $
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

#ifndef _mstl_type_traits_included
#define _mstl_type_traits_included

namespace mstl {

template <bool> struct bool_type {};

typedef bool_type<true>		true_type;
typedef bool_type<false>	false_type;

template <typename From, typename To> struct is_convertible;

template <typename T> struct is_reference;
template <typename T> struct is_pointer;

template <typename T> struct is_integral;
template <typename T> struct is_signed_integral;
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
template <typename T> struct memory_is_contiguous;

template <typename T> struct remove_reference;

} // namespace mstl

#include "m_type_traits.ipp"

#endif // _mstl_type_traits_included

// vi:set ts=3 sw=3:
