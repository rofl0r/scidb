# ======================================================================
# Author : $Author$
# Version: $Revision: 216 $
# Date   : $Date: 2012-01-29 19:02:12 +0000 (Sun, 29 Jan 2012) $
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
# Copyright: (C) 2012 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

namespace eval shadow {

array set Used {}


proc prepare {w x y width height} {
	variable Geometry

	if {$width > 1 && $height > 1} {
		set Geometry($w) [list $x $y $width $height]
	}
}


proc map {w} {
	variable Geometry
	variable Mapped

	# kill dangling shadows (should not happen)
	set cls [winfo class $w]
	foreach v [array names Mapped] {
		if {	$cls ne "Menu"
			|| [winfo class $v] ne "Menu"
			|| ![winfo ismapped $v]
			|| ![string match $v* $w]} {

			set id $Mapped($v)
			array unset Mapped $v

			set b .__shadow__b__$id
			set r .__shadow__r__$id

			if {[winfo exists $b]} {
				wm withdraw $b
				wm withdraw $r
			}
		}
	}

	if {![info exists Geometry($w)]} { return }
	set id [Create]
	lassign $Geometry($w) x y width height
	set Mapped($w) $id

	set b .__shadow__b__$id
	set r .__shadow__r__$id
	if {![winfo exists $b]} { return }

	set bx [expr {$x + 5}]
	set by [expr {$y + $height}]
	set rx [expr {$x + $width}]
	set ry [expr {$y + 5}]

	set img [$b.c itemcget image -image]
	$img configure -width $width
	::scidb::tk::x11 region $bx $by $img
	::scidb::tk::image shadow 1.0 x $img

	set img [$r.c itemcget image -image]
	$img configure -height $height
	::scidb::tk::x11 region $rx $ry $img
	::scidb::tk::image shadow 1.0 y $img

	wm geometry $b ${width}x5+${bx}+${by}
	wm geometry $r 5x${height}+${rx}+${ry}
	wm deiconify $b
	wm deiconify $r
	raise $b
	raise $r

	update idletasks
}


proc unmap {w} {
	variable Geometry
	variable Mapped
	variable Used

	if {![info exists Mapped($w)]} { return }
	set id $Mapped($w)
	set Used($id) 0
	array unset Mapped $w

	set b .__shadow__b__$id
	set r .__shadow__r__$id

	if {[winfo exists $b]} {
		wm withdraw $b
		wm withdraw $r
	}

	update idletasks
}


proc Create {} {
	variable Used

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
			wm withdraw $w
			wm overrideredirect $w 1
			pack [tk::canvas $w.c -borderwidth 0] -fill both -expand yes
			$w.c xview moveto 0
			$w.c yview moveto 0
		}

		$b.c create image 0 0 -anchor nw -image [image create photo -width 0 -height 5] -tag image
		$r.c create image 0 0 -anchor nw -image [image create photo -width 5 -height 0] -tag image
	}

	return $id
}

} ;# namespace shadow

### M E N U #####################################################################################

bind Menu <Configure> {+
	if {![string match *#menu %W]} {
		shadow::prepare %W %x %y %w %h
	}
}

bind Menu <Map> {+
	if {![string match *#menu %W]} {
		after idle { shadow::map %W }
	}
}

bind Menu <Unmap> {+
	variable ::shadow::Geometry

	if {![string match *#menu %W]} {
		shadow::unmap %W
		array unset Geometry %W
	}
}

bind Menu <Destroy> {+
	variable ::shadow::Geometry

	if {![string match *#menu %W]} {
		shadow::unmap %W
		array unset Geometry %W
	}
}

###  C O M B O B O X  P O P D O W N #############################################################

bind ComboboxPopdown <Configure>		{+ shadow::prepare %W %x %y %w %h }
bind ComboboxPopdown <Map>				{+ after idle { shadow::map %W } }
bind ComboboxPopdown <Unmap>			{+ shadow::unmap %W }
bind ComboboxPopdown <Destroy>		{+ shadow::unmap %W }
bind ComboboxPopdown <Destroy>		{+ array unset ::shadow::Geometry %W }

###  A D D  L A N G U A G E  P O P D O W N ######################################################

bind AddLanguagePopdown <Configure>	{+ shadow::prepare %W %x %y %w %h }
bind AddLanguagePopdown <Map>			{+ after idle { shadow::map %W } }
bind AddLanguagePopdown <Destroy>	{+ shadow::unmap %W }
bind AddLanguagePopdown <Destroy>	{+ array unset ::shadow::Geometry %W }

###  T O O L T I P  P O P U P ###################################################################

bind TooltipPopup <Configure>			{+ shadow::prepare %W %x %y %w %h }
bind TooltipPopup <Map>					{+ after idle { shadow::map %W } }
bind TooltipPopup <Unmap>				{+ shadow::unmap %W }
bind TooltipPopup <Destroy>			{+ shadow::unmap %W }
bind TooltipPopup <Destroy>			{+ array unset ::shadow::Geometry %W }

# vi:set ts=3 sw=3:
