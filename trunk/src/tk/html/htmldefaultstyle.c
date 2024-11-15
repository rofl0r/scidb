
#define HTML_DEFAULT_TCL \
        "# ======================================================================\n" \
        "# Author : $Author$\n" \
        "# Version: $Revision: 1001 $\n" \
        "# Date   : $Date: 2014-05-21 18:41:00 +0000 (Wed, 21 May 2014) $\n" \
        "# Url    : $URL$\n" \
        "# ======================================================================\n" \
        "\n" \
        "#\n" \
        "# tkhtml.tcl --\n" \
        "#\n" \
        "#     This file contains:\n" \
        "#\n" \
        "#         - The default bindings for the Html widget, and\n" \
        "#         - Some Tcl functions used by the stylesheet html.css.\n" \
        "#\n" \
        "# ------------------------------------------------------------------------\n" \
        "#\n" \
        "# Copyright (c) 2005 Eolas Technologies Inc.\n" \
        "# All rights reserved.\n" \
        "# \n" \
        "# This Open Source project was made possible through the financial support\n" \
        "# of Eolas Technologies Inc.\n" \
        "# \n" \
        "# Redistribution and use in source and binary forms, with or without\n" \
        "# modification, are permitted provided that the following conditions are met:\n" \
        "# \n" \
        "#     * Redistributions of source code must retain the above copyright\n" \
        "#       notice, this list of conditions and the following disclaimer.\n" \
        "#     * Redistributions in binary form must reproduce the above copyright\n" \
        "#       notice, this list of conditions and the following disclaimer in the\n" \
        "#       documentation and/or other materials provided with the distribution.\n" \
        "#     * Neither the name of the <ORGANIZATION> nor the names of its\n" \
        "#       contributors may be used to endorse or promote products derived from\n" \
        "#       this software without specific prior written permission.\n" \
        "# \n" \
        "# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n" \
        "# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n" \
        "# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n" \
        "# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE\n" \
        "# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n" \
        "# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\n" \
        "# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\n" \
        "# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\n" \
        "# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n" \
        "# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n" \
        "# POSSIBILITY OF SUCH DAMAGE.\n" \
        "#\n" \
        "\n" \
        "switch -- $::tcl_platform(platform) {\n" \
        "  windows {\n" \
        "    bind Html <MouseWheel>   { %W yview scroll [expr %D/-30] units }\n" \
        "  }\n" \
        "  macintosh {\n" \
        "    bind Html <MouseWheel>   { %W yview scroll [expr %D*-4] units }\n" \
        "  }\n" \
        "  default {\n" \
        "    # Assume X windows by default.\n" \
        "    bind Html <ButtonPress-4>   { %W yview scroll -4 units }\n" \
        "    bind Html <ButtonPress-5>   { %W yview scroll  4 units }\n" \
        "  }\n" \
        "}\n" \
        "\n" \
        "\n" \
        "# Some Tcl procs used by html.css\n" \
        "#\n" \
        "namespace eval tkhtml {\n" \
        "\n" \
        "    # This is called for <input type=text> tags that have a size\n" \
        "    # attribute. The size attribute in this case is supposed to be\n" \
        "    # the width in characters.\n" \
        "    proc inputsize_to_css {} {\n" \
        "        upvar N node\n" \
        "        set size [$node attr size]\n" \
        "        catch {\n" \
        "          if {$size < 0} {error \"Bad value for size attribute\"}\n" \
        "        }\n" \
        "\n" \
        "        # Figure out if we are talking characters or pixels:\n" \
        "        switch -- [string tolower [$node attr -default text type]] {\n" \
        "          text     { \n" \
        "            incr size [expr {int(($size/10)+1)}]\n" \
        "            set units ex \n" \
        "          }\n" \
        "          password { \n" \
        "            incr size [expr {int(($size/10)+1)}]\n" \
        "            set units ex \n" \
        "          }\n" \
        "          file     { \n" \
        "            incr size 10 \n" \
        "            set units ex \n" \
        "          }\n" \
        "          default  { set units px }\n" \
        "        }\n" \
        "\n" \
        "        return \"${size}${units}\"\n" \
        "    }\n" \
        "\n" \
        "    proc if_disabled {if else} {\n" \
        "      upvar N node\n" \
        "      set disabled [$node attr -default 0 disabled]\n" \
        "      if {$disabled} {return $if}\n" \
        "      return $else\n" \
        "    }\n" \
        "    \n" \
        "    # The following two procs are used to determine the width and height of\n" \
        "    # <textarea> markups. Technically speaking, the \"cols\" and \"rows\"\n" \
        "    # attributes are compulsory for <textarea> elements.\n" \
        "    proc textarea_width {} {\n" \
        "        upvar N node\n" \
        "        set cols [$node attr -default \"\" cols]\n" \
        "        if {[regexp {[[:digit:]]+}] $cols} { return \"${cols}ex\" }\n" \
        "        return $cols\n" \
        "    }\n" \
        "    proc textarea_height {} {\n" \
        "        upvar N node\n" \
        "        set rows [$node attr -default \"\" rows]\n" \
        "        if {[regexp {[[:digit:]]+} $rows]} { return \"[expr ${rows} * 1.2]em\" }\n" \
        "        return $rows\n" \
        "    }\n" \
        "\n" \
        "    proc size_to_fontsize {} {\n" \
        "        upvar N node\n" \
        "        set size [$node attr size]\n" \
        "\n" \
        "        if {![regexp {([+-]?)([0123456789]+)} $size dummy sign quantity]} {\n" \
        "          error \"not an integer\"\n" \
        "        }\n" \
        "\n" \
        "        if {$sign eq \"\"} {\n" \
        "            switch -- $quantity {\n" \
        "                1 {return xx-small}\n" \
        "                2 {return small}\n" \
        "                3 {return medium}\n" \
        "                4 {return large}\n" \
        "                5 {return x-large}\n" \
        "                6 {return xx-large}\n" \
        "                default { error \"out of range: $size\" }\n" \
        "            }\n" \
        "        }\n" \
        "\n" \
        "        if {$sign eq \"-\"} {\n" \
        "            if {$quantity eq \"1\"} {return smaller}\n" \
        "            return \"[expr 100 * pow(0.85, $quantity)]%\"\n" \
        "        }\n" \
        "\n" \
        "        if {$sign eq \"+\"} {\n" \
        "            if {$quantity eq \"1\"} {return larger}\n" \
        "            return \"[expr 100 * pow(1.176, $quantity)]%\"\n" \
        "        }\n" \
        "\n" \
        "        error \"logic error\"\n" \
        "    }\n" \
        "\n" \
        "    proc vscrollbar {base node} {\n" \
        "      set sb [scrollbar ${base}.vsb_[string map {: _} $node]]\n" \
        "      $sb configure -borderwidth 1 -highlightthickness 0 -command \"$node yview\"\n" \
        "      return $sb\n" \
        "    }\n" \
        "    proc hscrollbar {base node} {\n" \
        "      set sb [scrollbar ${base}.hsb_[string map {: _} $node] -orient horiz]\n" \
        "      $sb configure -borderwidth 1 -highlightthickness 0 -command \"$node xview\"\n" \
        "      return $sb\n" \
        "    }\n" \
        "\n" \
        "    proc ol_liststyletype {} {\n" \
        "      switch -exact -- [uplevel {$N attr type}] {\n" \
        "        i {return lower-roman}\n" \
        "        I {return upper-roman}\n" \
        "        a {return lower-alpha}\n" \
        "        A {return upper-alpha}\n" \
        "        1 {return decimal}\n" \
        "      }\n" \
        "      error \"Unrecognized type attribute on OL element\"\n" \
        "    }\n" \
        "}\n" \
        "\n" \
        "\n" \



#define HTML_DEFAULT_CSS \
        "/* ======================================================================\n" \
        " * Author : $Author$\n" \
        " * Version: $Revision: 1001 $\n" \
        " * Date   : $Date: 2014-05-21 18:41:00 +0000 (Wed, 21 May 2014) $\n" \
        " * Url    : $URL$\n" \
        " * ======================================================================\n" \
        " */\n" \
        "\n" \
        "/* Display types for non-table items. */\n" \
        "  ADDRESS, BLOCKQUOTE, BODY, DD, DIV, DL, DT, FIELDSET, \n" \
        "  FRAME, H1, H2, H3, H4, H5, H6, NOFRAMES, \n" \
        "  OL, P, UL, APPLET, CENTER, DIR, HR, MENU, PRE, FORM\n" \
        "                { display: block }\n" \
        "\n" \
        "HEAD, SCRIPT, TITLE { display: none }\n" \
        "BODY {\n" \
        "  margin:8px;\n" \
        "}\n" \
        "\n" \
        "/* Rules for unordered-lists */\n" \
        "LI                   { display: list-item }\n" \
        "UL[type=\"square\"]>LI { list-style-type : square } \n" \
        "UL[type=\"disc\"]>LI   { list-style-type : disc   } \n" \
        "UL[type=\"circle\"]>LI { list-style-type : circle } \n" \
        "LI[type=\"circle\"]    { list-style-type : circle }\n" \
        "LI[type=\"square\"]    { list-style-type : square }\n" \
        "LI[type=\"disc\"]      { list-style-type : disc   }\n" \
        "\n" \
        "OL, UL, DIR, MENU, DD  { padding-left: 40px ; margin-left: 1em }\n" \
        "\n" \
        "OL[type]         { list-style-type : tcl(::tkhtml::ol_liststyletype) }\n" \
        "\n" \
        "NOBR {\n" \
        "  white-space: nowrap;\n" \
        "}\n" \
        "\n" \
        "/* Map the 'align' attribute to the 'float' property. Todo: This should\n" \
        " * only be done for images, tables etc. \"align\" can mean different things\n" \
        " * for different elements.\n" \
        " */\n" \
        "TABLE[align=\"left\"]       { float:left } \n" \
        "TABLE[align=\"right\"]      { \n" \
        "    float:right; \n" \
        "    text-align: inherit;\n" \
        "}\n" \
        "TABLE[align=\"center\"]     { \n" \
        "    margin-left:auto;\n" \
        "    margin-right:auto;\n" \
        "    text-align:inherit;\n" \
        "}\n" \
        "IMG[align=\"left\"]         { float:left }\n" \
        "IMG[align=\"right\"]        { float:right }\n" \
        "\n" \
        "/* If the 'align' attribute was not mapped to float by the rules above, map\n" \
        " * it to 'text-align'. The rules above take precedence because of their\n" \
        " * higher specificity. \n" \
        " *\n" \
        " * Also the <center> tag means to center align things.\n" \
        " */\n" \
        "[align=\"right\"]              { text-align: -tkhtml-right }\n" \
        "[align=\"left\"]               { text-align: -tkhtml-left  }\n" \
        "CENTER, [align=\"center\"]     { text-align: -tkhtml-center }\n" \
        "\n" \
        "/* Rules for unordered-lists */\n" \
        "/* Todo! */\n" \
        "\n" \
        "TD, TH {\n" \
        "  padding: 1px;\n" \
        "  border-bottom-color: grey60;\n" \
        "  border-right-color: grey60;\n" \
        "  border-top-color: grey25;\n" \
        "  border-left-color: grey25;\n" \
        "}\n" \
        "\n" \
        "/* For a horizontal line, use a table with no content. We use a table\n" \
        " * instead of a block because tables are laid out around floating boxes, \n" \
        " * whereas regular blocks are not.\n" \
        " */\n" \
        "/*\n" \
        "HR { \n" \
        "  display: table; \n" \
        "  border-top: 1px solid grey45;\n" \
        "  background: grey80;\n" \
        "  height: 1px;\n" \
        "  width: 100%;\n" \
        "  text-align: center;\n" \
        "  margin: 0.5em 0;\n" \
        "}\n" \
        "*/\n" \
        "\n" \
        "HR {\n" \
        "  display: block;\n" \
        "  border-top:    1px solid grey45;\n" \
        "  border-bottom: 1px solid grey80;\n" \
        "  margin: 0.5em auto 0.5em auto;\n" \
        "}\n" \
        "\n" \
        "/* Basic table tag rules. */\n" \
        "TABLE { \n" \
        "  display: table;\n" \
        "  border-spacing: 2px;\n" \
        "  table-layout: auto;\n" \
        "\n" \
        "  border-bottom-color: grey25;\n" \
        "  border-right-color: grey25;\n" \
        "  border-top-color: grey60;\n" \
        "  border-left-color: grey60;\n" \
        "\n" \
        "  /* <table> elements do not inherit text-align by default. Strictly\n" \
        "   * speaking, this rule should not be used with documents that\n" \
        "   * use the \"strict\" DTD. Or something.\n" \
        "   */\n" \
        "  text-align: left;\n" \
        "}\n" \
        "\n" \
        "TR              { display: table-row }\n" \
        "THEAD           { display: table-header-group }\n" \
        "TBODY           { display: table-row-group }\n" \
        "TFOOT           { display: table-footer-group }\n" \
        "COL             { display: table-column }\n" \
        "COLGROUP        { display: table-column-group }\n" \
        "TD, TH          { display: table-cell }\n" \
        "CAPTION         { display: table-caption }\n" \
        "TH              { font-weight: bolder; text-align: center }\n" \
        "CAPTION         { text-align: center }\n" \
        "\n" \
        "H1              { font-size: 2em; margin: .67em 0 }\n" \
        "H2              { font-size: 1.5em; margin: .83em 0 }\n" \
        "H3              { font-size: 1.17em; margin: 1em 0 }\n" \
        "H4, P,\n" \
        "BLOCKQUOTE, UL,\n" \
        "FIELDSET, \n" \
        "OL, DL, DIR,\n" \
        "MENU            { margin-top: 1.0em; margin-bottom: 1.0em }\n" \
        "H5              { font-size: .83em; line-height: 1.17em; margin: 1.67em 0 }\n" \
        "H6              { font-size: .67em; margin: 2.33em 0 }\n" \
        "H1, H2, H3, H4,\n" \
        "H5, H6, B,\n" \
        "STRONG          { font-weight: bolder }\n" \
        "BLOCKQUOTE      { margin-left: 40px; margin-right: 40px }\n" \
        "I, CITE, EM,\n" \
        "VAR, ADDRESS    { font-style: italic }\n" \
        "PRE, TT, CODE,\n" \
        "KBD, SAMP       { font-family: courier }\n" \
        "BIG             { font-size: 1.17em }\n" \
        "SMALL           { font-size: .83em }\n" \
        "SUB             { font-size: 0.67em; vertical-align: sub }\n" \
        "SUP             { font-size: 0.67em; vertical-align: super }\n" \
        "S, STRIKE, DEL  { text-decoration: line-through }\n" \
        "OL              { list-style-type: decimal }\n" \
        "OL UL, UL OL,\n" \
        "UL UL, OL OL    { margin-top: 0; margin-bottom: 0 }\n" \
        "U, INS          { text-decoration: underline }\n" \
        "ABBR, ACRONYM   { font-variant: small-caps; letter-spacing: 0.1em }\n" \
        "\n" \
        "/* Formatting for <pre> etc. */\n" \
        "PRE, PLAINTEXT, XMP { \n" \
        "  display: block;\n" \
        "  white-space: pre;\n" \
        "  margin: 1em 0;\n" \
        "  font-family: courier;\n" \
        "}\n" \
        "\n" \
        "/* Display properties for hyperlinks */\n" \
        ":link    { color: darkblue; text-decoration: underline ; cursor: pointer }\n" \
        ":visited { color: purple; text-decoration: underline ; cursor: pointer }\n" \
        "\n" \
        "/* Deal with the \"nowrap\" HTML attribute on table cells. */\n" \
        "TD[nowrap] ,     TH[nowrap]     { white-space: nowrap; }\n" \
        "TD[nowrap=\"0\"] , TH[nowrap=\"0\"] { white-space: normal; }\n" \
        "\n" \
        "BR { \n" \
        "    display: block;\n" \
        "}\n" \
        "/* BR:before       { content: \"\\A\" } */\n" \
        "\n" \
        "/*\n" \
        " * Default decorations for form items. \n" \
        " */\n" \
        "INPUT[type=\"hidden\"] { display: none }\n" \
        "INPUT[type] { border: none }\n" \
        "\n" \
        "INPUT, INPUT[type=\"file\"], INPUT[type=\"text\"], INPUT[type=\"password\"], \n" \
        "TEXTAREA, SELECT { \n" \
        "  border: 2px solid;\n" \
        "  border-color: #848280 #ececec #ececec #848280;\n" \
        "  line-height: normal;\n" \
        "}\n" \
        "\n" \
        "INPUT[type=\"image\"][src] { -tkhtml-replacement-image: attr(src) }\n" \
        "\n" \
        "/*\n" \
        " * Default style for buttons created using <input> elements.\n" \
        " */\n" \
        "INPUT[type=\"submit\"],INPUT[type=\"button\"] {\n" \
        "  display: -tkhtml-inline-button;\n" \
        "  border: 2px solid;\n" \
        "  border-color: #ffffff #828282 #828282 #ffffff;\n" \
        "  background-color: #d9d9d9;\n" \
        "  color: #000000;\n" \
        "  /* padding: 3px 10px 1px 10px; */\n" \
        "  padding: 3px 3px 1px 3px;\n" \
        "  white-space: nowrap;\n" \
        "  color:               tcl(::tkhtml::if_disabled #666666 #000000);\n" \
        "}\n" \
        "INPUT[type=\"submit\"]:after,INPUT[type=\"button\"]:after {\n" \
        "  content: attr(value);\n" \
        "  position: relative;\n" \
        "}\n" \
        "\n" \
        "INPUT[type=\"submit\"]:hover:active,INPUT[type=\"button\"]:hover:active {\n" \
        "  border-top-color:    tcl(::tkhtml::if_disabled #ffffff #828282);\n" \
        "  border-left-color:   tcl(::tkhtml::if_disabled #ffffff #828282);\n" \
        "  border-right-color:  tcl(::tkhtml::if_disabled #828282 #ffffff);\n" \
        "  border-bottom-color: tcl(::tkhtml::if_disabled #828282 #ffffff);\n" \
        "}\n" \
        "\n" \
        "INPUT[size] { width: tcl(::tkhtml::inputsize_to_css) }\n" \
        "\n" \
        "BUTTON {\n" \
        "  white-space:nowrap;\n" \
        "  border-width: 2px;\n" \
        "  border-style: solid;\n" \
        "}\n" \
        "\n" \
        "/* Handle \"cols\" and \"rows\" on a <textarea> element. By default, use\n" \
        " * a fixed width font in <textarea> elements.\n" \
        " */\n" \
        "TEXTAREA[cols] { width: tcl(::tkhtml::textarea_width) }\n" \
        "TEXTAREA[rows] { height: tcl(::tkhtml::textarea_height) }\n" \
        "TEXTAREA {\n" \
        "  font-family: fixed;\n" \
        "}\n" \
        "\n" \
        "FRAMESET {\n" \
        "  display: none;\n" \
        "}\n" \
        "\n" \
        "/* Default size for <IFRAME> elements */\n" \
        "IFRAME {\n" \
        "  width: 300px;\n" \
        "  height: 200px;\n" \
        "}\n" \
        "\n" \
        "\n" \
        "/*\n" \
        " *************************************************************************\n" \
        " * Below this point are stylesheet rules for mapping presentational \n" \
        " * attributes of Html to CSS property values. Strictly speaking, this \n" \
        " * shouldn't be specified here (in the UA stylesheet), but it doesn't matter\n" \
        " * in practice. See CSS 2.1 spec for more details.\n" \
        " */\n" \
        "\n" \
        "/* 'color' */\n" \
        "[color]              { color: attr(color) }\n" \
        "body a[href]:link    { color: attr(link x body) }\n" \
        "body a[href]:visited { color: attr(vlink x body) }\n" \
        "\n" \
        "/* 'width', 'height', 'background-color' and 'font-size' */\n" \
        "[width]            { width:            attr(width l) }\n" \
        "[height]           { height:           attr(height l) }\n" \
        "basefont[size]     { font-size:        attr(size) }\n" \
        "font[size]         { font-size:        tcl(::tkhtml::size_to_fontsize) }\n" \
        "[bgcolor]          { background-color: attr(bgcolor) }\n" \
        "\n" \
        "BR[clear]          { clear: attr(clear) }\n" \
        "BR[clear=\"all\"]    { clear: both; }\n" \
        "\n" \
        "/* Standard html <img> tags - replace the node with the image at url $src */\n" \
        "IMG[src]              { -tkhtml-replacement-image: attr(src) }\n" \
        "IMG                   { -tkhtml-replacement-image: \"\" }\n" \
        "\n" \
        "/*\n" \
        " * Properties of table cells (th, td):\n" \
        " *\n" \
        " *     'border-width'\n" \
        " *     'border-style'\n" \
        " *     'padding'\n" \
        " *     'border-spacing'\n" \
        " */\n" \
        "TABLE[border], TABLE[border] TD, TABLE[border] TH {\n" \
        "    border-top-width:     attr(border l table);\n" \
        "    border-right-width:   attr(border l table);\n" \
        "    border-bottom-width:  attr(border l table);\n" \
        "    border-left-width:    attr(border l table);\n" \
        "\n" \
        "    border-top-style:     attr(border x table solid);\n" \
        "    border-right-style:   attr(border x table solid);\n" \
        "    border-bottom-style:  attr(border x table solid);\n" \
        "    border-left-style:    attr(border x table solid);\n" \
        "}\n" \
        "TABLE[border=\"\"], TABLE[border=\"\"] td, TABLE[border=\"\"] th {\n" \
        "    border-top-width:     attr(border x table solid);\n" \
        "    border-right-width:   attr(border x table solid);\n" \
        "    border-bottom-width:  attr(border x table solid);\n" \
        "    border-left-width:    attr(border x table solid);\n" \
        "}\n" \
        "TABLE[cellpadding] td, TABLE[cellpadding] th {\n" \
        "    padding-top:    attr(cellpadding l table);\n" \
        "    padding-right:  attr(cellpadding l table);\n" \
        "    padding-bottom: attr(cellpadding l table);\n" \
        "    padding-left:   attr(cellpadding l table);\n" \
        "}\n" \
        "TABLE[cellspacing], table[cellspacing] {\n" \
        "    border-spacing: attr(cellspacing l);\n" \
        "}\n" \
        "\n" \
        "/* Map the valign attribute to the 'vertical-align' property for table \n" \
        " * cells. The default value is \"middle\", or use the actual value of \n" \
        " * valign if it is defined.\n" \
        " */\n" \
        "TD,TH                        {vertical-align: middle}\n" \
        "TR[valign]>TD, TR[valign]>TH {vertical-align: attr(valign x tr)}\n" \
        "TR>TD[valign], TR>TH[valign] {vertical-align: attr(valign)}\n" \
        "\n" \
        "\n" \
        "/* Support the \"text\" attribute on the <body> tag */\n" \
        "body[text]       {color: attr(text)}\n" \
        "\n" \
        "/* Allow background images to be specified using the \"background\" attribute.\n" \
        " * According to HTML 4.01 this is only allowed for <body> elements, but\n" \
        " * many websites use it arbitrarily.\n" \
        " */\n" \
        "[background] { background-image: attr(background) }\n" \
        "\n" \
        "/* The vspace and hspace attributes map to margins for elements of type\n" \
        " * <IMG>, <OBJECT> and <APPLET> only. Note that this attribute is\n" \
        " * deprecated in HTML 4.01.\n" \
        " */\n" \
        "IMG[vspace], OBJECT[vspace], APPLET[vspace] {\n" \
        "    margin-top: attr(vspace l);\n" \
        "    margin-bottom: attr(vspace l);\n" \
        "}\n" \
        "IMG[hspace], OBJECT[hspace], APPLET[hspace] {\n" \
        "    margin-left: attr(hspace l);\n" \
        "    margin-right: attr(hspace l);\n" \
        "}\n" \
        "\n" \
        "/* marginheight and marginwidth attributes on <BODY> (netscape compatibility) */\n" \
        "BODY[marginheight] {\n" \
        "  margin-top: attr(marginheight l);\n" \
        "  margin-bottom: attr(marginheight l);\n" \
        "}\n" \
        "BODY[marginwidth] {\n" \
        "  margin-left: attr(marginwidth l);\n" \
        "  margin-right: attr(marginwidth l);\n" \
        "}\n" \
        "\n" \
        "SPAN[spancontent]:after {\n" \
        "  content: attr(spancontent);\n" \
        "}\n" \
        "\n" \
        "\n" \



#define HTML_DEFAULT_QUIRKS \
        "/* ======================================================================\n" \
        " * Author : $Author$\n" \
        " * Version: $Revision: 1001 $\n" \
        " * Date   : $Date: 2014-05-21 18:41:00 +0000 (Wed, 21 May 2014) $\n" \
        " * Url    : $URL$\n" \
        " * ======================================================================\n" \
        " */\n" \
        "\n" \
        "/*------------------------------*/\n" \
        "/*      QUIRKS MODE RULES       */\n" \
        "/*------------------------------*/\n" \
        " \n" \
        "/* Tables are historically special. All font attributes except font-family,\n" \
        " * text-align, white-space and line-height) take their initial values.\n" \
        " */\n" \
        "TABLE {\n" \
        "  white-space:  normal;\n" \
        "  line-height:  normal;\n" \
        "  font-size:    medium;\n" \
        "  font-weight:  normal;\n" \
        "  font-style:   normal;\n" \
        "  font-variant: normal;\n" \
        "  text-align: left;\n" \
        "}\n" \
        "\n" \
        "TABLE[align] {\n" \
        "  text-align: left;\n" \
        "}\n" \
        "\n" \
        "/* Vertical margins of <p> elements do not collapse against the top or\n" \
        " * bottom of containing table cells.\n" \
        " */\n" \
        "TH > P:first-child, TD > P:first-child {\n" \
        "  margin-top: 0px;\n" \
        "}\n" \
        "TH > P:last-child, TD > P:last-child {\n" \
        "  margin-bottom: 0px;\n" \
        "}\n" \
        "\n" \
        "FORM {\n" \
        "  margin-bottom: 1em;\n" \
        "}\n" \
        "\n" \



#define HTML_SOURCE_FILES \
    "csssearch.c,v 1.5 2007/07/16 15:35:49 danielk1977 Exp\n" \
    "cssdynamic.c,v 1.12 2007/06/10 07:53:03 danielk1977 Exp\n" \
    "htmltable.c,v 1.122 2007/09/15 07:59:12 danielk1977 Exp\n" \
    "htmlinline.c,v 1.47 2007/06/10 07:53:03 danielk1977 Exp\n" \
    "htmltree.c,v 1.150 2007/09/25 11:21:43 danielk1977 Exp\n" \
    "htmldecode.c,v 1.6 2007/09/20 18:09:31 danielk1977 Exp\n" \
    "htmltagdb.c,v 1.10 2006/07/14 13:37:56 danielk1977 Exp\n" \
    "css.c,v 1.125 2007/10/03 17:46:37 danielk1977 Exp\n" \
    "swproc.c,v 1.6 2006/06/10 12:38:38 danielk1977 Exp\n" \
    "htmlhash.c,v 1.22 2006/10/27 06:40:33 danielk1977 Exp\n" \
    "htmlfloat.c,v 1.21 2006/10/27 15:19:18 danielk1977 Exp\n" \
    "htmlstyle.c,v 1.57 2007/09/25 11:21:42 danielk1977 Exp\n" \
    "restrack.c,v 1.11 2007/01/27 12:53:15 danielk1977 Exp\n" \
    "htmlparse.c,v 1.117 2007/09/25 11:21:42 danielk1977 Exp\n" \
    "htmllayout.c,v 1.259 2007/09/28 14:14:56 danielk1977 Exp\n" \
    "htmlimage.c,v 1.65 2007/09/25 11:21:42 danielk1977 Exp\n" \
    "htmlprop.c,v 1.122 2007/10/05 18:33:57 danielk1977 Exp\n" \
    "htmldraw.c,v 1.201 2007/10/06 10:11:51 danielk1977 Exp\n" \
    "htmltcl.c,v 1.191 2007/10/03 10:06:38 danielk1977 Exp\n" \

