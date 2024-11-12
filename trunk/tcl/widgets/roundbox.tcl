# ======================================================================
# Author : $Author$
# Version: $Revision: 1 $
# Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
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
# Copyright: (C) 2011 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

proc roundbox {w args} {
	return [::roundbox::Build $w {*}$args]
}


namespace eval roundbox {

proc Build {w args} {
	ttk::entry $w \
		-exportselection no \
		-width 6 \
		-validate key \
		-validatecommand { return [regexp {^[0-9.]*$} %P] } \
		-invalidcommand { bell } \
		{*}$args \
		;
	catch { rename ::$w $w.__w__ }
	proc ::$w {command args} "[namespace current]::WidgetProc $w \$command {*}\$args"
	return $w
}


proc WidgetProc {w command args} {
	switch -- $command {
		bind {
			if {1 > [llength $args] || [llength $args] > 3} {
				error "wrong # args: should be \"[namespace curent] bind <tag> ?<sequence>? ?<script?>\""
			}
			return [bind $w {*}$args]
		}

		valid? {
			set value [string trim [$w.__w__ get]]
			if {$value eq "?" || $value eq "-"} { return 1 }
			if {![regexp {^([1-9][0-9]*(\.[1-9][0-9]*)?)?$} $value]} { return 0 }
			return 1
		}
		
		check {
			return [CheckRange [string trim [$w.__w__ get]]]
		}

		value {
			return [string trim [$w.__w__ get]]
		}

		focus {
			focus $w
			$w.__w__ selection clear
			$w.__w__ selection range 0 end
			return
		}
		
		set {
			if {[llength $args] != 1} {
				error "wrong # args: should be \"[namespace current] set <value>\""
			}
			$w.__w__ delete 0 end
			$w.__w__ insert 0 [lindex $args 0]
			return $w
		}

		instate {
			if {[llength $args] != 1 && [llength $args] != 2} {
				error "wrong # args: should be \"[namespace curent] $command <statespec> ?<script>?\""
			}
			if {[llength $args] == 2} {
				if {[$w.__w__ instate [lindex $args 0]]} {
					return [uplevel 2 [lindex $args 1]]
				}
			}
		}
	}

	return [$w.__w__ $command {*}$args]
}


proc CheckRange {value} {
	set round 0
	set subround 0
	lassign [split $value .] round subround
	if {$round > 255} { return 1 }
	if {$subround > 255} { return 2 }
	return 0
}

} ;# namespace roundbox

# vi:set ts=3 sw=3:
