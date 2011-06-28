# ======================================================================
# Author : $Author$
# Version: $Revision: 56 $
# Date   : $Date: 2011-06-28 14:04:22 +0000 (Tue, 28 Jun 2011) $
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

# --- Special popups for BETA version only -----------------------------

namespace eval beta {

variable Welcome 0
#array set NotYetImplemented {}


proc Enc {s} { return [encoding convertfrom utf-8 $s] }


proc welcomeToScidb {parent} {
	variable Welcome

	if {$Welcome} { return }

	set hdr(de) [Enc "Willkommen zu Scidb!"]
	set hdr(en) [Enc "Welcome to Scidb!"]
	set hdr(es) [Enc "Welcome to Scidb!"]
	set hdr(it) [Enc "Benvenuto su Scidb!"]

	set msg(de) [Enc "Dies ist eine Vorabversion von Scidb zum Ausprobieren und Testen. Die erste Vollversion wird erst nach einer genügend langen Testphase erscheinen. Beim Ausprobieren dieser Version sollte folgendes berücksichtigt werden:

1. Diese Version enthält viele Debugging-Informationen und ist an einigen Stellen noch entsprechend langsam.

2. Die Unterstützung der Scid-Datenbanken (.si3/.si4) erfolgt durch eine recht komplexe Emulation, die noch nicht genügend ausgetestet ist. Also bitte nicht die Original-Scid-Datenbanken mit diesem Programm bearbeiten. Standardmäßig werden die Scid-Datenbanken nur im Lesemodus, der natürlich unkritisch ist, geöffnet.

3. Das aktuelle Scidb-Format (.sci) wird spätestens mit der ersten Vollversion vollendet (d.h. Versionsnummer 1.0 erreichen, die aktuelle Versionsnummer ist 0.91). Das aktuelle Format ist aber bereits voll einsetzbar und es wird eine Upgrade-Möglichkeit geben.

4. Die Unterstützung des ChessBase-Formats (.cbh) ist noch nicht vollendet. Zur Zeit können z.B. keine Schach-960 Partien gelesen werden.

5. Von Zeit zu Zeit wird eine aktuellere Testversion ver�ffentlicht werden.

Viel Freude mit Scidb, dessen Entwicklung bisher bereits drei Jahre in Anspruch genommen hat!"]
#-------------------------------------------------------------------------------------------------
	set msg(en) [Enc "This is a preliminary version of Scidb for try-out and testing. The first full version will be released after a sufficiently long test period. Please consider while experimenting with Scidb:

1. This version contains a lot of debugging information and is accordingly slow in several places.

2. The support of Scid databases (.si3/.si4) is performed by a quite complex emulation which is not yet sufficiently tested. Therefore please do not edit your original Scid databases with this program. Per default the Scid databases will be opened in read-only mode, which is of course uncritical.

3. The current Scidb format (.sci) will be finished at latest with the first full version (i.e. it will reach version 1.0 later, the current version is 0.91). Nevertheless the current format is already fully usable, and there will be offered an upgrade capability.

4. The support of the ChessBase format (.cbh) is not yet finished. Currently this application cannot open Chess 960 games.

5. From time to time a newer test version will be released.

Have a lot of fun with Scidb, whose development has already taken three years!"]
#-------------------------------------------------------------------------------------------------
	set msg(es) [Enc "This is a preliminary version of Scidb for try-out and testing. The first full version will be released after a sufficiently long test period. Please consider while experimenting with Scidb:

1. This version contains a lot of debugging information and is accordingly slow at several places.

2. The support of Scid databases (.si3/.si4) is performed by a quite complex emulation which is not yet sufficiently tested. Therefore please do not edit your original Scid databases with this program. Per default the Scid databases will be opened in read-only mode, which is of course uncritical.

3. The current Scidb format (.sci) will be finished at latest with the first full version (i.e. it will reach version 1.0 later, the current version is 0.91). Nevertheless the current format is already fully usable, and there will be offered an upgrade capability.

4. The support of the ChessBase format (.cbh) is not yet finished. Currently this application cannot open Chess 960 games.

5. From time to time a newer test version will be released.

Have a lot of fun with Scidb, whose development has already taken three years!"]
#-------------------------------------------------------------------------------------------------
	set msg(it) [Enc "Questa � una versione preliminare di Scidb per prove e test. La prima versione completa sar� rilasciata dopo un periodo sufficientemente lungo di test. Tieni a mente le seguenti cose quando fai esperimenti con Scidb:

1. Questa versione contiene molte informazioni di debugging ed � quindi lenta in diversi punti.

2. Il supporto al formato database di Scid (.si3/.si4) � reso possibile da una emulazione piuttosto complessa che non � stata ancora testata a sufficienza. Quindi per favore non modificare i tuoi database originali in formato Scid con questo programma. Per default i database Scid saranno aperti in modalit� sola-lettura, che ovviamente non comporta problemi.

3. Il formato di Scidb attuale (.sci) sar� finito al pi� tardi con il rilascio della prima versione completa (cio� quando sar� raggiunta la versione 1.0 o superiore, la versione corrente � la 0.91). Nonostante ci� il formato attuale � del tutto utilizzabile, e sar� possibile aggiornarlo in futuro.

4. Il supporto al formato database di Chessbase (.cbh) non � ancora finito. Ad oggi questo programma non pu� aprire partite di Scacchi 960.

5. Col tempo saranno rilasciate nuove versioni di test.

Divertitevi con Scidb, il cui sviluppo ha richiesto gi� tre anni!"]

	set reply [::dialog::info -message $hdr($::mc::langID) -detail $msg($::mc::langID) -parent $parent]
	if {$reply eq "ok"} { set Welcome 1 }
}


proc notYetImplemented {parent what} {
#	variable NotYetImplemented

#	if {[info exists NotYetImplemented($what)]} { return }

	set hdr(de) [Enc "Noch nicht implementiert."]
	set hdr(en) [Enc "Not yet implemented."]
	set hdr(es) [Enc "Not yet implemented."]
	set hdr(it) [Enc "Non ancora implementato."]

	set msg(de) [Enc "Diese Funktionalität ist noch nicht implementiert worden, sie dient nur zur Voransicht."]
	set msg(en) [Enc "This functionality is not yet implemented. This is only a preview."]
	set msg(es) [Enc "This functionality is not yet implemented. This is only a preview."]
	set msg(it) [Enc "Questa funzione non � ancora implementata. Questa � solo un'anteprima."]

	::dialog::info -message $hdr($::mc::langID) -detail $msg($::mc::langID) -parent $parent
#	set NotYetImplemented($what) 1
}


proc WriteOptions {chan} {
	::options::writeItem $chan [namespace current]::Welcome
#	::options::writeItem $chan [namespace current]::NotYetImplemented
}

::options::hookWriter [namespace current]::WriteOptions

}

# --- Initalization ----------------------------------------------------

if {[tk windowingsystem] eq "x11"} {
	proc NoWindowDecor {w} {
		update idletasks
		::scidb::tk::wm noDecor $w
	}

	proc ::dialog::choosecolor::x11NoWindowDecor {w} { NoWindowDecor $w }
	proc ::toolbar::x11NoWindowDecor {w} { NoWindowDecor $w }
}

set dialog::iconOk			$icon::iconOk
set dialog::iconCancel		$icon::iconCancel
set dialog::iconGoNext		$icon::iconGoNext
set dialog::iconYes			$icon::iconOk

set dialog::choosefont::iconOk		$icon::iconOk
set dialog::choosefont::iconCancel	$icon::iconCancel
set dialog::choosefont::iconApply	$icon::iconApply
set dialog::choosefont::iconReset	$icon::iconReset

proc ::dialog::choosefont::messageBox {parent msg buttons defaultButton} {
	return [::dialog::warning -parent $parent -message $msg -buttons $buttons -default $defaultButton]
}

set dialog::choosecolor::iconOk		$icon::iconOk
set dialog::choosecolor::iconCancel	$icon::iconCancel

proc ::dialog::choosecolor::tooltip {args} { ::tooltip::tooltip {*}$args }

proc ::calendar::tooltip {args} { ::tooltip::tooltip {*}$args }

proc ::scrolledframe::sbset {w first last} { ::widget::sbset $w $first $last }

set ::dialog::fsbox::showHiddenBtn	1
set ::dialog::fsbox::showHiddenVar	0
set ::dialog::fsbox::destroyOnExit	1
set ::dialog::fsbox::iconAdd			$icon::iconAdd
set ::dialog::fsbox::iconRemove		$icon::iconRemove
set ::dialog::fsbox::iconSave			$icon::iconSave
set ::dialog::fsbox::iconCancel		$icon::iconCancel
set ::dialog::fsbox::iconOpen			$icon::iconOpen

proc ::dialog::fsbox::makeStateSpecificIcons {img} { return [::icon::makeStateSpecificIcons $img] }
proc ::dialog::fsbox::tooltip {args} { ::tooltip::tooltip {*}$args }
proc ::dialog::fsbox::messageBox {args} { return [::dialog::messageBox {*}$args] }

dialog::fsbox::setupIcon ".sci"	$::icon::16x16::filetypeScidbBase
dialog::fsbox::setupIcon ".si4"	$::icon::16x16::filetypeScid4Base
dialog::fsbox::setupIcon ".si3"	$::icon::16x16::filetypeScid3Base
dialog::fsbox::setupIcon ".cbh"	$::icon::16x16::filetypeChessBase
dialog::fsbox::setupIcon ".pgn"	$::icon::16x16::filetypePGN
dialog::fsbox::setupIcon ".gz"	$::icon::16x16::filetypePGN
dialog::fsbox::setupIcon ".zip"	$::icon::16x16::filetypeZipFile

proc ::dialog::progressbar::busyCursor {w state} { ::widget::busyCursor $w $state }

proc ::colormenu::tooltip {args} { ::tooltip::tooltip {*}$args }

proc WriteOptions {chan} {
	::options::writeList $chan ::dialog::choosecolor::UserColorList
	::options::writeItem $chan ::dialog::fsbox::showHiddenBtn
	::options::writeItem $chan ::table::options
	::options::writeItem $chan ::menu::Theme
	puts $chan "::dialog::fsbox::setBookmarks {[::dialog::fsbox::getBookmarks]}"
}
::options::hookWriter [namespace current]::WriteOptions

# --- Read options -----------------------------------------------------

if {[file readable $::scidb::file::options]} {
	::load::source $::scidb::file::options -message $::load::mc::ReadingOptionsFile -encoding utf-8
}

# --- Initalization ----------------------------------------------------

#if {[llength $::engine::Engines] == 0} {
#	foreach entry {
#		{
#			Name			Stockfish
#			Elo			0
#			CCRL			0
#			Command		stockfish-191-32-ja
#			Parameters	{}
#			Logo			stockfish
#			Url			http://www.stockfishchess.com/download/all/index.html
#			Protocol		UCI
#			Options		{}
#			Timestamp	0
#		}
#		{
#			Name			Crafty
#			Elo			0
#			CCRL			0
#			Command		crafty
#			Parameters	{}
#			Logo			crafty
#			Url			ftp://ftp.cis.uab.edu/pub/hyatt
#			Protocol		WB
#			Options		{}
#			Timestamp	0
#		}
#		{
#			Name			Fruit
#			Elo			0
#			CCRL			0
#			Command		fruit
#			Parameters	{}
#			Logo			fruit
#			Url			http://www.fruitchess.com
#			Protocol		UCI/FRC
#			Options		{}
#			Timestamp	0
#		}
#		{
#			Name			Phalanx
#			Elo			0
#			CCRL			0
#			Command		phalanx
#			Parameters	{}
#			Logo			phalanx
#			Url			http://phalanx.sourceforge.net
#			Protocol		WB
#			Options		{}
#			Timestamp	0
#		}
#		{
#			Name			{Gullydeckel 2}
#			Elo			0
#			CCRL			0
#			Command		gully2
#			Parameters	{}
#			Logo			gully2
#			Url			http://borriss.com
#			Protocol		WB
#			Options		{}
#			Timestamp	0
#		}
#		{
#			Name			Micro-Max
#			Elo			0
#			CCRL			0
#			Command		micromax
#			Parameters	{}
#			Logo			micromax
#			Url			http://home.hccnet.nl/h.g.muller/max-src2.html
#			Protocol		WB
#			Options		{}
#			Timestamp	0
#		}} {
#
#		array set arr $entry
#		set arr(Directory) $::scidb::dir::user
#		set arr(Command) "[file join $::scidb::dir::share engines $arr(Command)]"
#
#		if {[file executable $arr(Command)]} {
#			::engine::engine $entry
#		}
#	}
#}

::theme::setTheme $menu::Theme
::menu::setup
::board::setupTheme
::tooltip::init
::mc::selectLang
::font::useLanguage $mc::langID
application::open

# vi:set ts=3 sw=3:
