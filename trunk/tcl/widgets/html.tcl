# ======================================================================
# Author : $Author$
# Version: $Revision: 189 $
# Date   : $Date: 2012-01-14 14:31:37 +0000 (Sat, 14 Jan 2012) $
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

package require Tk 8.5
package require Tkhtml 3.1
package require scrolledframe

rename html __html_widget

proc html {args} {
	if {[llength $args] == 0} {
		return error -code "wrong # args: should be \"html pathName ?options?\""
	}

	if {[winfo exists [lindex $args 0]]} {
		return error -code "window name \"[lindex $args 0]\" already exists"
	}

	if {[llength $args] % 2 == 0} {
		return error -code "value for \"[lindex $args end]\" missing"
	}

	return [html::Build {*}$args]
}

namespace eval html {

namespace import ::tcl::mathfunc::min
namespace import ::tcl::mathfunc::max

variable Margin	8 ;# do not change!
variable MaxWidth	12000


proc Build {w args} {
	variable MaxWidth

	array set opts {
		-width			800
		-height			600
		-background		white
		-borderwidth	{}
		-relief			{}
		-imagecmd		{}
		-doublebuffer	yes
		-center			no
		-delay			0
		-css				{}
	}

	array set opts $args

#	append css "body\{"
#	append css "font-family: \"Helvetica\", sans-serif;"
#	append css "font-size: 14px;"
#	append css "line-height: 16px;"
#	append css "\}"
	set css $opts(-css)

	set options {}
	set htmlOptions {}
	foreach name [array names opts] {
		switch -- $name {
			-delay - -css - -center {}

			-imagecmd - -doublebuffer {
				set value $opts($name)
				if {[llength $value]} { lappend htmlOptions $name $value }
			}

			default {
				set value $opts($name)
				if {[llength $value]} { lappend options $name $value }
			}
		}
	}

	if {!$opts(-center)} { lappend options -avoidconfigureresize yes }
	set parent [::scrolledframe $w -fill both {*}$options]
	if {!$opts(-center)} {
		bind $parent <<ScrollbarChanged>> [namespace code { ConfigureFrame %W {*}%d }]
	}
	set html $parent.html

	namespace eval [namespace current]::$parent {}
	variable [namespace current]::${parent}::Priv
	variable [namespace current]::${parent}::HoverNodes
	variable [namespace current]::${parent}::ActiveNodes1
	variable [namespace current]::${parent}::ActiveNodes2
	variable [namespace current]::${parent}::ActiveNodes3

	array set Priv {
		horzScrollbar	{}
		vertScrollbar	{}
		onmouseover		{}
		onmouseout		{}
		onmousedown1	{}
		onmouseup1		{}
		onmousedown2	{}
		onmouseup2		{}
		onmousedown3	{}
		onmouseup3		{}
		nodeList			{}
		afterId			{}
		request			{}
		bbox				{}
		pointer			{0 0}
		sbwidth			0
	}

	set options {}
	set Priv(delay) $opts(-delay)
	set Priv(center) $opts(-center)
	set Priv(bw) $opts(-borderwidth)

	if {[llength $Priv(bw)] == 0} { set Priv(bw) 0 }

	rename ::$w $w.__html__
	proc ::$w {command args} "[namespace current]::WidgetProc $w $parent \$command {*}\$args"

	set options {}
	if {$Priv(center)} { lappend options -width $MaxWidth }
	__html_widget $html {*}$htmlOptions {*}$options -shrink yes
	$html style -id user $css
	grid $html

	if {$Priv(center)} {
		grid anchor $parent center
	} else {
		bind $w <Configure> [namespace code [list Configure $parent %w %#]]
	}

	return $w
}


proc WidgetProc {w parent command args} {
	variable ${parent}::Priv

	switch -glob -- $command {
		parse {
			variable Margin
			variable MaxWidth

			array unset [namespace current]::HoverNodes
			array unset [namespace current]::ActiveNodes1
			array unset [namespace current]::ActiveNodes2
			array unset [namespace current]::ActiveNodes3
			set Priv(nodeList) {}
			$parent.html reset
			$parent xview moveto 0
			$parent yview moveto 0
			if {$Priv(center)} {
				$parent.html configure -width $MaxWidth
				update idletasks
			}
			$parent.html parse -final [lindex $args 0]
			set Priv(bbox) [ComputeBoundingBox $parent.html [$parent.html node] $Priv(center)]
			lset Priv(bbox) 2 [expr {min([lindex $Priv(bbox) 2],4000)}]
#			lset Priv(bbox) 3 [expr {min([lindex $Priv(bbox) 3],8000)}]
			if {[llength $Priv(bbox)]} {
				if {$Priv(center)} {
					lset Priv(bbox) 2 [expr {[lindex $Priv(bbox) 2] + $Margin}]
					$parent.html configure -width [lindex $Priv(bbox) 2]
					update idletasks
					$parent resize
				} else {
					lset Priv(bbox) 3 [expr {[lindex $Priv(bbox) 3] + $Margin}]
					$parent.html configure -height [lindex $Priv(bbox) 3]
					update idletasks
					$parent resize
				}
			}
			return
		}

		handler - search - style {
			return [$parent.html $command {*}$args]
		}

		onmouse* {
			if {[llength $args] != 1} {
				error "wrong # args: should be \"[namespace current] $command <script>\""
			}
			if {![info exists Priv($command)]} {
				append msg "unknown event type \""
				append msg $command
				append msg "\": should be one of"
				append msg [join [array names Priv onmouse*] ", "]
				error $msg
			}
			lappend Priv($command) [lindex $args 0]
			return
		}

		stimulate {
			array unset HoverNodes
			set Priv(nodeList) {}
			set x [expr {[winfo pointerx .] - [winfo rootx $parent.html]}]
			set y [expr {[winfo pointery .] - [winfo rooty $parent.html]}]
			event generate $parent.html <Motion> -x $x -y $y
			return
		}

		drawable	{ return $parent.html }
		pointer	{ return $Priv(pointer) }
		font		{ return HtmlFont }

		bbox {
			if {[llength $args] == 0} {
				return $Priv(bbox)
			}
			if {[llength $args] != 1} {
				error "wrong # args: should be \"[namespace current] $command ?<node>?\""
			}
			return [$parent.html bbox [lindex $args 0]]
		}

		size {
			lassign $Priv(bbox) x y w h
			set w [expr {$w + 2*$x}]
			set h [expr {$h + 2*$y}]
			return [list $w $h]
		}

		xview - yview {
			return [$parent $command {*}$args]
		}

		scrollto {
			if {[llength $args] != 1} {
				error "wrong # args: should be \"[namespace current] $command <px>\""
			}
			variable Margin
			set height [winfo height $parent.html]
			set y [expr {max(0, [lindex $args 0] - $Margin)}]
			set fraction [expr {double($y)/double($height)}]
			return [$parent yview moveto $fraction]
		}

		root {
			return [$parent.html node]
		}
	}

	return [$w.__html__ $command {*}$args]
}


proc Configure {parent width req} {
	variable ${parent}::Priv

	if {[winfo width $parent.html] == $width} { return }

	# This (using the serial field from the event) is working under x11
	# to prevent endless loops. But does this work with windows and mac?
	if {$req == $Priv(request)} { return }
	set Priv(request) $req

	$parent.html configure -width [expr {max(1,$width - 2*$Priv(bw) - $Priv(sbwidth))}]
	after cancel $Priv(afterId)
	set afterId [after idle [namespace code [list ComputeSize $parent $req]]]
}


proc ConfigureFrame {parent sb visible} {
	variable ${parent}::Priv

	if {[$sb cget -orient] eq "vertical"} {
		after cancel $Priv(afterId)
		set Priv(afterId) {}
		set width [$parent.html cget -width]
		set Priv(sbwidth) [expr {[winfo reqwidth $sb]*$visible}]
		incr width $Priv(sbwidth)
		Configure $parent $width 0
	}
}


proc ComputeSize {parent req} {
	variable ${parent}::Priv
	variable Margin

	after cancel $Priv(afterId)
	set Priv(afterId) {}

	set Priv(bbox) [ComputeBoundingBox $parent.html [$parent.html node] $Priv(center)]
	if {[llength $Priv(bbox)]} {
		lset Priv(bbox) 3 [expr {[lindex $Priv(bbox) 3] + $Margin}]
		$parent.html configure -height [lindex $Priv(bbox) 3]
		update idletasks
		$parent resize
	}
}


proc ComputeBoundingBox {w node skipBody} {
	variable MaxWidth
	variable Margin

	set tag [$node tag]
	set result {}

	if {[string length $tag]} {
		if {$tag ne "html" && (!$skipBody || $tag ne "body")} {
			set bbox [$w bbox $node]
			if {[llength $bbox]} {
				if {[lindex $bbox 2] == [expr {$MaxWidth - $Margin}]} { lset bbox 2 0 }
				set result $bbox
			}
		}
	}

	if {[llength $result] == 0} {
		foreach n [$node children] {
			set bbox [ComputeBoundingBox $w $n $skipBody]
			if {[llength $bbox] && [lindex $bbox 2] > 0} {
				set result [CombineBox $result $bbox]
			}
		}
	}

	return $result
}


proc CombineBox {box1 box2} {
	if {[llength $box1] == 0} { return $box2 }
	if {[llength $box2] == 0} { return $box1 }

	lassign $box1 x11 y11 x12 y12
	lassign $box2 x21 y21 x22 y22

	return [list [min $x11 $x21] [min $y11 $y21] [max $x12 $x22] [max $y12 $y22]]
}


proc Motion {w x y state} {
	variable [winfo parent $w]::Priv

	set Priv(pointer) [list $x $y]
	if {$state >= 256} { return }

	set nodelist [lindex [$w node $x $y] end]
	if {$Priv(nodeList) eq $nodelist} { return }

	if {[llength $Priv(afterId)]} {
		after cancel $Priv(afterId)
		set Priv(afterId) {}
	}

	if {$Priv(delay)} {
		set Priv(afterId) [after $Priv(delay) [namespace code [list HandleMotion $w $nodelist]]]
	} else {
		HandleMotion $w $nodelist
	}
}


proc HandleMotion {w nodelist} {
	variable [winfo parent $w]::HoverNodes
	variable [winfo parent $w]::Priv

	set Priv(nodeList) $nodelist
	set events(onmouseover) {}

	foreach node $nodelist {
		if {[string length [$node tag]] == 0} { set node [$node parent] }

		for {set n $node} {[string length $n] > 0} {set n [$n parent]} {
			if {[info exists hoverNodes($n)]} { break }

			if {[info exists HoverNodes($n)]} {
				unset HoverNodes($n)
			} else {
				lappend events(onmouseover) $n
			}

			set hoverNodes($n) 1
		}
	}

	set events(onmouseout) [array names HoverNodes]

	array unset HoverNodes
	array set HoverNodes [array get hoverNodes]

	set eventlist {}

	foreach key {onmouseout onmouseover} {
		foreach node $events($key) {
			lappend eventlist $key $node
		}
	}

	GenerateEvents $w $eventlist
}


proc Leave {w} {
	variable [winfo parent $w]::HoverNodes
	variable [winfo parent $w]::Priv

	if {[llength $Priv(afterId)]} {
		after cancel $Priv(afterId)
		set Priv(afterId) {}
	}

	set eventlist {}

	foreach node [array names HoverNodes] {
		lappend eventlist onmouseout $node
	}

	array unset HoverNodes
	GenerateEvents $w $eventlist
}


proc Mapped {w} {
	lassign [winfo pointerxy $w] x y
	set x [expr {$x - [winfo rootx $w]}]
	set y [expr {$y - [winfo rooty $w]}]
	Motion $w $x $y 0
}


proc ButtonPress {w x y k} {
	variable [winfo parent $w]::ActiveNodes$k
	variable [winfo parent $w]::Priv

	array unset ActiveNodes$k
	set node [lindex [$w node $x $y] end]
	if {[llength $node]} {
		if {[string length [$node tag]] == 0} { set node [$node parent] }
	}
	if {[string length $node] == 0 || [string length [$node tag]] == 0} { set node [$w node] }

	for {set n $node} {[string length $n] > 0} {set n [$n parent]} {
		set ActiveNodes${k}($n) 1
	}

	set eventlist {}
	foreach node [array names ActiveNodes$k] {
		lappend eventlist onmousedown${k} $node
	}
	lappend eventlist onmousedown${k} {}

	GenerateEvents $w $eventlist
}


proc ButtonRelease {w x y k} {
	variable [winfo parent $w]::ActiveNodes$k
	variable [winfo parent $w]::Priv

	set node [lindex [$w node $x $y] end]
	if {[llength $node]} {
		if {[string length [$node tag]] == 0} { set node [$node parent] }
	}
	if {[string length $node] == 0 || [string length [$node tag]] == 0} { set node [$w node] }

	set eventlist {}
	foreach node [array names ActiveNodes$k] {
		lappend eventlist onmouseup${k} $node
	}
	lappend eventlist onmouseup${k} {}

	GenerateEvents $w $eventlist
	array unset ActiveNodes$k
}


proc GenerateEvents {w eventlist} {
	variable [winfo parent $w]::Priv

	foreach {event node} $eventlist {
		if {[llength $node] == 0 || [llength [info commands $node]]} {
			foreach script $Priv($event) {
				{*}$script $node
			}
		}
	}
}


bind Html <Motion>				[list [namespace current]::Motion %W %x %y %s ]
bind Html <Leave>					[list [namespace current]::Leave %W ]
bind Html <ButtonPress-1>		[list [namespace current]::ButtonPress %W %x %y 1 ]
bind Html <ButtonRelease-1>	[list [namespace current]::ButtonRelease %W %x %y 1 ]
bind Html <ButtonPress-2>		[list [namespace current]::ButtonPress %W %x %y 2 ]
bind Html <ButtonRelease-2>	[list [namespace current]::ButtonRelease %W %x %y 2 ]
bind Html <ButtonPress-3>		[list [namespace current]::ButtonPress %W %x %y 3 ]
bind Html <ButtonRelease-3>	[list [namespace current]::ButtonRelease %W %x %y 3 ]
bind Html <Unmap>					[list [namespace current]::Leave %W ]
bind Html <Map>					[list [namespace current]::Mapped %W ]

switch [tk windowingsystem] {
	win32 {
		bind Html <MouseWheel> { [winfo parent %W] yview scroll [expr %D/-120] units }
	}
	aqua {
		bind Html <MouseWheel> { [winfo parent %W] yview scroll [expr %D*-1] units }
	}
	x11 {
		bind Html <ButtonPress-4> { [winfo parent %W] yview scroll -1 units }
		bind Html <ButtonPress-5> { [winfo parent %W] yview scroll +1 units }
	}
}

} ;# namespace html

# vi:set ts=3 sw=3:
