# ======================================================================
# Author : $Author$
# Version: $Revision: 163 $
# Date   : $Date: 2011-12-20 19:43:40 +0000 (Tue, 20 Dec 2011) $
# Url    : $URL$
# ======================================================================

# ======================================================================
# Copyright: (C) 2008-2011 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

namespace eval validate {

namespace import ::tcl::mathfunc::max

proc spinboxInt {w {clamp 1}} {
	set min [expr {int([$w cget -from])}]
	set max [expr {int([$w cget -to])}]
	set len [max [string length $min] [string length $max]]
	if {$min < 0} {
		set vcmd [namespace code [list validateInteger %P $len]]
	} else {
		set vcmd [namespace code [list validateUnsigned %P $len]]
	}
	$w configure -validatecommand $vcmd -invalidcommand { bell }
	if {$clamp} {
		bind $w <FocusOut> +[namespace code { ClampInt %W }]
	}
	bind $w <FocusOut> {+ %W selection clear }
	bind $w <FocusIn>  {+ %W configure -validate key }
}


proc entryFloat {w} {
	$w configure -validatecommand [namespace code { validateFloat %P }] -invalidcommand { bell }
	bind $w <FocusOut> +[namespace code { formatFloat %W }]
	bind $w <FocusOut> {+ %W selection clear }
	bind $w <FocusIn>  {+ %W configure -validate key }
}


proc entryUnsigned {w} {
	$w configure -validatecommand [namespace code { validateUnsigned %P }] -invalidcommand { bell }
	bind $w <FocusOut> +[namespace code { formatUnsigned %W }]
	bind $w <FocusOut> {+ %W selection clear }
	bind $w <FocusIn>  {+ %W configure -validate key }
}


proc validateUnsigned {value maxlen} {
	set value [string trim $value]
	if {[string length $value] > $maxlen} { return 0 }
	if {![regexp {[0-9]*} $value result]} { return 0 }
	if {[string length $value] != [string length $result]} { return 0 }
	return 1
}


proc validateInteger {value maxlen} {
	set value [string trim $value]
	if {[string length $value] > $maxlen} { return 0 }
	if {![regexp {[+-]?[0-9]*} $value result]} { return 0 }
	if {[string length $value] != [string length $result]} { return 0 }
	return 1
}


proc validateFloat {value} {
	set value [string trim $value]
	if {![regexp {[0-9]*[.,]?[0-9]*} $value result]} { return 0 }
	if {[string length $value] != [string length $result]} { return 0 }
	return 1
}


proc formatFloat {w} {
	set var [$w cget -textvariable]
	set val [string trimleft [string trim [set $var]] "0"]
	set val [string map {, .} $val]

	if {[string length $val] == 0} {
		set val "0"
	} else {
		if {[string index $val 0] eq "."} { set val "0$val" }
		if {[string index $val end] eq "."} { set val [string range $val 0 end-1] }
	}

	if {[set $var] ne $val} { set $var $val }
}


proc ClampInt {w} {
	set min [expr {int([$w cget -from])}]
	set max [expr {int([$w cget -to])}]
	set var [$w cget -textvariable]
	set val [string trimleft [string trim [set $var]] "0"]

	if {$val == ""} { set val 0 }

	if {$val < $min} {
		set val $min
	} elseif {$val > $max} {
		set val $max
	}

	if {[set $var] ne $val} { set $var $val }
}

} ;# namespace validate

# vi:set ts=3 sw=3:
