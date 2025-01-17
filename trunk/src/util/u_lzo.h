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

#ifndef _u_lzo_included
#define _u_lzo_included

#define U_LZO_OVERHEAD_COMPRESS(n) (((n) >> 4) + 64 + 3)

namespace util {

class ByteStream;

struct Lzo
{
	static bool compress(ByteStream const& src, ByteStream& dst);
	static void decompress(ByteStream const& src, ByteStream& dst);

	static unsigned maxSizeAfterCompression(unsigned sizeBeforeCompression);
};

} // namespace util

#endif // _u_lzo_included

// vi:set ts=3 sw=3:
