#ifndef RW_TOOLS_TRAITS_RWTISCLASS_H
#define RW_TOOLS_TRAITS_RWTISCLASS_H

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
 * $Id: //tools/13/rw/tools/traits/RWTIsClass.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWFalseType.h>
#include <rw/tools/traits/RWNoType.h>
#include <rw/tools/traits/RWTrueType.h>
#include <rw/tools/traits/RWYesType.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  define RW_IS_CLASS(type) __is_class(type)
#elif defined(RW_GCC_VERSION) && (RW_GCC_VERSION >= 40300)
#  define RW_IS_CLASS(type) __is_class(type)
#elif defined(__IBMCPP__) && (__IBMCPP__ >= 0x1110)
#  define RW_IS_CLASS(type) _Traits_is_class(type)
#else
#  define RW_NO_PRIMITIVE_IS_CLASS
#endif

#if !defined(RW_NO_PRIMITIVE_IS_CLASS)
template <typename T, bool = RW_IS_CLASS(T)>
struct RWTIsClassImp : public RWFalseType {
};

#else
#  include <rw/tools/traits/RWTIsUnion.h>

template <typename T>
struct RWTAllowsMemberFunctionPtr {

    template <typename U>
    static RWYesType check(void(U::*)(void));

    template <typename U>
    static RWNoType check(...);

    static const bool value = (sizeof(check<T>(0)) == sizeof(RWYesType));
};

template < typename T, bool = RWTAllowsMemberFunctionPtr<T>::value && !RWTIsUnion<T>::value >
struct RWTIsClassImp : public RWFalseType {
};
#endif

template <typename T>
struct RWTIsClassImp<T, true> : public RWTrueType {
};

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Determines if a type is a non-union class type
 *
 * If \c T is a potentially cv-qualified non-union class type, derives from
 * RWTrueType; otherwise derives from RWFalseType.
 *
 * @note
 * Not all compilers support distinguishing between union and non-union
 * class types. On those platforms, RWTIsUnion will always derive from
 * RWFalseType, and RWTIsClass will derive from RWTrueType, even for union
 * types. In order to prevent this, RWTIsUnion must be specialized for
 * union types to derive from RWTrueType. The macro \c RW_NO_PRIMITIVE_IS_UNION
 * can be checked to determine if this specialization is required.
 *
 * @section example Example
 * @code
 * class C {};
 * struct S {};
 * union U {};
 *
 * RWTIsClass<C>::value        // -> true
 * RWTIsClass<const C>::value  // -> true
 * RWTIsClass<C*>::value       // -> false
 * RWTIsClass<C&>::value       // -> false
 * RWTIsClass<S>::value        // -> true
 * RWTIsClass<U>::value        // -> false
 * @endcode
 */
template <typename T>
struct RWTIsClass : public RWTIsClassImp<T> {
};

#endif
