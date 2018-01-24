#ifndef RW_TOOLS_TRAITS_RWTREMOVEREFERENCE_H
#define RW_TOOLS_TRAITS_RWTREMOVEREFERENCE_H

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
 * $Id: //tools/13/rw/tools/traits/RWTRemoveReference.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWTIsLvalueReference.h>
#include <rw/tools/traits/RWTIsRvalueReference.h>

#if !defined(RW_NO_RVALUE_REFERENCES)
template < typename T,
         bool = RWTIsLvalueReference<T>::value,
         bool = RWTIsRvalueReference<T>::value >
struct RWTRemoveReferenceImp {
    typedef T type;
};

template <typename T>
struct RWTRemoveReferenceImp<T&, true, false> {
    typedef T type;
};

template <typename T>
struct RWTRemoveReferenceImp < T && , false, true > {
    typedef T type;
};
#else
template <typename T>
struct RWTRemoveReferenceImp {
    typedef T type;
};

template <typename T>
struct RWTRemoveReferenceImp<T&> {
    typedef T type;
};
#endif

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Removes an lvalue or rvalue reference from a type
 *
 * Specifies a typedef to \c T1 if \c T is "a reference to \c T1";
 * otherwise \c T.
 *
 * @section example Example
 * @code
 * RWTRemoveReference<int>::type            // -> int
 * RWTRemoveReference<int&>::type           // -> int
 * RWTRemoveReference<int&&>::type          // -> int
 * RWTRemoveReference<int*>::type           // -> int*
 * RWTRemoveReference<int(&)[]>::type       // -> int[]
 * RWTRemoveReference<int(&&)[]>::type      // -> int[]
 * RWTRemoveReference<int(&)(void)>::type   // -> int(void)
 * RWTRemoveReference<int(&&)(void)>::type  // -> int(void)
 * @endcode
 */
template <typename T>
struct RWTRemoveReference {
    /**
     * Specifies a typedef to \c T1 if \c T is "a reference to \c T1";
     * otherwise \c T.
     */
#if DOXYGEN
    typedef unspecified_type type;
#else
    typedef typename RWTRemoveReferenceImp<T>::type type;
#endif
};

#endif
