# ======================================================================
# Author : $Author$
# Version: $Revision: 1498 $
# Date   : $Date: 2018-07-11 11:53:52 +0000 (Wed, 11 Jul 2018) $
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
# Copyright: (C) 2009-2013 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

::util::source options

namespace eval options {

namespace import ::tcl::mathfunc::max

array set WriteCallbacks {}
array set TableCallbacks {}
variable SaveCallbacks {}
variable RestoreCallbacks {}
variable CompareCallbacks {}


proc hookWriter {callback {file options}} {
	variable WriteCallbacks

	if {![info exists WriteCallbacks($file)]} {
		set i -1
	} else {
		set i [lsearch -exact $WriteCallbacks($file) $callback]
	}
	if {$i == -1} { lappend WriteCallbacks($file) $callback }
}


proc hookTableWriter {callback {file options}} {
	variable TableCallbacks

	if {![info exists TableCallbacks($file)]} {
		set i -1
	} else {
		set i [lsearch -exact $TableCallbacks($file) $callback]
	}
	if {$i == -1} { lappend TableCallbacks($file) $callback }
}


proc unhookWriter {callback {file options}} {
	variable WriteCallbacks

	if {[info exists WriteCallbacks($file)]} {
		set i [lsearch -exact $WriteCallbacks($file) $callback]
		if {$i >= 0} { set WriteCallbacks($file) [lreplace $WriteCallbacks($file) $i $i] }
	}
}


proc hookTableWriter {callback {file options}} {
	variable TableCallbacks

	if {![info exists TableCallbacks($file)]} {
		set i -1
	} else {
		set i [lsearch -exact $TableCallbacks($file) $callback]
	}
	if {$i == -1} { lappend TableCallbacks($file) $callback }
}


proc writeHeader {chan file} {
	puts $chan "# Scidb $file file"
	puts $chan "# Version: $::scidb::version"
	puts $chan "# Syntax: Tcl language format"
	puts $chan ""
}


proc write {} {
	variable WriteCallbacks
	variable TableCallbacks

	foreach file [array names WriteCallbacks] {
		if {$file eq "options" || [llength $WriteCallbacks($file)] > 0} {
			set filename [set ::scidb::file::$file]
			set fd($file) [set chan [open $filename.tmp w]]
			fconfigure $chan -encoding utf-8
			writeHeader $chan $file
			foreach callback $WriteCallbacks($file) { $callback $chan }

			if {$file eq "options"} {
				foreach dialog [::toolbar::toolbarDialogs] {
					puts $chan "::toolbar::setOptions $dialog {"
					::options::writeArray $chan [::toolbar::getOptions $dialog]
					puts $chan "}"
				}
			}
		}
	}

	foreach file [array names TableCallbacks] {
		if {$file eq "options" || [llength $TableCallbacks($file)] > 0} {
			set filename [set ::scidb::file::$file]
			set chan $fd($file)
			foreach callback $TableCallbacks($file) { $callback $chan }
		}
	}

	foreach file [array names fd] {
		close $fd($file)
		set filename [set ::scidb::file::$file]
		file rename -force $filename.tmp $filename
	}
}


proc writeTableOptions {chan id} {
	variable TableCallbacks

	if {[info exists TableCallbacks(options)]} {
		foreach callback $TableCallbacks(options) { $callback $chan $id }
	}
}


proc writeEvalNS {chan namespace} {
	puts $chan "namespace eval $namespace {}"
}


proc writeItem {chan var {lowercaseOnly 1}} {
	if {[array exists $var]} {
		puts $chan "array set $var {"
		writeArray $chan [array get $var] $lowercaseOnly
		puts $chan "}"
	} else {
		switch [llength [set $var]] {
			0			{ puts $chan "set $var {}" }
			1			{ puts $chan "set $var [set $var]" }
			default	{ puts $chan "set $var {[set $var]}" }
		}
	}
}


proc writeList {chan var} {
	if {[llength [set $var]] == 0} {
		puts $chan "set $var {}"
	} else {
		puts $chan "set $var {[set $var]}"
	}
}


proc writeArray {chan arr {lowercaseOnly 1}} {
	set maxlength 0
	foreach {key val} $arr {
		set length [string length $key]
		if {[llength $key] > 1} { incr length 2 }
		set maxlength [max $maxlength $length]
	}
	set lst {}
	foreach {key val} $arr { lappend lst [list $key $val] }
	foreach elem [lsort -index 0 $lst] {
		lassign $elem key val
		if {!$lowercaseOnly || ![string is upper [string index $key 0]]} {
			if {[llength $key] > 1} { set key [list $key] }
			set spaces [string repeat " " [expr {$maxlength - [string length $key] + 1}]]
			if {[llength $val] == 0} {
				puts $chan "  $key$spaces{}"
			} else {
				puts $chan "  $key$spaces{$val}"
			}
		}
	}
}


proc hookSaveOptions {saveCallback restoreCallback compareCallback} {
	variable SaveCallbacks
	variable RestoreCallbacks 
	variable CompareCallbacks

	lappend SaveCallbacks $saveCallback
	lappend RestoreCallbacks $restoreCallback
	lappend CompareCallbacks $compareCallback
}


proc save {twm variant} {
	variable SaveCallbacks
	foreach callback $SaveCallbacks { $callback $twm $variant }
}


proc restore {twm variant} {
	variable RestoreCallbacks
	foreach callback $RestoreCallbacks { $callback $twm $variant }
}


proc compare {twm variant} {
	variable CompareCallbacks

	foreach callback $CompareCallbacks {
		if {![$callback $twm $variant]} { return false }
	}
	return true
}

} ;# namespace options

# vi:set ts=3 sw=3:
