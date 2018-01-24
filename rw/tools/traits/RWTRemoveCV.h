#ifndef RW_TOOLS_TRAITS_RWTREMOVECV_H
#define RW_TOOLS_TRAITS_RWTREMOVECV_H

/***************************************************************************
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
 * $Id: //tools/13/rw/tools/traits/RWTRemoveCV.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWTRemoveConst.h>
#include <rw/tools/traits/RWTRemoveVolatile.h>

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Removes cv-qualification from a type
 *
 * Specifies a typedef \c type to \c T with all cv-qualifications
 * removed. If \c T is a "<tt>const T1</tt>", "<tt>volatile T1</tt>", or
 * "<tt>const volatile T1</tt>", \c type is a typedef to \c T1; otherwise
 * \c type is a typedef to \c T.
 *
 * @section example Example
 * @code
 * RWTRemoveCV<int>::type                 // -> int
 * RWTRemoveCV<const int>::type           // -> int
 * RWTRemoveCV<volatile int>::type        // -> int
 * RWTRemoveCV<const volatile int>::type  // -> int
 * RWTRemoveCV<const int*>::type          // -> const int*
 * RWTRemoveCV<int* const>::type          // -> int*
 * RWTRemoveCV<const int&>::type          // -> const int&
 * RWTRemoveCV<const int[]>::type         // -> int[]
 * @endcode
 */
template <typename T>
struct RWTRemoveCV {
    /**
     * Specifies a typedef to \c T with all cv-qualifications removed.
     */
#if DOXYGEN
    typedef unspecified_type type;
#else
    typedef typename RWTRemoveConst<typename RWTRemoveVolatile<T>::type>::type type;
#endif
};

#endif
