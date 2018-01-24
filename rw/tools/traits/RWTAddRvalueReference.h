#ifndef RW_TOOLS_TRAITS_RWTADDRVALUEREFERENCE_H
#define RW_TOOLS_TRAITS_RWTADDRVALUEREFERENCE_H

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
 * $Id: //tools/13/rw/tools/traits/RWTAddRvalueReference.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWTIsReference.h>
#include <rw/tools/traits/RWTIsVoid.h>

#if !defined(RW_NO_RVALUE_REFERENCES) || defined(DOXYGEN)
template < typename T, bool = RWTIsReference<T>::value || RWTIsVoid<T>::value >
struct RWTAddRvalueReferenceImp {
    typedef T && type;
};

template <typename T>
struct RWTAddRvalueReferenceImp<T, true> {
    typedef T type;
};

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Adds an rvalue reference to a type
 *
 * Specifies a typedef \c type to an rvalue reference version of \c T. If
 * \c T is an object or function type, sets \c type to \c T&&; otherwise
 * sets \c type to \c T.
 *
 * @section example Example
 * @code
 * RWTAddLvalueReference<int>::type       // -> int&&
 * RWTAddLvalueReference<int*>::type      // -> int*&&
 * RWTAddLvalueReference<int&>::type      // -> int&
 * RWTAddLvalueReference<int&&>::type     // -> int&&
 * RWTAddLvalueReference<int(int)>::type  // -> int(&&)(int)
 * @endcode
 */
template <typename T>
struct RWTAddRvalueReference {
    /**
     * An rvalue reference type for \c T.
     */
#  if defined(DOXYGEN)
    typedef unspecified_type type;
#  else
    typedef typename RWTAddRvalueReferenceImp<T>::type type;
#  endif
};

#endif

#endif
