# ======================================================================
# Author : $Author$
# Version: $Revision: 1502 $
# Date   : $Date: 2018-07-16 12:55:14 +0000 (Mon, 16 Jul 2018) $
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
# Copyright: (C) 2010-2018 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

::util::source eco-selection-box

proc ecobox {w args} {
	return [::ecobox::Build $w {*}$args]
}


namespace eval ecobox {
namespace eval mc {

set OpenEcoDialog "Open ECO dialog"

}


proc Build {w variant args} {
	namespace eval [namespace current]::${w} {}
	variable ${w}::Content

	array set opts {
		-textvar {}
		-textvariable {}
	}
	array set opts $args

	if {[llength $opts(-textvariable)] == 0} {
		set opts(-textvariable) $opts(-textvar)
	}
	if {[llength $opts(-textvariable)] == 0} {
		set opts(-textvariable) [namespace current]::${w}::Content
	}

	ttk::frame $w -borderwidth 0
	ttk::entry $w.entry \
		-textvariable $opts(-textvariable) \
		-exportselection no \
		-width 4 \
		-validate key \
		-validatecommand [namespace code { ValidateEco %P %S }] \
		-invalidcommand { bell } \
		;
	grid $w.entry -row 0 -column 0 -sticky ew
	grid columnconfigure $w {0} -weight 1
	# XXX currently not working
	if {$variant eq "xormal" && [llength [namespace which openEcoDialog]]} {
		ttk::button $w.eco \
			-style icon.TButton \
			-image $::icon::16x16::lines \
			-command [namespace code [list OpenEcoDialog $w]] \
			;
		grid $w.eco -row 0 -column 2
		grid columnconfigure $w {1} -minsize $::theme::padx
		tooltip $w.eco [namespace current]::mc::OpenEcoDialog
	}

	bind $w.entry <Destroy> [list catch [list namespace delete [namespace current]::${w}]]
	bind $w.entry <Any-Key> [namespace code [list Completion $w.entry %A %K $opts(-textvariable)]]
	bind $w.entry <<LanguageChanged>> \
		[namespace code [list LanguageChanged $w.entry $opts(-textvariable)]]

	catch { rename ::$w $w.__w__ }
	proc ::$w {command args} "[namespace current]::WidgetProc $w \$command {*}\$args"

	return $w
}


proc tooltip {args} {}


proc OpenEcoDialog {w} {
	set eco [openEcoDialog $w]
	if {[string length $eco]} { $w.entry set $eco }
}


proc WidgetProc {w command args} {
	switch -- $command {
		bind {
			if {1 > [llength $args] || [llength $args] > 3} {
				error "wrong # args: should be \"[namespace curent] bind <tag> ?<sequence>? ?<script?>\""
			}
			bind $w.entry {*}$args
			return
		}

		cget {
			if {[llength $args] != 1} {
				error "wrong # args: should be \"[namespace curent] cget <option>\""
			}
			if {[lindex $args 0] eq "-takefocus"} {
				$w.entry instate disabled { return 0 }
				return 1
			}
		}

		valid? {
			set value [string range [$w.entry get] 0 2]
			return [regexp {([A-E][0-9][0-9])?} $value]
		}

		value {
			return [string range [$w.entry get] 0 2]
		}

		set {
			if {[llength $args] != 1} {
				error "wrong # args: should be \"[namespace current] set <value>\""
			}
			set var [$w.entry cget -textvariable]
			set $var [lindex $args 0]
			Completion2 $w.entry $var no
			return $w
		}

		instate {
			if {[llength $args] != 1 && [llength $args] != 2} {
				error "wrong # args: should be \"[namespace curent] $command <statespec> ?<script>?\""
			}
			if {[llength $args] == 2} {
				if {[$w.entry instate [lindex $args 0]]} {
					return [uplevel 2 [lindex $args 1]]
				}
			}
		}
	}

	return [$w.entry $command {*}$args]
}


proc LanguageChanged {w var} {
	Completion2 $w $var no
}


proc ValidateEco {eco key} {
	switch [string length $eco] {
		0 { return 1 }
		1 { return [expr {$eco eq " " || [string match {[A-Ea-e]} $eco]}] }
		2 { return [string match {[A-Ea-e][0-9]} $eco] }
		3 { return [string match {[A-Ea-e][0-9][0-9]} $eco] }
	}

	return 0
}


proc Completion {w code sym var} {
	if {$sym eq "Tab"} {
		after idle [namespace code [list Completion2 $w.entry $var no]]
	} elseif {[string is alnum -strict $code] || $code eq " "} {
		after idle [namespace code [list Completion2 $w.entry $var yes]]
	}
}


proc Completion2 {w var selection} {
	set content [string trimleft [string toupper [set $var] 0 1]]
	set len [string length $content]

	if {$len == 0} {
		set $var ""
	} elseif {[string length $content] >= 3} {
		set content [string range $content 0 2]
		set opening [::scidb::app::lookup ecoCode $content]
		lassign $opening long short
		set vars [lrange $opening 2 end]
		append content " \u2013 "
		if {[llength $vars]} {
			append content [::mc::translateEco $short]
			foreach var $vars { append content ", " [::mc::translateEco $var] }
		} else {
			append content [::mc::translateEco $long]
		}
		set $var $content

		if {$selection} {
			$w.entry selection clear
			$w.entry selection range 3 end
		}
	} else {
		set $var [string range $content 0 2]
	}
}

} ;# namespace ecobox

# vi:set ts=3 sw=3:
