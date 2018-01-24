#ifndef __FUNCOBJ_CC__
#define __FUNCOBJ_CC__

/***************************************************************************
 *
 * funcobj.cc
 *
 * $Id: //math/13/rw/math/funcobj.cc#1 $
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
 **************************************************************************
 * $Log: funcobj.cc,v $
 * Revision 1.3  1996/05/13 18:56:37  steves
 * Added copyrite macros.
 *
 *************************************************************************/

#include <vector>
#include <iterator>
#include <rw/tools/pointer.h>

template <class InputIterator, class OutputIterator, class UnaryOperation>
RW_INLINE_TMPL_FUNC OutputIterator rw_math_transform(InputIterator first, InputIterator last,
        OutputIterator result, UnaryOperation op)
{
    while (first != last) {
        *result++ = op(*first++);
    }
    return result;
}

template < class InputIterator1, class InputIterator2, class OutputIterator,
         class BinaryOperation >
RW_INLINE_TMPL_FUNC OutputIterator rw_math_transform(InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, OutputIterator result,
        BinaryOperation binary_op)
{
    while (first1 != last1) {
        * result++ = binary_op(*first1++, *first2++);
    }
    return result;
}

template <class ForwardIterator, class Generator>
RW_INLINE_TMPL_FUNC void rw_math_generate(ForwardIterator first, ForwardIterator last,
        Generator gen)
{
    while (first != last) {
        *first++ = gen();
    }
}

template <class ForwardIterator>
RW_INLINE_TMPL_FUNC void rw_math_generate2(ForwardIterator first, ForwardIterator last,
        RWRandInterface& gen)
{
    RWTScopedPointer<double[]> vecptr(0);
    double* vec;
    double* it;
    double local[128];

#if !defined(RW_3ARG_DISTANCE)
    typename ForwardIterator::difference_type size = std::distance(first, last);
#else
    unsigned int size = 0;
    std::distance(first, last, size);
#endif

    if (size <= 128) {
        vec = local;
    }
    else {
        vecptr.reset(new double[static_cast<size_t>(size)]);
        vec = vecptr.get();
    }
    gen(vec, static_cast<unsigned int>(size));
    it = vec;
    while (first != last) {
        *first++ = *it++;
    }
}

// raise a numberic type T to an integer power using right-to-left binary method
// as described in Knuth's Semi Numerical Algorithms page 442
//
// Requirements for T:
//  * must be constructible from an integer
//  * must efine the *= operator

template<class T>
T RW_pow2<T>::operator()(const T& x, int n)
{
    if (x == T(0) && n <= 0) {
        int p = (int)pow(0.0, (double)n);
        return T(p);
    }
    if (x == T(0) || n == 1) {
        return x;
    }
    if (n == 0) {                          //checking if raised to zero
        return T(1);
    }

    T y(1);
    T z(x);                                //initialization
    int power(n);

    power = n < 0 ? -n : n;                //negating negative power

    while (1) {                            //right-to-left binary method
        if ((power % 2) == 0) {            //for exponentiation
            power = (power / 2);
        }
        else {
            power = (power / 2);
            y *= z;
            return(n < 0 ? 1 / y : y);
        }
        z = z * z;
    }
}

#endif // __FUNCOBJ_CC__
