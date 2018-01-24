#ifndef RW_TOOLS_TRAITS_RWTADDCONST_H
#define RW_TOOLS_TRAITS_RWTADDCONST_H

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
 * $Id: //tools/13/rw/tools/traits/RWTAddConst.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWTIsReference.h>
#if defined(__IBMCPP__) && (__IBMCPP__ <= 0x1110)
#  include <rw/tools/traits/RWTIsFunction.h>
#endif

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
#  pragma warning(push,3)
#  pragma warning(disable: 4180)
#endif

#if defined(__IBMCPP__) && (__IBMCPP__ <= 0x1110)
template < typename T, bool = RWTIsReference<T>::value || RWTIsFunction<T>::value >
#else
template <typename T, bool = RWTIsReference<T>::value>
#endif
struct RWTAddConstImp {
    typedef const T type;
};

template <typename T>
struct RWTAddConstImp<T, true> {
    typedef T type;
};

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Adds \c const qualification to a type
 *
 * Specifies a typedef \c type to a <tt>const</tt>-qualified
 * version of \c T. If \c T is a function, reference, or is already
 * \c const, \c type will be a typedef to \c T.
 *
 * @section example Example
 * @code
 * RWTAddConst<int>::type                 // -> const int
 * RWTAddConst<const int>::type           // -> const int
 * RWTAddConst<volatile int>::type        // -> const volatile int
 * RWTAddConst<const volatile int>::type  // -> const volatile int
 * RWTAddConst<int*>::type                // -> int* const
 * RWTAddConst<int&>::type                // -> int&
 * RWTAddConst<int(int)>::type            // -> int(int)
 * @endcode
 */
template <typename T>
struct RWTAddConst {
    /**
     * A <tt>const</tt>-qualified version of \c T
     */
#if defined(DOXYGEN)
    typedef unspecified_type type;
#else
    typedef typename RWTAddConstImp<T>::type type;
#endif
};

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
#  pragma warning(pop)
#endif

#endif
