#!/bin/sh
#\
exec tclsh "$0" "$@"
# ======================================================================
# Author : $Author$
# Version: $Revision: 179 $
# Date   : $Date: 2012-01-08 22:42:46 +0000 (Sun, 08 Jan 2012) $
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

package require Tcl 8.5

# utf-8 encoded (you may use gedit for editing)
array set Title {
	de "Übersicht"
	en "Overview"
}

if {$argc != 1} {
	puts stderr "Usage: [info nameofexecutable] <contents-file>"
	exit 1
}

set contentsFile [lindex $argv 0]
if {![file readable $contentsFile]} {
	puts stderr "Error: '$contentsFile' is not readable"
	exit 1
}

set lang [file tail [pwd]] 

if {![info exists Title($lang)]} {
	puts stderr "Error: missing entry in Title for language '$lang'"
	exit 1
}

set file [file join .. .. lang localization.tcl]
source $file

foreach entry $i18n::languages {
	lassign $entry _ codeName charset _
	if {$codeName eq $lang} { break }
}

if {$codeName ne $lang} {
	puts stderr "Language \"$lang\" not defined in file \"$file\"."
	puts stderr "You have to edit \"$file\"."
	exit 1
}

set file "[file rootname $contentsFile].html"
set src [open $contentsFile r]
chan configure $src -encoding $charset
set close 0

puts "set Contents \{"
puts "  \{"
puts "     {{{$Title($lang)} {$file}}}"

while {[gets $src line] >= 0} {
	if {[regexp {<h[1-6] name=\"(.*)\">(.*)</h[1-6]>} $line _ href section]} {
		puts "  \}"
		puts "  \{"
		puts "    {{{$section} {$file} {$href}}}"
	} elseif {[regexp {<a href=\"(.*)\">(.*)</a>} $line _ href title]} {
		puts "    {{{$title} {$href}}}"
	}
}

puts "  \}"
puts "\}"
close $src

# vi:set ts=3 sw=3:
