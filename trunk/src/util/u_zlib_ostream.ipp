// ======================================================================
// Author : $Author$
// Version: $Revision: 609 $
// Date   : $Date: 2013-01-02 17:35:19 +0000 (Wed, 02 Jan 2013) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2012-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

namespace util {

inline bool ZlibOStream::isOpen() const					{ return m_dst != 0; }
inline uint32_t ZlibOStream::crc() const					{ return m_crc; }
inline unsigned ZlibOStream::size() const					{ return m_size; }
inline unsigned ZlibOStream::compressedSize() const	{ return m_compressedSize; }

} // namespace util

// vi:set ts=3 sw=3:
