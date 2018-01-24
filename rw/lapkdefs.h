#ifndef __RWLAPKDEFS_H__
#define __RWLAPKDEFS_H__

/*
 * Linear Algebra Module common definitions
 *
 * $Id: //math/13/rw/lapkdefs.h#1 $
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
 */

#include <rw/mathdefs.h>

/*
 *     W I N D O W S - S P E C I F I C   C O D E

 * Deal with DLL related issues. Note that rwwind.h and mathwind.h have
 * already been and defines issues related to the Essential Tools
 * components used in Essential Math and Linear Algebra Modules
 */
#include <rw/lapkwind.h>

// Define compatibility macro
#define RWLAPACK     RW_LAPACK_VERSION

// VC 7.0 requires explicit instantiation of
// some templates or
// we will get get unresolved symbols in the tests.
// I have left the above code in, but it appears to be
// obsolete and not used anywhere.  I have stolen the
// code below from stdlib 2.x.

// VC 9.0 also requires explicit instantiation.
// So, changing the condition to _MSC_VER <= 1500.

#define RWLAPK_NO_EXPLICIT_INSTANTIATION 1
#define RWLAPK_NO_INSTANTIATE

#if defined(_MSC_VER) && (_MSC_VER <= 1500)
#  undef RWLAPK_NO_EXPLICIT_INSTANTIATION
#  undef RWLAPK_NO_INSTANTIATE
#endif

#if  defined(RW_LAPK_INSTANTIATE_TEMPLATES)         \
    && !defined (RWLAPK_NO_EXPLICIT_INSTANTIATION)   \
    && !defined (RWLAPK_NO_INSTANTIATE)
#  define RWLAPK_INSTANTIATE_1(arg)          template arg
#  define RWLAPK_INSTANTIATE_2(a1, a2)       template a1, a2
#  define RWLAPK_INSTANTIATE_3(a1, a2, a3)   template a1, a2, a3
#else
#  if    defined(_MSC_VER)                          \
      && !defined (RWLAPK_NO_EXPLICIT_INSTANTIATION) \
      && !defined (RWLAPK_NO_INSTANTIATE)
#    define RWLAPK_INSTANTIATE_1(arg)        extern template arg
#    define RWLAPK_INSTANTIATE_2(a1, a2)     extern template a1, a2
#    define RWLAPK_INSTANTIATE_3(a1, a2, a3) extern template a1, a2, a3
#  else
#    define RWLAPK_INSTANTIATE_1(ignore)           \
            typedef void __rw_unused_typedef
#    define RWLAPK_INSTANTIATE_2(ign1, ign2)       \
            typedef void __rw_unused_typedef
#    define RWLAPK_INSTANTIATE_3(ign1, ign2, ign3) \
            typedef void __rw_unused_typedef
#  endif
#endif   // RWLAPK_INSTANTIATE_TEMPLATES

template <class T> class RWMathVec;
typedef RWMathVec<rw_lapack_select_t>   RWSelectVec;

#endif
