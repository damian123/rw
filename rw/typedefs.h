#ifndef RW_TOOLS_TYPEDEFS_H
#define RW_TOOLS_TYPEDEFS_H

/**********************************************************************
 *
 *  Type definitions
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/typedefs.h#2 $
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

#include <rw/config/rwconfig_tls.h>      /* Set compiler-specific flags */

// Boolean
typedef bool RWBoolean;

// Integral types
// (type has no padding bits and is most likely a 2's complement)

#if  RW_SIZEOF_CHAR == 1 && RW_SCHAR_MAX == 0x7F

typedef signed char   rwint8;
typedef unsigned char rwuint8;

#endif


#if  RW_SIZEOF_SHORT == 2 && RW_SHORT_MAX == 0x7FFF

typedef signed short int   rwint16;
typedef unsigned short int rwuint16;

#elif  RW_SIZEOF_INT == 2 && RW_INT_MAX == 0x7FFF

typedef signed int   rwint16;
typedef unsigned int rwuint16;

#endif


#if  RW_SIZEOF_SHORT == 4 && RW_SHORT_MAX == 0x7FFFFFFF

typedef signed short int   rwint32;
typedef unsigned short int rwuint32;
#  define RW_INT32_MAX (RW_SHORT_MAX)
#  define RW_INT32_MIN (-RW_SHORT_MAX)
#  define RW_UINT32_MAX (RW_USHORT_MAX)

#elif  RW_SIZEOF_INT == 4 && RW_INT_MAX == 0x7FFFFFFF

typedef signed int   rwint32;
typedef unsigned int rwuint32;
#  define RW_INT32_MAX (RW_INT_MAX)
#  define RW_INT32_MIN (-RW_INT_MAX)
#  define RW_UINT32_MAX (RW_UINT_MAX)

#elif  RW_SIZEOF_LONG_INT == 4 && RW_LONG_MAX == 0x7FFFFFFF

typedef signed long int   rwint32;
typedef unsigned long int rwuint32;
#  define RW_INT32_MAX (RW_LONG_MAX)
#  define RW_INT32_MIN (-RW_LONG_MAX)
#  define RW_UINT32_MAX (RW_ULONG_MAX)

#endif


// Workaround for broken suffix warning in g++ 3.2.x.  (See
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=8594 for more details.)
#define RW_LIMIT_CHECK(name, value) name == value
#if defined(__GNUG__) && !defined(__INTEL_COMPILER)
#  if (__GNUG__ == 3) && (__GNUC_MINOR__ == 2)
#    undef  RW_LIMIT_CHECK
#    define RW_LIMIT_CHECK(name, value) 1
#  endif // (__GNUG__ == 3) && (__GNUC_MINOR__ == 2)
#endif // defined(__GNUG__) && !defined (__INTEL_COMPILER)


#if  RW_SIZEOF_SHORT == 8 \
     && RW_LIMIT_CHECK(RW_SHORT_MAX, 0x7FFFFFFFFFFFFFFFLL)

typedef signed short int   rwint64;
typedef unsigned short int rwuint64;

#elif  RW_SIZEOF_INT == 8 \
     && RW_LIMIT_CHECK(RW_INT_MAX, 0x7FFFFFFFFFFFFFFFLL)

typedef signed int   rwint64;
typedef unsigned int rwuint64;

#elif  RW_SIZEOF_LONG_LONG_INT == 8 \
       && RW_LIMIT_CHECK(RW_LLONG_MAX, 0x7FFFFFFFFFFFFFFFLL)

#  if defined(_MSC_VER) && _MSC_VER <= 1200

typedef __int64 rwint64;
typedef unsigned __int64 rwuint64;

#  else

typedef signed long long int   rwint64;
typedef unsigned long long int rwuint64;

#  endif // _MSC_VER && _MSC_VER <= 1200

#elif  RW_SIZEOF_LONG_INT == 8 \
       && RW_LIMIT_CHECK(RW_LONG_MAX, 0x7FFFFFFFFFFFFFFFLL)

typedef signed long int   rwint64;
typedef unsigned long int rwuint64;

#else

#  error No 64-bit integral type available.

#endif

#undef RW_LIMIT_CHECK

#endif /* RW_TOOLS_TYPEDEFS_H */
