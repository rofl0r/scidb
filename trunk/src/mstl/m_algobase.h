// ======================================================================
// Author : $Author$
// Version: $Revision: 809 $
// Date   : $Date: 2013-05-27 17:09:11 +0000 (Mon, 27 May 2013) $
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

#ifndef _mstl_algobase_included
#define _mstl_algobase_included

#include "m_types.h"

namespace mstl {

template <typename InputIterator, typename OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result);

template <typename InputIterator, typename OutputIterator>
OutputIterator copy_backward(InputIterator first, InputIterator last, OutputIterator result);

template <typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, T const& value);

template <typename OutputIterator, typename T>
OutputIterator fill_n(OutputIterator first, unsigned count, T const& value);

template <typename RandomAccessIterator>
void reverse(RandomAccessIterator first, RandomAccessIterator last);

} // namespace mstl

#include "m_algobase.ipp"

#endif // _mstl_algobase_included

// vi:set ts=3 sw=3:
