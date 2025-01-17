// ======================================================================
// Author : $Author$
// Version: $Revision: 1507 $
// Date   : $Date: 2018-08-13 12:17:53 +0000 (Mon, 13 Aug 2018) $
// Url    : $URL$
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
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

#ifndef _tcl_base_included
#define _tcl_base_included

#include <stdint.h>
#include <stdarg.h>

#include <tcl.h>

namespace mstl { class string; }
namespace mstl { template <typename T> class vector; }
namespace mstl { template <typename T> class carray; }

namespace tcl {

class DString
{
public:

	DString();
	~DString();

	Tcl_Obj* toObj() const;

	DString& startList();
	DString& endList();

	DString& append(char const* str);
	DString& append(mstl::string const& str);
	DString& append(Tcl_Obj* obj);
	DString& append(int value);
	DString& append(unsigned value);

private:

	Tcl_DString m_str;
};


typedef mstl::carray<Tcl_Obj*> Array;
typedef mstl::vector<Tcl_Obj*> List;

Tcl_Interp* interp();

Tcl_Obj* incrRef(Tcl_Obj* obj);
void decrRef(Tcl_Obj* obj);

Tcl_Obj* newObj();
Tcl_Obj* newObj(char const* s);
Tcl_Obj* newObj(char const* s, unsigned len);
Tcl_Obj* newObj(mstl::string const& s);
Tcl_Obj* newObj(Tcl_Obj* obj1, Tcl_Obj* obj2);
Tcl_Obj* newObj(unsigned objc, Tcl_Obj* const objv[]);
template <int N> Tcl_Obj* newObj(Tcl_Obj* (&objv)[N]);
Tcl_Obj* newObj(List const& list);
Tcl_Obj* newObj(Array const& list);
Tcl_Obj* newObj(int value);
Tcl_Obj* newObj(unsigned value);
Tcl_Obj* newObj(int64_t value);

Tcl_Obj* newListObj(char const* s);
Tcl_Obj* newListObj(char const* s, unsigned len);
Tcl_Obj* newListObj(mstl::string const& s);

bool isInt(Tcl_Obj* obj);
bool isUnsigned(Tcl_Obj* obj);
bool isBoolean(Tcl_Obj* obj);

char const* asString(Tcl_Obj* obj);
char const* asString(Tcl_Obj* obj, int& len);
char const* asString(Tcl_Obj* obj, unsigned& len);
int asInt(Tcl_Obj* obj);
unsigned asUnsigned(Tcl_Obj* obj);
bool asBoolean(Tcl_Obj* obj);

bool equal(char const* lhs, Tcl_Obj* rhs);
bool equal(Tcl_Obj* lhs, char const* rhs);
bool equal(Tcl_Obj* lhs, Tcl_Obj* rhs);
bool equal(char const* lhs, char const* rhs);

bool equal(char const* lhs, Tcl_Obj* rhs, unsigned n);
bool equal(Tcl_Obj* lhs, char const* rhs, unsigned n);
bool equal(Tcl_Obj* lhs, Tcl_Obj* rhs, unsigned n);
bool equal(char const* lhs, char const* rhs, unsigned n);

bool eqOrNull(Tcl_Obj* lhs, Tcl_Obj* rhs);
bool eqOrNull(Tcl_Obj* lhs, Tcl_Obj* rhs, unsigned n);

void set(Tcl_Obj*& obj, Tcl_Obj* value);
void zero(Tcl_Obj*& obj);

Tcl_Obj* getGlobalVar(Tcl_Obj* var);
void setGlobalVar(Tcl_Obj* var, Tcl_Obj* value);

unsigned countElements(Tcl_Obj* obj);
unsigned getElements(Tcl_Obj* obj, Tcl_Obj**& objv);
Array getElements(Tcl_Obj* obj);
Tcl_Obj* addElement(Tcl_Obj*& list, Tcl_Obj* elem);
template <int N> Tcl_Obj* addElement(Tcl_Obj*& list, Tcl_Obj* (&objv)[N]);
Tcl_Obj* insertElement(Tcl_Obj*& list, Tcl_Obj* elem, unsigned position);
template <int N> Tcl_Obj* insertElement(Tcl_Obj*& list, Tcl_Obj* (&objv)[N], unsigned position);
Tcl_Obj* removeElement(Tcl_Obj*& list, unsigned position);

int findElement(Tcl_Obj* obj, mstl::string const& what);
int findElement(Tcl_Obj* obj, char const* what);
int findElement(Tcl_Obj* obj, Tcl_Obj* what);
int findElement(List const& list, mstl::string const& what);
int findElement(List const& list, char const* what);
int findElement(List const& list, Tcl_Obj* what);
int findElement(Array const& list, mstl::string const& what);
int findElement(Array const& list, char const* what);
int findElement(Array const& list, Tcl_Obj* what);

bool containsElement(Tcl_Obj* obj, mstl::string const& what);
bool containsElement(Tcl_Obj* obj, char const* what);
bool containsElement(Tcl_Obj* obj, Tcl_Obj* what);
bool containsElement(List const& list, mstl::string const& what);
bool containsElement(List const& list, char const* what);
bool containsElement(List const& list, Tcl_Obj* what);
bool containsElement(Array const& list, mstl::string const& what);
bool containsElement(Array const& list, char const* what);
bool containsElement(Array const& list, Tcl_Obj* what);

List& sort(List& list);

// NOTE: both arguments must be sorted
bool removeElements(List& list, List const& toRemove);

Tcl_Obj* result();

} // namespace tcl


namespace tcl {

void init(Tcl_Interp* ti);

bool updateTreeIsBlocked();

int uniqueMatch(char const* option, char const** options);
int uniqueMatchObj(Tcl_Obj* obj, char const** options);

void
__attribute__((__format__(__printf__, 1, 2)))
appendResult(char const* format, ...);

void setResult(char const* result);
void setResult(Tcl_Obj* obj);
void setResult(int objc, Tcl_Obj* const objv[]);
template <int N> void setResult(Tcl_Obj* (&objv)[N]);
void setResult(mstl::string const& s);
void setResult(int result);
void setResult(unsigned result);
void setResult(bool result);
void setResult(int64_t result);
void setResult(List const& list);
void
__attribute__((__format__(__printf__, 1, 2)))
setResultV(char const* format, ...);
int setError(char const* type);
int wrongNumArgs(int objc, Tcl_Obj* const objv[], char const* args);

int ioError(mstl::string const& file, mstl::string const& error, mstl::string const& message);
int interrupt(int count);

int
error(
	char const* cmd, char const* subcmd, char const* subsubcmd,
	char const* format, va_list& ap);

int
__attribute__((__format__(__printf__, 4, 5)))
error(
	char const* cmd, char const* subcmd, char const* subsubcmd,
	char const* format, ...);

int
usage(
	char const* cmd, char const* subcmd, char const* subsubcmd,
	char const** options, char const** args = 0);

int invoke(char const* callee, Tcl_Obj* cmd, ...);
int invoke(	char const* callee,
				Tcl_Obj* cmd, Tcl_Obj* arg1, Tcl_Obj* arg2,
				int objc, Tcl_Obj* const objv[]);

Tcl_Obj* call(char const* callee, Tcl_Obj* cmd, ...);
Tcl_Obj* call(	char const* callee,
					Tcl_Obj* cmd,
					int objc, Tcl_Obj* const objv[]);
Tcl_Obj* call(	char const* callee,
					Tcl_Obj* cmd, Tcl_Obj* arg1,
					int objc, Tcl_Obj* const objv[]);
Tcl_Obj* call(	char const* callee,
					Tcl_Obj* cmd, Tcl_Obj* arg1, Tcl_Obj* arg2,
					int objc, Tcl_Obj* const objv[]);
Tcl_Obj* call(	char const* callee,
					Tcl_Obj* cmd, Tcl_Obj* arg1, Tcl_Obj* arg2, Tcl_Obj* arg3,
					int objc, Tcl_Obj* const objv[]);
Tcl_Obj* call(	char const* callee,
					Tcl_Obj* cmd, Tcl_Obj* arg1, Tcl_Obj* arg2, Tcl_Obj* arg3, Tcl_Obj* arg4,
					int objc, Tcl_Obj* const objv[]);

Tcl_Command createCommand(Tcl_Interp* ti, char const* cmdName, Tcl_ObjCmdProc* proc);

char const* stringFromObj(unsigned objc, Tcl_Obj* const objv[], unsigned index);
int intFromObj(unsigned objc, Tcl_Obj* const objv[], unsigned index);
unsigned unsignedFromObj(unsigned objc, Tcl_Obj* const objv[], unsigned index);
long longFromObj(unsigned objc, Tcl_Obj* const objv[], unsigned index);
bool boolFromObj(unsigned objc, Tcl_Obj* const objv[], unsigned index);
Tcl_Obj* objectFromObj(unsigned objc, Tcl_Obj* const objv[], unsigned index);
int64_t wideIntFromObj(unsigned objc, Tcl_Obj* const objv[], unsigned index);

} // namespace tcl

#include "tcl_base.ipp"

#endif // _tcl_base_included

// vi:set ts=3 sw=3:
