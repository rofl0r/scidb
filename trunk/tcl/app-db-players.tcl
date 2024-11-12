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
# Copyright: (C) 2009-2011 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

namespace eval application {
namespace eval database {
namespace eval players {
namespace eval mc {

set EditPlayer	"Edit Player"
set Score		"Score"

} ;# namespace mc


array set Defaults {
	sort:players	{}
	sort:events		{}
}

variable Tables {}


proc build {parent} {
	variable Tables
#	variable ::icon::12x12::I_Federation

	set top [panedwindow $parent.top \
		-orient horizontal \
		-opaqueresize true \
		-borderwidth 0]
	pack $top -fill both -expand yes
	lappend Tables $top

	set lt ${top}.players

	set rt [panedwindow $top.info \
		-orient vertical \
		-opaqueresize true \
		-borderwidth 0]

	set gl ${rt}.games
	set ev ${rt}.events

	namespace eval [namespace current]::$top {}
	variable ${top}::Vars

	set Vars(after:games) {}
	set Vars(after:events) {}
	set Vars(after:players) {}
	set Vars(active) 0

	set columns {lastName firstName type sex rating1 federation title frequency}
	::playertable::build $lt [namespace code [list View $top]] $columns \
		-selectcmd [list [namespace current]::players::Search $top] \
		-usefind 1 \
		;
	set columns {white whiteElo black blackElo event result date length}
	::gametable::build $gl [namespace code [list View $top]] $columns
	set columns {event eventType eventDate eventMode timeMode eventCountry site}
	::eventtable::build $ev [namespace code [list View $top]] $columns \
		-selectcmd [namespace code [list SelectEvent $top]] \
		;

	::scidb::db::subscribe playerList \
		[namespace current]::players::Update \
		[namespace current]::Close \
		$top \
		;
	::scidb::db::subscribe gameList \
		[namespace current]::games::Update \
		$top \
		;
	::scidb::db::subscribe eventList \
		[namespace current]::events::Update \
		$top \
		;

	bind $rt <<TableMinSize>>	[namespace code [list TableMinSize $rt %d]]
	bind $lt <<TableMinSize>>	[namespace code [list TableMinSize $lt %d]]

	$top add $lt
	$top add $rt

	$rt add $gl
	$rt add $ev

	$top paneconfigure $lt -sticky nsew -stretch middle -minsize 420	;# XXX
	$top paneconfigure $rt -sticky nsew -stretch always

	$rt paneconfigure $gl -sticky nsew -stretch always
	$rt paneconfigure $ev -sticky nsew -stretch always

	return $top
}


proc activate {w menu flag} {
	set path $w.top
	variable ${path}::Vars

	set Vars(active) $flag
	players::Update2 $path [::scidb::db::get name]
	::toolbar::activate $path.players $flag
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

	set position [::scidb::db::get lookupPlayer $index $Vars($base:view) $base]
	::playertable::see $path.players $position
	update idle
	set row [::playertable::indexToRow $path.players $position]
	::playertable::setSelection $path.players $row
}


proc View {path base} {
	variable ${path}::Vars
	return $Vars($base:view)
}


proc Close {path base} {
	variable ${path}::Vars

	array unset Vars $base:*
	::playertable::clear $path.players
	::playertable::forget $path.players $base
	::eventtable::clear $path.info.events
	::eventtable::forget $path.info.events $base
	::gametable::clear $path.info.games
	::gametable::forget $path.info.games $base
}


proc InitBase {path base} {
	variable ${path}::Vars
	variable Defaults

	if {![info exists Vars($base:view)]} {
		set Vars($base:view) [::scidb::view::new $base]
		set Vars($base:update:players) 1
		set Vars($base:sort:players) $Defaults(sort:players)
		set Vars($base:sort:events) $Defaults(sort:events)
		set Vars($base:lastChange) [::scidb::db::get lastChange $base]
		set Vars($base:select) -1
		::playertable::init $path.players $base
		::gametable::init $path.info.games $base
		::eventtable::init $path.info.events $base
		::scidb::view::search $base $Vars($base:view) null events
	}
}


proc TableMinSize {pane minsize} {
	[winfo parent $pane] paneconfigure $pane -minsize [lindex $minsize 0 0]
}


proc SelectEvent {path base view} {
	[namespace parent]::selectEvent $base [::eventtable::selectedEvent $path.info.events $base]
}


namespace eval games {

proc Update {path base {view -1} {index -1}} {
	variable [namespace parent]::${path}::Vars

	[namespace parent]::InitBase $path $base

	if {$view == $Vars($base:view)} {
		after cancel $Vars(after:games)
		set Vars(after:games) [after idle [namespace code [list Update2 $path $base]]]
	}
}


proc Update2 {path base} {
	variable [namespace parent]::${path}::Vars

	set lastChange $Vars($base:lastChange)
	set Vars($base:lastChange) [::scidb::db::get lastChange $base]
	set selected [::playertable::selectedPlayer $path.players $base]

	if {$selected >= 0 && $lastChange < $Vars($base:lastChange)} {
		[namespace parent]::players::Search $path $base $Vars($base:view)
	} else {
		set Vars($base:lastChange) $lastChange

		set n [::scidb::view::count games $base $Vars($base:view)]
		after idle [list ::gametable::update $path.info.games $base $n]

		set n [::scidb::view::count events $base $Vars($base:view)]
		after idle [list ::eventtable::update $path.info.events $base $n]
	}
}

} ;# namespace games


namespace eval players {

proc Search {path base view} {
	variable [namespace parent]::${path}::Vars

	::widget::busyCursor on
	::gametable::activate $path.info.games none
	::gametable::select $path.info.games none
	::eventtable::activate $path.info.events none
	::eventtable::select $path.info.events none
	set selected [::playertable::selectedPlayer $path.players $base]
	# TODO: we do an exact search, but probably we like to seach only for player name!
	::scidb::view::search $base $view null events [list player $selected]
	::widget::busyCursor off
}


proc Update {path base {view -1} {index -1}} {
	variable [namespace parent]::${path}::Vars

	after cancel $Vars(after:players)
	set Vars(after:players) [after idle [namespace code [list Update2 $path $base]]]
}


proc Update2 {path base} {
	variable [namespace parent]::${path}::Vars

	[namespace parent]::InitBase $path $base
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
			set n [::scidb::db::count players $base]
			after idle [list ::playertable::update $path.players $base $n]
			after idle [namespace code [list [namespace parent]::games::Update2 $path $base]]
			set Vars($base:update:players) 0
			if {$Vars($base:select) >= 0} {
				after idle [list [namespace parent]::Select $path $base $Vars($base:select)]
				set Vars($base:select) -1
			}
		}
	} else {
		set Vars($base:update:players) 1
	}
}


proc RenamePlayer {table index} {
	set path [winfo parent $table]
	variable ${path}::Vars

	set parent [winfo toplevel $table]
	set dlg $parent.rename
	toplevel $dlg -class Scidb
	wm withdraw $dlg
	set top [::ttk::frame $dlg.top]
	pack $dlg.top

	set info [scidb::db::get playerInfo $index $Vars([::scidb::db::get name]:view)]
	lassign $info iName unused iElo iRating unused iFederation iTitles
	set iFirstName ""
	lassign [split $iName ","] iLastName iFirstName
	set iLastName [string trim $iLastName]
	set iFirstName [string trim $iFirstName]

	### Last name, First name ##############################
	set [namespace current]::Name_ disabled
	set [namespace current]::FirstName_ $iFirstName
	set [namespace current]::LastName_ $iLastName
	::ttk::checkbutton $top.namecb \
		-text $mc::Name \
		-onvalue normal \
		-offvalue disabled \
		-variable [namespace current]::Name_ \
		-command "
			$top.name_elast configure -state \$[namespace current]::Name_
			$top.name_efirst configure -state \$[namespace current]::Name_
			"
	set name [::ttk::labelframe $top.name -labelwidget $top.namecb]
	::ttk::label $top.name_llast -text $mc::F_LastName
	::ttk::label $top.name_lfirst -text $mc::F_FirstName
	::ttk::entry $top.name_elast -textvariable [namespace current]::LastName_ -state disabled
	::ttk::entry $top.name_efirst -textvariable [namespace current]::FirstName_ -state disabled

	grid $top.name_llast  -row 1 -column 1 -sticky w -in $name
	grid $top.name_lfirst -row 1 -column 3 -sticky w -in $name
	grid $top.name_elast  -row 3 -column 1 -in $name
	grid $top.name_efirst -row 3 -column 3 -in $name
	grid rowconfigure $name {2 4} -minsize $::theme::padding
	grid columnconfigure $name {0 2 4} -minsize $::theme::padding

	### ELO ################################################
	set [namespace current]::Elo_ disabled
	set [namespace current]::Score_ [abs [lindex $iElo 0]]
	::ttk::checkbutton $top.elocb \
		-text "Elo" \
		-onvalue normal \
		-offvalue disabled \
		-variable [namespace current]::Elo_ \
		-command "$top.elo_text configure -state \$[namespace current]::Elo_" \
		;
	set elo [::ttk::labelframe $top.elo -labelwidget $top.elocb]
	::ttk::label $top.elo_label -text $mc::Score
	::ttk::spinbox $top.elo_text \
		-textvariable [namespace current]::Score_ \
		-state disabled \
		-width 5 \
		-from 0 \
		-to 4000 \
		;
	::validate::spinboxInt $top.elo_text
	::theme::configureSpinbox $top.elo_text

	grid $top.elo_label -row 1 -column 1 -sticky w -in $elo
	grid $top.elo_text  -row 1 -column 3 -in $elo
	grid rowconfigure $elo {0 2} -minsize $::theme::padding
	grid columnconfigure $elo {0 2 4} -minsize $::theme::padding

	### Rating #############################################
	### Federation #########################################
	### Titles #############################################

	########################################################

	grid $top.name	-row 1 -column 1 -sticky ew
	grid $top.elo  -row 3 -column 1 -sticky w

	grid rowconfigure $top {0 2 4} -minsize $::theme::padding
	grid columnconfigure $top {0 2} -minsize $::theme::padding

	::widget::dialogButtons $dlg {ok cancel} ok
	$dlg.cancel configure -command [list destroy $dlg]

	wm withdraw $dlg
	wm protocol $dlg WM_DELETE_WINDOW [list destroy $dlg]
	wm transient $dlg $parent
	wm title $dlg "[tk appname] - $mc::EditPlayer"
	wm resizable $dlg false false
	::util::place $dlg center $parent
	wm deiconify $dlg
	focus $top.namecb
}

} ;# namespace players

namespace eval events {

proc Update {path base {view -1} {index -1}} {
	variable [namespace parent]::${path}::Vars

	set Vars($base:update:events) 1
	after cancel $Vars(after:events)
	set Vars(after:events) [after idle [namespace code [list Update2 $path $base]]]
}


proc Update2 {path base} {
	variable [namespace parent]::${path}::Vars

	[namespace parent]::InitBase $path $base
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
			set n [::scidb::view::count events $base $Vars($base:view)]
			after idle [list ::eventtable::update $path.info.events $base $n]
			set Vars($base:update:events) 0
		}
	} else {
		set Vars($base:update:events) 1
	}
}

} ;# namespace events


proc WriteOptions {chan} {
	variable Tables

	foreach table $Tables {
		puts $chan "::playertable::setOptions $table.players {"
		::options::writeArray $chan [::playertable::getOptions $table.players]
		puts $chan "}"
		puts $chan "::gametable::setOptions $table.info.games {"
		::options::writeArray $chan [::gametable::getOptions $table.info.games]
		puts $chan "}"
		puts $chan "::eventtable::setOptions $table.info.events {"
		::options::writeArray $chan [::eventtable::getOptions $table.info.events]
		puts $chan "}"
	}
}

::options::hookWriter [namespace current]::WriteOptions

} ;# namespace players
} ;# namespace database
} ;# namespace application

# vi:set ts=3 sw=3:
