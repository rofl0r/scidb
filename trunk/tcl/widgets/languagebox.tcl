# ======================================================================
# Author : $Author$
# Version: $Revision: 1510 $
# Date   : $Date: 2018-08-19 12:42:28 +0000 (Sun, 19 Aug 2018) $
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
# Copyright: (C) 2011-2018 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

::util::source language-selection-box

proc languagebox {w args} {
	return [::languagebox::Build $w {*}$args]
}


namespace eval languagebox {
namespace eval mc {

set AllLanguages	"All languages"
set None				"None"

}

proc Build {w args} {
	namespace eval [namespace current]::${w} {}
	variable ${w}::Content ""
	variable ${w}::Key ""
	variable ${w}::List {}
	variable ${w}::Values {}
	variable ${w}::None 0

	array set opts {
		-height			15
		-width			20
		-textvar			{}
		-textvariable	{}
		-state			readonly
		-list				{}
		-none				0
	}
	array set opts $args

	if {[llength $opts(-textvariable)] == 0} {
		set opts(-textvariable) $opts(-textvar)
	}
	if {[llength $opts(-textvariable)] == 0} {
		set opts(-textvariable) [namespace current]::${w}::Content
	}
	set List $opts(-list)
	set None $opts(-none)

	ttk::tcombobox $w \
		-height $opts(-height) \
		-showcolumns {flag name} \
		-format "%2" \
		-width $opts(-width) \
		-textvariable $opts(-textvariable) \
		-scrollcolumn name \
		-exportselection no \
		-disabledbackground white \
		-disabledforeground grey60 \
		-state $opts(-state) \
		-placeicon yes \
		;

	$w addcol image -id flag -width 20 -justify center
	$w addcol text  -id name

	bind $w <Destroy> [list catch [list namespace delete [namespace current]::${w}]]
	bind $w <Any-Key> [namespace code [list Completion $w %A %K $opts(-textvariable)]]
	bind $w <<ComboBoxUnposted>> +[list set [namespace current]::${w}::Key ""]
	bind $w <<LanguageChanged>> [namespace code [list LanguageChanged $w]]

	SetupList $w

	catch { rename ::$w $w.__w__ }
	proc ::$w {command args} "[namespace current]::WidgetProc $w \$command {*}\$args"

	return $w
}


proc WidgetProc {w command args} {
	switch -- $command {
		bind {
			if {1 > [llength $args] || [llength $args] > 3} {
				error "wrong # args: should be \"[namespace current] bind <tag> ?<sequence>? ?<script?>\""
			}
			return [bind $w {*}$args]
		}

		value {
			set lang [$w.__w__ get [$w.__w__ current] name]
			if {$lang eq $mc::None} { return "" }
			variable ${w}::Values
			set n [lsearch -exact -index 1 $Values $lang]
			return [lindex $Values $n 2]
		}

		valid? {
			return [expr {[$w.__w__ find [$w.__w__ get]] >= 0}]
		}

		set {
			if {[llength $args] != 1} {
				error "wrong # args: should be \"[namespace current] set <value>\""
			}
			set lang [lindex $args 0]
			if {[string length $lang] == 0} {
				set lang $mc::None
			} elseif {[string length $lang] == 2} {
				variable ${w}::Values
				set n [lsearch -exact -index 2 $Values $lang]
				if {$n >= 0} { set lang [lindex $Values $n 1] }
			}
			set var [$w.__w__ cget -textvariable]
			set $var $lang
			Search $w $var 1
			$w placeicon
			return $w
		}

		instate {
			if {[llength $args] != 1 && [llength $args] != 2} {
				error "wrong # args: should be \"[namespace current] $command <statespec> ?<script>?\""
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


proc SetupList {w} {
	variable ${w}::List
	variable ${w}::None
	variable ${w}::Values

	set Values {}

	foreach entry [::country::makeCountryList] {
		if {[llength $List] == 0 || [lindex $entry 1] in $List} {
			lappend Values $entry
		}
	}

	set Values [lsort -index 1 -dictionary $Values]

	if {"xx" in $List} {
		set name $mc::AllLanguages
		set flag $::country::icon::flag(ZZX)
		set Values [linsert $Values 0 [list xx $name $flag]]
	}
	if {$None} {
		set name $mc::None
		set flag {}
		set Values [linsert $Values 0 [list "  " $name $flag]]
	}

	foreach entry $Values {
		lassign $entry flag name _
		$w listinsert [list $flag $name]
	}

	$w resize -force
	$w current 0
	$w mapping [::mc::mappingForSort] [::mc::mappingToAscii]
}


proc LanguageChanged {w} {
	variable ${w}::Values

	set lang ""
	set content [$w get]
	set n [lsearch -exact -index 2 $Values $content]
	if {$n >= 0} { set lang [lindex $Values $n 2] }

	SetupList $w

	if {[llength $lang]} {
		set n [lsearch -exact -index 0 $Values $lang]
		if {$n >= 0} { $w set [lindex $Values $n 1] }
	}

	if {[$w state] ne "readonly"} {
		$w icursor end
	}
}


proc Completion {w code sym var} {
	if {[$w popdown?]} { return }
	if {[$w state] eq "readonly"} { return }

	switch -- $sym {
		Tab {
			set $var [string trimleft [set $var]]
			Search $w $var 1
			$w placeicon
		}

		default {
			$w forgeticon
			if {[string is alnum -strict $code] || [string is punct -strict $code] || $code eq " "} {
				after idle [namespace code [list Completion2 $w $var [set $var]]]
			}
		}
	}
}


proc Completion2 {w var prevContent} {
	set content [string trimleft [set $var]]

	if {[string length $content] && [string range $content 0 end-1] eq $prevContent} {
		$w testicon
		Search $w $var 0
	}
}


proc Search {w var full} {
	set content [set $var]
	if {[llength $content] == 0} { return }

	if {$full} {
		$w current search name $content
	} else {
		$w current match name $content
		set newContent [$w get]
		
		if {$content ne $newContent} {
			set k 0
			set j 0
			set n [string length $content]
			while {$j < $n} {
				set c [string index $newContent $k]

				if {$c eq [string index $content $j]} {
					incr j
				} else {
					incr j [string length [::mc::mapForSort $c]]
				}

				incr k
			}

			if {[$w state] ne "readonly"} {
				$w icursor $k
				$w selection clear
				$w selection range $k end
			}
		}
	}
}

} ;# namespace languagebox

# vi:set ts=3 sw=3:
