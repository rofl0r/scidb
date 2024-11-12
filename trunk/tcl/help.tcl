# ======================================================================
# Author : $Author$
# Version: $Revision: 169 $
# Date   : $Date: 2012-01-04 03:39:00 +0000 (Wed, 04 Jan 2012) $
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

namespace eval help {
namespace eval mc {

set Help					"Help"
set Contents			"Contents"
set GoBack				"Go back one page (Alt-Left)"
set GoForward			"Go forward one page (Alt-Right)"
set ExpandAllItems	"Expand all items"
set CollapseAllItems	"Collapse all items"

} ;# namespace mc

variable Geometry {}


proc build {parent} {
	variable Priv
	variable Links
	variable Geometry

	array unset Links
	array set Links {}
	set Priv(topic) ""

	set dlg $parent.help
	if {[winfo exists $dlg]} {
		raise $dlg
		focus $dlg
		return
	}
	set Priv(dlg) $dlg

	toplevel $dlg -class Scidb
	wm protocol $dlg WM_DELETE_WINDOW [list destroy $dlg]
	wm withdraw $dlg

	set top [ttk::frame $dlg.top]
	pack $top -fill both -expand yes

	set pw [tk::panedwindow $top.pw -orient horizontal]
	::theme::configurePanedWindow $top.pw
	pack $pw -expand yes -fill both

	### Left side ########################################
	set control [ttk::frame $pw.control]

	set buttons [ttk::frame $control.buttons]
	ttk::button $buttons.back \
		-image $::icon::16x16::controlBackward \
		-command [namespace code GoBack] \
		-state disabled \
		;
	::tooltip::tooltip $buttons.back [namespace current]::mc::GoBack
	set Priv(button:back) $buttons.back
	ttk::button $buttons.forward \
		-image $::icon::16x16::controlForward \
		-command [namespace code GoForward] \
		-state disabled \
		;
	::tooltip::tooltip $buttons.forward [namespace current]::mc::GoForward
	ttk::separator $buttons.sep -orient vertical
	ttk::button $buttons.expand \
		-image $icon::16x16::collapse \
		-command [namespace code ExpandAllItems] \
		;
	::tooltip::tooltip $buttons.expand [namespace current]::mc::ExpandAllItems
	ttk::button $buttons.collapse \
		-image $icon::16x16::expand \
		-command [namespace code CollapseAllItems] \
		;
	::tooltip::tooltip $buttons.collapse [namespace current]::mc::CollapseAllItems
	set Priv(button:forward) $buttons.forward
	grid $buttons.back     -row 1 -column 1
	grid $buttons.forward  -row 1 -column 3
	grid $buttons.sep      -row 1 -column 5 -sticky ns
	grid $buttons.expand   -row 1 -column 7
	grid $buttons.collapse -row 1 -column 9
	grid columnconfigure $buttons {0 2 4 6 8 10} -minsize $::theme::padding
	grid rowconfigure $buttons {0 2} -minsize $::theme::padding

	set nb [ttk::notebook $control.nb -takefocus 1 -width 300]
#	bind $nb <<NotebookTabChanged>> [namespace code [list TabChanged $nb]]
	BuildContentsFrame $nb.contents
	$nb add $nb.contents -sticky nsew -padding $::theme::padding
	::widget::notebookTextvarHook $nb 0 [namespace current]::mc::Contents

	grid $buttons -row 1 -column 1 -sticky w
	grid $nb      -row 3 -column 1 -sticky nsew
	grid columnconfigure $control 1 -weight 1
	grid rowconfigure $control 3 -weight 1

	### Right side #######################################
	set html $pw.html
	set Priv(html) $html
	BuildHtmlFrame $html

	bind $dlg <Alt-Left>		[namespace code GoBack]
	bind $dlg <Alt-Right>	[namespace code GoForward]

	$pw add $control -sticky nswe -stretch never  -minsize 100
	$pw add $html    -sticky nswe -stretch always -minsize 400

	wm deiconify $dlg
	bind $dlg <Configure> [namespace code [list RecordGeometry $dlg]]
	if {[llength $Geometry] == 0} {
		update idletasks
		set Geometry [winfo width $dlg]x[winfo height $dlg]
	}
	wm geometry $dlg $Geometry
	wm minsize $dlg 430 300
	bind $nb <<LanguageChanged>> [namespace code UpdateTitle]
	after idle [focus $Priv(contents)]

	ReloadCurrentPage
}


proc UpdateTitle {} {
	variable Priv

	append title $::scidb::app
	append title ": "
	append title $mc::Help

	if {[string length $Priv(topic)]} {
		append title " - "
		append title $Priv(topic)
	}

	wm title $Priv(dlg) $title
}


proc ExpandAllItems {} {
	variable Priv
	$Priv(contents) expand -recurse root
}


proc CollapseAllItems {} {
	variable Priv
	$Priv(contents) collapse -recurse root
}


proc BuildHtmlFrame {w} {
	::html $w \
		-imagecmd [namespace code GetImage] \
		-center no \
		-width 600 \
		-height 800 \
		-cursor left_ptr \
		-borderwidth 1 \
		-relief sunken \
		;
	bind $w <<LanguageChanged>> [namespace code ReloadCurrentPage]

	$w handler node link [namespace current]::LinkHandler
	$w handler node a    [namespace current]::A_NodeHandler

	$w onmouseover [namespace current]::MouseEnter
	$w onmouseout	[namespace current]::MouseLeave
	$w onmouseup1	[namespace current]::Mouse1Up
}


proc BuildContentsFrame {w} {
	variable Priv

	ttk::frame $w

	set t $w.tree
	set Priv(contents) $t
	set images [list $icon::16x16::collapse open $icon::16x16::expand {}]
	treectrl $t \
		-takefocus 1 \
		-highlightthickness 0 \
		-borderwidth 1 \
		-relief sunken \
		-showheader no \
		-showbuttons yes \
		-buttonimage $images \
		-showroot no \
		-showlines no \
		-xscrollincrement 1 \
		-background white \
		-linestyle solid \
		;
	bind $t <<LanguageChanged>> [namespace code [list UpdateContents $t]]
	set height [font metrics [$t cget -font] -linespace]
	if {$height < 18} { set height 18 }
	$t configure -itemheight $height

	$t state define hilite
	$t column create -tags item
	$t configure -treecolumn item
	$t element create elemImg image
	$t element create elemTxt text -lines 1
	$t element create elemSel rect \
		-fill {	\#ffdd76 {selected focus}
					\#f2f2f2 {selected !focus}
					\#ebf4f5 {active focus}
					\#f0f9fa {selected hilite}
					\#f0f9fa {hilite}} \
		;
	$t element create elemBrd border \
		-filled no \
		-relief raised \
		-thickness 1 \
		-background {#e5e5e5 {active focus} {} {}} \
		;

	$t style create styText
	$t style elements styText {elemSel elemBrd elemImg elemTxt}
	$t style layout styText elemImg -expand ns -padx {2 0}
	$t style layout styText elemTxt -padx {6 2} -expand ns -squeeze x
	$t style layout styText elemSel -union {elemTxt} -iexpand nsew
	$t style layout styText elemBrd -iexpand xy -detach yes

	$t notify install <Item-enter>
	$t notify install <Item-leave>
	$t notify bind $t <Item-enter> [namespace code [list VisitItem $w $t enter %I]]
	$t notify bind $t <Item-leave> [namespace code [list VisitItem $w $t leave %I]]
	$t notify bind $t <Selection>  [namespace code [list LoadPage $t %S]]

	bind $t <ButtonPress-1> {+
		set id [%W identify %x %y]
		if {[llength $id] == 0} { return }
		lassign $id where item arg1
		if {$where ne "item"} { return }
		if {$arg1 ne "column"} { return }
		%W selection clear
		%W selection add $item
	}
	bind $t <KeyPress-Up> {
		set item [TreeCtrl::UpDown %W active -1]
		if {$item eq ""} return
		%W activate $item
		%W see active
		break
	}
	bind $t <KeyPress-Down> {
		set item [TreeCtrl::UpDown %W active +1]
		if {$item eq ""} return
		%W activate $item
		%W see active
		break
	}
	bind TreeCtrl <KeyPress-space> {
		 %W selection clear
		 %W selection add active
	}
	bind TreeCtrl <KeyPress-Return> {
		 %W selection clear
		 %W selection add active
	}
	bind $t <KeyPress-Left>  { %W item collapse [%W item id active] }
	bind $t <KeyPress-Right> { %W item expand [%W item id active] }

	ttk::scrollbar $w.sh -orient horizontal -command [list $t xview]
	$t notify bind $w.sh <Scroll-x> { ::scrolledframe::sbset %W %l %u }
	bind $w.sh <ButtonPress-1> [list focus $t]
	ttk::scrollbar $w.sv -orient vertical -command [list $t yview]
	$t notify bind $w.sv <Scroll-y> { ::scrolledframe::sbset %W %l %u }
	bind $w.sv <ButtonPress-1> [list focus $t]

	grid $t    -row 0 -column 0 -sticky nsew
	grid $w.sh -row 1 -column 0 -sticky ew
	grid $w.sv -row 0 -column 1 -sticky ns
	grid columnconfigure $w 0 -weight 1
	grid rowconfigure $w 0 -weight 1

	UpdateContents $t
}


proc VisitItem {w t mode item} {
	if {[string length $item] == 0} { return }

	switch $mode {
		enter {
			foreach i [$t item children root] { $t item state set $i {!hilite} }
			catch { $t item state set $item {hilite} }
		}

		leave { catch { $t item state set $item {!hilite} } }
	}
}


proc LoadPage {t item} {
	variable Priv
	variable Links

	if {[info exists Priv(uri:$item)]} {
		set file [$Priv(uri:$item) path]
		set fragment [$Priv(uri:$item) fragment]
		set Links($file) 1
		Load $file $fragment
	}
}


proc UpdateContents {t} {
	variable Priv

	set Contents {}
	set file [file normalize [file join $::scidb::dir::help $::mc::langID Contents.dat]]
	catch { source -encoding utf-8 $file }
	foreach name [array names Priv uri:*] { $Priv($name) destroy }
	array unset Priv uri:*
	$t item delete all
	FillContents $t 0 root $Contents
	catch { $t activate 1 }
}


proc FillContents {t depth root contents} {
	variable Priv

	foreach group $contents {
		set lastchild $root
		foreach entry $group {
			set count 0
			if {[llength $entry] == 1} {
				set topic [lindex $entry 0]
				set item [$t item create -button auto -tags $depth.$root.$count]
				set fill {}
				if {[llength $topic] > 0} {
					set file [file normalize [file join $::scidb::dir::help $::mc::langID [lindex $topic 1]]]
					if {![file readable $file]} {
						lappend fill -fill #999999
					} elseif {[llength $topic] > 2} {
						set Priv(uri:$item) [::tkhtml::uri $file#[lindex $topic 2]]
					} else {
						set Priv(uri:$item) [::tkhtml::uri $file]
					}
				}
				set title [lindex $topic 0]
				if {[llength $topic] == 2} {
					set Priv(topic:$file) $title
					if {$item == 1} { set icon library } else { set icon document }
					set icon [set icon::16x16::$icon]
				} else {
					$t item collapse $item
					set icon [list $icon::16x16::bookClosed {!open} $icon::16x16::bookOpen {open}]
				}
				$t item style set $item item styText
				$t item element configure $item item elemTxt -text $title {*}$fill
				$t item lastchild $lastchild $item
				$t item element configure $item item elemImg -image $icon
				if {[llength $topic] != 2} { set lastchild $item }
				incr count
			} else {
				FillContents $t [expr {$depth + 1}] $root $entry
			}
		}
	}
}


proc RecordGeometry {dlg} {
	set [namespace current]::Geometry [wm geometry $dlg]
}


proc GetImage {file} {
	set file [file normalize [file join $::scidb::dir::help $::mc::langID $file]]
	if {[catch { set img [image create photo -file $file] }]} {
		set src $icon::16x16::broken
		set img [image create photo -width [image width $src] -height [image height $src]]
		$img copy $src
	}
	return $img
}


proc A_NodeHandler {node} {
	variable Nodes
	variable Links

	$node dynamic set link
	set Nodes($node) 0

	set href [$node attribute -default {} href]
	if {[llength $href]} {
		set file [file normalize [file join $::scidb::dir::help $::mc::langID $href]]
		if {[info exists Links($file)]} { $node dynamic set visited }
	}
}


proc LinkHandler {node} {
	if {[$node attribute rel] eq "stylesheet"} {
		variable Priv

		ImportHandler author [$node attribute href]

		# overwrite CSS values
		set css "
			:link    { color: blue2; text-decoration: none; }
			:visited { color: purple; text-decoration: none; }
			:hover   { text-decoration: none; background: #ffff00; }
		"
		$Priv(html) style -id user $css
	}
}


proc ImportHandler {parentid uri} {
	variable _StyleCount
	variable Priv

	set file [file join $::scidb::dir::help $::mc::langID $uri]
	set fd [open $file r]
	set content [read $fd]
	close $fd

	set id "$parentid.[format %.4d [incr _StyleCount]]"
	set handler [namespace code [list ImportHandler $id]]
	$Priv(html) style -id $id.9999 -importcmd $handler $content
}


proc MouseEnter {node} {
	variable Nodes
	variable Priv

	if {[llength $node] == 0} { return }

	if {[info exists Nodes($node)]} {
		$node dynamic set hover
		[$Priv(html) drawable] configure -cursor hand2
	}
}


proc MouseLeave {node} {
	variable Nodes
	variable Priv

	if {[llength $node] == 0} { return }

	if {[info exists Nodes($node)]} {
		$node dynamic clear hover
		[$Priv(html) drawable] configure -cursor {}
	}
}


proc Mouse1Up {node} {
	variable Nodes
	variable Links
	variable Priv

	if {[llength $node] == 0} { return }

	if {[info exists Nodes($node)]} {
		set Nodes($node) 1
		$node dynamic clear link
		$node dynamic set visited

		set href [$node attribute -default {} href]
		if {[llength $href]} {
			set fragment ""
			lassign [split $href \#] href fragment
			set file [file normalize [file join $::scidb::dir::help $::mc::langID $href]]
			set Links($file) 1
			Load $file $fragment
		}
	}
}


proc ReloadCurrentPage {} {
	variable Priv

	set file ""
	if {[info exists Priv(lastfile)] && [string length $Priv(lastfile)] > 0} {
		set file $Priv(lastfile)
	}

	if {	[string length $file] == 0
		|| ![file readable [file join $::scidb::dir::help $::mc::langID [file tail $file]]]} {
		set file Overview.html
	}

	set Priv(history) {}
	set Priv(history:index) -2
	set Priv(lastfile) ""

	Load [file normalize [file join $::scidb::dir::help $::mc::langID [file tail $file]]]
}


proc Goto {position} {
	variable Priv

	if {[string length $position] == 0} {
		set position 0
	} elseif {![string is integer -strict $position]} {
		if {[string index $position 0] eq "#"} { set position [string range $position 1 end] }
		set selector [format {[name="%s"]} $position]
		set node [lindex [$Priv(html) search $selector] 0]
		if {[llength $node] == 0} { return }
		set position [lindex [$Priv(html) bbox $node] 1]
	}

	$Priv(html) scrollto $position
}


proc Load {file {position {}}} {
	variable Priv

	if {$Priv(lastfile) eq $file} { return [Goto $position] }

	Parse $file
	Goto $position

	incr Priv(history:index)
	set Priv(history) [lrange $Priv(history) 0 $Priv(history:index)]
	lappend Priv(history) [list $file 0]
	SetupButtons
}


proc Parse {file {moveto 0}} {
	variable Nodes
	variable Priv

	if {$Priv(lastfile) eq $file} { return }

	set index [expr {$Priv(history:index) + 1}]
	if {$index >= 0 && $index < [llength $Priv(history)]} {
		lset Priv(history) $index 1 [lindex [$Priv(html) yview] 0]
		SetupButtons
	}

	array unset Nodes
	array set Nodes {}
	set content "<>"

	if {[file readable $file]} {
		catch {
			set fd [open $file r]
			chan configure $fd -encoding utf-8
			set content [read $fd]
			close $fd
		}
	}

	if {$content eq "<>"} {
		set content "
			<html><body>
			<h1>File not found</h1>
			<p>Can't find the file at <b>$file</b>.</p>
			</body></html>
		"
	}

	[$Priv(html) drawable] configure -cursor {}
	$Priv(html) parse $content
	$Priv(html) yview moveto $moveto
	set Priv(lastfile) $file

	set Priv(topic) ""
	catch { set Priv(topic) $Priv(topic:$file) }
	UpdateTitle
}


proc GoBack {} {
	variable Priv

	if {$Priv(history:index) >= 0} {
		Parse {*}[lindex $Priv(history) $Priv(history:index)]
		incr Priv(history:index) -1
		SetupButtons
	}
}


proc GoForward {} {
	variable Priv

	if {$Priv(history:index) + 2 < [llength $Priv(history)]} {
		Parse {*}[lindex $Priv(history) [expr {$Priv(history:index) + 2}]]
		incr Priv(history:index)
		SetupButtons
	}
}


proc SetupButtons {} {
	variable Priv

	if {$Priv(history:index) + 2 < [llength $Priv(history)]} {
		set state normal
	} else {
		set state disabled
	}
	$Priv(button:forward) configure -state $state

	if {$Priv(history:index) >= 0} {
		set state normal
	} else {
		set state disabled
	}
	$Priv(button:back) configure -state $state
}


namespace eval icon {
namespace eval 16x16 {

set broken [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QAAAAAAAD5Q7t/AAAA
	CXBIWXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH1QgYESoxCKsW2AAAAxJJREFUOMuNk11sk3UU
	xn//frzv2m6jQ1oHONaNZXU1XhiZm4yQiRCjTV00cdWl0yVIYsICTaaZroORDAwzkRgMXHGD
	M35gNMo2y0TDdEv8QIMG7ULUFu1gwQwiYWxr37c93rQTEi98rs55Ts6TnPOco0QEpVS/z+fb
	4Ha7dQARAVCFmirkmKZBIjH9tYjsowBbMUilUm38D/z4w9m1BdGB2wQAhr9I0vVqnJVlGs4S
	O+VOOw5HCVkDylw6vspShntb75mamrIqpSwisqcooAGs12eIdmykrfFO8qaBkRMsuovmwB18
	8tZRWsPdALS0tNydTP7+tFJK2QD3S7HYC/VOJ5bsIofGT/PzFQ+XZ69i0xy0b/HgtVtp1DW+
	/PYnHt1xgLGxcTZULtTV1tY2Et29+3IlyK/hsEyH26XBoclHI6PS/d4l+S59Q0qtyKmtD8ul
	ri7ZUV0l0e0RiX34p4iI+P3+uEXT9aUMML+0hOglvBtqY+8zT1G/GGfb+hWcaN3CujVruWkY
	XM8sUe2rAfIAKKXEMjQ0dG/34OBMaGKCq3NzaCUO3gk+zkhsL+8/tA3fXVWIYbDr8894sLcX
	b/Blspns8uKLXu8TkYEKTWMkGMSzejUZpbAB1kyGXfHTPHvgNQKPhZmdgWvzN4lsdhEIBD4t
	umACjF5IEKzzcyyfZ7Hg8SjgeG475xvOE3ujH7tpsqAvENl85bZDsgFsrW/gzXwevUBagTAQ
	O36c9kdOwBOKC8lfuH7j7+URLICrf//+5yvsdo6YJlUOBwADNhuzgO50chCTgx1PUv1NDS7n
	Cs6mvi/2Cz09PSkFMr5qlUx3dspXDQFZAzIcH5fy0pVyzOGSRCQi55qbZSPIkcHD8sChJhER
	CQQCo5acaeo2oGbTJiy5HKHpBEfPnGIs9zbxP86wM5vl3MQEdaEQpUBy5jea/E3/3r+IuF/s
	6/urDKQC5L9QrmmyDqSvf0/uVt7r9X6gRASPx7OzMxJ5xTQMx8cnT76eTqfnl31Wimg02nFt
	bu6+5MWLhycnJ9O3/N/9/wBvyWHwrbQl3wAAAABJRU5ErkJggg==
}]

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

set bookOpen [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAGXRFWHRTb2Z0d2FyZQBBZG9i
	ZSBJbWFnZVJlYWR5ccllPAAAAkdJREFUeNqUUz1vE0EQfbt3e3ZwnICDk2AXSEFBIEsERIVE
	R0eBKxoq6ggqGv4H0KcBRBco+BA9KBJSIkcxYAkJGzCJDIY4Ocucb3eY3bOJQSiI0Y20HzNv
	35uZEydvPseDywFyfhe19y3M5Cdp7sSZzWajdiM7kb23sfEWvueDIACjYcigVCphfUvj2qNv
	kBgYEVnPkTFIH8rPzi1cuKu1fqENrWhNxOse+6LR8VljaJgGn5fLvC/bw+L0OCLNJwxioWeL
	x859b7xCrjAPNX4kFYvMnTDcBWNBG8kPiocWoFxI7eJwfgrdcAfvPnUsHcsJYOqSYk4wGPMV
	JqcKyKsDfLUNFa3AQJSdBKYGy8o4Gdgzcp+TZ5iV4RpwFCcY/mJ3yQACFMcYzdvXBoGGcyBE
	UkTNlRX4PzMszZoDIB3/fiss3D6QfGW7NQBgCVYPBkk2WXj/ABKJBAdg0biIw7jsmMHH1y/R
	7XzhLijGEH+FIFdQngOnx9ga2JclDmYCRL0mmpUacsVT8GwrPQ9SegwmXYxllnSEiyh5o5SE
	DHyknCtEkUZ3p4ftz1XX2q9bLfzoduAF6eR5mXZxnAxfsdx6dRXVSoWDRUKZXVtG7UIyVOE6
	Gm/WmOp9NxNSSmQU8T9SZgDEncV6eUJzMo0UqU8SS8efud3VD5eghB5tAjxBCNDv2PXC0YvX
	T5Pu6+Ffpfu9aOb8ldtaBNPuKGw9ba8+WZI+axSuNdwo5dUf31obFjizN2O/5m2eXQ32bfbN
	P3pq1+FPAQYA17kRbxpFka4AAAAASUVORK5CYII=
}]

set bookClosed [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAByFBMVEUfUZIWY7YbS4kUR4Yb
	S4gbS4kbTIkbTYoKSZNMs99nveNdvORau+MRRIASQHllxehmxehnxehrxulvx+l3y+sPQ4MS
	UZYXV5kijcQlnNAqns0xo9QxrNg8qtg9r9pJsdtJtd1VuN9XuuBjwONkwORuxOZ0x+h3yeh+
	zeuAzOoNQIB+zesMN2sLNmoLNmscR3slUIO43usAMnEANHMANXMBNXMHSY8HSpAHS5AdisAd
	l8cejsIfkcQfk8UflMUglsYgmsgkpNIlnc8no9IoqNQpq9YprNYprdc0qNU2qNU5rtg6sNg7
	s9o8tds8tttAZ5dFsdpIstpLt91Ofq9OvN9PvuBPv+BXut9au99dsdZewOFgut5hxONjxuRo
	vOBowuRtxeVvx+ZxweJyyedzy+d1zOh1zeh3yuh6k7B6xuV+zuqC0OuDyuiD0OyD0euE0euG
	0+yH0+yI0+2Kz+qK1vCL0euL1O2L2vOM1e6M1/CM2vKM2vON1e6N2vON2/OO2PCP1u6P2PCP
	2/OQ2PCR0uyS2fGU1OyV2e+V3fOX2vCY2/Kas82d4PSh3vOq1+mw4PC24vG95fPE6PTK6/bQ
	7ffS6/TU7/j///9grXZJAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxMA
	AAsTAQCanBgAAAAHdElNRQfcAQMVICLc9PR3AAAAoUlEQVQY02NgYBAVl5KGAQYgMMjo7uuf
	MHHS5CnTpoIF9P2DI6PjkjJzilvawAK6RmZW0sh69IwtHNy8g6ISs4oqq8ACJub27r4hMSnZ
	ZfXVUAE7V7/Q2NS8ikYkgbDY1PyaxlqiBWwxBXxDY9Pya5rroAI2Lj4hMakFDa1NUAFrZ6+g
	qOTC9q4OkICOqZmlk2dgREJ2Z28P2DNaYhJIXgEAYWlBFIyfP+wAAAAASUVORK5CYII=
}]

set document [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAABFElEQVQYGQXBPWpUURgA0HPf
	uzOJ5A/RwmARgnbBMuAGbKOCTSrrbMQma3AfVmKnGIJ1bBUcmYGMiTrMvDvfl3MKH5cxAND1
	vy9P35gA2H7dsmXLli1btlzk93z/1S5AlVwZ+2NLCE/98/Z49eHsxAxsv2r5K6c5zVlOc5JD
	XuQkW55/9gg6uDY3NTN3oxj8dOnl8/0TqHCoGIykIh1rmntKDxW+KEInPDFyo7NwBKhwoLcy
	EjbteQAAKjxULWyimbv1F88AFS4UoRMOjP1QdQhQYV+1VDUjW450AkCFXb0dpE7qpR5AXeNK
	qNb+u2+lqA7tWYMKS53BjmqsaDYUAajwQiBAAHpAXXx7/C4CAgBRuP4Edyw3eA24NyyYAAAA
	AElFTkSuQmCC
}]

set library [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA/wD/AP+gvaeTAAAA
	CXBIWXMAAABIAAAASABGyWs+AAACYUlEQVQ4y4VS3UuTURj/Pee8H25rHzVXftS2HG5YmiNq
	EiWBSYkXhQR1EXTVv1C3/QXddi8EQTdCBQ5vKggiM7OyRhM3M52iLF/SOdv7nvd08W7kx6zn
	6jnw/D6e33M46hcpasPRcOLcjXiy/4GqqppRXPwAQAMgdgzWGqYosC3LfagllozEUlfbEsnB
	7u6O+OmuI3o+8yY7ks5O5Wdzq/np9H0AxRpOAcBDQORUs//yZs/AUOzk2dSl7jOBzvaD8Lg4
	cgWJFdYXb0z2xL3x91YwVKp8G58aNmHnIfGLrjf5h/tSnb3R873hhxTnF0+0IhLQMfGjhIXv
	fgiporVFoCvGEG1U8fZjZvPZ68nS19HndxSX66kyeDhw+0L7cSzbwJf8Il5OZNAgBdzRCK4k
	29CfCMKtEQolE09m1jAvPW4Rjuik6zrzHICiCQtLY2nMcxfszlv4XakAwoLFVpAzSnj0TsOc
	SQh4m8EYA2MM0radAKSE4iRIAGPVTAkEhrK5hVfZGWiyAb6WNgS8TWBEe86loArZcZfqkzMG
	LhmICKgDrmnv/gJ7mf5RDKC/5HXm5f8JnBWopki71tnPhZRc9fm2Rbef2/3wtq0ujI2CzW2Z
	S4ZlY0NICLkNRYCsw0qMOWrS5kTElcfLxs1xHQMFTXasRNcHGdd0kHPjmrrjkGCbJsqFBWvt
	86fZrWJxFYDqjGguoFL2USJ1l3uD17iwNCumN7KwL6jZGtyeoAyV+c+NbHZiLTOd3pjLv5BC
	5ACUd3vkAEIAOBLhIX6s6Z4iyJTF9bScL4yYhjEJwABg1wB/ABDT1Vm/mN9CAAAAAElFTkSu
	QmCC
}]

} ;# namespace 16x16
} ;# namespace icon
} ;# namespace help

# vi:set ts=3 sw=3:
