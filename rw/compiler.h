#ifndef RW_TOOLS_COMPILER_H
#define RW_TOOLS_COMPILER_H

// RCB
#include <rw/config/rwconfig_tls.h>      /* Set compiler-specific flags */

/**********************************************************************
 *
 * This file is now an internal one used for utility and cases not
 * otherwise handled. The results of compiler behavior macros
 * no longer have this file as their point of origin.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/compiler.h#4 $
 *
 **********************************************************************
 *
 * Copyright (c) 1989-2015 Rogue Wave Software, Inc.  All Rights Reserved.
 * 
 * This computer software is owned by Rogue Wave Software, Inc. and is
 * protected by U.S. copyright laws and other laws and by international
 * treaties.  This computer software is furnished by Rogue Wave Software, Inc.
 * pursuant to a written license agreement and may be used, copied, transmitted,
 * and stored only in accordance with the terms of such license agreement and 
 * with the inclusion of the above copyright notice.  This computer software or
 * any other copies thereof may not be provided or otherwise made available to
 * any other person.
 * 
 * U.S. Government Restricted Rights.  This computer software: (a) was
 * developed at private expense and is in all respects the proprietary
 * information of Rogue Wave Software, Inc.; (b) was not developed with 
 * government funds; (c) is a trade secret of Rogue Wave Software, Inc. for all
 * purposes of the Freedom of Information Act; and (d) is a commercial item and
 * thus, pursuant to Section 12.212 of the Federal Acquisition Regulations (FAR)
 * and DFAR Supplement Section 227.7202, Government's use, duplication or
 * disclosure of the computer software is subject to the restrictions set forth
 * by Rogue Wave Software, Inc.
 *
 **********************************************************************/

// The next several lines #define constants for messages use
#define RW_NOMSG    0x00
#define RW_CATGETS  0x01
#define RW_GETTEXT  0x02
#define RW_DGETTEXT 0x03

// Used in RWCString to work around Intel compiler issue (PR #29003)
#define RW_INLINE inline

/************************ Microsoft C/C++ *****************************/

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)

#  define RW_MSC_BACKEND                        1

#  define RW_NO_STATIC_MUTEX_INIT               1

// MSVC 6.0
#  if (_MSC_VER == 1200)
#    define RW_WCSXFRM_WRITES_TO_NULL_BUG         1
#    define RW_AMBIGUOUS_ON_CONST                 1

#    if !defined(RW_STDLIB_VER)
// The MS Stdlib has a bug, where the following code will
// segfault if the "str1[0]" statement is commented out.
//  std::string str0("1234567890\0\r\n0123456789X", 24);
//  std::string str1(str0);
//  str1[0];
//  str1.erase(23, 1);
#      define RW_MSVC_STDLIB_BUG                1
#      define RW_GETLINE_BUG                    1
#    endif

// Do not use typename with MS VC++ 6.0
#    define RW_NO_TYPENAME

#    if !defined(RW_C4786_ON) && !defined(_RWSTD_C4786_ON)
// disable warnings about identifiers being
// truncated in debug builds
#      pragma warning( disable : 4786 )
#      pragma warning( disable : 4503 )
#    endif

#    define RW_NO_XDR                           1
#    define RW_NO_POSIX_RE                      1
#    define RW_NO_XMSG                          1
#    define RW_NO_PUBSEEKOFF                    1
#    define RW_NO_TYPEDEF_ITERATOR              1
#    define RW_NO_PTR_PAIR_CTOR                 1

#    define RW_BROKEN_INITIALIZER               1
#    define RW_NO_INHERIT_TEMPL_ACCESS          1

#    define RW_MEMBER_OPERATOR_SYNTAX_PROBLEM   1
#    define RW_FALSE_NEW_DELETE_MISMATCH        1

#    if defined(RW_STDLIB_VER) && RW_STDLIB_VER >= 0x02000000
#      define RW_3ARG_DISTANCE
#      define RW_ALLOC_INTERFACE_STLV2X_HACK
#    endif

// Force the following for STLport builds where STLport
// allocator class has rebind nested type
#    if !defined(RW_ALLOCATOR_NO_REBIND)
#      define RW_ALLOCATOR_NO_REBIND
#    endif

#  endif

// MSVC 7.0
#  if (_MSC_VER == 1300)
#    if !defined(RW_STDLIB_VER)
#      if defined(_WIN64)
#        define RW_MSVC_STDLIB_BUG            1
#        define RW_GETLINE_BUG                1
#      endif
#    endif

#    if defined(RW_STDLIB_VER) && RW_STDLIB_VER >= 0x02000000
#      define RW_3ARG_DISTANCE
#    endif

#    define RW_MEMBER_OPERATOR_SYNTAX_PROBLEM 1
#    define RW_NO_STATIC_CONST_MEMBER_INIT    1
#  endif // _MSC_VER == 1300

#  ifdef _MT
#    define RW_MULTI_THREAD 1
#  endif

#  pragma warning (disable: 4114)

// MSVC 7.1
#  if (_MSC_VER == 1310)
#    if !defined(RW_STDLIB_VER)
#      if defined(_WIN64)
#        define RW_MSVC_STDLIB_BUG            1
#        define RW_GETLINE_BUG                1
#      endif
#    endif

#    if defined(RW_STDLIB_VER) && RW_STDLIB_VER >= 0x02000000
#      define RW_3ARG_DISTANCE
#    endif

#    define RW_MEMBER_OPERATOR_SYNTAX_PROBLEM 1
#    define RW_NO_STATIC_CONST_MEMBER_INIT    1
#  endif // _MSC_VER == 1310

// MSVC 8.0+
#  if (_MSC_VER > 1310)
#    define RW_NO_STATIC_CONST_MEMBER_INIT    1
#  endif // _MSC_VER > 1310

#  if defined(RW_ENABLE_DEPRECATED_CHECKS)
#    define RW_DEPRECATE_TYPE(str) __declspec(deprecated(str))
#    define RW_DEPRECATE_FUNC(str) __declspec(deprecated(str))
#    define RW_DEPRECATE_VAR(entity, str) __declspec(deprecated(str)) entity
#    define RW_DEPRECATE_MACRO(macro, str) __pragma(deprecated(macro))

#    define RW_SUPPRESS_DEPRECATED_WARNINGS \
            __pragma(warning(push)) \
            __pragma(warning(disable:4995)) \
            __pragma(warning(disable:4996))

#    define RW_RESTORE_DEPRECATED_WARNINGS \
            __pragma(warning(pop))
#  endif
#endif  //_MSC_VER


/********************** IBM C++ (AIX) ***********************/

#if defined(__IBMCPP__)
#  if __IBMCPP__ >= 500
#    define RW_TLS_ALLOCATE(oA,N,ignore)       oA.allocate(N)
#    define RW_NO_IMPLICIT_INCLUSION           1
#    define RW_DECLARE_GLOBAL_FRIENDS_FIRST    1
#    define RW_THROW_OPTIMIZATION_BUG          1
#  endif

#  if !defined(RW_STDLIB_VER)
#    define RW_IBMCPP_STDLIB
#  endif
#endif


/********************** GCC *********************************/

#if defined(__GNUG__) && !defined(__INTEL_COMPILER) && !defined(__clang__)

#  define RW_GCC_VERSION (__GNUC__ * 10000 \
                      + __GNUC_MINOR__ * 100 \
                      + __GNUC_PATCHLEVEL__)

// Solaris GCC is missing outer braces in its PTHREAD_ONCE_INIT definition.
#  if defined(__sun) && (RW_GCC_VERSION < 40200)
#    define RW_BROKEN_PTHREAD_ONCE_INIT
#  endif

// All supported versions of GNUC support attribute syntax.  Support for
// individual attributes will of course vary between GNUC versions.
#  define RW_GNUC_ATTR(attr) __attribute__((attr))

// Also supported by all supported versions of GNUC.
#  define RW_UNUSED_ATTR RW_GNUC_ATTR(unused)

#  if (__GNUC__ == 4)
#    if (__GNUC_MINOR__ == 3) || (__GNUC_MINOR__ == 4)
#      if defined(__GXX_EXPERIMENTAL_CXX0X__)
#        define RW_BROKEN_GCC_MATH_TEMPLATE
#      endif
#    endif

#    if (__GNUC_MINOR__ >= 6)
#      if defined(RW_ENABLE_DEPRECATED_CHECKS)
#        define RW_DEPRECATE_TYPE(str) __attribute__((deprecated(str)))
#        define RW_DEPRECATE_FUNC(str) __attribute__((deprecated(str)))
#        define RW_DEPRECATE_VAR(entity, str) __attribute__((deprecated(str))) entity

#        define RW_SUPPRESS_DEPRECATED_WARNINGS \
            _Pragma("GCC diagnostic push") \
            _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")

#        define RW_RESTORE_DEPRECATED_WARNINGS \
            _Pragma("GCC diagnostic pop")
#      endif
#    endif
#  elif (__GNUG__ == 3)

#    if (__GNUC_MINOR__ < 1)
#      define RW_INHERITANCE_HIERARCHY_BUG     1
#      define RW_NO_NEW_HEADER                 1
#      if !defined(RW_STDLIB_VER) || RW_STDLIB_VER < 0x02000000
#        define RW_WHITE_SPACE_BUG             1
#        define RW_COPY_ON_WRITE_STRING        1
#      endif
#    endif

#    if (__GNUC_MINOR__ < 4)
#      define RW_WARNING_ON_ENCLOSING_TYPEDEF  1
#    endif

#  elif ( __GNUG__ <= 2 )

#    define RW_WARNING_ON_ENCLOSING_TYPEDEF    1
#    define RW_BROKEN_PLACEMENT_NEW            1

#    if defined(__LP64__)
#      define RW_STREAMBUF_BUG                 1
#      if !defined(RW_DEBUG) && \
            defined(RW_STDLIB_VER) && RW_STDLIB_VER >= 0x02000000
#        define RW_RWSTD_DEQUE_CONST_ITERATOR_PROBLEM 1
#      endif
#    endif

#    if !defined(__hpux)
#      define RW_WCTYPEH_NEEDED                1
#    endif

#    define RW_STD_COLLECTION_PROBLEM          1

#    if defined(__sun)
#      define RW_NO_XDR                        1
#    endif

#    undef  RW_NO_IO_SENTRY
#    define RW_NO_LIBC_IN_STD                  1
#    if !defined(RW_STDLIB_VER)
#      define RW_NO_ANSILOCALE                 1
#      define RW_NO_NEW_LOCALE_HEADER          1
#      define RW_NO_IO_SENTRY                  1
#      define RW_NO_STD_WIDE_STREAMS           1
#      define RW_ITER_NO_TAGS                  1
#      define RW_COPY_ON_WRITE_STRING          1
#    endif // !defined(RW_STDLIB_VER)

#  endif // if (__GNUG__ > 2)

#endif // if defined(__GNUG__)


/********************** Clang **********************************/

#if defined(__clang__)

#  define RW_CLANG_VERSION (__clang_major__ * 10000 \
                           + __clang_minor__ * 100 \
                           + __clang_patchlevel__)

// All supported versions of Clang support attribute syntax.  Support for
// individual attributes will of course vary between Clang versions.
#  define RW_CLANG_ATTR(attr) __attribute__((attr))

// Also supported by all supported versions of GNUC.
#  define RW_UNUSED_ATTR RW_CLANG_ATTR(unused)

#  if defined(RW_ENABLE_DEPRECATED_CHECKS)
#    define RW_DEPRECATE_TYPE(str) __attribute__((deprecated(str)))
#    define RW_DEPRECATE_FUNC(str) __attribute__((deprecated(str)))
#    define RW_DEPRECATE_VAR(entity, str) __attribute__((deprecated(str))) entity
#    define RW_DEPRECATE_ENUMERATOR(entity, str) entity __attribute__((deprecated(str)))

#    define RW_SUPPRESS_DEPRECATED_WARNINGS \
            _Pragma("GCC diagnostic push") \
            _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")

#    define RW_RESTORE_DEPRECATED_WARNINGS \
            _Pragma("GCC diagnostic pop")
#  endif
#endif // if defined(__clang__)


/********************** Sunpro *********************************/

#if defined(__SUNPRO_CC)
#  define RW_MACRO_GUARD(macro) macro
#  if (__SUNPRO_CC == 0x500)
#    define RW_XITER_ARGS_REQ 1

#    if defined(RW_STDLIB_VER) && RW_STDLIB_VER == 0x02010100 && \
       !defined(RW_SUNPRO_NATIVE_STDLIB)
#      define RW_SUNPRO_NATIVE_STDLIB         1
#    endif // Sun, stdlib version 2.1.1

#  elif (__SUNPRO_CC >= 0x510)
#    if defined(RW_STDLIB_VER) && RW_STDLIB_VER == 0x02010100 && \
       !defined(RW_SUNPRO_NATIVE_STDLIB)
#      define RW_SUNPRO_NATIVE_STDLIB         1
#    endif
#  endif // if (__SUNPRO_CC >= 0x510)

#  if (__SUNPRO_CC >= 0x530)
#    define RW_3ARG_DISTANCE                   1
#    define RW_NO_WCSTOMBS_PROTOTYPE           1
#    define RW_NO_MBTOWC_PROTOTYPE             1
#  endif

#  if defined(_STLPORT_VERSION) && (_STLPORT_VERSION >= 0x452) && \
      (_STLPORT_VERSION <= 0x453)
#    define RW_NO_ANSILOCALE                 1
#  endif

#endif // __SUNPRO_CC


/********************** HP-UX aCC *********************************/

// historical reasons only
#if defined(__HP_aCC)

// Do not use the memory pool for HP ANSI
#  define RW_DONT_USE_MEMPOOL

#endif // __HP_aCC

// The HP-UX aCC compiler will warn that without const qualifying the return
// value from this method that data encapsulation is being broken.  While
// technically true, this is part of the intended API for this class.  Disable
// the warning for this method.
#if defined(__HP_aCC) && __HP_aCC >= 61600
template <class T> inline T& rw_expose(T& x)
{
    return x;
}
#  define RW_EXPOSE(x)   rw_expose (x)
#else
#  define RW_EXPOSE(x)   x
#endif

#if defined(__HP_aCC) && (__HP_aCC <= 62801)

#if (__HP_aCC >= 62800)
// remark #2550-D: variable 'XXX' was set but never used
#  define RW_SUPPRESS_UNUSED_WARNING _Pragma("diag_suppress 2550")
#  define RW_RESTORE_UNUSED_WARNING _Pragma("diag_default 2550")

// remark #6383-D: dynamic exception specifications are deprecated
#  define RW_SUPPRESS_DYNAMIC_EXCEPTION_SPEC_DEPRECATED_WARNING _Pragma("diag_suppress 6383")
#  define RW_RESTORE_DYNAMIC_EXCEPTION_SPEC_DEPRECATED_WARNING _Pragma("diag_default 6383")
#else
#  define RW_SUPPRESS_UNUSED_WARNING
#  define RW_RESTORE_UNUSED_WARNING

#  define RW_SUPPRESS_DYNAMIC_EXCEPTION_SPEC_DEPRECATED_WARNING
#  define RW_RESTORE_DYNAMIC_EXCEPTION_SPEC_DEPRECATED_WARNING
#endif

// warning #2554-D: "XXX::operator XXX()" will not be called for implicit or
// explicit conversions
#  define RW_SUPPRESS_CONVERSION_OP_NEVER_CALLED_WARNING _Pragma("diag_suppress 2554")
#  define RW_RESTORE_CONVERSION_OP_NEVER_CALLED_WARNING _Pragma("diag_default 2554")

// remark #4230-D: 64 bit migration: conversion from "XXX *" to "XXX *" may
// cause target of pointers to have a different size
#  define RW_SUPPRESS_64BIT_TARGET_OF_POINTER_SIZE_WARNING _Pragma("diag_suppress 4230")
#  define RW_RESTORE_64BIT_TARGET_OF_POINTER_SIZE_WARNING _Pragma("diag_default 4230")

// remark #4231-D: 64 bit migration: conversion between types of different
// sizes has occurred (from "XXX" to "YYY" )
#  define RW_SUPPRESS_64BIT_DIFFERENT_SIZE_TYPE_CONVERSION_WARNING _Pragma("diag_suppress 4231")
#  define RW_RESTORE_64BIT_DIFFERENT_SIZE_TYPE_CONVERSION_WARNING _Pragma("diag_default 4231")

// warning #4232-D: conversion from "XXX *" to a more strictly aligned type
// "YYY *" may cause misaligned access
#  define RW_SUPPRESS_CAST_STRICTER_ALIGNMENT_WARNING _Pragma("diag_suppress 4232")
#  define RW_RESTORE_CAST_STRICTER_ALIGNMENT_WARNING _Pragma("diag_default 4232")

// remark #4235-D: conversion from "XXX" to "YYY" may lose significant bits
#  define RW_SUPPRESS_LOSE_SIGNIFICANT_BITS_WARNING _Pragma("diag_suppress 4235")
#  define RW_RESTORE_LOSE_SIGNIFICANT_BITS_WARNING _Pragma("diag_default 4235")

// remark #4273-D: floating-point equality and inequality comparisons may be
// inappropriate due to roundoff common in floating-point computation
#  define RW_SUPPRESS_FLOATING_POINT_EQUALITY_WARNING _Pragma("diag_suppress 4273")
#  define RW_RESTORE_FLOATING_POINT_EQUALITY_WARNING _Pragma("diag_default 4273")

// remark #4282-D: virtual function is called from constructor/destructor
#  define RW_SUPPRESS_CTOR_CALLS_VIRTUAL_FUNC_WARNING _Pragma("diag_suppress 4282")
#  define RW_RESTORE_CTOR_CALLS_VIRTUAL_FUNC_WARNING _Pragma("diag_default 4282")

// remark #4283-D: access global variable "XXX" in constructor "YYY"
#  define RW_SUPPRESS_CTOR_ACCESSES_GLOBAL_VAR_WARNING _Pragma("diag_suppress 4283")
#  define RW_RESTORE_CTOR_ACCESSES_GLOBAL_VAR_WARNING _Pragma("diag_default 4283")

// remark #4285-D: operator= does not have a check for the source and
// destination addresses being non-identical
#  define RW_SUPPRESS_OP_ASSIGN_SELF_CHECK_WARNING _Pragma("diag_suppress 4285")
#  define RW_RESTORE_OP_ASSIGN_SELF_CHECK_WARNING _Pragma("diag_default 4285")

// remark #4356-D: operand of sizeof is a constant rvalue, this might not be
// what you intended
#  define RW_SUPPRESS_SIZEOF_CONSTANT_RVALUE_WARNING _Pragma("diag_suppress 4356")
#  define RW_RESTORE_SIZEOF_CONSTANT_RVALUE_WARNING _Pragma("diag_default 4356")

// warning #20200-D: Potential null pointer dereference through XXX is detected
#  define RW_SUPPRESS_NULL_POINTER_DEREF_WARNING _Pragma("diag_suppress 20200")
#  define RW_RESTORE_NULL_POINTER_DEREF_WARNING _Pragma("diag_default 20200")

// warning #20201-D: Memory leak is detected
#  define RW_SUPPRESS_MEMORY_LEAK_WARNING _Pragma("diag_suppress 20201")
#  define RW_RESTORE_MEMORY_LEAK_WARNING _Pragma("diag_default 20201")

// warning #20206-D: Possible out of bound access (In expression "XXX", YYY
// has byte range [M .. N], writing byte range [O .. P].)
// warning #20207-D: Out of bound access (In expression "XXX", variable "YYY"
// [...] (type: ZZZ ) has N bytes, reading byte range [M .. N].)
#  define RW_SUPPRESS_OUT_OF_BOUNDS_ACCESS_WARNING _Pragma("diag_suppress 20206, 20207")
#  define RW_RESTORE_OUT_OF_BOUNDS_ACCESS_WARNING _Pragma("diag_default 20206, 20207")
#else
#  define RW_SUPPRESS_UNUSED_WARNING
#  define RW_RESTORE_UNUSED_WARNING

#  define RW_SUPPRESS_CONVERSION_OP_NEVER_CALLED_WARNING
#  define RW_RESTORE_CONVERSION_OP_NEVER_CALLED_WARNING

#  define RW_SUPPRESS_64BIT_TARGET_OF_POINTER_SIZE_WARNING
#  define RW_RESTORE_64BIT_TARGET_OF_POINTER_SIZE_WARNING

#  define RW_SUPPRESS_64BIT_DIFFERENT_SIZE_TYPE_CONVERSION_WARNING
#  define RW_RESTORE_64BIT_DIFFERENT_SIZE_TYPE_CONVERSION_WARNING

#  define RW_SUPPRESS_CAST_STRICTER_ALIGNMENT_WARNING
#  define RW_RESTORE_CAST_STRICTER_ALIGNMENT_WARNING

#  define RW_SUPPRESS_LOSE_SIGNIFICANT_BITS_WARNING
#  define RW_RESTORE_LOSE_SIGNIFICANT_BITS_WARNING

#  define RW_SUPPRESS_FLOATING_POINT_EQUALITY_WARNING
#  define RW_RESTORE_FLOATING_POINT_EQUALITY_WARNING

#  define RW_SUPPRESS_CTOR_CALLS_VIRTUAL_FUNC_WARNING
#  define RW_RESTORE_CTOR_CALLS_VIRTUAL_FUNC_WARNING

#  define RW_SUPPRESS_CTOR_ACCESSES_GLOBAL_VAR_WARNING
#  define RW_RESTORE_CTOR_ACCESSES_GLOBAL_VAR_WARNING

#  define RW_SUPPRESS_OP_ASSIGN_SELF_CHECK_WARNING
#  define RW_RESTORE_OP_ASSIGN_SELF_CHECK_WARNING

#  define RW_SUPPRESS_SIZEOF_CONSTANT_RVALUE_WARNING
#  define RW_RESTORE_SIZEOF_CONSTANT_RVALUE_WARNING

#  define RW_SUPPRESS_DYNAMIC_EXCEPTION_SPEC_DEPRECATED_WARNING
#  define RW_RESTORE_DYNAMIC_EXCEPTION_SPEC_DEPRECATED_WARNING

#  define RW_SUPPRESS_NULL_POINTER_DEREF_WARNING
#  define RW_RESTORE_NULL_POINTER_DEREF_WARNING

#  define RW_SUPPRESS_MEMORY_LEAK_WARNING
#  define RW_RESTORE_MEMORY_LEAK_WARNING

#  define RW_SUPPRESS_OUT_OF_BOUNDS_ACCESS_WARNING
#  define RW_RESTORE_OUT_OF_BOUNDS_ACCESS_WARNING
#endif

#if defined(__HP_aCC) && (__HP_aCC >= 62700) && defined(RW_ENABLE_DEPRECATED_CHECKS)
#  define RW_DEPRECATE_TYPE(str) __attribute__((deprecated(str)))
#  define RW_DEPRECATE_FUNC(str) __attribute__((deprecated(str)))
#  define RW_DEPRECATE_VAR(entity, str) __attribute__((deprecated(str))) entity

#  define RW_SUPPRESS_DEPRECATED_WARNINGS \
        _Pragma("diag_push") \
        _Pragma("diag_suppress 3444")

#  define RW_RESTORE_DEPRECATED_WARNINGS \
        _Pragma("diag_pop")
#endif

/********************** Intel C++ *********************************/

// Keep this define close to the Intel compiler block
#define RW_NO_ITC

#if defined(__INTEL_COMPILER)

// #  define RW_MSC_BACKEND                          1

#  define RW_NO_NATIVE_GENERIC_H                  1

// Working around a compiler issue  (PR #29003)
#  if __INTEL_COMPILER <= 700 && defined(_LP64)
#    undef  RW_INLINE
#    define RW_INLINE
#  endif

#  if defined(_WIN32)
#    define RW_NO_STATIC_MUTEX_INIT               1
#  endif

#  if defined(_TCHECK)
#    if defined(RW_MULTI_THREAD) && defined(RW_DEBUG)
#      undef RW_NO_ITC
#    endif // RW_MULTI_THREAD && RW_DEBUG
#  endif // _TCHECK

#  if defined(__GNUC__) && (__GNUC__ == 4)
#    if (__GNUC_MINOR__ == 3) || (__GNUC_MINOR__ == 4)
#      if defined(__GXX_EXPERIMENTAL_CXX0X__)
#        define RW_BROKEN_GCC_MATH_TEMPLATE
#      endif
#    endif
#  endif

#  if defined(RW_ENABLE_DEPRECATED_CHECKS)
#    if defined(_MSC_VER)
#      define RW_DEPRECATE_TYPE(str) __declspec(deprecated(str))
#      define RW_DEPRECATE_FUNC(str) __declspec(deprecated(str))
#      define RW_DEPRECATE_VAR(entity, str) __declspec(deprecated(str)) entity
#      define RW_DEPRECATE_MACRO(macro, str) __pragma(deprecated(macro))

#      define RW_SUPPRESS_DEPRECATED_WARNINGS \
            __pragma(warning(push)) \
            __pragma(warning(disable:4995 4996))

#      define RW_RESTORE_DEPRECATED_WARNINGS \
            __pragma(warning(pop))
#    else
#      define RW_DEPRECATE_TYPE(str) __attribute__((deprecated))
#      define RW_DEPRECATE_FUNC(str) __attribute__((deprecated))
#      define RW_DEPRECATE_VAR(entity, str) __attribute__((deprecated)) entity
#      define RW_SUPPRESS_DEPRECATED_WARNINGS \
            _Pragma("warning(push)") \
            _Pragma("warning(disable:1478)")

#      define RW_RESTORE_DEPRECATED_WARNINGS \
            _Pragma("warning(pop)")
#    endif
#  endif

#endif // __INTEL_COMPILER


#if defined(RW_USE_OLD_STRING_CLASSES) && !defined(RW_COPY_ON_WRITE_STRING)
#  define RW_COPY_ON_WRITE_STRING                 1
#endif

#if defined(RW_COPY_ON_WRITE_STRING) && !defined(RW_USE_OLD_STRING_CLASSES)
#  define RW_USE_OLD_STRING_CLASSES               1
#endif

/********************** DOXYGEN ******************************/
#if defined(DOXYGEN)
#  define RW_MACRO_GUARD(macro) macro
#endif

/********************** Bullseye *****************************/
// Disable Bullseye instrumentation of this code to prevent processing
// of the logical && and || operators
#if _BullseyeCoverage
#define RW_ENABLE_BULLSEYE "BullseyeCoverage restore";
#define RW_DISABLE_BULLSEYE "BullseyeCoverage save off";
#else
#  define RW_ENABLE_BULLSEYE /* empty */
#  define RW_DISABLE_BULLSEYE  /* empty */
#endif

//////////////////////////////////////////////////////////
// Different Standard Library Implementation Issues
//////////////////////////////////////////////////////////

#if defined(RW_STDLIB_VER) && RW_STDLIB_VER >= 0x02000000
#  undef  RW_NO_PUBSEEKOFF
#  undef  RW_NO_IO_SENTRY
#  if !defined(RW_NO_NAMESPACE)
#    undef  RW_NO_IOSTD
#  endif
#  if defined(RW_SUNPRO_NATIVE_STDLIB)
#    define RW_NO_NESTED_CLASS_DTOR          1
#    define RW_ALLOC_INTERFACE_STLV2X_HACK   1
#    define RW_3ARG_DISTANCE                 1
#    define RW_BROKEN_STD_DEQUE_ASSIGN_RANGE 1
#    define RW_BROKEN_STD_DEQUE_CTOR_RANGE   1
#    define RW_BROKEN_STD_DEQUE_INSERT_RANGE 1
#    define RW_BROKEN_STD_DEQUE_ASSIGN_RANGE_RANDOM_ACCESS 1
#    define RW_BROKEN_STD_DEQUE_INSERT_RANGE_RANDOM_ACCESS 1
#    define RW_BROKEN_STD_LIST_ASSIGN_RANGE 1
#    define RW_BROKEN_STD_LIST_CTOR_RANGE   1
#    define RW_BROKEN_STD_LIST_INSERT_RANGE 1
#    define RW_BROKEN_STD_LIST_ASSIGN_RANGE_RANDOM_ACCESS 1
#    define RW_BROKEN_STD_LIST_INSERT_RANGE_RANDOM_ACCESS 1
#    define RW_BROKEN_STD_MAP_CTOR_RANGE 1
#    define RW_BROKEN_STD_MAP_INSERT_RANGE 1
#    define RW_BROKEN_STD_MULTIMAP_CTOR_RANGE 1
#    define RW_BROKEN_STD_MULTIMAP_INSERT_RANGE 1
#    define RW_BROKEN_STD_MULTISET_CTOR_RANGE 1
#    define RW_BROKEN_STD_MULTISET_INSERT_RANGE 1
#    define RW_BROKEN_STD_SET_CTOR_RANGE 1
#    define RW_BROKEN_STD_SET_INSERT_RANGE 1
#    define RW_BROKEN_STD_VECTOR_ASSIGN_RANGE 1
#    define RW_BROKEN_STD_VECTOR_CTOR_RANGE   1
#    define RW_BROKEN_STD_VECTOR_INSERT_RANGE 1
#    define RW_BROKEN_STD_VECTOR_ASSIGN_RANGE_RANDOM_ACCESS 1
#    define RW_BROKEN_STD_VECTOR_INSERT_RANGE_RANDOM_ACCESS 1
#  endif
#  if RW_STDLIB_VER <= 0x02020100
#    define RW_BROKEN_STD_DEQUE_ASSIGN_RANGE 1
#    define RW_BROKEN_STD_DEQUE_CTOR_RANGE   1
#    define RW_BROKEN_STD_DEQUE_INSERT_RANGE 1
#    define RW_BROKEN_STD_DEQUE_ASSIGN_RANGE_RANDOM_ACCESS 1
#    define RW_BROKEN_STD_DEQUE_INSERT_RANGE_RANDOM_ACCESS 1
#  endif
#endif

#if defined(RW_STDLIB_VER) && RW_STDLIB_VER == 0x01020100
#  define RW_ALLOCATOR_PLAIN                1
#  define RW_NO_LONG_DOUBLE                 1
#  define RW_ITER_NO_TAGS                   1
#  define RW_3ARG_DISTANCE                  1
#  undef RW_TLS_ALLOCATE
#  define RW_TLS_ALLOCATE(oA,N,type)     oA.allocate(N*sizeof(type),0)
#  define RW_TLS_DEALLOCATE(oA,P,N)      oA.deallocate(P)
#endif

/* allocator interfaces. So the same code in rw/stdex works (always :) */
/* RW_TLS_ALLOCATE(Type2Allocator,Count)     */
/* Other macros may go here as we port to other "standard" libs        */
#ifdef _MSC_VER
#  undef  RW_NO_PUBSEEKOFF
#  define RW_NO_NEW_LOCALE_HEADER
#  define RW_NO_STRINGSTREAM_APPEND
#  define RW_BROKEN_STREAM_INIT
#  undef RW_TLS_ALLOCATE
#  if (defined(_MSC_VER) && (_MSC_VER < 1300)) || defined(_WIN64)
#    define RW_TLS_ALLOCATE(oA,N,ignore)   oA.allocate(N,0)
#  else
#    define RW_TLS_ALLOCATE(oA,N,ignore)   oA.allocate(N)
#  endif
#endif

#if defined(RW_IBMCPP_STDLIB)
#  define RW_NO_STRINGSTREAM_APPEND 1
#endif




#if !defined(RW_TLS_ALLOCATE)
#  define RW_TLS_ALLOCATE(oA,N,ignore)   oA.allocate(N,0)
#endif

#if !defined(RW_TLS_DEALLOCATE)
#  define RW_TLS_DEALLOCATE(oA,P,N)   oA.deallocate(P,N)
#endif

#if !defined(RW_MACRO_GUARD)
#  define RW_MACRO_GUARD(macro) (macro)
#endif

#if !defined(RW_DEPRECATE_TYPE)
#  define RW_DEPRECATE_TYPE(str)
#endif

#if !defined(RW_DEPRECATE_FUNC)
#  define RW_DEPRECATE_FUNC(str)
#endif

#if !defined(RW_DEPRECATE_VAR)
#  define RW_DEPRECATE_VAR(entity, str) entity
#endif

#if !defined(RW_DEPRECATE_ENUMERATOR)
#  define RW_DEPRECATE_ENUMERATOR(entity, str) entity
#endif

#if !defined(RW_DEPRECATE_MACRO)
#  define RW_DEPRECATE_MACRO(macro, str)
#endif

#if !defined(RW_SUPPRESS_DEPRECATED_WARNINGS)
#  define RW_SUPPRESS_DEPRECATED_WARNINGS
#endif

#if !defined(RW_RESTORE_DEPRECATED_WARNINGS)
#  define RW_RESTORE_DEPRECATED_WARNINGS
#endif

#ifndef RW_TOOLS_DEFS_H
#  include <rw/defs.h>
#endif

#endif /* RW_TOOLS_COMPILER_H */
