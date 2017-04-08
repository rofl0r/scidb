# ======================================================================
# Author : $Author$
# Version: $Revision: 1138 $
# Date   : $Date: 2017-04-08 15:54:51 +0000 (Sat, 08 Apr 2017) $
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
# Copyright: (C) 2013 Gregor Cramer
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

::util::source emoticons

namespace eval emoticons {
namespace eval mc {

set Tooltip(smile)		"Smiling (Smiley)"
set Tooltip(frown)		"Frown (Frowny)"
set Tooltip(saint)		"Saint"
set Tooltip(evil)			"Evil"
set Tooltip(gleeful)		"Gleeful"
set Tooltip(wink)			"Winking"
set Tooltip(cool)			"Cool"
set Tooltip(grin)			"Grinning"
set Tooltip(neutral)		"Neutral"
set Tooltip(sweat)		"Sweating"
set Tooltip(confuse)		"Confused"
set Tooltip(shock)		"Shocked"
set Tooltip(kiss)			"Kissing"
set Tooltip(razz)			"Razzing"
set Tooltip(grumpy)		"Disappointed / Grumpy"
set Tooltip(upset)		"Upset"
set Tooltip(cry)			"Crying"
set Tooltip(yell)			"Yelling"
set Tooltip(surprise)	"Surprised"
set Tooltip(red)			"Ashamed"
set Tooltip(sleep)		"Sleepy"
set Tooltio(eek)			"Scared"
set Tooltip(kitty)		"Kitty"
set Tooltip(roll)			"Eye-rolling"
set Tooltip(blink)		"Blinking"
set Tooltip(glasses)		"Glasses"

} ;# namespace mc

# character map for font Emoticons
array set CharMap {
	smile		"A"
	frown		"("
	neutral	"C"
	grin		"j"
	gleeful	"1"
	wink		"H"
	confuse	"G"
	shock		"o"
	grumpy	"W"
	upset		"4"
	cry		"n"
	surprise	"u"
	red		"s"
	eek		"o"
	yell		"3"
	roll		"U"
	blink		"b"
	sweat		"Q"
	razz		"F"
	sleep		"v"
	saint		"7"
	evil		"6"
	cool		"B"
	kitty		"9"
	glasses	"8"
	kiss		"i"
}


proc emoticons {} {
	return [::scidb::misc::emoticons list]
}


proc lookupCode {emotion} {
	if {![string is alpha -strict $emotion]} { return $emotion }
	return [scidb::misc::emoticons code $emotion]
}


proc lookupEmotion {code} {
	if {[string is alpha -strict $code]} { return $code }
	return [scidb::misc::emoticons parse $code]
}


proc getCharCode {emotion} {
	variable CharMap
	return $CharMap($emotion)
}


proc lookupChar {ch} {
	variable ReverseMap

	if {![info exists ReverseMap]} {
		variable CharMap
		foreach {name ch} [array get CharMap] {
			set ReverseMap($ch) $name
		}
	}

	return $ReverseMap($ch)
}


set icon(smile) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABAlBMVEUAAABzCgCVEACeEgCv
	KAC0KwDDJgC/MAC/MgDBNQDFNwDXMwDPQwDQRgDhQwAWgerVTQDhTQDoUgDcWgDtYwDxcwDp
	gQHqhAHxiAHzkgH2lQDymADymwD2rAT6rAD5rwD6tAD6wQH5xQD6ywL70AX41Af51wX41wn9
	2AL72gH82gL82wP92wP82g363A393AX53gz+3gL+3wL84AT+4AP+4wL54Rv+4wP84wn+5AP9
	5QX95A395RL+6AP+6gb87An+6Rb+7Qb+7wr97w3+7Bz+7B798Az+8RH98Rf+9hn+9jX99z/+
	+nr++oH++4Hz7+P++5v++6H+/L3+/v3+/v4BFoHKgx6CAAAAAXRSTlMAQObYZgAAAAFiS0dE
	AIgFHUgAAADESURBVBgZbcHVEoJAAAXQi60Ya2N3N2Fgi52r4P//iu44w5PnAEyegSnbuNAX
	vTSy+Mkd7zf6pLf7MQcmqfUJGV6HhPS1JL6Kp2ql1j13a5XqqQggulxL+nt8GL91ab2MApH9
	vKkHWotWQG/O9xEgtFGU0YQZKcomBPjVQUoUZVkWxfRA9QN8qe60eQkhPrurXuIBBNsF98Mw
	jIen0A7iy5rolWNhEo6XewkrGIswXW1329VUsOCHc2Q6s07GwcHEMfjvA35MHbT7TCbjAAAA
	AElFTkSuQmCC
}]

set icon(frown) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAA9lBMVEUACQJzCgCVEACeEgDD
	JgDXMwDhQwAWgerVTQDhTQDoUgDtYwDjcwHxcwDxiAHzkgHqmAL2lQD2rAT6rAD5rwD1sQP6
	tADzvwT1wQP5xQD6ywL6zQT20A792AL72gH82gL92gP82wP92wL92wP82g393AX+3AP+3gL+
	3wL93wX84AT+3wT+4AP+4AT84Q/+4wL+4wP84wn+5AP+4wr95QX95A395RL+6AP+6gb87An+
	6Rb76hn+7Qb+6hj86xn+7wr97w3+7Bz+7B798Az+8RH98Rf+9hn+9jX++Cz99z/++nr++oH+
	+4H++5v++6H+/L3+/v0BFoHpIXEDAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAADFSURB
	VBgZbcHpEoFQAAbQL6Lsa5ItshRKWbPvIcT1/i/DHTN+OQegchR+MhXXe3puJYOvrHO/eQ/v
	dneyoFLbyyJZupaSi8s2hQ/plI+K6/NajOZPEoDYbNkhr/6x/yKd5SwGRA62TLjatMYR2T5E
	gPDGNK0BZZnmJgwER6qqabqua5qqjoJAoFBWWl3DMLotpVwIAAjVq+0iz/PFdrUewoc/0RwK
	vVVPGDYTflC++Hi+2+/m47gPXwybbkwaaZbBD0PhvzfZ9RsIX+gFjwAAAABJRU5ErkJggg==
}]

set icon(gleeful) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAA9lBMVEUACQKVEACeEgCPGgCn
	JACvJgDDJgDDNwDDPQDHOwDXMwDJQwDhQwDUSgDVTQDhTQDYUgDUVQDeUgDYVwDoUgDgZgHt
	YwDxcwDxiAHzkgH2lQDwmwLbrCjzqgD2rAT6rAD2rwH5rwD0tgL6tAD0uQL5xQD7xQH6ywL7
	1Qj21xXv0kr72gH82g353g/63w363w/95QX95RLV1dX+6QT+6gX+6gb87An+7Qb+7gj+7wr9
	7w398Az+8A3+8RD98Rf+8xLf3tr+9hn++Cz99z/w69jt7e3++oH++4H48tj++5v28ub++6H1
	9fX+/L329vb8/Pz+/v5zCgABC8FfAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAADGSURB
	VBgZbcFnF4FgAAbQR7aWHYnsLZRkz0Ik9f//DO9xjk/uBYgGgR95cLaf9nkg46u2u17sh325
	7mogxNVpRNM9mh6dViI+Wsd9RhnHx0pmf2wBEObGor7ZNreb+sKYC0B+Xda0ma7rM00rr/NA
	dhkMRVmO49hoKLjMAswkbLqqq7qqa4YnDJDopPue4zme4/XTnQQAvh27WS/rZd1ibR4fVLGa
	ku6Hu5SqFikQgUK3lEvmSt1CAF9+pDKcDisRHz8+gf/ePigd0nbEUVgAAAAASUVORK5CYII=
}]

set icon(saint) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABCFBMVEUAAABzCgCVEAChEgDD
	JgDXMADgMwDhPQAWger8OQDVTQDbSgT8PQDnTQC9V1W/XV3FZlftYwDpbwDjZk3xcwD5dgHy
	egHxhAHXiFXSiIj2lQD9lQH9mAH9ngH9pwH2rAT8qgLnsUr5xQD6xwT9zQL7zwL90gT41Af8
	1AX91AP91QL41wn82wP82g31vb383wT+3gj+3wP+3wT+4AP+4AT+4wP84wnqycn+5AP95A39
	5Qr+5Qv95RLwzcf+6gb55DX87An+6Rb+7Ab97Av87A797g3+7Bz+7B7+8RH98iX+9hn+9jX5
	7Oz58fH88/P89vb99/f9+fn9/Pr+/Pz+/f3+/v3+/v4BFoGPPM5/AAAAAXRSTlMAQObYZgAA
	AAFiS0dEAIgFHUgAAADNSURBVBgZbcHnFoEAAAbQLzuyV0Qhe6RhbyJ7S+//JsQ5frkX+EM4
	3x7Px+0s4Ct8P3YT/kT3eA/jwzM+nC7Xy+kw9sCUXE6zdMAXoLPTZRJvud2qVNvut7XSapcD
	EJktmrrR2XQMvbmYRYCgqhR0V31Sd+kFRQ0CXpFl81K715byLCt6AWeGotJVSZGqaYrKOAF7
	lCRTFbklV1IkGbUDcDBMqDwYDsohhnHgzRor8m4TX4xZYbLE+3Ntrc37cQu+CBvXGDU4G4Ef
	woSPF1stHShC0Q8KAAAAAElFTkSuQmCC
}]

set icon(evil) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABEVBMVEUAAABgAABvAABzCgCS
	AACsAACVEACeEgDDAADPAACvKAC0KwDDJgC/MAD8EQi/MgDBNQDFNwDXMwDPQwDQRgDhQwAW
	gerVTQDhTQDoUgDcWgDtYwDxcwDpgQHqhAHxiAHqkgLqlQLzkgHymADymwD2rAT6tAD3uQHz
	uwP3uwH4wQH6wQH4wwL5xQD6xQH6xwH6ywL70AX70gL61wP92AL72gH92wP82g393AX82w7+
	3gL+3wL+4AP+4wL95QX95A385wn95RL+6AP75xX87An+6Rb+7Qb96SH+7wr+7Bz77Cb+7B79
	8Az98h7+9hn99z/++nP++nr++oH++4Hz7+P++5v++6H+/L3+/v3+/v4BFoFy8J7RAAAAAXRS
	TlMAQObYZgAAAAFiS0dEAIgFHUgAAADVSURBVBgZbcFnd8FgGAbgm9h7P7YKasWqLTFqxB5F
	SPz/H9I3p+f41OsCzAA+dQDMYEx2W/esPJVz12Y3gTF41rer8lCut7XHAMY4PGZ3l5/LLnsc
	GsHUTrMSLT4WVJqdagCSS7mtvojopbblZRJIHOYVNSIIQkStzA8JILYZ14vlxlejXKyPNzEg
	PBnlRFGSJFHMjyZhINDsuB1BIgo5vZ1mAEC0V/XdNU27+6u9KBgrP2il4hRPtwa8FTpLZrra
	7reracaCP5yr0P/uF1wc3jgd/vcLLEMg29DfsXwAAAAASUVORK5CYII=
}]

set icon(wink) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABL1BMVEUJAgBzCgCVEACeEgDD
	JgDXMwDhQwAWgerVTQDXUgDhTQDYUgDoUgDhZgDtYwDjbADlbADlcwHxcwDpgQHxiAHqkgHz
	kgH2lQD0pwH2rAT6rAD5rwDztAL1tAH6tAD4uwL4vQP1vwP1wQP5xQD4xQX7yQL6ywL70AX4
	1Af51wX41wn92AL72gH82gL92gP82wP92wL92wP82g363A393AX+3AP53gz+3gL+3wL84AT+
	3wT+4AP+4AT+4wL54Rv+4wP84wn+5AP+4wr95Ar95QX95A3+5Qr95RL+6AP75xP+6gb87An+
	6Rb+7Qb+7wr97w3+7Bz+7B798Az+8RH98Rf+9hn+9jL+9jX++Cz99z/++Dv++Vr++nP++nr+
	+oH++4H++5v++6H+/L3+/v0BFoEQYJHLAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJ
	cEhZcwAACxMAAAsTAQCanBgAAAAHdElNRQfdBRASJQ+dY8fWAAAA1ElEQVQY02NgAAE5EGCA
	AymdyISkhEgdKShfOiQ+LiExIS4+RBrMF/c34+Cwi42JjorwFwcJKIdqamgZh4d5iiqFKgP5
	Qt6+lskpDsGK/DKWvt5CDAx8QR66yez6XrIcHLoeQXwMDDx+trb2jiBgb2vrx8PAwOZibm5h
	YW1tbcBtbu7CxsDAqqZtYGplY2OjKmCgrcYKNJXLUE9Cwc1ZnVdez5ALZC2zmImTMAenoIqT
	iRgz2GVMIq4+AYEBPq4iTFC3M7JIGrkbSbIwIrzHCAIM2AEACzIhmlVS27AAAAAASUVORK5C
	YII=
}]

set icon(cool) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABAlBMVEUAAAABAAAAAQICAQAD
	AQADAwMDBgkJBQEGBgYLCAJzCgBVIQFaJgCVEACeEgB9KQCBKwBGUmDDJgDXMwDhQwDVTQB9
	b0rhTQDoUgDYbAHtYwDxcwDxiAHzkgH2lQD2rAT6rAD5rwD6tAD3tgH4wwL4wwX5xQD6ywL6
	ywX8zwH70gL41Af41wn92AL82Ab72gH92gP92wL92wP82g393AX+3AP53gz93wX+3wT+4AP+
	4AT54Rv+4wj+4wr95QX95A3+5Qr+5Qv85wn95RL+6Rb86xn+7wr97w3+7Bz+7B798Az98Rf+
	9hn+9jX99z/++nr++oH++4H++5v++6H+/L0AAABk4SurAAAAAXRSTlMAQObYZgAAAAFiS0dE
	AIgFHUgAAADESURBVBgZbcFVEsIwAAXAFwoEiktwDw7Ftbi7Fbj/VWiGGb7YBYSMgJ9U/aq9
	tGs9ha/06XHXntr9cUpDSOx6jI1uI8Z6uwR0+fOyUt1cNtXK8pwHELWTgOVNbMRjIcQeBSJH
	yWoySFnJbDJKxwgQ2roppeFymFLq2oYA/7TEOVeGCue8NPUDvoLS7A5UVR10m0rBByDYarDa
	fDEf9xutIHTOeKfIdLlJJ+6E4IjN1vvDfj2LOfAle5PtVTvplfEjC/jvA0izGyZ66i9MAAAA
	AElFTkSuQmCC
}]

set icon(grin) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAA7VBMVEUAAAlzCgCVEACeEgCP
	GgCnJACvJgDDJgDDNwDDPQDXMwDhQwDUSgDVTQDhTQDYUgDUVQDeUgDoUgDgZgHtYwDxcwDx
	iAHzkgH2lQDwmwLbrCjzqgD2rAT6rAD2rwH5rwD6tAD5xQD7xQH6ywL71Qj51wXv0kr72gH8
	2g363A353gz84AT54Rv84wn95QX95RLV1dX+6QT+6gX+6gb87An+7Qb+7wr97w398Az+8RH9
	8Rff3tr+9hn+9jX99z/w69jt7e3++nr++oH++4H48tj++5v28ub++6H19fX+/L329vb8/Pz+
	/v3+/v4BFoFsNM1HAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAADFSURBVBgZbcHpFoFA
	AAbQL/s2dlFJ9l1KItlCkSHv/zjMcY5f7gWYJoMfeXDw7t5hIONLsS9n7+adL7YCprYeEzI9
	TQkZr2v4aO16lI72I0p7uxaAytKcPF+zzez1nJjLClC2JF1fGIax0HXJKgPFVSSayBBCMolo
	ZFUE0mrsSDWqUY0eY2oaSHUK/cAP/MAP+oVOCkC2nXSdh/Nw3GQ7i49wtZEXr9urmG9Uw2BC
	fFco5UpClw/hi4vXh/NhPc7hh2Pw3xvO/hsmhl11AAAAAABJRU5ErkJggg==
}]

set icon(neutral) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAA8FBMVEUAAABzCgCVEACeEgDD
	JgDXMwDhQwAWgerVTQDhTQDoUgDtYwDxcwDxiAHzkgH2lQD2rAT6rAD5rwD6tAD4wwX5xQD6
	ywL41Af41wn92AL72gH82gL92gP82wP92wL92wP82g393AX+3AP53gz+3gL+3wL93wX84AT+
	3wT+4AP+4AT+4wL54Rv+4wP84wn+4wj+5AP+4wr95QX95A3+5Qr+5Qv85wn95RL+6AP+6gb8
	7An+6Rb+7Qb86xn+7wr97w3+7Bz+7B798Az+8RH98Rf+9hn+9jX99z/++nr++oH++4H++5v+
	+6H+/L3+/v0BFoG2Rz57AAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAADDSURBVBgZbcFn
	F4FQAAbgt0LXTvaMBg17RjZZkf//b3SPc3zyPACVpfCTqVy9l3etZPAlnh937+ndH2cRVHqn
	ENK9dQlRdmkE8u68WNpcNqXi3M0DSKzWHf/dP/Xffme9SgDxo131+fqyzvtV+xgHYlvL6g2p
	nmVtY0BkrKqaZhiGpqnqOAKEC+WGrJumqcuNciEMICrV2oPpbDpo16QoAlyqNcoRQnKjVooD
	xSYnzv6wdyZJFl9MSGgumkKIwQ9D4b8PAV8ZxPlX/uEAAAAASUVORK5CYII=
}]

set icon(sweat) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABNVBMVEUAAAkEN3aVEAAOSkoA
	NeaeEgAAQewSXWOvKAC0KwDDJgC/MAC/MgDBNQDFNwDXMwDPQwBgehPQRgA3iEbhQwDVTQAd
	j5vhTQDoUgDcWgDtYwDjcwHxcwC/lQPpgQHqhAFsuV3xiAHzkgHqmALymADymwCbwUH2rAT6
	rAD5rwD6tADzvwT1wQO70kj6wQH6ywL20A770AX92AL72gH82gL82wP92wP82g393AX+3gL+
	3wL84AT+4AP84Qb84wf+4wL+4wP84wn+5AP95QX95A395RL+6AP+6gb87An+6Rb76hn+7Qb8
	6xn+7wr97w3+7Bz+7B798Az+8RH98Rf+9hn+9jX++Cz99z/++nr++oH++4Hz7+P++5v++6Ho
	9vn+/L3z+v30+vz3+/z5/Pz7/f7+/v5zCgC8pcWKAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgF
	HUgAAADXSURBVBgZbcF3XwFxAAfgrzOyIvIzIysjex3OcWSUlYrMo3n3/l+CfvHxl+cBJAD3
	FI5+ESssxA9xUYjh35d3ul6JW3G1nsZB/fjng1B2mQ0N5s+WT0B5kUp6EqP3UcKTnF3vNLh5
	GtbqjdZbq1GvDe3fKrhfe0VCSo8lQoq9iAJwjgWh+UA1BWHsBGztapjjeJ7nuLtq2waYM3mD
	9ooQYtUZ8xkzAAebvtxIkrQxpVkH/qiDlZzPRVy3uUpQDYoJdPqTl0m/E2BwIOuj5W45qpdx
	IlM4bw9+iSc4Lxsi+wAAAABJRU5ErkJggg==
}]

set icon(confuse) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABAlBMVEUAAABzCgCVEACeEgDD
	JgDXMwDhQwAWgerVTQDhTQDoUgDtYwDjcwHxcwDpegHpfQHphAHxiAHzkgHqmAL2lQD1pwH1
	qgL0rAL2rAT6rAD5rwD6tADzvwT1wQP5xQD6ywL20A792AL82Ab72gH82gL92gP82wP92wL9
	2wP82g393AX+3AP+3gL83gv+3wL84AT+3wT+4AP+4AT+4wL+4wP84wn+5AP95QX95A3+5Qr9
	5RL+6AP+6gb87An+6Rb76hn+6hX+7Qb86xn+7wr97w3+7Bz+7B798Az+8RH98Rf+9hn+9jX+
	+Cz99z/++nr++oH++4H++5v++6H+/L3+/v0BFoGS+FDiAAAAAXRSTlMAQObYZgAAAAFiS0dE
	AIgFHUgAAADFSURBVBgZbcFnF4FgAAbQJyMhexbZqyQq2XtvIv//r+g9zvHJvQCRIfDDla/m
	y7yWOXzxx8fdfJr3x5EHEd9c5rHCrRCbXzZx2HIngc2uzqssK5xyAALTZdt6GwfjbbWX0wDg
	348rlqc2qXmsynjvB3xrXe/2iK6ur30A3ZdlRVFVVVFkuU8D7nyx3uxomtZp1ot5NwBvoxos
	DUMMk6g2vLA5o1KSYcKLVkqKOkE4IoPZdredDSIOfFGutDgS0y4KPxSB/z4p2hzldES9HQAA
	AABJRU5ErkJggg==
}]

set icon(shock) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAA/FBMVEUAAABzCgCVEACeEgDD
	JgDXMwDXRgCbZgnVTQDhTQCVcw+VcxDFdgPohACenkbxiAG/oSLzkgGqsVWhtGPHtCz4qgD2
	rAT6rADbuRvcuRvUuzP1tgH6tADUvz3cwyTjwyD4wwX4xwLyyw73ywTyzxLz1Bb72gH92gP9
	2wP82g383AX93AX22yT93wX+4AP+4AT54Rj+4wj+5AP+4wr95A3+5Qr+5Qv95RL+6gb+6Rb+
	7Bz+7B7+8RH88j3980P+9jX99z/++nr38ND++5v++6H389/38+P59uX+/L339vP6+O78+uz8
	+vD8+vH6+vr7+/r8/Pz9/f3+/f0BFoGqx7P9AAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgA
	AADISURBVBgZbcHVEoJAAAXQi4EYYLfYjYrdgWCjC6v//y+644xPngMwOQY/ybpmXAytnsRX
	vNLfGgdj26/EwWTN56Oz2W8696eZBSCeKaW32q52o5SeRSBg2S/bSq3Tlv2yrQDgPxFCjrF5
	7EgIOfkAj6xf9Ux+mM/oV132AHxUCkmRdq8dkUJSlAfcpXKhqqiqqlQL5ZIbgLfZ6A4m08mg
	22h68eEMK6OiIAjFkRJ2gnEEx4vlarkYBx344lyJ1qyVcHH44Rj89wbDOB2c6ynxVAAAAABJ
	RU5ErkJggg==
}]

set icon(kiss) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABhlBMVEUACQKVEACeEgCkEADe
	FADDJgDjHQDfIADXMwDLPwD4LADPQwDpNwDhQwDUSgD8OQDVTQD8OwD8PQDhTQD7RgHoUgDc
	WgDsWgDtYwDjbwHjcwHxcwDyegDxhAHxiAHwjwDzjwHqlQLqmAP2lQD9kgHwmwL9lQH9mwH9
	ngH9ngL1pwH9pwH2rAT6rAD5rwD6tAD1uQXzvQTzvwX1wwT9vwX9wQL5xQD6xQL9xQL2zQb9
	zQL2zwz9zQb7zwL20A/70AL90gL90gT91AL91QL91QP91wT72gH92Aj92gX82wP82g383gP+
	3gL83wT+3wP+3wT84Ab94AX+4AP+4AT84Qb94An84wf84wj+4wP84wn+5AP94w3+5QP95A39
	5Qv+5A/95RL96Qr+6gb86wn87An+6RX+6Rb76xT+7An+6hX86xb+7Qb+7Qf+6hj87Bv97wz9
	7w3+7Bz77CX+8RD98Rf+8xL+9hn+9jL++Cz99z/++Dv++nP++oH++4H++5v++6H+/L1zCgBd
	cG4/AAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxMAAAsTAQCanBgAAAAH
	dElNRQfdBRASJyuTVkGFAAAA10lEQVQY02NgAAF9EGCAA123yrqGuko3XShfr6S2pq6+rqa2
	RA/MVy6oqCpSsqu2VkwvUAYJmJWVGksZ5ZVnGUpamgH50inZOQFh4ZHF4SG+3snSDAwS+TFB
	9gICnkmeAgK2ieIMDKIZVtraVn4RURE+VtomPAwMvIHqQkIazlp88k7qQiJsDAwc5iqCgpou
	/sGcDmr8XCxAU4Ut1DRtQuO4xTxM1dhB1jIpOLrHp+Zmcoa6yjCDXcYoG51WaMCnGivHCHV7
	I6uOV4KXDmsjwnuNIMCAHQAAFdktFGYZ7rYAAAAASUVORK5CYII=
}]

set icon(razz) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAA/1BMVEUJAgBzCgCVEACeEgDz
	AwOvKADhDwC0KwD7CAjDJgC/MADXMwD+ICD2LgTPQwDQRgDhQwAWgerVTQDhTQDoUgDtYwDx
	cwDxiAHzkgH2lQDymAD2rAT6rAD5rwD6tAD5xQD3xQj6ywL41Af51wX41wn92AL72gH82gL9
	2gP82wP92wP82g363A393AX73A/53gz+3gL+3wL93wX84AT+4AP+4wL54Rv+4wP84wn+4wj+
	5AP+4wr95QX95A395RL+6AP+5xD+6gb87An+6Rb+7Qb+7wr97w398Az+8RH98Rf+9hn+9jX9
	9z/++nr++oH++4H++5v++6H+/L3+/v0BFoFvsQj+AAAAAXRSTlMAQObYZgAAAAFiS0dEAIgF
	HUgAAADGSURBVBgZbcFnF4FgAAbQJyMzI5tI9ibKzAqRVdT//y28xzl9ci9AlAg4uMbdfJv3
	Boefom48zZf5NPQiiJw2ZNnpY8qyQy2HL/5ardR6t16tUr3yANLqcWLZ88vctiZHNQ2kztum
	FW/v23GruT2ngORJlmdLYibLpyQQU0Z5UZQkSRTzIyUGhIR60BuhaTrqC9aFEIBEpzVe+MP+
	xbjVSeDLne2vdoyH2a36WTcIV2Z9GJQHh3XGhR8qUOhuuoUABQdF4L8POV4dB1a7olcAAAAA
	SUVORK5CYII=
}]

set icon(grumpy) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAA81BMVEUAAABzCgCVEACeEgDD
	JgDXMwDhQwAWgerVTQDhTQDoUgDtYwDxcwDxiAHzkgHqmAP2lQD2rAT6rAD5rwD1sQP6tAD3
	uQH3uwHzvQT4wQH4wwL5xQD6xQH6xwH6ywL4zQT6zQT70gL41Af51wX61wP41wz92AL72gH9
	2gP92wL92wP82g393AX+3AP53gz+3gL+3wL93wX+3wT+4AP+4AT84Q/+4wL+4wr95QX95A39
	5RL+6AP87An+6Rb+7Qb+6hj+7wr97w3+7Bz+7B798Az98Rf+9hn99z/++nP++nr++oH++4H+
	+5v++6H+/L3+/v0BFoGnyhXCAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAADHSURBVBgZ
	bcFnF4FQAAbgN6trJJsysneDInuWUOT//xrucY5Pngeg8hR+Mj3He3pOL4OvrO3evYd3d+0s
	qNRRIfz1duWJckzho3xpNUukQ0rN1qUMILHZtf2XJEkvv73bJIC4tRZ8VhRF1hfWVhyIHeaV
	QrHaqBYLlfkhBkQWqqppuq5rmqouIkC41h3IU8MwpvKgWwsDiA77kzohpD7pD6P4CCbHZm62
	n+XMcTIIKsAtt6fzabvkAvhiQunRapQOMfhhKPz3Bh+MGmvAQGI7AAAAAElFTkSuQmCC
}]

set icon(upset) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABRFBMVEUAAABzCgAEN3aVEAAO
	SEoANeaeEgAAQewRXWPDJgC7LgC7MADFNwDFOQDXMwBgehM5iEbhQwAWgerVTQAdj5vhTQDa
	VQDoUgDjZgDtYwDjbAHlbADlcwHxcwC/lQRsuV3xiAHzkgHqmAObwUH2rAT6rAD5rwD6tAD3
	uQH3uwHzvQT5vwH4wQG90kj6wQH4wwL6xQH6xwH6ywL5ywj4zQT6zQj70gL41Af51wX61wP4
	1wz92AL72gH92gP92wL92wP82g393AX+3AP53gz+3gL+3wL93wX+4APs1Xb+4wL+4wr95QX9
	5A3t2Hr95RL+6AP87An+7Qb+7wr97w398Az98Rf+9hn99z/47bb577n++nP++nr++oH++4H+
	+5v++6Ho9vn18+/+/L3z+v30+vz3+/z5/Pz7/f7+/fz+/v3+/v4BFoE9uoiMAAAAAXRSTlMA
	QObYZgAAAAFiS0dEAIgFHUgAAADdSURBVBgZbcFpX8EAAAfg/1KiQrNQJCx3amiuHHNslqPc
	ybVydCzf/32Gn1eeB/gD4FNg6weeTF+aSf2MB2uLm95kLH1I40nPC8W3o0TRw9GQpkodwxw4
	1NwnE0HqiQomkm/nn8ewN1specmy7FJOtS6+jnDVbYRkI8MwRjnUuD0ALO1K2B+IxCMBf7jS
	tgBkjecFQRRFQeD5Ggnoo+lc8e7SbHso5tJRPQATly2fPE9fzspZzoQVtatQPX3/HeiqBZca
	CpWz/mg1X8fqThU2CK07/5p3awnsEArs9w8yVCaIKaSD8QAAAABJRU5ErkJggg==
}]

set icon(cry) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAA/1BMVEUJAgCVEACeEgDDJgDX
	MwDhQwDVTQDhTQDoUgDtYwDjcwHxcwDpfQHvfQDwjwLqmAL2lQD6rAD2rwH5rwD6tADzvwT1
	wQP5wwb5xQD3xQf3xQj7xwH7yQL6ywL7ywL7zQH7zwH20A771An72Av72gH82gL82wP92wL8
	3gv+3wL84AT+3wT+4AP+4AT84Qb84wf84wn+5AP94w/95QX94xH95A3+6APlz8/+6gb87An7
	6hn+7Qb86xn+7wr97w398Az+8RH98Rf+8CH+9hn+9jX++Cz99z/++nr++oH+95X++4H++JX+
	+5v++6Hz8/z+/L34+P37+/39/f7+/v5zCgDaeqG3AAAAAXRSTlMAQObYZgAAAAFiS0dEAIgF
	HUgAAADGSURBVBgZbcFnF4FQAAbg17qozOwdMtKwN9lCsvr/v4V7nOOT5wGoGIWfSPVg3sxD
	NYIvfn02zKtpnNc8qPByPw0JRyE03S/D+EhtE0x8vpvHmcQ2BYCdzCRVa6/amirNJizg14ci
	IbVxjRBxqPsB36L+Ol02nc3l9KovfICnV3yWmrIsN0vPYs8DuPPZR6GlKEqr8Mjm3QC8uXuU
	UNF7zosPZ8DiGt1+t8FZAScox2hQSWfSlcHIgS/bFUyWk0GXjR+bwn9vq84coPQ7sO0AAAAA
	SUVORK5CYII=
}]

set icon(yell) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABEVBMVEUAAACVEAC5AAC/AACe
	EgCPGgC0CACSGwC5CQDYAADgAADtAQHyAgL1AwOvKACxKAC0KQC0KwC2KwDDJgDXMwDPQwDQ
	RgDhQwDVTQDYTwDhTQDaUgDoUgDtYwDjbAHobADpbADjcwHxcwDxiAHqlQLzkgH2lQDlqhXm
	rBbzrAP2rAT6rAD5rwD6tADzuwP1vwP5xQD2yQf6ywL2zQf81AT92AL72gH92wP82g3+3gL9
	3gr84AT+4wL95QX85wn95RL+6AP+6QT+6gX+6gb87An+7Qb+7wr97w377Cb98Az+8RH98Rf+
	9hn+9jX99z/++nP++nr++oH++4H179v++5v++6H19fT+/L34+Pj+/v5zCgCWQ8xyAAAAAXRS
	TlMAQObYZgAAAAFiS0dEAIgFHUgAAADGSURBVBgZbcHVEoJAAAXQa4sdqCs2dncnoohdWPz/
	h+iOMz55DkCVKfwUOwfloRw6RXyVtpezclPOl20JVGbVTMnH01FONVcZfNR2k0qivq8nKpNd
	DQC/WA4JqW6qhAyXCx6IrGeCMBdFcS4Is3UECEsjh8NHCPG7nSMpDATGfcP9Rd1N/XEAYBtt
	l5ax2m0WnafdYAFw3ZiRsdptFnO8y+FDn24Fs9fnNRdqpfWgNMlBPkqi+UFSgy/VW+hNewWv
	ih+Vwn9vTKgh2J1BLz4AAAAASUVORK5CYII=
}]

set icon(surprise) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABCFBMVEUAAABzCgCVEACeEgCh
	IQCvKAC0KwDDJgDBMwDXMwDNQQDSSADhQwAWgerUTQDVTQDhTQDaVwDoUgDtYwDldgHxcwDn
	fQHxiAHzkgH2lQD2rAT6rAD5rwD6tAD5xQD6ywL41Af51wX41wn92AL72gH82gL92gP82wP9
	2wL92wP82g363A382w393AX+3AP83A353gz+3gL+3wL93wX84AT+4AP+4wL54Rv+4wP84wn+
	5AP+4wr95QX95A395RL+6AP+6gb87An+7Qb+7wr97w314W/98Az+8RH98Rf25HP+9hn+9jX9
	9z/++nr++oH++4H++5v++6H+/L37+vn7+/v+/fv+/v0BFoEt+tmtAAAAAXRSTlMAQObYZgAA
	AAFiS0dEAIgFHUgAAADGSURBVBgZbcHVEoJAAAXQiy3GqhjYit0Cgt3dWPj/f6I7zPjkOQCV
	pPATz5+0u3bKx2FI7K4X7aZdrrsEqOiqTUj33CWkvYriK3XIZoTasSZksocUgNB80dHf/W3/
	rXcW8xDAbaYF3VOalTx6YbrhgMBSVXsDqqeqywDgHomiJCmKIkmiOHIDznSu3LSxxOVolnNp
	JwBfpShbHs+HVS5WfPgy840hu37tvcMGbwZliozrQX+4NY6YYGDsseqkGrMz+GEo/PcBhm4d
	1stpTEMAAAAASUVORK5CYII=
}]

set icon(red) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAA+VBMVEUAAABzCgCVEAChEgDD
	JgDXMADgMwDhPQAWger8OQDVTQD8PQDnTQDtYwDjcwHxcwD5dgHyegHxhAHqmAL2lQD9lQH9
	mAH9ngH9pwH2rAT8qgL6rAD5rwD6tADzvwT1wQP4wQX5xQD9xwT9zQL7zwL20A790gT91AP9
	1QL72gH82wP82g383wT93gj93gn+3gj+3wP+3wT+4AP+4AT+4wP84wn+5AP95A395Qr+5Qv9
	5RL+6gb87An+6Rb+7Ab76hn97Av86xn97w3+7Bz+7B7+8RH98Rf+9hn+9jX++Cz99z/++nr+
	+oH++4H++5v++6H+/L3+/v0BFoH/FNIIAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAADI
	SURBVBgZbcFnF4FgAAbQJ7uhrMgskj2SFbJnRuT//xje45w+uRcgigQ8+ebVeTnXZh4/hdPj
	7jyd++NUAJHa2eukdtOSa3uXwlf5LPOl7WVb4uVzGYBgrTrue3wcv93OyhKA6MaoupHWohVx
	q8YmCnB9SVL10WSkq5LU54BwjmUzDd3QGxmWzYWBYIym0/XBcFBP03QsCCAkipWuOTO7FVEM
	4csfr/UUhmGUXi3uB+FLTJf7w345TfjwQwWy7Xk7G6DgoQj89wHhFxp96tgT7gAAAABJRU5E
	rkJggg==
}]

set icon(sleep) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABRFBMVEUAAAADAQANBwAkFgI7
	DwArHgZPEgBzCgCVEACeEgBdQwqkIQCxJADDJgDXMwDhQwDVTQDhTQDoUgDcXQDfYwDtYwDx
	cwDsgQDshADxiAHzkgH2lQDppwHwpwP1pwHzrAP2rAT5rwD6tAD3uQH3uwH4wQH4wQL6wQH4
	wwL5xQD6xQL6yQL5ywL6ywL7ywP5zwP50AT50AXv1BP92AL82gP92gP82wP92wL92wP93AXz
	10P+3gL+3wL93wX+4AP+4AT+4wL+4wP+4wj+5AP+4wr95QX95A3+5Qr+5Qv+6AP+6QT+5xD+
	6gb87An+6Rb+6hX+7Qb+6hj87Q/+7gj+7wr97w398Az+8RD+8xL+9R3+9hn+9in99iz+9jL+
	9jX++Cz99z/++Dv++Vr++nP++nr++oH++4H++5v++6H+/L39+/gAAAD29u7wAAAAAXRSTlMA
	QObYZgAAAAFiS0dEAIgFHUgAAADVSURBVBgZbcFXe8FgAAbQV9HYPmoXnTQ2tRsjxGir6LD3
	rJH/f0+e9nHlHIDnecm9ACci+2yz28y+8U/0/LuWbqVrxegOgssLsXixXC0X82n7GkeeQa8/
	HE/Gw35v4AFgqn/+hAiJdMOE+OsmwNh5e4/RNP2RcLtcHSOgb7FsgeMeOa7APrT0gLbMMM5M
	zpJzZhhzWQtoAtGkl5Bg3kfIU0ADQJeKZ4vV12oxG0/pcCS3pUu15lezVkrb5BDIrJWG48pw
	U7HK8IdS3ypf9io1hRNKgPMOtL4kM4F4OwgAAAAASUVORK5CYII=
}]

set icon(eek) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABDlBMVEUACQJzCgCVEACeEgDD
	JgC7LgC7MADFNwDFOQDXMwDhQwAWgerVTQDhTQDaVQDoUgDjZgDtYwDjbAHlbADlcwHxcwDx
	iAHzkgH2lQD2rAT6rAD5rwD6tAD5vwH6wQH5xQD6ywL5ywj6zQj41Af41wn92AL72gH82gL9
	2gP82wP92wL92wP82g393AX+3AP53gz+3gL+3wL93wX84AT+4APs1Xb+4wL54Rv+4wP84wn+
	5AP+4wr95QX95A3t2Hr85wn95RL+6AP+6gb87An+7Qb86xn+7wr97w398Az+8RH98Rf+9hn+
	9jX99z/47bb577n++nr++oH++4H++5v++6H18+/+/L3+/fz+/v0BFoFE2bMyAAAAAXRSTlMA
	QObYZgAAAAFiS0dEAIgFHUgAAADGSURBVBgZbcHVEoJQAAXAg61gt9jdAiJ2CwY2Bv7/j+gd
	Z3xyFyDiBH5ixYP20A7FGL7Y3fWi3bTLdceCiKxbDNM9dxmmtY7gI6VOc3l5L+dzUzUFILhc
	tfVXf9t/6e3VMgh4N/OSTlcWFVovzTdewK1IUm9A9CRJcQOOEcfxvCiKPM9xIwdgTxeqzYTH
	5U82q4W0HYCzVhbMnfvEKpRrTnwYw42h5fg82YaNsBGEITTO+FyB7DhkwBdlitZn9aiJwg9F
	4L83ER8edKOd9xYAAAAASUVORK5CYII=
}]

set icon(kitty) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABAlBMVEUAAABzCgCVEACeEgC5
	JQDDJgC2LgC5LgDFLgDFOQDhQwAWgerVTQDXTQDhTQDgYwDtYwDxcwDxiAHpjwHzkgH2lQD4
	oQD0pAH1pwH3rAD2rAT6rADzsQT2sQL6tAD4vQH5wQP5wwP5xQD41Af71QD51wX41wn72gH5
	2gj82gL92gP82wP92wL92wP82g363A393AX+3gL+3wL84AT+3wT+4AP+4AT+4wL54Rv+4wP8
	4wn+5AP95QX95A395RL+6AP+6gb87An+6Rb+7Qb+7Bz+7B798Az+8RH+9hf+9jX99z/++jf+
	+2n+/Gz++nr++oH++33++5v++6H+/L3+/v0BFoGTwgHUAAAAAXRSTlMAQObYZgAAAAFiS0dE
	AIgFHUgAAADOSURBVBgZbcFnF4FgAAbQJ3tkZCfKJgpF2XtnRv7/X/F2nOOTewEeAG8DwIPg
	KlzdMF+mUecqHIjU9fK4m0/z/rhcUyCS5wZND29Dmm6ckyBKx2q51jl1auXqsQSAWW161nt8
	GL+t3mbFANH9QrSCrWUraImLfRTwxYXBaGIbDYS4DwhMFUVVNU1TVUWZBgBXhJXSfT2j99MS
	G3EBCMvNUKHoLxZCTTkMwpno5j3enNeT7yacsDlis/V2t13PYg58Ue5se97Ouin8UDb89wE6
	kB3rbeg2wgAAAABJRU5ErkJggg==
}]

set icon(roll) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABL1BMVEUJAgBzCgCVEACeEgDD
	JgDXMwDhQwDVTQDhTQDoUgDtYwDxcwDrfQDsiADxiAHsiwHviwDvjwHvkgHzkgH2lQD0oQH0
	pAH2rAT6rAD5rwD0tAH6tADzuwT0uwP0vQT5vQH5vwH4wwX5xQD2yQT5yQL5ywL6ywL3zQf8
	1AH81wL91wL92AL72gH62gb92gL92gP92wL92wP82g393AX+3AP83gP53gz93wX+3wT+4AP+
	4AT84wj54Rv+4wj+5AP+4wr95QX95A3+5Qr+5Qv95RL+6gb87An+6Rb86hj87Bv+7wr97w3+
	7Bz+7B798Az+8RH98Rf98hX15IT+9Rf+9jX36Y/99z/++nr++oH++4H89b3++5v89cP++6H+
	/L349/X8+/r8/Pr8/Pz+/v0BFoGg/IktAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJ
	cEhZcwAACxMAAAsTAQCanBgAAAAHdElNRQfdBRASIh64knHjAAAA0UlEQVQY022PxRLCUAxF
	U+zh7k5xdytuxd0pDv//DfQxDGw4iztzskhuADAWDHwxRjbMmdlEjB83zfY75sTs9jPT27XD
	HELlbRmh3FCLB+Tcbff3Vz2rPbokWZfQ5sPjWZrajo9nlpYAiCey6/3m7SjY9EzEAKKBenFZ
	O6s4HQMRgKAeUEr1sWJQJdWF6gIAvi+cyBQoiipkEmEfn90qTMbzlWarWcnHk0J8lqtJ11wI
	IVctreG+m3Hkje5oPOo25JxPd4JnSLVTBh7xe4/AwH9eTGwg2/3/H4sAAAAASUVORK5CYII=
}]

set icon(blink) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAA7VBMVEUAAAmVEACeEgDDJgDX
	MwDhQwDVTQDhTQDoUgDtYwDxcwDxiAHzkgH2lQD2rAT6rAD5rwD6tAD4wwX5xQD6ywL51AX9
	2AL72gH82gL92gP82wP92wL52w392wP82g393AX+3AP+3gL+3wL93wX84AT+3wT+4AP+4AT8
	4Qb84wf+4wL+4wP84wn+4wj+5AP+4wr95QX95A3+5Qr+5Qv95RL+6AP+6gb87An+6Rb+7Qb+
	7wr97w3+7Bz+7B798Az+8RH98Rf+9hn+9jX++Cz99z/++Dv++Vr++nP++nr++oH++4H++5v+
	+6H+/L1zCgD1jj7pAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAADCSURBVBgZbcFnF4Fg
	GAbg23qyd8iuKCJl71kZRf//5/Ae5/jkugAmx+AnU7a9l2eXM/jKntyH9/Qe7ikLJrW1nOvt
	frs61jaFj8KZJ6pf6kT8uQAgvlx3ev3hcdjvddbLOBA7zCtE1UWVqDI/xIDoRtMGI2agaZso
	EDFFUZIURZEkUTQjQLhYqrW7qqp227VSMQyAawiyPplOdFlocPgIJltGnojyRisZBBNIjFe7
	/W41TgTw5YfSzVkzHfLx4zP47w1brBoN40X00QAAAABJRU5ErkJggg==
}]

set icon(glasses) [image create photo -data {
	iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAA6lBMVEUAAAAKCgoBFoFzCgAr
	KyuVEACeEgBzLAHDJgDXMwDhQwAWgerVTQDhTQDoUgDtYwDpegG0kimnlV3xiAGqnmzcmwbz
	kgHcngnDoT/FpEHaryjasSv2rAT6rAD5rwD6tADowRzowR34wQH4wwL4xwL6ywL6ywX4zQT4
	0gX41Af92AL82Ab72gH72gX92gP92wL92wP82g393AX+3AP63gr+3wT+4AP+4AT+4wL+4wP+
	5AP95A3+5Qr+5Qv95RL+6gb+6Rb+7Bz+7B798Rf+9jX99z/++nr++oH++4H++5v++6H+/L3+
	/v0AAAD5bYD+AAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAC+SURBVBgZbcHpEoFgGAbQ
	51W27AqRnZQtu0T2raj7vx19Y8Yv5wBMg8FPbXR33+59VMNX/eo83Zf7dK51MFEK5B45CkQR
	oILeL9GNSn29QAB86nohjYi0kNclH8jwbS/R4Y98J+G1+QyQlpobq1Xe5lvWpimlAWFZkWVl
	vR4rslxZCkBcHUwWK9M0V4vJQI0DSE2N7NDe28OsMU0hECnOe0mmNy9GwITF3el8OZ92Yhhf
	XKw6O8yqMQ4/HIP/Po0BFwJFfgMGAAAAAElFTkSuQmCC
}]

} ;# namespace emoticons

# vi:set ts=3 sw=3:
