#ifndef RW_TOOLS_TRAITS_RWTISUNION_H
#define RW_TOOLS_TRAITS_RWTISUNION_H

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
 * $Id: //tools/13/rw/tools/traits/RWTIsUnion.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWFalseType.h>
#include <rw/tools/traits/RWTrueType.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  define RW_IS_UNION(type) __is_union(type)
#elif defined(RW_GCC_VERSION) && (RW_GCC_VERSION >= 40300)
#  define RW_IS_UNION(type) __is_union(type)
#elif defined(__IBMCPP__) && (__IBMCPP__ >= 0x1110)
#  define RW_IS_UNION(type) _Traits_is_union(type)
#else
#  define RW_NO_PRIMITIVE_IS_UNION
#endif

#if !defined(RW_NO_PRIMITIVE_IS_UNION)
template <typename T, bool = RW_IS_UNION(T)>
struct RWTIsUnionImp : public RWFalseType {
};

template <typename T>
struct RWTIsUnionImp<T, true> : public RWTrueType {
};

#else
template <typename T>
struct RWTIsUnionImp : public RWFalseType {
};
#endif

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Determines if a type is a union class type
 *
 * If \c T is a potentially cv-qualified union class type, derives from
 * RWTrueType; otherwise derives from RWFalseType.
 *
 * @note
 * Not all compilers support distinguishing between union and non-union
 * class types. On those platforms, RWTIsUnion will always derive from
 * RWFalseType. In order to prevent this, RWTIsUnion must be specialized for
 * union types to derive from RWTrueType. The macro \c RW_NO_PRIMITIVE_IS_UNION
 * can be checked to determine if this specialization is required.
 *
 * @section example Example
 * @code
 * class C {};
 * struct S {};
 * union U {};
 *
 * RWTIsUnion<C>::value        // -> true
 * RWTIsUnion<const C>::value  // -> true
 * RWTIsUnion<C*>::value       // -> false
 * RWTIsUnion<C&>::value       // -> false
 * RWTIsUnion<S>::value        // -> true
 * RWTIsUnion<U>::value        // -> false
 * @endcode
 */
template <typename T>
struct RWTIsUnion : public RWTIsUnionImp<T> {
};

#endif
