// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2011-2013 Gregor Cramer
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
