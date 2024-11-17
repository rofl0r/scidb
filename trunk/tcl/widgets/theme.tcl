# ======================================================================
# Author : $Author$
# Version: $Revision: 1520 $
# Date   : $Date: 2018-09-12 10:22:56 +0000 (Wed, 12 Sep 2018) $
# Url    : $URL$
# ======================================================================

# ======================================================================
# Copyright: (C) 2009-2018 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

namespace eval theme {

variable padding 5
variable padx 5
variable pady 5
variable padY 10
variable padX 10
variable sliderWidth 8
variable strongTtk false
variable repeatDelay 0
variable repeatInterval 0
variable useCustomStyleMenuEntries 1

variable Settings
variable Setup 1
variable ActiveBackground {}
variable DisabledMenu {}


# this should be already done in tk8.x.x/library/tk.tcl,
# but it doesn't work for any reason
switch [tk windowingsystem] {
	x11 {
		event add <<Cut>>		<Control-Key-x> <Key-F20> 
		event add <<Copy>>	<Control-Key-c> <Key-F16>
		event add <<Paste>>	<Control-Key-v> <Key-F18>
		event add <<PasteSelection>> <ButtonRelease-2>
		event add <<Undo>>	<Control-Key-z>
		event add <<Redo>>	<Control-Key-y>
	}

	win32 {
		event add <<Cut>>		<Control-Key-x> <Shift-Key-Delete>
		event add <<Copy>>	<Control-Key-c> <Control-Key-Insert>
		event add <<Paste>>	<Control-Key-v> <Shift-Key-Insert>
		event add <<PasteSelection>> <ButtonRelease-2>
		event add <<Undo>>	<Control-Key-z>
		event add <<Redo>>	<Control-Key-y>
	}

	aqua {
		event add <<Cut>>		<Command-Key-x> <Key-F2> 
		event add <<Copy>>	<Command-Key-c> <Key-F3>
		event add <<Paste>>	<Command-Key-v> <Key-F4>
		event add <<PasteSelection>> <ButtonRelease-2>
		event add <<Clear>>	<Clear>
		event add <<Undo>>	<Command-Key-z>
		event add <<Redo>>	<Command-Key-y>
	}
}

proc bindCut {w {cmd {#}}} {
	switch [tk windowingsystem] {
		x11 	{ set keys {<Control-Key-x> <Key-F20>} }
		win32	{ set keys {<Control-Key-x> <Shift-Key-Delete>} }
		aqua	{ set keys {<Control-Key-x> <Key-F2> } }
	}
	foreach key $keys {
		bind $w $key $cmd
	}
}


proc bindCopy {w {cmd {#}}} {
	switch [tk windowingsystem] {
		x11 	{ set keys {<Control-Key-c> <Key-F16>} }
		win32	{ set keys {<Control-Key-c> <Control-Key-Insert>} }
		aqua	{ set keys {<Control-Key-c> <Key-F3> } }
	}
	foreach key $keys {
		bind $w $key $cmd
	}
}


proc bindPaste {w {cmd {#}}} {
	switch [tk windowingsystem] {
		x11 	{ set keys {<Control-Key-v> <Key-F18>} }
		win32	{ set keys {<Control-Key-v> <Shift-Key-Insert>} }
		aqua	{ set keys {<Control-Key-v> <Key-F4> } }
	}
	foreach key $keys {
		bind $w $key $cmd
	}
}


proc bindUndo {w {cmd {#}}} {
	foreach key {<Control-Key-z> <Control-Key-Z>} {
		bind $w $key $cmd
	}
}


proc bindRedo {w {cmd {#}}} {
	foreach key {<Control-Key-y> <Control-Key-Y>} {
		bind $w $key $cmd
	}
}


proc bindPasteSelection {w {cmd {#}}} {
	bind $w <ButtonRelease-2> $cmd
}


event add <<ControlOn>>		<KeyPress-Control_L>    <KeyPress-Control_R>
event add <<ControlOff>>	<KeyRelease-Control_L>	<KeyRelease-Control_R>
event add <<ShiftOn>>		<KeyPress-Shift_L>		<KeyPress-Shift_R>
event add <<ShiftOff>>		<KeyRelease-Shift_L>		<KeyRelease-Shift_R>


switch $::tcl_platform(platform) {
	unix			{ event add <<Cancel>> <Control-c> <Escape> }
	windows		{ event add <<Cancel>> <Escape> }
	macintosh	{ event add <<Cancel>> <Command-period> }
}


# This is not working under older Tk versions.
#proc currentTheme {} { return [ttk::style theme use] }
proc currentTheme {} { return $::ttk::currentTheme }


proc getToplevelBackground {} {
	return [set [namespace current]::Settings(tk:background)]
}


proc getColor {which} {
	switch $which {
		disabled {
			return [ttk::style lookup [currentTheme] -foreground disabled]
		}

		activebackground {
			# for any reason the correct implementation is not
			# working, so I'm using a workaround
			return #efefef

#	correct implementation
#	set activebg [ttk::style lookup [currentTheme] -activebackground]
#
#	if {[string length $activebg] == 0} {
#		variable ActiveBackground
#
#		if {[llength $ActiveBackground] == 0} {
#			set btn [tk::button .__btn_activebackground__]
#			set ActiveBackground [$btn cget -activebackground]
#			destroy $btn
#		}
#
#		set activebg $ActiveBackground
#	}
#
#	return $activebg
		}
	}

	return [ttk::style lookup [currentTheme] -$which]
}


proc setTheme {name} {
	variable Setup

	if {$name ne [currentTheme] || $Setup} {
		::ttk::setTheme $name
		SetupCurrentTheme
		set Setup 0
	}
}


proc configureCanvas {canv} {
	configureBackground $canv
	bind $canv <<ThemeChanged>> [namespace code [list configureBackground $canv]]
}


proc configurePanedWindow {win} {
	$win configure -background [GetTroughColor]
}


proc configureSpinbox {sbox} {
	if {[winfo class $sbox] eq "Spinbox"} {
		$sbox configure -relief sunken
		ConfigureSpinboxBackground $sbox
		bind $sbox <<ThemeChanged>> [namespace code [list ConfigureSpinboxBackground $sbox]]
	}
}


proc enableScale {scale {enable true}} {
	if {$enable} {
		$scale configure \
			-foreground [::ttk::style lookup [currentTheme] -foreground] \
			-troughcolor [GetTroughColor] \
			-state normal
	} else {
		disableScale $scale
	}
}


proc configureCheckEntry {menu {index end}} {
	variable useCustomStyleMenuEntries

	set entry [$menu index $index]
	set text [$menu entrycget $entry -label]
	if {[string index $text 0] ne " "} {
		$menu entryconfigure $entry -label " $text"
	}

	if {$useCustomStyleMenuEntries} {
		set img [$menu entrycget $entry -image]
		if {[string length $img] == 0} {
			# TODO: using 14x14 is only a work-around, because the menu is adding
			# an extra pixel, otherwise 16x16 should be preferred.
			$menu entryconfigure $entry \
				-image $icon::14x14::checkNo \
				-selectimage $icon::14x14::checkYes \
				-compound left \
				-indicatoron no \
				;
		} else {
			variable Icons
			if {![info exists Icons($img)]} {
				set y [expr {(16 - [image height $img])/2}]
				set imgOff [image create photo -width 36 -height 16]
				$imgOff copy $icon::16x16::checkNo -to 0 0
				$imgOff copy $img -to 20 $y
				set imgOn [image create photo -width 36 -height 16]
				$imgOn blank
				$imgOn copy $img -to 20 $y
				$imgOn copy $icon::16x16::checkYes -to 0 0
				set Icons($img) [list $imgOff $imgOn]
			} else {
				lassign $Icons($img) imgOff imgOn
			}
			$menu entryconfigure $entry -indicatoron off -image $imgOff -selectimage $imgOn -compound left
		}
	}
}

proc configureRadioEntry {menu {index end}} {
	variable useCustomStyleMenuEntries

	set entry [$menu index $index]

	if {$useCustomStyleMenuEntries} {
		set text [$menu entrycget $entry -label]
		if {[string index $text 0] ne " "} {
			$menu entryconfigure $entry -label " $text"
		}
		set img [$menu entrycget $entry -image]
		if {[string length $img] == 0} {
			# TODO: using 14x14 is only a work-around, because the menu is adding
			# an extra pixel, otherwise 16x16 should be preferred.
			$menu entryconfigure $entry \
				-indicatoron off \
				-image $icon::14x14::radioOff \
				-selectimage $icon::14x14::radioOn \
				-compound left \
				;
		} else {
			variable Icons
			if {![info exists Icons($img)]} {
				set y [expr {(16 - [image height $img])/2}]
				set imgOff [image create photo -width 36 -height 16]
				$imgOff copy $icon::16x16::radioOff -to 0 0
				$imgOff copy $img -to 20 $y
				set imgOn [image create photo -width 36 -height 16]
				$imgOn copy $icon::16x16::radioOn -to 0 0
				$imgOn copy $img -to 20 $y
				set Icons($img) [list $imgOff $imgOn]
			} else {
				lassign $Icons($img) imgOff imgOn
			}
			$menu entryconfigure $entry -indicatoron off -image $imgOff -selectimage $imgOn -compound left
		}
	} else {
		$menu entryconfigure $entry \
			-indicatoron off \
			-image $::icon::15x15::none \
			-selectimage $icon::15x15::Dot \
			-compound left \
			;
	}
}


proc disableScale {scale} {
	$scale configure -state disabled
	ConfigureScaleBackground $scale
}


proc configureDefaultButton {but} {
	$but configure -default active
}


proc configureBackground {w} {
	$w configure -background [::ttk::style lookup [currentTheme] -background]
}


proc notebookBorderwidth {} {
	switch [currentTheme] {
		alt { return 1 }
		clam - clearlooks - scidblue { return 2 }
	}
	set result [ttk::style lookup TNotebook -borderwidth]
	if {[string is integer -strict $result]} { return $result }
	return 1
}


proc notebookTabPaneSize {nb} {
	set padding [ttk::style lookup TNotebook.Tab -padding]
	if {[llength $padding] == 0} {
		puts stderr "\[ttk::style lookup TNotebook.Tab -padding\] returns empty list for '[currentTheme]'"
		set padding {2 2}
	}
	set size [expr {2*[notebookBorderwidth] + 1}] ;# plus one overlapping pixel
	switch [llength $padding] {
		2 { incr size [expr {2*[lindex $padding 1]}] }
		3 { incr size [lindex $padding 1] }
		4 { incr size [lindex $padding 1]; incr size [lindex $padding 3] }
	}
	set linespace [font metrics [ttk::style lookup TNotebook.Tab -font] -linespace]
	set haveImage 0
	foreach tab [$nb tabs] {
		if {[string length [set img [$nb tab $tab -image]]]} {
			set linespace [expr {max($linespace, [image height $img])}]
			set haveImage 1
		}
	}
	incr size $haveImage
	incr size $linespace
#	if {	[string length [$nb select]]
#		&& [set ht [expr {[winfo height $nb] - [winfo height [$nb select]]}]] > 1
#		&& $ht != $size} { puts stderr "notebookTabPaneSize($nb): computed=$size, but measured=$ht" }
	return $size
}


proc SetupCurrentTheme {} {
	variable strongTtk
	variable Settings

	# 1. Makes a correction of read-only state color. Read-Only does not mean disabled!
	set fbg {}
	switch "_[currentTheme]" {
		_alt		{ set fbg [list readonly white disabled [ttk::style lookup [currentTheme] -background]] }
		_clam		{ set fbg [list \
									readonly white \
									{readonly focus} [::ttk::style lookup [currentTheme] -selectbackground]] }
		_default	{ set fbg [list readonly white disabled [ttk::style lookup [currentTheme] -background]] }
	}
	if {[llength $fbg]} {
		ttk::style map TCombobox -fieldbackground $fbg
		ttk::style map TEntry -fieldbackground $fbg
		if {[info tclversion] >= "8.6"} {
			ttk::style map TSpinbox -fieldbackground $fbg
		}
	}

	# adjust padding of arrows in spinbox (the default padding is quite ugly)
	switch "_[currentTheme]" {
		_alt - _clam - _default { ttk::style configure TSpinbox -padding {2 0 2 0} }
		_clearlooks - _scidblue { ttk::style configure TSpinbox -padding {2 0 2 0} }
	}

	# get current settings
	set background [::ttk::style lookup [currentTheme] -background]
	set f [tk::frame .__hopefully_unique_widget_id__[clock milliseconds]]
	set Settings(tk:background) [$f cget -background]
	destroy $f

	# 2a. Provide a left aligned button.
	ttk::style configure aligned.TButton -anchor w -width -9
	# 2b. Provide a button for icons.
	ttk::style configure icon.TButton -padding 0

	# 3. Provide a ttk::scale with keyboard focus.
	ttk::style configure active.Horizontal.TScale -troughcolor [GetActiveTroughColor]
	ttk::style configure active.Vertical.TScale -troughcolor [GetActiveTroughColor]

	# 4. Set theme options
#	option add *Frame.background $Settings(tk:background)
#	option add *Button.background $background
	option add *Spinbox.selectBackground [::ttk::style lookup [currentTheme] -selectbackground]
	option add *Spinbox.disabledBackground $background
	option add *Entry.selectBackground [::ttk::style lookup [currentTheme] -selectbackground]
	option add *Scale.highlightBackground $background
	option add *Scale.background $background
	option add *Scale.troughColor [GetTroughColor]
	option add *Listbox.disabledForeground [::ttk::style lookup [currentTheme] -foreground disabled]
#	option add *Listbox.selectBackground [::ttk::style lookup [currentTheme] -selectbackground]
#	option add *Listbox.selectForeground [::ttk::style lookup [currentTheme] -selectforeground]
#	option add *Panedwindow.background [GetTroughColor]

	if {$strongTtk} {
		option add *Frame.background $background
		option add *Toplevel.background $background
		option add *Dialog.background $background
		option add *Label.background $background
		option add *Canvas.background $background
		option add *Notebook.background $background
	}
}


proc ConfigureListbox {list} {
	$list configure \
		-disabledforeground [::ttk::style lookup [currentTheme] -foreground disabled]
#		-selectbackground [::ttk::style lookup [currentTheme] -selectbackground]
#		-selectforeground [::ttk::style lookup [currentTheme] -selectforeground]
}


proc ConfigureSpinboxBackground {sbox} {
	set borderwidth 1
	switch "_[currentTheme]" { _alt { set borderwidth 2 } }

	$sbox configure \
		-buttonbackground [::ttk::style lookup [currentTheme] -background] \
		-selectbackground [::ttk::style lookup [currentTheme] -selectbackground] \
		-disabledbackground [::ttk::style lookup [currentTheme] -background] \
		-borderwidth $borderwidth
}


proc ConfigureScaleBackground {scale} {
	set background [::ttk::style lookup [currentTheme] -background]

	if {[$scale cget -state] eq "disabled"} {
		$scale configure \
			-foreground [getColor disabled] \
			-highlightbackground $background \
			-background $background \
			-troughcolor $background
	} else {
		$scale configure \
			-highlightbackground $background \
			-background $background \
			-troughcolor [GetTroughColor]
	}
}


proc GetTroughColor {} {
	variable Settings

	switch "_[currentTheme]" {
		_aqua			-
		_winnative	-
		_xpnative	{
			if {![info exists Settings([currentTheme]:troughcolor)} {
				set sc [ttk::scale .__hopefully_unique_widget_id__[clock milliseconds]]
				set Settings([currentTheme]:troughcolor) [$sc cget -troughcolor]
				destroy $sc
			}

			return $Settings([currentTheme]:troughcolor)
		}
	}

	return [::ttk::style lookup [currentTheme] -troughcolor]
}


proc GetActiveTroughColor {} {
	return [::ttk::style lookup [currentTheme] -selectbackground]
}


proc ListboxHome {w} {
	switch [$w cget -selectmode] {
		single {
			$w activate 0
			$w see 0
		}
		browse {
			$w selection clear 0 end
			$w selection set 0
			$w activate 0
			$w see 0
		}
		default {
			return -code error "ListboxHome not implemented for select mode [$w cget -selectmode]"
		}
	}
}


proc ListboxEnd {w} {
	switch [$w cget -selectmode] {
		single {
			$w activate end
			$w see end
		}
		browse {
			$w selection clear 0 end
			$w selection set end
			$w activate end
			$w see end
		}
		default {
			return -code error "ListboxEnd not implemented for select mode [$w cget -selectmode]"
		}
	}
}


proc ListboxNext {w} {
	if {[$w cget -selectmode] eq "extended"} {
		return -code error "ListboxNext not implemented for select mode 'extended'"
	}
	set index [$w nearest 0]
	set active [$w index active]
	set linespace [expr {[font metrics [$w cget -font] -linespace] + 1}]
	set nrows [expr {[winfo height $w]/$linespace}]
	set last [expr {$index + $nrows - 1}]
	if {$last == $active} {
		$w yview scroll +1 pages
		$w yview scroll +1 units
	}
	$w activate @0,10000
	if {[$w cget -selectmode] eq "browse"} {
		$w selection clear 0 end
		$w selection set [$w index active]
		event generate $w <<ListboxSelect>>
	}
}


proc ListboxPrior {w} {
	if {[$w cget -selectmode] eq "extended"} {
		return -code error "ListboxPrior not implemented for select mode 'extended'"
	}
	set index [$w index active]
	if {$index eq ""} { return }
	if {[$w bbox $index] ne ""} {
		set bbox [$w bbox [expr {$index - 1}]]
		if {$bbox eq ""} {
			$w yview scroll -1 pages
			$w yview scroll -2 units
		}
	}
	$w activate @0,0
	if {[$w cget -selectmode] eq "browse"} {
		$w selection clear 0 end
		$w selection set [$w index active]
		event generate $w <<ListboxSelect>>
	}
}


proc ListboxUpDown {w dir} {
	if {[$w cget -selectmode] eq "extended"} {
		return -code error "ListboxUpDown not implemented for select mode 'extended'"
	}
	set index [$w index active]
	if {$index eq ""} { return }
	set bbox [$w bbox $index]
	if {$bbox eq ""} {
		if {$dir == -1} {
			$w activate @0,10000
		} else {
			$w activate @0,0
		}
	} else {
		$w activate [expr {$index + $dir}]
		$w see active
	}
	if {[$w cget -selectmode] eq "browse"} {
		$w selection clear 0 end
		$w selection set [$w index active]
		event generate $w <<ListboxSelect>>
	}
}


proc ScaleMove {scale delta} {
	$scale set [expr {[$scale get] + $delta}]
}


proc ScaleJump {scale x y} {
	set curr [$scale get]
	set next [$scale get $x $y]

	if {$next < $curr} {
		$scale set [$scale cget -from]
	} elseif {$next > $curr} {
		$scale set [$scale cget -to]
	}
}


proc ScaleTakeFocus {scale} {
	if {[$scale cget -takefocus] && [$scale cget -state] ne "disabled"} { focus $scale }
}


proc ComboboxSelected {w} {
	# theme "clam" has a bug, we cannot clear the selection
	if {[currentTheme] ne "clam"} { $w selection clear }
}

option add *background #efefef
option add *inactiveSelectBackground darkgrey
option add *inactiveSelectForeground white

option add *Spinbox.background white
option add *Spinbox.foreground black
option add *Spinbox.selectForeground white
option add *Spinbox.readonlyBackground white

option add *Entry.foreground black
option add *Entry.selectForeground black
option add *Entry.readonlyBackground white
#option add *Entry.selectBackground #678db2

option add *Listbox.background white
option add *Listbox.foreground black
option add *Listbox.selectBorderWidth 0
option add *Listbox.activeStyle dotbox
option add *Listbox.selectBackground #ffdd76
option add *Listbox.selectForeground black

option add *Panedwindow.sashWidth 4
option add *Panedwindow.sashRelief raised

option add *Menu.tearOff 0
option add *Menu.foreground black
option add *Menu.activeBackground #678db2
#option add *Menu.activeBackground #437597
option add *Menu.activeForeground white
option add *Menu.activeBorderWidth 0

option add *Text.background white
option add *Text.foreground black
#option add *Text.selectBackground #678db2

option add *Label.foreground black
option add *Labelframe.foreground black

option add *Button.foreground black
option add *Button.highlightThickness 0

option add *Radiobutton.foreground black
option add *Checkbutton.foreground black
option add *Menubutton.foreground black

option add *Canvas.highlightThickness 0

option add *Scale.foreground black

option add *TreeCtrl.highlightThickness 0
option add *TreeCtrl.foreground black

option add *TButton.foreground black
option add *TRadiobutton.foreground black
option add *TCheckbutton.foreground black
option add *TMenubutton.foreground black
option add *TLabel.foreground black
option add *TScale.foreground black
option add *TNotebook.foreground black

option add *TCombobox.exportSelection 0 ;# fixing a bug with -exportselection
option add *TCombobox.foreground black

option add *TSpinbox.wrap 0
option add *TSpinbox.foreground black

option add *TEntry.exportSelection 0 ;# fixing a bug with -exportselection
option add *TEntry.background white
option add *TEntry.foreground black
#option add *TEntry.selectBackground #678db2
#option add *TEntry.cursor xterm

#option add *Combobox.cursor xterm

bind Listbox <Home>	[namespace code { ListboxHome %W }]
bind Listbox <End>	[namespace code { ListboxEnd %W }]
bind Listbox <Next>	[namespace code { ListboxNext %W }]
bind Listbox <Prior>	[namespace code { ListboxPrior %W }]
bind Listbox <Up>		[namespace code { ListboxUpDown %W -1 }]
bind Listbox <Down>	[namespace code { ListboxUpDown %W +1 }]

bind Scale <Button-1>	+[namespace code { ScaleTakeFocus %W }]
bind Scale <FocusIn>		+[namespace code { %W configure -troughcolor [GetActiveTroughColor] }]
bind Scale <FocusOut>	+[namespace code { %W configure -troughcolor [GetTroughColor] }]

bind Spinbox <FocusIn>  {+ if {[%W cget -state] eq "normal"} { %W selection range 0 end } }
bind Spinbox <FocusOut> {+ if {![%W cget -exportselection]} { %W selection clear } }

#bind TSpinbox <FocusIn>  {+ %W instate {!readonly !disabled} { %W selection range 0 end } }
bind TSpinbox <FocusOut> {+ if {![%W cget -exportselection]} { %W selection clear } }

bindRedo Text { catch { %W edit redo } }

# TODO: probably we want to override <Ctrl-A> of text/entry/ttk::entry

bind TCombobox <<ComboboxSelected>> +[namespace code { ComboboxSelected %W }]
bind TCombobox <FocusOut> {+ after idle { if {![%W cget -exportselection]} { %W selection clear } } }
bind TCombobox <B1-Leave> { break }	;# avoid AutoScroll (bug in Tk)

bind TEntry <FocusOut> {+ if {![%W cget -exportselection]} { %W selection clear } }

# change readonly behavior of ttk::entry

bind TEntry <ButtonPress-2> {
	%W instate {!readonly !disabled} { ttk::entry::ScanMark %W %x }
}
bind TEntry <Shift-ButtonPress-1> {
	%W instate {!readonly !disabled} { ttk::entry::Shift-Press %W %x }
}
bind TEntry <Double-ButtonPress-1> {
	%W instate {!readonly !disabled} { ttk::entry::Select %W %x word }
}
bind TEntry <Triple-ButtonPress-1> {
	%W instate {!readonly !disabled} { ttk::entry::Select %W %x line }
}
bind TEntry <<TraverseIn>> {
	%W instate {!readonly !disabled} {
		%W selection range 0 end
		%W icursor end
	}
}
bind TEntry <Control-Key-slash> {
	%W instate {!readonly !disabled} {
		%W selection range 0 end
	}
}

# an entry should have a xterm cursor if in normal state
bind TEntry <Enter> {
	%W instate {!readonly !disabled} {
		%W configure -cursor xterm
	}
}

bind TEntry <Leave> {
	%W instate {!readonly !disabled} {
		%W configure -cursor {}
	}
}

bind THorzScale <ButtonPress-1>		[bind TScale <ButtonPress-1>]
bind THorzScale <B1-Motion>			[bind TScale <B1-Motion>]
bind THorzScale <ButtonRelease-1>	[bind TScale <ButtonRelease-1>]
bind THorzScale <Control-Button-1>	[namespace code { ScaleJump %W %x %y }]
bind THorzScale <Key-Right>			[namespace code { ScaleMove %W +1 }]
bind THorzScale <Key-Left>				[namespace code { ScaleMove %W -1 }]
bind THorzScale <Control-Right>		[namespace code { ScaleMove %W +10 }]
bind THorzScale <Control-Left>		[namespace code { ScaleMove %W -10 }]
bind THorzScale <Home>					{ %W set [%W cget -from] }
bind THorzScale <End>					{ %W set [%W cget -to] }
bind THorzScale <FocusIn>				{ %W configure -style active.Horizontal.TScale }
bind THorzScale <FocusOut>				{ %W configure -style Horizontal.TScale }
bind THorzScale <Button-1>				{+ focus %W }

bind TVertScale <ButtonPress-1>		[bind TScale <ButtonPress-1>]
bind TVertScale <B1-Motion>			[bind TScale <B1-Motion>]
bind TVertScale <ButtonRelease-1>	[bind TScale <ButtonRelease-1>]
bind THorzScale <Control-Button-1>	[namespace code { ScaleJump %W %x %y }]
bind TVertScale <Key-Down>				[namespace code { ScaleMove %W +1 }]
bind TVertScale <Key-Up>				[namespace code { ScaleMove %W -1 }]
bind THorzScale <Control-Down>		[namespace code { ScaleMove %W +10 }]
bind THorzScale <Control-Up>			[namespace code { ScaleMove %W -10 }]
bind THorzScale <Home>					{ %W set [%W cget -from] }
bind THorzScale <End>					{ %W set [%W cget -to] }
bind TVertScale <FocusIn>				{ %W configure -style active.Vertical.TScale }
bind TVertScale <FocusOut>				{ %W configure -style Vertical.TScale }
bind TVertScale <Button-1>				{+ focus %W }

bind Spinbox		<<ThemeChanged>> [namespace code [list ConfigureSpinboxBackground %W]]
bind Scale			<<ThemeChanged>> [namespace code [list ConfigureScaleBackground %W]]
bind Listbox		<<ThemeChanged>> [namespace code [list ConfigureListbox %W]]
bind Panedwindow	<<ThemeChanged>> [namespace code [list configurePanedWindow %W]]


scrollbar .__scrollbar__
set repeatDelay [.__scrollbar__ cget -repeatdelay]
set repeatInterval [.__scrollbar__ cget -repeatinterval]
destroy .__scrollbar__


namespace eval icon {
namespace eval 14x14 {

set checkYes [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAQAAAC1QeVaAAABQUlEQVQY003JPSwDcRjA4d/7v6se
	7tB2YFCDUINEGMVXJDbpYiEMxGIxCAsDK1NXi8FgEJvNYJCIsJEwiKlIk2r0gzhUXV+LhDzjI42x
	87lY2g0CgDqDNhrenh4tcZMHcrbQ3e/PVhEULShBRgk53mkuX7qxE/5r0iCAIFGwokLhszTdfn9y
	bdxAo8ofIUfK2XM+uypx8yX/S3lnm2NcnEurwWApBvNbsMsR48xQ01fK28YNuMLQg2BxyB69LBOi
	Chh5e2afFU5RLkgRZx2PLxSwv4Pm4mQkzSYvbNPEGu0UUQCMvlciw6zisUWZRQbJAwqEHbsYEgqM
	8MoOo0yQxaAI5WytZ590JO9KiRxjtNFJFiHAIvKR/rbUvvVaMn0JB2GAD+oBqHCbffBbfQl3z08V
	Gsq2qCIAVGkKx/wha2njB7TmgujKPNHYAAAAAElFTkSuQmCC
}]

set checkNo [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAQAAAC1QeVaAAAA20lEQVQY02MskTC3lJLk+v/zMysn
	AxT8Yvr4S/qb2EEW5wgl9x8WjAJcDAjAziCw4z/z829MHO9/eTAJMDIgw/8Mvz0YP10VYuL9/RfI
	QYf/GISCP8uy/GMEcdDBf4a/75i4WRgYsEuCxICS/4C2oIN/YJIFJPUPi04Ghu9fmFg5/3z4j8VJ
	jEIfPrIwMYCchKkTZBXLJ1Y2LA5iZPgFFGW6psB3l4HhLwr8zyDA8PQDHyvLnR+i33UYOJF0g3Q9
	Y3hwQ/UTI69qcf495j9sTEje+PNF7q/WnVnzATQiZnzWNkXcAAAAAElFTkSuQmCC
}]

set radioOn [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAYAAAAfSC3RAAACkUlEQVQoz03Sy29UVQCA8e+ce+fe
	ufNghspKCEimMSSNmILRgK+VxJUr3FgT2LhpgpGda4OWxBSYNHHXDbKeyNKtiZagkxjb0oRJKuhi
	nvb2znTuua9zjgtc+PsXvg9ekEB1fX39tb+ePbsXx/EfWhc2SRI1mUx2er1eu9PpvAo4/I8DNHu9
	3g2l4ng2PbST8ciORn07Ho/sNDq0SaLsYRiqvb29zwEXQADN7e3t661W6+58NsUaA0IghMBaC9aC
	EFRrddIso9/vf7G0tLQhb968ca7Van1zNI3QRiOkJKiUsdZSqQQIKTHWMD+a4Xse9XrtdqfTWXQ2
	Nze/rFUrl/MsQ0qHeTxno73Bg+8f8Gu3y/L58/iBjzEGgQBwK9WqcF85c/b92SwCwPc82u02D394
	SJambO/skKcpa7fXiOOYJEs4VjvGaDj8QHq+t6i1BmOZJ4qnT3tkaYqUEqM1u7tPiJXCGIvRGtdz
	yXVxVqYqASvQ1lAplzl95jSlUulFIylpLbYIggBrDRJIkwytNc4nKyufNur1E0VRUBQ577z7HsPR
	kFgpli8s89Wtr8kShTWWkudxEIZE02jf3dr65fePr149l2UKXRjcEqzfuYsxBiklk+GQpNAIIXHd
	Ev3BgESpn5woOnAuXbr80cLCCVfrnCzPOJpGJEoRhf+Q5RkCQTkIGI7HhOFB+lu3+5nz/PnfWfN4
	wzt58tSbCwsv4TgSXWh0kQMCz/PwywH9/oD9/X0Oo+jW6upqxwHU48c//1mv1bHwukU49UaToFLF
	CsFgMGBn9wmj4SALo3Dt2rXr3wKp+G+7su/z8ltvvH3xyodXVpqNxgXfD05JKXKL6Od5vrX16NF3
	93+832VIDNh/ATYUWOuu70eXAAAAAElFTkSuQmCC
}]

set radioOff [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAQAAAC1QeVaAAABG0lEQVQY02XKvUrDUAAG0O/2JoXS
	DNVB3EUKooPUJ6g4F0Sok4gBn6CDi4M+SHWVKqgVAlUHoaP4BIppmyb3pv7USkzBwOciQixnPQCQ
	Qb5d9S9ilSRjpc/bVViQAABIFPx6zBFfGHLAD8YM6piCAQAF9/iLIRU1NTUVNSP2jjCNDE63Y2qq
	FM2Il1uw8Ngc0meQ4vONT03MIlIB+/RT+gz4qUQR34lHj/0Ujx7HibmMkfLYm+BxqMwV3Dua3Qma
	D45ZQm3vnV12/nnlfk0swCi3GgN2+Ez3j+bdibGKGWBeVq7ONIPf4DNkqyErWIQJ5LJLct0+dG7d
	MEy6+vrGPpAbZgkWBCCQy86JsqwaO5ldwxabYg3FQh4C+AGj2AxdwPoCiQAAAABJRU5ErkJggg==
}]

} ;# namespace 14x14
namespace eval 15x15 {

set None [image create photo -width 15 -height 15]

set Dot [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAA8AAAAPCAQAAACR313BAAAANUlEQVQY02P4z4APMtBJGgaw
	SgPBFIY3QDiFAav0FDhzCjbpN3DmGzKk8Rv+H+60gQg1bBAALHvWNn3PjjEAAAAASUVORK5C
	YII=
}]

} ;# namespace 15x15

namespace eval 16x16 {

set checkYes [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAAAAXNSR0IArs4c6QAAAZNJREFUKM9N
	0T9oU1EcxfHv796b5KXmpU1SUcGIFI2DIHYU/yG4SRYpKDoozg6iiw666tS1i4NDBxEXF3FwKHTQ
	TUGH4hSVYoxN8gx9bV7eu+/nYNGeM36WA0cOl71M7IbDsjv5bFr0VsF5GZXf3Wx0Kt4DTBl02rD5
	87uVSvuFVZluvF44fiq+kSMoOlD8ulIIwtVeP/q88NIls6141DYIIEgdbF0YjKNrc19XPqXGSL3i
	ta78r9BjMVgOxsfSZlIw6iaym5UtlnhLheCDrYIRxSoGs8PwjDdc4jrF+agPDkzF8xHDCQTLK5Y5
	yV0K5AAYkM0NnnOPVZT3LNLkISETFAAHmd83vFLr8JjfLDHDA+YY7jAY0K20do77hDwh4TZn6AMK
	lAJwMCwIA84z4ikXuEwXgyIk3XIIzo9XjrS/RK0eFznEUboIHkttu5NZNbmzv9bC/evzrQDhNNvs
	ASBlrfstPhiXMtlbHTVvXR1UEyeqyN8rmSk14rP2zqM8kebUD5tXC428+G84QOaGaXQgs/oHqAWl
	E5oYLZoAAAAASUVORK5CYII=
}]

set checkNo [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAAAAXNSR0IArs4c6QAAARhJREFUKM9t
	j71OAlEUhL9z9+4iKLhglKjREBMbYq/WFtLb+BYW+jQ2voSFrZ0voLFAY+NPNOFnIbBedu+1ICSL
	7Ex1zpnMmRFA8Aiqmgx0+mNIcCDIVf3weGuz5H4HfnEmMKpvtkcb9/UvTenkfO80PpKwlHEoEN45
	73PErQrqS13TUqGQpWPSkuixhlKmWp6kuAVaameDHXyNZ8Xh+A9H2lHLoKdDnmC60wAWWRDYWeHp
	2eY4wHgICvxi0nM5MaXW64MGhZW8DDJ7EflBTkjB4FCigvipUXmBdI6OkPdexbep1t/teH18QDHj
	Ihg+eHvej0gEv9y4vHj1kkBlKibD3bTZvr55iAVBr6yaNQpzQRLdGXWZ4P4Ai759ExxviKEAAAAA
	SUVORK5CYII=
}]

set radioOn [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAvNJREFUOMtV
	k89rXFUUxz/3vjfvzZtkZuIYEE1oLTOYQBBBRakuXFlcuaorhXZRXaSktYu6FhVTkNgMgn9A/4DB
	Ll1kI2hKMdCapIEMxCQu5kdiJy+TeT/um3uvi05h/G7Ol8M53805HwEIwPJMApCAA+RG1QAK0CM/
	Pvvcw2hxYmVl5fXD/f3VKIr+0npokySJj4+Pt5rNZr3RaLw2CmUsBEbNqWazuRTHUdQ/PbHHR13b
	7bbs0VHXnoYnNklie9LrxTs7OzcAdzxBAOXNzc2r1Wr17qB/ijUGhEAIgbUWrAUhmJgskipFq9X6
	cmFh4SfASMC/dWtpvlqtfn92GqKNRkhJUMhjraVQCBBSYqxhcNbH9zyKxck7jUajBggXKF279sVn
	mUoCYzRSOpwNzqivrtLcbfLy7Aw3rl9nslzEaI1KU7ycl5+bm1sCbrpA5dXzFz7o90MAfM+jXq9z
	/5f7qDRlc2uLLE1ZvrNMFEUkKqE0WaLb6XwIODKXY8LzvZrWGoxlkMTs7jZRaYqUEqM129tPiOIY
	YyxGa1zPJdPDC4AjASeNE7ACbQ2FfJ5z58+Ry+We3VZKqrUqQRBgrUECaaLQWgPgZhlq//Bw/6Xp
	6fk0TYgGA27f/gprLY8fPaZWq/L1N98y6PexxuL4Ht1uF6XUAWBdoL++/sejTy5fnlcqRg8Nbg5W
	fryLMQYpJcedDslQI4TEdXO02m2SOP4N0A5gw/BpdvHiex9XKtOu1hkqU5ydhiRxTNj7F5UpBIJ8
	ENA5OqLXe5r+ubHx+draWtsBsoODf9KpF8rezMzsO5XKiziORA81epgBAs/z8PMBrVabvb09TsLw
	u8XFxQZgnBEQ8cOHv/9dnCxi4Q2LcIrlKYLCBFYI2u02W9tP6Hbaqhf2lq9cufoDkAJWjL103vd5
	5d2333/r0keXPp0ql9/0/WBWSpFZRCvLsvX1Bw9+vvfrvQ06RM9JFPxfDlAASp5HyVo8kZEp6AMh
	EAHDcRr/A+2ogOpCjTT3AAAAAElFTkSuQmCC
}]

set radioOff [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAAAAXNSR0IArs4c6QAAAWpJREFUKM9t
	kU9LVFEYxn/nnHuvNneYpmEqEsK1umnjsoWbVi6CCJe56SP0QfoC1kIlQtAClwaBi2gztEkQElHT
	uTpCNjrX8d5zHhdN/8T3t3ufF57n5TEYBBgsjhhH4AJPGGwFAJZ0fWb/Xd4uy/N2trI+QxUHgAFw
	1Pfncv1UR4c60olyHcxxi4jBTX37dU+HaitTpkxtZTrT7isaWLAMLT2++6xLAIQQIE5pzL6fpoKx
	1B486eMH4m88fSaeUsVYGvcm8yuyEH3uTJqbuChOk6bn6ghP3IwqhbO4vAMi/IeA8w4ObHGx2Yqv
	sYjZaiFk6b75mPz54C8Ry2tljnfoc5hK748XhH8y3ODr2+erYZtTRxHc4sHDZHTMERCGhCG+LD1a
	8LvsERwij83i9+PNhm9W02Hf2fj0cv7FB7sTvtFHBjAMJyPFqL1t0hBZ78841k5970cPDdoCHBVq
	SU1JWajLCT3KX01dAj9AxUyj1kNqAAAAAElFTkSuQmCC
}]

} ;# namespace 16x16
} ;# namespace icon
} ;# namespace theme


bind Button <Map> {
	variable ::tk::Priv

	if {[info exists Priv(buttonWindow)] && $Priv(buttonWindow) eq "%W"} {
		# Fix bug in Button widget:
		# Reset this variable otherwise ::tk::ButtonEnter may
		# set the relief to sunken if the mouse hovers the
		# button during popup.
		set Priv(buttonWindow) ""
	}
}


# We fix a problem with tk::ButtonEnter; this function is not working
# well if it is called twice (this may happen under some circumstances).

rename tk::ButtonEnter tk::ButtonEnter_theme_orig_
rename tk::MbEnter     tk::MbEnter_theme_orig_
rename ttk::entry::CharSelect ttk::entry::CharSelect_theme_orig_
rename ttk::entry::WordSelect ttk::entry::WordSelect_theme_orig_
rename ttk::entry::LineSelect ttk::entry::LineSelect_theme_orig_

namespace eval tk {

proc ButtonEnter {w} {
	variable ::tk::Priv

	if {$Priv(window) ne $w} {
		ButtonEnter_theme_orig_ $w
	}
}


# we have to tweak the Alt key handling
proc FindAltKeyTarget {path char} {
	switch -- [winfo class $path] {
		Button - Label - Menubutton - TButton - TLabel - TCheckbutton - TRadiobutton {
			if {[string equal -nocase $char [string index [$path cget -text] [$path cget -underline]]]} {
				return $path
			}
			return
		}
		TNotebook {
			foreach tab [$path tabs] {
				set i [$path index $tab]
				set text [$path tab $i -text]
				set ul [$path tab $i -underline]
				if {[string equal -nocase $char [string index $text $ul]]} {
					ttk::notebook::ActivateTab $path [$path index $i]
					return {}
				}
			}
			set target [FindAltKeyTarget [$path select] $char]
			if {$target ne ""} {
				return $target
			}
		}
	}

	foreach child [concat [grid slaves $path] [pack slaves $path] [place slaves $path]] {
		set target [FindAltKeyTarget $child $char]
		if {$target ne ""} {
			return $target
		}
	}

	return {}
}


# we have to tweak the Alt key handling
proc AltKeyInDialog {path key} {
	set target [tk::FindAltKeyTarget $path $key]
	if {[llength $target] == 0} { return }

	switch [winfo class $target] {
		TButton {
			ttk::button::activate $target
		}
		Menubutton {
			tk::MbPost $target
			tk::MenuFirstEntry [$target cget -menu]
		}
		default {
			event generate $target <<AltUnderlined>> -when head
		}
	}
}


# fix tk::spinbox::AutoScan
rename ::tk::spinbox::AutoScan ::tk::spinbox::AutoScan_buggy_

proc spinbox::AutoScan {w} {
	if {[winfo exists $w]} { return [AutoScan_buggy_ $w] }
}

} ;# namespace tk


namespace eval ttk {

if {[info tclversion] < "8.6"} {

	proc spinbox {args} { ::spinbox {*}$args }

	# we don't like text selection while an arrow is pressed
	bind Spinbox <1> {+
		switch -exact [%W identify %x %y] {
			buttonup - buttondown {
				%W selection clear
			}
		}
	}

} else {

	namespace eval spinbox {

	# - ttk::spinbox takes the focus in state "readonly"; this is misplaced!
	# - we don't like text selection while an arrow is pressed
	proc Press {w x y} {
		$w instate disabled { return }
		$w instate !readonly { focus $w }
		switch -glob -- [$w identify $x $y] {
			uparrow - downarrow { $w selection clear }
		}
		switch -glob -- [$w identify $x $y] {
			*textarea	{ ttk::entry::Press $w $x }
			*rightarrow	-
			*uparrow 	{ ttk::Repeatedly event generate $w <<Increment>> }
			*leftarrow	-
			*downarrow	{ ttk::Repeatedly event generate $w <<Decrement>> }
			*spinbutton {
				if {$y * 2 >= [winfo height $w]} {
					set event <<Decrement>>
				} else {
					set event <<Increment>>
				}
				ttk::Repeatedly event generate $w $event
			}
		}
	}


	# should not select if readonly!
	proc SelectAll {w} {
		$w instate !readonly {
			$w selection range 0 end
			$w icursor end
		}
	}


	# - we have to fix value adjustment (who has tested ttk::spinbox?)
	# - we don't like text selection while an arrow is pressed
	proc Spin {w dir} {
		set nvalues [llength [set values [$w cget -values]]]
		set value [$w get]
		if {$nvalues} {
			set current [lsearch -exact $values $value]
			set index [Adjust $w [expr {$current + $dir}] 0 [expr {$nvalues - 1}]]
			$w set [lindex $values $index]
		} else {
			if {[catch {set v [expr {[scan [$w get] %f] + $dir*[$w cget -increment]}]}]} {
				set v [$w cget -from]
			}
			$w set [FormatValue $w [Adjust $w $v [$w cget -from] [$w cget -to]]]
		}
		uplevel #0 [$w cget -command]
	}

	} ;# namespace spinbox

} ;# [info tclversion] >= "8.6"


namespace eval entry {

# ttk::entry takes the focus in state "readonly"; this is misplaced!
proc Press {w x} {
    variable State

    $w icursor [ClosestGap $w $x]
    $w selection clear
    $w instate {!readonly !disabled} { focus $w }

    # Set up for future drag, double-click, or triple-click.
    set State(x) $x
    set State(selectMode) char
    set State(anchor) [$w index insert]
}


# should not select if readonly!
proc LineSelect {w from to} {
	$w instate !readonly {
		LineSelect_theme_orig_ $w $from $to
	}
}


# should not select if readonly!
proc CharSelect {w from to} {
	$w instate !readonly {
		CharSelect_theme_orig_ $w $from $to
	}
}


# should not select if readonly!
proc WordSelect {w from to} {
	$w instate !readonly {
		WordSelect_theme_orig_ $w $from $to
	}
}

} ;# namespace entry


namespace eval combobox {

# The implementation of ttk::combobox is a bit clumsy.
# The textarea, but only the textarea, should have a
# xterm cursor.

bind TCombobox <Motion>	+[namespace code { CBMotion %W %x %y }]

proc CBMotion {w x y} {
	variable Priv

	set cursor [$w cget -cursor]

	if {[$w identify $x $y] eq "textarea"} {
		if {[$w cget -state] eq "normal" && $cursor ne "xterm"} {
			$w configure -cursor xterm
		}
	} elseif {[llength $cursor]} {
		$w configure -cursor {}
	}
}

# # we want the invocation with a space key-press, but only if
# #   - it is empty
# #   - or the insertion cursor is at position 0
# #   - or the insertion cursor is at end of selection, and the
# #     selection starts at position 0
# bind TCombobox <KeyPress-space> {
# 	if {	[string length [%W get]] == 0
# 		|| [%W index insert] == 0
# 		|| ([%W index sel.first] == 0 && [%W index insert] == [%W index sel.last])} {
# 		ttk::combobox::Post %W
# 	} else {
# 		ttk::entry::Insert %W " "
# 	}
# }

if {[tk windowingsystem] eq "x11"} {

# Bug in Tk: Mouse wheel is generating a button press event.
# This shouldn't unpost the list box.
bind ComboboxPopdown <ButtonPress> {
	if {%b <= 3} { ttk::combobox::Unpost [winfo parent %W] }
}


# We like to bind the mouse wheel to the list box.
bind ComboboxListbox <ButtonPress-4> {
	%W yview scroll -1 units
	after idle { ttk::combobox::LBHover %W %x %y }
}

bind ComboboxListbox <ButtonPress-5> {
	%W yview scroll +1 units
	after idle { ttk::combobox::LBHover %W %x %y }
}

bind ComboboxListbox <Any-Key> {
	if {[string is alnum -strict "%A"]} {
		set key  [string toupper "%A"]
		set cb [ttk::combobox::LBMaster %W]
		set values [$cb cget -values]
		set i [$cb current]
		if {$i >= 0} {
			while {$i < [llength $values] && [string index [lindex $values $i] 0] eq $key} { incr i }
			set i [lsearch -glob -start $i $values ${key}*]
		}
		if {$i == -1} { set i [lsearch -glob $values ${key}*] }
		if {$i >= 0} {
			%W activate $i
			%W selection clear 0 end
			%W selection set $i
			%W see $i
		}
	}
}

bind ComboboxPopdown <ButtonPress-4> { %W.l yview scroll -1 units }
bind ComboboxPopdown <ButtonPress-5> { %W.l yview scroll +1 units }

}

} ;# namespace combobox


namespace eval theme::clam {

ttk::style theme settings clam {
	ttk::style map TCombobox -foreground [list disabled $colors(-disabledfg)]
}

} ;# namespace theme::clam


#namespace eval notebook {
#
# make keyboard traversal in notebooks comfortable
# (otherwise "-takefocus 0" have to be set in all children of each tab)
#proc ActivateTab {w tab} {
#	if {[$w index $tab] eq [$w index current]} {
#		focus $w
#	} else {
#		$w select $tab
#	}
#}
#
#} ;# namespace notebook
} ;# namespace ttk

# vi:set ts=3 sw=3:
