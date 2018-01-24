#ifndef RW_TOOLS_TRAITS_RWTADDCV
#define RW_TOOLS_TRAITS_RWTADDCV

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
 * $Id: //tools/13/rw/tools/traits/RWTAddCV.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWTAddConst.h>
#include <rw/tools/traits/RWTAddVolatile.h>

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Adds <tt>const volatile</tt> qualifications to a type
 *
 * Specifies a typedef \c type to a <tt>const volatile</tt>-qualified
 * version of \c T. If \c T is a function, reference, or is already
 * <tt>const volatile</tt>, \c type will be a typedef to \c T.
 *
 * @section example Example
 * @code
 * RWTAddCV<int>::type                 // -> const volatile int
 * RWTAddCV<const int>::type           // -> const volatile int
 * RWTAddCV<volatile int>::type        // -> const volatile int
 * RWTAddCV<const volatile int>::type  // -> const volatile int
 * RWTAddCV<int*>::type                // -> int* const volatile
 * RWTAddCV<int&>::type                // -> int&
 * RWTAddCV<int(int)>::type            // -> int(int)
 * @endcode
 */
template <typename T>
struct RWTAddCV {
    /**
     * A <tt>const volatile</tt>-qualified version of \c T
     */
#if DOXYGEN
    typedef unspecified_type type;
#else
    typedef typename RWTAddConst<typename RWTAddVolatile<T>::type>::type type;
#endif
};

#endif
