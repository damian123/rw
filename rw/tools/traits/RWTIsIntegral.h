#ifndef RW_TOOLS_TRAITS_RWTISINTEGRAL_H
#define RW_TOOLS_TRAITS_RWTISINTEGRAL_H

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
 * $Id: //tools/13/rw/tools/traits/RWTIsIntegral.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWFalseType.h>
#include <rw/tools/traits/RWTRemoveCV.h>
#include <rw/tools/traits/RWTrueType.h>

template <typename T>
struct RWTIsIntegralImp : public RWFalseType {};

template <>
struct RWTIsIntegralImp<bool> : public RWTrueType {};

template <>
struct RWTIsIntegralImp<char> : public RWTrueType {};

template <>
struct RWTIsIntegralImp<wchar_t> : public RWTrueType {};

template <>
struct RWTIsIntegralImp<signed char> : public RWTrueType {};

template <>
struct RWTIsIntegralImp<short> : public RWTrueType {};

template <>
struct RWTIsIntegralImp<int> : public RWTrueType {};

template <>
struct RWTIsIntegralImp<long> : public RWTrueType {};

template <>
struct RWTIsIntegralImp<long long> : public RWTrueType {};

template <>
struct RWTIsIntegralImp<unsigned char> : public RWTrueType {};

template <>
struct RWTIsIntegralImp<unsigned short> : public RWTrueType {};

template <>
struct RWTIsIntegralImp<unsigned int> : public RWTrueType {};

template <>
struct RWTIsIntegralImp<unsigned long> : public RWTrueType {};

template <>
struct RWTIsIntegralImp<unsigned long long> : public RWTrueType {};

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Determines if a type is an integral type
 *
 * If \c T is a potentially cv-qualified integral type, derives from
 * RWTrueType; otherwise derives from RWFalseType.
 *
 * @section example Example
 * @code
 * class C {};
 *
 * RWTIsIntegral<int>::value        // -> true
 * RWTIsIntegral<const int>::value  // -> true
 * RWTIsIntegral<C>::value          // -> false
 * @endcode
 */
template <typename T>
struct RWTIsIntegral : public RWTIsIntegralImp<typename RWTRemoveCV<T>::type> {
};

#endif
