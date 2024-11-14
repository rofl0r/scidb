# ======================================================================
# Author : $Author$
# Version: $Revision: 806 $
# Date   : $Date: 2013-05-26 14:59:23 +0000 (Sun, 26 May 2013) $
# Url    : $URL$
# ======================================================================

# ======================================================================
#    _/|            __
#   // o\         /    )           ,        /    /
#   || ._)    ----\---------__----------__-/----/__-
#   //__\          \      /   '  /    /   /    /   )
#   )___(     _(____/____(___ __/____(___/____(___/_
# ======================================================================

# ======================================================================
# Copyright: (C) 2013 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

::util::source clipboard

namespace eval selection {

variable CurrentSelection ""

proc selectText {text} {
	clipboard clear -displayof "."
	clipboard append -displayof "." $text

	if {[tk windowingsystem] eq "x11"} {
		variable CurrentSelection
		set CurrentSelection $text

		selection handle -selection PRIMARY "." [namespace current]::PrimaryTransfer
		selection own -selection PRIMARY -command [namespace current]::LostSelection "."
	}
}

if {[tk windowingsystem] eq "x11"} {

	proc PrimaryTransfer {offset maxChars} {
		variable CurrentSelection
		return [string range $CurrentSelection $offset [expr {$offset + $maxChars - 1}]]
	}


	proc LostSelection {} {
		variable CurrentSelection
		set CurrentSelection ""
	}

}

} ;# namespace selection

# vi:set ts=3 sw=3:
