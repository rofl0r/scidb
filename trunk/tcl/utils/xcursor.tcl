# ======================================================================
# Author : $Author$
# Version: $Revision: 1 $
# Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
# Url    : $URL$
# ======================================================================

# ======================================================================
# Copyright: (C) 2009-2011 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

# ======================================================================
# This source is adopted from http://wiki.tcl.tk/27678.
# ======================================================================

namespace eval xcursor {

variable Lookup [dict create None 0]


proc setCursor {window cursor} {
	variable Lookup

	if {![winfo exists $window]} {
		return -code error "bad window path name \"$window\""
	}

	if {![dict exists $Lookup $cursor]} {
		return -code error "cursor \"$cursor\" unknown"
	}

	DefineCursor $window [dict get $Lookup $cursor]
}


proc loadCursor {filename {alias {}}} {
	variable Lookup

	if {![file readable $filename]} {
		return -code error "could not read \"$filename\": no such file or directory"
	}

	if {[llength $alias]} {
		set cursorName $alias
	} else {
		set cursorName [lindex [file split [file rootname $filename]] end]
	}

	if {![dict exists $Lookup $cursorName]} {
		dict set Lookup $cursorName [LoadFromFile $filename]
	}

	return $cursorName
}


proc names {} {
	return [dict keys [set [namespace current]::Lookup]]
}


proc deleteCursor {cursor} {
	variable Lookup

	if {$cursor eq "None"} { return }

	if {![dict exists $Lookup $cursor]} {
		return -code error "cursor \"$cursor\" unknown"
	}

	FreeCursor [dict get $Lookup $cursor]
	set Lookup [dict remove $Lookup $cursor]
}

} ;# namespace xcursor

# vi:set ts=3 sw=3:
