# ======================================================================
# Author : $Author$
# Version: $Revision: 416 $
# Date   : $Date: 2012-09-02 20:54:30 +0000 (Sun, 02 Sep 2012) $
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
# Copyright: (C) 2009-2012 Gregor Cramer
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
namespace eval events {

array set Defaults {
	sort:players	{}
	sort:events		{}
}

variable Tables {}


proc build {parent} {
	variable Tables

	set top [tk::panedwindow $parent.top \
		-orient horizontal \
		-opaqueresize true \
		-borderwidth 0]
	pack $top -fill both -expand yes
	lappend Tables $top

	set lt ${top}.events
	set rt ${top}.info

	set gl ${rt}.games
	set pl ${rt}.players

	namespace eval [namespace current]::$top {}
	variable ${top}::Vars

	set Vars(after:games) {}
	set Vars(after:events) {}
	set Vars(after:players) {}
	set Vars(active) 0

	::eventtable::build $lt [namespace code [list View $top]] {} \
		-selectcmd [list [namespace current]::events::Search $top] \
		-usefind 1 \
		;

	tk::panedwindow $rt -orient vertical -opaqueresize true -borderwidth 0
	set columns {white whiteElo black blackElo result date round length}
	::gametable::build $gl [namespace code [list View $top]] $columns
	set columns {lastName firstName type sex rating1 federation title}
	::playertable::build $pl [namespace code [list View $top]] $columns \
		-selectcmd [namespace code [list SelectPlayer $top]] \
		;

	::scidb::db::subscribe eventList \
		[namespace current]::players::Update \
		[namespace current]::Close \
		$top \
		;
	::scidb::db::subscribe gameList \
		[namespace current]::games::Update \
		$top \
		;
	::scidb::db::subscribe playerList \
		[namespace current]::events::Update \
		$top \
		;

	bind $rt <<TableMinSize>>	[namespace code [list TableMinSize $rt %d]]
	bind $lt <<TableMinSize>>	[namespace code [list TableMinSize $lt %d]]

	$top add $lt
	$top add $rt

	$rt add $gl
	$rt add $pl

	$top paneconfigure $lt -sticky nsew -stretch middle -minsize 580	;# XXX
	$top paneconfigure $rt -sticky nsew -stretch always

	$rt paneconfigure $gl -sticky nsew -stretch always
	$rt paneconfigure $pl -sticky nsew -stretch always

	return $top
}


proc activate {w flag} {
	set path $w.top
	variable ${path}::Vars

	set Vars(active) $flag
	set base [::scidb::db::get name]
	set Vars($base:update:events) 1
	events::DoUpdate $path $base

	if {[winfo toplevel $w] ne $w} {
		::toolbar::activate $path.events $flag
	}
}


proc overhang {parent} {
	return [::eventtable::overhang $parent.top.events]
}


proc linespace {parent} {
	return [::eventtable::linespace $parent.top.events]
}


proc select {parent base index} {
	set path $parent.top
	variable ${path}::Vars

	if {$Vars(active)} {
		Select $path $base $index
	} else {
		set Vars($base:select) $index
	}
}


proc Select {path base index} {
	variable ${path}::Vars

	set position [::scidb::db::get lookupEvent $index $Vars($base:view) $base]
	::eventtable::see $path.events $position
	update idletasks
	set row [::eventtable::indexToRow $path.events $position]
	::eventtable::setSelection $path.events $row
}


proc Close {path base} {
	variable ${path}::Vars

	array unset Vars $base:*
	::eventtable::clear $path.events
	::eventtable::forget $path.events $base
	::playertable::clear $path.info.players
	::playertable::forget $path.info.players $base
	::gametable::clear $path.info.games
	::gametable::forget $path.info.games $base
}


proc View {path base} {
	variable ${path}::Vars
	return $Vars($base:view)
}


proc InitBase {path base} {
	variable ${path}::Vars
	variable Defaults

	if {[info exists Vars($base:initializing)]} { return }

	if {![info exists Vars($base:view)]} {
		set Vars($base:initializing) 1
		set Vars($base:view) [::scidb::view::new $base slave slave master slave slave]
		set Vars($base:update:events) 1
		set Vars($base:sort:events) $Defaults(sort:events)
		set Vars($base:sort:players) $Defaults(sort:players)
		set Vars($base:lastChange) [::scidb::db::get lastChange $base]
		set Vars($base:games:lastId) -1
		set Vars($base:events:lastId) -1
		set Vars($base:players:lastId) -1
		set Vars($base:select) -1
		set Vars($base:selected:key) {}
		::eventtable::init $path.events $base
		::playertable::init $path.info.players $base
		::gametable::init $path.info.games $base
		::scidb::view::search $base $Vars($base:view) null player
	}
}


proc TableMinSize {pane minsize} {
	[winfo parent $pane] paneconfigure $pane -minsize [lindex $minsize 0 0]
}


proc SelectPlayer {path base view} {
	[namespace parent]::selectPlayer $base [::playertable::selectedPlayer $path.info.players $base]
}


namespace eval games {

proc Update {path id base {view -1} {index -1}} {
	variable [namespace parent]::${path}::Vars

	[namespace parent]::InitBase $path $base

	if {$view == $Vars($base:view)} {
		after cancel $Vars(after:games)
		set Vars(after:games) [after idle [namespace code [list Update2 $id $path $base]]]
	}
}


proc Update2 {id path base} {
	variable [namespace parent]::${path}::Vars

	if {$id <= $Vars($base:games:lastId)} { return }
	set Vars($base:games:lastId) $id
	set lastChange $Vars($base:lastChange)
	set Vars($base:lastChange) [::scidb::db::get lastChange $base]
	set selected [::eventtable::selectedEvent $path.events $base]
	set view $Vars($base:view)

	if {$selected >= 0 && $lastChange < $Vars($base:lastChange)} {
		if {[llength $Vars($base:selected:key)]} {
			set index [::scidb::db::find event $base $Vars($base:selected:key)]
			if {$index >= 0} {
				set selected [::scidb::db::get lookupEvent $index $view $base]
				[namespace parent]::events::Search $path $base $view $selected
			} else {
				[namespace parent]::events::Reset $path $base
			}
		}
	} else {
		set Vars($base:lastChange) $lastChange

		set n [::scidb::view::count games $base $Vars($base:view)]
		after idle [list ::gametable::update $path.info.games $base $n]

		set n [::scidb::view::count players $base $Vars($base:view)]
		after idle [list ::playertable::update $path.info.players $base $n]
		set Vars($base:players:lastId) $id
	}
}

} ;# namespace games


namespace eval events {

proc Reset {path base} {
	variable [namespace parent]::${path}::Vars

	::playertable::clear $path.info.players
	::gametable::clear $path.info.games
	::eventtable::select $path.events none
	set Vars($base:selected:key) {}
}


proc Search {path base view {selected -1}} {
	variable [namespace parent]::${path}::Vars

	::widget::busyCursor on
	::gametable::activate $path.info.games none
	::gametable::select $path.info.games none
	::playertable::activate $path.info.players none
	::playertable::select $path.info.players none

	if {$selected == -1} {
		set selected [::eventtable::selectedEvent $path.events $base]
		if {$selected >= 0} {
			set index [::scidb::db::get eventIndex $selected $view $base]
			set Vars($base:selected:key) [scidb::db::get eventKey $base event $index]
		}
	}

	if {$selected >= 0} {
		# TODO: we do an exact search, but probably we like to seach only for player name!
		::scidb::view::search $base $view null player [list event $selected]
		::playertable::scroll $path.info.players home
		::gametable::scroll $path.info.games home
	} else {
		Reset $path $base
	}

	::widget::busyCursor off
}


proc Update {path id base {view -1} {index -1}} {
	variable [namespace parent]::${path}::Vars

	after cancel $Vars(after:events)
	set Vars(after:events) [after idle [namespace code [list Update2 $id $path $base]]]
}


proc Update2 {id path base} {
	variable [namespace parent]::${path}::Vars

	[namespace parent]::InitBase $path $base
	if {$id <= $Vars($base:events:lastId)} { return }
	set Vars($base:events:lastId) $id
	set Vars($base:update:events) 1
	DoUpdate $path $base
}


proc DoUpdate {path base} {
	variable [namespace parent]::${path}::Vars

	if {$Vars(active)} {
		if {[llength $Vars($base:sort:events)]} {
			::widget::busyCursor on
			::scidb::db::sort event $base $Vars($base:sort:events) $Vars($base:view)
			::widget::busyCursor off
			set Vars($base:sort:events) {}
		}
		if {$Vars($base:update:events)} {
			set n [::scidb::db::count events $base]
			after idle [list ::eventtable::update $path.events $base $n]
			after idle [namespace code \
				[list [namespace parent]::games::Update2 $Vars($base:events:lastId) $path $base]]
			set Vars($base:update:events) 0
			if {$Vars($base:select) >= 0} {
				after idle [list [namespace parent]::Select $path $base $Vars($base:select)]
				set Vars($base:select) -1
			}
		}
	}
}

} ;# namespace events

namespace eval players {

proc Update {path id base {view -1} {index -1}} {
	variable [namespace parent]::${path}::Vars

	after cancel $Vars(after:players)
	set Vars(after:players) [after idle [namespace code [list Update2 $id $path $base]]]
}


proc Update2 {id path base} {
	variable [namespace parent]::${path}::Vars

	[namespace parent]::InitBase $path $base
	if {$id <= $Vars($base:players:lastId)} { return }
	set Vars($base:players:lastId) $id
	set Vars($base:update:players) 1
	DoUpdate $path $base
}


proc DoUpdate {path base} {
	variable [namespace parent]::${path}::Vars

	if {$Vars(active)} {
		if {[llength $Vars($base:sort:players)]} {
			::widget::busyCursor on
			::scidb::db::sort player $base $Vars($base:sort:players) $Vars($base:view)
			::widget::busyCursor off
			set Vars($base:sort:players) {}
		}
		if {$Vars($base:update:players)} {
			set n [::scidb::view::count players $base $Vars($base:view)]
			after idle [list ::playertable::update $path.info.players $base $n]
			set Vars($base:update:players) 0
		}
	}
}

} ;# namespace players


proc WriteOptions {chan} {
	variable Tables

	foreach table $Tables {
		puts $chan "::eventtable::setOptions $table.events {"
		::options::writeArray $chan [::eventtable::getOptions $table.events]
		puts $chan "}"
		puts $chan "::gametable::setOptions $table.info.games {"
		::options::writeArray $chan [::gametable::getOptions $table.info.games]
		puts $chan "}"
		puts $chan "::playertable::setOptions $table.info.players {"
		::options::writeArray $chan [::playertable::getOptions $table.info.players]
		puts $chan "}"
	}
}

::options::hookWriter [namespace current]::WriteOptions

} ;# namespace events
} ;# namespace database
} ;# namespace application

# vi:set ts=3 sw=3:
