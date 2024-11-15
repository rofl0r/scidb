/*
 * tkTextPriv.h --
 *
 *	Private implementation for text widget.
 *
 * Copyright (c) 2015-2016 Gregor Cramer
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

#ifndef _TKTEXT
# error "do not include this private header file"
#endif


#ifndef _TKTEXTPRIV
#define _TKTEXTPRIV

/*
 * The following struct is private for TkTextBTree.c, but we want fast access to
 * the internal content.
 *
 * The data structure below defines an entire B-tree. Since text widgets are
 * the only current B-tree clients, 'clients' and 'numPixelReferences' are
 * identical.
 */

struct TkBTreeNodePixelInfo;

struct TkTextMyBTree {
    struct Node *rootPtr;
				/* Pointer to root of B-tree. */
    unsigned clients;		/* Number of clients of this B-tree. */
    unsigned numPixelReferences;
				/* Number of clients of this B-tree which care about pixel heights. */
    struct TkBTreeNodePixelInfo *pixelInfoBuffer;
    				/* Buffer of size numPixelReferences used for recomputation of pixel
    				 * information. */
    unsigned stateEpoch;	/* Updated each time any aspect of the B-tree changes. */
    TkSharedText *sharedTextPtr;/* Used to find tagTable in consistency checking code, and to access
    				 * list of all B-tree clients. */
};

#endif /* _TKTEXTPRIV */

#ifdef _TK_NEED_IMPLEMENTATION

#include <assert.h>

#if __STDC_VERSION__ < 199901L
# define inline
#endif

/*
 *----------------------------------------------------------------------
 *
 * TkTextIsSpecialMark --
 *
 *	Test whether this is a special mark: "insert", or "current".
 *
 * Results:
 *	Whether this is a special mark.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
bool
TkTextIsSpecialMark(
    const TkTextSegment *segPtr)
{
    assert(segPtr);
    assert(!(segPtr->insertMarkFlag | segPtr->currentMarkFlag)
	    || segPtr->typePtr->group == SEG_GROUP_MARK);

    return !!(segPtr->insertMarkFlag | segPtr->currentMarkFlag);
}

/*
 *----------------------------------------------------------------------
 *
 * TkTextIsPrivateMark --
 *
 *	Test whether this is a private mark, not visible with "inspect"
 *	or "dump". These kind of marks will be used in library/text.tcl.
 *	Furthemore in practice it is guaranteed that this mark has a
 *	unique name.
 *
 * Results:
 *	Whether this is a private mark.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
bool
TkTextIsPrivateMark(
    const TkTextSegment *segPtr)
{
    assert(segPtr);
    assert(!segPtr->privateMarkFlag || segPtr->typePtr->group == SEG_GROUP_MARK);

    return segPtr->privateMarkFlag;
}

/*
 *----------------------------------------------------------------------
 *
 * TkTextIsNormalMark --
 *
 *	Test whether this is a mark, and it is neither special, nor
 *	private, nor a start/end marker.
 *
 * Results:
 *	Whether this is a normal mark.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
bool
TkTextIsNormalMark(
    const TkTextSegment *segPtr)
{
    assert(segPtr);

    return segPtr->typePtr->group == SEG_GROUP_MARK
	    && !(segPtr->startEndMarkFlag
		| segPtr->privateMarkFlag
		| segPtr->insertMarkFlag
		| segPtr->currentMarkFlag);
}

/*
 *----------------------------------------------------------------------
 *
 * TkTextIsStartEndMarker --
 *
 *	Test whether this is a start/end marker. This must not be a mark,
 *	it can also be a break segment.
 *
 * Results:
 *	Whether this is a start/end marker.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
bool
TkTextIsStartEndMarker(
    const TkTextSegment *segPtr)
{
    assert(segPtr);
    return segPtr->startEndMarkFlag;
}

/*
 *----------------------------------------------------------------------
 *
 * TkTextIsStableMark --
 *
 *	Test whether this is a mark, and it is neither special, nor
 *	private. Note that also a break segment is interpreted as
 *	a stable mark.
 *
 * Results:
 *	Whether this is a stable mark.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
bool
TkTextIsStableMark(
    const TkTextSegment *segPtr)
{
    assert(segPtr);
    return TkTextIsStartEndMarker(segPtr) || TkTextIsNormalMark(segPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * TkTextIsSpecialOrPrivateMark --
 *
 *	Test whether this is a special mark, or a private mark.
 *
 * Results:
 *	Whether this is a special or private mark.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
bool
TkTextIsSpecialOrPrivateMark(
    const TkTextSegment *segPtr)
{
    assert(segPtr);

    return segPtr->typePtr->group == SEG_GROUP_MARK
	    && !!(segPtr->privateMarkFlag | segPtr->insertMarkFlag | segPtr->currentMarkFlag);
}

/*
 *----------------------------------------------------------------------
 *
 * TkTextIsNormalOrSpecialMark --
 *
 *	Test whether this is a normal mark, or a special mark.
 *
 * Results:
 *	Whether this is a normal or special mark.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
bool
TkTextIsNormalOrSpecialMark(
    const TkTextSegment *segPtr)
{
    assert(segPtr);

    return segPtr->typePtr->group == SEG_GROUP_MARK
	    && !(segPtr->startEndMarkFlag | segPtr->privateMarkFlag);
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreeLinePixelInfo --
 *
 *	Return widget pixel information for specified line.
 *
 * Results:
 *	The pixel information of this line for specified widget.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
TkTextPixelInfo *
TkBTreeLinePixelInfo(
    const TkText *textPtr,
    TkTextLine *linePtr)
{
    assert(textPtr);
    assert(textPtr->pixelReference >= 0);
    assert(linePtr);

    return linePtr->pixelInfo + textPtr->pixelReference;
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreeGetStartLine --
 *
 *	This function returns the first line for this text widget.
 *
 * Results:
 *	The first line in this widget.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
TkTextLine *
TkBTreeGetStartLine(
    const TkText *textPtr)
{
    assert(textPtr);
    return textPtr->startMarker->sectionPtr->linePtr;
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreeGetLastLine --
 *
 *	This function returns the last line for this text widget.
 *
 * Results:
 *	The last line in this widget.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

MODULE_SCOPE TkTextLine * TkBTreeMyGetLastLine(
    const TkSharedText *sharedTextPtr, const TkText *textPtr);

inline
TkTextLine *
TkBTreeGetLastLine(
    const TkText *textPtr)
{
    assert(textPtr);
    return TkBTreeMyGetLastLine(NULL, textPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreeGetShared --
 *
 *	Get the shared resource for given tree.
 *
 * Results:
 *	The shared resource.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
TkSharedText *
TkBTreeGetShared(
    TkTextBTree tree)		/* Return shared resource of this tree. */
{
    return ((struct TkTextMyBTree *) tree)->sharedTextPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreeIncrEpoch --
 *
 *	Increment the epoch of the tree.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Increment the epoch number.
 *
 *----------------------------------------------------------------------
 */

inline
unsigned
TkBTreeIncrEpoch(
    TkTextBTree tree)		/* Tree to increment epoch. */
{
    return ((struct TkTextMyBTree *) tree)->stateEpoch += 1;
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreeEpoch --
 *
 *	Return the epoch for the B-tree. This number is incremented any time
 *	anything changes in the tree.
 *
 * Results:
 *	The epoch number.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
unsigned
TkBTreeEpoch(
    TkTextBTree tree)		/* Tree to get epoch for. */
{
    return ((struct TkTextMyBTree *) tree)->stateEpoch;
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreeGetRoot --
 *
 *	Return the root node of the B-Tree.
 *
 * Results:
 *	The root node of the B-Tree.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
struct Node *
TkBTreeGetRoot(
    TkTextBTree tree)		/* Tree to get root node for. */
{
    return ((struct TkTextMyBTree *) tree)->rootPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreeNextLine --
 *
 *	Given an existing line in a B-tree, this function locates the next
 *	line in the B-tree, regarding the end line of this widget.
 *	B-tree.
 *
 * Results:
 *	The return value is a pointer to the line that immediately follows
 *	linePtr, or NULL if there is no such line.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
TkTextLine *
TkBTreeNextLine(
    const TkText *textPtr,	/* Next line in the context of this client, can be NULL. */
    TkTextLine *linePtr)	/* Pointer to existing line in B-tree. */
{
    return textPtr && linePtr == TkBTreeGetLastLine(textPtr) ? NULL : linePtr->nextPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreePrevLine --
 *
 *	Given an existing line in a B-tree, this function locates the previous
 *	line in the B-tree, regarding the start line of this widget.
 *
 * Results:
 *	The return value is a pointer to the line that immediately preceeds
 *	linePtr, or NULL if there is no such line.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
TkTextLine *
TkBTreePrevLine(
    const TkText *textPtr,	/* Relative to this client of the B-tree, can be NULL. */
    TkTextLine *linePtr)	/* Pointer to existing line in B-tree. */
{
    return textPtr && linePtr == TkBTreeGetStartLine(textPtr) ? NULL : linePtr->prevPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreePrevLogicalLine --
 *
 *	Given a line, this function is searching for the previous logical line,
 *	which don't has a predecessing line with elided newline. If the search
 *	reaches the start of the text, then the first line will be returned,
 *	even if it's not a logical line (the latter can only happen in peers
 *	with restricted ranges).
 *
 * Results:
 *	The return value is the previous logical line, in most cases this
 *	will be simply the previous line.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
TkTextLine *
TkBTreePrevLogicalLine(
    const TkSharedText* sharedTextPtr,
    const TkText *textPtr,	/* can be NULL */
    TkTextLine *linePtr)
{
    assert(linePtr);
    assert(linePtr != (textPtr ?
	    TkBTreeGetStartLine(textPtr) : sharedTextPtr->startMarker->sectionPtr->linePtr));

    return TkBTreeGetLogicalLine(sharedTextPtr, textPtr, linePtr->prevPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreeCountLines --
 *
 *	This function counts the number of lines inside a given range.
 *
 * Results:
 *	The return value is the number of lines inside a given range.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
unsigned
TkBTreeCountLines(
    const TkTextBTree tree,
    const TkTextLine *linePtr1,	/* Start counting at this line. */
    const TkTextLine *linePtr2)	/* Stop counting at this line (don't count this line). */
{
    assert(TkBTreeLinesTo(tree, NULL, linePtr1, NULL) <= TkBTreeLinesTo(tree, NULL, linePtr2, NULL));

    if (linePtr1 == linePtr2) {
	return 0; /* this is catching a frequent case */
    }
    if (linePtr1->nextPtr == linePtr2) {
	return 1; /* this is catching a frequent case */
    }

    return TkBTreeLinesTo(tree, NULL, linePtr2, NULL) - TkBTreeLinesTo(tree, NULL, linePtr1, NULL);
}

/*
 *----------------------------------------------------------------------
 *
 * TkTextIndexGetShared --
 *
 *	Get the shared resource of this index.
 *
 * Results:
 *	The shared resource.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
TkSharedText *
TkTextIndexGetShared(
    const TkTextIndex *indexPtr)
{
    assert(indexPtr);
    assert(indexPtr->tree);
    return TkBTreeGetShared(indexPtr->tree);
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreeGetTags --
 *
 *	Return information about all of the tags that are associated with a
 *	particular character in a B-tree of text.
 *
 * Results:
 *      The return value is the root of the tag chain, containing all tags
 *	associated with the character at the position given by linePtr and ch.
 *	If there are no tags at the given character then a NULL pointer is
 *	returned.
 *
 * Side effects:
 *	The attribute nextPtr of TkTextTag will be modified for any tag.
 *
 *----------------------------------------------------------------------
 */

inline
TkTextTag *
TkBTreeGetTags(
    const TkTextIndex *indexPtr)/* Indicates a particular position in the B-tree. */
{
    TkTextSegment *segPtr = TkTextIndexGetContentSegment(indexPtr, NULL);
    return TkBTreeGetSegmentTags(TkTextIndexGetShared(indexPtr), segPtr, indexPtr->textPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * TkTextIndexGetLine --
 *
 *	Get the line pointer of this index.
 *
 * Results:
 *	The line pointer.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
TkTextLine *
TkTextIndexGetLine(
    const TkTextIndex *indexPtr)
{
    assert(indexPtr->priv.linePtr);
    assert(indexPtr->priv.linePtr->parentPtr); /* expired? */

    return indexPtr->priv.linePtr;
}

/*
 *----------------------------------------------------------------------
 *
 * TkTextIndexSameLines --
 *
 *	Test whether both given indicies are referring the same line.
 *
 * Results:
 *	Return true if both indices are referring the same line, otherwise
 *	false will be returned.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
bool
TkTextIndexSameLines(
    const TkTextIndex *indexPtr1,	/* Pointer to index. */
    const TkTextIndex *indexPtr2)	/* Pointer to index. */
{
    assert(indexPtr1->priv.linePtr);
    assert(indexPtr2->priv.linePtr);
    assert(indexPtr1->priv.linePtr->parentPtr); /* expired? */
    assert(indexPtr2->priv.linePtr->parentPtr); /* expired? */

    return indexPtr1->priv.linePtr == indexPtr2->priv.linePtr;
}

/*
 *----------------------------------------------------------------------
 *
 * TkTextIndexSetEpoch --
 *
 *	Update epoch of given index, don't clear the segment pointer.
 *	Use this function with care, it must be ensured that the
 *	segment pointer is still valid.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
void
TkTextIndexUpdateEpoch(
    TkTextIndex *indexPtr,
    unsigned epoch)
{
    assert(indexPtr->priv.linePtr);
    assert(indexPtr->priv.linePtr->parentPtr); /* expired? */

    indexPtr->stateEpoch = epoch;
    indexPtr->priv.lineNo = -1;
}

/*
 *----------------------------------------------------------------------
 *
 * TkTextIndexSetEpoch --
 *
 *	Set epoch of given index, and clear the segment pointer if
 *	the new epoch is different from last epoch.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
void
TkTextIndexSetEpoch(
    TkTextIndex *indexPtr,
    unsigned epoch)
{
    assert(indexPtr->priv.linePtr);
    assert(indexPtr->priv.linePtr->parentPtr); /* expired? */

    if (indexPtr->stateEpoch != epoch) {
	indexPtr->stateEpoch = epoch;
	indexPtr->priv.segPtr = NULL;
	indexPtr->priv.lineNo = -1;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * TkBTreeGetNumberOfDisplayLines --
 *
 *	Return the current number of display lines. This is the number
 *	of lines known by the B-Tree (not the number of lines known
 *	by the display stuff).
 *
 *	We are putting the implementation into this private header file,
 *	because it uses some facts only known by the display stuff.
 *
 * Results:
 *	Returns the current number of display lines (known by B-Tree).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

inline
int
TkBTreeGetNumberOfDisplayLines(
    const TkTextPixelInfo *pixelInfo)
{
    const TkTextDispLineInfo *dispLineInfo;

    if (pixelInfo->height == 0) {
	return 0;
    }
    if (!(dispLineInfo = pixelInfo->dispLineInfo)) {
	return 1;
    }
    if (pixelInfo->epoch & 0x80000000) {
	/*
	 * This will return the old number of display lines, because the
	 * computation of the corresponding logical line is currently in
	 * progress, and unfinshed.
	 */
	return dispLineInfo->entry[dispLineInfo->numDispLines].pixels;
    }
    return dispLineInfo->numDispLines;
}

#undef _TK_NEED_IMPLEMENTATION
#endif /* _TK_NEED_IMPLEMENTATION */
/* vi:set ts=8 sw=4: */
