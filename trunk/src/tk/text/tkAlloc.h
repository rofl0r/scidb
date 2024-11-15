/*
 * tkAlloc.h --
 *
 *	This module provides an interface to memory allocation functions, this
 *	is: malloc(), realloc(), free(). This has the following advantages:
 *
 *	1. The whole features of the very valuable tool Valgrind can be used,
 *	   this requires to bypass the Tcl allocation functions.
 *
 *	2. Backport to version 8.5, this is important because the Mac version
 *	   of wish8.6 is quite unstable.
 *
 * Copyright (c) 2015-2017 Gregor Cramer
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

#ifndef _TK_ALLOC
#define _TK_ALLOC

#ifndef _TK
# include "tk.h"
#endif

#if TK_VALGRIND

#include <stdlib.h>

/* enables compiler check that these functions will not be used */
# undef ckalloc
# undef ckrealloc
# undef ckfree

#else /* if !TK_VALGRIND */

/* the main reason for these definitions is portability to 8.5 */
# define malloc(size)		((void *) ckalloc(size))
# define realloc(ptr, size)	((void *) ckrealloc((char *) (ptr), size))
# define free(ptr)		ckfree((char *) (ptr))

#endif /* TK_VALGRIND */

#endif /* _TK_ALLOC */
/* vi:set ts=8 sw=4: */
