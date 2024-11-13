# ======================================================================
# Author : $Author$
# Version: $Revision: 193 $
# Date   : $Date: 2012-01-16 09:55:54 +0000 (Mon, 16 Jan 2012) $
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
# Copyright: (C) 2011-2012 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

package require Tk 8.5
package require Ttk

proc scrolledframe {path args} {
	array set opts { -expand none -padding 2 }
	array set opts $args
	set frameOpts {}
	set scrollOpts {}
	foreach key [array names opts] {
		switch -- $key {
			-background - -padding {
			}
			-width - -height - -cursor {
				lappend frameOpts $key $opts($key)
			}
			-class - -style - -borderwidth - -relief {
				lappend frameOpts $key $opts($key)
				array unset opts $key
			}
			-avoidconfigureresize {
				lappend scrollOpts -avoidconfigureresize $opts($key)
				array unset opts $key
			}
		}
	}
	ttk::frame $path {*}$frameOpts
	set f $path.__scrolledframe__
	bind $path <Map> [list after idle { ::scrolledframe::Map %W }]
	if {$opts(-expand) ne "y"} {
		set v $path.__vs__
		scrolledframe::Scrollbar $v -command [list $f yview] -orient vertical
		lappend scrollOpts -yscrollcommand [list ::scrolledframe::MySbSet $v]
		grid $v -row 0 -column 1 -sticky ns
	}
	if {$opts(-expand) ne "x"} {
		set h $path.__hs__
		scrolledframe::Scrollbar $h -command [list $f xview] -orient horizontal
		lappend scrollOpts -xscrollcommand [list ::scrolledframe::MySbSet $h]
		grid $h -row 1 -column 0 -sticky ew
	}
	::scrolledframe::scrolledframe $f {*}$scrollOpts {*}[array get opts]
	grid $f -row 0 -column 0 -sticky nsew
	grid rowconfigure $path 0 -weight 1
	grid columnconfigure $path 0 -weight 1
	rename $f.scrolled ::scrolledframe::_$path
	interp alias {} $f.scrolled {} ::scrolledframe::Dispatch $f
	return $f.scrolled
}

namespace eval scrolledframe {

set (sbset:cmd)		{}
set (sbset:orient)	{}
set (resize:request)	0
set (resize:count)	0


proc sbset {sb first last} {
	variable {}

	set afterScroll [expr {[llength $(sbset:orient)] == 0}]

	if {!$afterScroll && [$sb cget -orient] ne $(sbset:orient)} {
		set (sbset:cmd) [list $sb $first $last]
	} else {
		DoSbSet $sb $first $last
	}
}


# ==============================
#
# scrolledframe
set version 1.0
set (debug,place) 0
#
# a scrolled frame
#
# (C) 2003, ulis
#
# NOL licence (No Obligation Licence)
#
# Changes (C) 2004, KJN
#
# NOL licence (No Obligation Licence)
# ==============================
#
# Hacked package, no documentation, sorry
# See example at bottom
#
# ------------------------------
# v 0.9.1
#	automatic scroll on resize
# ==============================

package provide scrolledframe $version

# --------------
#
# create a scrolled frame
#
# --------------
# parm1: widget name
# parm2: options key/value list
# --------------
proc scrolledframe {w args} {
	variable {}

	array set opts {-avoidconfigureresize no}
	array set opts $args
	# create a scrolled frame
	tk::frame $w
	if {[info exists opts(-background)]} {
		$w configure -background $opts(-background)
	}
	# trap the reference
	rename $w [namespace current]::_$w
	# redirect to dispatch
	interp alias {} $w {} [namespace current]::Dispatch $w
	# create scrollable internal frame
	set frameOpts {}
	if {[info exists opts(-cursor)]} { lappend frameOpts -cursor $opts(-cursor) }
	tk::frame $w.scrolled {*}$frameOpts
	# place it
	place $w.scrolled -in $w -x 0 -y 0
	if {$(debug,place)} { puts "place $w.scrolled -in $w -x 0 -y 0" }
	# init internal data
	set ($w:vheight) 0
	set ($w:vwidth) 0
	set ($w:vtop) 0
	set ($w:vleft) 0
	set ($w:xscroll) ""
	set ($w:yscroll) ""
	set ($w:width) 0
	set ($w:height) 0
	set ($w:fillx) 0
	set ($w:filly) 0
	set ($w:expandx) 0
	set ($w:expandy) 0
	if {!$opts(-avoidconfigureresize)} {
puts "---> !avoidconfigureresize"
		bind $w <Configure> [namespace code [list Resize $w %#]]
		bind $w.scrolled <Configure> [namespace code [list Resize $w %#]]
	}
	array unset opts -avoidconfigureresize
	set args [array get opts]
	if {[llength $args]} [list uplevel 1 [namespace current]::Config $w $args]
	# return widget ref
	return $w
}


proc Map {w} {
	# Due to a bug in the Tk library we have to force window mapping
	[winfo parent $w] configure -width 0

	Resize $w.__scrolledframe__ 0 force
	bind $w <Map> {#}
}


# --------------
#
# dispatch the trapped command
#
# --------------
# parm1: widget name
# parm2: operation
# parm2: operation args
# --------------
proc Dispatch {w cmd args} {
	switch -- $cmd {
		resize		{ Resize $w 0 }
		see			{ See $w [lindex $args 0] }
		configure   { uplevel 1 [linsert $args 0 ::scrolledframe::Config $w] }
		xview       { uplevel 1 [linsert $args 0 ::scrolledframe::Xview $w] }
		yview       { uplevel 1 [linsert $args 0 ::scrolledframe::Yview $w] }
		default     { uplevel 1 [linsert $args 0 ::scrolledframe::_$w $cmd] }
	}
}


# --------------
# configure operation
#
# configure the widget
# --------------
# parm1: widget name
# parm2: options
# --------------
proc Config {w args} {
	variable {}
	set options {}
	set flag 0
	foreach {key value} $args {
		switch -- $key {
			-fill {
				# new fill option: what should the scrolled object do if it is
				# smaller than the viewing window?
				if {$value eq "none"} {
					 set ($w:fillx) 0
					 set ($w:filly) 0
				} elseif {$value eq "x"} {
					 set ($w:fillx) 1
					 set ($w:filly) 0
				} elseif {$value eq "y"} {
					 set ($w:fillx) 0
					 set ($w:filly) 1
				} elseif {$value eq "both"} {
					 set ($w:fillx) 1
					 set ($w:filly) 1
				} else {
					 error "invalid value: should be \"$w configure -fill value\", where \"value\" is \"x\", \"y\", \"none\", or \"both\""
				}
				Resize $w 0 force
				set flag 1
			}
			-xscrollcommand {
				# new xscroll option
				set ($w:xscroll) $value
				set flag 1
			}
			-yscrollcommand {
				# new yscroll option
				set ($w:yscroll) $value
				set flag 1
			}
			-background {
				$w.scrolled configure -background $value
				if {[llength $($w:yscroll)]} { [lindex $($w:yscroll) 1] background $value }
				if {[llength $($w:xscroll)]} { [lindex $($w:xscroll) 1] background $value }
			}
			-expand {
				if {$value eq "none"} {
					 set ($w:expandx) 0
					 set ($w:expandy) 0
				} elseif {$value eq "x"} {
					 set ($w:expandx) 1
					 set ($w:expandy) 0
				} elseif {$value eq "y"} {
					 set ($w:expandx) 0
					 set ($w:expandy) 1
				} else {
					 error "invalid value: should be \"$w configure -expand value\", where \"value\" is \"x\", \"y\", or \"none\""
				}
				Resize $w 0 force
				set flag 1
			}
			-padding {
				if {[llength $($w:yscroll)]} { [lindex $($w:yscroll) 1] padding $value }
				if {[llength $($w:xscroll)]} { [lindex $($w:xscroll) 1] padding $value }
			}
			default { lappend options $key $value }
		}
	}
	# check if needed
	if {!$flag || [llength $options]} {
		# call frame config
		uplevel 1 [linsert $options 0 [namespace current]::_$w configure]
	}
}


# --------------
# Resize proc
#
# Update the scrollbars if necessary, in response to a change in either the viewing window
# or the scrolled object.
# Replaces the old resize and the old vresize
# A <Configure> call may mean any change to the viewing window or the scrolled object.
# We only need to resize the scrollbars if the size of one of these objects has changed.
# Usually the window sizes have not changed, and so the proc will not resize the scrollbars.
# --------------
# parm1: widget name
# parm2: pass anything to force resize even if dimensions are unchanged
# --------------
proc Resize {w req {force {}}} {
	variable {}
	set force [llength $force]

	if {$req > 0 && $req eq $(resize:request)} { return }
	set (resize:request) $req

	set _vheight      $($w:vheight)
	set _vwidth       $($w:vwidth)
	# compute new height & width
	set ($w:vheight)  [winfo reqheight $w.scrolled]
	set ($w:vwidth)	[winfo reqwidth  $w.scrolled]

	# The size may have changed, e.g. by manual resizing of the window
	set _height		   $($w:height)
	set _width			$($w:width)
	set ($w:height)   [winfo height $w] ;# gives the actual height of the viewing window
	set ($w:width)    [winfo width  $w] ;# gives the actual width of the viewing window

	if {$($w:expandx)} {
		$w configure -width $($w:vwidth)
	}
	if {$($w:expandy)} {
		$w configure -height $($w:vheight)
	}

	if {!$($w:expandy) && ($force || $($w:vheight) != $_vheight || $($w:height) != $_height)} {
		# resize the vertical scroll bar
		Yview $w scroll 0 unit
		update idletasks
		# Yset $w
	}

	if {!$($w:expandx) && ($force || $($w:vwidth) != $_vwidth || $($w:width) != $_width)} {
		# resize the horizontal scroll bar
		Xview $w scroll 0 unit
		update idletasks
		# Xset $w
	}
} ;# end proc resize


# --------------
# Xset proc
#
# resize the visible part
# --------------
# parm1: widget name
# --------------
proc Xset {w} {
	variable {}
	# call the xscroll command
	set cmd $($w:xscroll)
	if {[llength $cmd]} { catch { eval $cmd [Xview $w] } }
}


# --------------
# Yset proc
#
# resize the visible part
# --------------
# parm1: widget name
# --------------
proc Yset {w} {
	variable {}
	# call the yscroll command
	set cmd $($w:yscroll)
	if {[llength $cmd]} { catch { eval $cmd [Yview $w] } }
}


# -------------
# Xview
#
# called on horizontal scrolling
# -------------
# parm1: widget path
# parm2: optional moveto or scroll
# parm3: fraction if parm2 == moveto, count unit if parm2 == scroll
# -------------
# return: scrolling info if parm2 is empty
# -------------
proc Xview {w {cmd ""} args} {
	variable {}
	# check args
	set len [llength $args]
	switch -- $cmd {
		""			{ set args {} }
		moveto   { if {$len != 1} { error "wrong # args: should be \"$w xview moveto fraction\"" } }
		scroll   { if {$len != 2} { error "wrong # args: should be \"$w xview scroll count unit\"" } }
		default  { error "unknown operation \"$cmd\": should be empty, moveto or scroll" }
	}
	# save old values:
	set _vleft  $($w:vleft)
	set _vwidth $($w:vwidth)
	set _width  $($w:width)
	# compute new vleft
	set count ""
	switch $len {
		0 {
			# return fractions
			if {$_vwidth == 0} { return {0 1} }
			set first [expr {double($_vleft)/$_vwidth}]
			set last [expr {double($_vleft + $_width)/$_vwidth}]
			if {$last > 1.0} { return {0 1} }
			return [list $first $last]
		}

		1 {
			# absolute movement
			set vleft [expr {int(double($args)*$_vwidth)}]
	 	}

		2 {
			# relative movement
			foreach {count unit} $args break
			if {[string match p* $unit]} { set count [expr {$count*9}] }
			set vleft [expr {$_vleft + $count*0.1*$_width}]
		}
	}
	if {$vleft + $_width > $_vwidth} { set vleft [expr {$_vwidth - $_width}] }
	if {$vleft < 0} { set vleft 0 }
	if {$vleft != $_vleft || $count == 0} {
		set ($w:vleft) $vleft
		Xset $w
		if {$($w:fillx) && ($_vwidth < $_width || [llength $($w:xscroll)] == 0) } {
			# "scrolled object" is not scrolled, because it is too small
			# or because no scrollbar was requested.
			# fillx means that, in these cases, we must tell the object what its width should be
			place $w.scrolled -in $w -x [expr {-$vleft}] -width $_width
			if {$(debug,place)} { puts "place $w.scrolled -in $w -x [expr {-$vleft}] -width $_width" }
		} else {
			place $w.scrolled -in $w -x [expr {-$vleft}] -width {}
			if {$(debug,place)} { puts "place $w.scrolled -in $w -x [expr {-$vleft}] -width {}" }
		}

	}
}


# -------------
# Yview
#
# called on vertical scrolling
# -------------
# parm1: widget path
# parm2: optional moveto or scroll
# parm3: fraction if parm2 == moveto, count unit if parm2 == scroll
# -------------
# return: scrolling info if parm2 is empty
# -------------
proc Yview {w {cmd ""} args} {
	variable {}
	# check args
	set len [llength $args]
	switch -- $cmd {
		""			{set args {}}
		moveto	{ if {$len != 1} { error "wrong # args: should be \"$w yview moveto fraction\"" } }
		scroll	{ if {$len != 2} { error "wrong # args: should be \"$w yview scroll count unit\"" } }
		default	{ error "unknown operation \"$cmd\": should be empty, moveto or scroll" }
	}
	# save old values
	set _vtop $($w:vtop)
	set _vheight $($w:vheight)
#	set _height [winfo height $w]
	set _height $($w:height)
	# compute new vtop
	set count ""
	switch $len {
		0 {
			# return fractions
			if {$_vheight == 0} { return {0 1} }
			set first [expr {double($_vtop)/$_vheight}]
			set last [expr {double($_vtop + $_height)/$_vheight}]
			if {$last > 1.0} { return {0 1} }
			return [list $first $last]
		}

		1 {
			# absolute movement
			set vtop [expr {int(double($args)*$_vheight)}]
		}

		2 {
			# relative movement
			foreach {count unit} $args break
			if {[string match p* $unit]} { set count [expr {$count*9}] }
			set vtop [expr {$_vtop + $count*0.1*$_height}]
		}
	}
	if {$vtop + $_height > $_vheight} { set vtop [expr {$_vheight - $_height}] }
	if {$vtop < 0} { set vtop 0 }
	if {$vtop != $_vtop || $count == 0} {
		set ($w:vtop) $vtop
		Yset $w
		if {$($w:filly) && ($_vheight < $_height || [llength $($w:yscroll)] == 0)} {
			# "scrolled object" is not scrolled, because it is too small
			# or because no scrollbar was requested.
			# filly means that, in these cases, we must tell the object what its height should be
			place $w.scrolled -in $w -y [expr {-$vtop}] -height $_height
			if {$(debug,place)} { puts "place $w.scrolled -in $w -y [expr {-$vtop}] -height $_height" }
		} else {
			place $w.scrolled -in $w -y [expr {-$vtop}] -height {}
			if {$(debug,place)} { puts "place $w.scrolled -in $w -y [expr {-$vtop}] -height {}" }
		}
	}
}


proc See {w child} {
	set hs [winfo parent $w].__hs__
	set vs [winfo parent $w].__vs__

	if {[winfo exists $hs]} {
		lassign [$hs get] first last
		set wv [winfo width $w.scrolled]
		set xc [winfo x $child]
		set x0 [expr {round($first*$wv)}]
		set x1 [expr {round($last*$wv)}]

		if {$xc < $x0} {
			$w xview moveto [expr {double($xc)/double($wv)}]
		} elseif {$xc + [winfo width $child] > $x1} {
			set x [expr {$xc + min(0, [winfo width $child] - [winfo width $w])}]
			$w xview moveto [expr {double($x)/double($wv)}]
		}
	}

	if {[winfo exists $vs]} {
		lassign [$vs get] first last
		set hv [winfo height $w.scrolled]
		set yc [winfo y $child]
		set y0 [expr {round($first*$hv)}]
		set y1 [expr {round($last*$hv)}]

		if {$yc < $y0} {
			$w yview moveto [expr {double($yc)/double($hv)}]
		} elseif {$yc + [winfo height $child] > $y1} {
			set y [expr {$yc + min(0, [winfo height $child] - [winfo height $w])}]
			$w yview moveto [expr {double($y)/double($hv)}]
		}
	}
}


proc Scrollbar {path args} {
	tk::frame $path -borderwidth 0
	ttk::scrollbar $path.sb {*}$args
	if {[$path.sb cget -orient] eq "horizontal"} {
		set dim column
		set sticky we
	} else {
		set dim row
		set sticky ns
	}
	grid $path.sb -row 1 -column 1 -sticky $sticky
	grid ${dim}configure $path 1 -weight 1
	rename $path [namespace current]::_$path
	interp alias {} $path {} [namespace current]::ScrollbarProc $path
}


proc ScrollbarProc {w cmd args} {
	switch $cmd {
		configure - cget {
			if {![string match -orient* [lindex $args 0]]} {
				return [[namespace current]::_$w $cmd {*}$args]
			}
		}

		padding {
			if {[$w.sb cget -orient] eq "horizontal"} { set dim row } else { set dim column }
			grid ${dim}configure $w 0 -minsize [lindex $args 0]
			return
		}

		background {
			return [[namespace current]::_$w configure -background [lindex $args 0]]
		}
	}

	return [$w.sb $cmd {*}$args]
}


proc MySbSet {sb first last} {
	variable {}

	set parent [winfo parent $sb]
	set slaves [grid slaves $parent]
	sbset $sb $first $last
	if {$slaves ne [grid slaves $parent]} {
		set data [list $sb [expr {$sb in [grid slaves $parent]}]]
		event generate $parent.__scrolledframe__.scrolled <<ScrollbarChanged>> -data $data
	}
}


proc SbSet {} {
	variable {}

	set (sbset:orient) {}
	if {[llength $(sbset:cmd)] == 0} { return }
	DoSbSet {*}$(sbset:cmd)
	set (sbset:cmd) {}
}


# Possible solution for flickering:
# 1. call Resize before gridding and avoid the call of DoSbSet during this time
# 2. call DoSbSet after resizing is done
proc DoSbSet {sb first last} {
	variable {}

	set parent [winfo parent $sb]

	if {$first <= 0 && $last >= 1} {
		if {$sb in [grid slaves $parent]} {
			grid remove $sb
		}
	} else {
		if {$sb ne [grid slaves $parent]} {
			grid $sb
		}
	}

	$sb set $first $last
}


bind TScrollbar <ButtonPress-1>	{+ set [namespace current]::(sbset:orient) [%W cget -orient] }
bind TScrollbar <ButtonPress-2>	{+ set [namespace current]::(sbset:orient) [%W cget -orient] }

bind TScrollbar <ButtonRelease-1> +[namespace code SbSet]
bind TScrollbar <ButtonRelease-2> +[namespace code SbSet]

} ;# namesspace scrolledframe

# vi:set ts=3 sw=3:
