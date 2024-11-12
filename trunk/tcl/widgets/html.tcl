# ======================================================================
# Author : $Author$
# Version: $Revision: 149 $
# Date   : $Date: 2011-12-09 21:13:24 +0000 (Fri, 09 Dec 2011) $
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

variable Margin	8
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
		-nodehandler	{}
		-doublebuffer	yes
		-delay			0
	}

	array set opts $args

#	append css "body\{"
#	append css "font-family: \"Helvetica\", sans-serif;"
#	append css "font-size: 14px;"
#	append css "line-height: 16px;"
#	append css "\}"
	set css {}

	set options {}
	set htmlOptions {}
	foreach name [array names opts] {
		switch -- $name {
			-nodehandler - -delay {
			}

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

	set parent [::scrolledframe $w -fill both {*}$options]
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
	}

	set options {}
	set Priv(bbox) {}
	set Priv(pointer) {0 0}
	set Priv(delay) $opts(-delay)

	rename ::$w $w.__html__
	proc ::$w {command args} "[namespace current]::WidgetProc $w $parent \$command {*}\$args"

	__html_widget $html -shrink true {*}$htmlOptions -width $MaxWidth
	$html style -id user $css
	if {[llength $opts(-nodehandler)]} {
		$html handler node td $opts(-nodehandler)
	}
	grid $html
	grid anchor $parent center

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
			$parent.html configure -width $MaxWidth
			update idletasks
			$parent.html parse -final [lindex $args 0]
			set Priv(bbox) [ComputeBoundingBox $parent.html [$parent.html node]]
			if {[llength $Priv(bbox)]} {
				lset Priv(bbox) 2 [expr {[lindex $Priv(bbox) 2] + $Margin}]
				$parent.html configure -width [lindex $Priv(bbox) 2]
				update idletasks
				$parent resize
			}
			return
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
		bbox		{ return $Priv(bbox) }
		font		{ return HtmlFont }
	}

	return [$w.__html__ $command {*}$args]
}


proc ComputeBoundingBox {w node} {
	variable MaxWidth
	variable Margin

	set tag [$node tag]
	set result {}

	if {[string length $tag]} {
		switch $tag {
			html - body { ;# skip }

			default {
				set bbox [$w bbox $node]
				if {[llength $bbox]} {
					if {[lindex $bbox 2] == [expr {$MaxWidth - $Margin}]} { lset bbox 2 0 }
					set result $bbox
				}
			}
		}
	}

	if {[llength $result] == 0} {
		foreach n [$node children] {
			set bbox [ComputeBoundingBox $w $n]
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

} ;# namespace html

# vi:set ts=3 sw=3:
