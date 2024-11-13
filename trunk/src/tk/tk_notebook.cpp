// ======================================================================
// Author : $Author$
// Version: $Revision: 267 $
// Date   : $Date: 2012-03-06 08:52:13 +0000 (Tue, 06 Mar 2012) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C)2011-2012 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "tk_init.h"

#include "tcl_base.h"

#define namespace namespace_	// bug in tcl8.6/tkInt.h
#include "tkInt.h"
#undef namespace

#include "m_types.h"

#include <ctype.h>
#include <assert.h>


#ifndef MAX
# define MIN(a,b)	((a) < (b) ? (a) : (b))
# define MAX(a,b)	((a) < (b) ? (b) : (a))
#endif


// Flag values for "sticky"ness. The 16 combinations subsume the packer's
// notion of anchor and fill.
//
// STICK_NORTH  	This window sticks to the top of its cavity.
// STICK_EAST		This window sticks to the right edge of its cavity.
// STICK_SOUTH		This window sticks to the bottom of its cavity.
// STICK_WEST		This window sticks to the left edge of its cavity.
#define STICK_NORTH		1
#define STICK_EAST		2
#define STICK_SOUTH		4
#define STICK_WEST		8


// forward declaration
struct Notebook;

extern "C"
{
	int Tk_NotebookObjCmd(ClientData, Tcl_Interp*, int, Tcl_Obj* const []);
}


typedef struct
{
	Tk_OptionTable nbOptions;		// Token for notebook option table.
	Tk_OptionTable paneOptions;	// Token for pane option table.
}
OptionTables;


typedef struct
{
	int left;
	int top;
	int right;
	int bottom;
}
Padding;


// One structure of the following type is kept for each window
// managed by a notebook widget.
typedef struct Slave
{
    Tk_Window	tkwin;		// Window being managed.
    Tcl_Obj*	widthObj;	// Tcl_Obj rep's of slave width, to allow for null values.
	 Tcl_Obj*	heightObj;	// Tcl_Obj rep's of slave height, to allow for null values.
	 Tcl_Obj*	underlineObj;
	 Tcl_Obj*	paddingObj;
	 Tcl_Obj*	textObj;
	 Padding		padding;
    int			width;		// Slave width.
    int			height;		// Slave height.
	 int			state;		// Either TK_STATE_NORMAL, TK_STATE_DISABLED, or TK_STATE_HIDDEN.
    int			sticky;		// Sticky string.
    int			hide;			// Controls visibility of pane.
    Tk_Window	after;		// Placeholder for parsing options.
    Tk_Window	before;		// Placeholder for parsing options.

    struct Notebook* master; // Notebook managing the window.
}
Slave;


// A data structure of the following type is kept for each notebook widget
// managed by this file:
typedef struct Notebook
{
    Tk_Window			tkwin;			// Window that embodies the notebook.
    Tcl_Interp*		interp;			// Interpreter associated with widget.
    Tcl_Command		widgetCmd;		// Token for square's widget command.
    Tk_OptionTable	optionTable;	// Token representing the configuration * specifications.
	 Tk_OptionTable	paneOptions;
    Tk_3DBorder		background;		// Background color.
    int					borderWidth;	// Value of -borderwidth option.
    int					relief;			// 3D border effect (TK_RELIEF_RAISED, etc)
    Tcl_Obj*			widthObj;		// Tcl_Obj rep for width.
    Tcl_Obj*			heightObj;		// Tcl_Obj rep for height.
	 Tcl_Obj*			paddingObj;
	 Padding				padding;
    int					width;			// Width of the widget.
	 int					height;			// Height of the widget.
    Tk_Cursor			cursor;			// Current cursor for window, or None.
    GC					gc;				// Graphics context for copying from off-screen pixmap onto screen.
    Slave**				slaves;			// Pointer to array of Slaves.
    int					numSlaves;		// Number of slaves.
	 int					overlay;			// Overlay flag.
    int					flags;			// Flags for widget; see below.
}
Notebook;


// Flags used for notebooks:
//
// REDRAW_PENDING:		Non-zero means a DoWhenIdle handler has been
//				queued to redraw this window.
//
// WIDGET_DELETED:		Non-zero means that the notebook has been,
//				or is in the process of being, deleted.
//
// RESIZE_PENDING:		Non-zero means that the window might need to
//				change its size (or the size of its panes)
//				because of a change in the size of one of its
//				children.
#define REDRAW_PENDING			0x0001
#define WIDGET_DELETED			0x0002
#define REQUESTED_RELAYOUT		0x0004
#define RECOMPUTE_GEOMETRY		0x0008
#define PROXY_REDRAW_PENDING	0x0010
#define RESIZE_PENDING			0x0020


// declarations
static void NotebookReqProc(ClientData, Tk_Window);
static void NotebookLostSlaveProc(ClientData, Tk_Window);
static int SetSticky(ClientData, Tcl_Interp*, Tk_Window, Tcl_Obj**, char*, int, char*, int);
static Tcl_Obj* GetSticky(ClientData, Tk_Window, char*, int);
static void RestoreSticky(ClientData, Tk_Window, char*, char*);
static int SetPadding(ClientData, Tcl_Interp*, Tk_Window, Tcl_Obj**, char*, int, char*, int);
static Tcl_Obj* GetPadding(ClientData, Tk_Window, char*, int);
static void RestorePadding(ClientData, Tk_Window, char*, char*);


static const Tk_GeomMgr notebookMgrType =
{
    "notebook",				// name
    NotebookReqProc,			// requestProc
    NotebookLostSlaveProc,	// lostSlaveProc
};


// Information used for objv parsing.
#define GEOMETRY		0x0001


// The following structure contains pointers to functions used for processing
// the custom "-sticky" option for slave windows.
static Tk_ObjCustomOption stickyOption =
{
    "sticky",			/* name */
    SetSticky,			/* setProc */
    GetSticky,			/* getProc */
    RestoreSticky,	/* restoreProc */
    nullptr,			/* freeProc */
    0
};


static Tk_ObjCustomOption paddingOption =
{
    "padding",			/* name */
    SetPadding,		/* setProc */
    GetPadding,		/* getProc */
    RestorePadding,	/* restoreProc */
    nullptr,			/* freeProc */
    0
};


#if defined(__MacOSX__)
# define NORMAL_BG	"systemWindowBody"
#elif defined(WIN32)
# define NORMAL_BG	"SystemButtonFace"
#elif defined(__unix__)
# define NORMAL_BG	"#d9d9d9"
#else
# error "unsupported platform"
#endif

#define DEF_NOTEBOOK_BG_COLOR		NORMAL_BG
#define DEF_NOTEBOOK_BG_MONO		"#ffffff"
#define DEF_NOTEBOOK_BORDERWIDTH	"1"
#define DEF_NOTEBOOK_OVERLAY		"0"
#define DEF_NOTEBOOK_CURSOR		""
#define DEF_NOTEBOOK_HEIGHT		""
#define DEF_NOTEBOOK_RELIEF		"flat"
#define DEF_NOTEBOOK_WIDTH			""

static const Tk_OptionSpec optionSpecs[] =
{
   {TK_OPTION_BORDER, "-background", "background", "Background",
	 DEF_NOTEBOOK_BG_COLOR, -1, Tk_Offset(Notebook, background),
	 0, (ClientData)DEF_NOTEBOOK_BG_MONO},
   {TK_OPTION_SYNONYM, "-bd", nullptr, nullptr,
	 nullptr, 0, -1, 0, (ClientData) "-borderwidth"},
   {TK_OPTION_SYNONYM, "-bg", nullptr, nullptr,
	 nullptr, 0, -1, 0, (ClientData) "-background"},
   {TK_OPTION_PIXELS, "-borderwidth", "borderWidth", "BorderWidth",
	 DEF_NOTEBOOK_BORDERWIDTH, -1, Tk_Offset(Notebook, borderWidth),
	 0, 0, GEOMETRY},
   {TK_OPTION_CURSOR, "-cursor", "cursor", "Cursor",
	 DEF_NOTEBOOK_CURSOR, -1, Tk_Offset(Notebook, cursor),
	 TK_OPTION_NULL_OK, 0, 0},
   {TK_OPTION_PIXELS, "-height", "height", "Height",
	 DEF_NOTEBOOK_HEIGHT, Tk_Offset(Notebook, heightObj),
	 Tk_Offset(Notebook, height), TK_OPTION_NULL_OK, 0, GEOMETRY},
	 {TK_OPTION_BOOLEAN, "-overlay", "overlay", "Overlay",
	 DEF_NOTEBOOK_OVERLAY, -1, Tk_Offset(Notebook, overlay),
	 0, 0, 0},
	{TK_OPTION_STRING, "-padding", "padding", "Padding", "0",
	 Tk_Offset(Notebook, paddingObj), -1, 0,
	 (ClientData)&paddingOption, GEOMETRY },
   {TK_OPTION_RELIEF, "-relief", "relief", "Relief",
	 DEF_NOTEBOOK_RELIEF, -1, Tk_Offset(Notebook, relief), 0, 0, 0},
   {TK_OPTION_PIXELS, "-width", "width", "Width",
	 DEF_NOTEBOOK_WIDTH, Tk_Offset(Notebook, widthObj),
	 Tk_Offset(Notebook, width), TK_OPTION_NULL_OK, 0, GEOMETRY},
   {TK_OPTION_END}
};


#define DEF_NOTEBOOK_PANE_AFTER		""
#define DEF_NOTEBOOK_PANE_BEFORE		""
#define DEF_NOTEBOOK_PANE_HEIGHT		""
#define DEF_NOTEBOOK_PANE_HIDE			"0"
#define DEF_NOTEBOOK_PANE_STICKY	"nsew"
#define DEF_NOTEBOOK_PANE_UNDERLINE	"-1"
#define DEF_NOTEBOOK_PANE_WIDTH		""

static char *stateStrings[] = {
	"disabled", "normal", "hidden", NULL
};

static const Tk_OptionSpec paneOptionSpecs[] =
{
   {TK_OPTION_WINDOW, "-after", nullptr, nullptr,
	 DEF_NOTEBOOK_PANE_AFTER, -1, Tk_Offset(Slave, after),
	TK_OPTION_NULL_OK, 0, 0},
   {TK_OPTION_WINDOW, "-before", nullptr, nullptr,
	 DEF_NOTEBOOK_PANE_BEFORE, -1, Tk_Offset(Slave, before),
	TK_OPTION_NULL_OK, 0, 0},
   {TK_OPTION_PIXELS, "-height", nullptr, nullptr,
	 DEF_NOTEBOOK_PANE_HEIGHT, Tk_Offset(Slave, heightObj),
	 Tk_Offset(Slave, height), TK_OPTION_NULL_OK, 0, 0},
	{TK_OPTION_STRING, "-padding", "padding", "Padding", "0",
	 Tk_Offset(Slave, paddingObj), -1, 0,
	 (ClientData)&paddingOption, GEOMETRY },
	{TK_OPTION_STRING_TABLE, "-state", "", "",
	 "normal", -1, Tk_Offset(Slave, state), 0, ClientData(stateStrings), 0 },
   {TK_OPTION_CUSTOM, "-sticky", nullptr, nullptr,
	 DEF_NOTEBOOK_PANE_STICKY, -1, Tk_Offset(Slave, sticky), 0,
	 (ClientData)&stickyOption, GEOMETRY },
	{TK_OPTION_STRING, "-text", "text", "Text", "",
	 Tk_Offset(Slave,textObj), -1, 0, 0, GEOMETRY },
	{TK_OPTION_INT, "-underline", "underline", "Underline",
	 DEF_NOTEBOOK_PANE_UNDERLINE, Tk_Offset(Slave, underlineObj),
	 -1, 0, 0, GEOMETRY },
   {TK_OPTION_PIXELS, "-width", nullptr, nullptr,
	 DEF_NOTEBOOK_PANE_WIDTH, Tk_Offset(Slave, widthObj),
	Tk_Offset(Slave, width), TK_OPTION_NULL_OK, 0, 0},
   {TK_OPTION_END}
};



//----------------------------------------------------------------------
//
// SendVirtualEvent --
//
// 	Send a virtual event notification to the specified target window.
// 	Equivalent to "event generate $tgtWindow <<$eventName>>"
//
// 	Note that we use Tk_QueueWindowEvent, not Tk_HandleEvent,
// 	so this routine does not reenter the interpreter.
//
//----------------------------------------------------------------------
static void
SendVirtualEvent(Tk_Window tkwin, const char* eventName)
{
    XEvent event;

    memset(&event, 0, sizeof(event));
    event.xany.type = VirtualEvent;
    event.xany.serial = NextRequest(Tk_Display(tkwin));
    event.xany.send_event = False;
    event.xany.window = Tk_WindowId(tkwin);
    event.xany.display = Tk_Display(tkwin);
    ((XVirtualEvent *) &event)->name = Tk_GetUid(eventName);

    Tk_QueueWindowEvent(&event, TCL_QUEUE_TAIL);
}


//----------------------------------------------------------------------
//
// ObjectIsEmpty --
//
//	This function tests whether the string value of an object is empty.
//
// Results:
//	The return value is 1 if the string value of objPtr has length zero,
//	and 0 otherwise.
//
// Side effects:
//	May cause object shimmering, since this function can force a
//	conversion to a string object.
//
//----------------------------------------------------------------------
static int
ObjectIsEmpty(Tcl_Obj* objPtr)	// Object to test, may be nullptr
{
	int length;

	if (objPtr == nullptr)
		return 1;

	if (objPtr->bytes != nullptr)
		return (objPtr->length == 0);

	Tcl_GetStringFromObj(objPtr, &length);
	return length == 0;
}


//----------------------------------------------------------------------
//
// ComputeSlotAddress --
//
//	Given a pointer to the start of a record and the offset of a slot
//	within that record, compute the address of that slot.
//
// Results:
//	If offset is non-negative, returns the computed address; else, returns
//	nullptr.
//
// Side effects:
//	None.
//
//----------------------------------------------------------------------
static char*
ComputeSlotAddress(	char* recordPtr,	// Pointer to the start of a record
							int offset)			// Offset of a slot within that record; may be < 0
{
	return offset >= 0 ? recordPtr + offset : nullptr;
}


//----------------------------------------------------------------------
//
// GetPane --
//
//	Given a token to a Tk window, find the pane that corresponds to that
//	token in a given notebook.
//
// Results:
//	Pointer to the slave structure, or nullptr if the window is not managed
//	by this notebook.
//
// Side effects:
//	None.
//
//----------------------------------------------------------------------
static Slave*
GetPane(	Notebook* nb,		// Pointer to the notebook info
			Tk_Window tkwin)	// Window to search for
{
	int i;

	for (i = 0; i < nb->numSlaves; i++)
	{
		if (nb->slaves[i]->tkwin == tkwin)
			return nb->slaves[i];
	}

	return nullptr;
}


//----------------------------------------------------------------------
//
// GetPadding -
//
//	Converts an internal boolean combination of "padding" pixels into a Tcl
//	string obj containing four pixel elements.
//
// Results:
//	Tcl_Obj containing the string representation of the padding value.
//
// Side effects:
//	Creates a new Tcl_Obj.
//
//----------------------------------------------------------------------
static Tcl_Obj*
GetPadding(	ClientData clientData,
				Tk_Window tkwin,
				char* recordPtr,			// Pointer to widget record
				int internalOffset)		// Offset within *recordPtr containing the padding value
{
	Padding*	pad = (Padding*)(recordPtr + internalOffset);
	Tcl_Obj*	values[4];

	values[0] = Tcl_NewIntObj(pad->left);
	values[1] = Tcl_NewIntObj(pad->top);
	values[2] = Tcl_NewIntObj(pad->right);
	values[3] = Tcl_NewIntObj(pad->bottom);

	return Tcl_NewListObj(4, values);
}


//----------------------------------------------------------------------
//
// SetPadding --
//
//	Converts a Tcl_Obj representing a widgets padding into a padding
//	value.
//
// Results:
//	Standard Tcl result.
//
// Side effects:
//	May store the integer value into the internal representation pointer.
//	May change the pointer to the Tcl_Obj to nullptr to indicate that the
//	specified string was empty and that is acceptable.
//
//----------------------------------------------------------------------
static int
SetPadding(	ClientData clientData,
				Tcl_Interp* interp,		// Current interp; may be used for errors
				Tk_Window tkwin,			// Window for which option is being set
				Tcl_Obj** value,			// Pointer to the pointer to the value object.
												// We use a pointer to the pointer because we
												// may need to return a value (nullptr)
				char* recordPtr,			// Pointer to storage for the widget record
				int internalOffset,		// Offset within *recordPtr at which the
												// internal value is to be stored
				char* oldInternalPtr,	// Pointer to storage for the old value
				int flags)					// Flags for the option, set Tk_SetOptions
{
	char* internalPtr	= ComputeSlotAddress(recordPtr, internalOffset);
	Padding padding;

	padding.left = padding.top = padding.right = padding.bottom = 0;

	if ((flags & TK_OPTION_NULL_OK) && ObjectIsEmpty(*value))
	{
		*value = nullptr;
	}
	else
	{
		int padc;
		int i;
		int pixels[4];
		Tcl_Obj **padv;

		if (Tcl_ListObjGetElements(interp, *value, &padc, &padv) != TCL_OK)
			return TCL_ERROR;

		if (padc > 4)
		{
			Tcl_ResetResult(interp);
			Tcl_AppendResult(interp, "Wrong #elements in padding spec", NULL);
		}

		memset(pixels, 0, sizeof(pixels));

		for (i = 0; i < padc; ++i)
		{
			if (Tk_GetPixelsFromObj(interp, tkwin, padv[i], &pixels[i]) != TCL_OK)
				return TCL_ERROR;
		}
		
		padding.left   = pixels[0];
		padding.top    = pixels[1];
		padding.right  = pixels[2];
		padding.bottom = pixels[3];
	}

	if (internalPtr != nullptr)
	{
		memcpy(oldInternalPtr, internalPtr, sizeof(Padding));
		memcpy(internalPtr, &padding, sizeof(Padding));
	}

	return TCL_OK;
}


//----------------------------------------------------------------------
//
// RestorePadding --
//
//	Restore a sticky option value from a saved value.
//
// Results:
//	None.
//
// Side effects:
//	Restores the old value.
//
//----------------------------------------------------------------------
static void
RestorePadding(ClientData clientData,
					Tk_Window tkwin,
					char* internalPtr,		// Pointer to storage for value
					char* oldInternalPtr)	// Pointer to old value
{
	memcpy(internalPtr, oldInternalPtr, sizeof(Padding));
}


//----------------------------------------------------------------------
//
// GetSticky -
//
//	Converts an internal boolean combination of "sticky" bits into a Tcl
//	string obj containing zero or more of n, s, e, or w.
//
// Results:
//	Tcl_Obj containing the string representation of the sticky value.
//
// Side effects:
//	Creates a new Tcl_Obj.
//
//----------------------------------------------------------------------
static Tcl_Obj*
GetSticky(	ClientData clientData,
				Tk_Window tkwin,
				char* recordPtr,			// Pointer to widget record
				int internalOffset)		// Offset within *recordPtr containing the sticky value
{
	char	buffer[5];
	int	sticky		= *(int*)(recordPtr + internalOffset);
	int	count			= 0;

	if (sticky & STICK_NORTH)
		buffer[count++] = 'n';
	if (sticky & STICK_EAST)
		buffer[count++] = 'e';
	if (sticky & STICK_SOUTH)
		buffer[count++] = 's';
	if (sticky & STICK_WEST)
		buffer[count++] = 'w';
	buffer[count] = '\0';

	return Tcl_NewStringObj(buffer, -1);
}


//----------------------------------------------------------------------
//
// SetSticky --
//
//	Converts a Tcl_Obj representing a widgets stickyness into an integer
//	value.
//
// Results:
//	Standard Tcl result.
//
// Side effects:
//	May store the integer value into the internal representation pointer.
//	May change the pointer to the Tcl_Obj to nullptr to indicate that the
//	specified string was empty and that is acceptable.
//
//----------------------------------------------------------------------
static int
SetSticky(	ClientData clientData,
				Tcl_Interp* interp,		// Current interp; may be used for errors
				Tk_Window tkwin,			// Window for which option is being set
				Tcl_Obj** value,			// Pointer to the pointer to the value object.
												// We use a pointer to the pointer because we
												// may need to return a value (nullptr)
				char* recordPtr,			// Pointer to storage for the widget record
				int internalOffset,		// Offset within *recordPtr at which the
												// internal value is to be stored
				char* oldInternalPtr,	// Pointer to storage for the old value
				int flags)					// Flags for the option, set Tk_SetOptions
{
	char* internalPtr	= ComputeSlotAddress(recordPtr, internalOffset);
	int	sticky		= 0;

	if ((flags & TK_OPTION_NULL_OK) && ObjectIsEmpty(*value))
	{
		*value = nullptr;
	}
	else
	{
		// Convert the sticky specifier into an integer value.

		char* string = Tcl_GetString(*value);
		char	c;

		while ((c = toupper(*string++)) != '\0')
		{
			switch (c)
			{
				case 'N': sticky |= STICK_NORTH;	break;
				case 'E': sticky |= STICK_EAST;	break;
				case 'S': sticky |= STICK_SOUTH;	break;
				case 'W': sticky |= STICK_WEST;	break;

				default:
					if (c != ',' && !isspace(c))
					{
						Tcl_ResetResult(interp);
						Tcl_AppendResult(	interp,
												"bad stickyness value \"",
												Tcl_GetString(*value), "\": must be a string ",
												"containing zero or more of n, e, s, and w",
												nullptr);
						return TCL_ERROR;
					}
			}
		}
	}

	if (internalPtr != nullptr)
	{
		*((int*)oldInternalPtr) = *((int*)internalPtr);
		*((int*)internalPtr) = sticky;
	}

	return TCL_OK;
}


//----------------------------------------------------------------------
//
// RestoreSticky --
//
//	Restore a sticky option value from a saved value.
//
// Results:
//	None.
//
// Side effects:
//	Restores the old value.
//
//----------------------------------------------------------------------
static void
RestoreSticky(	ClientData clientData,
					Tk_Window tkwin,
					char* internalPtr,		// Pointer to storage for value
					char* oldInternalPtr)	// Pointer to old value
{
    *(int*)internalPtr = *(int*)oldInternalPtr;
}


//----------------------------------------------------------------------
//
// AdjustForSticky --
//
//	Given the x,y coords of the top-left corner of a pane, the dimensions
//	of that pane, and the dimensions of a slave, compute the x,y coords
//	and actual dimensions of the slave based on the slave's sticky value.
//
// Results:
//	No direct return; sets the x, y, slaveWidth and slaveHeight to correct
//	values.
//
// Side effects:
//	None.
//
//----------------------------------------------------------------------
static void
AdjustForSticky(	int sticky,				// Sticky value; see top of file for definition
						int cavityWidth,		// Width of the cavity
						int cavityHeight,		// Height of the cavity
						int *xPtr,
						int *yPtr,				// Initially, coordinates of the top-left
													// corner of cavity; also return values for
													// actual x, y coords of slave
						int *slaveWidthPtr,	// Slave width
						int *slaveHeightPtr)	// Slave height
{
	int diffx = cavityWidth - *slaveWidthPtr;
	int diffy = cavityHeight - *slaveHeightPtr;

	if ((sticky & STICK_EAST) && (sticky & STICK_WEST))
		*slaveWidthPtr += diffx;

	if ((sticky & STICK_NORTH) && (sticky & STICK_SOUTH))
		*slaveHeightPtr += diffy;

	if (diffx > 0 && !(sticky & STICK_WEST))
		*xPtr += sticky & STICK_EAST ? diffx : diffx/2;

	if (diffy > 0 && !(sticky & STICK_NORTH))
		*yPtr += sticky & STICK_SOUTH ? diffy : diffy/2;
}


//----------------------------------------------------------------------
//
// ComputeTabrowSize --
//
//	Compute geometry for the tab row.
//
// Results:
//	Stores geometry in 'width' and 'height'.
//
// Side effects:
//	None.
//
//----------------------------------------------------------------------
static void
ComputeTabrowSize(Notebook* nb, int* width, int* height)
{
	/* TODO */
	*width = 20;
	*height = 20;
}


//--------------------------------------------------------------
//
// ArrangePane --
//
//	This function is invoked (using the Tcl_DoWhenIdle mechanism) to
//	re-layout the visible window managed by a notebook. It is
//	invoked at idle time so that a series of pane requests can be
//	merged into a single layout operation.
//
// Results:
//	None.
//
// Side effects:
//	The visible slave of master may get resized or moved.
//
//--------------------------------------------------------------
static void
ArrangePane(ClientData clientData)	// Structure describing parent whose slaves are to be re-layed out
{
	Notebook* nb = (Notebook*)clientData;

	int doubleBw;
	int paneWidth;
	int paneHeight;
	int slaveWidth;
	int slaveHeight;
	int slaveX;
	int slaveY;
	int tabrowWidth;
	int tabrowHeight;

	Slave* slave;

	// If the parent has no slaves anymore, then don't do anything at all:
	// just leave the parent's size as-is. Otherwise there is no way to
	// "relinquish" control over the parent so another geometry manager can
	// take over.

	if (nb->numSlaves == 0 || nb->slaves[0]->hide)
		return;

	Tcl_Preserve((ClientData)nb);
	ComputeTabrowSize(nb, &tabrowWidth, &tabrowHeight);

	slave = nb->slaves[0];
	doubleBw = 2*Tk_Changes(slave->tkwin)->border_width;

	slaveX = nb->padding.left;
	slaveY = nb->padding.top;
	slaveWidth = slave->width > 0 ? slave->width : Tk_ReqWidth(slave->tkwin) + doubleBw;
	slaveHeight = slave->height > 0 ? slave->height : Tk_ReqHeight(slave->tkwin) + doubleBw;
	paneWidth = (nb->width > 0 ? nb->width : Tk_Width(nb->tkwin));
	paneHeight = (nb->height > 0 ? nb->height : Tk_Height(nb->tkwin));
	paneWidth -= nb->padding.left + nb->padding.right;
	paneHeight -= tabrowHeight + nb->padding.top - nb->padding.bottom;

	AdjustForSticky(slave->sticky, paneWidth, paneHeight, &slaveX, &slaveY, &slaveWidth, &slaveHeight);

	if (slaveWidth <= 0 || slaveHeight <= 0)
	{
		Tk_UnmaintainGeometry(slave->tkwin, nb->tkwin);
		Tk_UnmapWindow(slave->tkwin);
	}
	else
	{
		Tk_MaintainGeometry(slave->tkwin, nb->tkwin, slaveX, slaveY, slaveWidth, slaveHeight);
	}

	Tcl_Release((ClientData)nb);
}


//----------------------------------------------------------------------
//
// RaiseSlave --
//
// Rotate slaves until wanted slave is at top.
//
//----------------------------------------------------------------------
static void
RaiseSlave(	Notebook* nb,	// Information about notebook
				Slave* slave)	// New top slave, use first unhidden slave if zero
{
	int index = 0;
	Slave** slaves;

	if (slave == 0)
	{
		while (index < nb->numSlaves && nb->slaves[index]->hide)
			index++;

		if (index == 0)
			return;
	}
	else
	{
		slave->hide = 0;

		while (nb->slaves[index] != slave)
			index++;
	}

	slaves = (Slave**)ckalloc(sizeof(Slave*)*nb->numSlaves);
	memcpy(slaves, nb->slaves, sizeof(Slave*)*nb->numSlaves);

	memcpy(nb->slaves, slaves + index, sizeof(Slave*)*(nb->numSlaves - index));
	memcpy(nb->slaves + nb->numSlaves - index, slaves, sizeof(Slave*)*index);

	ckfree((char*)slaves);
}


//----------------------------------------------------------------------
//
// UnmapSlave --
//
// Unmap the specified slave, but leave it managed.
//
//----------------------------------------------------------------------
static void
UnmapSlave(	Notebook* nb,	// Information about notebook
				Slave* slave)	// Slave to unmap, use first unhidden slave if zero
{
	if (nb->overlay)
		return;

	if (slave == 0 && nb->numSlaves > 0 && !nb->slaves[0]->hide)
		slave = nb->slaves[0];

	if (slave)
	{
		Tk_UnmaintainGeometry(slave->tkwin, nb->tkwin);
		// Contrary to documentation, Tk_UnmaintainGeometry doesn't always unmap the slave:
		Tk_UnmapWindow(slave->tkwin);
	}
}


//----------------------------------------------------------------------
//
// PlaceSlave --
//
// Set the position and size of a child widget
// based on the current client area and slave options.
//
//----------------------------------------------------------------------
static void
PlaceSlave(	Notebook* nb,		// Information about notebook
				Slave* oldSlave)	// Previously mapped slave
{
	if (nb->numSlaves > 0 && !nb->slaves[0]->hide)
	{
		ArrangePane(nb);

		if (Tk_IsMapped(nb->tkwin))
		{
			Slave* slave = nb->slaves[0];

			Tk_RestackWindow(slave->tkwin, Above, nullptr);
			Tk_MapWindow(slave->tkwin);
		}
	}
}


//--------------------------------------------------------------
//
// DisplayNotebook --
//
//	This function redraws the contents of a notebook widget. It is
//	invoked as a do-when-idle handler, so it only runs when there's
//	nothing else for the application to do.
//
// Results:
//	None.
//
// Side effects:
//	Information appears on the screen.
//
//--------------------------------------------------------------
static void
DisplayNotebook(ClientData clientData)	// Information about window
{
	Notebook*	nb			= (Notebook*)clientData;
	Tk_Window	tkwin		= nb->tkwin;
	Pixmap		pixmap;

	nb->flags &= ~REDRAW_PENDING;
	if ((nb->tkwin == nullptr) || !Tk_IsMapped(tkwin))
		return;

	if (nb->flags & REQUESTED_RELAYOUT)
		ArrangePane(clientData);

#ifndef TK_NO_DOUBLE_BUFFERING
	// Create a pixmap for double-buffering, if necessary.
	pixmap = Tk_GetPixmap(	Tk_Display(tkwin),
									Tk_WindowId(tkwin),
									Tk_Width(tkwin),
									Tk_Height(tkwin),
									Tk_Depth(tkwin));
#else
	pixmap = Tk_WindowId(tkwin);
#endif

	// Redraw the widget's background and border.
	Tk_Fill3DRectangle(	tkwin,
								pixmap,
								nb->background,
								0,
								0,
								Tk_Width(tkwin),
								Tk_Height(tkwin),
								nb->borderWidth,
								nb->relief);

#ifndef TK_NO_DOUBLE_BUFFERING
	// Copy the information from the off-screen pixmap onto the screen, then
	// delete the pixmap.

	XCopyArea(	Tk_Display(tkwin),
					pixmap,
					Tk_WindowId(tkwin),
					nb->gc,
					0,
					0,
					(unsigned)Tk_Width(tkwin),
					(unsigned)Tk_Height(tkwin),
					0,
					0);
	Tk_FreePixmap(Tk_Display(tkwin), pixmap);
#endif /* TK_NO_DOUBLE_BUFFERING */
}


//----------------------------------------------------------------------
//
// ComputeGeometry --
//
//	Compute geometry for the notebook, including coordinates of all
//	slave windows and each sash.
//
// Results:
//	None.
//
// Side effects:
//	Recomputes geometry information for a notebook.
//
//----------------------------------------------------------------------
static void
ComputeGeometry(Notebook* nb)		// Pointer to the Notebook structure
{
	int i;
	int clientWidth	= 0;
	int clientHeight	= 0;
	int reqWidth		= 0;
	int reqHeight		= 0;
	int tabrowWidth;
	int tabrowHeight;
	int internalBw;

	ComputeTabrowSize(nb, &tabrowWidth, &tabrowHeight);

	nb->flags |= REQUESTED_RELAYOUT;

	// Compute max requested size of all slaves:
	for (i = 0; i < nb->numSlaves; i++)
	{
		if (!nb->slaves[i]->hide)
		{
			Tk_Window slaveWindow = nb->slaves[i]->tkwin;

			int doubleBw = 2*Tk_Changes(slaveWindow)->border_width;
			int slaveWidth = Tk_ReqWidth(slaveWindow) + doubleBw;
			int slaveHeight = Tk_ReqHeight(slaveWindow) + doubleBw;

			slaveWidth += nb->slaves[i]->padding.left + nb->slaves[i]->padding.right;
			slaveHeight += nb->slaves[i]->padding.top + nb->slaves[i]->padding.bottom;

			clientWidth = MAX(clientWidth, slaveWidth);
			clientHeight = MAX(clientHeight, slaveHeight);
		}
	}

	// Client width/height overridable by widget options:
	if (nb->widthObj)
		Tcl_GetIntFromObj(nullptr, nb->widthObj, &reqWidth);
	if (nb->heightObj)
		Tcl_GetIntFromObj(nullptr, nb->heightObj, &reqHeight);
	if (reqWidth > 0)
		clientWidth = reqWidth;
	if (reqHeight > 0)
		clientHeight = reqHeight;

	internalBw = Tk_InternalBorderWidth(nb->tkwin);

	clientWidth += internalBw;
	clientHeight += internalBw + tabrowHeight;

	clientWidth = MAX(clientWidth, tabrowWidth);
	Tk_GeometryRequest(nb->tkwin, clientWidth, clientHeight);

	if (Tk_IsMapped(nb->tkwin) && !(nb->flags & REDRAW_PENDING))
	{
		nb->flags |= REDRAW_PENDING;
		Tcl_DoWhenIdle(DisplayNotebook, (ClientData)nb);
	}
}


//----------------------------------------------------------------------
//
// Unlink --
//
//	Remove a slave from a notebook.
//
// Results:
//	None.
//
// Side effects:
//	The notebook will be scheduled for re-arranging and redrawing.
//
//----------------------------------------------------------------------
static void
Unlink(Slave* slave)		// Window to unlink
{
	Notebook *master = slave->master;
	int i;

	if (master == nullptr)
		return;

	// Find the specified slave in the notebook's list of slaves, then
	// remove it from that list.

	for (i = 0; i < master->numSlaves; i++)
	{
		if (master->slaves[i] == slave)
		{
			memmove(master->slaves + i, master->slaves + i + 1, sizeof(Slave*)*(master->numSlaves - i - 1));
			break;
		}
	}

	// Clean out any -after or -before references to this slave

	for (i = 0; i < master->numSlaves; i++)
	{
		if (master->slaves[i]->before == slave->tkwin)
			master->slaves[i]->before = None;

		if (master->slaves[i]->after == slave->tkwin)
			master->slaves[i]->after = None;
	}

	master->flags |= REQUESTED_RELAYOUT;

	if (!(master->flags & REDRAW_PENDING))
	{
		master->flags |= REDRAW_PENDING;
		Tcl_DoWhenIdle(DisplayNotebook, (ClientData)master);
	}

	// Set the slave's master to nullptr, so that we can tell that the slave
	// is no longer attached to any notebook.

	slave->master = nullptr;
	master->numSlaves--;
}


//--------------------------------------------------------------
//
// SlaveStructureProc --
//
//	This function is invoked whenever StructureNotify events occur for a
//	window that's managed by a notebook. This function's only purpose
//	is to clean up when windows are deleted.
//
// Results:
//	None.
//
// Side effects:
//	The notebook slave structure associated with the window
//	is freed, and the slave is disassociated from the notebook
//	window which managed it.
//
//--------------------------------------------------------------
static void
SlaveStructureProc(	ClientData clientData,	// Pointer to record describing window item
							XEvent* eventPtr)			// Describes what just happened
{
	Slave*		slave	= (Slave*)clientData;
	Notebook*	nb		= slave->master;

	if (eventPtr->type == DestroyNotify)
	{
		Unlink(slave);
		slave->tkwin = nullptr;
		ckfree((char*)slave);
		ComputeGeometry(nb);
	}
}


//----------------------------------------------------------------------
//
// ConfigureSlaves --
//
//	Add or alter the configuration options of a slave in a notebook.
//
// Results:
//	Standard Tcl result.
//
// Side effects:
//	Depends on options; may add a slave to the notebook, may alter the
//	geometry management options of a slave.
//
//----------------------------------------------------------------------
static int
ConfigureSlaves(	Notebook* nb,				// Information about notebook
						Tcl_Interp* interp,		// Current interpreter
						int objc,					// Number of arguments
						Tcl_Obj* const objv[])	// Argument objects
{
	int i, j;
	int firstOptionArg;
	int found;
	int index;
	int numNewSlaves;
	int haveLoc;
	int insertIndex;

	Tk_Window	tkwin = nullptr;
	Tk_Window	ancestor;
	Tk_Window	parent;
	Slave**		inserts;
	Slave**		newSlaves;
	Slave			options;

	// Find the non-window name arguments; these are the configure options for
	// the slaves. Also validate that the window names given are legitimate
	// (ie, they are real windows, they are not the notebook itself, etc.).

	for (i = 2; i < objc; i++)
	{
		char* arg = Tcl_GetString(objv[i]);

		if (arg[0] == '-')
			break;

		tkwin = Tk_NameToWindow(interp, arg, nb->tkwin);

		if (tkwin == nullptr)
		{
			// Just a plain old bad window;
			// Tk_NameToWindow filled in an error message for us.
			return TCL_ERROR;
		}

		if (tkwin == nb->tkwin)
		{
			// A notebook cannot manage itself.
			Tcl_ResetResult(interp);
			Tcl_AppendResult(interp, "can't add ", arg, " to itself", nullptr);
			return TCL_ERROR;
		}

		if (Tk_IsTopLevel(tkwin))
		{
			// A notebook cannot manage a toplevel.
			Tcl_ResetResult(interp);
			Tcl_AppendResult(	interp,
									"can't add toplevel ",
									arg,
									" to ",
									Tk_PathName(nb->tkwin),
									nullptr);
			return TCL_ERROR;
		}

		// Make sure the notebook is the parent of the slave,
		// or a descendant of the slave's parent.

		parent = Tk_Parent(tkwin);

		for (ancestor = nb->tkwin; ancestor != parent; ancestor = Tk_Parent(ancestor))
		{
			if (Tk_IsTopLevel(ancestor))
			{
				Tcl_ResetResult(interp);
				Tcl_AppendResult(interp, "can't add ", arg, " to ", Tk_PathName(nb->tkwin), nullptr);
				return TCL_ERROR;
			}
		}
	}

	firstOptionArg = i;

	// Pre-parse the configuration options, to get the before/after specifiers
	// into an easy-to-find location (a local variable). Also, check the
	// return from Tk_SetOptions once, here, so we can save a little bit of
	// extra testing in the for loop below.

	memset(&options, 0, sizeof(Slave));

	if (Tk_SetOptions(interp,
							(char*)&options,
							nb->paneOptions,
							objc - firstOptionArg,
							objv + firstOptionArg,
							nb->tkwin,
							nullptr,
							nullptr) != TCL_OK)
	{
		return TCL_ERROR;
	}

	// If either -after or -before was given, find the numerical index that
	// corresponds to the given window. If both -after and -before are given,
	// the option precedence is: -after, then -before.

	index = -1;
	haveLoc = 0;

	if (options.after != None)
	{
		tkwin = options.after;
		haveLoc = 1;

		for (i = 0; i < nb->numSlaves; i++)
		{
			if (options.after == nb->slaves[i]->tkwin)
			{
				index = i + 1;
				break;
			}
		}
	}
	else if (options.before != None)
	{
		tkwin = options.before;
		haveLoc = 1;

		for (i = 0; i < nb->numSlaves; i++)
		{
			if (options.before == nb->slaves[i]->tkwin)
			{
				index = i;
				break;
			}
		}
	}

	// If a window was given for -after/-before, but it's not a window managed
	// by the notebook, throw an error

	if (haveLoc && index == -1)
	{
		Tcl_ResetResult(interp);
		Tcl_AppendResult(	interp,
								"window \"",
								Tk_PathName(tkwin),
								"\" is not managed by ",
								Tk_PathName(nb->tkwin),
								nullptr);
		Tk_FreeConfigOptions((char*)&options, nb->paneOptions, nb->tkwin);
		return TCL_ERROR;
	}

	// Allocate an array to hold, in order, the pointers to the slave
	// structures corresponding to the windows specified. Some of those
	// structures may already have existed, some may be new.

	inserts = (Slave**)ckalloc(sizeof(Slave*)*(firstOptionArg - 2));
	insertIndex = 0;

	// Populate the inserts array, creating new slave structures as necessary,
	// applying the options to each structure as we go, and, if necessary,
	// marking the spot in the original slaves array as empty (for
	// pre-existing slave structures).

	for (i = 0, numNewSlaves = 0; i < firstOptionArg - 2; i++)
	{
		// We don't check that tkwin is nullptr here, because the pre-pass above
		// guarantees that the input at this stage is good.

		tkwin = Tk_NameToWindow(interp, Tcl_GetString(objv[i + 2]), nb->tkwin);
		found = 0;

		for (j = 0; j < nb->numSlaves; j++)
		{
			if (nb->slaves[j] != nullptr && nb->slaves[j]->tkwin == tkwin)
			{
				Tk_SetOptions(	interp,
									(char*)nb->slaves[j],
									nb->paneOptions,
									objc - firstOptionArg,
									objv + firstOptionArg,
									nb->tkwin,
									nullptr,
									nullptr);

				found = 1;

				// If the slave is supposed to move, add it to the inserts
				// array now; otherwise, leave it where it is.

				if (index != -1)
				{
					inserts[insertIndex++] = nb->slaves[j];
					nb->slaves[j] = nullptr;
				}
				break;
			}
		}

		// Make sure this slave wasn't already put into the inserts array,
		// i.e., when the user specifies the same window notebook times in a
		// single add commaned.

		for (j = 0; j < insertIndex && !found; j++)
		{
			if (inserts[j]->tkwin == tkwin)
				found = 1;
		}

		if (!found)
		{
			// Create a new slave structure and initialize it. All slaves start
			// out with their "natural" dimensions.

			Slave* slave = (Slave*)ckalloc(sizeof(Slave));
			memset(slave, 0, sizeof(Slave));
			Tk_InitOptions(interp, (char *)slave, nb->paneOptions, nb->tkwin);
			Tk_SetOptions(	interp,
								(char*)slave,
								nb->paneOptions,
								objc - firstOptionArg,
								objv + firstOptionArg,
								nb->tkwin,
								nullptr,
								nullptr);
			slave->tkwin = tkwin;
			slave->master = nb;

			// Set up the geometry management callbacks for this slave.

			Tk_CreateEventHandler(	slave->tkwin,
											StructureNotifyMask,
											SlaveStructureProc,
											(ClientData)slave);
			Tk_ManageGeometry(slave->tkwin, &notebookMgrType, (ClientData)slave);
			inserts[insertIndex++] = slave;
			numNewSlaves++;
		}
	}

	// Allocate the new slaves array, then copy the slaves into it, in order.

	i = sizeof(Slave*)*(nb->numSlaves + numNewSlaves);
	newSlaves = (Slave**)ckalloc((unsigned)i);
	memset(newSlaves, 0, (size_t)i);

	if (index == -1)
	{
		// If none of the existing slaves have to be moved, just copy the old
		// and append the new.
		memcpy(newSlaves, nb->slaves, sizeof(Slave*)*nb->numSlaves);
		memcpy(newSlaves + nb->numSlaves, inserts, sizeof(Slave*)*numNewSlaves);
	}
	else
	{
		// If some of the existing slaves were moved, the old slaves array
		// will be partially populated, with some valid and some invalid
		// entries. Walk through it, copying valid entries to the new slaves
		// array as we go; when we get to the insert location for the new
		// slaves, copy the inserts array over, then finish off the old slaves
		// array.

		for (i = 0, j = 0; i < index; i++)
		{
			if (nb->slaves[i] != nullptr)
			{
				newSlaves[j] = nb->slaves[i];
				j++;
			}
		}

		memcpy(newSlaves + j, inserts, sizeof(Slave*)*insertIndex);
		j += firstOptionArg - 2;

		for (i = index; i < nb->numSlaves; i++)
		{
			if (nb->slaves[i] != nullptr)
			{
				newSlaves[j] = nb->slaves[i];
				j++;
			}
		}
	}

	// Make the new slaves array the notebook's slave array, and clean up.

	ckfree((char*)nb->slaves);
	ckfree((char*)inserts);
	nb->slaves = newSlaves;

	// Set the notebook's slave count to the new value.

	nb->numSlaves += numNewSlaves;
	Tk_FreeConfigOptions((char*)&options, nb->paneOptions, nb->tkwin);
	ComputeGeometry(nb);

	return TCL_OK;
}


//----------------------------------------------------------------------
//
// NotebookWorldChanged --
//
//	This function is invoked anytime a notebook's world has changed in
//	some way that causes the widget to have to recompute graphics contexts
//	and geometry.
//
// Results:
//	None.
//
// Side effects:
//	Notebook will be relayed out and redisplayed.
//
//----------------------------------------------------------------------
static void
NotebookWorldChanged(ClientData instanceData)	// Information about the notebook
{
	XGCValues	gcValues;
	GC				newGC;
	Notebook*	nb = (Notebook*)instanceData;

	// Allocated a graphics context for drawing the notebook widget
	// elements (background, sashes, etc.) and set the window background.

	gcValues.background = Tk_3DBorderColor(nb->background)->pixel;
	newGC = Tk_GetGC(nb->tkwin, GCBackground, &gcValues);
	if (nb->gc != None)
		Tk_FreeGC(Tk_Display(nb->tkwin), nb->gc);
	nb->gc = newGC;
	Tk_SetWindowBackground(nb->tkwin, gcValues.background);

	// Issue geometry size requests to Tk.

	Tk_SetInternalBorder(nb->tkwin, nb->borderWidth);
	if (nb->width > 0 && nb->height > 0)
		Tk_GeometryRequest(nb->tkwin, nb->width, nb->height);

	// Arrange for the window to be redrawn, if neccessary.

	if (Tk_IsMapped(nb->tkwin) && !(nb->flags & REDRAW_PENDING))
	{
		Tcl_DoWhenIdle(DisplayNotebook, (ClientData)nb);
		nb->flags |= REDRAW_PENDING;
	}
}


//----------------------------------------------------------------------
//
// ConfigureNotebook --
//
//	This function is called to process an argv/argc list in conjunction
//	with the Tk option database to configure (or reconfigure) a notebook
//	window widget.
//
// Results:
//	The return value is a standard Tcl result. If TCL_ERROR is returned,
//	then the interp's result contains an error message.
//
// Side effects:
//	Configuration information, such as colors, border width, etc. get set
//	for nb; old resources get freed, if there were any.
//
//----------------------------------------------------------------------
static int
ConfigureNotebook(Tcl_Interp* interp,		// Used for error reporting
						Notebook* nb,				// Information about widget
						int objc,					// Number of arguments
						Tcl_Obj* const objv[])	// Argument values
{
	Tk_SavedOptions savedOptions;
	int typemask = 0;

	if (Tk_SetOptions(interp,
							(char*)nb,
							nb->optionTable,
							objc,
							objv,
							nb->tkwin,
							&savedOptions,
							&typemask) != TCL_OK)
	{
		Tk_RestoreSavedOptions(&savedOptions);
		return TCL_ERROR;
	}

	Tk_FreeSavedOptions(&savedOptions);
	NotebookWorldChanged((ClientData)nb);

	// If an option that affects geometry has changed, make a re-layout request.

	if (typemask & GEOMETRY)
		ComputeGeometry(nb);

	return TCL_OK;
}


//--------------------------------------------------------------
//
// NotebookWidgetObjCmd --
//
//	This function is invoked to process the Tcl command that corresponds
//	to a widget managed by this module. See the user documentation for
//	details on what it does.
//
// Results:
//	A standard Tcl result.
//
// Side effects:
//	See the user documentation.
//
//--------------------------------------------------------------
static int
NotebookWidgetObjCmd(ClientData clientData,	// Information about square widget
							Tcl_Interp* interp,		// Current interpreter
							int objc,					// Number of arguments
							Tcl_Obj* const objv[])	// Argument objects
{
	static const char *optionStrings[] =
	{
		"add", "cget", "configure", "forget", "panecget",
		"paneconfigure", "panes", "raise", "unmap", nullptr,
	};
	enum options
	{
		MW_ADD, MW_CGET, MW_CONFIGURE, MW_FORGET, MW_PANECGET,
		MW_PANECONFIGURE, MW_PANES, MW_RAISE, MW_UNMAP,
	};

	Notebook*	nb				= (Notebook*)clientData;
	int			result		= TCL_OK;
	Tcl_Obj*		resultObj;
	int			index;
	int			i;

	if (objc < 2)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "option ?arg arg...?");
		return TCL_ERROR;
	}

	if (Tcl_GetIndexFromObj(interp, objv[1], optionStrings, "command", 0, &index) != TCL_OK)
		return TCL_ERROR;

	Tcl_Preserve((ClientData)nb);

	switch ((enum options)index)
	{
		case MW_ADD:
			if (objc < 3)
			{
				Tcl_WrongNumArgs(interp, 2, objv, "widget ?widget ...?");
				result = TCL_ERROR;
			}
			else
			{
				result = ConfigureSlaves(nb, interp, objc, objv);
			}
			break;

		case MW_CGET:
			if (objc != 3)
			{
				Tcl_WrongNumArgs(interp, 2, objv, "option");
				result = TCL_ERROR;
			}
			else
			{
				resultObj = Tk_GetOptionValue(	interp,
															(char*)nb,
															nb->optionTable,
															objv[2],
															nb->tkwin);
				if (resultObj == nullptr)
					result = TCL_ERROR;
				else
					Tcl_SetObjResult(interp, resultObj);
			}
			break;

		case MW_CONFIGURE:
			resultObj = nullptr;
			if (objc <= 3)
			{
				resultObj = Tk_GetOptionInfo(	interp,
														(char*)nb,
														nb->optionTable,
														objc == 3 ? objv[2] : nullptr,
														nb->tkwin);
				if (resultObj == nullptr)
					result = TCL_ERROR;
				else
					Tcl_SetObjResult(interp, resultObj);
			}
			else
			{
				result = ConfigureNotebook(interp, nb, objc - 2, objv + 2);
			}
			break;

		case MW_FORGET:
			if (objc < 3)
			{
				Tcl_WrongNumArgs(interp, 2, objv, "widget ?widget ...?");
				result = TCL_ERROR;
			}
			else
			{
				int i;

				// Map successor window if required.
				if (nb->numSlaves > 0)
				{
					for (i = 2; i < objc; i++)
					{
						Tk_Window slaveWindow = Tk_NameToWindow(interp, Tcl_GetString(objv[i]), nb->tkwin);

						if (slaveWindow)
						{
							Slave* slave = GetPane(nb, slaveWindow);

							if (slave == nb->slaves[0])
							{
								int nextVisible = 1;

								while (nextVisible < nb->numSlaves && !nb->slaves[nextVisible]->hide)
									++nextVisible;

								if (nextVisible < nb->numSlaves)
								{
									RaiseSlave(nb, nb->slaves[nextVisible]);
									PlaceSlave(nb, slave);
									SendVirtualEvent(nb->tkwin, "NotebookTabChanged");
								}
							}
						}
					}
				}

				// Clean up each window named in the arg list.
				for (i = 2; i < objc; i++)
				{
					Tk_Window slaveWindow = Tk_NameToWindow(interp, Tcl_GetString(objv[i]), nb->tkwin);

					if (slaveWindow)
					{
						Slave* slave = GetPane(nb, slaveWindow);

						if (slave && slave->master)
						{
							Tk_ManageGeometry(slaveWindow, nullptr, (ClientData)nullptr);
							Tk_UnmaintainGeometry(slave->tkwin, nb->tkwin);
							Tk_DeleteEventHandler(	slave->tkwin,
															StructureNotifyMask,
															SlaveStructureProc,
															(ClientData)slave);
							Tk_UnmapWindow(slave->tkwin);
							Unlink(slave);
						}
					}
				}
			}
			break;

			case MW_PANECGET:
				if (objc != 4)
				{
					Tcl_WrongNumArgs(interp, 2, objv, "pane option");
					result = TCL_ERROR;
				}
				else
				{
					Tk_Window tkwin = Tk_NameToWindow(interp, Tcl_GetString(objv[2]), nb->tkwin);

					if (tkwin == nullptr)
					{
						result = TCL_ERROR;
					}
					else
					{
						resultObj = nullptr;

						for (i = 0; i < nb->numSlaves; i++)
						{
							if (nb->slaves[i]->tkwin == tkwin)
							{
								resultObj = Tk_GetOptionValue(interp,
																		(char*)nb->slaves[i],
																		nb->paneOptions,
																		objv[3],
																		tkwin);
							}
						}

						if (i == nb->numSlaves)
							Tcl_SetResult(interp, const_cast<char*>("not managed by this window"), TCL_STATIC);

						if (resultObj == nullptr)
							result = TCL_ERROR;
						else
							Tcl_SetObjResult(interp, resultObj);
					}
				}
				break;

		case MW_PANECONFIGURE:
			if (objc < 3)
			{
				Tcl_WrongNumArgs(interp, 2, objv, "pane ?option? ?value option value ...?");
				result = TCL_ERROR;
			}
			else
			{
				resultObj = nullptr;

				if (objc <= 4)
				{
					Tk_Window tkwin = Tk_NameToWindow(interp, Tcl_GetString(objv[2]), nb->tkwin);

					for (i = 0; i < nb->numSlaves; i++)
					{
						if (nb->slaves[i]->tkwin == tkwin)
						{
							resultObj = Tk_GetOptionInfo(	interp,
																	(char*)nb->slaves[i],
																	nb->paneOptions,
																	objc == 4 ? objv[3] : nullptr,
																	nb->tkwin);
							if (resultObj == nullptr)
								result = TCL_ERROR;
							else
								Tcl_SetObjResult(interp, resultObj);

							break;
						}
					}
				}
				else
				{
					result = ConfigureSlaves(nb, interp, objc, objv);
				}
			}
			break;

		case MW_PANES:
			resultObj = Tcl_NewObj();
			Tcl_IncrRefCount(resultObj);

			for (i = 0; i < nb->numSlaves; i++)
			{
				Tcl_ListObjAppendElement(interp, resultObj,
				Tcl_NewStringObj(Tk_PathName(nb->slaves[i]->tkwin),-1));
			}

			Tcl_SetObjResult(interp, resultObj);
			Tcl_DecrRefCount(resultObj);
			break;

		case MW_RAISE:
			if (objc != 3)
			{
				Tcl_WrongNumArgs(interp, 2, objv, "widget");
				result = TCL_ERROR;
			}
			else
			{
				Tk_Window tkwin = Tk_NameToWindow(interp, Tcl_GetString(objv[2]), nb->tkwin);

				if (nb->numSlaves > 0 && nb->slaves[0]->tkwin != tkwin)
				{
					Slave* slave	= nb->slaves[0];
					Slave* visible	= GetPane(nb, tkwin);

					if (visible)
					{
						RaiseSlave(nb, visible);
						PlaceSlave(nb, slave);
						UnmapSlave(nb, slave);
						SendVirtualEvent(nb->tkwin, "NotebookTabChanged");
					}
				}
			}
			break;

		case MW_UNMAP:
			if (objc < 3)
			{
				Tcl_WrongNumArgs(interp, 2, objv, "widget ?widget ...?");
				result = TCL_ERROR;
			}
			else
			{
				int i;

				for (i = 2; i < objc; i++)
				{
					Tk_Window slaveWindow = Tk_NameToWindow(interp, Tcl_GetString(objv[i]), nb->tkwin);

					if (slaveWindow)
					{
						Slave* slave = GetPane(nb, slaveWindow);

						if (slave && Tk_IsMapped(slave->tkwin))
						{
							Tk_UnmaintainGeometry(slave->tkwin, nb->tkwin);
							Tk_UnmapWindow(slave->tkwin);
						}
					}
				}
			}
			break;
	}

	Tcl_Release((ClientData)nb);
	return result;
}


//----------------------------------------------------------------------
//
// DestroyNotebook --
//
//	This function is invoked by NotebookEventProc to free the internal
//	structure of a notebook.
//
// Results:
//	None.
//
// Side effects:
//	Everything associated with the notebook is freed up.
//
//----------------------------------------------------------------------
static void
DestroyNotebook(Notebook* nb)		// Info about notebook widget
{
	int i;

	// First mark the widget as in the process of being deleted, so that any
	// code that causes calls to other notebook functions will abort.

	nb->flags |= WIDGET_DELETED;

	// Cancel idle callbacks for redrawing the widget and for rearranging the panes.

	if (nb->flags & REDRAW_PENDING)
		Tcl_CancelIdleCall(DisplayNotebook, (ClientData)nb);

	if (nb->flags & RESIZE_PENDING)
		Tcl_CancelIdleCall(ArrangePane, (ClientData)nb);

	// Clean up the slave list; foreach slave:
	//  o  Cancel the slave's structure notification callback
	//  o  Cancel geometry management for the slave.
	//  o  Free memory for the slave

	for (i = 0; i < nb->numSlaves; i++)
	{
		Tk_DeleteEventHandler(	nb->slaves[i]->tkwin,
										StructureNotifyMask,
										SlaveStructureProc,
										(ClientData)nb->slaves[i]);
		Tk_ManageGeometry(nb->slaves[i]->tkwin, nullptr, nullptr);
		Tk_FreeConfigOptions((char*)nb->slaves[i], nb->paneOptions, nb->tkwin);
		ckfree((char*)nb->slaves[i]);
		nb->slaves[i] = nullptr;
	}

	if (nb->slaves)
		ckfree((char*)nb->slaves);

	// Remove the widget command from the interpreter.
	Tcl_DeleteCommandFromToken(nb->interp, nb->widgetCmd);

	// Let Tk_FreeConfigOptions clean up the rest.

	Tk_FreeConfigOptions((char*)nb, nb->optionTable, nb->tkwin);
	Tcl_Release((ClientData)nb->tkwin);
	nb->tkwin = nullptr;
	Tcl_EventuallyFree((ClientData)nb, TCL_DYNAMIC);
}


//--------------------------------------------------------------
//
// NotebookEventProc --
//
//	This function is invoked by the Tk dispatcher for various events on
//	notebooks.
//
// Results:
//	None.
//
// Side effects:
//	When the window gets deleted, internal structures get cleaned up. When
//	it gets exposed, it is redisplayed.
//
//--------------------------------------------------------------
static void
NotebookEventProc(ClientData clientData,	// Information about window
  						XEvent* eventPtr)			// Information about event
{
	Notebook* nb = (Notebook*)clientData;

	switch (eventPtr->type)
	{
		case ConfigureNotify:
			nb->flags |= REQUESTED_RELAYOUT;
			// fallthru

		case Expose:
			if (nb->tkwin != nullptr && !(nb->flags & REDRAW_PENDING))
			{
				Tcl_DoWhenIdle(DisplayNotebook, (ClientData)nb);
				nb->flags |= REDRAW_PENDING;
			}
			break;

		case DestroyNotify:
			DestroyNotebook(nb);
			break;

		case MapNotify:
			RaiseSlave(nb, 0);
			PlaceSlave(nb, 0);
			break;

		case UnmapNotify:
			UnmapSlave(nb, 0);
			break;
	}
}


//----------------------------------------------------------------------
//
// NotebookCmdDeletedProc --
//
//	This function is invoked when a widget command is deleted. If the
//	widget isn't already in the process of being destroyed, this command
//	destroys it.
//
// Results:
//	None.
//
// Side effects:
//	The widget is destroyed.
//
//----------------------------------------------------------------------
static void
NotebookCmdDeletedProc(ClientData clientData)	// Pointer to widget record for widget
{
	Notebook* nb = (Notebook*)clientData;

	// This function could be invoked either because the window was destroyed
	// and the command was then deleted or because the command was deleted,
	// and then this function destroys the widget. The WIDGET_DELETED flag
	// distinguishes these cases.

	if (!(nb->flags & WIDGET_DELETED))
		Tk_DestroyWindow(nb->tkwin);
}


//----------------------------------------------------------------------
//
// DestroyOptionTables --
//
//	This function is registered as an exit callback when the notebook
//	command is first called. It cleans up the OptionTables structure
//	allocated by that command.
//
// Results:
//	None.
//
// Side effects:
//	Frees memory.
//
//----------------------------------------------------------------------
static void
DestroyOptionTables(	ClientData clientData,	// Pointer to the OptionTables struct
							Tcl_Interp* interp)		// Pointer to the calling interp
{
	ckfree((char*)clientData);
}


//--------------------------------------------------------------
//
// NotebookReqProc --
//
//	This function is invoked by Tk_GeometryRequest for windows managed by
//	a notebook.
//
// Results:
//	None.
//
// Side effects:
//	Arranges for tkwin, and all its managed siblings, to be re-arranged at
//	the next idle point.
//
//--------------------------------------------------------------
static void
NotebookReqProc(	ClientData clientData,	// Notebook's information about window
				 										// that got new preferred geometry
						Tk_Window tkwin)			// Other Tk-related information about the window
{
	Slave*		slave	= (Slave*)clientData;
	Notebook*	nb		= (Notebook*)slave->master;

	if (Tk_IsMapped(nb->tkwin))
	{
		if (!(nb->flags & RESIZE_PENDING))
		{
			nb->flags |= RESIZE_PENDING;
			Tcl_DoWhenIdle(ArrangePane, (ClientData)nb);
		}
	}
	else
	{
		ComputeGeometry(nb);
	}
}


//--------------------------------------------------------------
//
// NotebookLostSlaveProc --
//
//	This function is invoked by Tk whenever some other geometry claims
//	control over a slave that used to be managed by us.
//
// Results:
//	None.
//
// Side effects:
//	Forgets all information about the slave. Causes geometry to be
//	recomputed for the notebook.
//
//--------------------------------------------------------------
static void
NotebookLostSlaveProc(	ClientData clientData,	// Grid structure for slave window that was
				 												// stolen away
								Tk_Window tkwin)			// Tk's handle for the slave window
{
    Slave*		slave	= (Slave*)clientData;
    Notebook*	nb		= (Notebook*)slave->master;

    if (nb->tkwin != Tk_Parent(slave->tkwin))
		Tk_UnmaintainGeometry(slave->tkwin, nb->tkwin);

    Unlink(slave);
    Tk_DeleteEventHandler(	slave->tkwin,
	 								StructureNotifyMask,
									SlaveStructureProc,
									(ClientData)slave);
    Tk_UnmapWindow(slave->tkwin);
    slave->tkwin = nullptr;
    ckfree((char*)slave);
    ComputeGeometry(nb);
}


//--------------------------------------------------------------
//
// Tk_NotebookObjCmd --
//
//	This function is invoked to process the "notebook" Tcl command. It
//	creates a new "notebook" widget.
//
// Results:
//	A standard Tcl result.
//
// Side effects:
//	A new widget is created and configured.
//
//--------------------------------------------------------------
int
Tk_NotebookObjCmd(ClientData clientData,	// nullptr
    					Tcl_Interp* interp,		// Current interpreter
    					int objc,					// Number of arguments
    					Tcl_Obj* const objv[])	// Argument objects
{
	Notebook*		nb;
	Tk_Window		tkwin;
	OptionTables*	nbOptions;

	if (objc < 2)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "pathName ?options?");
		return TCL_ERROR;
	}

	tkwin = Tk_CreateWindowFromPath(interp, Tk_MainWindow(interp), Tcl_GetString(objv[1]), nullptr);
	if (tkwin == nullptr)
		return TCL_ERROR;

	nbOptions = (OptionTables*)Tcl_GetAssocData(interp, "NotebookOptionTables", nullptr);
	if (nbOptions == nullptr)
	{
		// The first time this function is invoked, the option tables will be
		// nullptr. We then create the option tables from the templates and store
		// a pointer to the tables as the command's clinical so we'll have
		// easy access to it in the future.

		nbOptions = (OptionTables*)ckalloc(sizeof(OptionTables));

		// Set up an exit handler to free the optionTables struct.
		Tcl_SetAssocData(interp, "NotebookOptionTables", DestroyOptionTables, (ClientData)nbOptions);

		// Create the notebook option tables.
		nbOptions->nbOptions = Tk_CreateOptionTable(interp, optionSpecs);
		nbOptions->paneOptions = Tk_CreateOptionTable(interp, paneOptionSpecs);
	}

	Tk_SetClass(tkwin, "Notebook");

	// Allocate and initialize the widget record.
	nb = (Notebook*)ckalloc(sizeof(Notebook));
	memset((void*)nb, 0, sizeof(Notebook));
	nb->tkwin = tkwin;
	nb->interp = interp;
	nb->widgetCmd = Tcl_CreateObjCommand(
								interp,
								Tk_PathName(nb->tkwin),
								NotebookWidgetObjCmd,
								(ClientData)nb,
								NotebookCmdDeletedProc);
	nb->optionTable = nbOptions->nbOptions;
	nb->paneOptions = nbOptions->paneOptions;
	nb->relief = TK_RELIEF_RAISED;
	nb->gc = None;
	nb->cursor = None;
	nb->overlay = 0;

	// Keep a hold of the associated tkwin until we destroy the widget,
	// otherwise Tk might free it while we still need it.

	Tcl_Preserve((ClientData)nb->tkwin);

	if (Tk_InitOptions(interp, (char*)nb, nbOptions->nbOptions, tkwin) != TCL_OK)
	{
		Tk_DestroyWindow(nb->tkwin);
		return TCL_ERROR;
	}

	Tk_CreateEventHandler(	nb->tkwin,
									ExposureMask|StructureNotifyMask,
									NotebookEventProc,
									(ClientData)nb);

	if (ConfigureNotebook(interp, nb, objc - 2, objv + 2) != TCL_OK)
	{
		Tk_DestroyWindow(nb->tkwin);
		return TCL_ERROR;
	}

	Tcl_SetStringObj(Tcl_GetObjResult(interp), Tk_PathName(nb->tkwin), -1);
	return TCL_OK;
}


//----------------------------------------------------------------------
//
// Initialization
//
//----------------------------------------------------------------------
void
tk::notebook_init(Tcl_Interp *interp)
{
	Tcl_PkgProvide(interp, "tknotebook", "1.0");
	::tcl::createCommand(interp, "::tk::notebook", Tk_NotebookObjCmd);
}

// vi:set ts=3 sw=3:
