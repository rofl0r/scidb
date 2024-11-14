// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
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

#ifndef _mstl_backtrace_included
#define _mstl_backtrace_included

#include "m_types.h"

namespace mstl {

class ostringstream;
template <typename T, bool Zero> class chunk_allocator;

class backtrace
{
public:

	backtrace();
	backtrace(backtrace const& v);
	~backtrace() throw();

	bool empty() const;

	backtrace const& operator=(backtrace const& v);
	void text_write(ostringstream& os, unsigned skip = 0) const;
	void clear();

	static bool is_debug_mode();

private:

#ifndef __OPTIMIZE__
	typedef chunk_allocator<char,false> allocator;

	void symbols();
# ifdef __unix__
	bool symbols_gdb();
	bool symbols_linux();
# endif

	void*			m_addresses[64];	///< Addresses of each function on the stack.
	char*			m_symbols[64];		///< Symbols corresponding to each address.
	unsigned		m_nframes;			///< Number of addresses in m_addresses.
	allocator*	m_allocator;		///< Allocator for symbols.
	unsigned		m_skip;				///< Skip first m_skip symbols.
	unsigned*	m_refCount;			///< Reference counter.
#endif
};

} // namespace mstl

#endif // _mstl_backtrace_included

// vi:set ts=3 sw=3:
