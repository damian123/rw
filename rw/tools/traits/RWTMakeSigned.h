#ifndef RW_TOOLS_TRAITS_RWTMAKESIGNED_H
#define RW_TOOLS_TRAITS_RWTMAKESIGNED_H

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
 * $Id: //tools/13/rw/tools/traits/RWTMakeSigned.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWTAddConst.h>
#include <rw/tools/traits/RWTAddVolatile.h>
#include <rw/tools/traits/RWTIsConst.h>
#include <rw/tools/traits/RWTIsIntegral.h>
#include <rw/tools/traits/RWTIsSame.h>
#include <rw/tools/traits/RWTIsVolatile.h>
#include <rw/tools/traits/RWTRemoveCV.h>

template <typename T, size_t = sizeof(T)>
struct RWTSignedType;

template <typename T>
struct RWTSignedType<T, sizeof(signed char)> {
    typedef signed char type;
};

template <typename T>
struct RWTSignedType<T, sizeof(short)> {
    typedef short type;
};

template <typename T>
struct RWTSignedType<T, sizeof(int)> {
    typedef int type;
};

#if (RW_SIZEOF_LONG_INT > RW_SIZEOF_INT)
template <typename T>
struct RWTSignedType<T, sizeof(long)> {
    typedef long type;
};
#endif

#if (RW_SIZEOF_LONG_LONG_INT > RW_SIZEOF_LONG_INT)
template <typename T>
struct RWTSignedType<T, sizeof(long long)> {
    typedef long long type;
};
#endif

template <>
struct RWTSignedType<signed char> {
    typedef signed char type;
};

template <>
struct RWTSignedType<unsigned char> {
    typedef signed char type;
};

template <>
struct RWTSignedType<short> {
    typedef short type;
};

template <>
struct RWTSignedType<unsigned short> {
    typedef short type;
};

template <>
struct RWTSignedType<int> {
    typedef int type;
};

template <>
struct RWTSignedType<unsigned int> {
    typedef int type;
};

template <>
struct RWTSignedType<long> {
    typedef long type;
};

template <>
struct RWTSignedType<unsigned long> {
    typedef long type;
};

template <>
struct RWTSignedType<long long> {
    typedef long long type;
};

template <>
struct RWTSignedType<unsigned long long> {
    typedef long long type;
};

template < typename T, bool = RWTIsIntegral<T>::value && !RWTIsSame<typename RWTRemoveCV<T>::type, bool>::value, bool = RWTIsConst<T>::value, bool = RWTIsVolatile<T>::value >
struct RWTMakeSignedImp;

template <typename T>
struct RWTMakeSignedImp<T, true, false, false> {
    typedef typename RWTSignedType<T>::type type;
};

template <typename T>
struct RWTMakeSignedImp<T, true, true, false> {
    typedef typename RWTAddConst<typename RWTSignedType<typename RWTRemoveConst<T>::type>::type>::type type;
};

template <typename T>
struct RWTMakeSignedImp<T, true, false, true> {
    typedef typename RWTAddVolatile<typename RWTSignedType<typename RWTRemoveVolatile<T>::type>::type>::type type;
};

template <typename T>
struct RWTMakeSignedImp<T, true, true, true> {
    typedef typename RWTAddConst<typename RWTAddVolatile<typename RWTSignedType<typename RWTRemoveCV<T>::type>::type>::type>::type type;
};

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Specifies a \c signed type of equivalent rank
 *
 * Specifies a typedef \c type that is a \c signed type of equivalent rank
 * to \c T. \c T must be an integral type other than \c bool.
 *
 * @note
 * This class differs from std::make_signed in that it does not support enum
 * types.
 *
 * @section example Example
 * @code
 * RWTMakeSigned<char>::type           // -> signed char
 * RWTMakeSigned<signed char>::type    // -> signed char
 * RWTMakeSigned<unsigned char>::type  // -> signed char
 * @endcode
 */
template <typename T>
struct RWTMakeSigned : RWTMakeSignedImp<T> {
#if defined(DOXYGEN)
    /**
     * Specifies a typedef to a \c signed_type with the same rank as \c T.
     */
    typedef signed_type type;
#endif
};

#endif
