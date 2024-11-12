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
# Copyright: (C) 2010-2011 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

proc eventtypebox {w args} {
	return [::eventtypebox::Build $w {*}$args]
}


namespace eval eventtypebox {
namespace eval mc {

set Type(game)		"Game"
set Type(match)	"Match"
set Type(tourn)	"Roound Robin"
set Type(swiss)	"Swiss-System Tournament"
set Type(team)		"Team Tournament"
set Type(k.o.)		"Knockout Tournament"
set Type(simul)	"Simultaneous Tournament"
set Type(schev)	"Scheveningen-System Tournament"

} ;# namespace mc


namespace import ::tcl::mathfunc::max

variable types {match tourn swiss team k.o. schev simul game}


proc minWidth {} {
	variable types

	set len 0
	foreach type $types {
		set len [max $len [string length $mc::Type($type)]]
	}

	return $len
}


proc Build {w args} {
	namespace eval [namespace current]::${w} {}
	variable ${w}::Content
	variable ${w}::Width
	variable ${w}::IgnoreKey 0

	array set opts {
		-textvar			{}
		-textvariable	{}
		-width			0
		-state			normal
	}
	array set opts $args

	if {[llength $opts(-textvariable)] == 0} {
		set opts(-textvariable) $opts(-textvar)
	}
	if {[llength $opts(-textvariable)] == 0} {
		set opts(-textvariable) [namespace current]::${w}::Content
	}

	ttk::frame $w -borderwidth 0 -takefocus 0
	ttk::tcombobox $w.__w__ \
		-textvariable $opts(-textvariable) \
		-exportselection no \
		-column type \
		-state $opts(-state) \
		;
	$w.__w__ addcol image -id icon
	$w.__w__ addcol text -id type
	pack $w.__w__ -anchor w

	set Width $opts(-width)
	Setup $w

	bind $w <Destroy> [list catch [list namespace delete [namespace current]::${w}]]
	bind $w.__w__ <Any-Key> [namespace code [list Completion $w %A %K $opts(-textvariable)]]
	bind $w.__w__ <<Language>> [namespace code [list LanguageChanged $w]]
	bind $w.__w__ <<ComboboxPosted>> [list set [namespace current]::${w}::IgnoreKey 1]
	bind $w.__w__ <<ComboboxUnposted>> [list set [namespace current]::${w}::IgnoreKey 0]
	bind $w.__w__ <<ComboboxCurrent>> [namespace code [list ShowIcon $w]]

	$w.__w__ current 0

	catch { rename ::$w $w.__eventtypebox__ }
	proc ::$w {command args} "[namespace current]::WidgetProc $w \$command {*}\$args"

	return $w
}


proc WidgetProc {w command args} {
	switch -- $command {
		cget {
			if {[lindex $args 0] eq "-takefocus"} {
				return 0
			}
		}

		bind {
			if {1 > [llength $args] || [llength $args] > 3} {
				error "wrong # args: should be \"[namespace current] bind <tag> ?<sequence>? ?<script?>\""
			}
			bind $w.__w__ {*}$args
			return
		}

		valid? {
			set value [$w.__w__ get]
			set index [lsearch [$w.__w__ cget -values] $value]
			if {$index >= 0} { return true }
			if {$value eq "-" || $value eq "--" || $value eq ""} { return true }
			return false
		}

		value {
			variable types
			set item [$w.__w__ current]
			if {$item <= 0} { return "" }
			return [lindex $types [expr {$item - 1}]]
		}

		set {
			if {[llength $args] != 1} {
				error "wrong # args: should be \"[namespace current] set <value>\""
			}
			set value [lindex $args 0]
			if {[info exists mc::Type($value)]} { set value $mc::Type($value) }
			$w.__w__ current search type $value
			ShowIcon $w
			return $w
		}

		focus {
			return [focus $w.__w__]
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


proc LanguageChanged {w} {
	$w.__w__ forgeticon
	set current [$w.__w__ current]
	Setup $w
	if {$current >= 0} {
		$w.__w__ current $current
	}
}


proc Setup {w} {
	variable ${w}::Width
	variable types

	$w.__w__ configure -width [expr {max([minWidth], $Width)}]
	$w.__w__ listinsert { "" "--" } -index 0
	set index 0
	foreach type $types {
		$w.__w__ listinsert [list $icon::12x12::Type($type) $mc::Type($type)] -index [incr index]
	}
	$w.__w__ resize
	$w.__w__ mapping [::mc::mappingForSort] [::mc::mappingToAscii]
}


proc Completion {w code sym var} {
	if {![info exists ${w}::IgnoreKey]} { return }
	variable ${w}::IgnoreKey

	if {$IgnoreKey} { return }

	switch -- $sym {
		Tab {
			set $var [string trimleft [set $var]]
			Search $w $var 1
			ShowIcon $w
		}

		default {
			if {[string is alnum -strict $code] || $code eq " "} {
				after idle [namespace code [list Completion2 $w $var [set $var]]]
			} else {
				after idle [namespace code [list ShowIcon $w]]
			}
		}
	}
}


proc Completion2 {w var prevContent} {
	variable types

	set content [string trimleft [set $var]]

	set len [string length $content]
	if {$len == 0} { return }

	if {$len == 1 && [string is digit $content]} {
		if {$content <= [llength $types]} {
			$w.__w__ current $content
			$w.__w__ icursor end
			$w.__w__ selection clear
			$w.__w__ selection range 0 end
		} else {
			$w.__w__ set ""
			bell
		}
	} elseif {[string equal -nocase -length [expr {$len - 1}] $content $prevContent]} {
		Search $w $var 0
	}

	ShowIcon $w
}


proc Search {w var full} {
	set content [set $var]
	if {[string length $content] == 0} { return }

	if {$full} {
		$w.__w__ current search -nocase type $content
	} else {
		$w.__w__ current match -nocase type $content
		set newContent [$w get]
		
		if {$content ne $newContent} {
			set k 0
			set j 0
			set n [string length $content]
			while {$j < $n} {
				set c [string index $newContent $k]

				if {[string equal -nocase $c [string index $content $j]]} {
					incr j
				} else {
					incr j [string length [::mc::mapForSort $c]]
				}

				incr k
			}

			$w.__w__ icursor $k
			$w.__w__ selection clear
			$w.__w__ selection range $k end
		}
	}
}


proc ShowIcon {w} {
	variable types

	set content [$w get]
	if {[string length $content]} {
		set idx [$w.__w__ find $content]
		if {$idx >= 1} {
			set img $icon::12x12::Type([lindex $types [expr {$idx - 1}]])
			if {[$w.__w__ placeicon $img]} {
				return
			}
		}
	}

	$w.__w__ forgeticon
}


namespace eval icon {
namespace eval 12x12 {

set Type(game)  $::icon::12x12::filetypeScidbBase
set Type(match) $::icon::12x12::human
set Type(tourn) $::icon::12x12::dot

set Type(swiss) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMAgMAAAArG7R0AAAADFBMVEUAAAAAAAD////YGADE
	zaH2AAAAGklEQVQI12P4//8/Axi/RuBfq9aDMbIYVB0AMnMiEa20/TkAAAAASUVORK5CYII=
}]

set Type(team) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAABmJLR0QA/wD/AP+gvaeTAAAC
	MUlEQVQYGV3By0sUcQAH8O/P+e1v5+Fjn664ZuuGkkpohxRFySU6FVZYRBB16GB19A+QoIuH
	IpI6RXkoCIqSBMuDEWhgWRrGGqmbMpNrti93zN3ZncdOh259PnjU04M74bD4ZXT01tbMTPLH
	+Pj81OBg18bYGE5Riv+Ru6EQVE070d7V9SzQ3CwWVBWrc3Mv30K+NDTxpFXgyuuSe5sLkcYL
	8tDjftB5WUbRtqsTk5NC7fQ0cpYFWdMC3a+uXmGiPSzxDnfaMD7fmBy4WBv0rdEKrxdOw0gS
	0yz46usFK50Gx6BGE4vnit/SXkFgSCTUjpWN2HGfy79G5VTKdT4SGfQHAgLleeyzLLQa+tEH
	H6dLv8kaPB4JyvqOmdnWsutbMdAAIZFdRelzMwbm9wOGCS0dF9vbD4Gr9do17iDxlFaJHS4E
	bp95B3IWuF9dVnbdTykkxmCaFnZ6XWgbuYyqSg8oEWATHSvxpdibT89P0yJwMF8qgRACQZKw
	W9IRb+aAzfdw+xmcvAP5PR2/fqoN0B0nKQcsSxzXHW5p4UNHOqBkFczys8jFF+EyHHA4CfK7
	Fna2LG4vp/fSHHCTMbYg1tR0VjU2VRlpraLotI9VVoji/romlAuVULY3kEluwrY5H3cYyM+a
	5lIbz0/5ON/EQ8fMa1XKiA3BxmBnqL/sgLvT1HU7p6SUZTWbv0df4J9r0aiFWNTqGxEyqa+F
	Yb2ePM39KYYE6KxU5LJEZ99XPhjyX2AN8yRXBsHPAAAAAElFTkSuQmCC
}]

set Type(k.o.) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAQAAAD8fJRsAAAAAmJLR0QA/4ePzL8AAAD6SURB
	VBgZBcGxLkNRGMDx//edc29u01Z7aSORkDTYDIYmXoHEzgPwECaDsHgAY2e7WKxlYJBIhFjv
	IgTVilR7z/n8fgKODnHJjsrNWLV3ruxsdJ/hYIGJ6vF0b1INqTVtXbvJa+PFL+bJPo1yGxQw
	zOxad0e3nrXxgStiDmCAeftKQ8g1vMWh4X4VDxhmWkrKj6sNZNm2pBLUEAxCUo+fo3M3Y/Jk
	3dAxAiCYxpacZo+uAgP62rLV4CMgyI07iWOBjFlkXi5DM/4JvsZh0vvAw5gU6lKkfZkEL+1Y
	CEM8QMTm3DM9K2G6EUoFPMAUHnxbd2RFndx9XyjwDzgjaNxaQ2jPAAAAAElFTkSuQmCC
}]

set Type(simul) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAQAAAD8fJRsAAAAdElEQVQY032PbQ3AIAxErxUw
	BVOAgglBCApwgB6EzECdMFpK2Eey8IfcK68HGuahY90bVhxZuHyAxXJHf6CHFdtUcRlwzio6
	LPZ34HNIKGkvFwp2cDDUZyi5UCjaDg6UXWK7NF6tskvqq64LZUw/fq5oxQ0XEYlxzPEu5kEA
	AAAASUVORK5CYII=
}]

set Type(schev) $::icon::12x12::gameflag(S)

}; # namespace 12x12
}; # namespace icon
} ;# namespace eventtypebox

# vi:set ts=3 sw=3:
