/*
 * tkSet.h --
 *
 *	This module implements an integer set.
 *
 * Copyright (c) 2015-2016 Gregor Cramer
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

#ifndef _TKINTSET
#define _TKINTSET

#ifndef _TK
#include "tk.h"
#endif

#include <stdint.h>

#if !defined(TK_BOOL_IS_DEFINED) && !defined(__cplusplus)
typedef int bool;
enum { true = (int) 1, false = (int) 0 };
# define TK_BOOL_IS_DEFINED
#endif

#if defined(__GNUC__) || defined(__clang__)
# define __inline__ extern inline
# define __warn_unused__ __attribute__((warn_unused_result))
#else
# define __inline__
# define __warn_unused__
#endif

struct TkBitField;


typedef uint32_t TkIntSetType;

/*
 * The struct below will be shared with the struct TkBitField, so the first two
 * members must exactly match the first two members in struct TkBitField. In this
 * way we have a struct inheritance, based on the first two members. This
 * is portable due to C99 section 6.7.2.1 bullet point 13:
 *
 *	Within a structure object, the non-bit-field members and the units 
 *	in which bit-fields reside have addresses that increase in the order
 *	in which they are declared. A pointer to a structure object, suitably
 *	converted, points to its initial member (or if that member is a
 *	bit-field, then to the unit in which it resides), and vice versa.
 *	There may be unnamed padding within a structure object, but not at
 *	beginning.
 *
 * This inheritance concept is also used in the portable GTK library.
 */

typedef struct TkIntSet {
    uint32_t refCount:31;
    uint32_t isSetFlag:1;
    TkIntSetType *end;
    TkIntSetType *curr; /* mutable */
    TkIntSetType buf[1];
} TkIntSet;


#define TK_SET_NPOS ((unsigned) -1)


TkIntSet *TkIntSetNew();
TkIntSet *TkIntSetFromBits(const struct TkBitField *bf);
void TkIntSetDestroy(TkIntSet **setPtr);

__inline__ unsigned TkIntSetByteSize(const TkIntSet *set);
__inline__ const unsigned char *TkIntSetData(const TkIntSet *set);

TkIntSet *TkIntSetCopy(const TkIntSet *set) __warn_unused__;

TkIntSet *TkIntSetJoin(TkIntSet *dst, const TkIntSet *src) __warn_unused__;
TkIntSet *TkIntSetIntersect(TkIntSet *dst, const TkIntSet *src) __warn_unused__;
TkIntSet *TkIntSetRemove(TkIntSet *dst, const TkIntSet *src) __warn_unused__;

TkIntSet *TkIntSetJoinBits(TkIntSet *dst, const struct TkBitField *src) __warn_unused__;
TkIntSet *TkIntSetIntersectBits(TkIntSet *dst, const struct TkBitField *src) __warn_unused__;
TkIntSet *TkIntSetRemoveBits(TkIntSet *dst, const struct TkBitField *src) __warn_unused__;

/* dst := dst + set1 + set2 */
TkIntSet *TkIntSetJoin2(TkIntSet *dst, const TkIntSet *set1, const TkIntSet *set2) __warn_unused__;
/* dst := src - dst */
TkIntSet *TkIntSetComplementTo(TkIntSet *dst, const TkIntSet *src) __warn_unused__;
/* dst := dst + (set2 - set1) */
TkIntSet* TkIntSetJoinComplementTo(TkIntSet *dst, const TkIntSet *set1, const TkIntSet *set2)
    __warn_unused__;
/* dst := dst + (set1 - set2) + (set2 - set1) */
TkIntSet *TkIntSetJoinNonIntersection(TkIntSet *dst, const TkIntSet *set1, const TkIntSet *set2)
    __warn_unused__;
/* dst := dst + add + ((set1 + set2) - (set1 & set2)) */
TkIntSet *TkIntSetJoin2ComplementToIntersection(TkIntSet *dst,
    const TkIntSet *add, const TkIntSet *set1, const TkIntSet *set2) __warn_unused__;
/* dst := (dst - set1) + (set1 - set2) */
TkIntSet *TkIntSetJoinOfDifferences(TkIntSet *dst, const TkIntSet *set1, const TkIntSet *set2)
    __warn_unused__;

/* dst := src - dst */
TkIntSet *TkIntSetComplementToBits(TkIntSet *dst, const struct TkBitField *src) __warn_unused__;

__inline__ bool TkIntSetIsEmpty(const TkIntSet *set);
__inline__ unsigned TkIntSetSize(const TkIntSet *set);
__inline__ unsigned TkIntSetMax(const TkIntSet *set);

__inline__ unsigned TkIntSetRefCount(const TkIntSet *set);
__inline__ void TkIntSetIncrRefCount(TkIntSet *set);
__inline__ unsigned TkIntSetDecrRefCount(TkIntSet *set);

__inline__ TkIntSetType TkIntSetAccess(const TkIntSet *set, unsigned index);

__inline__ bool TkIntSetTest(const TkIntSet *set, unsigned n);
__inline__ bool TkIntSetNone(const TkIntSet *set);
__inline__ bool TkIntSetAny(const TkIntSet *set);

__inline__ bool TkIntSetIsEqual(const TkIntSet *set1, const TkIntSet *set2);
__inline__ bool TkIntSetContains(const TkIntSet *set1, const TkIntSet *set2);
__inline__ bool TkIntSetDisjunctive(const TkIntSet *set1, const TkIntSet *set2);
__inline__ bool TkIntSetIntersects(const TkIntSet *set1, const TkIntSet *set2);
bool TkIntSetIntersectionIsEqual(const TkIntSet *set1, const TkIntSet *set2,
    const struct TkBitField *del);

bool TkIntSetIsEqualBits(const TkIntSet *set, const struct TkBitField *bf);
bool TkIntSetContainsBits(const TkIntSet *set, const struct TkBitField *bf);
bool TkIntSetDisjunctiveBits(const TkIntSet *set, const struct TkBitField *bf);
bool TkIntSetIntersectionIsEqualBits(const TkIntSet *set, const struct TkBitField *bf,
    const struct TkBitField *del);

__inline__ unsigned TkIntSetFindFirst(const TkIntSet *set);
__inline__ unsigned TkIntSetFindNext(const TkIntSet *set);

unsigned TkIntSetFindFirstInIntersection(const TkIntSet *set, const struct TkBitField *bf);

TkIntSet *TkIntSetAdd(TkIntSet *set, unsigned n) __warn_unused__;
TkIntSet *TkIntSetErase(TkIntSet *set, unsigned n) __warn_unused__;
TkIntSet *TkIntSetTestAndSet(TkIntSet *set, unsigned n) __warn_unused__;
TkIntSet *TkIntSetTestAndUnset(TkIntSet *set, unsigned n) __warn_unused__;
__inline__ TkIntSet *TkIntSetAddOrErase(TkIntSet *set, unsigned n, bool add) __warn_unused__;
TkIntSet* TkIntSetClear(TkIntSet *set) __warn_unused__;

#if !NDEBUG
void TkIntSetPrint(const TkIntSet *set);
#endif


#if TK_TEXT_LINE_TAGGING

/*
 * These functions are not yet needed, but shouldn't be removed, because they will
 * be important if the text widget is supporting line based tagging (currently line
 * based tagging is not supported by the display functions).
 */

/* dst := (dst + (set - sub)) & set */
TkIntSet *TkIntSetInnerJoinDifference(TkIntSet *dst, const TkIntSet *set, const TkIntSet *sub)
    __warn_unused__;
/* ((set + (add - sub)) & add) == nil */
bool TkIntSetInnerJoinDifferenceIsEmpty(const TkIntSet *set, const TkIntSet *add, const TkIntSet *sub);
/* set1 == set2 - sub2 */
bool TkIntSetIsEqualToDifference(const TkIntSet *set1, const TkIntSet *set2, const TkIntSet *sub2);
/* set1 == set2 + (add2 & set2) */
bool TkIntSetIsEqualToInnerJoin(const TkIntSet *set1, const TkIntSet *set2, const TkIntSet *add2);
/* set1 == ((set2 + (add2 - sub2)) & add2) */
bool TkIntSetIsEqualToInnerJoinDifference(const TkIntSet *set1, const TkIntSet *set2,
    const TkIntSet *add2, const TkIntSet *sub2);
/* ((set1 + (add - sub)) & add) == ((set2 + (add - sub)) & add) */
bool TkIntSetInnerJoinDifferenceIsEqual(const TkIntSet *set1, const TkIntSet *set2,
    const TkIntSet *add, const TkIntSet *sub);

#endif /* TK_TEXT_LINE_TAGGING */


#if defined(__GNUC__) || defined(__clang__)
# include "tkIntSetPriv.h"
#endif

#undef __warn_unused__
#undef __inline__
#endif /* _TKINTSET */
/* vi:set ts=8 sw=4: */
