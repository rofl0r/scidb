# ======================================================================
# Author : $Author: gcramer $
# Version: $Revision: 1498 $
# Date   : $Date: 2018-07-11 11:53:52 +0000 (Wed, 11 Jul 2018) $
# Url    : $URL: https://svn.code.sf.net/p/scidb/code/trunk/tcl/app-db-positions.tcl $
# ======================================================================

# ======================================================================
#    _/|            __
#   // o\         /    )           ,        /    /
#   || ._)    ----\---------__----------__-/----/__-
#   //__\          \      /   '  /    /   /    /   )
#   )___(     _(____/____(___ __/____(___/____(___/_
# ======================================================================

# ======================================================================
# Copyright: (C) 2014-2018 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

::util::source start-position-list

namespace eval application {
namespace eval database {
namespace eval positions {
namespace eval mc {

set NoCastle		"No castle"

set F_Position		"Position"
set F_Description	"Description"
set F_BackRank		"Back Rank"
set F_Frequency	"Frequency"

} ;# namespace mc

#		ID   		Adjustment	Min	Max	Width	Stretch	Removable	Elipsis	Color
#	-------------------------------------------------------------------------------------
set Columns {
	{ position		right		 0		 0		 5			0			0			0			{}			}
	{ description	left		10		 0		12			1			1			1			darkblue	}
	{ backRank		left		 0		 0		13			0			1			0			{}			}
	{ frequency		right		 4		12		 9			0			0			1			{}			}
}

variable columns {}
foreach col $Columns { lappend columns [lindex $col 0] }
unset col

array set Prios { position 200 games 100 }

array set FrameOptions {
	position { -width 300 -height 640 -minwidth 100 -minheight 100 -expand both }
	games    { -width 700 -height 640 -minwidth 200 -minheight 100 -expand both }
}

variable Layout {
	root { -shrink none -grow none } {
		panedwindow { -orient horz } {
			frame position %position%
			frame games %games%
		}
	}
}

variable Tables {}


proc build {parent} {
	variable Tables
	variable Layout
	variable FrameOptions

	set twm $parent.twm
	namespace eval [namespace current]::$twm {}
	variable ${twm}::Vars

	set Vars(active) 0
	set Vars(base) ""
	if {$twm ni $Tables} { lappend Tables $twm }

	::application::twm::make $twm position \
		[namespace current]::MakeFrame \
		[namespace current]::BuildFrame \
		[namespace current]::Prios \
		[array get FrameOptions] \
		$Layout \
		;
	::application::twm::load $twm
	return $twm
}


proc activate {w flag} {
	set path $w.twm
	variable ${path}::Vars

	set Vars(active) $flag
	set base [::scidb::db::get name]
	set variant [::scidb::app::variant]
	set Vars($base:$variant:update:positions) 1
	positions::UpdateTable $path $base $variant

	if {[winfo toplevel $w] ne $w} {
		::toolbar::activate $Vars(frame:position) $flag
	}
}


proc overhang {parent} {
	set path $parent.twm
	variable ${path}::Vars

	return [::scrolledtable::overhang $Vars(frame:position)]
}


# XXX linespace not needed anymore?
proc linespace {parent} {
	set path $parent.twm
	variable ${path}::Vars

	return [::scrolledtable::linespace $Vars(frame:position)]
}


proc setActive {flag} {
	# no action
}


proc MakeFrame {twmn parent type uid} {
	variable Prios

	set frame [tk::frame $parent.$uid -borderwidth 0 -takefocus 1]
	set nameVar ::application::twm::mc::Pane($uid)
	return [list $frame $nameVar $Prios($uid) [expr {$uid ne "position"}] yes yes]
}


proc BuildFrame {twm frame uid width height} {
	variable ${twm}::Vars
	set Vars(frame:$uid) $frame
	set id [::application::twm::getId $twm]

	switch $uid {
		position {
			variable Columns

			set columns {}
			foreach column $Columns {
				lassign $column cid adjustment minwidth maxwidth width stretch removable ellipsis color
				set menu {}

				if {$cid in {backRank frequency}} {
					lappend menu [list command \
						-command [namespace code [list positions::SortColumn $twm $cid ascending]] \
						-labelvar ::gametable::mc::SortAscending \
					]
					lappend menu [list command \
						-command [namespace code [list positions::SortColumn $twm $cid descending]] \
						-labelvar ::gametable::mc::SortDescending \
					]
					lappend menu [list command \
						-command [namespace code [list positions::SortColumn $twm $cid reverse]] \
						-labelvar ::gametable::mc::ReverseOrder \
					]
				}
				lappend menu [list command \
					-command [namespace code [list positions::SortColumn $twm $cid cancel]] \
					-labelvar ::gametable::mc::CancelSort \
				]

				set opts {}
				lappend opts -justify $adjustment
				lappend opts -minwidth $minwidth
				lappend opts -maxwidth $maxwidth
				lappend opts -width $width
				lappend opts -stretch $stretch
				lappend opts -removable $removable
				lappend opts -ellipsis $ellipsis
				lappend opts -foreground $color
				lappend opts -textvar [namespace current]::mc::F_[string toupper $cid 0 0]
				lappend opts -menu $menu

				lappend columns $cid $opts
			}

			set table [::scrolledtable::build $frame $columns -id db:positions:$id:$uid]
			::scrolledtable::configure $frame backRank \
				-specialfont [list [list $::font::figurine(text:normal) 9812 9823]] \
				;
			::scrolledtable::bind $frame <ButtonPress-2> [namespace code [list ShowBoard $twm %x %y]]
			::scrolledtable::bind $frame <ButtonRelease-2> [namespace code [list HideBoard $twm]]

			bind $frame <<TableFill>>			[namespace code [list positions::TableFill $twm %d]]
			bind $frame <<TableSelected>>		[namespace code [list positions::TableSelected $twm %d]]
			bind $frame <<LanguageChanged>>	[list ::scrolledtable::refresh $Vars(frame:position)]

			::scidb::db::subscribe positionList [namespace current]::positions::Update $twm
			::scidb::db::subscribe dbInfo [namespace current]::NoOp [namespace current]::Close $twm
		}
		games {
			set columns {white whiteElo black blackElo result event date length}
			::gametable::build $frame [namespace code [list View $twm]] $columns -id db:positions:$id:$uid
			::scidb::db::subscribe gameList [namespace current]::games::Update {} $twm
		}
	}
}


proc ShowBoard {path x y} {
	variable ${path}::Vars

	set table $Vars(frame:position)
	set index [::scrolledtable::at $table $y]
	set base [::scrolledtable::base $table]
	set variant [::scrolledtable::variant $table]
	set view $Vars($base:$variant:view)
	set idn [lindex [scidb::db::get position $index $view] 0]

	activate [winfo parent $path] [::scrolledtable::indexToRow $table $index]
	::scrolledtable::focus $table
	if {$idn == 0} { return }

	set fen [lindex [::scidb::board::idnToFen $idn] 0]
	set board [::scidb::board::fenToBoard $fen]
	::browser::showPosition $path $board
}


proc HideBoard {path} {
	::browser::hidePosition $path
}


proc View {path base variant} {
	variable ${path}::Vars

	if {[string length $base] == 0} { return 0 }
	return $Vars($base:$variant:view)
}


proc TableMinSize {pane minsize} {
	[winfo parent $pane] paneconfigure $pane -minsize [lindex $minsize 0 0]
}


proc InitBase {path base variant} {
	variable ${path}::Vars

	if {[info exists Vars($base:$variant:initializing)]} { return }

	if {![info exists Vars($base:$variant:view)]} {
		set Vars($base:$variant:initializing) 1
		set Vars($base:$variant:view) \
			[::scidb::view::new $base $variant unused unused unused unused master slave]
		set Vars($base:$variant:update) 1
		set Vars($base:$variant:position) ""
		set Vars($base:$variant:after:games) {}
		set Vars($base:$variant:after:positions) {}
		set Vars($base:$variant:lastChange) [::scidb::db::get lastChange $base $variant]
		set Vars($base:$variant:positions:lastId) -1
		set Vars($base:$variant:games:lastId) -1
		::scidb::view::search $base $variant $Vars($base:$variant:view) null none
	}
}


proc NoOp {args} {}


proc Close {path base variant} {
	variable ${path}::Vars

	#if {$action ne "close"} { return }

	array unset Vars $base:$variant:*
	::scrolledtable::forget $Vars(frame:position) $base $variant
	::gametable::forget $Vars(frame:games) $base $variant
	if {$Vars(base) eq "$base:$variant"} { ::scrolledtable::clear $Vars(frame:position) }
	if {$Vars(base) eq "$base:$variant"} { ::gametable::clear $Vars(frame:games) }
}


namespace eval games {

proc Update {path id base variant {view -1} {index -1}} {
	variable ::scidb::clipbaseName
	variable [namespace parent]::${path}::Vars

	if {$base ne $clipbaseName && [string length [file extension $base]] == 0} { return }
	[namespace parent]::InitBase $path $base $variant

	if {$view == $Vars($base:$variant:view)} {
		after cancel $Vars($base:$variant:after:games)
		set Vars($base:$variant:after:games) \
			[after idle [namespace code [list Update2 $id $path $base $variant]]]
	}
}


proc Update2 {id path base variant} {
	variable [namespace parent]::${path}::Vars

	if {$id <= $Vars($base:$variant:games:lastId)} { return }

	set Vars($base:$variant:games:lastId) $id
	set lastChange $Vars($base:$variant:lastChange)
	set Vars($base:$variant:lastChange) [::scidb::db::get lastChange $base $variant]
	set view $Vars($base:$variant:view)

	if {	[string is integer -strict $Vars($base:$variant:position)]
		&& $lastChange < $Vars($base:$variant:lastChange)} {
		set position [::scidb::db::find position $base $variant $Vars($base:$variant:position)]
		if {$position >= 0} {
			[namespace parent]::positions::TableSearch $path $base $variant $view
		} else {
			[namespace parent]::positions::Reset $path $base $variant
		}
	} else {
		set n [::scidb::view::count games $base $variant $view]
		after idle [list ::gametable::update $Vars(frame:games) $base $variant $n]
	}
}


proc TableOptions {path} {
	# TODO
}

} ;# namespace games

namespace eval positions {

proc Reset {path base variant} {
	variable [namespace parent]::${path}::Vars

	::gametable::clear $Vars(frame:games)
	::scrolledtable::select $Vars(frame:position) none
	::scrolledtable::activate none
	set Vars($base:$variant:position) ""
}


proc UpdateTable {path base variant} {
	variable [namespace parent]::${path}::Vars

	if {$Vars(active)} {
		if {$Vars($base:$variant:update)} {
			set n [::scidb::db::count positions $base $variant]
			after idle [list ::scrolledtable::update $Vars(frame:position) $base $variant $n]
			after idle [list [namespace parent]::games::Update2 \
				$Vars($base:$variant:positions:lastId) $path $base $variant]
			set Vars($base:$variant:update) 0
		}
	}
}


proc Update {path id base variant {view -1} {index -1}} {
	variable ::scidb::clipbaseName
	variable [namespace parent]::${path}::Vars

	if {$base ne $clipbaseName && [string length [file extension $base]] == 0} { return }

	set Vars(base) "$base:$variant"
	[namespace parent]::InitBase $path $base $variant
	after cancel $Vars($base:$variant:after:positions)
	set Vars($base:$variant:after:positions) \
		[after idle [namespace code [list Update2 $id $path $base $variant]]]
}


proc Update2 {id path base variant} {
	variable [namespace parent]::${path}::Vars

	if {$id <= $Vars($base:$variant:positions:lastId)} { return }
	set Vars($base:$variant:positions:lastId) $id
	set Vars($base:$variant:update) 1
	UpdateTable $path $base $variant
}


proc TableOptions {path} {
	# TODO
}


proc TableFill {path args} {
	variable [namespace parent]::${path}::Vars

	lassign [lindex $args 0] table base variant start first last columns
	if {![info exists Vars($base:$variant:view)]} { return }
	set view $Vars($base:$variant:view)
	set last [expr {min($last, [scidb::view::count positions $base $variant $view] - $start)}]

	for {set i $first} {$i < $last} {incr i} {
		set index [expr {$start + $i}]
		set line [scidb::db::get position $index $view]
		set text {}

		foreach id {position backRank frequency} value $line {
			switch $id {
				position {
					if {$value == 0} {
						set pos "FEN"
						set value ""
					} else {
						set pos $value
						if {$pos == 518} {
							set value $::setup::board::mc::StandardPosition
						} elseif {960 < $pos} {
							set pos [expr {$value - 960}]
							set value [set [namespace parent]::mc::NoCastle]
						} else {
							set value $::setup::board::mc::Chess960Castling
						}
					}
					lappend text $pos
				}

				frequency {
					set value [::locale::formatNumber $value]
				}
			}

			lappend text $value
		}

		::table::insert $table $i $text
	}
}


proc TableSelected {path index} {
	variable [namespace parent]::${path}::Vars

	::widget::busyCursor on
	set table $Vars(frame:position)
	set base [::scrolledtable::base $table]
	set variant [::scrolledtable::variant $table]
	set view $Vars($base:$variant:view)
	set position [scidb::db::get position $index $view]
	set Vars($base:$variant:position) [lindex $position 0]
	TableSearch $path $base $variant $view
	::widget::busyCursor off
}


proc TableSearch {path base variant view} {
	variable [namespace parent]::${path}::Vars

	::widget::busyCursor on
	::gametable::activate $Vars(frame:games) none
	::gametable::select $Vars(frame:games) none
	::gametable::scroll $Vars(frame:games) home
	::scidb::view::search $base $variant $view null none [list position $Vars($base:$variant:position)]
	::widget::busyCursor off
}


proc SortColumn {path id dir} {
	variable [namespace parent]::${path}::Vars

	::widget::busyCursor on
	set table $Vars(frame:position)
	set base [::scrolledtable::base $table]
	set variant [::scrolledtable::variant $table]
	set view $Vars($base:$variant:view)
	set see 0
	set selection [::scrolledtable::selection $table]
	if {$selection >= 0 && [::scrolledtable::selectionIsVisible? $table]} { set see 1 }
	switch $dir {
		reverse {
			::scidb::db::reverse position $base $variant $view
		}
		cancel {
			set columnNo [::scrolledtable::columnNo $table position]
			::scidb::db::sort position $base $variant $columnNo $view -ascending -reset
		}
		ascending - descending {
			set columnNo [::scrolledtable::columnNo $table $id]
			::scidb::db::sort position $base $variant $columnNo $view -$dir
		}
	}
	if {$selection >= 0} {
		set selection [::scidb::db::get lookupPosition $selection $view $base $variant]
	}
	::widget::busyCursor off
	::scrolledtable::updateColumn $table $selection $see
}

} ;# namespace positions


proc WriteTableOptions {chan {id "position"}} {
	variable Tables

	if {$id ne "position"} { return }

	foreach table $Tables {
		set id [::application::twm::getId $table]
		foreach uid {position games} {
			if {[::scrolledtable::countOptions db:positions:$id:$uid] > 0} {
				puts $chan "::scrolledtable::setOptions db:positions:$id:$uid {"
				::options::writeArray $chan [::scrolledtable::getOptions db:positions:$id:$uid]
				puts $chan "}"
			}
		}
	}
}
::options::hookTableWriter [namespace current]::WriteTableOptions


proc SaveOptions {twm variant} {
	variable TableOptions

	set id [::application::twm::getId $twm]
	foreach uid {position games} {
		set TableOptions($variant:$id:$uid) [::scrolledtable::getOptions db:positions:$id:$uid]
	}
}


proc RestoreOptions {twm variant} {
	variable TableOptions

	set id [::application::twm::getId $twm]
	foreach uid {position games} {
		::scrolledtable::setOptions db:positions:$id:$uid $TableOptions($variant:$id:$uid)
	}
}


proc CompareOptions {twm variant} {
	variable TableOptions

	set id [::application::twm::getId $twm]
	foreach uid {position games} {
		if {[::scrolledtable::countOptions db:positions:$id:$uid] > 0} {
			set lhs $TableOptions($variant:$id:$uid)
			set rhs [::scrolledtable::getOptions db:positions:$id:$uid]
			if {![::arrayListEqual $lhs $rhs]} { return false }
		}
	}
	return true
}


::options::hookSaveOptions \
	[namespace current]::SaveOptions \
	[namespace current]::RestoreOptions \
	[namespace current]::CompareOptions \
	;

} ;# namespace positions
} ;# namespace database
} ;# namespace application

# vi:set ts=3 sw=3:
