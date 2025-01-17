#!/bin/sh
#\
exec tclsh "$0" "$@"
# ======================================================================
# Author : $Author$
# Version: $Revision: 1367 $
# Date   : $Date: 2017-08-03 13:44:17 +0000 (Thu, 03 Aug 2017) $
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
# Copyright: (C) 2012-2013 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

package require Tcl 8.5
package require Tk 8.5

wm withdraw .

array set Pieces {
	de {K D T L S B}
	en {K Q R B N P}
	es {R D T A C P}
	it {R D T A C P}
	hu {K V B F H G}
	sv {K D T L S B}
}

set HtmlDocType {<?xml version="1.0" encoding="utf-8"?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
}

set HtmlHead {<head>
  <meta http-equiv="content-type"
           content="text/html; charset=utf-8" />
  <meta http-equiv="content-language"
           content="%LANG%" />
  <meta http-equiv="content-style-type"
           content="text/css" />

  <meta name="author"
     content="scidb.sourceforge.net" />
  <meta name="generator"
     content="scidb.sourceforge.net" />
  <meta name="description"
     content="%CONTENT%" />

  <link rel="icon"
       href="http://scidb.sourceforge.net/favicon.ico"
       type="image/x-icon" />
  <link rel="shortcut icon"
       href="http://scidb.sourceforge.net/favicon.ico" />
  <link rel="stylesheet"
       type="text/css"
      media="screen"
		 href="%STYLE%" />

  <title>%TITLE-PREFIX%: %TITLE%</title>
%HEAD%</head>
}

set HtmlH1 {<div class="title">
  <h1 class="title">%TITLE%</h1>
</div>
}

set HtmlDefs {}

set HtmlMapping {
	<menuitem>		{<span class='menuitem'>}
	</menuitem>		{</span>}

	<note/>			{<br/><img src='../images/note.png' alt='note' /> }
	<note>			{<p><img src='../images/note.png' alt='note' /> }
	</note>			{</p>}

	<nobr>			{<span style='white-space:nowrap;'>}
	</nobr>			{</span>}

	<box>				{<div class='box'>}
	</box>			{</div>}

	<expr>			{<span style='white-space:nowrap;'><code>}
	</expr>			{</code></span>}

	<verb>			{<div class='verb'><code>}
	</verb>			{</code></div>}

	<verbatim>		{<div class='box'><pre><code>}
	</verbatim>		{</code></pre></div>}

	<example>		{<div class='example'><pre>}
	</example>		{</pre></div>}

	<annotation>	{<div class='annotation'><img src='../images/annotation.png' style='float:left; margin:0 1em 0 0' alt='annotation' />}
	</annotation>	{</div>}

	<dir>				{<ul style='list-style-type: none'><li>}
	</dir>			{</li></ul>}

	<comment>		{<span class='comment'>}
	</comment>		{</span>}

	<keyword>		{<span class='keyword'>}
	</keyword>		{</span>}

	<see/>			&#x21d2;

	<chess>			{<span class='chess'>}
	</chess>			</span>

	<awesome>		{<span class='awesome'>}
	</awesome>		</span>

	<n>				{<span class='normal'>}
	</n>				</span>

	<NEW>				{<span class='NEW'>}
	</NEW>			{</span>}

	&King;			{<span class='chess'>&#x2654;</span>}
	&Queen;			{<span class='chess'>&#x2655;</span>}
	&Rook;			{<span class='chess'>&#x2656;</span>}
	&Bishop;			{<span class='chess'>&#x2657;</span>}
	&Knight;			{<span class='chess'>&#x2658;</span>}
	&Pawn;			{<span class='chess'>&#x2659;</span>}

	&king;			{<span class='chess'>&#x265a;</span>}
	&queen;			{<span class='chess'>&#x265b;</span>}
	&rook;			{<span class='chess'>&#x265c;</span>}
	&bishop;			{<span class='chess'>&#x265d;</span>}
	&knight;			{<span class='chess'>&#x265e;</span>}
	&pawn;			{<span class='chess'>&#x265f;</span>}

	&K;				{<span class='piece'>&#x2654;</span>}
	&Q;				{<span class='piece'>&#x2655;</span>}
	&R;				{<span class='piece'>&#x2656;</span>}
	&B;				{<span class='piece'>&#x2657;</span>}
	&N;				{<span class='piece'>&#x2658;</span>}
	&P;				{<span class='piece'>&#x2659;</span>}

	&k;				{<span class='piece'>&#x265a;</span>}
	&q;				{<span class='piece'>&#x265b;</span>}
	&r;				{<span class='piece'>&#x265c;</span>}
	&b;				{<span class='piece'>&#x265d;</span>}
	&n;				{<span class='piece'>&#x265e;</span>}
	&p;				{<span class='piece'>&#x265f;</span>}

	&A;				{<span class='cql' title='%::mc::PieceCQL(A)%'>&#x25cb;</span>}
	&a;				{<span class='cql' title='%::mc::PieceCQL(a)%'>&#x25cf;</span>}
	&M;				{<span class='cql' title='%::mc::PieceCQL(M)%'>&#x25b3;</span>}
	&m;				{<span class='cql' title='%::mc::PieceCQL(m)%'>&#x25b2;</span>}
	&I;				{<span class='cql' title='%::mc::PieceCQL(I)%'>&#x25bd;</span>}
	&i;				{<span class='cql' title='%::mc::PieceCQL(i)%'>&#x25bc;</span>}
	&U;				{<span class='cql' title='%::mc::PieceCQL(U)%'>&#x25d1;</span>}

	&L;				{<span class='cql' title='%::mc::SquareCQL(L)%'>&#x25a1;</span>}
	&D;				{<span class='cql' title='%::mc::SquareCQL(D)%'>&#x25a0;</span>}
	&H;				+

	fa-external-link-square	{&#xf14c;}
}

switch [tk windowingsystem] {
	x11	{
		lappend SysMapping <x11> {} </x11> {} <win32> {<!-- } </win32> { -->} <aqua> {<!-- } </aqua> { -->}
	}
	win32	{
		lappend SysMapping <x11> {<!-- } </x11> { -->} <win32> {} </win32> {} <aqua> {<!-- } </aqua> { -->}
	}
	aqua	{
		lappend SysMapping <x11> {<!-- } </x11> { -->} <win32> {<!-- } </win32> { -->} <aqua> {} </aqua> {}
	}
}

set PieceMapping {
	K {<span class='piece'>&#x2654;</span>}
	Q {<span class='piece'>&#x2655;</span>}
	R {<span class='piece'>&#x2656;</span>}
	B {<span class='piece'>&#x2657;</span>}
	N {<span class='piece'>&#x2658;</span>}
	P {<span class='piece'>&#x2659;</span>}
}

set f [open ../../../Makefile.in r]
while {[gets $f line] >= 0} {
	if {[regexp {SUFFIX[ \t]*=[ \t]*([^ \t]+)} $line _ suffix]} {
		lappend HtmlMapping %scidb% scidb$suffix
		break
	}
}
close $f
if {"%scidb%" ni $HtmlMapping} {
	puts stderr "Couldn't find declaration of 'SUFFIX' in ../../../Makefile.in"
	exit 1
}

proc print {chan source title head body} {
	variable lang

	if {[string length $head]} {
		set content \n
		foreach line [split [string trim $head] \n] {
			append content "  " $line \n
		}
		set head $content
	}

	set headerMap [list \
		%TITLE% $title \
		%TITLE-PREFIX% "Scidb - $::help::mc::Help" \
		%LANG% $lang \
		%CONTENT% "Scidb - Help Page" \
		%STYLE% "../styles/help.css" \
		%HEAD% $head
	]

if {0} {
	set p [open [list |../scripts/paragraphs.pl << $body] r] 
	set body ""
	while {![eof $p]} { append body [gets $p] \n } 
	close $p
}

	puts $chan $::HtmlDocType
	puts $chan "<!-- Generated from $source -->"
	puts $chan ""
	puts $chan "<html xmlns='http://www.w3.org/1999/xhtml' xml:lang='en' lang='$lang'>"
	puts $chan [string map $headerMap $::HtmlHead]
	puts $chan "<body>"
	puts $chan ""
	puts $chan "<div class=\"title\">"
	puts $chan "  <h1 class=\"title\">$title</h1>"
	puts $chan "</div>"
	puts $chan ""
	foreach line $body { puts $chan $line }
	puts $chan ""
	puts $chan "</body>"
	puts $chan "</html>"
}

proc readTranslationFile {file nagFile encoding} {
	set f [open $file r]
	chan configure $f -encoding $encoding

	while {[gets $f line] >= 0} {
		if {[string length $line] > 0 && [string index $line 0] ne "#"} {
			set var [lindex $line 0]
			set value [string map {& {} "..." {}} [lindex $line 1]]
			set ns [join [lrange [split $var ::] 1 end-2] ::]
			if {[llength $ns]} { namespace eval $ns {} }
			set $var $value
		}
	}

	close $f

	if {[file readable $nagFile]} {
		set f [open $nagFile r]
		chan configure $f -encoding $encoding

		namespace eval ::annotation {}
		namespace eval ::annotation::mc {}

		while {[gets $f line] >= 0} {
			if {[string length $line] > 0 && [string index $line 0] ne "#"} {
				set var [lindex $line 0]
				set value [string map {& {} "..." {}} [lindex $line 1]]
				set ns [join [lrange [split $var ::] 1 end-2] ::]
				if {[llength $ns]} { namespace eval $ns {} }
				set ::annotation::mc::Nag($var) $value
			}
		}

		close $f
	}
}

proc substituteVariables {line} {
	while {[regexp {%(::)?([a-zA-Z_]+::)*[a-zA-Z_]+(\([^)]+\))?%} $line pattern]} {
		set var [string range $pattern 1 end-1]
		if {[info exists $var]} {
			set line [string map [list $pattern [set $var]] $line]
		} else {
			puts stderr "Warning([info script]): Couldn't substitute $var"
			set line [string map [list $pattern $var] $line]
		}
	}

	return $line
}

proc example {chan} {
	puts $chan "<!-- Example usage: -->"
	puts $chan ""
	puts $chan "<!-- CHARSET may be omitted if the encoding"
	puts $chan "     of the language file will be used. -->"
	puts $chan ""
	puts $chan "CHARSET iso8859-1"
	puts $chan "TITLE   Clipbase"
	puts $chan "INDEX   Clipbase"
	puts $chan ""
	puts $chan "BEGIN"
	puts $chan ""
	puts $chan "<p>...</p>"
	puts $chan ""
	puts $chan "END"

	exit 1
}

proc getArg {line} {
	set n 0
	while {$n < [string length $line] && ![string is space [string index $line $n]]} { incr n }
	while {$n < [string length $line] && [string is space [string index $line $n]]} { incr n }
	return [string range $line $n end]
}

if {$argc < 1} {
	puts stderr "Usage: [info script] <input-file> [<output-file>]"
	exit 1
}

encoding system utf-8
set lang [file tail [pwd]] 
set file [file join .. .. lang localization.tcl]
source $file

foreach entry $i18n::languages {
	lassign $entry langName codeName charsetName translationFile
	if {$codeName eq $lang} { break }
}

if {$codeName ne $lang} {
	puts stderr "Error([info script]):"
	puts stderr "Language \"$lang\" not defined in file \"$file\"."
	puts stderr "You have to edit \"$file\"."
	exit 1
}

set transFile [file join .. .. lang $translationFile]
set nagFile [file join .. .. lang nag $translationFile]

if {![file readable $transFile]} {
	puts stderr "Error([info script]): Cannot open file \"$transFile\"."
	exit 1
}
#if {![file readable $nagFile]} {
#	puts stderr "Error([info script]): Cannot open file \"$nagFile\"."
#	exit 1
#}

set srcfile [lindex $argv 0]
set dstfile [lindex $argv 1]

set src [open $srcfile r]
set charset $charsetName
chan configure $src -encoding $charset
set title ""
set head ""
set parseHead 0
set skip 0

readTranslationFile $transFile $nagFile $charsetName

while {[gets $src line] >= 0} {
	if {$parseHead} {
		if {$line eq "END"} {
			set parseHead 0
		} else {
			append head $line \n
		}
	} elseif {[string match TITLE* $line]} {
		set title [substituteVariables [getArg $line]]
		break
	} elseif {[string match CHARSET* $line]} {
		set charset [getArg $line]
		chan configure $src -encoding $charset
	} elseif {[string match {DEFINE *} $line]} {
		if {[regexp {DEFINE\s+(<[a-z0-9/]+>)\s+(.*)} $line _ var subst]} {
			lappend HtmlDefs $var $subst
		} else {
			puts stderr "Error([info script]): DEFINE statement invalid"
		}
	} elseif {$line eq "HEAD"} {
		set parseHead 1
	}
}

if {![string match TITLE* $line]} {
	puts stderr "Error([info script]): Missing mandatory TITLE."
	example stderr
}

lappend HtmlMapping "<whitepieceletters/>" $Pieces($lang)
lappend HtmlMapping "<blackpieceletters/>" [string tolower $Pieces($lang)]

proc formatPath {path} {
	set parts {}
	set components [split $path "/"]
	for {set i 0} {$i < [llength $components]} {incr i} {
		set comp [lindex $components $i]
		if {[string length $comp] > 0} {
			lappend parts $comp
		} elseif {$i == 0} {
			if {[llength $components] > 1} {
				incr i
				lappend parts "/[lindex $components $i]"
			} else {
				lappend parts "/"
			}
		}
	}
	if {[string index $path end] eq "/" && [lindex $parts end] ne "/"} {
		lset $parts end "[lindex $parts end]/"
	}
	set result ""
	for {set i 0} {$i < [llength $parts]} {incr i} {
		set part [lindex $parts $i]
		append result "<code>$part/</code>"
		if {$i < [llength $parts] - 1 && [string length $part] > 2} { append result "&#8203;" }
	}
	return $result
}

proc parseGameToken {line expr} {
	variable PieceMapping

	set result ""
	set comma ""

	foreach tok [split $line " "] {
		set tok [string trim $tok]
		if {[string length $tok] > 0} {
			set tok [string map $PieceMapping $tok]
			set tok [string map {"O-O-O-O" "0-0-0-0" "O-O-O" "0-0-0" "O-O" "0-0"} $tok]
			append result $comma
			if {[string length $expr]} { append result "<$expr>" }
			append result $tok
			if {[string length $expr]} { append result "</$expr>" }
			set comma " "
		}
	}

	return $result
}

proc parseGame {line code expr} {
	variable insideComment

	set result ""
	set comma ""
	set n 0

	if {$insideComment} {
		set k 0
	} else {
		set k [string first "\{" $line]
	}

	while {$k >= 0} {
		set insideComment 1
		set j [string first "\}" $line $n]
		if {$j == -1} {
			set j end
		} else {
			set insideComment 0
			incr j -1
		}

		append result [parseGameToken [string range $line $n [expr {$k - 1}]] $expr]
		if {[string length $result] > 0} { append result " " }
		append result "("
		if {[string length $code]} { append result "<$code>" }
		append result [string range $line [expr {$k + 1}] $j]
		append result ")"
		if {[string length $code]} { append result "</$code>" }
		set comma " "

		if {$insideComment} {
			set k -1
		} else {
			set n [expr {$j + 2}]
			set k [string first "\{" $line $n]
		}
	}

	append result $comma
	append result [parseGameToken [string range $line $n end] $expr]

	return $result
}

proc formatUrl {url} {
	set i [string first :// $url]
	if {$i == -1} { return [formatPath $url] }
	set result "<code>[string range $url 0 [expr {$i - 1}]]://</code>&#8203;"
	append result [formatPath [string range $url [expr {$i + 3}] end]]
	return $result
}

proc readContents {chan file} {
	global argv
	variable HtmlMapping
	variable HtmlDefs
	variable SysMapping
	variable Pieces
	variable insideComment
	variable charset
	variable lang

	set thisSection ""
	set contents {}
	set indices {}
	set linePref ""
	set parseGame 0
	set insideVerbatim 0
	set insideComment 0

	set s [string tolower [lindex [split [lindex $argv 0] -] 1]]
	if {$s eq "match" || $s eq "position" || $s eq "relation"} { set thisSection $s }

	while {[gets $chan line] >= 0} {
		set line [string trimright $line]
		if {[string match END* $line]} { break }

		if {[string length $linePref]} {
			append linePref $line
			set line $linePref
			set linePref ""
		}

		if {$insideVerbatim} {
			set expr "expr"
			set code "code"
		} else {
			set expr ""
			set code ""
		}

		if {[string match {*<game>*} $line]} {
			set insideComment 0
			set n [string first "<game>" $line]
			set k [string first "</game>" $line]
			set newline [string range $line 0 [expr {$n - 1}]]
			if {$k == -1} {
				append newline [parseGame [string range $line [expr {$n + 6}] end] $code $expr]
				set parseGame 1
			} else {
				append newline [parseGame [string range $line [expr {$n + 6}] [expr {$k - 1}]] $code $expr]
				append newline [string range $line [expr {$k + 7}] end]
			}
			set line $newline
		} elseif {[string match {*</game>*} $line]} {
			set n [string first "</game>" $line]
			set newline [parseGame [string range $line 0 [expr {$n - 1}]] $code $expr]
			append newline [string range $line [expr {$n + 7}] end]
			set line $newline
			set parseGame 0
		} elseif {$parseGame} {
			set line [parseGame $line $code $expr]
		}

		if {[string match *verbatim>* $line]} {
			if {[string match *<verbatim>* $line]} {
				append line "<!--"
				set linePref "-->"
			} elseif {[llength $contents] > 0} {
				set last [lindex $contents end]
				append last "<!--"
				lset contents end $last
				set s "-->"
				append s $line
				set line $s
			}
		}

		if {[string match *<verbatim>* $line] || [string match *<verb>* $line]} {
			set insideVerbatim 1
		}

		while {[regexp -indices {<furtherinfo href=.*/>} $line location]} {
			lassign $location i k
			set file [string trim [string range $line [expr {$i + 18}] [expr {$k - 2}]]]
			set file [string trim [string range $file 1 end-1]]
#			set content "<div style='text-align: center'>\n"
#			append content "  <div embed='$file'>\n"
#			append content "  </div>\n"
#			append content "</div>"
			set content "<p><a href='$file'>$::tips::mc::FurtherInformation...</a></p>\n"
			set newline [string range $line 0 [expr {$i - 1}]]
			append newline $content
			append newline [string range $line [expr {$k + 1}] end]
			set line $newline
		}

		while {[regexp -indices {<url>.*</url>} $line location]} {
			lassign $location i k
			set range [string range $line [expr {$i + 5}] [expr {$k - 6}]]
			set newline [string range $line 0 [expr {$i - 1}]]
			append newline [formatUrl [string range $line [expr {$i + 5}] [expr {$k - 6}]]]
			append newline [string range $line [expr {$k + 1}] end]
			set line $newline
		}

		while {[regexp -indices {<cql>[^/]*</cql>} $line location]} {
			lassign $location i k
			set keyword [string trim [string range $line [expr {$i + 5}] [expr {$k - 6}]]]
			set newline [string range $line 0 [expr {$i - 1}]]
			if {[string index $keyword 0] == ":"} {
				append newline "<nobr>$keyword</nobr>"
			} elseif {[string index $keyword 0] == "!" && [string index $keyword 1] == ":"} {
				append newline "<nobr>$keyword</nobr>"
			} else {
				set parts [split $keyword :]
				set section [lindex $parts 0]
				set exclamationMark ""
				if {[string index $section 0] == "!"} {
					set exclamationMark "!"
					set section [string range $section 1 end]
				}
				set keyword [string trim [lindex [split [lindex $parts 1] " "] 0]]
				set text [join [lrange $parts 1 end] ":"]
				set Section [string toupper $section 0 0]
				set href [string map {* _} $keyword]
				append newline \
					"<nobr>$exclamationMark<a href=\"CQL-$Section-List.html#$section:$href\">:$text</nobr>"
				if {!$insideVerbatim && [string length $thisSection] && $section ne $thisSection} {
					append newline "</nobr> : <nobr>$section</nobr>"
				}
				append newline "</a>"
			}
			append newline [string range $line [expr {$k + 1}] end]
			set line $newline
		}

		if {[string match *</verbatim>* $line] || [string match *</verb>* $line]} {
			set insideVerbatim 0
		}

		while {[regexp -indices {<keyword>[^/]*</keyword>} $line location]} {
			lassign $location i k
			set keyword [string trim [string range $line [expr {$i + 9}] [expr {$k - 10}]]]
			set exclamationMark ""
			if {[string index $keyword 0] == "!"} {
				set exclamationMark "!"
				set keyword [string range $keyword 1 end]
			}
			set newline [string range $line 0 [expr {$i - 1}]]
			set keyword ":[lindex [split $keyword :] 1]"
			append newline "<nobr>$exclamationMark<span class='keyword'>$keyword</span></nobr>"
			append newline [string range $line [expr {$k + 1}] end]
			set line $newline
		}

		set line [string map $HtmlDefs $line]

		set reImg {<img ([^>]*)src=[\"\']([^\"\']*)[\"\']([^>]*)/>}

		while {	[regexp $reImg $line _ extra1 src extra2]
				&& ![string match *alt=* $extra1]
				&& ![string match *alt=* $extra2]} {
			regexp -indices $reImg $line location
			lassign $location i k
			set newline [string range $line 0 [expr {$i - 1}]]
			append newline "<img ${extra1}src=\"${src}\" alt=\"\"${extra2}/>"
			append newline [string range $line [expr {$k + 1}] end]
			set line $newline
		}

		set line [string map $SysMapping $line]
		set line [string map $HtmlMapping $line]

		if {[llength $indices]} {
			if {[regexp -indices {.*</a>} $line indices]} {
				lassign $indices s e
				set newline [string range $line 0 [expr {$e - 4}]]
				append newline "&nbsp;<span class=\"awesome\">&#xf08e;</span></a>"
				append newline [string range $line [expr {$e + 1}] end]
				set line $newline
				set indices {}
			}
		} else {
			set e 0
			while {[regexp -indices {<a[^>]*href=.http[^>]*()>[^<]*</a>} $line indices inspos]} {
				lassign $indices s e; lassign $inspos f l
				set newline [string range $line 0 $l]
				append newline " target=\"blank_\""
				append newline [string range $line $f [expr {$e - 4}]]
				append newline "&nbsp;<span class=\"awesome\">&#xf08e;</span></a>"
				append newline [string range $line [expr {$e + 1}] end]
				set line $newline
			}
			while {[regexp -indices {<a[^>]*href=.ftp[^>]*()>[^<]*</a>} $line indices]} {
				lassign $indices s e; lassign $inspos f l
				set newline [string range $line 0 $l]
				append newline " target=\"blank_\""
				append newline [string range $line $f [expr {$e - 4}]]
				append newline "&nbsp;<span class=\"awesome\">&#xf08e;</span></a>"
				append newline [string range $line [expr {$e + 1}] end]
				set line $newline
			}
			while {[regexp -indices {<a[^>]*href=.mailto[^>]*()>[^<]*</a>} $line indices inspos]} {
				lassign $indices s e; lassign $inspos f l
				set newline [string range $line 0 $l]
				append newline " target=\"blank_\""
				append newline [string range $line $f [expr {$e - 4}]]
				append newline "&nbsp;<span class=\"awesome\">&#xf112;</span></a>"
				append newline [string range $line [expr {$e + 1}] end]
				set line $newline
			}
			set indices {}
			regexp -indices -start $e {<a[^>]*href=.http} $line indices
			if {[llength $indices] == 0} {
				regexp -indices -start $e {<a[^>]*href=.ftp} $line indices
			}
			if {[llength $indices]} {
				set href $line
			}
		}

		while {[regexp {<key>([a-zA-Z0-9\&%#:;\(\)_-]+)</key>} $line _ key]} {
			switch $key {
				King		{ set expr "<kbd class='key'>[lindex $Pieces($lang) 0]</kbd>" }
				Queen		{ set expr "<kbd class='key'>[lindex $Pieces($lang) 1]</kbd>" }
				Rook		{ set expr "<kbd class='key'>[lindex $Pieces($lang) 2]</kbd>" }
				Bishop	{ set expr "<kbd class='key'>[lindex $Pieces($lang) 3]</kbd>" }
				Knight	{ set expr "<kbd class='key'>[lindex $Pieces($lang) 4]</kbd>" }
				Pawn		{ set expr "<kbd class='key'>[lindex $Pieces($lang) 5]</kbd>" }

				default {
					if {[string length $key] == 1 || [string index $key 0] in {"%" "&"}} {
						set expr "<kbd class='key'>$key</kbd>"
					} elseif {[regexp {F[0-9][0-9]?} $key]} {
						set expr "<kbd class='key'>$key</kbd>"
					} else {
						set expr "<kbd class='key'>$::mc::Key($key)</kbd>"
					}
				}
			}
			set key [string map {( "\\(" ) "\\)"} $key]
			regsub -all -- {[&]} $expr {\\\0} expr
			set line [regsub -all "<key>$key</key>" $line $expr]
		}

		if {[string match CHARSET* $line]} {
			set charset [getArg $line]
			chan configure $src -encoding $charset
		} elseif {[regexp -indices {ENUM[(][0-9]+[.][.][0-9]+[)]} $line location]} {
			lassign $location i k
			set range [string range $line [expr {$i + 5}] [expr {$k - 1}]]
			lassign [split $range "."] from _ to
			set pref [string range $line 0 [expr {$i - 1}]]
			set suff [string range $line [expr {$k + 1}] end]
			for {} {$from <= $to} {incr from} {
				lappend contents "${pref}$from${suff}"
			}
		} else {
			lappend contents $line
		}
	}

	if {[llength $indices]} {
		puts stderr "unmatched <a href=...>: $href"
	}

	if {![string match END* $line]} {
		puts stderr "Error([info script]): Missing mandatory END."
		example stderr
	}

	close $chan

	while {[llength $contents] > 0 && [string length [lindex $contents 0]] == 0} {
		set contents [lreplace $contents 0 0]
	}

	while {[llength $contents] > 0 && [string length [lindex $contents end]] == 0} {
		set contents [lreplace $contents end end]
	}

	return $contents
}

proc processContents {contents} {
	variable body
	variable charset

	set indices {}
	set skip 0

	foreach line $contents {
		if {[string match {INCLUDE *} $line]} {
			set f [getArg $line]
			if {[catch { set inc [open $f r] }]} {
				puts stderr "Error([info script]): Cannot open file '$f'."
				exit 1
			}
			chan configure $inc -encoding utf-8
			while {[gets $inc line] >= 0} {
				if {[string match BEGIN* $line]} { break }
				if {[string match CHARSET* $line]} {
					chan configure $inc -encoding [getArg $line]
				}
			}
			if {![string match BEGIN* $line]} {
				puts stderr "Error($f): Missing mandatory END."
				exit 1
			}
			processContents [readContents $inc $f]
		} elseif {[string match EXCLUDE-BEGIN* $line]} {
			set skip 1
		} elseif {[string match EXCLUDE-END* $line]} {
			set skip 0
		} elseif {!$skip} {
			lappend body [substituteVariables $line]
		}
	}
}

set body {}
processContents [readContents $src [info script]]

if {[string length $dstfile] == 0} {
	fconfigure stdout -encoding utf-8
	print stdout [file join tcl help $lang $srcfile] $title $head $body
} else {
	set dst [open $dstfile w]
	fconfigure $dst -encoding utf-8
	print $dst [file join tcl help $lang $srcfile] $title $head $body
	close $dst
}

exit 0

# vi:set ts=3 sw=3:
