# ======================================================================
# Author : $Author$
# Version: $Revision: 1265 $
# Date   : $Date: 2017-07-09 09:25:05 +0000 (Sun, 09 Jul 2017) $
# Url    : $URL$
# ======================================================================

# ======================================================================
# Copyright: (C) 2012-2013 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

package require treectrl
package provide treetable

proc treetable {path args} { return [treetable::treetable $path {*}$args] }

namespace eval treetable {

array set Colors {
	selected:focus		selected:focus
	selected!focus		selected!focus
	active:focus		active:focus
	hilite!selected	hilite!selected
}
# #e0e0e0 is an alternative for {selected !focus}

proc treetable {path args} {
	array set opts {
		-takefocus				1
		-borderwidth			1
		-relief					sunken
		-buttonimage			{}
		-xscrollincrement		1
		-showfocus				1
		-width					0
		-showlines				1
		-showbuttons			0
		-showarrows				0
		-selectmode				single
		-background				treetable,background
		-disabledforeground	treetable,disabledforeground
	}
	array set opts $args

	set t $path.f.t
	namespace eval [namespace current]::${t} {}
	variable ${t}::Vars
	set Vars(0:lastchild) root
	set Vars(depth) 0
	set Vars(disabledforeground) $opts(-disabledforeground)

	if {$opts(-showarrows)} {
		set opts(-showbuttons) 1
		set opts(-showlines) 0
		if {[llength $opts(-buttonimage)] == 0} {
			set opts(-buttonimage) [list $icon::16x16::collapse open $icon::16x16::expand {}]
		}
	}

	tk::frame $path -borderwidth 0 -takefocus 0 -width $opts(-width)
	set f [tk::frame $path.f -borderwidth 0 -takefocus 0]
	pack $f -fill both -expand yes
	bind $path <FocusIn> [list focus $t]
	bind $f <FocusIn> [list focus $t]
	treectrl $t                                          \
		-class TreeTable                                  \
		-takefocus $opts(-takefocus)                      \
		-borderwidth $opts(-borderwidth)                  \
		-relief $opts(-relief)                            \
		-xscrollincrement $opts(-xscrollincrement)        \
		-background [::colors::lookup $opts(-background)] \
		-showlines $opts(-showlines)                      \
		-showbuttons $opts(-showbuttons)                  \
		-showheader no                                    \
		-highlightthickness 0                             \
		-linestyle solid                                  \
		-showroot no                                      \
		;
	set itemHeight [font metrics [$t cget -font] -linespace]
	if {$itemHeight < 18} { set itemHeight 18 }
	set Vars(itemheight) $itemHeight
	$t configure -itemheight $itemHeight

	if {[llength $opts(-buttonimage)]} {
		$t configure -buttonimage $opts(-buttonimage)
	}
	if {$opts(-selectmode) eq "browse"} {
		$t state define browse
	}

	$t state define hilite
	$t column create -tags item
	$t configure -treecolumn item
	$t element create elemImg image
	$t element create elemTxt text -lines 1
	$t element create elemSel rect -fill [list                         \
		[::colors::lookup treetable,selected:focus]  {selected focus}   \
		[::colors::lookup treetable,selected!focus]  {selected !focus}  \
		[::colors::lookup treetable,active:focus]    {active focus}     \
		[::colors::lookup treetable,hilite!selected] {hilite !selected} \
	]
	$t element create elemBrd border \
		-filled no \
		-relief raised \
		-thickness 1 \
		-background {#e5e5e5 {active focus} {} {}} \
		;
	$t style create styText
	$t style elements styText {elemSel elemBrd elemImg elemTxt}
	$t style layout styText elemImg -expand ns -padx {2 2}
	$t style layout styText elemTxt -padx {2 2} -expand ns -squeeze x
	$t style layout styText elemSel -union {elemTxt} -iexpand nes -ipadx {2 2}
	$t style layout styText elemBrd -union {elemTxt} -iexpand nes -ipadx {2 2} -detach yes

	$t notify install <Elem-enter>
	$t notify install <Elem-leave>
	$t notify bind $t <Elem-enter>	[namespace code [list VisitElem $t enter %I %E]]
	$t notify bind $t <Elem-leave>	[namespace code [list VisitElem $t leave %I %E]]
	$t notify bind $t <Expand-after>	[namespace code [list ExpandAfter $t %I]]
	$t notify bind $t <Selection>  	[namespace code [list Selection $t %S]]

	ttk::scrollbar $f.sh -orient horizontal -command [list $t xview]
	$t notify bind $f.sh <Scroll-x> { ::scrolledframe::sbset %W %l %u }
	bind $f.sh <ButtonPress-1> [list focus $t]
	ttk::scrollbar $f.sv -orient vertical -command [list $t yview]
	$t notify bind $f.sv <Scroll-y> { ::scrolledframe::sbset %W %l %u }
	bind $f.sv <ButtonPress-1> [list focus $t]

	grid $t    -row 0 -column 0 -sticky nsew
	grid $f.sh -row 1 -column 0 -sticky ew
	grid $f.sv -row 0 -column 1 -sticky ns
	grid columnconfigure $f 0 -weight 1
	grid rowconfigure $f 0 -weight 1

	catch { rename ::$path $path.__t__ }
	proc ::$path {command args} "[namespace current]::WidgetProc $t \$command {*}\$args"

	return $path
}


proc WidgetProc {t command args} {
	variable ${t}::Vars

	switch -- $command {
		add {
			if {1 > [llength $args]} {
				error "wrong # args: should be \"[namespace current] add <depth> ?<args>?\""
			}
			lassign $args depth text
			if {![info exists Vars($depth:lastchild)]} {
				error "item with depth \"$depth\" has no parent"
			}
			array set opts {
				-collapse	1
				-enabled		1
				-text			""
				-icon			""
				-fill			{}
				-tags			{}
				-tag			{}
			}
			array set opts [lrange $args 1 end]
			set collapse $opts(-collapse)
			set enabled $opts(-enabled)
			set tags $opts(-tags)
			set icon $opts(-icon)
			if {[llength $tags] == 0} { set tags $opts(-tag) }
			array unset opts -collapse
			array unset opts -enabled
			array unset opts -icon
			array unset opts -tags
			array unset opts -tag
			if {[llength $opts(-fill)] == 0} {
				set opts(-fill) [list black enabled \
					[::colors::lookup $Vars(disabledforeground)] !enabled]
			}
			set args [array get opts]
			set item [$t item create -button auto -tags $tags]
			if {$collapse} { set action collapse } else { set action expand }
			$t item $action $item
			$t item style set $item item styText
			$t item element configure $item item elemTxt {*}$args
			if {[string length $icon]} { $t item element configure $item item elemImg -image $icon }
			$t item enabled $item $enabled
			$t item lastchild $Vars($depth:lastchild) $item
			set Vars([expr {$depth + 1}]:lastchild) $item
			return $item
		}

		resize {
			return [ComputeWidth $t]
		}

		select {
			if {[llength $args] < 1} {
				error "wrong # args: should be \"[namespace current] select <item>\""
			}
			set item [$t item id [lindex $args 0]]
			set parent [$t item parent $item]
			while {[llength $parent]} {
				$t item expand $parent
				set parent [$t item parent $parent]
			}
			$t selection clear
			$t selection add $item
			$t activate $item
			$t see $item
			return
		}

		clear {
			return [$t item delete all]
		}

		activate {
			if {[llength $args] < 1} {
				error "wrong # args: should be \"[namespace current] activate <item>\""
			}
			return [$t activate [lindex $args 0]]
		}

		enable {
			if {[llength $args] < 1} {
				error "wrong # args: should be \"[namespace current] enable <item>\""
			}
			set item [$t item id [lindex $args 0]]
			$t item enabled $item 1
			return
		}

		disable {
			if {[llength $args] < 1} {
				error "wrong # args: should be \"[namespace current] enable <item>\""
			}
			set item [$t item id [lindex $args 0]]
			$t item enabled $item 0
			return
		}

		collapse {
			return [$t collapse -recurse root]
		}

		expand {
			return [$t expand -recurse root]
		}

		itemheight? {
			return $Vars(itemheight)
		}

		bind {
			if {[llength $args] > 2} {
				error "wrong # args: should be \"bind ?pattern? ?command?\""
			}
			return [bind $t {*}$args]
		}
	}

	return [$t $command {*}$args]
}


proc ComputeWidth {t} {
	set w [winfo parent $t]
	grid propagate $w no
	$t expand -recurse root
	$t column optimize item
	set width [$t column cget item -width]
	if {[llength $width] == 0} { set width [$t column width item] }
	incr width 20 ;# should fit all scrollbar themes
	$w configure -width $width
	set w [winfo parent $t]
	$w configure -width $width
	$t collapse -recurse root
}


proc VisitElem {t mode item elem} {
	if {[string length $item] == 0} { return }
	if {![$t item enabled $item]} { return }
	if {$elem ne "elemTxt" && $elem ne "elemBrd"} { return }

	switch $mode {
		enter {
			foreach i [$t item children root] { $t item state set $i {!hilite} }
			catch { $t item state set $item {hilite} }
		}

		leave { catch { $t item state set $item {!hilite} } }
	}
}


proc ExpandAfter {t item} {
	set lastchild [$t item lastchild $item]
	if {[llength $lastchild]} {
		$t see [$t item lastchild $item]
		$t see $item
	}
}


proc Selection {t item} {
	if {[llength $item] > 0} {
		set parent [winfo parent [winfo parent $t]]
		event generate $parent <<TreeTableSelection>> -data [$t item tag names $item]
	}
}


proc SelectStyle {t x y} {
	set id [$t identify $x $y]
	if {[string length $id] == 0} { return }
	if {[lindex $id 0] eq "header"} { return }
	set item [lindex $id 1]
	$t selection clear
	$t selection add $item
}


proc UpDown {w item n} {
	set rnc [$w item rnc $item]
	if {$rnc eq ""} {
		return [$w item id {first visible state enabled}]
	}
	if {$n > 0} {
		set next [$w item nextsibling $item]
		if {[llength $next] > 0} { return $next }
	} else {
		set prev [$w item prevsibling $item]
		if {[llength $prev] > 0} { return $prev }
	}
	return ""
}

namespace eval icon {
namespace eval 16x16 {

set collapse [image create photo -data {
	R0lGODlhEAAQALIAAAAAAAAAMwAAZgAAmQAAzAAA/wAzAAAzMyH5BAUAAAYALAAAAAAQABAA
	ggAAAGZmzIiIiLu7u5mZ/8zM/////wAAAAMlaLrc/jDKSRm4OAMHiv8EIAwcYRKBSD6AmY4S
	8K4xXNFVru9SAgAh/oBUaGlzIGFuaW1hdGVkIEdJRiBmaWxlIHdhcyBjb25zdHJ1Y3RlZCB1
	c2luZyBVbGVhZCBHSUYgQW5pbWF0b3IgTGl0ZSwgdmlzaXQgdXMgYXQgaHR0cDovL3d3dy51
	bGVhZC5jb20gdG8gZmluZCBvdXQgbW9yZS4BVVNTUENNVAAh/wtQSUFOWUdJRjIuMAdJbWFn
	ZQEBADs=
}]

set expand [image create photo -data {
	R0lGODlhEAAQALIAAAAAAAAAMwAAZgAAmQAAzAAA/wAzAAAzMyH5BAUAAAYALAAAAAAQABAA
	ggAAAGZmzIiIiLu7u5mZ/8zM/////wAAAAMnaLrc/lCB6MCkC5SLNeGR93UFQQRgVaLCEBas
	G35tB9Qdjhny7vsJACH+gFRoaXMgYW5pbWF0ZWQgR0lGIGZpbGUgd2FzIGNvbnN0cnVjdGVk
	IHVzaW5nIFVsZWFkIEdJRiBBbmltYXRvciBMaXRlLCB2aXNpdCB1cyBhdCBodHRwOi8vd3d3
	LnVsZWFkLmNvbSB0byBmaW5kIG91dCBtb3JlLgFVU1NQQ01UACH/C1BJQU5ZR0lGMi4wB0lt
	YWdlAQEAOw==
}]

} ;# namespace 16x16
} ;# namespace icon
} ;# namespace treetable


ttk::copyBindings TreeCtrl TreeTable

bind TreeTable <Motion> {
	TreeCtrl::MotionInItems %W %x %y
	TreeCtrl::MotionInElems %W %x %y
}

bind TreeTable <Leave> {
	TreeCtrl::MotionInItems %W
	TreeCtrl::MotionInElems %W
}

bind TreeTable <ButtonPress-1> {
	set id [%W identify %x %y]
	if {[llength $id] == 0} { return }
	lassign $id where item arg1 _ _ elem
	if {$arg1 eq "button"} {
		TreeCtrl::ButtonPress1 %W %x %y
	} elseif {$where eq "item" && $arg1 eq "column" && ($elem eq "elemTxt" || $elem eq "elemBrd")} {
		TreeCtrl::ButtonPress1 %W %x %y
		%W selection clear
		%W selection add $item
	}
}

bind TreeTable <Double-Button-1> {
	set id [%W identify %x %y]
	if {[llength $id] == 0} { return }
	set item [lindex $id 1]
	if {[%W item isopen $item]} { set op collapse } else { set op expand }
	%W item $op $item
}

bind TreeTable <KeyPress-Left>	{ %W item collapse [%W item id active] }
bind TreeTable <KeyPress-Right>	{ %W item expand [%W item id active] }

bind TreeTable <Shift-Up> {
	set item [::treetable::UpDown %W active -1]
	if {$item ne ""} {
		%W activate $item
		if {"browse" in [%W state names]} {
			%W selection clear
			%W selection add $item
		}
		%W see active
	}
	break
}

bind TreeTable <Shift-Down> {
	set item [::treetable::UpDown %W active +1]
	if {$item ne ""} {
		%W activate $item
		if {"browse" in [%W state names]} {
			%W selection clear
			%W selection add $item
		}
		%W see active
	}
	break
}

bind TreeTable <Up> {
	set item [TreeCtrl::UpDown %W active -1]
	if {$item ne ""} {
		%W activate $item
		if {"browse" in [%W state names]} {
			%W selection clear
			%W selection add $item
		}
		%W see active
	}
	break
}

bind TreeTable <Down> {
	set item [TreeCtrl::UpDown %W active +1]
	if {$item ne ""} {
		%W activate $item
		if {"browse" in [%W state names]} {
			%W selection clear
			%W selection add $item
		}
		%W see active
	}
	break
}

bind TreeTable <Prior> {
	set item [%W item id {nearest 0 0}]
	if {$item ne "" && $item ne [%W item id active]} {
		%W activate $item
	} else {
		%W yview scroll -1 pages
		set item [%W item id {nearest 0 0}]
		if {$item ne ""} { %W activate $item }
	}
}

bind TreeTable <Next> {
	set h [expr {[winfo height %W] - 5}]
	set item [%W item id [list nearest 0 $h]]
	if {$item ne "" && $item ne [%W item id active]} {
		%W activate $item
	} else {
		%W yview scroll 1 pages
		set item [%W item id [list nearest 0 $h]]
		if {$item ne ""} { %W activate $item }
	}
}

bind TreeTable <Home> {
	%W yview moveto 0
	set item [%W item id {nearest 0 0}]
	if {$item ne ""} { %W activate $item }
}

bind TreeTable <End> {
	%W yview moveto 1
	set h [expr {[winfo height %W] - 5}]
	set item [%W item id [list nearest 0 $h]]
	if {$item ne ""} { %W activate $item }
}

bind TreeTable <space> {
	 %W selection clear
	 %W selection add active
	 break
}

bind TreeTable <Return> {
	 %W selection clear
	 %W selection add active
	 break
}

# vi:set ts=3 sw=3:
