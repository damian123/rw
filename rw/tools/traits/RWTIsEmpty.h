#ifndef RW_TOOLS_TRAITS_RWTISEMPTY_H
#define RW_TOOLS_TRAITS_RWTISEMPTY_H

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
 * $Id: //tools/13/rw/tools/traits/RWTIsEmpty.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWFalseType.h>
#include <rw/tools/traits/RWTIsClass.h>
#include <rw/tools/traits/RWTRemoveCV.h>
#include <rw/tools/traits/RWTrueType.h>

template <typename T, bool = RWTIsClass<T>::value>
struct RWTEmptyBase {
    struct X {
        int val_;
    };
    struct Y : private RWTRemoveCV<T>::type {
        int val_;
    };

    static const bool value = (sizeof(X) == sizeof(Y));
};

template <typename T>
struct RWTEmptyBase<T, false> {
    static const bool value = false;
};

template < typename T, bool = RWTEmptyBase<T>::value>
struct RWTIsEmptyImp : public RWFalseType {
};

template <typename T>
struct RWTIsEmptyImp<T, true> : public RWTrueType {
};

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Determines if a type is an empty class type
 *
 * If \c T is a non-union class type with no non-static data members and no
 * virtual functions, derives from RWTrueType; otherwise derives from
 * RWFalseType.
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
 * class E {};
 * class C { int val_; };
 *
 * RWTIsEmpty<E>::value        // -> true
 * RWTIsEmpty<const E>::value  // -> true
 * RWTIsClass<E*>::value       // -> false
 * RWTIsClass<E&>::value       // -> false
 * RWTIsClass<C>::value        // -> false
 * @endcode
 */
template <typename T>
struct RWTIsEmpty : public RWTIsEmptyImp<T> {
};

#endif
