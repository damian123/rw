#ifndef RW_TOOLS_TRAITS_RWTISCONVERTIBLE_H
#define RW_TOOLS_TRAITS_RWTISCONVERTIBLE_H

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
 * $Id: //tools/13/rw/tools/traits/RWTIsConvertible.h#3 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWFalseType.h>
#include <rw/tools/traits/RWTrueType.h>
#include <rw/tools/traits/RWTIsVoid.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1500 && _MSC_VER <= 1600)
#  include <type_traits>
#  define RW_IS_CONVERTIBLE(from, to) std::tr1::is_convertible<from, to>::value
#elif defined(_MSC_VER) && (_MSC_VER >= 1600)
#  include <type_traits>
#  if defined(__INTEL_COMPILER) && (__INTEL_COMPILER == 1400)
#    define RW_IS_CONVERTIBLE(from, to) (std::is_convertible<from, to>::value || \
                                         (RWTIsVoid<from>::value && RWTIsVoid<to>::value))
#  else
#    define RW_IS_CONVERTIBLE(from, to) std::is_convertible<from, to>::value
#  endif
#elif defined(RW_GCC_VERSION)
#  if __cplusplus >= 201103L
#    include <type_traits>
#    define RW_IS_CONVERTIBLE(from, to) ((std::is_convertible<from, to>::value) && \
                                         (RWTIsVoid<from>::value == RWTIsVoid<to>::value))
#  else
#    include <tr1/type_traits>
#    define RW_IS_CONVERTIBLE(from, to) ((std::tr1::is_convertible<from, to>::value) && \
                                         (RWTIsVoid<from>::value == RWTIsVoid<to>::value))
#  endif
#elif defined(RW_CLANG_VERSION) && (RW_CLANG_VERSION >= 30200)
#  if __cplusplus >= 201103L
#    include <type_traits>
#    define RW_IS_CONVERTIBLE(from, to) std::is_convertible<from, to>::value
#  else
#    include <tr1/type_traits>
#    define RW_IS_CONVERTIBLE(from, to) std::tr1::is_convertible<from, to>::value
#  endif
#elif defined(__HP_aCC) && (__HP_aCC >= 62700)
#  define RW_IS_CONVERTIBLE(from, to) (__is_convertible_to(from, to) && \
                                       (RWTIsVoid<from>::value == RWTIsVoid<to>::value))
#else
#  define RW_NO_PRIMITIVE_IS_CONVERTIBLE
#endif

#if !defined(RW_NO_PRIMITIVE_IS_CONVERTIBLE)

template <typename T, typename U, bool = (RW_IS_CONVERTIBLE(T, U))>
struct RWTIsConvertibleImp : public RWFalseType {
};

#else
#  include <rw/tools/traits/RWTAddRvalueReference.h>
#  include <rw/tools/traits/RWTIsArray.h>
#  include <rw/tools/traits/RWTIsBaseOf.h>
#  include <rw/tools/traits/RWTIsFunction.h>
#  include <rw/tools/traits/RWTIsSame.h>

// xlC warns that non-POD types cannot be passed through an ellipsis.
#if defined(__IBMCPP__) && (__IBMCPP__ >= 1110 && __IBMCPP__ < 1320)
#  pragma report(disable, "1540-2924")
#endif

template < typename T, typename U, bool = (RWTIsVoid<T>::value || RWTIsVoid<U>::value || RWTIsArray<T>::value || RWTIsFunction<T>::value) >
struct RWTIsConvertibleHelper {
    typedef struct { } no;
    typedef struct {
        no field_[2];
    } yes;

    static yes check(U);
    static no check(...);

    static T makeT();

    static const bool value = sizeof(check(makeT())) == sizeof(yes);
};

#if defined(__IBMCPP__) && (__IBMCPP__ >= 1110 && __IBMCPP__ < 1320)
#  pragma report(pop)
#endif

template <typename T, typename U>
struct RWTIsConvertibleHelper<T, U, true> {
    static const bool value = (RWTIsVoid<T>::value && RWTIsVoid<U>::value);
};

template <typename T, typename U, bool = RWTIsConvertibleHelper<T, U>::value >
struct RWTIsConvertibleImp : public RWFalseType {
};

#endif

template <typename T, typename U>
struct RWTIsConvertibleImp<T, U, true> : public RWTrueType {
};

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Determines if a type \c T is convertible to a type \c U
 *
 * If type \c T is convertible to type \c U, derives from RWTrueType;
 * otherwise derives from RWFalseType.
 *
 * @note
 * Due to compiler limitations, not all conversions can be correctly
 * detected across all platforms, and in some cases may result in
 * compilation failures when using this trait. For portability, the following
 * conversions should be avoided or otherwise guarded against.
 * - Types that are related by protected inheritance
 * - Types that are related by private inheritance
 * - Conversions involving volatile types
 * - Conversions involving function types (including pointers and
 *   references)
 * - Conversions involving array types (including conversion to pointer
 *   types)
 * - Conversions involving rvalue reference types
 *
 * @section example Example
 * @code
 * RWTIsConvertible<int, int>::value        // -> true
 * RWTIsConvertible<int, long>::value       // -> true
 * RWTIsConvertible<int, const int>::value  // -> true
 * RWTIsConvertible<int, int*>::value       // -> false
 * RWTIsConvertible<int, int&>::value       // -> false
 * RWTIsConvertible<int, int[]>::value      // -> false
 * @endcode
 */
template <typename T, typename U>
struct RWTIsConvertible : public RWTIsConvertibleImp<T, U> {
};

#endif

