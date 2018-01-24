#ifndef __RWNUMTRAIT_H__
#define __RWNUMTRAIT_H__

/***************************************************************************
 *
 * numtrait.h
 *
 * $Id: //math/13/rw/math/numtrait.h#1 $
 *
 * Copyright (c) 1991-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 **************************************************************************/

#include <rw/dcomplex.h>
#include <rw/mathwind.h>

/**
 * @ingroup math_collection_classes
 *
 * @brief Helper class containing typedefs associated with numeric types,
 * useful when creating a new numeric type.
 *
 * The rw_numeric_traits class is a helper class that contains only
 * typedef information. The generic \link rw_numeric_traits rw_numeric_traits<T>
 * \endlink assumes that all the typedefs are set to T. Since this
 * is not desirable for many types, we specialize for each numeric
 * type that is used in the library.
 *
 * The specializations used in the library are contained in the
 * file <tt>rw/math/numtrait.h</tt>. For an overview of how to add a
 * new numeric type, see Section 2.2.2, "Adding New Numeric Types" in the
 * Essential Math Module User's Guide.
 *
 * There are seven public typedefs in each specialized rw_numeric_traits
 * class. Each typedef must be defined in a given specialization
 * for proper results. The generic class is:
 *
 * @code
 *
 * template< class T >
 * class rw_numeric_traits
 * {
 * public:
 * typedef T norm_type;
 * typedef T calc_type;
 * typedef T numeric_type;
 * typedef T c_type;
 * typedef T promote_type;
 *
 * typedef T (* mathFunType)(T);
 * typedef norm_type (*mathFunType2)(T);
 * };
 * @endcode
 *
 * @section synopsis Synopsis
 *
 * @code
 * template <class T>
 * class rw_numeric_traits
 * @endcode
 *
 * @section specializations Specializations
 *
 * @code
 *   rw_numeric_traits<int>
 *   rw_numeric_traits<float>
 *   rw_numeric_traits<double>
 *   rw_numeric_traits<DComplex>
 *   rw_numeric_traits<UChar>
 *   rw_numeric_traits<SChar>
 * @endcode
 */
template< class T >
class rw_numeric_traits
{
public:
    /**
     * The return type of \c abs(T) or similar numerical \c norm-like functions,
     * such as \c rw_numeric_traits<DComplex>::norm_type, is \c double
     * because abs(DComplex) returns a \c double. This is most often
     * \c double for built-in types.
     */
    typedef T norm_type;

    /**
     * For some aggregate functions such as \c sum, a higher-precision
     * numeric type can be used for intermediate calculations. This
     * is intended mostly for floating point types to reduce round-off
     * error in computations. For example, \c rw_numeric_traits<float>::calc_type
     * is \c double because this makes the sum of a vector of \c floats
     * less susceptible to round-off error.
     */
    typedef T calc_type;

    /**
     * For most types this should be set to T. This typedef is aimed
     * to improve I/O operations for char based collections. If T is
     * a \c char-like type, but it is used in any \link RWMathVec RWMathVec<T>
     * \endlink, \link RWGenMat RWGenMat<T> \endlink, or \link RWMathArray
     * RWMathArray<T> \endlink, the typedef should be set to \c int.
     */
    typedef T numeric_type;

    /**
     * This is the companion to #numeric_type. If T is a \c char-like
     * type, it should be set to \c char; otherwise, it should be set
     * to T.
     */
    typedef T c_type;

    /**
     * Automatic conversions and templates do <i>not</i> work well together
     * because they readily lead to ambiguity errors. For that reason,
     * automatic conversions of the collection classes are limited to
     * one promotion type per numeric type T. The \c promote_type setting
     * allows an \link RWMathVec RWMathVec<T> \endlink to be converted
     * automatically to  \c RWMathVec<rw_numeric_traits<T>::promote_type>.
     * In other words, \c rw_numeric_traits<float>::promote_type is
     * \c double, while \c rw_numeric_traits<double>::promote_type is
     * DComplex. This typedef is designed to assist in backward compatibility,
     * and may become obsolete when new language features become available.
     */
    typedef T promote_type;

    /**
     * The member functions RWMathVec<T>::apply(), RWGenMat<T>::apply(),
     * and RWMathArray<T>::apply() take a function pointer of type
     * \link rw_numeric_traits::mathFunType mathFunType\endlink.
     * These functions are used to apply a global function
     * to each element of a collection. The compiler must make an exact
     * match for the \c apply function to work, and since most global
     * numerical functions take a \c double and return a \c double,
     * the \link rw_numeric_traits::mathFunType mathFunType\endlink
     * is set to \c double in almost all cases except
     * complex. If type T is readily converted to type \c double and
     * you want to use the \c apply function with built-in functions,
     * \a mathFunType should be set by:
     *
     * <tt>typedef double (*\a mathFunType)(double)</tt>;
     *
     * If, on the other hand, your type T uses specific functions that
     * take T and return T, the default setting is appropriate. Note
     * that some compilers distinguish and do \e not match between
     * the above typedef and:
     *
     * <tt>typedef double (*\a mathFunType)(const double&)</tt>;
     *
     * Be sure that your definition of
     * \link rw_numeric_traits::mathFunType mathFunType\endlink
     * is consistent with the functions you use in \c apply.
     */
    typedef T(* mathFunType)(T);

    /**
     * The member functions RWMathVec<T>::apply2(), RWGenMat<T>::apply2(),
     * and RWMathArray<T>::apply2() take a function pointer of type
     * \link rw_numeric_traits::mathFunType2 mathFunType2\endlink. The
     * discussion of
     * \link rw_numeric_traits::mathFunType mathFunType\endlink
     * applies equally here,  with the exception of the return type. The return
     * type for the \c apply2 function is type norm_type.
     * The \link rw_numeric_traits::mathFunType mathFunType\endlink
     * and \link rw_numeric_traits::mathFunType2 mathFunType2\endlink
     * definitions are most often the same except that the return type
     * of \link rw_numeric_traits::mathFunType2 mathFunType2\endlink is
     * norm_type.
     */
    typedef norm_type(*mathFunType2)(T);
};

RW_MATH_SPECIALIZE
class rw_numeric_traits<int>
{
public:
    typedef int    norm_type;
    typedef double calc_type;
    typedef int    numeric_type;
    typedef int    c_type;
    typedef double promote_type;

#if defined(__IBMCPP__)
    typedef mathFunTy mathFunType;
#else
    typedef double(* mathFunType)(double);
#endif
    typedef norm_type(*mathFunType2)(double);
};

RW_MATH_SPECIALIZE
class rw_numeric_traits<float>
{
public:
    typedef float  norm_type;
    typedef double calc_type;
    typedef float  numeric_type;
    typedef float  c_type;
    typedef double promote_type;

#if defined(__IBMCPP__)
    typedef mathFunTy mathFunType;
#else
    typedef double(* mathFunType)(double);
#endif
    typedef norm_type(*mathFunType2)(double);
};

RW_MATH_SPECIALIZE
class rw_numeric_traits<double>
{
public:
    typedef double  norm_type;
    typedef double calc_type;
    typedef double  numeric_type;
    typedef double  c_type;
    typedef DComplex promote_type;

#if defined(__IBMCPP__)
    typedef mathFunTy mathFunType;
#else
    typedef double(* mathFunType)(double);
#endif
    typedef norm_type(*mathFunType2)(double);
};

RW_MATH_SPECIALIZE
class rw_numeric_traits<DComplex>
{
public:
    typedef double   norm_type;
    typedef DComplex calc_type;
    typedef DComplex numeric_type;
    typedef DComplex c_type;
    typedef DComplex promote_type;
    typedef CmathFunTy mathFunType;
    typedef CmathFunTy2 mathFunType2;
};

RW_MATH_SPECIALIZE
class rw_numeric_traits<SChar>
{
public:
    typedef SChar  norm_type;
    typedef SChar  calc_type;
    typedef int          numeric_type;
    typedef  char        c_type;
    typedef int          promote_type;

    typedef double(* mathFunType)(double);
    typedef norm_type(*mathFunType2)(double);
};

RW_MATH_SPECIALIZE
class rw_numeric_traits<UChar>
{
public:
    typedef UChar norm_type;
    typedef UChar calc_type;
    typedef int           numeric_type;
    typedef char          c_type;
    typedef int           promote_type;

    typedef double(* mathFunType)(double);
    typedef norm_type(*mathFunType2)(double);
};



#endif
