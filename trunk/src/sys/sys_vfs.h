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

#ifndef _sys_vfs_included
#define _sys_vfs_included

#include "m_types.h"

namespace mstl { class string; }

namespace sys {
namespace vfs {

int64_t freeSize(mstl::string const& path);

} // namespace vfs
} // namespace sys

#endif // _sys_vfs_included

// vi:se ts=3 sw=3:
