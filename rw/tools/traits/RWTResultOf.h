#ifndef RW_TOOLS_TRAITS_RWTRESULTOF_H
#define RW_TOOLS_TRAITS_RWTRESULTOF_H

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
 * $Id: //tools/13/rw/tools/traits/RWTResultOf.h#2 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWYesType.h>
#include <rw/tools/traits/RWNoType.h>

#if defined(__IBMCPP__) && (__IBMCPP__ < 1320)
#  define RW_BROKEN_DECLTYPE_MEMBER_TYPEDEF 1
#endif

#if DOXYGEN

/**
 * @ingroup type_traits Type Traits
 * @brief Determines the return type of \c T
 *
 * For \c T of the form <tt>F(A1, A2, ..., AN)</tt>, sets the \c type member
 * typedef to the return type of the expression <tt>f(a1, a2, ..., aN)</tt>,
 * where \c f is an instance of \c F, and <tt>a1, a2, ..., aN</tt> are
 * instances of <tt>A1, A2, ..., AN</tt>, respectively.
 *
 * The return type is determined by one of the following methods (in order):
 * - If \c F is a function pointer or function reference, \c type is the
 *   return type of \c F.
 * - If \c F is a member function pointer, \c type is the return type of \c F.
 * - If the compiler supports the \c decltype keyword, \c type is the result
 *   of <tt>decltype(f(a1, a2, ..., aN))</tt>.
 * - If the compiler supports the \c std::result_of type trait, \c type is an
 *   alias for \c std::result_of<T>::type.
 * - If the compiler supports the \c std::tr1::result_of type trait, \c type
 *   is an alias for \c std::tr1::result_of<T>::type.
 * - If \c F is a class type with a member type \c F::result_type, \c type is
 *   an alias for \c F::result_type;
 * - If \c F is a class type without a member type \c F::result_type, or
 *   if \c F::result_type is not a type:
 *     - If <tt>N = 0</tt> (no arguments) \c type is \c void.
 *     - If <tt>N > 0</tt> \c type is <tt>F::template result<F(A1, A2, ..., AN)>::%type</tt>
 * - Otherwise, the program is ill-formed.
 *
 * @note
 * \c N is allowed to be between 0 and 10, inclusive.
 */
template <typename T>
struct RWTResultOf {
    /**
     * The return type of \c T.
     */
    typedef unspecified_type type;
};

#else

template <class T>
struct RWTHasResultType {
private:

    template <class U> struct ResultTypeDetector;

    template <class U>
    static RWYesType check(ResultTypeDetector<typename U::result_type>*);

    template <class U>
    static RWNoType  check(...);

public:
    enum { value = sizeof(check<T>(0)) == sizeof(RWYesType) };
};

template <class F, class FA>
struct RWTHasTemplateResult {
private:

    template <class U> struct ResultTypeDetector;

    template <class U>
    static RWYesType foundResultType(ResultTypeDetector<typename U::template result<FA>::type>*);

    template <class U>
    static RWNoType  foundResultType(...);

public:
    enum { value = sizeof(foundResultType<F>(0)) == sizeof(RWYesType) };
};

#  if !defined(RW_NO_DECLTYPE) && !defined(RW_BROKEN_DECLTYPE_MEMBER_TYPEDEF)


#    if !defined(RW_NO_DECLVAL)
#      include <utility>
#      define rwDeclVal std::declval
#    else

template <typename T>
T && rwDeclVal();

#    endif

template <typename T>
struct RWTDeclType;

template <typename F>
struct RWTDeclType<F()> {
    typedef decltype(rwDeclVal<F>()()) type;
};

template <typename F, typename A1>
struct RWTDeclType<F(A1)> {
    typedef decltype(rwDeclVal<F>()(rwDeclVal<A1>())) type;
};

template <typename F, typename A1, typename A2>
struct RWTDeclType<F(A1, A2)> {
    typedef decltype(rwDeclVal<F>()(rwDeclVal<A1>(), rwDeclVal<A2>())) type;
};

template <typename F, typename A1, typename A2, typename A3>
struct RWTDeclType<F(A1, A2, A3)> {
    typedef decltype(rwDeclVal<F>()(rwDeclVal<A1>(), rwDeclVal<A2>(),
                                    rwDeclVal<A3>())) type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4>
struct RWTDeclType<F(A1, A2, A3, A4)> {
    typedef decltype(rwDeclVal<F>()(rwDeclVal<A1>(), rwDeclVal<A2>(),
                                    rwDeclVal<A3>(), rwDeclVal<A4>())) type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5>
struct RWTDeclType<F(A1, A2, A3, A4, A5)> {
    typedef decltype(rwDeclVal<F>()(rwDeclVal<A1>(), rwDeclVal<A2>(),
                                    rwDeclVal<A3>(), rwDeclVal<A4>(),
                                    rwDeclVal<A5>())) type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct RWTDeclType<F(A1, A2, A3, A4, A5, A6)> {
    typedef decltype(rwDeclVal<F>()(rwDeclVal<A1>(), rwDeclVal<A2>(),
                                    rwDeclVal<A3>(), rwDeclVal<A4>(),
                                    rwDeclVal<A5>(), rwDeclVal<A6>())) type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
struct RWTDeclType<F(A1, A2, A3, A4, A5, A6, A7)> {
    typedef decltype(rwDeclVal<F>()(rwDeclVal<A1>(), rwDeclVal<A2>(),
                                    rwDeclVal<A3>(), rwDeclVal<A4>(),
                                    rwDeclVal<A5>(), rwDeclVal<A6>(),
                                    rwDeclVal<A7>())) type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
struct RWTDeclType<F(A1, A2, A3, A4, A5, A6, A7, A8)> {
    typedef decltype(rwDeclVal<F>()(rwDeclVal<A1>(), rwDeclVal<A2>(),
                                    rwDeclVal<A3>(), rwDeclVal<A4>(),
                                    rwDeclVal<A5>(), rwDeclVal<A6>(),
                                    rwDeclVal<A7>(), rwDeclVal<A8>())) type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct RWTDeclType<F(A1, A2, A3, A4, A5, A6, A7, A8, A9)> {
    typedef decltype(rwDeclVal<F>()(rwDeclVal<A1>(), rwDeclVal<A2>(),
                                    rwDeclVal<A3>(), rwDeclVal<A4>(),
                                    rwDeclVal<A5>(), rwDeclVal<A6>(),
                                    rwDeclVal<A7>(), rwDeclVal<A8>(),
                                    rwDeclVal<A9>())) type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
struct RWTDeclType<F(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)> {
    typedef decltype(rwDeclVal<F>()(rwDeclVal<A1>(), rwDeclVal<A2>(),
                                    rwDeclVal<A3>(), rwDeclVal<A4>(),
                                    rwDeclVal<A5>(), rwDeclVal<A6>(),
                                    rwDeclVal<A7>(), rwDeclVal<A8>(),
                                    rwDeclVal<A9>(), rwDeclVal<A10>())) type;
};

template <typename F, typename T, bool = RWTHasResultType<F>::value, bool = RWTHasTemplateResult<F, T>::value >
struct RWTResultOfImp {
    typedef typename RWTDeclType<T>::type type;
};

#  elif !defined(RW_NO_STD_RESULT_OF)
#    include <functional>

template <typename F, typename T, bool = RWTHasResultType<F>::value, bool = RWTHasTemplateResult<F, T>::value >
struct RWTResultOfImp {
    typedef typename std::result_of<T>::type type;
};

#  elif !defined(RW_NO_STD_TR1_RESULT_OF)

#    if !defined(RW_NO_STD_TR1_FUNCTIONAL_HEADER)
#      include <tr1/functional>
#    else
#      include <functional>
#    endif

template <typename F, typename T, bool = RWTHasResultType<F>::value, bool = RWTHasTemplateResult<F, T>::value >
struct RWTResultOfImp {
    typedef typename std::tr1::result_of<T>::type type;
};

#  else

template <typename F, typename T, bool = RWTHasResultType<F>::value, bool = RWTHasTemplateResult<F, T>::value >
struct RWTResultOfImp;

#  endif


template <typename C, typename T, bool other>
struct RWTResultOfImp<C, T, true, other> {
    typedef typename C::result_type type;
};

#  if (defined(RW_NO_DECLTYPE) || defined(RW_BROKEN_DECLTYPE_MEMBER_TYPEDEF)) && \
       ((defined(RW_NO_STD_RESULT_OF) && defined(RW_NO_STD_TR1_RESULT_OF)) || defined(RW_BROKEN_RESULT_OF_TEMPLATE_RESULT))

template <typename C>
struct RWTResultOfImp<C, C(), false, true> {
    typedef void type;
};

template <typename C, typename T>
struct RWTResultOfImp<C, T, false, true> {
    typedef typename C::template result<T>::type type;
};

#  endif

// Function pointer specializations

template <typename R, typename T>
struct RWTResultOfImp<R(*)(), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename T>
struct RWTResultOfImp<R(*)(A1), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename T>
struct RWTResultOfImp<R(*)(A1, A2), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename T>
struct RWTResultOfImp<R(*)(A1, A2, A3), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename T>
struct RWTResultOfImp<R(*)(A1, A2, A3, A4), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename T>
struct RWTResultOfImp<R(*)(A1, A2, A3, A4, A5), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename T>
struct RWTResultOfImp<R(*)(A1, A2, A3, A4, A5, A6), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename T>
struct RWTResultOfImp<R(*)(A1, A2, A3, A4, A5, A6, A7), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename T>
struct RWTResultOfImp<R(*)(A1, A2, A3, A4, A5, A6, A7, A8), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename T>
struct RWTResultOfImp<R(*)(A1, A2, A3, A4, A5, A6, A7, A8, A9), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename T>
struct RWTResultOfImp<R(*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10), T, false, false> {
    typedef R type;
};

// Function reference specializations

template <typename R, typename T>
struct RWTResultOfImp<R(&)(), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename T>
struct RWTResultOfImp<R(&)(A1), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename T>
struct RWTResultOfImp<R(&)(A1, A2), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename T>
struct RWTResultOfImp<R(&)(A1, A2, A3), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename T>
struct RWTResultOfImp<R(&)(A1, A2, A3, A4), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename T>
struct RWTResultOfImp<R(&)(A1, A2, A3, A4, A5), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename T>
struct RWTResultOfImp<R(&)(A1, A2, A3, A4, A5, A6), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename T>
struct RWTResultOfImp<R(&)(A1, A2, A3, A4, A5, A6, A7), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename T>
struct RWTResultOfImp<R(&)(A1, A2, A3, A4, A5, A6, A7, A8), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename T>
struct RWTResultOfImp<R(&)(A1, A2, A3, A4, A5, A6, A7, A8, A9), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename T>
struct RWTResultOfImp<R(&)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10), T, false, false> {
    typedef R type;
};

// Member function pointer specializations

template <typename R, typename A1, typename T>
struct RWTResultOfImp<R(A1::*)(), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename T>
struct RWTResultOfImp<R(A1::*)(A2), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8, A9), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8, A9, A10), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8, A9, A10, A11), T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename T>
struct RWTResultOfImp<R(A1::*)() const, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename T>
struct RWTResultOfImp<R(A1::*)(A2) const, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3) const, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4) const, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5) const, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6) const, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7) const, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8) const, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8, A9) const, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8, A9, A10) const, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8, A9, A10, A11) const, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename T>
struct RWTResultOfImp<R(A1::*)() volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename T>
struct RWTResultOfImp<R(A1::*)(A2) volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3) volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4) volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5) volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6) volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7) volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8) volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8, A9) volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8, A9, A10) volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8, A9, A10, A11) volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename T>
struct RWTResultOfImp<R(A1::*)() const volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename T>
struct RWTResultOfImp<R(A1::*)(A2) const volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3) const volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4) const volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5) const volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6) const volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7) const volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8) const volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8, A9) const volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8, A9, A10) const volatile, T, false, false> {
    typedef R type;
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename T>
struct RWTResultOfImp<R(A1::*)(A2, A3, A4, A5, A6, A7, A8, A9, A10, A11) const volatile, T, false, false> {
    typedef R type;
};

// RWTResultOf

template <typename T>
struct RWTResultOf;

template <typename F>
struct RWTResultOf<F()> {
    typedef typename RWTResultOfImp<F, F()>::type type;
};

template <typename F, typename A1>
struct RWTResultOf<F(A1)> {
    typedef typename RWTResultOfImp<F, F(A1)>::type type;
};

template <typename F, typename A1, typename A2>
struct RWTResultOf<F(A1, A2)> {
    typedef typename RWTResultOfImp<F, F(A1, A2)>::type type;
};

template <typename F, typename A1, typename A2, typename A3>
struct RWTResultOf<F(A1, A2, A3)> {
    typedef typename RWTResultOfImp<F, F(A1, A2, A3)>::type type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4>
struct RWTResultOf<F(A1, A2, A3, A4)> {
    typedef typename RWTResultOfImp<F, F(A1, A2, A3, A4)>::type type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5>
struct RWTResultOf<F(A1, A2, A3, A4, A5)> {
    typedef typename RWTResultOfImp<F, F(A1, A2, A3, A4, A5)>::type type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct RWTResultOf<F(A1, A2, A3, A4, A5, A6)> {
    typedef typename RWTResultOfImp<F, F(A1, A2, A3, A4, A5, A6)>::type type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
struct RWTResultOf<F(A1, A2, A3, A4, A5, A6, A7)> {
    typedef typename RWTResultOfImp<F, F(A1, A2, A3, A4, A5, A6, A7)>::type type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
struct RWTResultOf<F(A1, A2, A3, A4, A5, A6, A7, A8)> {
    typedef typename RWTResultOfImp<F, F(A1, A2, A3, A4, A5, A6, A7, A8)>::type type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct RWTResultOf<F(A1, A2, A3, A4, A5, A6, A7, A8, A9)> {
    typedef typename RWTResultOfImp<F, F(A1, A2, A3, A4, A5, A6, A7, A8, A9)>::type type;
};

template <typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
struct RWTResultOf<F(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)> {
    typedef typename RWTResultOfImp<F, F(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)>::type type;
};

#endif

#endif

