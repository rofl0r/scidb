// ======================================================================
// Author : $Author$
// Version: $Revision: 1213 $
// Date   : $Date: 2017-06-24 13:30:42 +0000 (Sat, 24 Jun 2017) $
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
// Copyright: (C) 2009-2013 Gregor Cramer
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

extern "C"
{
	struct Tcl_Interp;
	struct Tcl_Obj;
	struct Tcl_Command_;

	typedef int Tcl_ObjCmdProc(void*, Tcl_Interp*, int, Tcl_Obj *const []);
}

namespace mstl { class string; }
namespace mstl { template <typename T> class vector; }
namespace mstl { template <typename T> class carray; }

namespace tcl {

typedef mstl::carray<Tcl_Obj*> Array;
typedef mstl::vector<Tcl_Obj*> List;

Tcl_Interp* interp();

Tcl_Obj* incrRef(Tcl_Obj* obj);
void decrRef(Tcl_Obj* obj);

Tcl_Obj* newObj();
Tcl_Obj* newObj(char const* s);
Tcl_Obj* newObj(char const* s, unsigned len);
Tcl_Obj* newObj(mstl::string const& s);
Tcl_Obj* newObj(unsigned objc, Tcl_Obj* const objv[]);
Tcl_Obj* newObj(mstl::vector<Tcl_Obj*> const& list);
Tcl_Obj* newObj(int value);

bool isInt(Tcl_Obj* obj);
bool isUnsigned(Tcl_Obj* obj);
bool isBoolean(Tcl_Obj* obj);

char const* asString(Tcl_Obj* obj);
int asInt(Tcl_Obj* obj);
unsigned asUnsigned(Tcl_Obj* obj);
bool asBoolean(Tcl_Obj* obj);

bool equal(char const* lhs, Tcl_Obj* rhs);
bool equal(Tcl_Obj* lhs, char const* rhs);
bool equal(Tcl_Obj* lhs, Tcl_Obj* rhs);
bool equal(char const* lhs, char const* rhs);

void set(Tcl_Obj*& obj, Tcl_Obj* value);
void zero(Tcl_Obj*& obj);

Tcl_Obj* getGlobalVar(Tcl_Obj* var);
void setGlobalVar(Tcl_Obj* var, Tcl_Obj* value);

unsigned getElements(Tcl_Obj* obj, Tcl_Obj**& objv);
Array getElements(Tcl_Obj* obj);
Tcl_Obj* addElement(Tcl_Obj*& list, Tcl_Obj* elem);

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
void setResult(mstl::string const& s);
void setResult(int result);
void setResult(unsigned result);
void setResult(bool result);
void setResult(long result);
void setResult(unsigned long result);
void setResult(mstl::vector<Tcl_Obj*> const& list);
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

int invoke(char const* callee, char const* cmd, ...);
int invoke(char const* callee, Tcl_Obj* cmd, ...);
int invoke(	char const* callee,
				Tcl_Obj* cmd, Tcl_Obj* arg1, Tcl_Obj* arg2,
				int objc, Tcl_Obj* const objv[]);

Tcl_Obj* call(char const* callee, char const* cmd, ...);
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

Tcl_Command_* createCommand(Tcl_Interp* ti, char const* cmdName, Tcl_ObjCmdProc* proc);

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
