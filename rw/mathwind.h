#ifndef __RW_MATHWIND_H__
#define __RW_MATHWIND_H__

/*************************************************************************
 * $Id: //math/13/rw/mathwind.h#1 $
 *
 *
 * Copyright (c) 1991-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 ************************************************************************/

#include <rw/compiler.h>
#include <rw/config/rwconfig_mth.h> // need copyright strings, etc

// Define dll macros

#ifdef RWDLL
#  ifndef _RWTOOLSDLL
#    define _RWTOOLSDLL
#  endif
#  ifndef _RWMATHDLL
#    define _RWMATHDLL
#  endif
#endif

#if defined(_RWMATHDLL)
#  ifndef _RWTOOLSDLL
#    define _RWTOOLSDLL
#  endif
#endif

/*
 * Check for Microsoft C/C++ and massage as necessary.
 */
#if defined(_MSC_VER)
#  if defined(_WINDLL) && !defined(__DLL__)
#    define __DLL__ 1
#  endif
#endif

// The Math DLL related defines.
#if defined(RW_SHARED)
#  if defined(_RWBUILD_mth)
// Compiling the Math DLL.
#    define RW_MATH_GLOBAL          RW_COMPILER_GLOBAL_EXPORT
#    define RW_MATH_SYMBOLIC        RW_COMPILER_SYMBOLIC_EXPORT
#  else
#    define RW_MATH_GLOBAL          RW_COMPILER_GLOBAL_IMPORT
#    define RW_MATH_SYMBOLIC        RW_COMPILER_GLOBAL_IMPORT
#  endif
#else
// Neither compiling, nor using the Math DLL.
#  define RW_MATH_GLOBAL
#  define RW_MATH_SYMBOLIC
#endif

#endif
