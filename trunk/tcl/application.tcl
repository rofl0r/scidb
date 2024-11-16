# ======================================================================
# Author : $Author$
# Version: $Revision: 1188 $
# Date   : $Date: 2017-05-31 07:42:21 +0000 (Wed, 31 May 2017) $
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
# Copyright: (C) 2009-2013 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

::util::source application

namespace eval application {
namespace eval mc {

set Information				"&Information"
set Database					"&Database"
set Board						"&Board"
set MainMenu					"&Main Menu"

set DockWindow					"Dock Window"
set UndockWindow				"Undock Window"
set ChessInfoDatabase		"Chess Information Data Base"
set Shutdown					"Shutdown..."
set QuitAnyway					"Quit anyway?"
set CancelLogout				"Cancel Logout"
set AbortWriteOperation		"Abort write operation"

set UpdatesAvailable			"Updates available"

set WriteOperationInProgress "Write operation in progress: currently Scidb is modifying/writing database '%s'."
set LogoutNotPossible		"Logout is currently not possible, the result would be a corrupted database."
set RestartLogout				"Aborting the write operation will restart the logout process."
set UnsavedFiles				"The following PGN files are unsaved:"
set ThrowAwayAllChanges		"Do you really want to throw away all changes?"

set Deleted						"Games deleted: %d"
set Changed						"Games changed: %d"
set Added						"Games added: %d"
set DescriptionHasChanged	"Description has changed"

} ;# namespace mc

namespace import ::tcl::mathfunc::abs
namespace import ::tcl::mathfunc::max

array set Attr {
	top,width				900
	top,height				520
	top,minWidth			400
	top,minHeight			300
	top,stretch				always
	top,float				0
	top,before				{}
	top,after				{}
	top,type					panedwindow

	board,width				580
	board,height			520
	board,minWidth			200
	board,minHeight		200
	board,stretch			always
	board,float				0
	board,before			{}
	board,after				{}
	board,type				frame

	right,width				320
	right,height			520
	right,minWidth			280
	right,minHeight		200
	right,stretch			never
	right,float				0
	right,before			{}
	right,after				board
	right,type				frame

	pgn,width				320
	pgn,height				350
	pgn,minWidth			200
	pgn,minHeight			200
	pgn,stretch				always
	pgn,float				1
	pgn,before				{}
	pgn,after				{}
	pgn,type					frame

	bottom,width			900
	bottom,height			200
	bottom,minWidth		400
	bottom,minHeight		150
	bottom,stretch			never
	bottom,float			0
	bottom,before			{}
	bottom,after			top
	bottom,type				panedwindow

	tree,width				400
	tree,height				200
	tree,minWidth			200
	tree,minHeight			150
	tree,stretch			never
	tree,float				1
	tree,before				{}
	tree,after				{}
	tree,type				frame

	games,width				450
	games,height			200
	games,minWidth			200
	games,minHeight		150
	games,stretch			always
	games,float				1
	games,before			{}
	games,after				tree
	games,type				frame

	clock,width				320
	clock,height			150
	clock,minWidth			280
	clock,minHeight		100
	clock,stretch			never
	clock,float				1
	clock,before			{}
	clock,after				pgn
	clock,type				frame
}

array set Vars {
	tabs:changed	0
	exit:save		1
	active			1

	menu:locked			0
	menu:state			normal
	menu:background	#c3c3c3

	updates {}
}


proc open {} {
	variable Attr
	variable Vars

	# setup
	::move::setup
	::tcl::mathfunc::srand [clock milliseconds]
	set app .application
	::widget::dialogWatch $app
	set ::util::place::mainWindow $app
	wm protocol $app WM_DELETE_WINDOW [namespace code shutdown]
	set nb [::ttk::notebook $app.nb -takefocus 0] ;# otherwise board does not have focus
	set Vars(control) [::widget::dialogFullscreenButtons $nb]
	bind $nb <Configure> [namespace code PlaceMenues]
	bind $Vars(control) <Configure> +[namespace code PlaceMenues]
	::theme::configureBackground $Vars(control).minimize
	::theme::configureBackground $Vars(control).restore
	::theme::configureBackground $Vars(control).close
	$Vars(control).minimize configure -command { wm iconify .application }
	$Vars(control).restore configure -command { ::menu::viewFullscreen toggle }
	$Vars(control).close configure -command [namespace code shutdown]

	set m [tk::menubutton $nb.menu_main \
		-borderwidth 1 \
		-relief raised \
		-padx 2 \
		-pady 2 \
		-background $Vars(menu:background) \
		-activebackground [::dropdownbutton::activebackground] \
		-activeforeground [::dropdownbutton::activeforeground] \
		-foreground black \
		-activeforeground white \
		-image $icon::16x12::downArrow(black) \
		-compound right \
	]
	SetSettingsText $m
	bind $m <<LanguageChanged>> [namespace code [list SetSettingsText $m]]
	bind $m <Enter> [namespace code [list EnterSettings $m]]
	bind $m <Leave> [namespace code [list LeaveSettings $m]]
	bind $m <<MenuWillPost>> [namespace code [list BuildSettingsMenu $m]]
	bind $m <<MenuWillUnpost>> [namespace code [list FinishSettings $m]]
	bind $m <Configure> [namespace code PlaceMenues]
	set Vars(menu:main) $m
	set Vars(menu:updates) $nb.menu_updates

	::ttk::notebook::enableTraversal $nb
	set info [::ttk::frame $nb.information]
	set db [::ttk::frame $nb.database]
	set main [tk::panedwindow $nb.board -orient vertical -opaqueresize true]
	$nb add $info -sticky nsew
	$nb add $db   -sticky nsew
	$nb add $main -sticky nsew
	::widget::notebookTextvarHook $nb $info [namespace current]::mc::Information
	::widget::notebookTextvarHook $nb $db   [namespace current]::mc::Database
	::widget::notebookTextvarHook $nb $main [namespace current]::mc::Board

	bind $main <Configure> [namespace code [list ConfigureEvent main $main %W %w %h]]
#	bind $app <Tab> [namespace code [list SwitchTab $nb +1]]
#	bind $app <Shift-Tab> [namespace code [list SwitchTab $nb -1]]
#	bind $app <ISO_Left_Tab> [namespace code [list SwitchTab $nb -1]]
	pack $nb -fill both -expand yes

	if {[::process::testOption show-board]} {
		set tab board
	} elseif {[::process::testOption re-open] || [llength [::process::arguments]]} {
		set tab database
	} else {
		set tab information
	}
	$nb select .application.nb.$tab

	foreach {sub} {top bottom} {
		set $sub [tk::panedwindow $main.$sub -orient horizontal -opaqueresize true]
		$main paneconfigure $main.$sub \
			-sticky nswe \
			-minsize $Attr($sub,minHeight) \
			-stretch $Attr($sub,stretch) \
			;
		bind $main.$sub <Configure> [namespace code [list ConfigureEvent $sub $main.$sub %W %w %h]]
		$main add $main.$sub
	}

	foreach {sub class} {board Board right Frame} {
		set $sub [tk::frame $top.$sub \
						-class $class \
						-width $Attr($sub,width) \
						-height $Attr($sub,height)]
		$top paneconfigure $top.$sub \
			-sticky nswe \
			-minsize $Attr($sub,minWidth) \
			-stretch $Attr($sub,stretch) \
			;
		bind $top.$sub <Configure> [namespace code [list ConfigureEvent $sub $top.$sub %W %w %h]]
		$top add $top.$sub

#		if {$Attr($sub,float)} {
			set m [menu $top.$sub.popup -tearoff false]
#			$m add command -command [namespace code [list Undock $top.$sub $sub]]
#			::widget::menuTextvarHook $m 0 [namespace current]::mc::UndockWindow
#			bind $top.$sub <ButtonPress-3> [list tk_popup $m %X %Y 0]
#		}
	}

#	set right [tk::panedwindow $right.pw -orient vertical -opaqueresize true]
#	pack $right -fill both -expand yes

#	foreach {sub class} {pgn Frame clock Frame} {
#		set $sub [tk::frame $right.$sub \
#						-class $class \
#						-width $Attr($sub,width) \
#						-height $Attr($sub,height)]
#		pack [set $sub] -side top -fill both -expand yes
#
#		if {$Attr($sub,float)} {
#			set m [menu $right.$sub.popup -tearoff false]
#			$m add command -command [namespace code [list Undock $right.$sub $sub]]
#			::widget::menuTextvarHook $m 0 [namespace current]::mc::UndockWindow
#			bind $right.$sub <ButtonPress-3> [list tk_popup $m %X %Y 0]
#		}
#	}

	set pgn [tk::frame $right.pgn -class Frame -width $Attr(pgn,width)]

if {[::process::testOption use-clock]} {
	set clock [tk::frame $right.clock -class Frame -width $Attr(clock,width)]
	grid $clock -row 0 -column 0 -sticky nsew
}

	grid $pgn -row 1 -column 0 -sticky nsew

	grid rowconfigure $right 1 -weight 1
	grid columnconfigure $right 0 -weight 1

	foreach {sub class} {tree Frame games Frame} {
		set $sub [tk::frame $bottom.$sub \
						-class $class \
						-width $Attr($sub,width) \
						-height $Attr($sub,height)]
		$bottom paneconfigure $bottom.$sub \
			-sticky nswe \
			-minsize $Attr($sub,minWidth) \
			-stretch $Attr($sub,stretch) \
			;
		if {$sub eq "tree"} { $bottom paneconfigure $bottom.$sub -width 400 }
		bind $bottom.$sub <Configure> [namespace code [list ConfigureEvent $sub $bottom.$sub %W %w %h]]
		$bottom add $bottom.$sub

#		if {$Attr($sub,float)} {
#			set m [menu $bottom.$sub.popup -tearoff false]
#			$m add command -command [namespace code [list Undock $bottom.$sub $sub]]
#			::widget::menuTextvarHook $m 0 [namespace current]::mc::UndockWindow
#			bind $bottom.$sub <ButtonPress-3> [list tk_popup $m %X %Y 0]
#		}
	}

	information::build $info $Attr(board,width) $Attr(board,height)
	database::build $db $Attr(board,width) $Attr(board,height)
	board::build $top.board $Attr(board,width) $Attr(board,height)
	pgn::build $right.pgn $Attr(pgn,width) $Attr(pgn,height)
	tree::build $bottom.tree $Attr(tree,width) $Attr(tree,height) $bottom.games
	tree::games::build $bottom.games $Attr(games,width) $Attr(games,height)

if {[::process::testOption use-clock]} {
	clock::build $right.clock $Attr(clock,width) $Attr(clock,height)
}

	bind $nb <<NotebookTabChanged>> [namespace code [list TabChanged $nb $app]]
	bind $app <Destroy> [namespace code { Exit %W }]
	bind $app <FocusIn> [namespace code [list Activate $nb]]
	bind $app <FocusOut> [namespace code [list Deactivate $nb]]
	ComputeMinSize $main

	if {[tk windowingsystem] eq "x11"} {
		wm client $app [lindex [split [info hostname] .] 0]
		wm protocol $app WM_SAVE_YOURSELF [namespace code [list WmSaveYourself $app]]
		WmSaveYourself $app 0 ;# initial setup

		::scidb::tk::sm connect \
			-restart no \
			-saveYourself [namespace code SmSaveYourself] \
			-interactRequest [namespace code SmInteractRequest] \
			;
	}

	::util::place $app -position center
	::widget::dialogSetTitle $app [namespace code Title]
	wm deiconify $app
	database::finish $app
	::splash::close
	ChooseLanguage $app
	TabChanged $nb $app
	::load::writeLog
	update idletasks
	set ::remote::blocked 0

	database::preOpen $app

	foreach file [::process::arguments] {
		database::openBase .application [::util::databasePath $file] yes \
			-encoding $::encoding::autoEncoding
	}
 
	if {[::game::recover $app] + [::game::reopenLockedGames $app] > 0} {
		set tab board
	}

	after idle [namespace code [list switchTab $tab]]
	after idle [list ::beta::welcomeToScidb $app]
	::util::photos::checkForUpdate [namespace current]::InformAboutUpdates
}


proc shutdown {} {
	variable icon::32x32::shutdown
	variable ::scidb::mergebaseName
	variable Vars

	set dlg .application.shutdown
	if {[winfo exists $dlg]} { return }

	if {[::dialog::messagebox::open?] eq "question"} { bell; return }
	if {[string match .application* [grab current]]} { bell; return }

	::widget::dialogRaise .application

	if {[::util::photos::busy?]} {
		append msg $::util::photos::mc::DownloadStillInProgress "\n\n"
		append msg $mc::QuitAnyway
		set reply [::dialog::question -parent .application -message $msg -default no]
		if {$reply ne "yes"} { return }
		::util::photos::terminateUpdate
	}

	set unsavedFiles [::scidb::app::get unsavedFiles]
	set n 0
	if {$mergebaseName in $unsavedFiles} { incr n }

	if {[llength $unsavedFiles] > $n} {
		append msg $mc::UnsavedFiles
		append msg <embed>
		append msg $mc::ThrowAwayAllChanges
	
		set reply [::dialog::question \
			-parent .application \
			-message $msg \
			-default no \
			-embed [namespace code [list EmbedUnsavedFiles $unsavedFiles]] \
		]
		if {$reply ne "yes"} { return }
	}

	switch [::game::queryCloseApplication .application] {
		restore	{ set backup 1 }
		discard	{ set backup 0 }
		cancel	{ return }
	}

	foreach toplevel [winfo children .] {
		if {$toplevel ne ".application"} { destroy $toplevel }
	}

	tk::toplevel $dlg -class Scidb
	wm withdraw $dlg
	pack [tk::frame $dlg.f -border 2 -relief raised]
	pack [tk::label $dlg.f.text -compound left -image $shutdown -text " $mc::Shutdown"] -padx 10 -pady 10
	wm resizable $dlg no no
	wm transient $dlg .application
	::util::place $dlg -parent .application -position center
	update idletasks
	::scidb::tk::wm frameless $dlg
	wm deiconify $dlg
	::ttk::grabWindow $dlg
	::widget::busyCursor on

	prepareExit $backup
	if {[tk windowingsystem] eq "x11"} { ::scidb::tk::sm disconnect }

	::widget::busyCursor off
	::ttk::releaseGrab $dlg
	destroy .application
}


proc prepareExit {{backup 1}} {
	variable Vars

	if {$Vars(exit:save)} {
		::log::delay
		::remote::cleanup
		database::prepareClose
		::scidb::app::close
		if {$backup} { ::game::backup }
		::scidb::app::finalize
		set Vars(exit:save) 0
	}
}


proc switchTab {which} {
	.application.nb select .application.nb.$which
	update idletasks
	${which}::setFocus
}


if {[tk windowingsystem] eq "x11"} {

proc WmSaveYourself {app {shutdown 0}} {
	if {$shutdown} { prepareExit }
	wm command $app [concat [::scidb::tk::sm get -command] [::scidb::tk::sm get -argv]]
}


proc SmSaveYourself {shutdown} {
	if {$shutdown} {
		prepareExit
	}
}


proc SmInteractRequest {shutdown} {
	set base [::scidb::app::writing -background no]

	## Handle foreground process ####################################
	if {[string length $base]} {
		set parent [grab current]
		if {[string length $parent] == 0} { set parent .application }
		set base [::util::databaseName $base]
		set msg [format $mc::WriteOperationInProgress $base]

		if {![::scidb::progress::interruptable?]} {
			set detail $mc::LogoutNotPossible
			dialog::error -parent $parent -message $msg -detail $detail -topmost yes 
		} else {
			set buttons [list [list cancel $mc::CancelLogout] [list abort $mc::AbortWriteOperation]]
			set detail  $mc::RestartLogout
			set reply [::dialog::warning \
				-parent $parent \
				-message $msg \
				-detail $detail \
				-buttons $buttons \
				-topmost yes \
				-centeronscreen yes \
			]
			if {$reply eq "cancel"} { return 0 }
			set cmd [namespace code [list SmCallSaveYourself $shutdown]]
			if {[::scidb::progress::interrupt -inform $cmd]} { return 1 }
		}

		::log::suppress yes
		update
		return 0
	}

	## Handle background process - always interruptable #############
	set base [::scidb::app::writing -background yes]
	if {[string length $base] == 0} { return 1 }
	set base [::util::databaseName $base]
	set msg [format $mc::WriteOperationInProgress $base]
	set buttons [list [list cancel $mc::CancelLogout] [list abort $mc::AbortWriteOperation]]

	set reply [::dialog::warning -parent $parent \
		-message $msg \
		-buttons $buttons \
		-topmost yes \
		-centeronscreen yes \
	]
	if {$reply eq "cancel"} { return 0 }
	::log::suppress yes
	::scidb::progress::interrupt -wait yes
	return 1
}


proc SmCallSaveYourself {shutdown} {
	update
	::log::suppress no
	::scidb::tk::sm saveyourself -shutdown $shutdown
}

} ;# [tk windowingsystem] eq "x11"


proc InformAboutUpdates {item} {
	variable Vars

	lappend Vars(updates) $item
	BuildUpdatesButton
}


proc Activate {nb} {
	variable Vars

	if {!$Vars(active)} {
		set Vars(active) 1
		set current [lindex [split [$nb select] .] end]
		${current}::setActive yes
	}
}	


proc Deactivate {nb} {
	variable Vars

	if {$Vars(active) && ![string match {.application.*} [focus]]} {
		set Vars(active) 0
		set current [lindex [split [$nb select] .] end]
		${current}::setActive no
	}
}	


proc EnterSettings {w} {
	variable Vars

	set Vars(menu:state) active

	if {!$Vars(menu:locked)} {
		$w configure -state active -image $icon::16x12::downArrow(white)
	}
}


proc LeaveSettings {w} {
	variable Vars

	set Vars(menu:state) normal

	if {!$Vars(menu:locked)} {
		$w configure -state normal -image $icon::16x12::downArrow(black)
	}
}


proc BuildSettingsMenu {m} {
	variable Vars

	catch { destroy $m.entries }
	::menu $m.entries
	::menu::build $m.entries

	$m configure \
		-background [::dropdownbutton::activebackground] \
		-activebackground [::dropdownbutton::activebackground] \
		-foreground white \
		-activeforeground [::dropdownbutton::activeforeground] \
		-image $icon::16x12::downArrow(white) \
		-menu $m.entries \
		-direction below \
		;
	set Vars(menu:locked) 1
}


proc FinishSettings {m} {
	variable Vars

	$m configure \
		-background $Vars(menu:background) \
		-activebackground [::dropdownbutton::activebackground] \
		-foreground black \
		-activeforeground white \
		-image $icon::16x12::downArrow(black) \
		;
	set Vars(menu:locked) 0

	if {$Vars(menu:state) eq "normal"} {
		LeaveSettings $m
	} else {
		EnterSettings $m
	}
}


proc MakeUpdateInfo {} {
	variable Vars

	if {[llength $Vars(updates)]} {
		set Vars(updates:tooltip) "${mc::UpdatesAvailable}:"
		foreach item $Vars(updates) {
			switch $item {
				photos { append Vars(updates:tooltip) \n $::util::photos::mc::PhotoFiles }
			}
		}
	}
}


proc BuildUpdatesButton {} {
	variable Vars

	set m $Vars(menu:updates)
	if {[winfo exists $m]} { return }

	tk::menubutton $m \
		-borderwidth 1 \
		-relief raised \
		-padx 2 \
		-pady 2 \
		-background $Vars(menu:background) \
		-activebackground [::dropdownbutton::activebackground] \
		-foreground black \
		-activeforeground white \
		-image $icon::16x16::softwareUpdate \
		;
	bind $m <<LanguageChanged>> [namespace code MakeUpdateInfo]
	bind $m <Enter> [list set [namespace current]::Vars(menu:state) active]
	bind $m <Leave> [list set [namespace current]::Vars(menu:state) normal]
	bind $m <<MenuWillPost>> [namespace code [list BuildUpdatesMenu $m]]
	bind $m <<MenuWillUnpost>> [namespace code [list FinishUpdates $m]]
	bind $m <Configure> [namespace code PlaceMenues]

	PlaceMenues
	MakeUpdateInfo
	tooltip::tooltip $m [namespace current]::Vars(updates:tooltip)
}


proc BuildUpdatesMenu {m} {
	variable Vars

	catch { destroy $m.updates }
	::menu $m.updates

	foreach item $Vars(updates) {
		switch $item {
			photos { set txt $::util::photos::mc::PhotoFiles }
		}
		$m.updates add command -label $txt -command [namespace code [list InstallUpdate $item]]
	}

	$m configure \
		-background [::dropdownbutton::activebackground] \
		-activebackground [::dropdownbutton::activebackground] \
		-foreground white \
		-activeforeground white \
		-menu $m.updates \
		-direction below \
		;
	set Vars(menu:locked) 1
}


proc FinishUpdates {m} {
	variable Vars

	$m configure \
		-background $Vars(menu:background) \
		-activebackground [::dropdownbutton::activebackground] \
		-foreground black \
		-activeforeground white \
		;
	set Vars(menu:locked) 0
}


proc InstallUpdate {item} {
	variable Vars

	::util::photos::openDialog .application
	set i [lsearch $Vars(updates) $item]
	set Vars(updates) [lreplace $Vars(updates) $i $i]

	if {[llength $Vars(updates)] == 0} {
		destroy $Vars(menu:updates)
	}
}


proc EmbedUnsavedFiles {unsaved w infoFont alertFont} {
	::html $w.t \
		-center no \
		-fittowidth no \
		-borderwidth 0 \
		-doublebuffer no \
		-exportselection yes \
		-imagecmd [namespace code GetImage] \
		-css "html { background: [$w cget -background] }"
		;
	place $w.t -x 10 -y 0

	$w.t onmouseover [list [namespace current]::MouseEnter $w.t]
	$w.t onmouseout  [list [namespace current]::MouseLeave $w.t]

	array set font [font actual $infoFont]
	set family $font(-family)
	set size [expr {abs($font(-size))}]

	append content "<table style='font-family: ${family}; font-size: ${size}px;'>"

	foreach file $unsaved {
		lassign [::scidb::db::get changes $file] added changed deleted descriptionHasChanged
		append content "<tr>"
		append content "<td id='$file'>[::util::databaseName $file]&ensp;</td>"
		if {$deleted > 0} {
			append content "<td id='Deleted' count='$deleted'><img src='deleted'></td>"
		}
		if {$changed > 0} {
			append content "<td id='Changed' count='$changed'><img src='edit'></td>"
		}
		if {$added > 0} {
			append content "<td id='Added' count='$added'><img src='plus'></td>"
		}
		if {$descriptionHasChanged} {
			append content "<td id='DescriptionHasChanged'><img src='info'></td>"
		}
		append content "</tr>"
	}

	append content "</table>"
	lassign [$w.t parse $content] x0 y0 x1 y1
	set margins [expr {2*[$w.t margin]}]
	$w configure -width [expr {$x1 - $x0 + $margins + 10}] -height [expr {$y1 - $y0 + $margins}]
}


proc GetImage {code} {
	return [list [set ::icon::12x12::$code] [namespace code DoNothing]]
}


proc DoNothing {args} {
	# nothing to do
}


proc MouseEnter {w nodes} {
	foreach node $nodes {
		set id [$node attribute -default {} id]
		if {[llength $id]} {
			if {[info exists mc::$id]} {
				set count [$node attribute -default 0 count]
				if {$count} {
					set id [format [set mc::$id] $count]
				} else {
					set id [set mc::$id]
				}
			}
			::tooltip::show $w $id
		}
	}
}


proc MouseLeave {w node} {
	foreach node $nodes {
		set id [$node attribute -default {} id]
		if {[llength $id]} { return [::tooltip::hide] }
	}
}


proc PlaceMenues {} {
	variable Vars

	# place controls
	set w $Vars(control)
	set parent [winfo parent $w]
	if {[::menu::fullscreen?]} {
		set x [expr {[winfo width $parent] - [winfo width $w]}]
		place $w -x $x -y 0 -height [winfo height $Vars(menu:main)]
	} else {
		place forget $w
	}

	# place main menu
	set m $Vars(menu:main)
	set parent [winfo parent $m]
	set x [expr {[winfo width $parent] - [winfo width $m]}]
	if {$Vars(control) in [place slaves $parent]} {
		set x [expr {$x - [winfo width $Vars(control)]}]
	}
	place $m -x $x -y 0

	# place update menu
	set m $Vars(menu:updates)
	if {[winfo exists $m]} {
		$m configure -height [expr {[winfo height $Vars(menu:main)] - 2}]
		set x [expr {$x - [winfo width $m] - 5}]
		place $m -x $x -y 0
	}
}


proc SetSettingsText {w} {
	variable Vars

	lassign [::tk::UnderlineAmpersand $mc::MainMenu] text ul
	$w configure -text " $text" -underline [incr ul]
}


proc ChooseLanguage {parent} {
	variable ::country::icon::flag

	if {!$::scidb::dir::setup} { return }
	wm protocol $parent WM_DELETE_WINDOW {#}
	set dlg $parent.lang
	tk::toplevel $dlg -class Scidb
	wm withdraw $dlg
	set top [tk::frame $dlg.top -border 2 -relief raised]
	pack $top
	set r 0
	foreach lang [lsort [array names ::mc::input]] {
		if {[string length $lang]} {
			set icon ""
			catch { set icon $flag([set ::mc::langToCountry([set ::mc::lang$lang])]) }
			if {[string length $icon] == 0} { set icon none }
			set code [set ::mc::lang$lang]
			ttk::button $top.$code \
				-style aligned.TButton \
				-text " $lang" \
				-image $icon \
				-compound left \
				-command [namespace code [list SetupLang $code]] \
				;
			grid $top.$code -column 1 -row [incr r 1]
			bind $top.$code <Return>	{ event generate %W <Key-space>; break }
			bind $top.$code <Down>		{ focus [tk_focusNext [focus]] }
			bind $top.$code <Up>			{ focus [tk_focusPrev [focus]] }
		}
	}
	wm resizable $dlg no no
	wm transient $dlg $parent
	::util::place $dlg -parent $parent -position center
	update idletasks
	::scidb::tk::wm frameless $dlg
	wm deiconify $dlg
	focus $top.en
	::ttk::grabWindow $dlg
	vwait ::mc::langID
	::ttk::releaseGrab $dlg
	catch { destroy $dlg }
	::mc::setLang $mc::langID
	wm protocol $parent WM_DELETE_WINDOW [namespace code shutdown]
	focus -force .application
}


proc SetupLang {langID} {
	pgn::setActiveLang $::mc::langID 0
	set ::mc::langID $langID
	::font::useLanguage $langID
	pgn::setActiveLang $langID 1
}


proc Title {} {
	return "$::scidb::app - $mc::ChessInfoDatabase"
}


proc Exit {w} {
	if {$w eq ".application"} {
		set ::remote::blocking 1
		set ::remote::postponed 0
		::options::write
		::load::write
		exit
	}
}


proc TabChanged {nb app} {
	variable Vars

	set current [lindex [split [$nb select] .] end]

	switch $current {
		information	{
			information::activate $nb.information 1
			database::activate $nb.database 0
			board::activate $nb.board.top.board 0
			tree::activate $nb.board.bottom.tree 0
#			clock::activate $nb.board.top.right.clock 0
		}

		database	{
			database::activate $nb.database 1
			information::activate $nb.information 0
			board::activate $nb.board.top.board 0
			tree::activate $nb.board.bottom.tree 0
#			clock::activate $nb.board.top.right.clock 0
		}

		board {
			database::activate $nb.database 0
			information::activate $nb.information 0
			board::activate $nb.board.top.board 1
			tree::activate $nb.board.bottom.tree 1
#			clock::activate $nb.board.top.right.clock 1
		}
	}

	if {$Vars(tabs:changed) > 0} {
		# avoid resizing (multicolumn problem)
		wm geometry $app [wm geometry $app]
	}
	incr Vars(tabs:changed)
}


proc SwitchTab {nb dir} {
	set index [expr {[$nb index [$nb select]] + $dir}]
	set num [llength [$nb tabs]]
	if {$index == -1} { set index [expr {$num - 1}] }
	if {$index == $num} { set index 0 }
	$nb select $index
}


proc ComputeMinSize {main} {
	variable Attr

	set sash [$main cget -sashwidth]
	if {[$main cget -showhandle]} { set sash [max $sash [$main cget -handlesize]] }
	set sash [expr {$sash + 2*[$main cget -sashpad]}]
	set minW [expr {2*[$main cget -borderwidth] - ([llength [$main panes]] > 1 ? $sash : 0)}]
	set minH 0

	foreach sub [$main panes] {
		set which [lindex [split $sub "."] end]
		set minH [max $minH $Attr($which,minHeight)]
		incr minW $Attr($which,minWidth)
		incr minW $sash
	}

	wm minsize [winfo toplevel $main] $minW $minH
}


proc ConfigureEvent {name pane w width height} {
	variable Attr

	if {$pane ne $w || $width <= 1} { return }

	set Attr($name,width) $width
	set Attr($name,height) $height
}


proc Undock {w name} {
	variable Attr

	set x [winfo rootx $w]
	set y [winfo rooty $w]
	set main [winfo parent $w]
	$main forget $w
	$w.popup delete 0
	$w.popup add command -command [namespace code [list Dock $w $name]]
	::widget::menuTextvarHook $w.popup 0 [namespace current]::mc::DockWindow
	$w configure -width $Attr($name,width) -height $Attr($name,height)
	wm manage $w ;# -iconic
	::util::place $w -x $x -y [expr {$y - 22}]
	wm protocol $w WM_DELETE_WINDOW [namespace code [list Dock $w $name]]
#	wm iconphoto $w -default $::icon::64x64::logo $::icon::16x16::logo
	ComputeMinSize $main
}


proc Dock {w name} {
	variable Attr

	set main [winfo parent $w]
	wm forget $w
	$main add $w -minsize $Attr($name,minWidth) -stretch $Attr($name,stretch)
	$w.popup delete 0
	$w.popup add command \
		-label [::mc::translate [namespace current]::mc::UndockWindow] \
		-command [namespace code [list Undock $w $name]]

	if {[llength $Attr($name,before)]} { $main paneconfigure $w -before $main.$Attr($name,before) }
	if {[llength $Attr($name,after)]} { $main paneconfigure $w -after $main.$Attr($name,after) }

	ComputeMinSize $main
}


wm iconphoto .application -default $::icon::64x64::logo $::icon::16x16::logo


namespace eval icon {
namespace eval 16x12 {

set downArrow(white) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAMAQMAAABRKa/CAAAABlBMVEUAAwD////ieeUNAAAA
	AXRSTlMAQObYZgAAAB9JREFUCNdjYIAB+z8M8j8Y+D8wsD9gYD7AwNgAlwEAZ6cFfe/e8VYA
	AAAASUVORK5CYII=
}]

set downArrow(black) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAMAQMAAABRKa/CAAAABlBMVEUAAwAAAADix8MfAAAA
	AXRSTlMAQObYZgAAAB9JREFUCNdjYIAB+z8M8j8Y+D8wsD9gYD7AwNgAlwEAZ6cFfe/e8VYA
	AAAASUVORK5CYII=
}]

} ;# namespace 16x12
namespace eval 32x32 {

set shutdown [image create photo -data {
	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
}]

} ;# namespace 32x32
} ;# namespace icon
} ;# namespace application

rename ::tk::PostOverPoint ::tk::_PostOverPoint_application

proc ::tk::PostOverPoint {menu x y {entry {}}} {
	if {[string match ".application.nb.menu_*.entries" $menu]} {
		set rx [winfo rootx .application]
		set mw [winfo reqwidth $menu]
		set x [expr {min($rx + [winfo width .application] - $mw, $x)}]
	}

	::tk::_PostOverPoint_application $menu $x $y $entry
}

# vi:set ts=3 sw=3:
