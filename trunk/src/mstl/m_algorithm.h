// ======================================================================
// Author : $Author$
// Version: $Revision: 5 $
// Date   : $Date: 2011-05-05 07:51:24 +0000 (Thu, 05 May 2011) $
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

#ifndef _mstl_algorithm_included
#define _mstl_algorithm_included

#include "m_algobase.h"

namespace mstl {

template <typename ForwardIterator, typename EqualityComparable>
ForwardIterator
find(ForwardIterator first, ForwardIterator last, const EqualityComparable& value);

template <typename ForwardIterator, typename LessThanComparable>
ForwardIterator
lower_bound(ForwardIterator first, ForwardIterator last, LessThanComparable const& value);

template <typename ForwardIterator, typename LessThanComparable>
ForwardIterator
upper_bound(ForwardIterator first, ForwardIterator last, LessThanComparable const& value);

template <typename ForwardIterator, typename LessThanComparable>
ForwardIterator
binary_search(ForwardIterator first, ForwardIterator last, LessThanComparable const& value);

template <typename ForwardIterator>
ForwardIterator rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last);

} // namespace mstl

#include "m_algorithm.ipp"

#endif // _mstl_algorithm_included

// vi:set ts=3 sw=3:
