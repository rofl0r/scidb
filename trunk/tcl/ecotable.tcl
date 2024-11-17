# ======================================================================
# Author : $Author: gcramer $
# Version: $Revision: 1507 $
# Date   : $Date: 2018-08-13 12:17:53 +0000 (Mon, 13 Aug 2018) $
# Url    : $URL: https://svn.code.sf.net/p/scidb/code/trunk/tcl/ecotable.tcl $
# ======================================================================

# ======================================================================
#    _/|            __
#   // o\         /    )           ,        /    /
#   || ._)    ----\---------__----------__-/----/__-
#   //__\          \      /   '  /    /   /    /   )
#   )___(     _(____/____(___ __/____(___/____(___/_
# ======================================================================

# ======================================================================
# Copyright: (C) 2018 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

::util::source eco-table

namespace eval application {
namespace eval eco {
namespace eval mc {

set SelectEco		"Select ECO code"

set Mode(single)	"Per ply"
set Mode(compact)	"Transitions only"

set F_Line			"Line"

}


array set Defaults {
	move:figurines	graphic
	move:notation	san
	move:mode		single
	miniboard:size	30
}
array set Options {}

variable IdList {}


proc build {parent args} {
	variable MoveStyle
	variable Defaults
	variable Options
	variable IdList

	array set opts {
		-id "table"
		-takefocus 0
		-fillcolumn end
		-listmode yes
		-sortable no
		-hidebar yes
		-highlightcolor tree,emphasize
		-stripes eco,stripes
	}
	array set opts $args

	set id $opts(-id)
	if {$id ni $IdList} { lappend IdList $id }
	set opts(-id) eco:$id

	namespace eval $parent {}
	variable ${parent}::

	if {![info exists MoveStyle]} {
		trace add variable ::pgn::setup::mc::Setup(MoveStyle) write [namespace code UpdateMoveStyleLabel]
		UpdateMoveStyleLabel
	}

	array set Options [array get Defaults]
	::scrolledtable::bindOptions eco:$id [namespace current]::Options [array names Defaults]
	set tb $parent.table

	lappend col1 \
		-justify left \
		-minwidth 5 \
		-maxwidth 0 \
		-width 10 \
		-stretch 0 \
		-removable 0 \
		-ellipsis 1 \
		-visible 1 \
		-foreground black \
		-textvar [namespace current]::mc::F_Line \
		;
	lappend menu [list radiobutton \
		-command [namespace code [list Update $parent]] \
		-labelvar [namespace current]::mc::Mode(single) \
		-variable [namespace current]::Options(move:mode) \
		-value single \
	]
	lappend menu [list radiobutton \
		-command [namespace code [list Update $parent]] \
		-labelvar [namespace current]::mc::Mode(compact) \
		-variable [namespace current]::Options(move:mode) \
		-value compact \
	]
	lappend menu { separator }
	lappend menu {*}[::notation::buildMenuForShortNotation \
		[namespace code [list ::scrolledtable::refresh $tb]] \
		[namespace current]::Options(move:notation) \
	]
	lappend menu { separator }
	lappend menu [list command \
		-command [namespace code [list SelectFigurines $tb]] \
		-labelvar [namespace current]::MoveStyle \
	]
	lappend col1 -menu $menu
	lappend columns line $col1
	lappend col2 \
		-justify left \
		-minwidth 4 \
		-maxwidth 4 \
		-width 4 \
		-stretch 0 \
		-removable 0 \
		-ellipsis 0 \
		-visible 1 \
		-foreground darkgreen \
		-textvar ::gametable::mc::F_Eco \
		;
	lappend columns eco $col2
	lappend col3 \
		-justify right \
		-minwidth 9 \
		-maxwidth 9 \
		-width 4 \
		-stretch 0 \
		-removable 1 \
		-ellipsis 0 \
		-visible 0 \
		-foreground magenta4 \
		-textvar ::gametable::mc::F_Key \
		;
	lappend columns key $col3
	lappend col4 \
		-justify left \
		-minwidth 10 \
		-maxwidth 0 \
		-width 40 \
		-stretch 1 \
		-removable 1 \
		-ellipsis 1 \
		-visible 1 \
		-foreground black \
		-textvar ::gametable::mc::F_Opening \
		;
	lappend columns opening $col4
	::scrolledtable::build $tb $columns {*}[array get opts]
	::font::registerFigurineFonts movelist
	set specialfont [list [list $::font::figurine(movelist:normal) 9812 9823]]
	::scrolledtable::configure $tb line -specialfont $specialfont
	pack $tb -fill both -expand yes
	bind $tb <<TableFill>> [namespace code [list FillTable $tb %d]]
	bind $tb <<TableVisit>>	[namespace code [list TableVisit $parent $tb %d]]
	bind $tb <<TableSelected>>	[namespace code [list TableSelected $parent $tb %d]]
	bind $tb <<LanguageChanged>> [namespace code [list FillTable $tb %d]]
	set path [::scrolledtable::tablePath $tb]
	bind $path <ButtonPress-1> [namespace code [list TableShow $parent $tb %x %y %s]]
	bind $path <Leave> +[namespace code [list HideBoard $parent]]
	set (lastpos) -1
	set (oldData) {}
	set (data) {}
	set (active) 0
	set (afterid) {}
	set (locked) 0
	set (mode) $Options(move:mode)
	set (update) [list [namespace current]::Update $parent]
	set (listmode) $opts(-listmode)
	return $tb
}


proc open {parent id} {
	variable eco_

	set dlg [tk::toplevel $parent.__eco__]
	set tb [build $dlg -id $id -listmode no -takefocus 1]
	::widget::dialogButtons $dlg {ok cancel}
	$dlg.ok configure -command [namespace code [list CurrentSelection $dlg]] -state disabled
	$dlg.cancel configure -command [list set [namespace current]::eco_ ""]
	pack $tb -expand yes -fill both
	set path [::scrolledtable::tablePath $tb]
	bind $tb <<TableSelected>>	[namespace code [list SelectEco $dlg %d]]
	bind $tb <<TableActivated>> [namespace code [list HandleSelection $dlg %d]]
	bind $dlg <Escape> [list $dlg.cancel invoke]
	::widget::dialogGeometry $dlg $parent 600x400
	wm transient $dlg [winfo toplevel $parent]
	wm withdraw $dlg
	wm title $dlg $mc::SelectEco
	wm resizable $dlg true true
	wm deiconify $dlg
	Update $dlg yes
	::scrolledtable::focus $tb
	::ttk::grabWindow $dlg
	tkwait variable [namespace current]::eco_
	::ttk::releaseGrab $dlg
	destroy $dlg
	return $eco_
}


proc activate {w flag} {
	variable ${w}::

	if {$(active) eq $flag} { return }
	set (active) $flag

	if {$flag} {
		::scidb::game::subscribe opening [::scidb::game::current] $(update)
		::scidb::db::subscribe gameSwitch [list [namespace current]::GameSwitched $w]
		Update $w
	} else {
		HideBoard $w
		if {$(lastpos) >= 0} { ::scidb::game::unsubscribe opening $(lastpos) $(update) }
		::scidb::db::unsubscribe gameSwitch [list [namespace current]::GameSwitched $w]
	}
}


proc closed {w} {
	variable ${w}::
	if {$(active)} { activate $w false }
}


proc HandleSelection {dlg row} {
	if {$row >= 0} { set state normal } else { set state disabled }
	$dlg.ok configure -state $state
}


proc SelectEco {w row} {
	variable ${w}::
	variable eco_

	HideBoard $w
	set index [::scrolledtable::rowToIndex $w.table $row]
	set eco_ [lindex $(data) $index 1]
}


proc CurrentSelection {w} {
	variable ${w}::

	variable eco_
	set index [::scrolledtable::activeIndex $w.table]
	if {$index >= 0} { set eco_ [lindex $(data) $index 1] } else { set eco_ "" }
}


proc Update {w {force no}} {
	variable Options
	variable ${w}::

	HideBoard $w
	if {$(mode) eq $Options(move:mode)} { set (oldData) $(data) } else { set (oldData) {} }
	set (data) [::scidb::game::ecotable -notation $Options(move:notation) -mode $Options(move:mode)]
	if {$(active) || $force} {
		set variant [::scidb::game::query variant]
		set base [::scidb::game::query database]
		::scrolledtable::update $w.table $base $variant [llength $(data)]
	}
}


proc GameSwitched {w oldPos newPos} {
	variable ${w}::

	HideBoard $w
	set (lastpos) $oldPos
	if {$oldPos >= 0} { ::scidb::game::unsubscribe opening $oldPos $(update) }
	::scidb::game::subscribe opening $newPos $(update)
	Update $w
}


proc UpdateMoveStyleLabel {args} {
	set [namespace current]::MoveStyle "$::pgn::setup::mc::Setup(MoveStyle)..."
}


proc SelectFigurines {path} {
	variable Options

	set lang [::figurines::openDialog $path $Options(move:figurines)]
	if {$lang eq $Options(move:figurines)} { return }
	set Options(move:figurines) $lang
	::scrolledtable::refresh $path
}


proc FillTable {path args} {
	set w [winfo parent $path]
	variable ${w}::

	if {$(data) eq $(oldData)} { return }
	HideBoard $w
	lassign [lindex $args 0] table base variant start first last columns
	incr first $start
	incr last $start
	set lastRow [llength $(oldData)]

	for {set row $first} {$row < $last} {incr row} {
		set rowData [lindex $(data) $row]
		if {$row >= $lastRow || $rowData ne [lindex $(oldData) $row]} {
			lassign $rowData line eco key names
			set index [expr {$row - $start}]
			table::insert $table $index [list $line $eco $key [MakeOpening $names]]
		}
	}

	if {!$(listmode)} {
		::scrolledtable::see $path end
		::scrolledtable::activate $path end
	}
}


proc MakeOpening {names} {
	set opening ""
	set i 0
	foreach name $names {
		if {[incr i] == 2} { continue }
		if {[string length $name] == 0} { break }
		append opening [expr {$i > 1 ? ", " : ""}] [::mc::translateEco $name]
	}
	return $opening
}


proc TableVisit {w table data} {
	variable ${w}::

	lassign $data base variant mode id row
	if {$mode eq "leave"} {
		::tooltip::hide true
		set row none
	} elseif {$row >= 0 && $id eq "eco" && ![::scrolledtable::visible? $table opening]} {
		set index [::scrolledtable::rowToIndex $table $row]
		::tooltip::show $table [MakeOpening [lindex $(data) $index 3]] cursor
	}
	if {$(listmode)} {
		::scrolledtable::activate $table $row
	}
}


proc TableSelected {w table index} {
	variable ${w}::

	if {$(locked)} {
		return [::scrolledtable::select $table none]
	}
	HideBoard $w
	set row [::scrolledtable::indexToRow $table $index]
	::scrolledtable::select $table none
	::scrolledtable::select $table $row
	set fen [::scidb::game::codeToFen [lindex $(data) $index 2]]
	::scidb::game::go position $fen
}


proc TableShow {w table x y state} {
	variable ${w}::

	if {[::util::shiftIsHeldDown? $state]} {
		after cancel $(afterid)
		set (afterid) [after idle [list [namespace current]::TableShow_ $w $table $x $y]]
		after 350 [list set [namespace current]::${w}::(locked) 0]
		set (locked) 1
	} else {
		HideBoard $w
		if {!$(listmode)} {
			lassign [::scrolledtable::identify $table $x $y] row col elem id
			if {$row >= 0} { ::scrolledtable::activate $table $row }
			$w.ok configure -state [expr {$row >= 0 ? "normal" : "disabled"}]
		}
	}
}


proc TableShow_ {w table x y} {
	variable ${w}::

	if {![winfo exists $w]} { return }
   lassign [::scrolledtable::identify $table $x $y] row col elem id
	if {$row == -1} { return }
	set index [::scrolledtable::rowToIndex $table $row]
	set fen [::scidb::game::codeToFen [lindex $(data) $index 2]]
	ShowBoard $w $fen
}


proc ShowBoard {parent fen} {
	set w .application.showboard:ecotable

	if {![winfo exists $w]} {
		variable Defaults

		destroy [::util::makePopup $w]
		::board::diagram::new $w.board $Defaults(miniboard:size) -bordersize 2
		pack $w.board
	}

	UpdateBoard $parent $fen
	::tooltip::popup $parent $w
}


proc UpdateBoard {parent fen} {
	set w .application.showboard:ecotable

	if {![winfo exists $w]} { 
		return [ShowBoard $parent $fen]
	}
	::board::diagram::update $w.board [::scidb::board::fenToBoard $fen]
}


proc HideBoard {w} {
	variable ${w}::

	after cancel $(afterid)

	if {[winfo exists .application.showboard:ecotable]} {
		::tooltip::popdown .application.showboard:ecotable
	}
}


proc WriteTableOptions {chan variant {id "board"}} {
	variable TableOptions
	variable IdList

	if {$id ne "board"} { return }

	foreach uid $IdList {
		if {[info exists TableOptions($variant:$id)]} {
			puts $chan "::scrolledtable::setOptions eco:$uid {"
			::options::writeArray $chan $TableOptions($variant:$id)
			puts $chan "}"
		}
	}
}
::options::hookTableWriter [namespace current]::WriteTableOptions


proc WriteOptions {chan} {
	::options::writeItem $chan [namespace current]::Options
}
::options::hookWriter [namespace current]::WriteOptions


proc SaveOptions {twm variant} {
	variable TableOptions

	if {[set id [::application::twm::getId $twm]] eq "board"} {
		set TableOptions($variant:$id) [::scrolledtable::getOptions eco:$id]
	}
}


proc RestoreOptions {twm variant} {
	variable TableOptions

	if {[::application::twm::getId $twm] eq "board"} {
		::scrolledtable::setOptions eco:board $TableOptions($variant:board)
	}
}


proc CompareOptions {twm variant} {
	variable TableOptions

	if {[set id [::application::twm::getId $twm]] ne "board"} { return true }
	if {[::scrolledtable::countOptions eco:$id] == 0} { return true }
	set lhs $TableOptions($variant:$id)
	set rhs [::scrolledtable::getOptions eco:$id]
	return [::arrayListEqual $lhs $rhs]
}


::options::hookSaveOptions \
	[namespace current]::SaveOptions \
	[namespace current]::RestoreOptions \
	[namespace current]::CompareOptions \
	;

} ;# namespace eco
} ;# namespace application

# vi:set ts=3 sw=3:
