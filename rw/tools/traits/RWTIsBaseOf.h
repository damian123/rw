#ifndef RW_TOOLS_TRAITS_RWTISBASEOF_H
#define RW_TOOLS_TRAITS_RWTISBASEOF_H

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
 * $Id: //tools/13/rw/tools/traits/RWTIsBaseOf.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWFalseType.h>
#include <rw/tools/traits/RWNoType.h>
#include <rw/tools/traits/RWTrueType.h>
#include <rw/tools/traits/RWYesType.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  include <rw/tools/traits/RWTIsUnion.h>
#  define RW_IS_BASE_OF(base, derived) (__is_base_of(base, derived) && !RWTIsUnion<base>::value)
#elif defined(RW_GCC_VERSION) && (RW_GCC_VERSION >= 40300)
#  define RW_IS_BASE_OF(base, derived) __is_base_of(base, derived)
#elif defined(__IBMCPP__) && (__IBMCPP__ >= 0x1110)
#  define RW_IS_BASE_OF(base, derived) _Traits_is_base_of(base, derived)
#else
#  define RW_NO_PRIMITIVE_IS_BASE_OF
#endif

#if !defined(RW_NO_PRIMITIVE_IS_BASE_OF)
template <typename B, typename D, bool = RW_IS_BASE_OF(B, D)>
struct RWTIsBaseOfImp : public RWFalseType {
};

#else
#  include <rw/tools/traits/RWTIsClass.h>
#  include <rw/tools/traits/RWTIsSame.h>
#  include <rw/tools/traits/RWTRemoveCV.h>

template <typename B, typename D>
struct RWTIsBaseOfConvertible {
    operator B* () const;
    operator D* ();
};

template < typename B, typename D, bool = RWTIsClass<B>::value && RWTIsClass<D>::value >
struct RWTIsBaseOfHelper : public RWFalseType {
};

template <typename B, typename D>
struct RWTIsBaseOfHelper<B, D, true> {
    typedef typename RWTRemoveCV<B>::type Base;
    typedef typename RWTRemoveCV<D>::type Derived;

    template <typename T>
    static RWYesType check(Derived*, T);
    static RWNoType check(Base*, int);

    static const bool value = RWTIsSame<Base, Derived>::value ||
                              sizeof(check(RWTIsBaseOfConvertible<Base, Derived>(), int())) == sizeof(RWYesType);
};

template <typename B, typename D, bool = RWTIsBaseOfHelper<B, D>::value>
struct RWTIsBaseOfImp : public RWFalseType {
};

#endif

template <typename B, typename D>
struct RWTIsBaseOfImp<B, D, true> : public RWTrueType {
};

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Determines if a type is a base of another type
 *
 * If \c B is a base class of \c D, or if \c B and \c D are the same non-union
 * class type, regardless of cv-qualifiers, derives from RWTrueType; otherwise
 * derives from RWFalseType.
 *
 * @note
 * Not all compilers support distinguishing between union and non-union
 * class types. On those platforms, RWTIsUnion will always derive from
 * RWFalseType, and RWTIsBaseOf will not be able to distinguish between
 * union and non-union types. In order to prevent this, RWTIsUnion must be
 * specialized for union types to derive from RWTrueType. The macro
 * \c RW_NO_PRIMITIVE_IS_UNION can be checked to determine if this
 * specialization is required.
 *
 * @section example Example
 * @code
 * class C {};
 * class D : private C {};
 * struct S {};
 * union U {};
 *
 * RWTIsBaseOf<C, C>::value        // -> true
 * RWTIsBaseOf<C, D>::value        // -> true
 * RWTIsBaseOf<C, const D>::value  // -> true
 * RWTIsBaseOf<D, C>::value        // -> false
 * RWTIsBaseOf<C*, D*>::value      // -> false
 * RWTIsBaseOf<C&, D&>::value      // -> false
 * RWTIsBaseOf<U, U>::value        // -> false
 * RWTIsBaseOf<int, int>::value    // -> false
 * @endcode
 */
template <typename B, typename D>
struct RWTIsBaseOf : public RWTIsBaseOfImp<B, D> {
};

#endif
