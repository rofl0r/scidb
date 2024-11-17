# ======================================================================
# Author : $Author$
# Version: $Revision: 1519 $
# Date   : $Date: 2018-09-11 11:41:52 +0000 (Tue, 11 Sep 2018) $
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
# Copyright: (C) 2009-2018 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

::util::source event-list

namespace eval application {
namespace eval database {
namespace eval event {

array set Defaults {
	sort:players	{}
	sort:events		{}
}

array set Prios { event 300 games 200 player 100 }

array set FrameOptions {
	event	 { -width 400 -height 100% -minwidth 200 -minheight 4u -expand both }
	games	 { -width 600 -height  50% -minwidth 200 -minheight 4u -expand both }
	player { -width 600 -height  50% -minwidth 200 -minheight 4u -expand both }
}

variable Layout {
	root { -shrink none -grow none } {
		panedwindow { -orient horz } {
			frame event %event%
			panedwindow { -orient vert } {
				frame games %games%
				frame player %player%
			}
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

	if {$twm ni $Tables} { lappend Tables $twm }
	set Vars(after:games) {}
	set Vars(after:events) {}
	set Vars(after:players) {}
	set Vars(active) 0
	set Vars(base) ""

	::application::twm::make $twm event \
		[namespace current]::Prios \
		[array get FrameOptions] \
		$Layout \
		-makepane [namespace current]::MakeFrame \
		-buildpane [namespace current]::BuildFrame \
		-adjustcmd [namespace current]::adjustFrame \
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
	set Vars($base:$variant:update:events) 1
	event::DoUpdate $path $base $variant

	if {[winfo toplevel $w] ne $w} {
		::toolbar::activate $Vars(frame:event) $flag
	}
}


proc overhang {parent} {
	set path $parent.twm
	variable ${path}::Vars

	return [::eventtable::overhang $Vars(frame:event)]
}


proc linespace {parent} {
	set path $parent.twm
	variable ${path}::Vars

	return [::eventtable::linespace $Vars(frame:event)]
}


proc adjustFrame {twm frame id dimensions} {
	lassign $dimensions width height minwidth minheight maxwidth maxheight
	set linespace [::${id}table::linespace $frame.$id]
	$twm set $frame vgrid $linespace
	set overhang [::${id}table::computeHeight $frame.$id]
	set minheight [expr {(max(0,$minheight - $overhang)/$linespace)*$linespace + $overhang}]
	if {$minheight == 0} { set minheight $overhang }
	return [list $width $height $minwidth $minheight $maxwidth $maxheight]
}


proc setActive {flag} {
	# no action
}


proc select {parent base variant index} {
	set path $parent.twm
	variable ${path}::Vars

	if {$Vars(active)} {
		Select $path $base $variant $index
	} else {
		set Vars($base:$variant:select) $index
	}
}


proc MakeFrame {twm parent type uid} {
	variable Prios

	set frame [tk::frame $parent.$uid -borderwidth 0 -takefocus 1]
	set nameVar ::application::twm::mc::Pane($uid)
	return [list $frame $nameVar $Prios($uid) no [expr {$uid ne "event"}] yes yes]
}


proc BuildFrame {twm frame uid width height} {
	variable ${twm}::Vars
	set Vars(frame:$uid) $frame
	set id [::application::twm::getId $twm]

	switch $uid {
		event {
			::eventtable::build $frame [namespace code [list View $twm]] {} \
				-selectcmd [list [namespace current]::event::Search $twm] \
				-usefind yes \
				-id db:events:$id:$uid \
				;
			::scidb::db::subscribe eventList \
				[list [namespace current]::player::Update $twm] \
				[list [namespace current]::Close $twm] \
				;
		}
		games {
			set columns {white whiteElo black blackElo result date round length}
			::gamestable::build $frame [namespace code [list View $twm]] $columns -id db:events:$id:$uid
			::scidb::db::subscribe gameList [list [namespace current]::games::Update $twm]
		}
		player {
			set columns {lastName firstName type sex rating1 federation title}
			::playertable::build $frame [namespace code [list View $twm]] $columns \
				-selectcmd [namespace code [list SelectPlayer $twm]] \
				-id db:events:$id:$uid \
				;
			::scidb::db::subscribe playerList [list [namespace current]::event::Update $twm]
		}
	}
}


proc Select {path base variant index} {
	variable ${path}::Vars

	set position [::scidb::db::get lookupEvent $index $Vars($base:$variant:view) $base $variant]
	::eventtable::see $Vars(frame:event) $position
	update idletasks
	set row [::eventtable::indexToRow $Vars(frame:event) $position]
	::eventtable::setSelection $Vars(frame:event) $row
}


proc Close {path base variant} {
	variable ${path}::Vars

	array unset Vars $base:$variant:*
	::eventtable::forget $Vars(frame:event) $base $variant
	::playertable::forget $Vars(frame:player) $base $variant
	::gamestable::forget $Vars(frame:games) $base $variant

	if {$Vars(base) eq "$base:$variant"} {
		::eventtable::clear $Vars(frame:event)
		::playertable::clear $Vars(frame:player)
		::gamestable::clear $Vars(frame:games)
	}
}


proc View {path base variant} {
	variable ${path}::Vars

	if {[string length $base] == 0} { return 0 }
	return $Vars($base:$variant:view)
}


proc InitBase {path base variant} {
	variable ${path}::Vars
	variable Defaults

	if {[info exists Vars($base:$variant:initializing)]} { return }

	if {![info exists Vars($base:$variant:view)]} {
		set Vars($base:$variant:initializing) 1
		set Vars($base:$variant:view) \
			[::scidb::view::new $base $variant slave slave master slave slave slave]
		set Vars($base:$variant:update:events) 1
		set Vars($base:$variant:sort:events) $Defaults(sort:events)
		set Vars($base:$variant:sort:players) $Defaults(sort:players)
		set Vars($base:$variant:lastChange) [::scidb::db::get lastChange $base $variant]
		set Vars($base:$variant:games:lastId) -1
		set Vars($base:$variant:events:lastId) -1
		set Vars($base:$variant:players:lastId) -1
		set Vars($base:$variant:select) -1
		set Vars($base:$variant:selected:key) {}

		::eventtable::init $Vars(frame:event) $base $variant
		::playertable::init $Vars(frame:player) $base $variant
		::gamestable::init $Vars(frame:games) $base $variant
		::scidb::view::search $base $variant $Vars($base:$variant:view) null player
	}
}


proc SelectPlayer {path base variant view} {
	variable ${path}::Vars

	set index [::playertable::selectedPlayer $Vars(frame:player) $base $variant]
	[namespace parent]::selectPlayer $base $variant $index
}


namespace eval games {

proc Update {path id base variant {view -1} {index -1}} {
	variable [namespace parent]::${path}::Vars

	[namespace parent]::InitBase $path $base $variant

	if {$view == $Vars($base:$variant:view)} {
		after cancel $Vars(after:games)
		set Vars(after:games) [after idle [namespace code [list Update2 $id $path $base $variant]]]
	}
}


proc Update2 {id path base variant} {
	variable [namespace parent]::${path}::Vars

	if {$id <= $Vars($base:$variant:games:lastId)} { return }
	set Vars($base:$variant:games:lastId) $id
	set lastChange $Vars($base:$variant:lastChange)
	set Vars($base:$variant:lastChange) [::scidb::db::get lastChange $base $variant]
	set selected [::eventtable::selectedEvent $Vars(frame:event) $base $variant]
	set view $Vars($base:$variant:view)

	if {$selected >= 0 && $lastChange < $Vars($base:$variant:lastChange)} {
		if {[llength $Vars($base:$variant:selected:key)]} {
			set index [::scidb::db::find event $base $variant $Vars($base:$variant:selected:key)]
			if {$index >= 0} {
				set selected [::scidb::db::get lookupEvent $index $view $base $variant]
				[namespace parent]::event::Search $path $base $variant $view $selected
			} else {
				[namespace parent]::event::Reset $path $base $variant
			}
		}
	} else {
		set Vars($base:$variant:lastChange) $lastChange

		set n [::scidb::view::count games $base $variant $Vars($base:$variant:view)]
		after idle [list ::gamestable::update $Vars(frame:games) $base $variant $n]

		set n [::scidb::view::count players $base $variant $Vars($base:$variant:view)]
		after idle [list ::playertable::update $Vars(frame:player) $base $variant $n]
		set Vars($base:$variant:players:lastId) $id
	}
}

} ;# namespace games


namespace eval event {

proc Reset {path base variant} {
	variable [namespace parent]::${path}::Vars

	::playertable::clear $Vars(frame:player)
	::gamestable::clear $Vars(frame:games)
	::eventtable::select $Vars(frame:event) none
	set Vars($base:$variant:selected:key) {}
}


proc Search {path base variant view {selected -1}} {
	variable [namespace parent]::${path}::Vars

	::widget::busyCursor on
	::gamestable::activate $Vars(frame:games) none
	::gamestable::select $Vars(frame:games) none
	::playertable::activate $Vars(frame:player) none
	::playertable::select $Vars(frame:player) none

	if {$selected == -1} {
		set selected [::eventtable::selectedEvent $Vars(frame:event) $base $variant]
		if {$selected >= 0} {
			set index [::scidb::db::get eventIndex $selected $view $base $variant]
			set Vars($base:$variant:selected:key) [scidb::db::get eventKey $base $variant event $index]
		}
	}

	if {$selected >= 0} {
		# TODO: we do an exact search, but probably we like to seach only for player name!
		::scidb::view::search $base $variant $view null player [list event $selected]
		::playertable::scroll $Vars(frame:player) home
		::gamestable::scroll $Vars(frame:games) home
	} else {
		Reset $path $base $variant
	}

	::widget::busyCursor off
}


proc Update {path id base variant {view -1} {index -1}} {
	variable ::scidb::clipbaseName
	variable [namespace parent]::${path}::Vars

	if {$base ne $clipbaseName && [string length [file extension $base]] == 0} { return }

	set Vars(base) "$base:$variant"
	after cancel $Vars(after:events)
	set Vars(after:events) [after idle [namespace code [list Update2 $id $path $base $variant]]]
}


proc Update2 {id path base variant} {
	variable [namespace parent]::${path}::Vars

	[namespace parent]::InitBase $path $base $variant
	if {$id <= $Vars($base:$variant:events:lastId)} { return }
	set Vars($base:$variant:events:lastId) $id
	set Vars($base:$variant:update:events) 1
	DoUpdate $path $base $variant
}


proc DoUpdate {path base variant} {
	variable [namespace parent]::${path}::Vars

	if {$Vars(active)} {
		if {[llength $Vars($base:$variant:sort:events)]} {
			::widget::busyCursor on
			set view $Vars($base:$variant:view)
			::scidb::db::sort event $base $variant $Vars($base:$variant:sort:events) $view
			::widget::busyCursor off
			set Vars($base:$variant:sort:events) {}
		}
		if {$Vars($base:$variant:update:events)} {
			set n [::scidb::db::count events $base $variant]
			after idle [list ::eventtable::update $Vars(frame:event) $base $variant $n]
			after idle [namespace code \
				[list [namespace parent]::games::Update2 \
					$Vars($base:$variant:events:lastId) $path $base $variant]]
			set Vars($base:$variant:update:events) 0
			if {$Vars($base:$variant:select) >= 0} {
				after idle \
					[list [namespace parent]::Select $path $base $variant $Vars($base:$variant:select)]
				set Vars($base:$variant:select) -1
			}
		}
	}
}

} ;# namespace event

namespace eval player {

proc Update {path id base variant {view -1} {index -1}} {
	variable [namespace parent]::${path}::Vars

	after cancel $Vars(after:players)
	set Vars(after:players) [after idle [namespace code [list Update2 $id $path $base $variant]]]
}


proc Update2 {id path base variant} {
	variable [namespace parent]::${path}::Vars

	[namespace parent]::InitBase $path $base $variant
	if {$id <= $Vars($base:$variant:players:lastId)} { return }
	set Vars($base:$variant:players:lastId) $id
	set Vars($base:$variant:update:players) 1
	DoUpdate $path $base $variant
}


proc DoUpdate {path base variant} {
	variable [namespace parent]::${path}::Vars

	if {$Vars(active)} {
		if {[llength $Vars($base:$variant:sort:players)]} {
			::widget::busyCursor on
			set view $Vars($base:$variant:view)
			::scidb::db::sort player $base $variant $Vars($base:$variant:sort:players) $view
			::widget::busyCursor off
			set Vars($base:$variant:sort:players) {}
		}
		if {$Vars($base:$variant:update:players)} {
			set n [::scidb::view::count players $base $variant $Vars($base:$variant:view)]
			after idle [list ::playertable::update $Vars(frame:player) $base $variant $n]
			set Vars($base:$variant:update:players) 0
		}
	}
}

} ;# namespace player


proc WriteTableOptions {chan variant {id "event"}} {
	variable TableOptions
	variable Tables

	if {$id ne "event"} { return }

	foreach table $Tables {
		set id [::application::twm::getId $table]
		foreach uid {event games player} {
			if {[info exists TableOptions($variant:$id:$uid)]} {
				puts $chan "::scrolledtable::setOptions db:events:$id:$uid {"
				::options::writeArray $chan $TableOptions($variant:$id:$uid)
				puts $chan "}"
			}
		}
	}
}
::options::hookTableWriter [namespace current]::WriteTableOptions


proc SaveOptions {twm variant} {
	variable TableOptions

	set id [::application::twm::getId $twm]
	foreach uid {event games player} {
		set TableOptions($variant:$id:$uid) [::scrolledtable::getOptions db:events:$id:$uid]
	}
}


proc RestoreOptions {twm variant} {
	variable TableOptions

	set id [::application::twm::getId $twm]
	foreach uid {event games player} {
		::scrolledtable::setOptions db:events:$id:$uid $TableOptions($variant:$id:$uid)
	}
}


proc CompareOptions {twm variant} {
	variable TableOptions

	set id [::application::twm::getId $twm]
	foreach uid {event games player} {
		if {[::scrolledtable::countOptions db:events:$id:$uid] > 0} {
			set lhs $TableOptions($variant:$id:$uid)
			set rhs [::scrolledtable::getOptions db:events:$id:$uid]
			if {![::table::equal $lhs $rhs]} { return false }
		}
	}
	return true
}


::options::hookSaveOptions \
	[namespace current]::SaveOptions \
	[namespace current]::RestoreOptions \
	[namespace current]::CompareOptions \
	;

} ;# namespace event
} ;# namespace database
} ;# namespace application

# vi:set ts=3 sw=3:
