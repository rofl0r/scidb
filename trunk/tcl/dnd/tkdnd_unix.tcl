# ======================================================================
# Author : $Author$
# Version: $Revision: 1158 $
# Date   : $Date: 2017-05-12 13:24:36 +0000 (Fri, 12 May 2017) $
# Url    : $URL$
# ======================================================================

# ======================================================================
# Copyright: (C) 2011-2013 Gregor Cramer
# ======================================================================

# ======================================================================
# Modified by Gregor Cramer to make it useful.
# ======================================================================

#
# tkdnd_unix.tcl --
# 
#    This file implements some utility procedures that are used by the TkDND
#    package.
#
# This software is copyrighted by:
# George Petasis, National Centre for Scientific Research "Demokritos",
# Aghia Paraskevi, Athens, Greece.
# e-mail: petasis@iit.demokritos.gr
#
# The following terms apply to all files associated
# with the software unless explicitly disclaimed in individual files.
#
# The authors hereby grant permission to use, copy, modify, distribute,
# and license this software and its documentation for any purpose, provided
# that existing copyright notices are retained in all copies and that this
# notice is included verbatim in any distributions. No written agreement,
# license, or royalty fee is required for any of the authorized uses.
# Modifications to this software may be copyrighted by their authors
# and need not follow the licensing terms described here, provided that
# the new terms are clearly indicated on the first page of each file where
# they apply.
# 
# IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
# FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
# ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
# DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# 
# THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
# IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
# NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
# MODIFICATIONS.
#

namespace eval tkdnd {
namespace eval xdnd {
  variable _types {}
  variable _typelist {}
  variable _codelist {}
  variable _actionlist {}
  variable _fetch_action_list 0
  variable _pressedkeys {}
  variable _action {}
  variable _common_drag_source_types {}
  variable _common_drop_target_types {}
  variable _drag_source {}
  variable _drop_target {}
  variable _prev_drop_target {}
  variable _dragging 0
  variable _drag_source_cursor {}

  proc debug {msg} {
    puts $msg
  };# debug

  proc registerWrapper {w {alias ""}} { return [_register_wrapper $w $alias] }
};# namespace xdnd

# ----------------------------------------------------------------------------
#  Command xdnd::_HandleXdndEnter
# ----------------------------------------------------------------------------
proc xdnd::_HandleXdndEnter { path drag_source typelist } {
  variable _typelist;                 set _typelist    $typelist
  variable _pressedkeys;              set _pressedkeys 1
  variable _action;                   set _action      {}
  variable _common_drag_source_types; set _common_drag_source_types {}
  variable _common_drop_target_types; set _common_drop_target_types {}
  variable _drag_source;              set _drag_source $drag_source
  variable _drop_target;              set _drop_target {}
  variable _actionlist;               set _actionlist  \
                                           {copy move link ask private}
  variable _fetch_action_list;        set _fetch_action_list 1
  # debug "\n==============================================================="
  # debug "xdnd::_HandleXdndEnter: path=$path, drag_source=$drag_source,\
  #        typelist=$typelist"
  # debug "xdnd::_HandleXdndEnter: ACTION: default"
  return default
};# tkdnd::xdnd::_HandleXdndEnter

# ----------------------------------------------------------------------------
#  Command xdnd::_HandleXdndPosition
# ----------------------------------------------------------------------------
proc xdnd::_HandleXdndPosition { drop_target rootX rootY drag_source } {
  variable _types
  variable _typelist
  variable _actionlist
  variable _fetch_action_list
  variable _pressedkeys
  variable _action
  variable _common_drag_source_types
  variable _common_drop_target_types
  variable _drag_source
  variable _drop_target
  # debug "xdnd::_HandleXdndPosition: drop_target=$drop_target,\
  #             _drop_target=$_drop_target, rootX=$rootX, rootY=$rootY"

  if {![info exists _drag_source] && ![string length $_drag_source]} {
    # debug "xdnd::_HandleXdndPosition: no or empty _drag_source: return refuse_drop"
    return refuse_drop
  }

  if {$drag_source ne "" && $drag_source ne $_drag_source} {
    # debug "XDND position event from unexpected source: $_drag_source != $drag_source"
    return refuse_drop
  }

  if {$_fetch_action_list} {
    set actions [_fetch_action_list $drag_source]
    if {[llength $actions]} { set _actionlist $actions }
    set _fetch_action_list 0
  }

  ## Does the new drop target support any of our new types? 
  set _types [bind $drop_target <<DropTargetTypes>>]
  # debug ">> Accepted types: $drop_target $_types"
  if {[llength $_types]} {
    ## Examine the drop target types, to find at least one match with the drag
    ## source types...
    set supported_types [_supported_types $_typelist]
    foreach type $_types {
      foreach matched [lsearch -glob -all -inline $supported_types $type] {
        ## Drop target supports this type.
        lappend common_drag_source_types $matched
        lappend common_drop_target_types $type
      }
    }
  }
 
  # debug "\t($_drop_target) -> ($drop_target)"
  if {$drop_target != $_drop_target} {
    if {[string length $_drop_target]} {
      ## Call the <<DropLeave>> event.
      # debug "\t<<DropLeave>> on $_drop_target"
      set cmd [bind $_drop_target <<DropLeave>>]
      if {[string length $cmd]} {
        set _codelist $_typelist
        set cmd [string map [list %W $_drop_target %X $rootX %Y $rootY \
          %CST \{$_common_drag_source_types\} \
          %CTT \{$_common_drop_target_types\} \
          %ST  \{$_typelist\}    %TT \{$_types\} \
          %A   \{$_action\}      %a \{$_actionlist\} \
          %b   \{$_pressedkeys\} %m \{$_pressedkeys\} \
          %D   \{\}              %e <<DropLeave>> \
          %L   \{$_typelist\}    %% % \
          %t   \{$_typelist\}    %T  \{[lindex $_common_drag_source_types 0]\} \
          %c   \{$_codelist\}    %C  \{[lindex $_codelist 0]\} \
          ] $cmd]
        uplevel \#0 $cmd
      }
    }
    set _drop_target {}

    if {[info exists common_drag_source_types]} {
      set _action copy
      set _common_drag_source_types $common_drag_source_types
      set _common_drop_target_types $common_drop_target_types
      set _drop_target $drop_target
      ## Drop target supports at least one type. Send a <<DropEnter>>.
      # puts "<<DropEnter>> -> $drop_target"
      set cmd [bind $drop_target <<DropEnter>>]
      if {[string length $cmd]} {
        focus $drop_target
        set _codelist $_typelist
        set cmd [string map [list %W $drop_target %X $rootX %Y $rootY \
          %CST \{$_common_drag_source_types\} \
          %CTT \{$_common_drop_target_types\} \
          %ST  \{$_typelist\}    %TT \{$_types\} \
          %A   $_action          %a  \{$_actionlist\} \
          %b   \{$_pressedkeys\} %m  \{$_pressedkeys\} \
          %D   \{\}              %e  <<DropEnter>> \
          %L   \{$_typelist\}    %%  % \
          %t   \{$_typelist\}    %T  \{[lindex $_common_drag_source_types 0]\} \
          %c   \{$_codelist\}    %C  \{[lindex $_codelist 0]\} \
          ] $cmd]
        set _action [uplevel \#0 $cmd]
      }
    }
    set _drop_target $drop_target
  }
  
  set _action refuse_drop
  set _drop_target {}
  if {[info exists common_drag_source_types]} {
    set _action copy
    set _common_drag_source_types $common_drag_source_types
    set _common_drop_target_types $common_drop_target_types
    set _drop_target $drop_target
    ## Drop target supports at least one type. Send a <<DropPosition>>.
    set cmd [bind $drop_target <<DropPosition>>]
    if {[string length $cmd]} {
      set _codelist $_typelist
      set cmd [string map [list %W $drop_target %X $rootX %Y $rootY \
        %CST \{$_common_drag_source_types\} \
        %CTT \{$_common_drop_target_types\} \
        %ST  \{$_typelist\}    %TT \{$_types\} \
        %A   $_action          %a  \{$_actionlist\} \
        %b   \{$_pressedkeys\} %m  \{$_pressedkeys\} \
        %D   \{\}              %e  <<DropPosition>> \
        %L   \{$_typelist\}    %%  % \
        %t   \{$_typelist\}    %T  \{[lindex $_common_drag_source_types 0]\} \
        %c   \{$_codelist\}    %C  \{[lindex $_codelist 0]\} \
        ] $cmd]
      set _action [uplevel \#0 $cmd]
    }
  }
  # Return values: copy, move, link, ask, private, refuse_drop, default
  # debug "xdnd::_HandleXdndPosition: ACTION: $_action"
  return $_action
};# xdnd::_HandleXdndPosition

# ----------------------------------------------------------------------------
#  Command xdnd::_HandleXdndLeave
# ----------------------------------------------------------------------------
proc xdnd::_HandleXdndLeave {  } {
  variable _types
  variable _typelist
  variable _actionlist
  variable _fetch_action_list
  variable _pressedkeys
  variable _action
  variable _common_drag_source_types
  variable _common_drop_target_types
  variable _drag_source
  variable _drop_target
  if {![info exists _drop_target]} {set _drop_target {}}
  # debug "xdnd::_HandleXdndLeave: _drop_target=$_drop_target"
  if {[info exists _drop_target] && [string length $_drop_target]} {
    set cmd [bind $_drop_target <<DropLeave>>]
    if {[string length $cmd]} {
      set _codelist $_typelist
      set cmd [string map [list %W $_drop_target %X 0 %Y 0 \
        %CST \{$_common_drag_source_types\} \
        %CTT \{$_common_drop_target_types\} \
        %ST  \{$_typelist\}    %TT \{$_types\} \
        %A   \{$_action\}      %a  \{$_actionlist\} \
        %b   \{$_pressedkeys\} %m  \{$_pressedkeys\} \
        %D   \{\}              %e  <<DropLeave>> \
        %L   \{$_typelist\}    %%  % \
        %t   \{$_typelist\}    %T  \{[lindex $_common_drag_source_types 0]\} \
        %c   \{$_codelist\}    %C  \{[lindex $_codelist 0]\} \
        ] $cmd]
      set _action [uplevel \#0 $cmd]
    }
  }
  lassign {{} {} {} {} {} {} {} {} {} 1} \
    _types _typelist _actionlist _pressedkeys _action \
    _common_drag_source_types _common_drop_target_types \
    _drag_source _drop_target _fetch_action_list
};# xdnd::_HandleXdndLeave

# ----------------------------------------------------------------------------
#  Command xdnd::_HandleXdndDrop
# ----------------------------------------------------------------------------
proc xdnd::_HandleXdndDrop { time } {
  variable _types
  variable _typelist
  variable _actionlist
  variable _fetch_action_list
  variable _pressedkeys
  variable _action
  variable _common_drag_source_types
  variable _common_drop_target_types
  variable _drag_source
  variable _drop_target

  # puts "xdnd::_HandleXdndDrop: $time"

  if {![info exists _drag_source] || ![string length $_drag_source]} {
    return refuse_drop
  }
  if {![info exists _drop_target] || ![string length $_drop_target]} {
    return refuse_drop
  }
  if {![llength $_common_drag_source_types]} {return refuse_drop}

  if {$_fetch_action_list} {
    set actions [_fetch_action_list $drag_source]
    if {[llength $actions]} { set _actionlist $actions }
    set _fetch_action_list 0
  }

  lassign [winfo pointerxy .] rootX rootY
  ## Get the dropped data.
  set data [_GetDroppedData $time]
  ## Try to select the most specific <<Drop>> event.
  foreach type [concat $_common_drag_source_types $_common_drop_target_types] {
    set type [_platform_independent_type $type]
    set cmd [bind $_drop_target <<Drop:$type>>]
    if {[string length $cmd]} {
      set _codelist $_typelist
      set cmd [string map [list %W $_drop_target %X $rootX %Y $rootY \
        %CST \{$_common_drag_source_types\} \
        %CTT \{$_common_drop_target_types\} \
        %ST  \{$_typelist\}    %TT \{$_types\} \
        %A   $_action          %a \{$_actionlist\} \
        %b   \{$_pressedkeys\} %m \{$_pressedkeys\} \
        %D   [list $data]      %e <<Drop:$type>> \
        %L   \{$_typelist\}    %% % \
        %t   \{$_typelist\}    %T  \{[lindex $_common_drag_source_types 0]\} \
        %c   \{$_codelist\}    %C  \{[lindex $_codelist 0]\} \
        ] $cmd]
      return [uplevel \#0 $cmd]
    }
  }
  set cmd [bind $_drop_target <<Drop>>]
  if {[string length $cmd]} {
    set _codelist $_typelist
    set cmd [string map [list %W $_drop_target %X $rootX %Y $rootY \
      %CST \{$_common_drag_source_types\} \
      %CTT \{$_common_drop_target_types\} \
      %ST  \{$_typelist\}    %TT \{$_types\} \
      %A   $_action          %a \{$_actionlist\} \
      %b   \{$_pressedkeys\} %m \{$_pressedkeys\} \
      %D   [list $data]      %e <<Drop>> \
      %L   \{$_typelist\}    %% % \
      %t   \{$_typelist\}    %T  \{[lindex $_common_drag_source_types 0]\} \
      %c   \{$_codelist\}    %C  \{[lindex $_codelist 0]\} \
      ] $cmd]
    set _action [uplevel \#0 $cmd]
  }
  # Return values: XdndActionCopy, XdndActionMove,    XdndActionLink,
  #                XdndActionAsk,  XdndActionPrivate, refuse_drop
  return $_action
};# xdnd::_HandleXdndDrop

# ----------------------------------------------------------------------------
#  Command xdnd::_HandleDragPosition
# ----------------------------------------------------------------------------
proc xdnd::_HandleDragPosition { window rootX rootY } {
  variable _types
  variable _typelist
  variable _actionlist
  variable _pressedkeys
  variable _action
  variable _common_drag_source_types
  variable _common_drop_target_types
  variable _dodragdrop_drag_source

  ## Call the <<DragPosition>> event.
  # debug "\t<<DragPosition>> on $_dodragdrop_drag_source"
  set cmd [bind $_dodragdrop_drag_source <<DragPosition>>]
  if {[string length $cmd]} {
    set _codelist $_typelist
    set cmd [string map [list \
      %W   $_dodragdrop_drag_source \
      %V   \{$window\} \
      %CST \{$_common_drag_source_types\} \
      %CTT \{$_common_drop_target_types\} \
      %X   $rootX            %Y $rootY \
      %ST  \{$_typelist\}    %TT \{$_types\} \
      %A   \{$_action\}      %a \{$_actionlist\} \
      %b   \{$_pressedkeys\} %m \{$_pressedkeys\} \
      %D   \{\}              %e <<DragPosition>> \
      %L   \{$_typelist\}    %% % \
      %t   \{$_typelist\}    %T  \{[lindex $_common_drag_source_types 0]\} \
      %c   \{$_codelist\}    %C  \{[lindex $_codelist 0]\} \
      ] $cmd]
    return [uplevel \#0 $cmd]
  }
  return ""
}

# ----------------------------------------------------------------------------
#  Command xdnd::_GetDroppedData
# ----------------------------------------------------------------------------
proc xdnd::_GetDroppedData { time } {
  variable _drop_target
  variable _prev_drop_target
  variable _common_drag_source_types
  if {![llength $_common_drag_source_types]} {
    error "no common data types between the drag source and drop target widgets"
  }
  set _prev_drop_target $_drop_target
  foreach type $_common_drag_source_types {
    # puts "TYPE: $type ($_drop_target)"
    # _get_selection $_drop_target $time $type
    if {![catch {
      selection get -displayof $_drop_target -selection XdndSelection \
                    -type $type -time $time} result options]} {
      return [_normalise_data $type $result]
    }
  }
  # target should receive a leave event in any case
  _HandleXdndLeave
  return -options $options $result
};# xdnd::_GetDroppedData

# ----------------------------------------------------------------------------
#  Command xdnd::_GetDragSource
# ----------------------------------------------------------------------------
proc xdnd::_GetDragSource {  } {
  variable _drag_source
  return $_drag_source
};# xdnd::_GetDragSource

# ----------------------------------------------------------------------------
#  Command xdnd::_GetDropTarget
# ----------------------------------------------------------------------------
proc xdnd::_GetDropTarget {  } {
  variable _drop_target
  if {[string length $_drop_target]} {
    return [winfo id $_drop_target]
  }
  return 0
};# xdnd::_GetDropTarget

# ----------------------------------------------------------------------------
#  Command xdnd::_supported_types
# ----------------------------------------------------------------------------
proc xdnd::_supported_types { types } {
  set new_types {}
  foreach type $types {
    if {[_supported_type $type]} {lappend new_types $type}
  }
  return $new_types
}; # xdnd::_supported_types

# ----------------------------------------------------------------------------
#  Command xdnd::_platform_specific_types
# ----------------------------------------------------------------------------
proc xdnd::_platform_specific_types { types } {
  set new_types {}
  foreach type $types {
    set new_types [concat $new_types [_platform_specific_type $type]]
  }
  return $new_types
}; # xdnd::_platform_specific_types

# ----------------------------------------------------------------------------
#  Command xdnd::_normalise_data
# ----------------------------------------------------------------------------
proc xdnd::_normalise_data { type data } {
  switch $type {
    text/uri-list - text/x-moz-url {
      set list {}
      foreach file [split $data \n] {
        set file [string trim $file "\n\r"]
        if {[string length $file] > 0} {
          lappend list $file
        }
      }
      return $list
    }
    application/x-color {return $data ;# ???}
  }
  return $data
}; # xdnd::_normalise_data

# ----------------------------------------------------------------------------
#  Command xdnd::_platform_specific_type
# ----------------------------------------------------------------------------
proc xdnd::_platform_specific_type { type } {
  switch $type {
    DND_Text   {return {text/plain
                        text/html
                        text/plain;charset=UTF-8
                        text/plain;charset=ISO-8859-1
                        text/html;charset=UTF-8
                        text/html;charset=ISO-8859-1
                        UTF8_STRING
                        STRING}}
    DND_Files  {return {text/uri-list text/x-moz-url application/x-qiconlist}}
    DND_Color  {return {application/x-color}}
    default    {return [list $type]}
  }
}; # xdnd::_platform_specific_type

# ----------------------------------------------------------------------------
#  Command xdnd::_platform_independent_type
# ----------------------------------------------------------------------------
proc xdnd::_platform_independent_type { type } {
  switch $type {
    UTF8_STRING                   -
    STRING                        -
    text/plain;charset=UTF-8      -
    text/plain;charset=ISO-8859-1 -
    text/html;charset=UTF-8       -
    text/html;charset=ISO-8859-1  -
    text/html                     -
    text/plain                    {return DND_Text}
    text/uri-list                 -
    application/x-qiconlist       -
    text/x-moz-url                {return DND_Files}
    application/x-color           {return DND_Color}
    default                       {return $type}
  }
}; # xdnd::_platform_independent_type

# ----------------------------------------------------------------------------
#  Command xdnd::_supported_type
# ----------------------------------------------------------------------------
proc xdnd::_supported_type { type } {
  switch $type {
    UTF8_STRING                   -
    STRING                        -
    text/plain                    -
    text/html                     -
    text/plain;charset=UTF-8      -
    text/plain;charset=ISO-8859-1 -
    text/html;charset=UTF-8       -
    text/html;charset=ISO-8859-1  -
    text/uri-list                 -
    text/x-moz-url                -
    application/x-qiconlist       -
    application/x-color           {return 1}
  }
  return 0
}; # xdnd::_supported_type

#############################################################################
##
##  XDND drag implementation
##
#############################################################################

# ----------------------------------------------------------------------------
#  Command xdnd::_selection_ownership_lost
# ----------------------------------------------------------------------------
proc xdnd::_selection_ownership_lost {} {
  variable _dragging
  set _dragging 0
};# _selection_ownership_lost

# ----------------------------------------------------------------------------
#  Command xdnd::_dodragdrop
# ----------------------------------------------------------------------------
proc xdnd::_dodragdrop { source actions types data button } {
  variable _dragging

  # puts "xdnd::_dodragdrop: source: $source, actions: $actions, types: $types,\
  #       data: \"$data\", button: $button"
  if {$_dragging} {
    ## We are in the middle of another drag operation...
    error "another drag operation in progress"
  }

  variable _dodragdrop_drag_source                $source
  variable _dodragdrop_drop_target                0
  variable _dodragdrop_drop_target_proxy          0
  variable _dodragdrop_actions                    $actions
  variable _dodragdrop_action_descriptions        $actions
  variable _dodragdrop_actions_len                [llength $actions]
  variable _dodragdrop_types                      $types
  variable _dodragdrop_types_len                  [llength $types]
  variable _dodragdrop_data                       $data
  variable _dodragdrop_transfer_data              {}
  variable _dodragdrop_button                     $button
  variable _dodragdrop_time                       0
  variable _dodragdrop_default_action             refuse_drop
  variable _dodragdrop_waiting_status             0
  variable _dodragdrop_drop_target_accepts_drop   0
  variable _dodragdrop_drop_target_accepts_action refuse_drop
  variable _dodragdrop_current_cursor             $_dodragdrop_default_action
  variable _dodragdrop_drop_occured               0
  variable _dodragdrop_selection_requestor        0
  variable _dodragdrop_action                     {}
  variable _drag_source_cursor                    {}

  ##
  ## If we have more than 3 types, the property XdndTypeList must be set on
  ## the drag source widget...
  ##
  if {$_dodragdrop_types_len > 3} {
    _announce_type_list $_dodragdrop_drag_source $_dodragdrop_types
  }

  ##
  ## Announce the actions & their descriptions on the XdndActionList &
  ## XdndActionDescription properties...
  ##
  _announce_action_list $_dodragdrop_drag_source $_dodragdrop_actions \
                        $_dodragdrop_action_descriptions

  ##
  ## Arrange selection handlers for our drag source, and all the supported types
  ##
# registerSelectionHandler $source $types

  ##
  ## Step 1: When a drag begins, the source takes ownership of XdndSelection.
  ##
  selection own -command ::tkdnd::xdnd::_selection_ownership_lost \
                -selection XdndSelection $source
  set _dragging 1
  catch { set _drag_source_cursor [$_dodragdrop_drag_source cget -cursor] }

  ## Grab the mouse pointer...
  # NOTE: _grab_pointer cannot work because it is not interacting with ttk::grab.
  # _grab_pointer $source $_dodragdrop_default_action
  ttk::globalGrab $source

  ## Register our generic event handler...
  #  The generic event callback will report events by modifying variable
  #  ::xdnd::_dodragdrop_event: a dict with event information will be set as
  #  the value of the variable...
  _register_generic_event_handler

  ## Set a timeout for debugging purposes...
  #  after 60000 {set ::tkdnd::xdnd::_dragging 0}

  tkwait variable ::tkdnd::xdnd::_dragging
  _SendXdndLeave

  set _dragging 0
  # _ungrab_pointer $source
  ttk::releaseGrab $source
  _unregister_generic_event_handler
  catch {selection clear -selection XdndSelection}
# unregisterSelectionHandler $source $types
  return $_dodragdrop_action
};# xdnd::_dodragdrop

# ----------------------------------------------------------------------------
#  Command xdnd::_process_drag_events
# ----------------------------------------------------------------------------
proc xdnd::_process_drag_events {event} {
  # The return value from proc is normally 0. A non-zero return value indicates
  # that the event is not to be handled further; that is, proc has done all
  # processing that is to be allowed for the event
  variable _dragging
  if {!$_dragging} {return 0}
  # puts $event

  variable _dodragdrop_time
  set time [dict get $event time]
  set type [dict get $event type]
  if {$time < $_dodragdrop_time && ![string equal $type SelectionRequest]} {
    return 0
  }
  set _dodragdrop_time $time

  variable _dodragdrop_drag_source
  variable _dodragdrop_drop_target
  variable _dodragdrop_drop_target_proxy
  variable _dodragdrop_default_action
  switch $type {
    MotionNotify {
      set rootx  [dict get $event x_root]
      set rooty  [dict get $event y_root]
      set window [_find_drop_target_window $_dodragdrop_drag_source $rootx $rooty]
      set action [_HandleDragPosition $window $rootx $rooty]
      if {[string length $window] && $action ne "refuse_drop"} {
        ## Examine the modifiers to suggest an action...
        set _dodragdrop_default_action [_default_action $event]
        ## Is it a Tk widget?
        # set path [winfo containing $rootx $rooty] 
        # puts "Window under mouse: $window ($path)"
        if {$_dodragdrop_drop_target != $window} {
          ## Send XdndLeave to $_dodragdrop_drop_target
          _SendXdndLeave
          ## Is there a proxy? If not, _find_drop_target_proxy returns the
          ## target window, so we always get a valid "proxy".
          set proxy [_find_drop_target_proxy $_dodragdrop_drag_source $window]
          ## Send XdndEnter to $window
          _SendXdndEnter $window $proxy
          ## Send XdndPosition to $_dodragdrop_drop_target
          _SendXdndPosition $rootx $rooty $_dodragdrop_default_action
        } else {
          ## Send XdndPosition to $_dodragdrop_drop_target
          _SendXdndPosition $rootx $rooty $_dodragdrop_default_action
        }
      } else {
        ## No window under the mouse. Send XdndLeave to $_dodragdrop_drop_target
        _SendXdndLeave
      }
    }
    ButtonPress {
    }
    ButtonRelease {
      variable _dodragdrop_button
      set button [dict get $event button]
      if {$button == $_dodragdrop_button} {
        ## The button that initiated the drag was released. Trigger drop...
        _SendXdndDrop
      }
      return 1
    }
    KeyPress {
    }
    KeyRelease {
      set keysym [dict get $event keysym]
      switch $keysym {
        Escape {
          ## The user has pressed escape. Abort...
          if {$_dragging} {set _dragging 0}
        }
      }
    }
    SelectionRequest {
      variable _dodragdrop_selection_requestor
      variable _dodragdrop_selection_property
      variable _dodragdrop_selection_selection
      variable _dodragdrop_selection_target
      variable _dodragdrop_selection_time
      set _dodragdrop_selection_requestor [dict get $event requestor]
      set _dodragdrop_selection_property  [dict get $event property]
      set _dodragdrop_selection_selection [dict get $event selection]
      set _dodragdrop_selection_target    [dict get $event target]
      set _dodragdrop_selection_time      $time
      selection send \
        $_dodragdrop_drag_source \
        $_dodragdrop_selection_requestor \
        $_dodragdrop_selection_property \
        $_dodragdrop_selection_target \
        $_dodragdrop_selection_time \
        [_SendData $_dodragdrop_selection_target 0 -1] \
        ;
      return 1
    }
    default {
      return 0
    }
  }
  return 0
};# _process_drag_events

# ----------------------------------------------------------------------------
#  Command xdnd::_SendXdndEnter
# ----------------------------------------------------------------------------
proc xdnd::_SendXdndEnter {window proxy} {
  variable _dodragdrop_drag_source
  variable _dodragdrop_drop_target
  variable _dodragdrop_drop_target_proxy
  variable _dodragdrop_types
  variable _dodragdrop_waiting_status
  variable _dodragdrop_drop_occured
  if {$_dodragdrop_drop_target > 0} _SendXdndLeave
  if {$_dodragdrop_drop_occured} return
  set _dodragdrop_drop_target       $window
  set _dodragdrop_drop_target_proxy $proxy
  set _dodragdrop_waiting_status    0
  if {$_dodragdrop_drop_target < 1} return
  # puts "XdndEnter: $_dodragdrop_drop_target $_dodragdrop_drop_target_proxy"
  _send_XdndEnter $_dodragdrop_drag_source $_dodragdrop_drop_target \
                  $_dodragdrop_drop_target_proxy $_dodragdrop_types
};# xdnd::_SendXdndEnter

# ----------------------------------------------------------------------------
#  Command xdnd::_SendXdndPosition
# ----------------------------------------------------------------------------
proc xdnd::_SendXdndPosition {rootx rooty action} {
  variable _dodragdrop_drag_source
  variable _dodragdrop_drop_target
  if {$_dodragdrop_drop_target < 1} return
  variable _dodragdrop_drop_occured
  if {$_dodragdrop_drop_occured} return
  variable _dodragdrop_drop_target_proxy
  variable _dodragdrop_waiting_status
  ## Arrange a new XdndPosition, to be send periodically...
  variable _dodragdrop_xdnd_position_heartbeat
  catch {after cancel $_dodragdrop_xdnd_position_heartbeat}
  set _dodragdrop_xdnd_position_heartbeat [after 200 \
    [list ::tkdnd::xdnd::_SendXdndPosition $rootx $rooty $action]]
  if {$_dodragdrop_waiting_status} {return}
  # puts "XdndPosition: $_dodragdrop_drop_target $rootx $rooty $action"
  _send_XdndPosition $_dodragdrop_drag_source $_dodragdrop_drop_target \
                     $_dodragdrop_drop_target_proxy $rootx $rooty $action
  set _dodragdrop_waiting_status 1
};# xdnd::_SendXdndPosition

# ----------------------------------------------------------------------------
#  Command xdnd::_HandleXdndStatus
# ----------------------------------------------------------------------------
proc xdnd::_HandleXdndStatus {event} {
  variable _dodragdrop_drop_target
  variable _dodragdrop_waiting_status
  variable _dodragdrop_action

  variable _dodragdrop_drop_target_accepts_drop
  variable _dodragdrop_drop_target_accepts_action
  set _dodragdrop_waiting_status 0
  foreach key {target accept want_position action x y w h} {
    set $key [dict get $event $key]
  }
  set _dodragdrop_drop_target_accepts_drop   $accept
  set _dodragdrop_drop_target_accepts_action $action
  if {$_dodragdrop_drop_target < 1} return
  variable _dodragdrop_drop_occured
  if {$_dodragdrop_drop_occured} return
#puts "_HandleXdndStatus: [set [namespace current]::_dodragdrop_current_cursor]"
  _update_drag_cursor
  set _dodragdrop_action [dict get $event action]
  # puts "XdndStatus: $event"
};# xdnd::_HandleXdndStatus

# ----------------------------------------------------------------------------
#  Command xdnd::_HandleXdndFinished
# ----------------------------------------------------------------------------
proc xdnd::_HandleXdndFinished {event} {
  variable _dodragdrop_drop_target
  variable _dodragdrop_drag_source
  variable _drag_source_cursor
  set _dodragdrop_drop_target 0
  variable _dragging
  if {$_dragging} {
    set _dragging 0
    catch { $_dodragdrop_drag_source configure -cursor $_drag_source_cursor }
  }
  # puts "XdndFinished: $event"
};# xdnd::_HandleXdndFinished

# ----------------------------------------------------------------------------
#  Command xdnd::_SendXdndLeave
# ----------------------------------------------------------------------------
proc xdnd::_SendXdndLeave {} {
  variable _dodragdrop_drag_source
  variable _dodragdrop_drop_target
  if {$_dodragdrop_drop_target < 1} return
  variable _dodragdrop_drop_target_proxy
  # puts "XdndLeave: $_dodragdrop_drop_target"
  _send_XdndLeave $_dodragdrop_drag_source $_dodragdrop_drop_target \
                  $_dodragdrop_drop_target_proxy
  set _dodragdrop_drop_target 0
  variable _dodragdrop_drop_target_accepts_drop
  variable _dodragdrop_drop_target_accepts_action
  set _dodragdrop_drop_target_accepts_drop   0
  set _dodragdrop_drop_target_accepts_action refuse_drop
  variable _dodragdrop_drop_occured
  if {$_dodragdrop_drop_occured} return
#puts "_SendXdndLeave: [set [namespace current]::_dodragdrop_current_cursor]"
  _update_drag_cursor
};# xdnd::_SendXdndLeave

# ----------------------------------------------------------------------------
#  Command xdnd::_SendXdndDrop
# ----------------------------------------------------------------------------
proc xdnd::_SendXdndDrop {} {
  variable _dodragdrop_drag_source
  variable _dodragdrop_drop_target
  if {$_dodragdrop_drop_target < 1} {
    ## The mouse has been released over a widget that does not accept drops.
    _HandleXdndFinished {}
    return
  }
  variable _dodragdrop_drop_occured
  if {$_dodragdrop_drop_occured} {return}
  variable _dodragdrop_drop_target_proxy
  variable _dodragdrop_drop_target_accepts_drop
  variable _dodragdrop_drop_target_accepts_action

  set _dodragdrop_drop_occured 1
#puts "_SendXdndDrop(clock): [set [namespace current]::_dodragdrop_current_cursor]"
  _update_drag_cursor watch

  if {!$_dodragdrop_drop_target_accepts_drop} {
    _SendXdndLeave
    _HandleXdndFinished {}
    return
  }

  # Call the <<DradDropCmd>> binding.
  set source $_dodragdrop_drag_source
  set cmd [bind $source <<DragDropCmd>>]
  if {[string length $cmd]} {
    set action $_dodragdrop_drop_target_accepts_action
    set cmd [string map [list %W $source %e <<DragDropCmd>> %A \{$action\}] $cmd]
    set action [uplevel \#0 $cmd]
    if {$action == ""} {
      _SendXdndLeave
      _HandleXdndFinished {}
      return
    }
  }

  # puts "XdndDrop: $_dodragdrop_drop_target"
  variable _dodragdrop_drop_timestamp
  set _dodragdrop_drop_timestamp [_send_XdndDrop \
                 $_dodragdrop_drag_source $_dodragdrop_drop_target \
                 $_dodragdrop_drop_target_proxy]
  set _dodragdrop_drop_target 0
  # puts "XdndDrop: $_dodragdrop_drop_target"
  ## Arrange a timeout for receiving XdndFinished...
  after 500 [list ::tkdnd::xdnd::_HandleXdndFinished {}]
};# xdnd::_SendXdndDrop

# ----------------------------------------------------------------------------
#  Command xdnd::_update_drag_cursor
# ----------------------------------------------------------------------------
proc xdnd::_update_drag_cursor { {cursor {}}} {
  # puts "_update_drag_cursor $cursor"
  variable _dodragdrop_current_cursor
  variable _dodragdrop_drag_source
  variable _dodragdrop_drop_target_accepts_drop
  variable _dodragdrop_drop_target_accepts_action

  if {![string length $cursor]} {
    set cursor refuse_drop
    if {$_dodragdrop_drop_target_accepts_drop} {
      set cursor $_dodragdrop_drop_target_accepts_action
    }
  }
  if {![string equal $cursor $_dodragdrop_current_cursor]} {
    variable [namespace parent]::_drag_cursors
    if {[info exists _drag_cursors($_dodragdrop_drag_source:$cursor)]} {
      xcursor::setCursor $_dodragdrop_drag_source $_drag_cursors($_dodragdrop_drag_source:$cursor)
    } else {
      _set_pointer_cursor $_dodragdrop_drag_source $cursor
    }
    set _dodragdrop_current_cursor $cursor
  }
};# xdnd::_update_drag_cursor

# ----------------------------------------------------------------------------
#  Command xdnd::_default_action
# ----------------------------------------------------------------------------
proc xdnd::_default_action {event} {
  variable _dodragdrop_actions
  variable _dodragdrop_actions_len
  if {$_dodragdrop_actions_len == 1} {return [lindex $_dodragdrop_actions 0]}
  
  set alt     [dict get $event Alt]
  set shift   [dict get $event Shift]
  set control [dict get $event Control]

  if {$shift && $control && [lsearch $_dodragdrop_actions link] != -1} {
    return link
  } elseif {$control && [lsearch $_dodragdrop_actions copy] != -1} {
    return copy
  } elseif {$shift && [lsearch $_dodragdrop_actions move] != -1} {
    return move
  } elseif {$alt && [lsearch $_dodragdrop_actions link] != -1} {
    return link
  }
  return default
};# xdnd::_default_action

# ----------------------------------------------------------------------------
#  Command xdnd::getFormatForType
# ----------------------------------------------------------------------------
proc xdnd::getFormatForType {type} {
  switch $type {
    text/plain;charset=UTF-8      -
    text/html;charset=UTF-8       -
    text/uri-list                 -
    UTF8_STRING                   -
    text/plain;charset=ISO-8859-1 -
    text/html;charset=ISO-8859-1  -
    text/html                     -
    application/x-qiconlist       -
    text/plain                    -
    text/x-moz-url                {set format $type}
    STRING                        -
    TEXT                          -
    COMPOUND_TEXT                 {set format STRING}
    application/x-color           {set format $type}
    default                       {set format $type}
  }
  return $format
};# xdnd::getFormatForType

# ----------------------------------------------------------------------------
#  Command xdnd::registerSelectionHandler
# ----------------------------------------------------------------------------
proc xdnd::registerSelectionHandler {source types} {
  variable _dodragdrop_time
  foreach type $types {
    selection handle -selection XdndSelection \
                     -type $type \
                     -format [getFormatForType $type] \
                     -time $_dodragdrop_time \
                     $source [list ::tkdnd::xdnd::_SendData $type]
  }
};# xdnd::registerSelectionHandler

# ----------------------------------------------------------------------------
#  Command xdnd::unregisterSelectionHandler
# ----------------------------------------------------------------------------
proc xdnd::unregisterSelectionHandler {source types} {
  foreach type $types {
    catch {
      selection handle -selection XdndSelection \
                       -type $type \
                       -format [getFormatForType $type] \
                       $source {}
    }
  }
};# xdnd::unregisterSelectionHandler

# ----------------------------------------------------------------------------
#  Command xdnd::_convert_to_unsigned
# ----------------------------------------------------------------------------
proc xdnd::_convert_to_unsigned {data format} {
  switch $format {
    8  { set mask 0xff }
    16 { set mask 0xffff }
    32 { set mask 0xffffff }
    default {error "unsupported format $format"}
  }
  ## Convert signed integer into unsigned...
  set d {}
  foreach num $data { lappend d [expr { $num & $mask }] }
  return $d
};# xdnd::_convert_to_unsigned

# ----------------------------------------------------------------------------
#  Command xdnd::_SendData
# ----------------------------------------------------------------------------
proc xdnd::_SendData {type offset bytes args} {
  variable _dodragdrop_drag_source
  variable _dodragdrop_data
  variable _dodragdrop_transfer_data
  set format 8
  if {$offset == 0} {
    ## Prepare the data to be transfered...
    switch -glob $type {
      text/plain*             -
      text/html*              -
      text/x-moz-url          -
      application/x-qiconlist -
      UTF8_STRING             -
      STRING                  -
      TEXT                    -
      COMPOUND_TEXT           {
        return $_dodragdrop_data
      }
      text/uri-list {
        set files {}
        foreach file $_dodragdrop_data {
          if {[string match {[a-z]*://*} $file]} {
            lappend files $file
          } else {
            lappend files "file://$file"
          }
        }
        return "[join $files \r\n]\r\n"
      }
      application/x-color {
        set format 16
        ## Try to understand the provided data: we accept a standard Tk colour,
        ## or a list of 3 values (red green blue) or a list of 4 values
        ## (red green blue opacity).
        switch [llength $_dodragdrop_data] {
          1 { set color [winfo rgb $_dodragdrop_drag_source $_dodragdrop_data]; lappend color 65535 }
          3 { set color $_dodragdrop_data; lappend color 65535 }
          4 { set color $_dodragdrop_data }
          default {error "unknown color data: \"$_dodragdrop_data\""}
        }
        ## Convert the 4 elements into 16 bit values...
        set _dodragdrop_transfer_data {}
        foreach c $color { lappend _dodragdrop_transfer_data [format 0x%04X $c] }
      }
      default {
        set format 32
        binary scan $_dodragdrop_data c* _dodragdrop_transfer_data
      }
    }
  }

  ##
  ## Data has been split into bytes. Count the bytes requested, and return them
  ##
  if {$bytes >= 0} {
    set data [lrange $_dodragdrop_transfer_data $offset [expr {$offset + $bytes - 1}]]
  } else {
    set data $_dodragdrop_transfer_data
  }
  switch $format {
    8       { set data [binary format c* $data] }
    16 - 32 { ;# nothing to do }
    default { error "unsupported format $format" }
  }
  # puts "SendData: $type $offset $bytes $args ($_dodragdrop_data)"
  # puts "          $data"
  return $data
};# xdnd::_SendData

};# namespace tkdnd

# vi:set ts=2 sw=2 et:
