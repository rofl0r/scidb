# ======================================================================
# Author : $Author$
# Version: $Revision: 1295 $
# Date   : $Date: 2017-07-24 19:35:37 +0000 (Mon, 24 Jul 2017) $
# Url    : $URL$
# ======================================================================

# ======================================================================
# Copyright: (C) 2010-2017 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

# ======================================================================
# Providing a modified paned window:
#	- cursor handling overworked (the original implementation is clumsy)
#	- additional option -sashcmd
#	- additional pane option "-maxsize"
#	- Button-2 is unbound
#	- option -gridsize added; in this case option -minsize is declaring
#    the base size
#
# Issues:
#	- option "-opaqueresize" will be ignored (always on)
#	- "-cursor" option should not be changed with "configure"
#	- the cursor of the panes should not be set to "" after
#	  they are added to the paned window
#
# Why not using ttk::panedwindow?
#	- the design is quite halfhearted, for example no -minsize option, so
#    this widget is somewhat useless
# ======================================================================

package provide panedwindow 1.0

rename tk::panedwindow tk::panedwindow_old


proc panedwindow {args} {
	if {[llength $args] == 0} {
		return error -code "wrong # args: should be \"panedwindow pathName ?options?\""
	}

	if {[winfo exists [lindex $args 0]]} {
		return error -code "window name \"[lindex $args 0]\" already exists"
	}

	return [panedwindow::Build {*}$args]
}


namespace eval tk {

proc panedwindow {args} { return [::panedwindow {*}$args] }

} ;# namespace tk


namespace eval panedwindow {

bind PanedWindowFrame <Destroy> [namespace code { DestroyHandler %W }]


proc Build {w args} {
	variable Initialized

	array set opts {
		-borderwidth	0
		-cursor			{}
		-sashcmd			{}
		-state			"normal"
	}

	array set opts $args
	set sashcmd $opts(-sashcmd)
	unset -nocomplain opts(-opaqueresize)
	unset -nocomplain opts(-sashcmd)
	set state $opts(-state)
	array unset opts -state

	tk::panedwindow_old $w {*}[array get opts] -opaqueresize 1
	if {$state ne "disabled"} {
		set cursor sb_[string index [$w cget -orient] 0]_double_arrow
		if {[$w cget -cursor] ne $cursor} {
			$w configure -cursor $cursor
		}
	}
	
	namespace eval [namespace current]::$w {}
	variable [namespace current]::${w}::MaxSize
	variable [namespace current]::${w}::SashCmd $sashcmd
	variable [namespace current]::${w}::Cursor left_ptr
	variable [namespace current]::${w}::State $state

	rename ::$w $w.__panedwindow__
	proc ::$w {command args} "[namespace current]::WidgetProc $w \$command {*}\$args"

	return $w
}


proc WidgetProc {w command args} {
	if {$command eq "add" || $command eq "paneconfigure"} {
		if {[llength $args] % 2 == 0} {
			error "value for \"[lindex $args end]\" missing"
		}
		set child [lindex $args 0]
		array set opts [lrange $args 1 end]

		variable [namespace current]::${w}::MaxSize
		variable [namespace current]::${w}::GridSize

		if {![info exists MaxSize($child)]} {
			set MaxSize($child) 32000
		}
		if {![info exists GridSize($child)]} {
			set GridSize($child) 0
		}
		
		if {[info exists opts(-maxsize)]} {
			if {[string is integer -strict $opts(-maxsize)]} {
				set maxSize $opts(-maxsize)
				if {$maxSize <= 0} { set maxSize 32000 }
				set MaxSize($child) $maxSize
			} else {
				error "bad screen distance \"$opts(-maxsize)\""
			}

			unset opts(-maxsize)
		}
		if {[info exists opts(-gridsize)]} {
			if {[string is integer -strict $opts(-gridsize)]} {
				set gridsize $opts(-gridsize)
				if {$gridsize <= 0} { set gridsize 0 }
				set GridSize($child) $gridsize
			} else {
				error "bad screen distance \"$opts(-gridsize)\""
			}

			unset opts(-gridsize)
		}

		set args $child
		lappend args {*}[array get opts]
	}

	switch -- $command {
		add {
			variable ${w}::Cursor
			variable ${w}::Parent
			variable ${w}::State

			set child [lindex $args 0]
			
			if {	$State ne "disabled"
				&& [llength [$child cget -cursor]] == 0
				&& [$child cget -cursor] ne $Cursor} {
				$child configure -cursor $Cursor
			}
		}

		configure {
			if {[llength $args] % 2 == 1} {
				error "value for \"[lindex $args end]\" missing"
			}
			array set opts $args
			foreach {key val} $args {
				switch -- $key {
					-cursor {
						variable ${w}::Cursor
						set Cursor $val
						if {[llength $Cursor] == 0} { set Cursor left_ptr }
						unset opts($key)
					}

					-orient {
						set cursor sb_[string index $val 0]_double_arrow
						set child $w.__panedwindow__
						if {[$child cget -cursor] ne $cursor} {
							$child configure -cursor $cursor
						}
					}

					-opaqueresize {
						unset opts($key)
					}

					-sashcmd {
						variable ${w}::SashCmd
						set SashCmd $val
						unset opts($key)
					}
				}
			}
			set args [array get opts]
		}

		cget {
			switch -- [lindex $args 0] {
				-cursor {
					variable ${w}::Cursor
					return $Cursor
				}
			}
		}
	}

	return [$w.__panedwindow__ $command {*}$args]
}


proc DestroyHandler {w} {
	if {[winfo class $w] eq "PanedWindowFrame"} {
		namespace delete [namespace current]::$w
		rename $w {}
	}
}

} ;# namespace panedwindow


bind Panedwindow <ButtonPress-1>		{ tk::panedwindow::MarkSash %W %x %y }
bind Panedwindow <B1-Motion>			{ tk::panedwindow::DragSash %W %x %y }
bind Panedwindow <ButtonRelease-1>	{ tk::panedwindow::ReleaseSash %W }

bind Panedwindow <ButtonPress-2>		{ break }
bind Panedwindow <B2-Motion>			{ break }
bind Panedwindow <ButtonRelease-2>	{ break }
bind Panedwindow <Leave>				{ break }


namespace eval tk {
namespace eval panedwindow {

proc MarkSash {w x y} {
	variable ::tk::Priv
	variable ::panedwindow::${w}::State

	if {$State eq "disabled"} { return }

	set what [$w identify $x $y]
	if {[llength $what] != 2} { return }
	lassign $what index which
	if {$::tk_strictMotif && $which ne "handle"} { return }

	variable ::panedwindow::${w}::MaxSize
	variable ::panedwindow::${w}::GridSize
	variable ::panedwindow::${w}::SashCmd
	variable ::panedwindow::${w}::Cursor

	set panes [$w panes]
	lassign {0 0 0 0} lhsMax rhsMax lhsMin rhsMin
	if {[string match h* [$w cget -orient]]} { set which width } else { set which height }
	set npanes [llength $panes]

	for {set i 0} {$i < $npanes} {incr i} {
		set pane [lindex $panes $i]
		set maxSize($pane) $MaxSize($pane)

		if {$GridSize($pane) > 1} {
			set f [expr {($maxSize($pane) - [$w panecget $pane -minsize])/$GridSize($pane)}]
			set maxSize($pane) [expr {$f*$GridSize($pane) + [$w panecget $pane -minsize]}]
		}
	}

	for {set i 0} {$i <= $index} {incr i} {
		set pane [lindex $panes $i]
		set Priv(panesize:$pane) [expr {max($maxSize($pane), [winfo $which $pane])}]
		set lhsMax [expr {$lhsMax + $Priv(panesize:$pane) - [winfo $which $pane]}]
		set lhsMin [expr {$lhsMin + max(1, [$w panecget $pane -minsize])}]
	}

	for {set i [expr {$index + 1}]} {$i < $npanes} {incr i} {
		set pane [lindex $panes $i]
		set Priv(panesize:$pane) [expr {max($maxSize($pane), [winfo $which $pane])}]
		set rhsMax [expr {$rhsMax + $Priv(panesize:$pane) - [winfo $which $pane]}]
		set rhsMin [expr {$rhsMin + max(1, [$w panecget $pane -minsize])}]
	}

	set sashwidth [$w cget -sashwidth]
	incr lhsMax [expr {$index*$sashwidth}]
	incr lhsMin [expr {$index*$sashwidth}]
	incr rhsMax [expr {($npanes - $index - 1)*$sashwidth}]
	incr rhsMin [expr {($npanes - $index - 1)*$sashwidth}]

	set Priv(sash) $index
	lassign [$w sash coord $index] sx sy
	set Priv(dx) [expr {$sx - $x}]
	set Priv(dy) [expr {$sy - $y}]

	if {[string match h* [$w cget -orient]]} {
		set Priv(min) [expr {max($lhsMin, $sx - $rhsMax)}]
		set Priv(max) [expr {min([winfo width $w] - $rhsMin, $sx + $lhsMax)}]
	} else {
		set Priv(min) [expr {max($lhsMin, $sy - $rhsMax)}]
		set Priv(max) [expr {min([winfo height $w] - $rhsMin, $sy + $lhsMax)}]
	}

	set Priv(min) [expr {$Priv(min) + $sashwidth}]
	if {$Priv(min) >= $Priv(max)} { return }

	if {[llength $SashCmd]} {
		lassign [eval $SashCmd $w mark $sx $sy] sx sy
	}
	for {set i 0} {$i < $npanes - 1} {incr i} {
		$w sash mark $i $sx $sy
	}

	set Priv(cursorList) {}
	SetCursor [$w panes] sb_[string index [$w cget -orient] 0]_double_arrow
}


proc DragSash {w x y} {
	variable ::tk::Priv
	variable ::panedwindow::${w}::State

	if {$State eq "disabled"} { return }

	if {[info exists Priv(sash)]} {
		incr x $Priv(dx)
		incr y $Priv(dy)

		set x [expr {min($Priv(max), max($Priv(min), $x))}]
		set y [expr {min($Priv(max), max($Priv(min), $y))}]

		MoveSash $w $Priv(sash) $x $y 0
	}
}


proc ReleaseSash {w} {
	variable ::tk::Priv

	if {[info exists Priv(sash)]} {
		ResetCursor
		array unset Priv panesize:*
		unset -nocomplain Priv(sash) Priv(dx) Priv(dy) Priv(min) Priv(max) Priv(cursorList)
	}
}


proc MoveSash {w index x y offs} {
	variable ::panedwindow::${w}::SashCmd
	variable ::panedwindow::${w}::GridSize
	variable ::tk::Priv

	set panes [$w panes]
	set cx [expr {$x - $offs}]
	set cy [expr {$y - $offs}]

	set pane [lindex $panes $index]
	set gridsize $GridSize($pane)

	if {$gridsize > 1} {
		lassign [$w sash coord $index] x1 y1

		if {[string match h* [$w cget -orient]]} {
			set fx [expr {($cx - $x1 - $gridsize/2)/$gridsize}]
			set cx [expr {$x1 + $fx*$gridsize}]
		} else {
			set fy [expr {($cy - $y1 + $gridsize/2)/$gridsize}]
			set cy [expr {$y1 + $fy*$gridsize}]
		}
	}

	set sashSize [$w cget -sashwidth]
	if {[llength $SashCmd]} {
		lassign [eval $SashCmd $w drag $cx $cy] cx cy
	}

	$w sash place $index $cx $cy

	if {[string index [$w cget -orient] 0] == "h"} {
		if {$index > 0} {
			lassign [$w sash coord [expr {$index - 1}]] sx sy
			incr sx $sashSize
			set maxSize $Priv(panesize:$pane)

			if {$cx - $sx > $maxSize} {
				MoveSash $w [expr {$index - 1}] $x $y [expr {$offs + $maxSize + $sashSize}]
			}
		}

		if {$index < [llength $panes] - 2} {
			set pane [lindex $panes [expr {$index + 1}]]
			lassign [$w sash coord [expr {$index + 1}]] sx sy
			incr sx -$sashSize
			set maxSize $Priv(panesize:$pane)

			if {$sx - $cx > $maxSize} {
				MoveSash $w [expr {$index + 1}] $x $y [expr {$offs - $maxSize - $sashSize}]
			}
		}
	} else {
		if {$index > 0} {
			lassign [$w sash coord [expr {$index - 1}]] sx sy
			incr sy $sashSize
			set maxSize $Priv(panesize:$pane)

			if {$cy - $sy > $maxSize} {
				MoveSash $w [expr {$index - 1}] $x $y [expr {$offs + $maxSize + $sashSize}]
			}
		}

		if {$index < [llength $panes] - 2} {
			set pane [lindex $panes [expr {$index + 1}]]
			lassign [$w sash coord [expr {$index + 1}]] sx sy
			incr sy -$sashSize
			set maxSize $Priv(panesize:$pane)

			if {$sy - $cy > $maxSize} {
				MoveSash $w [expr {$index + 1}] $x $y [expr {$offs - $maxSize - $sashSize}]
			}
		}
	}
}


proc SetCursor {childs cursor} {
	variable ::tk::Priv

	foreach child $childs {
		catch {
			set cur [$child cget -cursor]
			if {$cur eq "sb_h_double_arrow" || $cur eq "sb_v_double_arrow"} { set cur "" }
			if {[$child cget -cursor] ne $cursor]} {
				$child configure -cursor $cursor
			}
			lappend Priv(cursorList) $child $cur
		}
		SetCursor [winfo children $child] $cursor
	}
}


proc ResetCursor {} {
	variable ::tk::Priv

	if {![info exists Priv(cursorList)]} { return }

	foreach {child cursor} $Priv(cursorList) {
		if {[winfo exists $child]} {
			catch {
				if {[$child cget -cursor] ne $cursor]} {
					$child configure -cursor $cursor
				}
			}
		}
	}
}

} ;# namespace panedwindow
} ;# namespace tk

# vi:set ts=3 sw=3:
