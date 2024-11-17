# ======================================================================
# Author : $Author$
# Version: $Revision: 1527 $
# Date   : $Date: 2018-10-26 12:11:06 +0000 (Fri, 26 Oct 2018) $
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
# Copyright: (C) 2012-2018 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

::util::source drop-down-shadows

namespace eval shadow {

set offset 5
set size 5

array set Used {}
array set Prevent {}


proc addBindings {class {mapShadowWhenMapped yes}} {
	bind $class <Configure>	{+ shadow::prepare %W %x %y %w %h }
	bind $class <Map>			+[list after idle [list shadow::map %W $mapShadowWhenMapped]]
	bind $class <Unmap>		{+ shadow::unmap %W }
	bind $class <Destroy>	{+ shadow::unmap %W }
	bind $class <Destroy>	{+ array unset ::shadow::Geometry %W }
}


proc prevent {w} {
	variable Prevent
	set Prevent($w) 1
}


proc allow {w} {
	variable Prevent
	array unset Prevent $w
}


proc prepare {w x y width height} {
	variable Geometry
	variable Prevent

	if {[info exists Prevent($w)]} { return }

	if {$width > 1 && $height > 1} {
		variable Mapped

		if {[info exists Geometry($w)] && [info exists Mapped($w)]} {
			lassign $Geometry($w) x0 y0
			if {$x != $x0 || $y != $y0} {
				variable offset

				set id $Mapped($w)
				set b .__shadow__b__$id
				set r .__shadow__r__$id
				if {![winfo exists $b]} { return }
				set bx [expr {$x + $offset}]
				set by [expr {$y + $height}]
				set rx [expr {$x + $width}]
				set ry [expr {$y + $offset}]
				wm geometry $b +${bx}+${by}
				wm geometry $r +${rx}+${ry}
			}
		}
		set Geometry($w) [list $x $y $width $height]
	}
}


proc map {w {checkIfGrabbed 0}} {
	variable Geometry
	variable Mapped
	variable offset
	variable size

	if {$checkIfGrabbed} {
		# sometimes the order of map/unmap events is confused,
		# this means that the unmap event comes first, and the
		# corresponding map event will come later. therefore
		# we will not map a shadow if no grab is done.
		if {[string length [grab current]] == 0} { return }
		if {![string match [grab current]* $w]}  { return }
	}

	if {![info exists Geometry($w)]} { return }
	if {[info exists Mapped($w)]} { return }
	set id [Create $w]
	lassign $Geometry($w) x y width height
	set Mapped($w) $id

	set b .__shadow__b__$id
	set r .__shadow__r__$id
	if {![winfo exists $b]} { return }

	set bx [expr {$x + $offset}]
	set by [expr {$y + $height}]
	set rx [expr {$x + $width}]
	set ry [expr {$y + $offset}]

	set img [$b.c itemcget image -image]
	$img configure -width $width
	::scidb::tk::x11 region $bx $by $img
	::scidb::tk::image shadow 1.0 x $img

	set img [$r.c itemcget image -image]
	$img configure -height $height
	::scidb::tk::x11 region $rx $ry $img
	::scidb::tk::image shadow 1.0 y $img

	wm geometry $b ${width}x${size}+${bx}+${by}
	wm geometry $r ${size}x${height}+${rx}+${ry}
	wm deiconify $b
	wm deiconify $r
	raise $b
	raise $r

	update idletasks
}


proc unmap {w} {
	variable Mapped
	variable Used

	if {![info exists Mapped($w)]} { return }
	set id $Mapped($w)
	set Used($id) 0
	array unset Mapped $w

	set b .__shadow__b__$id
	set r .__shadow__r__$id

	if {[winfo exists $b]} {
		after idle [list catch [list wm withdraw $b]]
		after idle [list catch [list wm withdraw $r]]
	}
}


proc kill {} {
	variable Mapped
	variable Used

	foreach w [array names Mapped] {
		unmap $w
	}
}


proc Create {parent} {
	variable Used
	variable offset

	set id -1

	foreach key [array names Used] {
		if {!$Used($key)} {
			set id $key
			break
		}
	}

	if {$id == -1} {
		set id [llength [array names Used]]
	}

	set Used($id) 1

	set b .__shadow__b__$id
	set r .__shadow__r__$id

	if {![winfo exists $b]} {
		foreach w [list $b $r] {
			toplevel $w -background black -borderwidth 0 -relief flat -takefocus 0
			wm attributes $w -topmost yes
			wm focusmodel $w passive
			catch { wm attributes $w -type tooltip }
			wm withdraw $w
			# TODO: For any reason setting overrideredirect is causing exposure problems when
			# umapping the shadows. But w/o overrideredirect this window will take the focus.
			wm overrideredirect $w yes
			wm transient $w $parent ;# TODO required?
			pack [tk::canvas $w.c -borderwidth 0 -takefocus 0] -fill both -expand yes
			$w.c xview moveto 0
			$w.c yview moveto 0
		}

		$b.c create image 0 0 -anchor nw -image [image create photo -width 0 -height $offset] -tag image
		$r.c create image 0 0 -anchor nw -image [image create photo -width $offset -height 0] -tag image
	}

	return $id
}

} ;# namespace shadow

rename grab __grab__shadow

proc grab {args} {
	# sometimes Tk is "hanging" and the unmap
	# event will come too late (at end of session).
	# therefore we will kill all shadows as soon as
	# a grab is released.
	if {[lindex $args 0] eq "release"} { shadow::kill }
	if {[llength $args] == 2 && ![winfo exists [lindex $args 1]]} { return }

	return [__grab__shadow {*}$args]
}

### M E N U #####################################################################################

bind Menu <Configure> {+
	if {![string match *#menu %W]} {
		shadow::prepare %W %x %y %w %h
	}
}

bind Menu <Map> {+
	if {![string match *#menu %W]} {
		# a slight delay is reducing the flickering (25ms)
		# but this may confuse the order of map/unmap events.
		after idle { shadow::map %W yes }
	}
}

bind Menu <Unmap> {+
	if {![string match *#menu %W]} {
		# IMPORTANT NOTE:
		# The unmapping should be called before the menu is unposting
		# to avoid glitches. This will be done via the virtual event
		# <<MenuWillUnpost>>. This call is for safety only.
		shadow::unmap %W
	}
}

bind Menu <Destroy> {+
	if {![string match *#menu %W]} {
		shadow::unmap %W
		array unset ::shadow::Geometry %W
	}
}

bind Menu <<MenuWillUnpost>> {+
	# Unmap the drop-shadow before unposting the menu to avoid glitches.
	if {![string match *#menu %W]} {
		shadow::unmap %W
	}
}

# vi:set ts=3 sw=3:
