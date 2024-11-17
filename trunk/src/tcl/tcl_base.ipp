// ======================================================================
// Author : $Author$
// Version: $Revision: 1507 $
// Date   : $Date: 2018-08-13 12:17:53 +0000 (Mon, 13 Aug 2018) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2018 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_carray.h"
#include "m_string.h"
#include "m_assert.h"

#include <string.h>

namespace tcl { namespace bits { extern Tcl_Interp* interp; } }

inline Tcl_Interp* tcl::interp() { return bits::interp; }


inline
Tcl_Obj*
tcl::incrRef(Tcl_Obj* obj)
{
	if (obj)
		Tcl_IncrRefCount(obj);
	return obj;
}


inline
void
tcl::decrRef(Tcl_Obj* obj)
{
	if (obj)
	{
#ifndef NDEBUG
		// Tcl does not provide any function for querying the reference count.
		// So we need a work-around.
		Tcl_IncrRefCount(obj);
		M_REQUIRE(Tcl_IsShared(obj));
		Tcl_DecrRefCount(obj);
#endif
		Tcl_DecrRefCount(obj);
	}
}


inline Tcl_Obj* tcl::newObj() { return Tcl_NewObj(); }


inline
Tcl_Obj*
tcl::newObj(char const* s)
{
	M_REQUIRE(s);
	return Tcl_NewStringObj(s, -1);
}


inline
Tcl_Obj*
tcl::newObj(char const* s, unsigned len)
{
	M_REQUIRE(s);
	return Tcl_NewStringObj(s, len);
}


inline
Tcl_Obj*
tcl::newObj(mstl::string const& s)
{
	M_REQUIRE(s);
	return Tcl_NewStringObj(s.c_str(), s.size());
}


inline
Tcl_Obj*
tcl::newObj(unsigned objc, Tcl_Obj* const objv[])
{
	M_REQUIRE(objc == 0 || objv);
	return Tcl_NewListObj(objc, objv);
}


inline Tcl_Obj* tcl::newObj(int value) { return Tcl_NewIntObj(value); }
inline Tcl_Obj* tcl::newObj(unsigned value) { return Tcl_NewIntObj(value); }
inline Tcl_Obj* tcl::newObj(int64_t value) { return Tcl_NewWideIntObj(value); }
template <int N> inline Tcl_Obj* tcl::newObj(Tcl_Obj* (&objv)[N]) { return newObj(N, objv); }
inline Tcl_Obj* tcl::newObj(Array const& list) { return newObj(list.size(), list.data()); }
inline Tcl_Obj* tcl::newListObj(mstl::string const& s) { return newListObj(s.c_str(), s.size()); }


inline
char const*
tcl::asString(Tcl_Obj* obj)
{
	M_REQUIRE(obj);
	return Tcl_GetString(obj);
}


inline
char const*
tcl::asString(Tcl_Obj* obj, int& len)
{
	M_REQUIRE(obj);
	return Tcl_GetStringFromObj(obj, &len);
}


inline
bool
tcl::equal(char const* lhs, char const* rhs)
{
	M_ASSERT(lhs);
	M_ASSERT(rhs);

	return ::strcmp(lhs, rhs) == 0;
}


inline bool tcl::equal(char const* lhs, Tcl_Obj* rhs) { return equal(lhs, tcl::asString(rhs)); }
inline bool tcl::equal(Tcl_Obj* lhs, char const* rhs) { return equal(tcl::asString(lhs), rhs); }

inline bool tcl::equal(Tcl_Obj* lhs, Tcl_Obj* rhs)
{ return equal(tcl::asString(lhs), tcl::asString(rhs)); }


inline
bool
tcl::equal(char const* lhs, char const* rhs, unsigned n)
{
	M_ASSERT(lhs);
	M_ASSERT(rhs);

	return ::strncmp(lhs, rhs, n) == 0;
}


inline
bool
tcl::equal(char const* lhs, Tcl_Obj* rhs, unsigned n)
{
	return equal(lhs, tcl::asString(rhs), n);
}


inline
bool
tcl::equal(Tcl_Obj* lhs, char const* rhs, unsigned n)
{
	return equal(tcl::asString(lhs), rhs, n);
}


inline
bool
tcl::equal(Tcl_Obj* lhs, Tcl_Obj* rhs, unsigned n)
{
	return equal(tcl::asString(lhs), tcl::asString(rhs), n);
}


inline
bool
tcl::eqOrNull(Tcl_Obj* lhs, Tcl_Obj* rhs)
{
	return !lhs ? !rhs : rhs && (tcl::equal(lhs, rhs));
}


inline
bool
tcl::eqOrNull(Tcl_Obj* lhs, Tcl_Obj* rhs, unsigned n)
{
	return !lhs ? !rhs : rhs && (tcl::equal(lhs, rhs, n));
}


inline
unsigned
tcl::countElements(Tcl_Obj* obj)
{
	int count = 0;
	if (obj)
		Tcl_ListObjLength(nullptr, obj, &count);
	return count;
}


inline
mstl::carray<Tcl_Obj*>
tcl::getElements(Tcl_Obj* obj)
{
	M_ASSERT(obj);
	Tcl_Obj** objv;
	size_t n = getElements(obj, objv);
	return mstl::carray<Tcl_Obj*>(objv, n);
}


template <int N>
inline
Tcl_Obj*
tcl::addElement(Tcl_Obj*& list, Tcl_Obj* (&objv)[N])
{
	return addElement(list, newObj(N, objv));
}


template <int N>
inline
Tcl_Obj*
tcl::insertElement(Tcl_Obj*& list, Tcl_Obj* (&objv)[N], unsigned position)
{
	return addElement(list, newObj(N, objv), position);
}


inline
bool
tcl::containsElement(Tcl_Obj* obj, mstl::string const& what)
{
	return findElement(obj, what) >= 0;
}


inline
bool
tcl::containsElement(Tcl_Obj* obj, char const* what)
{
	return findElement(obj, what) >= 0;
}


inline
bool
tcl::containsElement(Tcl_Obj* obj, Tcl_Obj* what)
{
	return findElement(obj, what) >= 0;
}


inline
bool
tcl::containsElement(List const& list, mstl::string const& what)
{
	return findElement(list, what) >= 0;
}


inline
bool
tcl::containsElement(List const& list, char const* what)
{
	return findElement(list, what) >= 0;
}


inline
bool
tcl::containsElement(List const& list, Tcl_Obj* what)
{
	return findElement(list, what) >= 0;
}


inline
bool
tcl::containsElement(Array const& arr, mstl::string const& what)
{
	return findElement(arr, what) >= 0;
}


inline
bool
tcl::containsElement(Array const& arr, char const* what)
{
	return findElement(arr, what) >= 0;
}


inline
bool
tcl::containsElement(Array const& arr, Tcl_Obj* what)
{
	return findElement(arr, what) >= 0;
}


inline
void
tcl::set(Tcl_Obj*& obj, Tcl_Obj* value)
{
	tcl::decrRef(obj);
	obj = tcl::incrRef(value);
}


inline
void
tcl::zero(Tcl_Obj*& obj)
{
	tcl::decrRef(obj);
	obj = nullptr;
}


inline
int
tcl::wrongNumArgs(int objc, Tcl_Obj* const objv[], char const* args)
{
	Tcl_WrongNumArgs(interp(), objc, objv, args);
	return TCL_ERROR;
}


template <int N> inline void tcl::setResult(Tcl_Obj* (&objv)[N]) { setResult(N, objv); }


inline
int
tcl::setError(char const* type)
{
	Tcl_SetErrorCode(interp(), "SCIDB", type, nullptr);
	return TCL_ERROR;
}


inline Tcl_Obj* tcl::result() { return Tcl_GetObjResult(interp()); }


namespace tcl {

inline DString::DString()	{ Tcl_DStringInit(&m_str); }
inline DString::~DString()	{ Tcl_DStringFree(&m_str); }

inline DString& DString::startList()	{ Tcl_DStringStartSublist(&m_str); return *this; }
inline DString& DString::endList()		{ Tcl_DStringEndSublist(&m_str); return *this; }


inline
Tcl_Obj*
DString::toObj() const
{
	return newObj(Tcl_DStringValue(&m_str), Tcl_DStringLength(&m_str));
}


inline
DString&
DString::append(char const* str)
{
	M_REQUIRE(str);
	Tcl_DStringAppendElement(&m_str, str);
	return *this;
}


inline
DString& DString::append(Tcl_Obj* obj)
{
	M_REQUIRE(obj);
	return append(asString(obj));
}


inline DString& DString::append(mstl::string const& str) { return append(str.c_str()); }

} // namespace tcl

// vi:set ts=3 sw=3:
