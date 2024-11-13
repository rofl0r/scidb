// ======================================================================
// Author : $Author$
// Version: $Revision: 430 $
// Date   : $Date: 2012-09-20 17:13:27 +0000 (Thu, 20 Sep 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2011-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _TeXt_Memory_included
#define _TeXt_Memory_included

#include "T_Config.h"

#ifdef USE_MEM_BLOCKS

#include "m_types.h"

namespace TeXt {

class MemoryBlock;

struct Memory
{
	static void* alloc(size_t n);
	static void release(void* obj);
	static void cleanup();
};

} // namespace TeXt

#endif

#endif // _TeXt_Alignment_included

// vi:set ts=3 sw=3:
