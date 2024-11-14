// ======================================================================
// Author : $Author$
// Version: $Revision: 810 $
// Date   : $Date: 2013-05-27 22:24:12 +0000 (Mon, 27 May 2013) $
// Url    : $URL$
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
// ======================================================================

// ======================================================================
// Copyright: (C) 2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

namespace app {

inline TreeAdmin::TreeP TreeAdmin::tree() const	{ return m_currentTree; }
inline sys::Thread& TreeAdmin::thread()			{ return m_thread; }

} // namespace app

// vi:set ts=3 sw=3:
