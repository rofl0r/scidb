// ======================================================================
// Author : $Author$
// Version: $Revision: 442 $
// Date   : $Date: 2012-09-23 23:56:28 +0000 (Sun, 23 Sep 2012) $
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
namespace bits {

inline bool file::is_open() const						{ return m_open; }
inline bool file::is_buffered() const					{ return m_buffered; }
inline bool file::is_unbuffered() const				{ return !m_buffered; }
inline unsigned file::bufsize() const					{ return m_bufsize; }
inline char* file::buffer() const						{ return m_buffer; }
inline mstl::string const& file::filename() const	{ return m_filename; }

} // namespace bits
} // namespace mstl

// vi:set ts=3 sw=3:
