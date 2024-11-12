# ======================================================================
# Author : $Author$
# Version: $Revision: 94 $
# Date   : $Date: 2011-08-21 16:47:29 +0000 (Sun, 21 Aug 2011) $
# Url    : $URL$
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

package require Tk 8.4
if {[catch { package require tkpng }]} { package require Img }
package provide choosedir 1.0


proc choosedir {w args} {
	return [::choosedir::Build $w {*}$args]
}


namespace eval choosedir {
namespace eval mc {

set FileSystem			"File System"
set ShowPredecessor	"Show Predecessor"
set ShowTail			"Show Tail"
set Folder				"Folder"

}

namespace import ::tcl::mathfunc::max

proc tooltip {args} {}
proc mc {tok} { return [tk::msgcat::mc [set $tok]] }

if {![catch {package require tooltip}]} {
	proc tooltip {args} { ::tooltip::tooltip {*}$args }
}

proc Tr {tok} { return [mc [namespace current]::mc::$tok] }


proc Build {w args} {
	namespace eval [namespace current]::${w} {}
	variable ${w}::Vars

	array set opts {
		-width				300
		-borderwidth		1
		-padx					2
		-pady					5
		-relief				raised
		-font					TkTextFont
		-activebackground	#d9d9d9
		-initialdir			{}
		-showlabel			0
	}
	array set opts $args

	set Vars(dir) ""
	set Vars(components) { "" }
	set Vars(size) 0
	set Vars(start) 1
	set Vars(font) $opts(-font)
	set Vars(padx) $opts(-padx)
	set Vars(pady) $opts(-pady)
	set Vars(initialdir) $opts(-initialdir)

	set Vars(linespace) [font metrics $opts(-font) -linespace]
	set Vars(activebackground) $opts(-activebackground)
	set Vars(showlabel) $opts(-showlabel)

	array unset opts -font
	array unset opts -pad*
	array unset opts -activebackground
	array unset opts -initialdir
	array unset opts -showlabel

	set opts(-height) [expr {$Vars(linespace) + 2*$Vars(pady)}]

	tk::frame $w -takefocus 0 {*}[array get opts]

	if {$Vars(showlabel)} {
		tk::label $w.label -text "[Tr Folder]:"
	}

	tk::button $w.prev                           \
		-image $icon::14x14::PrevComponent        \
		-relief flat                              \
		-overrelief raised                        \
		-activebackground $Vars(activebackground) \
		-takefocus 0                              \
		;
	tk::button $w.next                           \
		-image $icon::14x14::NextComponent        \
		-relief flat                              \
		-overrelief raised                        \
		-activebackground $Vars(activebackground) \
		-takefocus 0                              \
		;
	tk::button $w.image--1                                          \
		-image $icon::9x14::Delimiter                                \
		-relief flat                                                 \
		-pady 2                                                      \
		-overrelief raised                                           \
		-activebackground $Vars(activebackground)                    \
		-command [namespace code [list PopupDirs $w $w.image--1 -1]] \
		-takefocus 0                                                 \
		;
	tooltip $w.next [Tr ShowTail]
	if {$Vars(showlabel)} {
		grid $w.label -column 1  -row 1
		grid columnconfigure $w {0 2} -minsize $Vars(padx)
	}
	grid $w.prev -column 3 -row 1
	grid $w.image--1 -column 5 -row 1
	grid $w.next -column 1000 -row 1
	grid rowconfigure $w {0 2} -minsize $Vars(pady)
	grid columnconfigure $w {2 4 1001} -minsize $Vars(padx)

	bind $w <Destroy> [list catch [list namespace delete [namespace current]::${w}]]
	bind $w <Configure> [namespace code { Configure %W %w }]
	catch { rename ::$w $w.__choosedir__ }
	proc ::$w {command args} "[namespace current]::WidgetProc $w \$command {*}\$args"

	if {[string length $Vars(initialdir)] == 0} { set Vars(initialdir) [pwd] }
	$w set $Vars(initialdir)

	return $w
}


proc WidgetProc {w command args} {
	switch -- $command {
		set {
			if {[llength $args] != 1} {
				error "wrong # args: should be \"[namespace current] set <dir>\""
			}
			variable ${w}::Vars

			if {[lindex $args 0] ne $Vars(dir)} {
				set Vars(dir) [lindex $args 0]
				set Vars(components) [file split $Vars(dir)]
				if {[llength $Vars(components)] <= 1} {
					set Vars(components) { "" }
				} else {
					set Vars(components) [lreplace $Vars(components) 0 0]
				}
				set Vars(start) [llength $Vars(components)]
				Layout $w
			}

			return $w
		}

		dir {
			variable ${w}::Vars
			return $Vars(dir)
		}

		configure {
			if {[llength $args] % 2 == 1} {
				error "value for \"[lindex $args end]\" missing"
			}
			array set opts $args
			foreach option {padx pady} {
				if {[info exists opts(-$option)]} {
					set Vars($option) $opts(-$option)
				}
			}
			if {[info exists opts(-font)]} {
				set value $opts(-font)
				for {set i 0} {$i < $Vars(size)} {incr i} {
					$w.text-$i configure -font $value
				}
			}
			if {[info exists opts(-activebackground)]} {
				set value $opts(-activebackground)
				for {set i 0} {$i < $Vars(size)} {incr i} {
					$w.text-$i configure -activebackground $value
					$w.image-$i configure -activebackground $value
				}
			}
			set args [array get opts]
			if {[llength $args]} {
				$w.__choosedir__ {*}$args
			}
			Layout $w
			return $w
		}
	}

	return [$w.__choosedir__ {*}$args]
}


proc Layout {w} {
	variable ${w}::Vars

	set n [llength $Vars(components)]

	for {set i $Vars(size)} {$i < $n} {incr i} {
		tk::button $w.text-$i                                       \
			-font $Vars(font)                                        \
			-padx 2                                                  \
			-pady 2                                                  \
			-relief flat                                             \
			-overrelief raised                                       \
			-activebackground $Vars(activebackground)                \
			-command [namespace code [list Invoke $w $w.text-$i $i]] \
			-takefocus 0                                             \
			;
		tk::button $w.image-$i                                          \
			-image $icon::9x14::Delimiter                                \
			-relief flat                                                 \
			-pady 2                                                      \
			-overrelief raised                                           \
			-activebackground $Vars(activebackground)                    \
			-command [namespace code [list PopupDirs $w $w.image-$i $i]] \
			-takefocus 0                                                 \
			;
		# we have to help Tk a little bit
		bind $w.text-$i <Leave> { %W configure -relief flat }
		bind $w.image-$i <Leave> { %W configure -relief flat }
		set col [expr {4*($i + 2)}]
		grid $w.text-$i -column [incr col] -row 1
		grid $w.image-$i -column [incr col] -row 1
	}

	for {set i 0} {$i < $n} {incr i} {
		$w.text-$i configure -text [lindex $Vars(components) $i]
	}

	set Vars(size) [max $n $Vars(size)]
	set removeLast 0

	if {[llength [glob -nocomplain -tails -dir $Vars(dir) -types d *]] == 0} {
		set removeLast 1
	}

	update idletasks

	set iwd [winfo width $w.image-0]
	set bwd [$w.__choosedir__ cget -borderwidth]
	set twd [expr {[winfo width $w] - 2*$bwd - [winfo width $w.prev] - 3*$Vars(padx)}]
	if {!$removeLast} { set twd [expr {$twd - $iwd}] }
	if {$Vars(showlabel)} { set twd [expr {$twd - [winfo width $w.label] - $Vars(padx)}] }

	set i 0
	set x 0
	set width(0) 0

	while {$i < $n} {
		set wd [winfo width $w.text-$i]
		set x [expr {$x + $wd + $iwd}]
		set width([incr i]) $x
	}

	set e $n
	set f $e
	while {$f > 0 && $width($e) - $width([expr {$f - 1}]) <= $twd} { incr f -1 }

	while {$f > $Vars(start)} {
		incr f -1
		while {$e > $f && $width($e) - $width($f) > $twd} { incr e -1 }
	}

	if {$e < $n} {
		set twd [expr {$twd - [winfo width $w.next]}]
		if {$e > 0 && $width($e) - $width($f) > $twd} { incr e -1 }
	}

	if {$f > 0} {
		if {$width($e) - $width($f) > $twd} { incr f 1 }
		bind $w.next <ButtonRelease-1> [namespace code [list SetRange $w $w.next $n]]
	}
	if {$f == 0} {
		set img $icon::14x14::FileSystem
		$w.prev configure -command [namespace code [list Invoke $w $w.prev -1]]
		tooltip $w.prev [Tr FileSystem]
	} else {
		set img $icon::14x14::PrevComponent
		$w.prev configure -command [namespace code [list SetRange $w $w.prev [expr {$f - 1}]]]
		tooltip $w.prev [Tr ShowPredecessor]
	}
	$w.prev configure -image $img
	if {$f >= $e} { set f [expr {$e - 1}] }

	for {set i 0} {$i < $Vars(size)} {incr i} {
		if {$i >= $f && $i < $e} {
			grid $w.text-$i
			grid $w.image-$i
		} else {
			grid remove $w.text-$i
			grid remove $w.image-$i
		}
	}

	grid $w.image--1
	if {$removeLast} { grid remove $w.image-[expr {$n - 1}] }
	if {$e < $n} { grid $w.next } else { grid remove $w.next }
}


proc ButtonEnter {w} {
	# poor Tk: we have to help Tk a bit
	set ptrw [winfo containing {*}[winfo pointerxy .]]
	if {$ptrw eq $w} {
		$w configure -relief [$w cget -overrelief]
		::tk::ButtonEnter $w
	}
}


proc Invoke {w btn i} {
	variable ${w}::Vars

	set Vars(components) [lrange $Vars(components) 0 $i]
	set Vars(dir) [file join "/" {*}$Vars(components)]
	event generate $w <<SetDirectory>> -data $Vars(dir)
	set Vars(start) [llength $Vars(components)]
	Layout $w
	after idle [namespace code [list ButtonEnter $btn]]
}


proc PopupDirs {w btn i} {
	variable ${w}::Vars

	set rootdir [file join "/" {*}[lrange $Vars(components) 0 $i]]
	set subdirs [glob -nocomplain -tails -dir $rootdir -types d *]
	set subdirs [lsort -dictionary -unique $subdirs]

	set m $w.popup
	if {[winfo exists $m]} { destroy $m }
	menu $m -tearoff false

	if {[llength $subdirs] == 0} {
	} else {
		foreach dir $subdirs {
			$m add command -label $dir -command [namespace code [list ChangeDir $w $rootdir $dir]]
		}
	}

	bind $m <<MenuUnpost>> [namespace code [list ButtonEnter $btn]]
	tk_popup $m [winfo rootx $btn] [expr {[winfo rooty $btn] + [winfo height $btn]}]
}


proc ChangeDir {w rootdir subdir} {
	variable ${w}::Vars

	set Vars(dir) [file join $rootdir $subdir]
	event generate $w <<SetDirectory>> -data $Vars(dir)
	set Vars(components) [lrange [file split $Vars(dir)] 1 end]
	set Vars(start) [llength $Vars(components)]
	Layout $w
}


proc SetRange {w btn index} {
	variable ${w}::Vars

	set Vars(start) $index
	Layout $w
	after idle [namespace code [list ButtonEnter $btn]]
}


proc Configure {w width} {
	variable ${w}::Vars

	if {$width <= 1} { return }
	set Vars(start) [llength $Vars(components)]
	Layout $w
}


namespace eval icon {
namespace eval 14x14 {

set PrevComponent [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAQAAAC1QeVaAAAAT0lEQVQY02NgIBEYMEjikrJk
	2Mdggl3KhuEywwcGY2xSDgxXGP4zvMMm6cxwneE/NkkmBg+GWwz/sUuKMeyGSpGqE6+dBF1L
	wJ8EQgglbAEKliaZAtPhwwAAAABJRU5ErkJggg==
}]

set NextComponent [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAQAAAC1QeVaAAAAAmJLR0QA/4ePzL8AAABQSURB
	VBjTY2AgAkgyGOCWNGHYx2CJS9KY4QPDZQYbXJLvGP4zXGFwwC35n+E6gzNuyf8Mtxg8GJhw
	Sf5n2M0gRoZOPHbidC0ef+INIaxhCwCrHiaZy5kakAAAAABJRU5ErkJggg==
}]

set FileSystem [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAMAAAAolt3jAAABnlBMVEVvbW0MDAx+fHyBf39+
	fHx8e3t9fX1/fn6BgIAVFRaBgYGAgIB6enpOTk54dnYcHxyUkpKcmpqfnp6Mi4spKSiko6Op
	qamsrKyysbEfIR+2traOjo65ubkvODG4uLh+gH85OTc6Ojo7PDtGRUGXl5efn5+rq6uwsLC3
	t7e4uLi5ubkzOTRCQT4zSTmtrKyzsrK3tra7u7u/v7/GxcUmJiPMzMzQ0NCFhYXKysrR0dEq
	PS4tOC83Nzg5OTk5Ojk6e0g7OSRfXClvbC1wbS53d3d4eHiLi4uMjIiOjo6QmJKUppeVlZWZ
	mJOcoZ2dnZ2goKChoJuhoaGkpKKkpKSlpaWnp6eoqairq6utra2vr6+xsbGzs7O1tbW2tra4
	uLi5ubm6ubm8vLy9vb2+vr6/v7/AwMDBwcHCwsLDw8PFxcXGxsbHx8fIyMjJycnKysrLy8vN
	zc3Ozs7Pz8/Q0NDR0tHS0tLT1NPU1NTW1tbX19fZ2dnb29vc3Nzd3d3d3t3g4ODh4eHi4uLk
	5OTm5ubn5+fo6Ojp6enq6urr6+v///9jZt2HAAAAOnRSTlMjLzY6Q0tTWV9hY2Zpam95oKSs
	srOzusDExsjKy8zNzs/Pz8/Pz8/Pz8/P0NDR7/Dy8/T19vb3+Pj4+XarFgAAAAFiS0dEiRxh
	JswAAACdSURBVAgdBcFBTsNAEEXBft9tO4OIIisLEJwh189ROAFrhCDC9nh6mirsukjqpuz9
	79f1fntuWEBnuzv4Z1zqY+j9bcLBl828jE2nwMFe9kNM2/mhQVBn0/Z1Ws4tcHjKi451nveS
	OCoazUvuURou8NHM6GqSM1QNaQaZISenmoaZRUpOfoyuiexrJN6ituwNI/yocH1dLTCj6Pvn
	H8+nR5bTmXW8AAAAAElFTkSuQmCC
}]

} ;# namespace 14x14

namespace eval 9x14 {

set Delimiter [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAAkAAAAOCAMAAADKSsaaAAAAZlBMVEUAAAAAUdMAVcwAVM0A
	VckAV8sAVcgAV8kAVMgAWNEAVbsAVtIAWMUAWM8AVcQAV8sAMrEAWMwAObcAQboAV8gARsAA
	V8oAVsUAUcMAWMcATsEAScAAUL8AUswAV8UAT8MAR74APrp/s5o1AAAAInRSTlMARUtSWl5m
	bXN6e4KDhoqKj5GUnJ6hrLS8vMDDzM7c5OboX/pA0QAAAE5JREFUCB0FwQcCgkAMALAoMk5q
	UcB5zv9/0gQAygZgHLeAQ1N2wKlppwGsbd+tCZcuYrg+ce8z97czXiXjscA7oi7gk3UGvnUG
	/I4AAH/GOgNDmSduDgAAAABJRU5ErkJggg==
}]

} ;# namespace 9x14
} ;# namespace icon
} ;# namespace choosedir

# vi:set ts=3 sw=3:
