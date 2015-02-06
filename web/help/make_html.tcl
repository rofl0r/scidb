#!/bin/sh
#\
exec tclsh "$0" "$@"
# ======================================================================
# Author : $Author$
# Version: $Revision: 1018 $
# Date   : $Date: 2015-02-06 10:42:46 +0000 (Fri, 06 Feb 2015) $
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
# Copyright: (C) 2013 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

package require Tcl 8.5

file mkdir de
file mkdir en

set srcfile [lindex $argv 0]
set dstfile [lindex $argv 1]

set src [open $srcfile r]
set dst [open $dstfile w]
chan configure $src -encoding utf-8
chan configure $dst -encoding utf-8

if {[string match */de/* $srcfile]} {
	set home "../../de"
} else {
	set home "../.."
}

proc enc {s} { return [encoding convertfrom iso8859-1 $s] }

proc backlinks {divid} {
	variable dst
	variable home
	variable srcfile

	if {[string match *de $home]} {
		set backToHome [enc "Zurück zur Homepage"]
		set backToIndex [enc "Zurück zur Hilfe-Übersicht"]
	} else {
		set backToHome "Back to home page"
		set backToIndex "Back to help overview"
	}

  	puts $dst "<div id='$divid' align='center'>"
	if {![string match */Overview.html $srcfile]} {
		puts $dst "<a class='backlink' href='Overview.html'><img src='../../images/home.png' alt='index' align='absmiddle' border='0' /> <b>$backToIndex</b></a>"
		puts $dst "&emsp;"
	}
	puts $dst "<a class='backlink' href='${home}/index.html'><img src='../../images/logo-small.png' alt='home' align='absmiddle' border='0' /> <b>$backToHome</b></a>"
	puts $dst "</div><!-- $divid -->"
}


set expr {\|(::)?([a-zA-Z_]+::)*[a-zA-Z_]+(\([a-zA-Z_:-]*\))?\|[^|]+\|}

while {[gets $src line] >= 0} {
	if {[string match {*<link rel=\"stylesheet\"*} $line]} {
		puts $dst "  <link rel='stylesheet'"
		puts $dst "       type='text/css'"
		puts $dst "        href='http://fonts.googleapis.com/css?family=Abel' />"
		puts $dst "  <link rel='stylesheet'"
	} elseif {[string match "<!-- begin: exclude in web browser -->" $line]} {
		while {[gets $src line] >= 0 && ![string match "<!-- end: exclude in web browser -->" $line]} {
		}
	} elseif {[string match *<body>* $line]} {
		puts $dst $line
		puts $dst "<div id='wrapper_0815'>"
		backlinks header_0815
		puts $dst "<div id='contentarea_0815'>"
		puts $dst "<div class='block_0815'>"
	} elseif {[string match *</body>* $line]} {
		puts $dst "</div><!-- block_0815 -->"
		puts $dst "</div><!-- contentarea_0815 -->"
		backlinks footer_0815
		puts $dst "</div><!-- wrapper_0815 -->"
		puts $dst $line
	} else {
		set start 0
		while {[regexp -indices -start $start $expr $line pos]} {
			lassign $pos n1 n2
			incr n2 -1
			set n1 $n2
			while {[string index $line $n1] ne "|"} { incr n1 -1 }
			incr n1
			lassign $pos k1 k2
			set line [string replace $line $k1 $k2 [string range $line $n1 $n2]]
		}
		puts $dst $line
	}
}

# vi:set ts=3 sw=3 fileencoding=latin1:
