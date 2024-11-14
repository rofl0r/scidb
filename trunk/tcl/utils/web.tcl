# ======================================================================
# Author : $Author$
# Version: $Revision: 819 $
# Date   : $Date: 2013-06-03 22:58:13 +0000 (Mon, 03 Jun 2013) $
# Url    : $URL$
# ======================================================================

# ======================================================================
# Copyright: (C) 2010-2013 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

namespace eval web {

proc open {parent url} {
	variable Escape

	set url [::scidb::misc::url escape $url]
	::widget::busyCursor on

	switch -- [tk windowingsystem] {
		"aqua" {
			catch {exec open $url &}
		}

		"win32" {
			if {$::tcl_platform(os) eq "Windows NT"} {
				catch {exec $::env(COMSPEC) /c start $url &}
			} else {
				catch {exec start $url &}
			}
		}

		"x11" {
			variable DefaultBrowser
			variable Excluded
			variable Options

			if {![info exists DefaultBrowser]} {
				set DefaultBrowser [FindDefaultBrowser]
			}
			while {[llength $DefaultBrowser]} {
				if {[info exists Options($DefaultBrowser)]} {
					set options [string map [list %url% $url] $Options($DefaultBrowser)]
					if {![catch {exec /bin/sh -c "$DefaultBrowser $options"}]} { break }
				}
				if {![catch {exec /bin/sh -c "$DefaultBrowser '$url'" &}]} { break }
				lappend Excluded $DefaultBrowser
				set DefaultBrowser [FindDefaultBrowser]
			}
			if {[llength $DefaultBrowser] == 0} {
				::dialog::error \
					-parent $parent \
					-message $mc::CannotFindBrowser \
					-detail $mc::CannotFindBrowserDetail \
					;
			}
		}
	}

	::widget::busyCursor off
}


if {[tk windowingsystem] eq "x11"} {

namespace eval mc {
	set CannotFindBrowser			"Couldn't find a suitable web browser."
	set CannotFindBrowserDetail	"Set the BROWSER environment variable to your desired browser."
}

variable Browsers {google-chrome iceweasel firefox mozilla opera iexplorer konqueror epiphany galeon mosaic amaya browsex}
variable Options {
	mozilla		{-raise -remote 'openURL(%url%)'}
	iceweasel	{-remote 'openURL(%url%)'}
}
variable Excluded {}


proc FindDefaultBrowser {} {
	global env
	variable Browsers

	if {[info exists env(BROWSER)]} {
		foreach browser [split $env(BROWSER) :] {
			set browser [auto_execok $browser]
			if {![IsExcluded $browser] && [IsX11Browser $browser]} { return $browser }
		}
	}

	set browser [auto_execok x-www-browser]
	if {[llength $browser] && ![IsExcluded $browser]} { return $browser }

	set htmlviewrc [file join $::scidb::dir::home .htmlviewrc]
	if {[file readable $htmlviewrc]} {
		set chan [::open $htmlviewrc]

		while {[gets $chan line] >= 0} {
			if {[string range $line 0 9] eq "X11BROWSER"} {
				set browser [auto_execok [string trim [lindex [split $line =] 1]]]
				if {[llength $browser] && ![IsExcluded $browser]} { return $browser }
			}
		}

		close $chan
	}

	foreach browser $Browsers {
		set browser [auto_execok $browser]
		if {[llength $browser] && ![IsExcluded $browser]} { return $browser }
	}

	return ""
}


proc IsX11Browser {browser} {
	variable Browsers

	if {[llength $browser]} {
		foreach b $Browsers {
			if {[string match [list *$b*] $browser]} { return 1 }
		}
	}

	return 0
}


proc IsExcluded {browser} {
	variable Excluded

	return [expr {[lsearch -exact $Excluded $browser] >= 0}]
}

} ;# [tk windowingsystem] eq "x11"

} ;# namespace web

# vi:set ts=3 sw=3:
