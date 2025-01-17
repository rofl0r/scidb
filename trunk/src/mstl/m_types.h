// ======================================================================
// Author : $Author$
// Version: $Revision: 1453 $
// Date   : $Date: 2017-12-11 14:27:52 +0000 (Mon, 11 Dec 2017) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2017 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _mstl_types_included
#define _mstl_types_included

#include <stdint.h>
#include <stddef.h>

#ifndef __WIN32__
# if defined(_WIN32) || defined(WIN32) || defined(__MINGW32__) || defined(__WINDOWS_386__)
#  define __WIN32__
#  ifndef WIN32
#   define WIN32
#  endif
# endif
#endif

// STRICT: See MSDN Article Q83456
#ifdef __WIN32__
# ifndef STRICT
#  define STRICT
# endif
#endif

#ifndef __GNUC_PREREQ
# ifdef __GNUC__
#  define __GNUC_PREREQ(maj, min) ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
# else
#  define __GNUC_PREREQ(maj, min) 0
# endif
#endif

#ifdef __clang__
# define __CLANG_PREREQ(maj, min) ((__clang_major__ << 16) + __clang_minor__ >= ((maj) << 16) + (min))
#endif

#ifndef INT64_C
# if __WORDSIZE == 64
#  define INT64_C(c)		c ## L
#  define UINT64_C(c)	c ## UL
# else
#  define INT64_C(c)		c ## LL
#  define UINT64_C(c)	c ## ULL
# endif
#endif

#define __m_printf_format__(index, first_to_check) \
	__attribute__((__format__(__printf__, index, first_to_check)))

#define __m_likely(expr)		__builtin_expect(!!(expr), 1)
#define __m_unlikely(expr)		__builtin_expect(!!(expr), 0)

#define __m_warn_unused __attribute__((warn_unused_result))

///////////////////////////////////////////////////////////////////////////////
// C++Ox standard (reference: http://gcc.gnu.org/projects/cxx0x.html
//                            http://clang.llvm.org/cxx_status.html)
///////////////////////////////////////////////////////////////////////////////

#ifdef __clang__

# if __CLANG_PREREQ(2,9) ///////////////////////////////////////////////////////
# define HAVE_C11_RVALUE_REFERENCES 1
# define HAVE_C11_INITIALIZATION_OF_CLASS_OBJECTS_BY_RVALUES 1
# define HAVE_C11_VARIADIC_TEMPLATES 1
# define HAVE_C11_EXTENDED_VARIADIC_TEMPLATE_PARAMETERS 1
# define HAVE_C11_STATIC_ASSERTIONS 1
# define HAVE_C11_AUTO_TYPED_VARIABLES 1
# define HAVE_C11_MULTI_DECLARATOR_AUTO 1
# define HAVE_C11_REMOVAL_OF_AUTO_AS_A_STORAGE_CLASS_SPECIFIER 1
# define HAVE_C11_NEW_FUNCTION_DECLARATOR_SYNTAX 1
# define HAVE_C11_DECLARED_TYPE_OF_AN_EXPRESSSION 1
# define HAVE_C11_RIGHT_ANGLE_BRACKET 1
# define HAVE_C11_DEFAULT_TEMPLATE_ARGUMENTS_FOR_FUNCTION_TEMPLATES 1
# define HAVE_C11_SOLVING_THE_SFINAE_PROBLEM_FOR_EXPRESSIONS 1
# define HAVE_C11_EXTERN_TEMPLATES 1
# define HAVE_C11_STRONGLY_TYPED_ENUMS 1
# define HAVE_C11_NEW_CHARACTER_TYPES 1
# define HAVE_C11_EXTENDED_FRIEND_DECLARATIONS 1
# define HAVE_C11_INLINE_NAMESPACES 1
# define HAVE_C11_LOCAL_AND_UNNAMED_TYPES_AS_TEMPLATE_ARGUMENTS 1
# define HAVE_C11_PROPAGATING_EXCEPTIONS 1
# endif

# if __CLANG_PREREQ(3,0) ///////////////////////////////////////////////////////
# define HAVE_C11_NON_STATIC_DATA_MEMBER_INITIALIZERS 1
# define HAVE_C11_TEMPLATE_ALIASES
# define HAVE_C11_NULL_POINTER_CONSTANT 1
# define HAVE_C11_ALIGNMENT_SUPPORT 1
# define HAVE_C11_DELEGATING_CONSTRUCTORS 1
# define HAVE_C11_EXPLICIT_CONVERSION_OPERATORS 1
# define HAVE_C11_UNICODE_STRING_LITERALS 1
# define HAVE_C11_RAW_STRING_LITERALS 1
# define HAVE_C11_STANDARD_LAYOUT_TYPES 1
# define HAVE_C11_DEFAULTED_AND_DELETED_FUNCTIONS 1
# define HAVE_C11_RANGE_BASED_FOR 1
# define HAVE_C11_EXPLICIT_VIRTUAL_OVERRRIDES 1
# define HAVE_C11_ALLOWING_MOVE_CONSTRUCTORS_TO_THROW 1
# define HAVE_C11_DEFINING_MOVE_SPECIAL_MEMBER_FUNCTIONS 1
# endif

# if __CLANG_PREREQ(3,1) ///////////////////////////////////////////////////////
# define HAVE_C11_INITIALIZER_LISTS 1
# define HAVE_C11_LAMBDA_EXPRESSIONS 1
# define HAVE_C11_INCOMPLETE_RETURN_TYPES 1
# define HAVE_C11_FORWARD_DECLARATIONS_FOR_ENUMS 1
# define HAVE_C11_GENERALIZED_CONSTANT_EXPRESSIONS 1
# define HAVE_C11_UNIVERSAL_CHARACTER_NAME_LITERALS 1
# define HAVE_C11_USER_DEFINED_LITERALS 1
# define HAVE_C11_EXTENDING_SIZEOF 1
# define HAVE_C11_UNRESTRICTED_UNIONS 1
# define HAVE_C11_ATOMIC_OPERATORS 1
# define HAVE_C11_STRONG_COMPARE_AND_EXCHANGE 1
# define HAVE_C11_BIDIRECTIONAL_FENCES 1
# define HAVE_C11_ALLOW_ATOMICS_USE_IN_SIGNAL_HANDLERS 1
# endif

#elif defined(__GXX_EXPERIMENTAL_CXX0X__)

#ifndef USE_C11_STANDARD
// IMPORTANT NOTE: not really working with compiler versions < 4.6
# define USE_C11_STANDARD __GNUC_PREREQ(4,6)
#endif

#if USE_C11_STANDARD

# if __GNUC_PREREQ(4,3) ////////////////////////////////////////////////////////
# define HAVE_C11_INITIALIZATION_OF_CLASS_OBJECTS_BY_RVALUES 1
# define HAVE_C11_NON_STATIC_DATA_MEMBER_INITIALIZERS 0
# define HAVE_C11_CONSTANT_EXPRESSIONS 1
# define HAVE_C11_TEMPLATE_ALIASES 0
# define HAVE_C11_EXTERN_TEMPLATES 1
# define HAVE_C11_GENERALIZED_ATTRIBUTES 0
# define HAVE_C11_ALIGNMENT_SUPPORT 0
# define HAVE_C11_DELEGATING_CONSTRUCTORS 0
# define HAVE_C11_INHERITING_CONSTRUCTORS 0
# define HAVE_C11_USER_DEFINED_LITERALS 0
# define HAVE_C11_GARBAGE_COLLECTION 0
# define HAVE_C11_SEQUENCE_POINTS 0
# define HAVE_C11_STRONG_COMPARE_AND_EXCHANGE 0
# define HAVE_C11_BIDIRECTIONAL_FENCES 0
# define HAVE_C11_MEMORY_MODEL 0
# define HAVE_C11_DATA_DEPENDENCY_ORDERING 0
# define HAVE_C11_ABANDONING_A_PROCESS_AND_AT_QUICK_EXIT 0
# define HAVE_C11_ALLOW_ATOMICS_USE_IN_SIGNAL_HANDLERS 0
# define HAVE_C11_THREAD_LOCAL_STORAGE 0
# define HAVE_C11_DYNAMIC_INITIALIZATION_AND_DESTRUCTION_WITH_CONCURRENCY 0
# define HAVE_C11_EXTENDED_INTEGRAL_TYPES 0
# endif ////////////////////////////////////////////////////////////////////////


# if __GNUC_PREREQ(4,3) ////////////////////////////////////////////////////////
# define HAVE_C11_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR 1
# define HAVE_C11_EXPLICITLY_DEFAULTED_AND_DELETED_SPECIAL_MEMBER_FUNCTIONS 1
# define HAVE_C11_RVALUE_REFERENCES 1
# define HAVE_C11_VARIADIC_TEMPLATES 1
# define HAVE_C11_STATIC_ASSERTIONS 1
# define HAVE_C11_DECLARED_TYPE_OF_AN_EXPRESSSION 1
# define HAVE_C11_RIGHT_ANGLE_BRACKET 1
# define HAVE_C11_DEFAULT_TEMPLATE_ARGUMENTS_FOR_FUNCTION_TEMPLATES 1
# endif ////////////////////////////////////////////////////////////////////////


# if __GNUC_PREREQ(4,4) ////////////////////////////////////////////////////////
# define HAVE_C11_EXTENDED_VARIADIC_TEMPLATE_PARAMETERS 1
# define HAVE_C11_INITIALIZER_LISTS 1
# define HAVE_C11_AUTO_TYPED_VARIABLES 1
# define HAVE_C11_MULTI_DECLARATOR_AUTO 1
# define HAVE_C11_REMOVAL_OF_AUTO_AS_A_STORAGE_CLASS_SPECIFIER 1
# define HAVE_C11_NEW_FUNCTION_DECLARATOR_SYNTAX 1
# define HAVE_C11_SOLVING_THE_SFINAE_PROBLEM_FOR_EXPRESSIONS 1
# define HAVE_C11_STRONGLY_TYPED_ENUMS 1
# define HAVE_C11_NEW_CHARACTER_TYPES 1
# define HAVE_C11_DEFAULTED_AND_DELETED_FUNCTIONS 1
# define HAVE_C11_EXTENDING_SIZEOF 1
# define HAVE_C11_INLINE_NAMESPACES 1
# define HAVE_C11_ATOMIC_OPERATORS 1
# define HAVE_C11_PROPAGATING_EXCEPTIONS 1
# endif ///////////////////////////////////////////////////////////////////////


# if __GNUC_PREREQ(4,5) ////////////////////////////////////////////////////////
# define HAVE_C11_NEW_WORDING_FOR_LAMBDAS 1
# define HAVE_C11_EXPLICIT_CONVERSION_OPERATORS 1
# define HAVE_C11_UNICODE_STRING_LITERALS 1
# define HAVE_C11_RAW_STRING_LITERALS 1
# define HAVE_C11_UNIVERSAL_CHARACTER_NAME_LITERALS 1
# define HAVE_C11_STANDARD_LAYOUT_TYPES 1
# define HAVE_C11_LOCAL_AND_UNNAMED_TYPES_AS_TEMPLATE_ARGUMENTS 1
# endif ///////////////////////////////////////////////////////////////////////


# if __GNUC_PREREQ(4,6) ////////////////////////////////////////////////////////
# define HAVE_C11_NULL_POINTER_CONSTANT 1
# define HAVE_C11_FORWARD_DECLARATIONS_FOR_ENUMS 1
# define HAVE_C11_GENERALIZED_CONSTANT_EXPRESSIONS 1
# define HAVE_C11_UNRESTRICTED_UNIONS 1
# define HAVE_C11_RANGE_BASED_FOR 1
# define HAVE_C11_ALLOWING_MOVE_CONSTRUCTORS_TO_THROW 1
# define HAVE_C11_DEFINING_MOVE_SPECIAL_MEMBER_FUNCTIONS 1
# endif ///////////////////////////////////////////////////////////////////////


# if __GNUC_PREREQ(4,7) ////////////////////////////////////////////////////////
# define HAVE_C11_EXTENDED_FRIEND_DECLARATIONS 1
# define HAVE_C11_EXPLICIT_VIRTUAL_OVERRRIDES 1
# define HAVE_NON_STATIC_DATA_MEMBER_INITIALIZERS 1
# define HAVE_TEMPLATE_ALIASES 1
# define HAVE_DELEGATING_CONSTRUCTORS 1
# define HAVE_USER_DEFINED_LITERALS 1
# endif ///////////////////////////////////////////////////////////////////////

#endif // USE_C11_STANDARD
#endif // __GXX_EXPERIMENTAL_CXX0X__

#ifndef HAVE_C11_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR
# define HAVE_C11_MOVE_CONSTRCUTOR_AND_ASSIGMENT_OPERATOR 0
#endif
#ifndef HAVE_C11_EXPLICITLY_DEFAULTED_AND_DELETED_SPECIAL_MEMBER_FUNCTIONS
# define HAVE_C11_EXPLICITLY_DEFAULTED_AND_DELETED_SPECIAL_MEMBER_FUNCTIONS 0
#endif
#ifndef HAVE_C11_CONSTANT_EXPRESSIONS
# define HAVE_C11_CONSTANT_EXPRESSIONS 0
#endif
#ifndef HAVE_C11_RVALUE_REFERENCES
# define HAVE_C11_RVALUE_REFERENCES 0
#endif
#ifndef HAVE_C11_VARIADIC_TEMPLATES
# define HAVE_C11_VARIADIC_TEMPLATES 0
#endif
#ifndef HAVE_C11_STATIC_ASSERTIONS
# define HAVE_C11_STATIC_ASSERTIONS 0
#endif
#ifndef HAVE_C11_DECLARED_TYPE_OF_AN_EXPRESSSION
# define HAVE_C11_DECLARED_TYPE_OF_AN_EXPRESSSION 0
#endif
#ifndef HAVE_C11_RIGHT_ANGLE_BRACKET
# define HAVE_C11_RIGHT_ANGLE_BRACKET 0
#endif
#ifndef HAVE_C11_DEFAULT_TEMPLATE_ARGUMENTS_FOR_FUNCTION_TEMPLATES
# define HAVE_C11_DEFAULT_TEMPLATE_ARGUMENTS_FOR_FUNCTION_TEMPLATES 0
#endif
#ifndef HAVE_C11_EXTENDED_VARIADIC_TEMPLATE_PARAMETERS
# define HAVE_C11_EXTENDED_VARIADIC_TEMPLATE_PARAMETERS 0
#endif
#ifndef HAVE_C11_INITIALIZER_LISTS
# define HAVE_C11_INITIALIZER_LISTS 0
#endif
#ifndef HAVE_C11_AUTO_TYPED_VARIABLES
# define HAVE_C11_AUTO_TYPED_VARIABLES 0
#endif
#ifndef HAVE_C11_MULTI_DECLARATOR_AUTO
# define HAVE_C11_MULTI_DECLARATOR_AUTO 0
#endif
#ifndef HAVE_C11_REMOVAL_OF_AUTO_AS_A_STORAGE_CLASS_SPECIFIER
# define HAVE_C11_REMOVAL_OF_AUTO_AS_A_STORAGE_CLASS_SPECIFIER 0
#endif
#ifndef HAVE_C11_NEW_FUNCTION_DECLARATOR_SYNTAX
# define HAVE_C11_NEW_FUNCTION_DECLARATOR_SYNTAX 0
#endif
#ifndef HAVE_C11_SOLVING_THE_SFINAE_PROBLEM_FOR_EXPRESSIONS
# define HAVE_C11_SOLVING_THE_SFINAE_PROBLEM_FOR_EXPRESSIONS 0
#endif
#ifndef HAVE_C11_STRONGLY_TYPED_ENUMS
# define HAVE_C11_STRONGLY_TYPED_ENUMS 0
#endif
#ifndef HAVE_C11_NEW_CHARACTER_TYPES
# define HAVE_C11_NEW_CHARACTER_TYPES 0
#endif
#ifndef HAVE_C11_DEFAULTED_AND_DELETED_FUNCTIONS
# define HAVE_C11_DEFAULTED_AND_DELETED_FUNCTIONS 0
#endif
#ifndef HAVE_C11_EXTENDING_SIZEOF
# define HAVE_C11_EXTENDING_SIZEOF 0
#endif
#ifndef HAVE_C11_INLINE_NAMESPACES
# define HAVE_C11_INLINE_NAMESPACES 0
#endif
#ifndef HAVE_C11_ATOMIC_OPERATORS
# define HAVE_C11_ATOMIC_OPERATORS 0
#endif
#ifndef HAVE_C11_PROPAGATING_EXCEPTIONS
# define HAVE_C11_PROPAGATING_EXCEPTIONS 0
#endif
#ifndef HAVE_C11_NEW_WORDING_FOR_LAMBDAS
# define HAVE_C11_NEW_WORDING_FOR_LAMBDAS 0
#endif
#ifndef HAVE_C11_EXPLICIT_CONVERSION_OPERATORS
# define HAVE_C11_EXPLICIT_CONVERSION_OPERATORS 0
#endif
#ifndef HAVE_C11_UNICODE_STRING_LITERALS
# define HAVE_C11_UNICODE_STRING_LITERALS 0
#endif
#ifndef HAVE_C11_RAW_STRING_LITERALS
# define HAVE_C11_RAW_STRING_LITERALS 0
#endif
#ifndef HAVE_C11_UNIVERSAL_CHARACTER_NAME_LITERALS
# define HAVE_C11_UNIVERSAL_CHARACTER_NAME_LITERALS 0
#endif
#ifndef HAVE_C11_STANDARD_LAYOUT_TYPES
# define HAVE_C11_STANDARD_LAYOUT_TYPES 0
#endif
#ifndef HAVE_C11_LOCAL_AND_UNNAMED_TYPES_AS_TEMPLATE_ARGUMENTS
# define HAVE_C11_LOCAL_AND_UNNAMED_TYPES_AS_TEMPLATE_ARGUMENTS 0
#endif
#ifndef HAVE_C11_NULL_POINTER_CONSTANT
# define HAVE_C11_NULL_POINTER_CONSTANT 0
#endif
#ifndef HAVE_C11_FORWARD_DECLARATIONS_FOR_ENUMS
# define HAVE_C11_FORWARD_DECLARATIONS_FOR_ENUMS 0
#endif
#ifndef HAVE_C11_GENERALIZED_CONSTANT_EXPRESSIONS
# define HAVE_C11_GENERALIZED_CONSTANT_EXPRESSIONS 0
#endif
#ifndef HAVE_C11_UNRESTRICTED_UNIONS
# define HAVE_C11_UNRESTRICTED_UNIONS 0
#endif
#ifndef HAVE_C11_RANGE_BASED_FOR
# define HAVE_C11_RANGE_BASED_FOR 0
#endif
#ifndef HAVE_C11_ALLOWING_MOVE_CONSTRUCTORS_TO_THROW
# define HAVE_C11_ALLOWING_MOVE_CONSTRUCTORS_TO_THROW 0
#endif
#ifndef HAVE_C11_DEFINING_MOVE_SPECIAL_MEMBER_FUNCTIONS
# define HAVE_C11_DEFINING_MOVE_SPECIAL_MEMBER_FUNCTIONS 0
#endif
#ifndef HAVE_C11_EXTENDED_FRIEND_DECLARATIONS
# define HAVE_C11_EXTENDED_FRIEND_DECLARATIONS 0
#endif
#ifndef HAVE_C11_EXPLICIT_VIRTUAL_OVERRRIDES
# define HAVE_C11_EXPLICIT_VIRTUAL_OVERRRIDES 0
#endif
#ifndef HAVE_NON_STATIC_DATA_MEMBER_INITIALIZERS
# define HAVE_NON_STATIC_DATA_MEMBER_INITIALIZERS 0
#endif
#ifndef HAVE_TEMPLATE_ALIASES
# define HAVE_TEMPLATE_ALIASES 0
#endif
#ifndef HAVE_DELEGATING_CONSTRUCTORS
# define HAVE_DELEGATING_CONSTRUCTORS 0
#endif
#ifndef HAVE_USER_DEFINED_LITERALS
# define HAVE_USER_DEFINED_LITERALS 0
#endif
#ifndef HAVE_C11_LAMBDA_EXPRESSIONS
# define HAVE_C11_LAMBDA_EXPRESSIONS 0
#endif
#ifndef HAVE_C11_INCOMPLETE_RETURN_TYPES
# define HAVE_C11_INCOMPLETE_RETURN_TYPES 0
#endif

// C++Ox definitions //////////////////////////////////////////////////////////

#if !defined(__clang__)

# if !HAVE_C11_CONSTANT_EXPRESSIONS
#  define constexpr const
# endif

# if !HAVE_C11_NULL_POINTER_CONSTANT
#  define nullptr NULL
# endif

# if !HAVE_C11_EXPLICIT_VIRTUAL_OVERRRIDES
#  define override
# endif

# if !HAVE_C11_ALLOWING_MOVE_CONSTRUCTORS_TO_THROW
#  define noexcept
# endif

# if !__GNUC_PREREQ(4,3) || !defined(USE_C11_STANDARD)
#  define decltype typeof
# endif

#endif

#if !HAVE_C11_STATIC_ASSERTIONS

namespace mstl {
namespace bits {

template <bool> struct compile_time_checker;
template <> struct compile_time_checker<true> {};

} // namespace bits
} // namespace mstl

# define static_assert(expr, msg) \
	{::mstl::bits::compile_time_checker<((expr) != 0)> COMPILE_TIME_ERROR __attribute__((unused));}

#endif

// Overwrite __GNUC_PREREQ for clang compiler, needed to be true in any case
#ifdef __clang__
# undef __GNUC_PREREQ
# define __GNUC_PREREQ(maj, min)  1
#endif

// Testing ////////////////////////////////////////////////////////////////////

namespace mstl {
	namespace bits {
#ifdef TEST64
		typedef uint64_t size_t;
#else
		typedef ::size_t size_t;
#endif
	}
}

#endif // _mstl_types_included

// vi:set ts=3 sw=3:
