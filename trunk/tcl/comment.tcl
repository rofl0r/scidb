# ======================================================================
# Author : $Author$
# Version: $Revision: 28 $
# Date   : $Date: 2011-05-21 14:57:26 +0000 (Sat, 21 May 2011) $
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
# Copyright: (C) 2009-2011 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

namespace eval comment {
namespace eval mc {

set CommentEditor				"Comment Editor"
set Language					"Language"
set AllLanguages				"All languages"
set AddLanguage				"Add language..."
set SwitchLanguage			"Switch language"
set FormatText					"Format text"

set Bold							"Bold"
set Italic						"Italic"
set Underline					"Underline"

set InsertSymbol				"&Insert Symbol..."
set MiscellaneousSymbols	"Miscellaneous Symbols"
set Figurine					"Figurine"

} ;# namespace mc

namespace import ::dialog::choosecolor::getActualColor
namespace import ::dialog::choosecolor::rgb2hsv
namespace import ::dialog::choosecolor::hsv2rgb

array set Vars {
	widget:text	{}
	dialog		{}
	key			{}
	comment		{}
	content		{}
	langSet		{}
	after			{}
	format		{}
	wantedLang	{}
	countryList	{}
	lang			xx
	count			0
	undo			0
	redo			0
}

array set Fonts {
	normal		TkTextFont
	bold			{}
	italic		{}
	bold-italic	{}
}

array set Options {
	useComboBox 1
	menuColumns 3
}

variable Geometry {}

array set NagSet {
	prefix	{	140 141 142 143}
	suffix	{	  3   4   5   6   7   8
					 13  14  15  16  17
					147 148 149 150 153 154
					157 159 160 165 166 167
					173 174 175 176 178 179 180 181 182 183 184}
}

variable DingbatSet {
	"\u00bd" "\u2212" "\u2022" "\u2139" "\u20a4" "\u20ac" "\u270e" "\u263a" "\u2605" "\u25cf"
}
variable Colors {darkgreen darkred darkblue darkgreen darkred darkblue}

set Symbols $font::figurines(graphic)
foreach section {prefix suffix} {
	foreach nag $NagSet($section) {
		set c $::font::SymbolUtfEncoding($nag)
		if {[string length $c] == 1} {
			append Symbols $c
		}
	}
}


# Currently the undo/redo mechanism of the text widget is not working properly
# (and quite useless). The Tk team does not like to handle this problem (see
# bug item #3192483).
variable UndoIsWorking 0


proc open {parent lang} {
	variable UndoIsWorking
	variable Annotations
	variable Geometry
	variable Fonts
	variable Vars

	set dlg $parent.__comment__
	set Vars(dialog) $dlg
	toplevel $dlg -class Scidb
	wm withdraw $dlg

	set top [frame $dlg.top]
	set bg [$top cget -background]
	destroy $top

	set top [ttk::frame $dlg.top]
	pack $dlg.top -fill both -expand yes
	bind $dlg <<Language>> [namespace code [list LanguageChanged $dlg %W]]
	bind $dlg <Alt-Key> [list tk::AltKeyInDialog $dlg %A]

	set Vars(widget:text) $top.text
set Vars(key) [::scidb::game::position key]
	set Vars(lang) xx

	text $top.text \
		-height 6 \
		-width 0 \
		-background white \
		-font $Fonts(normal) \
		-wrap word \
		-setgrid 1 \
		-yscrollcommand [list ::widget::sbset $top.sb] \
		-undo $UndoIsWorking \
		-maxundo 0 \
		;
	ttk::scrollbar $top.sb -command [namespace code [list ::widget::textLineScroll $top.text]]

	set font $Fonts(normal)
	foreach {attr args} {bold bold italic italic bold-italic {bold italic}} {
		set f $Fonts($attr)
		if {[llength $f] == 0} {
			set f [list [font configure $font -family] [font configure $font -size] {*}$args]
		}
		$top.text tag configure $attr -font $f
		if {$attr eq "bold"} { $top.text tag configure codeb -font $f }
	}
	# XXX possibly we can use only {Scidb Symbol Traveller}
	$top.text tag configure figurine -font $::font::figurine
	$top.text tag configure symbol -font $::font::symbol
	$top.text tag configure symbolb -font $::font::symbolb
	$top.text tag configure underline -underline true

	bind $top.text <ButtonPress-3>	 [namespace code [list PopupMenu $top.text]]
	bind $top.text <Any-Button>		 [list $top.text configure -cursor xterm]
	bind $top.text <Any-Button>		+[list ::tooltip::tooltip hide]
	bind $top.text <Tab>					 "focus \[tk_focusNext $top.text]; break"
	bind $top.text <Shift-Tab>			 "focus \[tk_focusPrev $top.text]; break"
	bind $top.text <<Modified>>		 [list set [namespace current]::Vars(redo) 0]

	set butts [ttk::frame $top.buttons]
	ttk::button $butts.symbol \
		-style aligned.TButton \
		-compound left \
		-image $::icon::iconBlackPawn \
		-command [namespace code [list PopupSymbolTable $butts.symbol $top.text]] \
		;
	ttk::button $butts.clear \
		-style aligned.TButton \
		-compound left \
		-image $icon::iconClear \
		-command [namespace code Clear] \
		;
	ttk::button $butts.revert \
		-style aligned.TButton \
		-compound left \
		-image $icon::iconReset \
		-command [namespace code [list Revert $dlg]] \
		;
	set Vars(lang:label) [LanguageName]
	set Vars(widget:label) $butts.lang
	ttk::label $butts.lang \
		-compound left \
		-textvar [namespace current]::Vars(lang:label) \
		-image $::country::icon::flag([::mc::countryForLang $lang]) \
		-justify left \
		-relief ridge \
		-background $bg \
		-padding {2 2 2 2} \
		;
	::widget::buttonSetText $butts.symbol [namespace current]::mc::InsertSymbol
	::widget::buttonSetText $butts.clear ::widget::mc::Clear
	::widget::buttonSetText $butts.revert ::widget::mc::Revert

	grid $top.text		-row 1 -column 1 -sticky ewns
	grid $top.sb		-row 1 -column 2 -sticky ns
	grid $top.buttons	-row 1 -column 4 -sticky ewns

	grid columnconfigure $top {0 3 5} -minsize $::theme::padding
	grid columnconfigure $top 1 -weight 1
	grid rowconfigure $top {0 2} -minsize $::theme::padding
	grid rowconfigure $top 1 -weight 1

	grid $butts.lang		-row 1 -column 1 -sticky ew
	grid $butts.symbol	-row 3 -column 1 -sticky ew
	grid $butts.clear		-row 5 -column 1 -sticky ew
	grid $butts.revert	-row 7 -column 1 -sticky ew

	grid rowconfigure $butts 2 -minsize $::theme::padY
	grid rowconfigure $butts {4 6} -minsize $::theme::pady
	grid rowconfigure $butts 2 -weight 1

	::widget::dialogButtons $dlg {ok apply cancel} ok
	bind $dlg <Return> {}

	$dlg.apply	configure -command [namespace code Apply]
	$dlg.ok		configure -command [namespace code [list Ok $dlg]]
	$dlg.cancel	configure -command [namespace code [list Close $dlg]]

	set tb [::toolbar::toolbar $dlg -float 0 -side left -allow {left top bottom}]
	set Vars(tb) [::toolbar::toolbar $dlg -float 0 -side top -allow {left top bottom}]

	foreach format {Bold Italic Underline} {
		set fmt [string tolower $format]
		set Vars(format:$fmt) 0
		::toolbar::add $tb checkbutton \
			-image [set icon::toolbar$format] \
			-tooltipvar [namespace current]::mc::$format \
			-variable [namespace current]::Vars(format:$fmt) \
			-command [namespace code [list ChangeFormat $fmt]] \
			;
	}
	set Vars(addLang) [::toolbar::add $Vars(tb) button \
		-image $::icon::toolbarPlus \
		-tooltipvar [namespace current]::mc::AddLanguage \
		-command [namespace code [list PopupChooseLanguage $dlg]] \
	]
	::toolbar::addSeparator $Vars(tb)
	::toolbar::add $Vars(tb) button \
		-image [::country::makeToolbarIcon ZZX] \
		-command [namespace code { SwitchLanguage xx }] \
		-tooltipvar [namespace current]::mc::AllLanguages \
		-variable [namespace current]::Vars(lang) \
		-value xx \
		;

	::update idletasks
	bind $dlg <Configure> [namespace code [list RecordGeometry $dlg $parent]]
	scan [wm grid $dlg] "%d %d" w h
	wm minsize $dlg $w $h
	wm title $dlg "$::scidb::app: $mc::CommentEditor"
	wm transient $dlg $parent
	wm resizable $dlg true true
	wm protocol $dlg WM_DELETE_WINDOW [namespace code [list Close $dlg]]
	if {[llength $Geometry] == 4} {
		scan [wm geometry [winfo toplevel $parent]] "%dx%d+%d+%d" tw th tx ty
		set rx [expr {$tx + [lindex $Geometry 0]}]
		set ry [expr {$ty + [lindex $Geometry 1]}]
		set rw [winfo reqwidth $dlg]
		set rh [winfo reqheight $dlg]
		set sw [winfo screenwidth $dlg]
		set sh [winfo screenheight $dlg]
		set rx [expr {max(min($rx, $sw - $rw), 0)}]
		set ry [expr {max(min($ry, $sh - $rh), 0)}]
		set x0 [expr {max(0, [lindex $Geometry 2])}]
		set y0 [expr {max(0, [lindex $Geometry 3])}]
		wm geometry $dlg ${x0}x${y0}+${rx}+${ry}
	}

	Init $parent $lang
	wm deiconify $dlg
	focus $top.text
	ttk::grabWindow $dlg
	tkwait window $dlg
	ttk::releaseGrab $dlg
}


proc AddLanguageButton {lang} {
	variable Vars

	set countryCode [::mc::countryForLang $lang]

	if {[info exists ::encoding::mc::Lang($lang)]} {
		set languageNameVar ::encoding::mc::Lang($lang)
	} else {
		variable _LanguageName_$lang
		set languageNameVar _LanguageName_$lang
	}

	set w [::toolbar::add $Vars(tb) button \
		-image [::country::makeToolbarIcon $countryCode] \
		-command [namespace code [list SwitchLanguage $lang]] \
		-tooltipvar $languageNameVar \
		-variable [namespace current]::Vars(lang) \
		-value $lang \
	]
	set Vars(tb:$lang) $w
}


proc MakeLanguageButtons {} {
	variable Vars

	set langButtons [array names Vars tb:*]
	
	foreach button $langButtons {
		::toolbar::remove $Vars(tb) $Vars($button)
		unset Vars($button)
	}

	foreach lang $Vars(langSet) {
		AddLanguageButton $lang
	}
}


proc Accept {} {
	variable Vars

	set Vars(content) [ParseContent $Vars(lang)]
	set Vars(comment) [::scidb::misc::xmlFromList $Vars(content)]
	::scidb::game::update comment $Vars(key) $Vars(comment)
	$Vars(widget:text) edit reset
	$Vars(widget:text) edit modified no
	set Vars(redo) 0
}


proc Close {dlg} {
	variable Vars

	destroy $dlg
	array unset Vars tb:*
	set Vars(langSet) {}
}


proc Clear {} {
	variable Vars

	set w $Vars(widget:text)

	if {[$w count -chars 1.0 end] >= 1} {
		$w delete 1.0 end
		focus $w
		set Vars(count) 0
	}
}


proc Revert {dlg} {
	variable Vars

	set w $Vars(widget:text)

	if {[$w edit modified]} {
		$w delete 1.0 end
		InsertComment $Vars(lang)
		focus $w
	}
}


proc Apply {} {
	variable Vars

	Accept
	focus $Vars(widget:text)
}


proc Ok {dlg} {
	Apply
	Close $dlg
}


proc RecordGeometry {dlg parent} {
	variable Geometry

	scan [wm geometry $dlg] "%dx%d+%d+%d" fw fh fx fy
	scan [wm geometry [winfo toplevel [winfo toplevel $parent]]] "%dx%d+%d+%d" tw th tx ty
	scan [wm grid $dlg] "%d %d %d %d" bw bh wi hi
	set Geometry [list [expr {$fx - $tx}] [expr {$fy - $ty}] $fw $fh]
}


proc Init {parent lang} {
	variable Vars

	MakeCountryList

	set Vars(comment) [::scidb::game::query comment]
	set Vars(langSet) [::scidb::game::query langSet]

	SwitchLanguage $lang
	MakeLanguageButtons
	Update
}


proc Update {{setup 1}} {
	variable UndoIsWorking
	variable Vars

	set w $Vars(widget:text)
	$w delete 1.0 end

	if {$UndoIsWorking} {
		$w configure -undo no
	}
	array unset Vars content:*

	if {$setup} {
		set Vars(content) [::scidb::misc::xmlToList $Vars(comment)]
	}

	foreach entry $Vars(content) {
		lassign $entry lang comm
		if {$lang eq ""} { set lang xx }
		set Vars(content:$lang) $comm
		if {$lang eq $Vars(lang)} {
			InsertComment $lang
		}
	}

	if {$UndoIsWorking} {
		$w configure -undo yes
		set Vars(redo) 0
	}
	$w edit reset
	$w edit modified no
	UpdateFormatButtons $w
}


proc InsertComment {lang} {
	variable Vars

	if {![info exists Vars(content:$lang)]} { return }

	set flags 0
	set underline 0
	set w $Vars(widget:text)
	set Vars(symbols) {}
	set Vars(count) 0

	foreach comment $Vars(content:$lang) {
		lassign $comment code text
		set text [string map {"<brace/>" "\{" "\n" "\u00b6\n"} $text]

		switch -- $code {
			str {
				switch $flags {
					0 { set attrs {} }
					1 { set attrs bold }
					2 { set attrs italic }
					3 { set attrs bold-italic }
				}
				if {$underline} { lappend attrs underline }
				$w insert end $text $attrs
			}

			sym { InsertFigurine $w $text }
			nag { InsertNag $w $text [expr {$flags & 1}] }

			+bold			{ incr flags +1 }
			-bold			{ incr flags -1 }
			+italic		{ incr flags +2 }
			-italic		{ incr flags -2 }
			+underline	{ set underline 1 }
			-underline	{ set underline 0 }
		}
	}
}


proc InsertFigurine {w fig} {
	variable Vars

	set Vars(symbol:[incr Vars(count)]) $fig
	set selrange [$w tag ranges sel]
	set text [string map $::font::pieceMap $fig]
	set key key$Vars(count)
	set tags [list figurine $key]
	$w tag bind $key <Enter> {}
	$w tag bind $key <Leave> {}

	if {	[llength $selrange]
		&& [$w compare insert >= [lindex $selrange 0]]
		&& [$w compare insert <= [lindex $selrange 1]]} {
		$w replace {*}$selrange $text $tags
	} else {
		$w insert insert $text $tags
	}
}


proc InsertNag {w nag {bold {}}} {
	variable Vars

	if {[llength $bold] == 0} { set bold $Vars(format:bold) }
	set Vars(symbol:[incr Vars(count)]) $nag
	set key key$Vars(count)
	lassign [::font::splitAnnotation $nag] value sym tag
	if {$tag eq "symbol"} {
		if {$bold} { set tag symbolb }
	} else {
		if {$bold} { set tag codeb } else { set tag code }
		if {[string is digit -strict $sym]} {
			set sym "{\$$sym}"	;# use something like a question mark instead
		}
	}
	set tags [list $tag $key]
	set selrange [$w tag ranges sel]

	if {	[llength $selrange]
		&& [$w compare insert >= [lindex $selrange 0]]
		&& [$w compare insert <= [lindex $selrange 1]]} {
		$w replace {*}$selrange $sym $tags
	} else {
		$w insert insert $sym $tags
	}

	$w tag bind $key <Enter> [namespace code [list TooltipShow $w $key nag $value]]
	$w tag bind $key <Leave> [namespace code [list TooltipHide $w $key]]
}


proc InsertChar {w ch} {
	variable Vars

	if {$Vars(format:bold) && $Vars(format:italic)} {
		set fmt bold-italic
	} elseif {$Vars(format:bold)} {
		set fmt bold
	} elseif {$Vars(format:italic)} {
		set fmt italic
	} else {
		set fmt {}
	}
	if {$Vars(format:underline)} {
		lappend fmt underline
	}

	set selrange [$w tag ranges sel]

	if {	[llength $selrange]
		&& [$w compare insert >= [lindex $selrange 0]]
		&& [$w compare insert <= [lindex $selrange 1]]} {
		$w replace {*}$selrange $ch $fmt
	} else {
		$w insert insert $ch $fmt
	}
}


proc PasteText {w str} {
	variable Symbols

	set n [string length $str]
	set i 0

	while {$i < $n} {
		set m [expr {min($n - $i, 3)}]

		while {$m > 1} {
			set s [string range $str $i [expr {$i + $m - 1}]]
			if {[info exists ::font::mapCodeToNag($s)]} {
				InsertNag $w $::font::mapCodeToNag($s)
				incr i $m
				set m 0
			} else {
				incr m -1
			}
		}
		if {$m == 1} {
			set c [string index $str $i]
			set k [string first $c $Symbols]

			switch $k {
				-1 { $w insert insert $c }

				 0 { InsertFigurine $w K }
				 1 { InsertFigurine $w Q }
				 2 { InsertFigurine $w R }
				 3 { InsertFigurine $w B }
				 4 { InsertFigurine $w N }
				 5 { InsertFigurine $w P }

				default { InsertNag $w $::font::mapCodeToNag($c) }
			}

			incr i
		}
	}
}


proc ParseSelection {w} {
	variable Vars

	set dump [$w dump -tag -text sel.first sel.last]
	set token str
	set content ""
	set num 0

	foreach {key value index} $dump {
		switch $key {
			text {
				switch $token {
					str {
						if {[string length $value]} {
							if {[string index $value end-1] eq "\u00b6"} { set c "\n" } else { set c "" }
							append content [string map {"\u00b6" ""} $value] $c
						}
					}

					sym {
						append content [string map $::font::pieceMap $Vars(symbol:$num)]
					}

					nag {
						set nag $Vars(symbol:$num)
						set symbol [::font::mapNagToSymbol $nag]
						if {[string is digit -strict $symbol]} {
							append content "${"
							append content $symbol
							append content "}"
						} else {
							append content $symbol
						}
					}
				}
			}

			tagon {
				switch -glob $value {
					symbol*	{ set token nag }
					code*		{ set token nag }
					figurine	{ set token sym }
					key*		{ set num [string range $value 3 end] }
				}
			}

			tagoff {
				switch -glob $value {
					symbol* - code* - figurine { set token str }
				}
			}
		}
	}

	return $content
}


proc TooltipShow {w key type value} {
	variable Options

	::tooltip::show $w "$::annotation::mc::Nag($value) (\$$value)"
	$w configure -cursor question_arrow
}


proc TooltipHide {w key} {
	variable Options

	::tooltip::tooltip hide
	$w configure -cursor xterm
}


proc LanguageChanged {dlg w} {
	variable Vars

	if {$dlg eq $w} {
		wm title $dlg "$::scidb::app: $mc::CommentEditor"
	}

	set Vars(lang:label) [LanguageName]
	set Vars(countryList) {}

	MakeCountryList
}


proc SwitchLanguage {lang} {
	variable Vars

	foreach fmt {bold italic underline} {
		if {$Vars(format:$fmt)} { ToggleFormat $fmt }
	}

	if {$lang eq $Vars(lang)} { return }

	set Vars(content) [ParseContent $Vars(lang)]
	set Vars(lang) $lang
	set Vars(lang:label) [LanguageName]
	$Vars(widget:label) configure -image $::country::icon::flag([::mc::countryForLang $Vars(lang)])
	Update 0
}


proc ParseContent {lang} {
	variable Vars

	set w $Vars(widget:text)
	set dump [$w dump -tag -text 1.0 end]
	set count 0
	set fst 0
	set lst 0
	set n 1

	foreach {key value index} $dump {
		switch $key {
			text		{
				incr count $n
				set lst 1
				if {$fst == 0} { set fst 1 }
			}

			tagon {
				switch -glob $value {
					symbol* - code* - figurine {
						set n 0
						set lst 2
						if {$fst == 0} { set fst 2 }
					}
				}
			}

			tagoff {
				switch -glob $value {
					symbol* - code* - figurine { set n 1 }
				}
			}
		}
	}

	set n 0
	set token str
	set content {}
	set num 0
	set length 0
	array set flags { bold 0 italic 0 underline 0 }

	foreach {key value index} $dump {
		switch $key {
			text {
				if {$token eq "str"} {
					if {[incr n] == 1} {
						if {$fst == 1} { set value [string trimleft $value] }
					} elseif {$n == $count} {
						if {$lst == 1} { set value [string trimright $value] }
					}
				} else {
					set value $Vars(symbol:$num)
				}
				if {[string length $value]} {
					incr length [string length $value]
					lappend content [list $token $value]
				}
			}

			tagon {
				switch -glob $value {
					bold - italic - bold-italic - underline {
						foreach fmt [split $value -] {
							if {[incr flags($fmt)] == 1} {
								lappend content "+$fmt"
							}
						}
					}

					codeb - symbolb {
						if {[incr flags(bold)] == 1} {
							lappend content "+bold"
						}
						set token nag
					}

					key*				{ set num [string range $value 3 end] }
					code - symbol	{ set token nag }
					figurine			{ set token sym }
				}
			}

			tagoff {
				switch $value {
					bold - italic - bold-italic - underline {
						if {[incr flags($fmt) -1] == 0} {
							lappend content "-$fmt"
						}
					}

					symbolb - codeb {
						if {[incr flags(bold) -1] == 0} {
							lappend content "-bold"
						}
						set token str
					}

					code - symbol - figurine { set token str }
				}
			}
		}
	}

	set languages ""
	foreach name [array names Vars content:*] {
		lappend languages [string range $name 8 9]
	}
	if {$lang ni $languages} {
		lappend languages $lang
	}

	set result ""
	set content [string map {\u00b6 ""} $content]
	foreach l $languages {
		if {$l eq "xx"} { set code "" } else { set code $l }
		if {$lang eq $l} { set value $content } else { set value $Vars(content:$l) }
		lappend result [list $code $value]
	}

	return $result
}


proc PopupSymbolTable {w text} {
	variable NagSet
	variable DingbatSet
	variable Colors
	variable _Symbol

	set ncols 9
	set nrows 6
	set nsecs 7

	set parent [winfo toplevel $w]
	set m $parent.insert_symbol
	if {[winfo exists $m]} { return }
	menu $m -tearoff no
	bind $m <Escape> [list set [namespace current]::_Symbol {}]
	set top [frame $m.top]
	pack $top -padx $::theme::padding -pady $::theme::padding
	set size [expr {int(abs(double([font configure $::font::symbol -size])*2.0) + 0.5)}]
	set _Symbol {}
	set coords [list [expr {$size/2}] [expr {$size/2}]]
	set bg [$top cget -background]

	for {set x 0} {$x < $ncols} {incr x} {
		for {set y 0} {$y < $nrows} {incr y} {
			set canv $top.c_${x}_${y}
			canvas $canv -width $size -height $size -highlightthickness 1 -highlightbackground $bg
			bind $canv <Enter> [list $canv configure -background #ffdd76]
			bind $canv <Leave> [list $canv configure -background $bg]
			grid $canv -row $y -column $x
		}
	}

	set x 0
	set y 0
	set s 0

	foreach piece {K Q R B N P} {
		set canv $top.c_${x}_${y}
		if {![info exists first($s)]} { set first($s) $canv }
		set last($s) $canv
		set section($canv) $s
		set sym [string map $::font::pieceMap $piece]
		$canv create text {*}$coords -font $::font::figurine -text $sym
		foreach seq {space Return ButtonPress-1} {
			bind $canv <$seq> [namespace code [list set _Symbol [list fig $piece]]]
		}
		if {[incr y] == $nrows} {
			set y 0
			incr x
		}
	}

	set color 0
	set s 1

	foreach type {prefix suffix} {
		set ranges $::annotation::sections($type)

		foreach range $::annotation::sections($type) {
			lassign $range descr from to
			if {$from == 7} { set from 3}
			set nagSet $NagSet($type)
			set incr 0

			for {set nag $from} {$nag <= $to} {incr nag} {
				if {$nag in $nagSet} {
					lassign [::font::splitAnnotation $nag] value sym
					set canv $top.c_${x}_${y}
					if {![info exists first($s)]} { set first($s) $canv }
					set last($s) $canv
					set section($canv) $s
					$canv create text {*}$coords \
						-font $::font::symbol \
						-text $sym \
						-fill [lindex $Colors $color] \
						;
					foreach seq {space Return ButtonPress-1} {
						bind $canv <$seq> [namespace code [list set _Symbol [list nag $nag]]]
					}
					set tip "[string toupper $::annotation::mc::Nag($nag) 0 0] (\$$nag)"
					::tooltip::tooltip $canv $tip
					if {[incr y] == $nrows} {
						set y 0
						incr x
					}
					set incr 1
				}
			}

			if {$incr} {
				incr color $incr
				incr s
			}
		}
	}

	foreach c $DingbatSet {
		set canv $top.c_${x}_${y}
		$canv create text {*}$coords -text $c
		if {![info exists first($s)]} { set first($s) $canv }
		set last($s) $canv
		set section($canv) $s
		foreach seq {space Return ButtonPress-1} {
			bind $canv <$seq> [namespace code [list set _Symbol [list sym $c]]]
		}
		if {[incr y] == $nrows} {
			set y 0
			incr x
		}
	}

	for {set x 0} {$x < $ncols} {incr x} {
		for {set y 0} {$y < $nrows} {incr y} {
			set canv $top.c_${x}_${y}

			if {$section($canv) == 0} {
				set prior 0
			} else {
				set prior [expr {$section($canv) - 1}]
			}
			if {$section($canv) == $nsecs - 1} {
				set next [expr {$nsecs - 1}]
			} else {
				set next [expr {$section($canv) + 1}]
			}

			if {$x > 0} {
				set lt [expr {$x - 1}]_${y}
			} elseif {$y > 0} {
				set lt [expr {$ncols - 1}]_[expr {$y - 1}]
			} else {
				set lt [expr {$ncols - 1}]_[expr {$nrows - 1}]
			}
			if {$x < [expr {$ncols - 1}]} {
				set rt [expr {$x + 1}]_${y}
			} elseif {$y < $nrows - 1} {
				set rt 0_[expr {$y + 1}]
			} else {
				set rt 0_0
			}
			if {$y > 0} {
				set up ${x}_[expr {$y - 1}]
			} elseif {$x > 0} {
				set up [expr {$x - 1}]_[expr {$nrows - 1}]
			} else {
				set up [expr {$ncols - 1}]_[expr {$nrows - 1}]
			}
			if {$y < $nrows - 1} {
				set dn ${x}_[expr {$y + 1}]
			} elseif {$x < $ncols - 1} {
				set dn [expr {$x + 1}]_0
			} else {
				set dn 0_0
			}

			bind $canv <Left>  [list focus $top.c_$lt]
			bind $canv <Right> [list focus $top.c_$rt]
			bind $canv <Up>    [list focus $top.c_$up]
			bind $canv <Down>  [list focus $top.c_$dn]
			bind $canv <Prior> [list focus $first($prior)]
			bind $canv <Next>  [list focus $first($next)]
			bind $canv <Home>  [list focus $top.c_0_0]
			bind $canv <End>   [list focus $last([expr {$nsecs - 1}])]
		}
	}

	foreach b {1 2 3} {
		bind $top <ButtonPress-$b> [namespace code [list ExitInsertSymbol $top %X %Y]]
	}

	::tooltip::tooltip on $top*
	wm withdraw $m
	wm transient $m $parent
	util::place $m below $w
	wm deiconify $m
	raise $m
	if {[tk windowingsystem] == "x11"} {
		tkwait visibility $m
		::update
	}
	focus -force $top.c_0_0
	ttk::globalGrab $top
	vwait [namespace current]::_Symbol
	ttk::releaseGrab $top
	destroy $m
	::tooltip::tooltip on

	if {[llength $_Symbol]} {
		switch [lindex $_Symbol 0] {
			fig { InsertFigurine $text [lindex $_Symbol 1] }
			nag { InsertNag $text [lindex $_Symbol 1] }
			sym { InsertChar $text [lindex $_Symbol 1] }
		}
	}

	catch { focus -force $text }
}


proc ExitInsertSymbol {w x y} {
	if {![string match $w* [winfo containing $x $y]]} {
		set [namespace current]::_Symbol {}
	}
}


proc PopupMenu {parent} {
	variable UndoIsWorking
	variable Vars

	set m $parent.langMenu
	catch { destroy $m }
	menu $m -tearoff no

	if {[llength [$parent tag ranges sel]] == 0} {
		set state disabled
	} else {
		set state normal
	}

	menu $m.symbol -tearoff no
	$m add cascade \
		-compound left \
		-image $icon::12x12::blackPawn \
		-label [::menu::stripAmpersand $mc::InsertSymbol] \
		-menu $m.symbol \
		;
	MakeSymbolMenu $parent $m.symbol

	menu $m.format -tearoff no
	$m add cascade \
		-compound left \
		-image $icon::12x12::format \
		-label " $mc::FormatText" \
		-menu $m.format \
		-state $state \
		;
	foreach format {Bold Italic Underline} {
		set fmt [string tolower $format]
		$m.format add command \
			-compound left \
			-image [set icon::12x12::text-$fmt] \
			-label [set mc::$format] \
			-command [namespace code [list ChangeFormat $fmt]] \
			;
	}

	if {$UndoIsWorking} {
		if {[$Vars(widget:text) edit modified]} { set state normal } else { set state disabled }
		$m add separator
		$m add command \
			-compound left \
			-image $::icon::12x12::undo \
			-label $::mc::Undo \
			-command [namespace code Undo] \
			-state $state \
			;
		if {$Vars(redo)} { set state normal } else { set state disabled }
		$m add command \
			-compound left \
			-image $::icon::12x12::redo \
			-label $::mc::Redo \
			-command [namespace code Redo] \
			-state $state \
			;
	}

	if {[llength $Vars(langSet)]} { set state normal } else { set state disabled }
	$m add separator
	menu $m.switch -tearoff no
	$m add cascade \
		-compound left \
		-image $::icon::12x12::none \
		-label " $mc::SwitchLanguage" \
		-menu $m.switch \
		-state $state \
		;
	if {$Vars(lang) eq "xx"} { set state disabled } else { set state normal }
	$m.switch add command \
		-compound left \
		-image $::country::icon::flag([::mc::countryForLang xx]) \
		-label " $mc::AllLanguages" \
		-command [namespace code [list SwitchLanguage xx]] \
		-state $state \
		;

	foreach lang $Vars(langSet) {
		if {$lang eq $Vars(lang)} { set state disabled } else { set state normal }
		$m.switch add command \
			-compound left \
			-image $::country::icon::flag([::mc::countryForLang $lang]) \
			-label " [::encoding::languageName $lang]" \
			-command [namespace code [list SwitchLanguage $lang]] \
			-state $state \
			;
	}

	MakeLanguageMenu $m.languages
	$m add cascade \
		-compound left \
		-image $::icon::12x12::plus \
		-label " [lindex [split $mc::AddLanguage .] 0]" \
		-menu $m.languages \
		;
	$m add separator
	$m add command \
		-compound left \
		-image $::icon::12x12::clear \
		-label " [::menu::stripAmpersand $::widget::mc::Clear]" \
		-command [namespace code Clear] \
		;
	$m add command \
		-compound left \
		-image $::icon::12x12::reset \
		-label " [::menu::stripAmpersand $::widget::mc::Revert]" \
		-command [namespace code [list Revert [winfo toplevel $parent]]] \
		;
	
	tk_popup $m {*}[winfo pointerxy $parent]
}


proc MakeSymbolMenu {w menu} {
	variable NagSet
	variable DingbatSet

	set m [menu $menu.figurine -tearoff 0]
	$menu add cascade -menu $m -label $mc::Figurine

	foreach {fig} {K Q R B N P} {
		$m add command \
			-font $::font::figurine \
			-label [string map $::font::pieceMap $fig] \
			-command [namespace code [list InsertFigurine $w $fig]] \
			-columnbreak [expr {$fig eq "B"}] \
			;
	}

	foreach type {prefix suffix} {
		set ranges $::annotation::sections($type)
		set m [menu $menu.$type]

		if {[llength $ranges] == 1} {
			bind $m <<MenuSelect>> [namespace code { ::widget::menuItemHighlightSecond %W }]
		}

		foreach range $ranges {
			lassign $range descr from to
			if {$from == 7} { set from 3}
			set text [set ::annotation::mc::$descr]
			set nagSet $NagSet($type)

			if {[llength $ranges] == 1} {
				$menu add cascade -menu $m -label $text
				set sub $m
			} else {
				set sub [menu $m.[string tolower $descr 0 0]]
			}

			set nags {}
			for {set nag $from} {$nag <= $to} {incr nag} {
				if {$nag in $nagSet} {
					set symbol [::font::mapNagToSymbol $nag]
					$sub add command -label $symbol -command [namespace code [list InsertNag $w $nag]]
					lappend nags $nag
				}
			}

			if {[llength $nags] == 0} {
				destroy $sub
			} else {
				if {[llength $ranges] > 1} {
					bind $sub <<MenuSelect>> [namespace code { ::widget::menuItemHighlightSecond %W }]
					$menu add cascade -menu $sub -label $text
				}
				set columnbreak 1

				foreach nag $nags {
					$sub add command \
						-label [string toupper $::annotation::mc::Nag($nag) 0 0] \
						-command [namespace code [list InsertNag $w $nag]] \
						-columnbreak $columnbreak \
						;
					set columnbreak 0
				}
			}
		}
	}

	set m [menu $menu.misc -tearoff no]
	$menu add cascade -menu $m -label $mc::MiscellaneousSymbols
	set i -1
	foreach c $DingbatSet {
		$m add command \
			-label $c \
			-command [namespace code [list InsertChar $w $c]] \
			-columnbreak [expr {[incr i] == [llength $DingbatSet]/2}] \
			;
	}
}


proc MakeLanguageMenu {path} {
	variable Vars
	variable Options

	set m $path
	catch { destroy $m }
	menu $m -tearoff no

	set size [expr {([llength $Vars(countryList)] + $Options(menuColumns) - 1)/$Options(menuColumns)}]
	for {set i 1} {$i < $Options(menuColumns)} {incr i} { lappend breaks [expr {$i*$size}] }
	set n 0

	foreach entry $Vars(countryList) {
		lassign $entry flag name lang
		set opts {}
		if {$lang in $Vars(langSet)} { lappend opts -state disabled }
		if {$n in $breaks} { lappend opts -columnbreak 1 }
		$m add command {*}$opts \
			-command [namespace code [list NewLanguage $lang]] \
			-compound left \
			-image $flag \
			-label " $name" \
			;
		incr n
	}

	return $path
}


proc PopupChooseLanguage {dlg} {
	variable Options

	if {$Options(useComboBox)} {
		PopdownLaguages $dlg
	} else {
		PopupLanguageMenu $dlg
	}
}


proc PopupLanguageMenu {dlg} {
	variable Vars
	variable Options

	MakeLanguageMenu $dlg.langMenu

	scan [winfo geometry $Vars(addLang)] "%dx%d+%d+%d" tw th tx ty
	set x [expr {[winfo rootx $Vars(addLang)]}]
	set y [expr {[winfo rooty $Vars(addLang)] + $th}]

	tk_popup $dlg.langMenu $x $y
}


proc PopdownLaguages {dlg} {
	variable Vars

	set popdown $dlg.popdown
	toplevel $popdown -class AddLanguagePopdown
	wm withdraw $popdown

	switch -- [tk windowingsystem] {
		default -
		x11 {
			wm overrideredirect $popdown true
		}
		win32 {
			wm overrideredirect $popdown true
			wm attributes $popdown -topmost 1
		}
		aqua {
			tk::unsupported::MacWindowStyle style $popdown help {noActivates hideOnSuspend}
			wm resizable $popdown 0 0
		}
	}

	if {[info tclversion] >= "8.6"} {
		set borderwidth [::ttk::style lookup ComboboxPopdownFrame -borderwidth]
	} else {
		 switch -- [tk windowingsystem] {
			x11	{ set borderwidth 1 }
			win32	{ set borderwidth 1 }
			aqua	{ set borderwidth 0 }
		 }
	}

	$popdown configure -borderwidth $borderwidth -relief solid

	lappend listopts \
		-height 10 \
		-usescroll yes \
		-relief flat \
		-highlightthickness 0 \
		-selectmode browse \
		-borderwidth 1 \
		-showfocus 0 \
		-exportselection no \
		-disabledforeground grey60 \
		-disabledbackground white \
		;
	if {[info tclversion] >= "8.6"} {
		lappend listopts -style ComboboxPopdownFrame
	}

	set lb [::tlistbox $popdown.l {*}$listopts]

	bind $lb <<ItemVisit>> 		[namespace code [list Activate $lb %d]]

	$lb bind <KeyPress-Escape>	[namespace code [list UnpostPopdown $popdown]]
	$lb bind <KeyPress-Return>	[namespace code [list Selected $popdown]]
	$lb bind <ButtonRelease-1>	[namespace code [list Selected $popdown]]
	$lb bind <Any-KeyPress>		[namespace code [list Search $popdown %A %K]]

	$lb addcol image	-id flag	-width 18 -justify left
	$lb addcol text	-id name

	pack $lb -fill both

	set first -1
	set index 0
	foreach entry $Vars(countryList) {
		set lang [lindex $entry 2]
		if {$lang in $Vars(langSet)} {
			set enabled no
		} else {
			set enabled yes
			if {$first == -1} { set first $index }
		}
		$popdown.l insert $entry -enabled $enabled
		incr index
	}

	$popdown.l configure -cursor {}
	$popdown.l resize
	$popdown.l see 0

	set index [lsearch -index 2 $Vars(countryList) $Vars(wantedLang)]
	if {$index >= 0 && $Vars(wantedLang) ni $Vars(langSet)} {
		$popdown.l select $index
	} elseif {$first >= 0} {
		$popdown.l select $first
	}

	::update idletasks
	::util::place $popdown below $Vars(addLang)
	switch -- [tk windowingsystem] {
		x11 - win32 { wm transient $popdown $dlg }
	}
	wm attribute $popdown -topmost 1
	set Vars(focus) [focus]
	wm deiconify $popdown
	raise $popdown
}


proc UnpostPopdown {popdown} {
	variable Vars

	catch { focus -force $Vars(focus) }
	grab release $popdown	;# in case of stuck or unexpected grab
	destroy $popdown
}


proc MakeCountryList {} {
	variable Vars

	if {[llength $Vars(countryList)]} { return }

	set list {}
	foreach lang [array names ::mc::langToCountry] {
		if {$lang ne "xx"} {
			set country $::mc::langToCountry($lang)
			set flag $::country::icon::flag($country)
			set name [::encoding::languageName $lang]
			lappend list [list $flag $name $lang]
		}
	}

	set Vars(countryList) [lsort -index 1 -dictionary $list]
}


proc Activate {lb data} {
	if {[lindex $data 0] eq "enter"} {
		$lb select [lindex $data 2]
	}
}


proc Selected {popdown args} {
	variable Vars

	set index [$popdown.l curselection]

	if {$index >= 0} {
		set lang [lindex $Vars(countryList) $index 2]
		UnpostPopdown $popdown
		NewLanguage $lang
	}
}


proc NewLanguage {lang} {
	variable Vars

	if {$lang ni $Vars(langSet)} {
		lappend Vars(langSet) $lang
		SwitchLanguage $lang
		MakeLanguageButtons
	}
}


proc Search {popdown code sym} {
	if {![string is alpha -strict $code]} { return }
	$popdown.l search name $code [mc::mappingForSort] [mc::mappingToAscii]
}


proc LanguageName {} {
	variable Vars

	if {$Vars(lang) eq "xx" } { return $mc::AllLanguages }
	return [::encoding::languageName $Vars(lang)]
}


proc ChangeFormat {format} {
	variable Vars

	set w $Vars(widget:text)
	set selrange [$w tag ranges sel]
	ToggleFormat $format
	if {[llength $selrange] == 0} { return }

	lassign $selrange prevIndex lastIndex
	array set flags {bold 0 italic 0 underline 0 symbol 0 symbolb 0 figurine 0}
	set flags($format) $Vars(format:$format)

	foreach {key value index} [$w dump -tag 1.0 ${lastIndex}+1c] {
		if {[$w compare $prevIndex <= $index] && [$w compare $index <= $lastIndex]} {
			if {$prevIndex ne $index} {
				set range [list $prevIndex $index]
				if {$flags(symbol) || $flags(symbolb)} {
					if {$flags(symbol) && $flags(bold)} {
						$w tag remove symbol {*}$range
						$w tag add symbolb {*}$range
					} elseif {$flags(symbolb) && !$flags(bold)} {
						$w tag remove symbolb {*}$range
						$w tag add symbol {*}$range
					}
				} elseif {!$flags(figurine)} {
					if {$flags(bold) && $flags(italic)} {
						set fmt bold-italic
					} elseif {$flags(bold)} {
						set fmt bold
					} elseif {$flags(italic)} {
						set fmt italic
					} else {
						set fmt {}
					}
					if {$format eq "underline"} {
						if {$flags(underline)} { set cmd add } else { set cmd remove }
						$w tag $cmd underline {*}$range
					} else {
						foreach f {bold italic bold-italic} {
							if {$f eq $fmt} { set cmd add } else { set cmd remove }
							$w tag $cmd $f {*}$range
						}
					}
				}
			}
			set prevIndex $index
		}
		if {$value ne "sel"} {
			foreach fmt [split $value -] {
				if {$fmt ne $format} {
					switch $key {
						tagon  { incr flags($fmt) +1 }
						tagoff { incr flags($fmt) -1 }
					}
				}
			}
		}
	}
}


proc ToggleFormat {format} {
	variable Vars

	set Vars(format:$format) [expr {!$Vars(format:$format)}]

	if {$Vars(format:bold) && $Vars(format:italic)} {
		set Vars(format) bold-italic
	} elseif {$Vars(format:bold)} {
		set Vars(format) bold
	} elseif {$Vars(format:italic)} {
		set Vars(format) italic
	} else {
		set Vars(format) {}
	}

	if {$Vars(format:underline)} {
		lappend Vars(format) underline
	}
}


proc Undo {} {
	variable Vars

	catch {
		$Vars(widget:text) edit undo
		incr Vars(redo)
	}
}


proc Redo {} {
	variable Vars

	catch {
		$Vars(widget:text) edit redo
		if {$Vars(redo) > 0} { incr Vars(redo) -1 }
	}
}


proc UpdateFormatButtons {w} {
	variable Vars

	lassign {0 0 0} bold italic underline

	foreach fmt {bold italic bold-italic underline} {
		if {[llength [set range [$w tag prevrange $fmt insert]]]} {
			if {[$w compare [lindex $range 0] <= insert] && [$w compare [lindex $range end] >= insert]} {
				foreach f [split $fmt -] { set $f 1 }
			}
		}
	}

	if {[llength [set range [$w tag prevrange symbolb insert]]]} {
		if {[$w compare [lindex $range 0] <= insert] && [$w compare [lindex $range end] >= insert]} {
			set bold 1
		}
	}

	foreach fmt {bold italic underline} {
		if {$Vars(format:$fmt) != [set $fmt]} {
			ToggleFormat $fmt
		}
	}
}


proc WriteOptions {chan} {
	::options::writeItem $chan [namespace current]::Geometry
}


::options::hookWriter [namespace current]::WriteOptions


bind AddLanguagePopdown <Map>				{ ttk::grabWindow %W }
bind AddLanguagePopdown <Destroy>		{ ttk::releaseGrab %W }
bind AddLanguagePopdown <Map>				{+ focus -force %W }
bind AddLanguagePopdown <ButtonPress>	[namespace code { UnpostPopdown %W }]

switch -- [tk windowingsystem] {
	win32 {
		bind AddLanguagePopdown <FocusOut> [namespace code { UnpostPopdown %W }]
	}
}


namespace eval icon {
namespace eval 12x12 {

set format [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAQAAAD8fJRsAAAAAmJLR0QAAKqNIzIAAACDSURB
	VBgZBcHBDcFgGADQ93/9OQgVkfTCBKYQzq7SbbqPIcQOnaEXETbo570iAQCAisnDz8UZL087
	dwdGvYJBSgOK3lhdfSQASA/P8DZTUACYvQOoGhUABFA1lgAggCqsBQACqMLGAgABfGm3VrQA
	QNHvp1se06RXAOiM+zxlm9Kogz9QASBGCjU3BAAAAABJRU5ErkJggg==
}]

set bold [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAABmJLR0QA/wD/AP+gvaeTAAAB
	/ElEQVQoz2NggAIxn2kMUr6zGPjcJ3GK+0/jlwBifo9JnGLeMxhEgXIYwCF/FYNOwkKeqjnH
	ZvatvXiqd82FU5Wzj87UBYqB5DBA3qSDDAntOx1nbrvxftLmm/97N9z4P3nTtffxQLHcSQdQ
	FWvFzgGSdkx1805Mmbr15v+SWSd/5U8/8atr7fX/lbOPTWFgcGLSBKuBgoDKtQzeJSvVetdc
	vNO55tr/tN59uxK79u6qXnz5f+Pis3c8ileq+ZSvgSjWjZ7O8PTzf4ac/l2FXWuu/G1YevFn
	ZOPmqPD6TVElc87+LJ9//m9Sx/bCE3e/MejFAD1vkjCDQSdyklDVnKOHm1de/V+34ORz/9Jl
	8f5ly+JLZx57njvz/P/M/v2HdaImCRnGz2BgyOjeyhBes8qvev6ZrxULL/9vX3Xlb82Cc78r
	5539Xbv40t/EiWf/J/Ue+epTstwvon4D0E0a6Wzp3TsWF84+979oxskfmf37Hqb37buf0r33
	fkLH7odx3Yd/hLYf/x9Su2kxm3E2G4NtylTrvIn7nuVPP/U/pnHdDmnLOH1Z83BtINaRtYo1
	CKhYtTOy48j/yOYdz8wTJlszGEX1mvkVL6gKKFlQbxJS4w20UxyI5YBYHoglDQIqfdxz5za4
	ZM+t1o/oMUOPP0YgZgFiVihmgYrBAQDiS+PUwvneJgAAAABJRU5ErkJggg==
}]

set italic [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAABmJLR0QA/wD/AP+gvaeTAAAA
	CXBIWXMAAABIAAAASABGyWs+AAABz0lEQVQoz3WRT0hTcQDHv7+3t7fH3t6eFCxKsokpxpAw
	IZAaZY0OJRnkoSI0CGok7iCFp+iPFIQgQYcuBRUEHTqsQ0Gn5aXwkASryVthw6eu/2vl5t77
	/euSVCO/ty98PvCFL8FqMUbQdn4PPj6xA34zoAc0P2/eYFXIavyB0TTKP2vWzs6mMSukx50P
	31+nJ+0z6v/gnuEHeDUzrw32dZ0zgvppKqGtW2sK16U+pR7eOnAbmRtJ9Cdigw3h4FBu9lPm
	a6la8ignChH4R9jUPYBro8dx4ur9/esj1qXi5x+ZbH7ugq75lj3KAUH/EtpTOHIyiZv3HnVu
	3hgZr9bobO7t4ojwvKKqgHiUgUj2R+g/HMeLlzNNsdbGcSFFNGsXnvoV1tIYCfUQQHcpA5EU
	KgDsTt7Cu8Ki1b1tyxUrHN7LOZextuaLXIBICSJVDZVq6b0CBrXj6ASm39j+Q/viZw0j2JvL
	F9KMiy9SSgghQTk3LWtNn0s5FMmgZp899/UeO3jKNI2UM79w5/HdictLztTy76UsmkhFt+9I
	7KKUKYJ7UFu6OkLfSmVO3dpYfnry4ZIzJQCYK0KlaLsLTut1CVJWiazVP00A1H8jAMiV8gt+
	1MXS+opBggAAAABJRU5ErkJggg==
}]

set underline [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAABmJLR0QA/wD/AP+gvaeTAAAB
	/UlEQVQoz2NgQAJOBasZbHJWigXVbPIDYVsg27lgDQNWoBW7iOH///8MWRP2F/StvfSjd82l
	H5n9+wtAYlqxCzE1OOStZNCNny/cvvzc0ap5p76Xzzn5o3nJ2aP6CQuEQXIYoHT6IYbY5q2B
	E9df+ZHSs3dxYtee5d2rL/2IbtoaWDT1EKpinZiZQDeVsZXPPLy8d/XFr37la118K9b5tC67
	8AOoeDmDfgUbWA0M+JWtZHAvWGLcsfzsq+o5R3cLOrfwKgb2CZVMP3KsdsHpl675S4y9S6DO
	Uo3qB3s2oXVL88QNV/8XTtl3Ib17x4zUrh0z8ybtu9qy7NL/iIZNzSA1qtH9DAzG8VMYDKIm
	SBdOOXCpZdmF/3ULTv2pmnvyX8Xsk//KZp/8Uzr7zP/k7j2XDKMnShvHTgF6dtoBhoCyZYnl
	s4/9jqxft9Q+Y7aXa858b7fcBd5OmXO9gqvWLEvvP/Tbq2hJYlbfXgYGTtMcroDSJcsS2zY/
	tIjtsd4FtBoImEEY5AzT6C6biPoNj7wKFy3jMc/mYjBLXWDgXrh8tXfJik3+5asmBpSt6Pcv
	XTopAIiDKlb0h1atnhhYsWqzb+nKNRZAtQwWaQstA6o3rI1o3LoxvGHLlojGLVsiG7duiWra
	uiW6eduWuNbtW5I6dmyMaty81ipjkSUAF6P0pGax/3YAAAAASUVORK5CYII=
}]

} ;# namespace 12x12

namespace eval 16x16 {

set bold [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABVlBMVEUAAAAyY6IhTIkrWZcp
	VpQzZKMtW5ojT4wwYJ8wYJ8tW5onVJE0ZaMhTIkiTIovXJksWpkjTowkT4wzYZ81Y580Yp80
	Yp8iTIoiTYonU5IpVZI2ZqQ1ZqQiTYsmU5EqV5UrVpEsWJYtW5gtW5ktXJwvXp4yYJ08aaU/
	a6ZKcaZLdaxih7gvXZs0ZKE6ZqBnkMB9nsaCoslAb613lr4gSocjT4wpVpQzZKM2Z6k+cbVB
	bqxDd71IfsVJf8ZKfcBKgMZLfb5LgMZMfLpPg8dUh8lWiMlYispZi8pci8Vdjsteib9ejstg
	kMtjks1kjcBli7xmkspsl81ulcNumtBvk8BxnM9ymst1m8t1n9J+pNGFp9CIrdWJq9OJrdmM
	sNaNsdiUr8+bud6duNehvtyow+GqwNmswdusxd+wx+Wzxt2/z+DF1uzG1uzQ3e3a5fHn7vbo
	7/f///8Rrh6xAAAANHRSTlMAAgYGBxAdLUNEUVp1g4WVl7e31NTg4enp7u75+/z8/Pz8/Pz8
	/Pz8/Pz8/P39/f39/f7+kq+VdwAAAAFiS0dEca8HXOIAAAC1SURBVBjTY2DABlhlzIFAhhUu
	wGOYX1CQb8AD4zNK56bo6ydnSjNCBTi08+J0daPTtDihAiKp6UFion4xIcIQPpteVrwGF5d6
	aLgOG1iA3zgpTJmZWcnDx0gAxGdRTIj0MgMCJ1c7eRagALdmVHBEbGJGdo6jrSo3UEDKP9DX
	293F2d7W1tZSkoGBXSHAU80UDFRsbeTYGYSs3BxkmUCGMcla21oIMkiYmJjwQuznAzLFGQgC
	AOD6HwqbpJRFAAAAAElFTkSuQmCC
}]

set italic [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABFFBMVEUAAAAzZpkkSZIkbZIk
	SYYuUZcuaKIpXJkhSIUsWZYyZKEoWJUgTYktXJklUI4xYZ4yYJ8qVZI2ZaEjToowXpwsWJU3
	ZqMhTIkjT4wtWZY2ZqU4aKYuWpghS4kzX5s1ZqQmU5InUo8qWJctWpdBbKVBbqdDaqFQfbNU
	fLJZgLNdgrN2mMJFcq1Kd7FtksB9nMaXstMAAAAgSochTIkjTowlUY4nU5EpVpQrWJYsW5ku
	XZwwYJ4yYqEzZKNJf8ZKgMZOe7hViMlhkMxmlM1ok8ppls5smM92n9N4otKCo82DqNiOr9mP
	r9aSstyWttyYuN2ZtNabut2iveGlv+Kuxd+yx+G4zubD1OjD1enH2OvN3O3d6PMxeUQbAAAA
	MXRSTlMABAYGERISFCcnJzFHR3p6obe30uLm5vn5+fn5+/z8/P39/f39/f39/f39/f7+/v7+
	081UkAAAAAFiS0dEMdnbHXIAAACWSURBVBgZhcFVEoIAAAXAZ3cHdncPWIBgd3fd/x4KKn45
	7gK/6cMc58OX2n+4XzlIFO7KOX1qQWLP35LslsWHJXvLWJkJgzdT4lJyyugRjRdd6DgNaNDs
	NSFSeTeLuAFodBsQyF3ldbH+FGvXIbDlVv3heLbc7fkansypeaEqivBVAMbooONRQkDxFAAH
	SQa1EBEkgf8eDm8VVQEEaFIAAAAASUVORK5CYII=
}]

set underline [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABGlBMVEUAAAAzZKMrWZghTIkp
	V5UzZKMtXJojT4wxYaAxYaAtXJonVJI0ZaMiTIkiTIorWZcsWpctW5ovXJojTowlUIw1ZKA1
	ZKAiTIojTYonU5IqVpM3Z6U1ZqQiTYsjTosmU5EqWJYsWpctXJwuW5ouXJowW5UyYJ82Y582
	ZKE+aqNCbqdIc6pKda1agbQwX5w2ZaM7aKFYfa95m8ZGc65Kd7BfhraGo8mGp80gSocjT4wn
	VJIzZKM2Z6k+cbVDd71Ida9IfsVXhsJai8pejMZul8pxm9Bxm9J1msV5otN8o9R9pNR+ocp/
	ocuDqdODqdeOsNqZtdWcud+gvd6mv9ypwuOqwduyyOG1yuK1zOTD1OjP3e3R4O7d5/P///9E
	N4MfAAAAOHRSTlMAAgUGBxAdLUNET1p1g4WTk5WXt7fU3unp7u75+/z8/Pz8/Pz8/Pz8/Pz8
	/Pz8/f39/f3+/v7+/vySt1gAAAABYktHRF2d3zABAAAAn0lEQVQYGWXBAxLDUAAFwJcqtVPb
	Nn5t27z/OZqkmml3AY7YUas5xPhS5u73nBIflP1SLp/sFN5kyVs6fU5I8cYc1ibTbMTgRZK5
	xuXy2CIlwZO+tAsIBP5+QQeeyHssagFNvucRgaPIbqNVVqQbUoBjm08G4+Vqsx82rGDRwWm4
	wvO13TQAY6vjEoIldDbrBgAWQlTgqQkx4x/5AfLjAcGCG/gUWrLrAAAAAElFTkSuQmCC
}]

} ;# namespace 16x16

namespace eval 22x22 {

set bold [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABYAAAAWCAMAAADzapwJAAAAA3NCSVQICAjb4U/gAAAB5lBM
	VEX///8AAAAAAAAAAAAAAABAQIBAgL8AAAAAAAAAAAA5VaozZpkzZqYAAAAAAAAAAAAAAAAT
	LkkSK0QqWI00Y6EtWpMzZqUrVo4xYJszYJszYp81ZqUsWZAuWpA0Y6A3ZaI0YZw0ZaQvW5Qx
	XZQ3aKc6aacsVosrVoswXpgzX5k2ZaQ2ZqQ4aKY2Z6YqVIorVIo0Yp5Bcq1Dcq5NdKguWpI0
	X5U9bKdDcKlOerJbhLc3Z6RCbqk1ZaA8aaI9bapIdbBMeLEzYZ4zYp40Yp41Y55wk751mMR9
	nsktWI8uWI87a6pId7RKebVQerFSfraNq9BplMUqVIksV44vXJY0ZaRAc7VDd7tJfL5Jf8VJ
	f8ZLgMZNgcdOgsdRhchShchUh8lViMlXiclYhsBZicdai8pajMpejstfjslfj8xgkMxhkMxk
	k81lkcdmkMRnkcVnlM5nlc5olc5pls1qk8VtmdBtms9umc9umtBvmcpwm85zntF1n9F4oNN6
	odR6o9N7nsl7o9N9pNV+pdWCqNSCqdWFqteIrNiIrdaKrteMr9mUtdubud6hvuCjvuGkvt2k
	v+GowN2qw+SrxeKuxuGxyuWyyea2zOe6z+m60Oi/0ufD1ezI2ezM2+/U4fDV4vHa5vPc5/Pd
	6PMXWD0AAAAAUXRSTlMAAQIDBAQEBQYHCRQUFxodHiYpMTFVVXx9fX19p6enp6ir0NDQ0NTV
	7e3t7e3w8fHy8vLz9PT09PT09fX29vb29vf39/f4+Pj7+/z8/Pz8/f4Jio7oAAABA0lEQVQY
	02NgoB7g0Q0BAV0eVGFptwULFyyY7y6NIsqmM3NSe0Bz12QdNmRhMct5HcUhuXU9FmLIwqoT
	prQYKRpWN+WpIImKWM/tqNDm0sopK7ISQQjL+89ocJBikLIvzPaRg4tyu8zurHTmZuB2Ss9I
	duSGCUu6TqvyUwAyFLxTEu0koKKsmn1t5ebBIGCWEBeuwQoRFjXuL63p7p04dfqsObXRkQai
	EGHl+saC7Ky01MT42JioyMhQJbCosGlrfpJnEAR4hEVGmAiDhGV9SzJtxaH2iNtERnrJgFhq
	gYGB+oK8fPwCAvx8QnpAnjojRA0jEzMLOycnJwc7CxMjI9ViCwCcDT9SXVu0DQAAAABJRU5E
	rkJggg==
}]

set italic [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABYAAAAWCAYAAADEtGw7AAAABHNCSVQICAgIfAhkiAAAABl0
	RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAKiSURBVDiN1ZXbS1RRFMa/fc6c
	Oecw5TVtsBTTNPKCjPcJQSoMCgyDwQIfSjIstUCI6B8I6qWXDCLJJKI0rSwyJcVUQkZNIwTD
	pJREysQY56LOnHP27kEnvNdYPrRgva3942N9a69FGGPYjOA2hbqZYN1GH2aW1GYSjm+jlMkA
	wPNkouuGxfhX4Iyy2lhBJ7wqP54ix+zwx/SMgosV7dsX1/jcivTSOqOO8J1njiZsCfKT8LBz
	FIq6cgB8Amedfr5Vz3EdlgN7gqPDAsmtZwNzbdZh2FzKxsGpxbcFZlCas5MjIjPjwnR3Ggc9
	YxPTwwBgc24QTAiIXgyqSdodmpy7L0r/oPWjOvRl8pvGuP0A8MPhAdVU38Hm8/XXdxkDDxfk
	7JWevhmlbz+M21SqZnVXHJuimoophweM+gg2lz4uD/GTi4tyE+WWvnF09o+4mMqye26eGAMA
	RlVM2lcqXnfczGWP8g2yeKU4zyT3Dk2iuWtoRlGUQ32VBYPeGqopcM6qYNrSPq+pOL2kLlvQ
	CdVn80zyp68OPGkbmNVUxdJXWWBdXMc0FW6FgtI/UJx2rjZB4LkXp44kyk4PQ0v3Z7fi8chg
	9GVyYRUYowCjYIyC4/g5t8Ik9rtWpJfWhPME7fkH4wyCoMf7kWmkxEeK5qQoiAIHUeAhChwk
	PQ9Z4OFWqXT/9RjWBZsKqwNEWerIyYgO2BZgIFUNVpfN7jD8UkjpErWMUhBeN7czJlWiVFkd
	HHOhSQyW9K3GYP+w2PBQ/l5jr8tut9f03y0qWssHAIi3XJv/z8v2OvEuelNhdRohrMerhmhq
	07uGSyfhdHIAeMwb7TWbLqQWZ7k6AMZCQMj3wfrLEQBUxphGVrsghBACYK0EALYAZt5ky0Cr
	gv9F/H+n6SdBUTVgr3VVGQAAAABJRU5ErkJggg==
}]

set underline [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABYAAAAWCAYAAADEtGw7AAAABHNCSVQICAgIfAhkiAAAABl0
	RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAL3SURBVDiNtZRdSFRBFMf/M3eu
	7vVu6appSqAbIkEfC0ViFoX50HMPFUEuIRlREAiBtfUY1UOQBKWCICQiRQ++9GYfIJJlZVYr
	VOoqZbbrmtayu+7e+ejBlNaP1ZQOHOYczsyP/5lhDlFK4X8YW25D6dm2a4LLS7O5xuj17rvH
	PcudI8kUu9wtpmnqgevnytMc9lQEf03jcv2zSCRs5fTdqwwnA9NkxVSDuEu25RMQgoZH/Uoq
	gh1FOUS30crlFCcFM43VHtxVaLzs96Onz0e63n/D9uJ8Q2f04qrBJWdaK/Kz7NkbHCYevxwM
	AXjY2Tsc0vUUpNlSs0rOtFasCsxAPIfKitJ6B4KY/Bm27JZVGQpFrM9fJrEpL9skUEkfcFGw
	q7rJqWuqrDDPQZ68GIgKYdU9bT45LYRV5x0YjdoMkyguylzVTc5/AqdIVrNv52Zt8FsII6MB
	Ahavn2kjXj8enCDTFocj00F1SWtWDHa5W0yleFVxQa7+/P2wAGT7q8bTQQB41Xg6qKRsD/jH
	RXpGVooUssrlbjFXpliF3cWFuZiKCPR/9MVEnN9IKHN+I+AfiynKYDMMBRV2LwZe8PMYRW2x
	c5M5NPYL3OKpUsm3rhN3EvYQTkUsFoc9PcceDYdqAdTP5yT8PNfx2xUZGevaKw6U2n+E4tjo
	sIFpFLpGoGsUOqNI+eOd3gkMByIY870LyXjscF/b+cdLKiaMejY7C8yhrxPoffOaS6WWnCWE
	EJ5TsJWtz8yzTwVGPAASwHOKXUdvOsGYd8/e/YbX641OTgSvfbhfc3Up8LZjt66kpWd6svKK
	jNFPPVEi+Na+Bxd8CxRzJbbAkkbnsw5QQuMWIw1LQQHA0mRDZGr8QnjSbwAwALUFwBw46XRb
	iyUdQmuxuavYXdX0nQuZu1oQodT/pvnUxgVgi/Pck0fKoQBIqSAkIKRaJJ7J58edXd0JoubA
	FPA33+9YtWJKiT+hg9nHI4RQANpfzua5BkAB4ADEnzUhVkqJWfBvCddcPEUucyUAAAAASUVO
	RK5CYII=
}]

} ;# namespace 22x22

namespace eval 32x32 {

set bold [image create photo -data {
	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
	fIAAAAAASUVORK5CYII=
}]

set italic [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABHNCSVQICAgIfAhkiAAABOhJ
	REFUWMPtV2tsVEUYPXPn7qv7KgUKtGCRRxFapN1SK8RATZEKAoKhEIWgPCz6w1eMwRoT0Whi
	QkQ0aGJSBBWlacU/opGHoCQCFQoJsICAIF0LWKS73bW77d2Z+fyxu3X74tnGP0wymXtvzsw5
	c2bmm+8Cd8r/XFhfDTxpZdULhhTrONciiW9KkU5EprrKxe28el+QF67cXDI80/XWK4uKwBiz
	AQAIiAqJZ9ds74DtdQGe5V+MGuh2Vj0zz+M6eNaPYNgAiMFi1jBxZL8ueK03yQvKq90uu3XX
	cws8A841tiAYjqL26AW0RgWkJBCh7wSUlVVzm1ltL5+bP6w5otAYaMORE3+Gz/uuQkiCUASp
	VN8JaOgfrXyseEyew2HV6q+EUd/QJH6vb6wnUjBxDqkIQioQqd4XMKl884tFuUPnj7s73XK6
	IYRAKEJ13j98gmQpiGDRNSgFCKHQeR1uW0Dhss+mZWU4V8+YPNpx3BeEFAJ79nuvtInWB4FI
	M5GCzcIhhUJU9LIDnuWVo1Ld1i1LHvG4vb4QrDrDd3uP+VvaonOObFhxAQCICDYLh2IahCJQ
	bzlQUP6J26abdq94dOKAs3+F4bKb8MNerz8SaVtxdOOy2nYgKdjMHEKo+BL0ggNlZdWck23n
	k7M8Gf6IQmqKGT8dOBW6/Hfgg4OVS79JxhIRUsw6opIgu3HglgLReWdo4+zJY+8121I4mAbv
	mYbW385f/vbQp8vf7AD0A7DG9oCQCkZU3L4DE5dWvuQZkzFv9PAhlkhUIRgKiV21pw5lBR1L
	usMTKVhNHFIBxu0ew/wlH08bOsj1RvF92Y7GoIH+dk6bt/1aj5CcVVOzQHbfKxYBhSQIQbFL
	4VYE5Cz+aJTLYauaV5Ln9jUZGDkoBeurfr7SGglPratZ2dxTPyIFRRSPhl0duKE9kPfU+6kW
	ru95fGZh/4aAwPi7XFj3+Q74Ay3pmqadnrB4PQiMkVLW5H4GDFhMOogQi4TdBKLrCiguXq03
	k2XX/Ic8Q/wRDQUj05Bi4ah4eiaICATYiAAGBs41aFrMZcJ/XIklMKLy5h0IZg3aVFKYncPM
	du5rbMUl/2UoIigVO2KKYgSKCG1GzG4VfycClIo9mzmDkPzmHMhf9OHL40YMnpM1LNMaCAsU
	jXBAERCVErKTgMRzYvYq/o0IYAw4cKYFUXkTeyB34drp6QPcrxdNyHZeajbg1A2s2bAdTOOt
	Zp3HrJVSV6BrTsJi0jG39AEIqSAl3ZgD9yx8L9vpsG15eEp+an1TFOkOhq+/r20UimaaeOSc
	ET9wx756NQAw6ol8RNm7bodSAUMSCIhHwus4kFO2Ns2ss92zSwrTLjYrpDs1bNu5PxBq/eeJ
	UzWr6jqiK65/hEi1JyJSdb0L9I7kq81cZztKpxYMDkQ4+tk5fvylLth0tWnVia0V+wCkxDPp
	nrJpSq4tDV6Lfej49g0bS0h6XgLGuWPT/XnZ44YMTONEwL7DJ8M+38UvvVtfqwZgiwcuLUkE
	SyJOtCpeyWi+ZKXMHEhFyM20QEmRgHIAMvm/gI2d+87z4Nq6ZHVc0w4fr3l7OhBicWLeqU1U
	FR8wuVXuMVPsGbkzznW26eTWCnMCx7rJEZMH50kz1rpxoCf7VSc3VJI4kYS75p8RSxLFkkSw
	GxDQWQS63EJ3Srz8C63emhskwFOKAAAAAElFTkSuQmCC
}]

set underline [image create photo -data {
	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
	rkJggg==
}]

} ;# namespace 32x32

set toolbarBold [list $22x22::bold $16x16::bold $32x32::bold]
set toolbarItalic [list $22x22::italic $16x16::italic $32x32::italic]
set toolbarUnderline [list $22x22::underline $16x16::underline $32x32::underline]

} ;# namespace icon
} ;# namespace comment


rename tk_textPaste	tk_textPaste_comment_
rename tk_textCopy	tk_textCopy_comment_
rename tk_textCut		tk_textCut_comment_


proc tk_textPaste {w} {
	variable ::comment::Vars

	if {$w ne $Vars(widget:text)} { return [tk_textPaste_comment_ $w] }

	if {![catch {::tk::GetSelection $w CLIPBOARD} sel]} {
		global tcl_platform

#		set oldSeparator [$w cget -autoseparators]
#		if {$oldSeparator} {
#			$w configure -autoseparators 0
#			$w edit separator
#		}

		if {[tk windowingsystem] ne "x11"} {
			catch { $w delete sel.first sel.last }
		}

		set sel [string map {"\n" "\n\u00b6"} $sel]
		::comment::PasteText $w $sel

#		if {$oldSeparator} {
#			$w edit separator
#			$w configure -autoseparators 1
#		}
	}
}


proc tk_textCopy {w} {
	variable ::comment::Vars

	if {$w ne $Vars(widget:text)} { return [tk_textCopy_comment_ $w] }

	if {[llength [$w tag ranges sel]]} {
		set data [::comment::ParseSelection $w]
		clipboard clear -displayof $w
		clipboard append -displayof $w $data
	}
}


proc tk_textCut {w} {
	variable ::comment::Vars

	if {$w ne $Vars(widget:text)} { return [tk_textCut_comment_ $w] }

	if {[llength [$w tag ranges sel]]} {
		set data [::comment::ParseSelection $w]
		clipboard clear -displayof $w
		clipboard append -displayof $w $data
		if {[$w get sel.first] eq "\n"} { set decr -1c } else { set decr "" }
		if {[$w get sel.last] eq "\u00b6"} { set incr +1c } else { set incr "" }
		$w delete sel.first$decr sel.last$incr
	}
}


rename tk::TextInsert		tk::TextInsert_comment_
rename tk::TextButton1		tk::TextButton1_comment_
rename tk::TextSetCursor	tk::TextSetCursor_comment_
rename tk::TextUpDownLine	tk::TextUpDownLine_comment_
rename tk::TextPrevPara		tk::TextPrevPara_comment_
rename tk::TextNextPara		tk::TextNextPara_comment_
rename tk::TextNextPos		tk::TextNextPos_comment_
rename tk::TextPrevPos		tk::TextPrevPos_comment_

namespace eval tk {

proc TextInsert {w s} {
	variable ::comment::Vars

	if {$w ne $Vars(widget:text)} { return [TextInsert_comment_ $w $s] }

	if {$s eq "" || [$w cget -state] eq "disabled"} { return }

	set compound 0

	if {[llength [set range [$w tag ranges sel]]]} {
		if {[$w compare [lindex $range 0] <= insert] && [$w compare [lindex $range end] >= insert]} {
			set oldSeparator [$w cget -autoseparators]
			if {$oldSeparator} {
				$w configure -autoseparators 0
				$w edit separator
				set compound 1
			}
			$w delete [lindex $range 0] [lindex $range end]
		}
	}

	set c [$w get insert]

	if {$s eq "\n"} {
		set c [$w get insert]
		if {$c eq " "} {
			$w replace insert insert+1c "\u00b6\n"
		} elseif {$c eq "\n"} {
			$w insert insert+1c "\u00b6\n"
		} else {
			$w insert insert "\u00b6\n"
		}
	} else {
		if {$c eq "\n"} { $w mark set insert insert+1c }
		$w insert insert $s $Vars(format)
	}

	$w see insert

	if {$compound && $oldSeparator} {
		$w edit separator
		$w configure -autoseparators 1
	}
}


proc TextButton1 {w x y} {
	variable ::comment::Vars

	TextButton1_comment_ $w $x $y

	if {$w eq $Vars(widget:text)} {
		set c [$w get insert]
		if {$c eq "\n"} {
			$w mark set insert insert-1c
		}
		::comment::UpdateFormatButtons $w
	}
}


proc TextSetCursorExt {w pos {dir -1}} {
	variable ::comment::Vars

	TextSetCursor $w $pos

	if {$w eq $Vars(widget:text)} {
		if {$dir < 0} {
			if {[$w compare insert != 1.0]} {
				set c [$w get insert]
				if {$c eq "\n"} {
					$w mark set insert insert-1c
					$w see insert
				}
			}
		} else {
			if {[$w compare insert != end-1c]} {
				set c [$w get insert]
				if {$c eq "\n"} {
					$w mark set insert insert+1c
					$w see insert
				} else {
				}
			}
		}
	}
}


proc TextSetCursor {w pos} {
	variable ::comment::Vars

	TextSetCursor_comment_ $w $pos

	if {$w eq $Vars(widget:text)} {
		::comment::UpdateFormatButtons $w
	}
}


proc TextUpDownLine {w n} {
	variable ::tk::Priv
	variable ::comment::Vars

	set pos [TextUpDownLine_comment_ $w $n]
	if {$w eq $Vars(widget:text) && [$w compare $pos != 1.0]} {
		if {[$w get $pos] eq "\n"} { set pos [$w index $pos-1displayindices] }
		set Priv(prevPos) $pos
	}
	return $pos
}


proc TextPrevPara {w pos} {
	variable ::comment::Vars

	set pos [TextPrevPara_comment_ $w $pos]
	if {$w eq $Vars(widget:text) && [$w compare $pos != 1.0]} {
		if {[$w get $pos] eq "\n"} { set pos [$w index $pos-1displayindices] }
		set Priv(prevPos) $pos
	}
	return $pos
}


proc TextNextPara {w pos} {
	variable ::comment::Vars

	set pos [TextNextPara_comment_ $w $pos]
	if {$w eq $Vars(widget:text) && [$w compare $pos != 1.0]} {
		if {[$w get $pos] eq "\n"} { set pos [$w index $pos-1displayindices] }
		set Priv(prevPos) $pos
	}
	return $pos
}


proc TextNextPos {w start op} {
	variable ::comment::Vars

	set pos [TextNextPos_comment_ $w $start $op]
	if {$w eq $Vars(widget:text) && [$w compare $pos != 1.0]} {
		if {[$w get $pos] eq "\n"} { set pos [$w index $pos-1displayindices] }
	}
	return $pos
}


proc TextPrevPos {w start op} {
	variable ::comment::Vars

	set pos [TextPrevPos_comment_ $w $start $op]
	if {$w eq $Vars(widget:text) && [$w compare $pos != 1.0]} {
		if {[$w get $pos] eq "\n"} { set pos [$w index $pos-1displayindices] }
	}
	return $pos
}


proc TextBackSpace {w} {
	variable ::comment::Vars

	if {$w eq $Vars(widget:text)} {
		if {[$w tag nextrange sel 1.0 end] ne ""} {
			set c [$w get sel.last]
			if {$c eq "\u00b6"} { set incr +1c } else { set incr "" }
			set c [$w get sel.first]
			if {$c eq "\n" } { set decr -1c } else { set decr "" }
			$w delete sel.first$decr sel.last$incr
		} elseif {[$w compare insert != 1.0]} {
			set c [$w get insert-1c]
			if {$c eq "\n"} {
				if {	[$w compare insert-1c == 1.1]
					|| [string is space [$w get insert-3c]]
					|| [string is space [$w get insert]]} {
					$w delete insert-2c insert
				} else {
					$w replace insert-2c insert " "
				}
			} elseif {$c eq "\u00b6"} {
				if {	[$w compare insert == 1.1]
					|| [string is space [$w get insert-2c]]
					|| [string is space [$w get insert+1c]]} {
					$w delete insert-1c insert+1c
				} else {
					$w replace insert-1c insert+1c " "
				}
				if {[$w compare insert != end-1c]} {
					$w mark set insert insert-1c
				}
			} else {
				$w delete insert-1c
			}
			$w see insert
		}
	} else {
		if {[$w tag nextrange sel 1.0 end] ne ""} {
			$w delete sel.first sel.last
		} elseif {[$w compare insert != 1.0]} {
			$w delete insert-1c
			$w see insert
		}
	}
}


proc TextDelete {w} {
	variable ::comment::Vars

	if {$w eq $Vars(widget:text)} {
		if {[$w tag nextrange sel 1.0 end] ne ""} {
			set c [$w get sel.last]
			if {$c eq "\u00b6"} { set incr +1c } else { set incr "" }
			set c [$w get sel.first]
			if {$c eq "\n" } { set decr -1c } else { set decr "" }
			$w delete sel.first$decr sel.last$incr
		} else {
			set c [$w get insert]
			if {$c eq "\n"} {
				if {[$w compare insert == end-1c]} {
					# special case, seems to be Tk bug
					$w delete insert-2c insert
				} elseif {	[$w compare insert == 1.1]
							|| [string is space [$w get insert-2c]]
							|| [string is space [$w get insert+1c]]} {
					$w delete insert-1c insert+1c
				} else {
					$w replace insert-1c insert+1c " "
				}
			} elseif {$c eq "\u00b6"} {
				if {	[$w compare insert == 1.0]
					|| [string is space [$w get insert-1c]]
					|| [string is space [$w get insert+2c]]} {
					$w delete insert insert+2c
				} else {
					$w replace insert insert+2c " "
				}
				if {[$w compare insert != end-1c]} {
					$w mark set insert insert-1c
				}
				if {[$w get insert] eq "\n"} {
					$w mark set insert insert-1c
				}
			} else {
				$w delete insert
			}
			$w see insert
		}
	} else {
		if {[$w tag nextrange sel 1.0 end] ne ""} {
			$w delete sel.first sel.last
		} else {
			$w delete insert
			$w see insert
		}
	}
}


proc TextClear {w} {
	variable ::comment::Vars

	if {$w eq $Vars(widget:text)} {
		set c [$w get sel.last]
		if {$c eq "\u00b6"} { set incr +1c } else { set incr "" }
		set c [$w get sel.first]
		if {$c eq "\n" } { set decr -1c } else { set decr "" }
		$w delete sel.first$decr sel.last$incr
	} else {
		$w delete sel.first sel.last
	}
}


proc TextControlD {w} {
	variable ::comment::Vars

	if {$w eq $Vars(widget:text)} {
		set c [$w get insert]
		if {$c eq "\u00b6"} {
			$w delete insert insert+1c
		} elseif {$c eq "\n"} {
			$w delete insert-1c insert
			$w mark set insert insert-1c
		}
	}
	$w delete insert
}

} ;# namespace tk


bind Text <BackSpace>		{ tk::TextBackSpace %W }
bind Text <Delete>			{ tk::TextDelete %W }
bind Text <<Clear>>			{ catch { tk::TextClear %W } }
bind Text <Control-d>		{ tk::TextControlD %W }
bind Text <Left>				{ tk::TextSetCursorExt %W insert-1displayindices }
bind Text <Right>				{ tk::TextSetCursorExt %W insert+1displayindices +1 }
bind Text <Up>					{ tk::TextSetCursorExt %W [tk::TextUpDownLine %W -1] }
bind Text <Down>				{ tk::TextSetCursorExt %W [tk::TextUpDownLine %W 1] }
bind Text <Prior>				{ tk::TextSetCursorExt %W [tk::TextScrollPages %W -1] }
bind Text <Next>				{ tk::TextSetCursorExt %W [tk::TextScrollPages %W 1] }
bind Text <End>				{ tk::TextSetCursorExt %W {insert display lineend} }
bind Text <Control-Left>	{ tk::TextSetCursorExt %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]}
bind Text <Control-Right>	{ tk::TextSetCursorExt %W [tk::TextNextWord %W insert] }
bind Text <Control-Up>		{ tk::TextSetCursorExt %W [tk::TextPrevPara %W insert] }
bind Text <Control-Down>	{ tk::TextSetCursorExt %W [tk::TextNextPara %W insert] }
bind Text <Control-End>		{ tk::TextSetCursorExt %W {end - 1 indices} }
bind Text <Control-b>		{ tk::TextSetCursorExt %W insert-1displayindices }
bind Text <Control-e>		{ tk::TextSetCursorExt %W {insert display lineend} }
bind Text <Control-f>		{ tk::TextSetCursorExt %W insert+1displayindices +1 }
bind Text <Control-n>		{ tk::TextSetCursorExt %W [tk::TextUpDownLine %W 1] }
bind Text <Control-p>		{ tk::TextSetCursorExt %W [tk::TextUpDownLine %W -1] }
bind Text <Meta-f>			{ tk::TextSetCursorExt %W [tk::TextNextWord %W insert] }
bind Text <Meta-greater>	{ tk::TextSetCursorExt %W end-1c }


bind Text <Meta-b> {
	if {!$tk_strictMotif} {
		tk::TextSetCursorExt %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]
	}
}


if {[tk windowingsystem] eq "aqua"} {

bind Text <Option-Left>		{ tk::TextSetCursorExt %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]}
bind Text <Option-Right>	{ tk::TextSetCursorExt %W [tk::TextNextWord %W insert] +1 }
bind Text <Option-Up>		{ tk::TextSetCursorExt %W [tk::TextPrevPara %W insert] }
bind Text <Option-Down>		{ tk::TextSetCursorExt %W [tk::TextNextPara %W insert] +1 }

} ;# End of Mac only bindings


# TODO
bind Text <Control-k> {}
bind Text <Meta-d> {}
bind Text <Meta-BackSpace> {}
bind Text <Meta-Delete> {}
bind Text <Control-h> {}
bind Text <Control-o> {}

# vi:set ts=3 sw=3:
