#ifndef RW_CURRENCY_DCMLDEFS_H
#define RW_CURRENCY_DCMLDEFS_H

/***************************************************************************
 * Macro definitions for the RWDecimal classes
 *
 * $Id: //money/13/rw/dcmldefs.h#1 $
 *
 * Copyright (c) 1993-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 **************************************************************************/

/* for backward compatibility */
#ifndef RW_MONEY_DCMLDEFS_H
#  define RW_MONEY_DCMLDEFS_H
#endif

#include <rw/config/rwconfig_mny.h>

// Define legacy macro
#define RWDCML     RW_CURRENCY_VERSION

#if !defined(__RWDEFS_H__)
#  include <rw/defs.h>
#endif


#if (defined(__SUNPRO_CC) && ((RWSTDLIBTYPE != 0) || (defined(_STLPORT_VERSION))) && (__SUNPRO_CC >= 0x500))
#  define RW_DCML_MATH_FUNCTIONS_IN_STD  1
#endif

#if defined(__SUNPRO_CC)
#  define RW_MONEY_COMPILE_INSTANTIATE 1
#else
#  ifdef RW_COMPILE_INSTANTIATE
#    define RW_MONEY_COMPILE_INSTANTIATE 1
#  endif
#endif

// Set the product specific build macro if appropriate.
#if defined(RW_SHARED) && defined(_RWBUILD_mny)
#  define _RWDCMLBUILDDLL
#endif


// This works around a bug in the aix/gcc compiler.  The first
// set is used mainly in utlconst header files.  The other set
// is used to allow initialization of method/group data members
//  which are normally const and initialized in the header files.
// In only aix/gcc builds, these data members still needed to be
// initialized later, as can be seem in the examples.
#if defined(_AIX) && defined(__GNUG__) && (__GNUG__ == 2 && __GNU_MINOR__ < 96)
#  define RW_MNY_NO_EXPLICIT_INSTANTIATION 1
#  define RW_MNY_STATIC_CONST_DEL static
#  define RW_MNY_STATIC_CONST_DEF
#else
#  if defined(_MSC_VER) || defined(_AIX) || defined(__GNUG__) || (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x500)
#    define RW_MNY_STATIC_CONST_DEL static
#    define RW_MNY_STATIC_CONST_DEF
#  else
#    define RW_MNY_STATIC_CONST_DEL static const
#    define RW_MNY_STATIC_CONST_DEF const
#  endif
#endif

#if !defined(RW_NO_NEW_TEMPLATE_SYNTAX)
#  if !defined(_MSC_VER) || (_MSC_VER >= 1310)
#    define RW_MONEY_NEW_TEMPLATE_SYNTAX template<>
#  else // VC 6.0
#    define RW_MONEY_NEW_TEMPLATE_SYNTAX
#  endif
#endif

#if defined(_MSC_VER) && !(defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1200)
#  define RW_NO_STATIC_TEMPLATE_FUNCTION_PTR_INITIALIZER 1
#endif

#if defined(RWMNY_NO_LONG_DOUBLE_MATH)
// Some compilers (I won't specify names intentionally, but it is HP aCC++ 3.30)
// support only basic operations over long double. We need more complicated
// functionality. Convert long double to double.
#  define floorl(x) floor(double(x))
#  define fmodl(x, y) fmod(double(x), double(y))
#endif

#if defined(RW_NO_NEW_HEADER)
#  define RW_MNY_NO_NEW_HEADER 1
#endif

#ifdef RW_DCML_MATH_FUNCTIONS_IN_STD
#  define RWMNY_STD std
#else
#  define RWMNY_STD
#endif

#if  defined(RW_DCML_INSTANTIATE_TEMPLATES)         \
    && !defined (RWDCML_NO_EXPLICIT_INSTANTIATION)   \
    && !defined (RWDCML_NO_INSTANTIATE)
#  define RWDCML_INSTANTIATE_1(arg)          template arg
#  define RWDCML_INSTANTIATE_2(a1, a2)       template a1, a2
#  define RWDCML_INSTANTIATE_3(a1, a2, a3)   template a1, a2, a3
#else
#  if    defined(_MSC_VER) && (_MSC_VER >=1300)     \
      && !defined (RWDCML_NO_EXPLICIT_INSTANTIATION) \
      && !defined (RWDCML_NO_INSTANTIATE)
#    pragma warning (disable: 4231)
#    define RWDCML_INSTANTIATE_1(arg)        extern template arg
#    define RWDCML_INSTANTIATE_2(a1, a2)     extern template a1, a2
#    define RWDCML_INSTANTIATE_3(a1, a2, a3) extern template a1, a2, a3
#  else
#    define RWDCML_INSTANTIATE_1(ignore)           \
            typedef void __rw_unused_typedef
#    define RWDCML_INSTANTIATE_2(ign1, ign2)       \
            typedef void __rw_unused_typedef
#    define RWDCML_INSTANTIATE_3(ign1, ign2, ign3) \
            typedef void __rw_unused_typedef
#  endif
#endif   // RWDCML_INSTANTIATE_TEMPLATES

#endif //  RW_CURRENCY_DCMLDEFS_H
