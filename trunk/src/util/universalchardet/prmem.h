/* ======================================================================
 * Author : $Author$
 * Version: $Revision: 102 $
 * Date   : $Date: 2011-11-10 14:04:49 +0000 (Thu, 10 Nov 2011) $
 * Url    : $URL$
 * ====================================================================== */
/* 
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is the Netscape Portable Runtime (NSPR).
 * 
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are 
 * Copyright (C) 1998-2000 Netscape Communications Corporation.  All
 * Rights Reserved.
 * 
 * Contributor(s):
 * 
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU General Public License Version 2 or later (the
 * "GPL"), in which case the provisions of the GPL are applicable 
 * instead of those above.  If you wish to allow use of your 
 * version of this file only under the terms of the GPL and not to
 * allow others to use your version of this file under the MPL,
 * indicate your decision by deleting the provisions above and
 * replace them with the notice and other provisions required by
 * the GPL.  If you do not delete the provisions above, a recipient
 * may use your version of this file under either the MPL or the
 * GPL.
 */

/*
** File: prmem.h
** Description: API to NSPR 2.0 memory management functions
**
*/
#ifndef prmem_h___
#define prmem_h___

#include "prtypes.h"
#include <stddef.h>
#include <stdlib.h>

#if 1

inline static void* PR_Malloc(PRUint32 size)                    { return malloc(size); }
inline static void* PR_Calloc(PRUint32 nelem, PRUint32 elsize)  { return calloc(nelem, elsize); }
inline static void* PR_Realloc(void *ptr, PRUint32 size)        { return realloc(ptr, size); }
inline static void  PR_Free(void *ptr)                          { free(ptr); }

#else

PR_BEGIN_EXTERN_C

/*
** Thread safe memory allocation.
**
** NOTE: pr wraps up malloc, free, calloc, realloc so they are already
** thread safe (and are not declared here - look in stdlib.h).
*/

/*
** PR_Malloc, PR_Calloc, PR_Realloc, and PR_Free have the same signatures
** as their libc equivalent malloc, calloc, realloc, and free, and have
** the same semantics.  (Note that the argument type size_t is replaced
** by PRUint32.)  Memory allocated by PR_Malloc, PR_Calloc, or PR_Realloc
** must be freed by PR_Free.
*/

void *PR_Malloc(PRUint32 size);

void *PR_Calloc(PRUint32 nelem, PRUint32 elsize);

void *PR_Realloc(void *ptr, PRUint32 size);

void PR_Free(void *ptr);

PR_END_EXTERN_C

#endif

/*
** The following are some convenience macros defined in terms of
** PR_Malloc, PR_Calloc, PR_Realloc, and PR_Free.
*/

/***********************************************************************
** FUNCTION:	PR_MALLOC()
** DESCRIPTION:
**   PR_NEW() allocates an untyped item of size _size from the heap.
** INPUTS:  _size: size in bytes of item to be allocated
** OUTPUTS:	untyped pointer to the node allocated
** RETURN:	pointer to node or error returned from malloc().
***********************************************************************/
#define PR_MALLOC(_bytes) (PR_Malloc((_bytes)))

/***********************************************************************
** FUNCTION:	PR_NEW()
** DESCRIPTION:
**   PR_NEW() allocates an item of type _struct from the heap.
** INPUTS:  _struct: a data type
** OUTPUTS:	pointer to _struct
** RETURN:	pointer to _struct or error returns from malloc().
***********************************************************************/
#define PR_NEW(_struct) ((_struct *) PR_MALLOC(sizeof(_struct)))

/***********************************************************************
** FUNCTION:	PR_REALLOC()
** DESCRIPTION:
**   PR_REALLOC() re-allocates _ptr bytes from the heap as a _size
**   untyped item.
** INPUTS:	_ptr: pointer to node to reallocate
**          _size: size of node to allocate
** OUTPUTS:	pointer to node allocated
** RETURN:	pointer to node allocated
***********************************************************************/
#define PR_REALLOC(_ptr, _size) (PR_Realloc((_ptr), (_size)))

/***********************************************************************
** FUNCTION:	PR_CALLOC()
** DESCRIPTION:
**   PR_CALLOC() allocates a _size bytes untyped item from the heap
**   and sets the allocated memory to all 0x00.
** INPUTS:	_size: size of node to allocate
** OUTPUTS:	pointer to node allocated
** RETURN:	pointer to node allocated
***********************************************************************/
#define PR_CALLOC(_size) (PR_Calloc(1, (_size)))

/***********************************************************************
** FUNCTION:	PR_NEWZAP()
** DESCRIPTION:
**   PR_NEWZAP() allocates an item of type _struct from the heap
**   and sets the allocated memory to all 0x00.
** INPUTS:	_struct: a data type
** OUTPUTS:	pointer to _struct
** RETURN:	pointer to _struct
***********************************************************************/
#define PR_NEWZAP(_struct) ((_struct*)PR_Calloc(1, sizeof(_struct)))

/***********************************************************************
** FUNCTION:	PR_DELETE()
** DESCRIPTION:
**   PR_DELETE() unallocates an object previosly allocated via PR_NEW()
**   or PR_NEWZAP() to the heap.
** INPUTS:	pointer to previously allocated object
** OUTPUTS:	the referenced object is returned to the heap
** RETURN:	void
***********************************************************************/
#define PR_DELETE(_ptr) { PR_Free(_ptr); (_ptr) = NULL; }

/***********************************************************************
** FUNCTION:	PR_FREEIF()
** DESCRIPTION:
**   PR_FREEIF() conditionally unallocates an object previously allocated
**   vial PR_NEW() or PR_NEWZAP(). If the pointer to the object is
**   equal to zero (0), the object is not released.
** INPUTS:	pointer to previously allocated object
** OUTPUTS:	the referenced object is conditionally returned to the heap
** RETURN:	void
***********************************************************************/
#define PR_FREEIF(_ptr)	if (_ptr) PR_DELETE(_ptr)

#endif /* prmem_h___ */
/* vi:set ts=4 sw=4 et: */
