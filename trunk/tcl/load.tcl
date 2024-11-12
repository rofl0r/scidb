# ======================================================================
# Author : $Author$
# Version: $Revision: 42 $
# Date   : $Date: 2011-06-13 23:31:52 +0000 (Mon, 13 Jun 2011) $
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

namespace eval load {
namespace eval mc {

set FileIsCorrupt 		"File %s is corrupt:"

set Loading					"Loading %s"
set ReadingOptionsFile	"Reading options file"
set StartupFinished		"Startup finished"

set ECOFile					"ECO file"
set EngineFile				"engine file"
set SpellcheckFile		"spell-check file"
set LocalizationFile		"localization file"
set RatingList				"%s rating list"
set WikipediaLinks		"Wikipedia links"
set ChessgamesComLinks	"chessgames.com links"
set Cities					"cities"
set PhotoIndex				"photo index"
set PieceSet				"piece set"
set Theme					"theme"
set Icons					"icons"

}

variable currentFile {}
variable photoFile {}

variable LastMsg	""
variable Log		{}


proc source {path args} {
	variable LastMsg
	variable Log
	variable currentFile

	array set opts { -message "" -encoding "" }
	array set opts $args
	set msg $opts(-message)

	if {[string length $msg]} {
		if {$msg ne $LastMsg} {
			::splash::print "${msg}..."
			set LastMsg $msg
		}
	}

	if {[string match {.sp[ai]} [file extension $path]]} {
		set [namespace current]::photoFile [file rootname $path].spf
	}

	set currentFile $path
	set enc {}
	if {[string length $opts(-encoding)]} { set enc [list -encoding $opts(-encoding)] }

	if {[catch {uplevel ::source {*}$enc [list $path]} err]} {
		lappend Log error [format $mc::FileIsCorrupt $path] error $err
	} elseif {[string length $msg]} {
		lappend Log info "$msg: $path"
	}
}


proc load {msg type path} {
	variable LastMsg
	variable Log

	if {$msg ne $LastMsg} {
		::splash::print "${msg}..."
		set LastMsg $msg
	}

	set currentFile $path

	if {[catch {::scidb::app::load $type $path} err]} {
		set msg [format $mc::FileIsCorrupt $path]
		lappend Log error $msg error $err
		puts "$msg -- $err"
	} else {
		lappend Log info "$msg: $path"
	}
}


proc writeLog {} {
	variable Log

	foreach {type msg} $Log { log::$type $msg }
	array unset Log
	::log::info $mc::StartupFinished
	::log::close
}


proc write {} {
	set chan [open [file join $scidb::dir::config load.tcl] w]
	fconfigure $chan -encoding utf-8

	foreach name [info vars ::load::mc::*] {
		puts $chan "set $name \"[set $name]\""
	}

	close $chan
}

} ;# namespace load


if {[file readable [file join $scidb::dir::config load.tcl]]} {
	catch { source -encoding utf-8 [file join $scidb::dir::config load.tcl] }
}


# log will be closed in end.tcl
::log::open "Startup" 0

# --- Load ECO file ----------------------------------------------------
load::load	[format $load::mc::Loading $load::mc::ECOFile] \
				eco \
				[file join $scidb::dir::data eco.bin] \
				;

# --- Load engines -----------------------------------------------------
load::load	[format $load::mc::Loading $load::mc::EngineFile] \
				comp \
				[file join $scidb::dir::data engines.txt] \
				;

if {![::process::testOption fast-load]} {

# --- Load spellcheck files --------------------------------------------
foreach file {ratings_utf8.ssp.zip ratings-additional.ssp} {
	load::load	[format $load::mc::Loading $load::mc::SpellcheckFile] \
					ssp \
					[file join $scidb::dir::data $file] \
					;
}

# --- Load FIDE players ------------------------------------------------
load::load	[format $load::mc::Loading [format $load::mc::RatingList FIDE]] \
				fide \
				[file join $scidb::dir::data players_list.zip] \
				;

if {![::process::testOption elo-only]} {

# --- Load rating lists ------------------------------------------------
foreach rating {IPS DWZ ECF ICCF} {
	load::load	[format $load::mc::Loading [format $load::mc::RatingList $rating]] \
					ips \
					[file join $scidb::dir::data [string tolower $rating]-ratings.txt] \
					;
}

} ;# if elo-only

# --- Load Wikipedia links ---------------------------------------------
foreach lang {de en} {
	load::load	[format $load::mc::Loading $load::mc::WikipediaLinks] \
					wiki \
					[file join $scidb::dir::data wikipedia-${lang}.txt] \
					;
}

# --- Load chessgames.com links ----------------------------------------
load::load	[format $load::mc::Loading $load::mc::ChessgamesComLinks] \
				cgdc \
				[file join $scidb::dir::data chessgames.com.zip] \
				;

# --- Load cities ------------------------------------------------------
load::load	[format $load::mc::Loading $load::mc::Cities] \
				site \
				[file join $scidb::dir::data cities.txt] \
				;

# --- Load photo index files -------------------------------------------
if {![::process::testOption no-photos]} {

set msg [format $load::mc::Loading $load::mc::PhotoIndex]

foreach basedir [list $::scidb::dir::share $::scidb::dir::user] {
	if {[file isdirectory $basedir]} {
		foreach idx [glob -directory [file join $basedir photos] -nocomplain *.spi] {
			if {[file readable $idx]} {
				if {[file readable [file rootname $idx].spf]} {
					load::source $idx -message $msg
				}
			}
		}
	}
}

proc addPhotoAlias {alias name} {
	set name [string map {. "" " " "" - ""} [string tolower $name]]
	if {[info exists photo_Player($name)]} {
		set alias [string map {. "" " " "" - ""} [string tolower $alias]]
		set photo_Player($alias) $photo_Player($name)
	} elseif {[info exists photo_Engine($name)]} {
		set alias [string map {. "" " " "" - ""} [string tolower $alias]]
		set photo_Engine($alias) $photo_Engine($name)
	}
}
foreach spa [glob -directory [file join $::scidb::dir::share photos] -nocomplain *.spa] {
	if {[file readable $spa]} {
		load::source $spa -message $msg
	}
}

foreach idx [glob -directory [file join $::scidb::dir::user photos] -nocomplain *.spi] {
	if {[file readable $idx]} {
		if {[file readable [file rootname $idx].spf]} {
			load::source $idx -message $msg
		}
	}
}

} ;# if no-photos

} ;# if fast-load

# --- Load localization file -------------------------------------------
set file [file join $scidb::dir::share lang localization.tcl]
if {[file readable $file]} {
	load::source $file -message [format $load::mc::Loading $load::mc::LocalizationFile] -encoding utf-8
}

# --- Load piece sets --------------------------------------------------
foreach file [glob -directory [file join $::scidb::dir::share pieces] -nocomplain *.tcl] {
	load::source $file -message [format $load::mc::Loading $load::mc::PieceSet]
}

set msg [format $load::mc::Loading $load::mc::Theme]

# --- Load themes ------------------------------------------------------
foreach subdir {piece square {}} {
	foreach file [glob -directory [file join $::scidb::dir::user themes {*}$subdir] -nocomplain *.dat] {
		load::source $file -message $msg
	}
}

set file [file join $::scidb::dir::share textures preferences.dat]
if {[file readable $file]} { load::source $file }

# --- Load done --------------------------------------------------------
::scidb::app::load done

# vi:set ts=3 sw=3:
