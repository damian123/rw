#ifndef __RWMTHVECMA_CC__
#define __RWMTHVECMA_CC__

/***************************************************************************
 *
 * mthvecma.cc
 *
 * $Id: //math/13/rw/math/mthvecma.cc#1 $
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

#include <rw/mathdefs.h>
#ifdef RW_MATH_NO_NEW_HEADER
#  include <math.h>
#else
#  include <cmath>
#endif
#include <rw/math/rwbla.h>
#include <rw/rwerr.h>
#include <rw/matherr.h>



// Global Functions -----------------------------------------------------

#ifndef RW_INLINE_TRAITS_TEMPLATES
template <class T>
#  if defined(RW_NO_TYPENAME_OUTSIDE_DECLARATION) || (defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x530))
RWMathVec<RWMathVec<T>::norm_type> RWMathVec<T>::apply2(mathFunType2 f) const
#  else
RWMathVec<typename RWMathVec<T>::norm_type> RWMathVec<T>::apply2(mathFunType2 f) const
#  endif
{
    register size_t i = length();
    RWMathVec<norm_type> temp(i, rwUninitialized);
    register T* sp = (T*)data();
    register norm_type* dp = (norm_type*)temp.data();
    register int j = stride();
    while (i--) {
        *dp++ = (*f)(*sp);
        sp += j;
    }
    return temp;
}
#endif


#if defined(RW_NATIVE_EXTENDED)
RWMathVec<T>
RWMathVec<T>::apply(XmathFunType f) const
// apply function for compilers using the extended type
{
    register size_t i = length();
    RWMathVec<T> temp(i, rwUninitialized);
    register const T* sp = data();
    register T* dp = temp.data();
    register int j = stride();
    while (i--) {
        *dp++ = (*f)(*sp);
        sp += j;
    }
    return temp;
}
#endif


template< class T>
RWMathVec<T> atan2(const RWMathVec<T>& u, const RWMathVec<T>& v)
// Arctangent of u/v.
{
    register size_t i = v.length();
    u.lengthCheck(i);
    RWMathVec<T> temp(i, rwUninitialized);
    register const T* up = u.data();
    register const T* vp = v.data();
    register T* dp = temp.data();
    register int uj = u.stride();
    register int vj = v.stride();

    while (i--) {
        *dp++ = T(atan2(*up, *vp));
        up += uj;
        vp += vj;
    }

    return temp;
}


template< class T>
RWMathVec<T> cumsum(const RWMathVec<T>& s)
// Cumulative sum of RWMathVec<T> slice.
// Note: V == delta(cumsum(V)) == cumsum(delta(V))
{
    size_t i = s.length();
    RWMathVec<T> temp(i, rwUninitialized);
    register T* sp = (T*)s.data();
    register T* dp = temp.data();
    typename rw_numeric_traits<T>::calc_type c = 0;
    register int j = s.stride();
    while (i--) {
        c += *sp;
        *dp++ = T(c);
        sp += j;
    }

    return temp;
}


template< class T>
RWMathVec<T> delta(const RWMathVec<T>& s)
// Element differences of RWMathVec<T> slice.
// Note: V == delta(cumsum(V)) == cumsum(delta(V))
{
    size_t i = s.length();
    RWMathVec<T> temp(i, rwUninitialized);
    register T* sp = (T*)s.data();
    register T* dp = temp.data();
    typename rw_numeric_traits<T>::calc_type c = 0;
    register int j = s.stride();
    while (i--) {
        *dp++ = T(*sp - c);
        c = *sp;
        sp += j;
    }
    return temp;
}


template< class T>
T dot(const RWMathVec<T>& u, const RWMathVec<T>& v)
// Vector dot product.
{
    size_t N = v.length();
    u.lengthCheck(N);
    T* x = (T*)u.data();
    T* y = (T*)v.data();
    if (u.stride() < 0) {
        (x += (N - 1) * u.stride());
    }
    if (v.stride() < 0) {
        (y += (N - 1) * v.stride());
    }
    return RWBlas<T>::dot((int)N, x, u.stride(), y, v.stride());
}


template< class T>
int maxIndex(const RWMathVec<T>& s)
// Index of maximum element.
{
    if (s.length() == 0) {
        s.emptyErr("maxIndex");
    }

    RW_ASSERT(s.length() <= RW_LONG_MAX);
    long tempImax = RWBlas_util<T>::imax(static_cast<long>(s.length()),
                                         (T*)s.data(),
                                         s.stride());
    RW_LAPACK_INT_MAX_ASSERT(tempImax);
    int result = static_cast<int>(tempImax);
    return result;
}


template< class T>
T maxValue(const RWMathVec<T>& s)
{
    int inx = maxIndex(s);
    return s(inx);
}


template< class T>
int minIndex(const RWMathVec<T>& s)
// Index of minimum element.
{
    if (s.length() == 0) {
        s.emptyErr("minIndex");
    }

    RW_ASSERT(s.length() <= RW_LONG_MAX);
    long tempImin = RWBlas_util<T>::imin(static_cast<long>(s.length()), (T*)s.data(), s.stride());
    RW_LAPACK_INT_MAX_ASSERT(tempImin);
    int result = static_cast<int>(tempImin);
    return result;
}


template< class T>
T minValue(const RWMathVec<T>& s)
{
    int inx = minIndex(s);
    return s(inx);
}


template< class T>
T prod(const RWMathVec<T>& s)
// Product of RWMathVec<T> elements.
{
    register size_t i = s.length();
    register T* sp = (T*)s.data();
    typename rw_numeric_traits<T>::calc_type t = (typename rw_numeric_traits<T>::calc_type)1;
    register int j = s.stride();
    while (i--) {
        t *= *sp;
        sp += j;
    }
    return T(t);
}

template< class T >
RWMathVec<T>  crossProduct(const RWMathVec<T>& x, const RWMathVec<T>& y)
{
    RWMathVec<T> z(3, rwUninitialized);
    x.lengthCheck(3);
    y.lengthCheck(3);
    z(0) = x(1) * y(2) - x(2) * y(1);
    z(1) = x(2) * y(0) - x(0) * y(2);
    z(2) = x(0) * y(1) - x(1) * y(0);
    return z;
}


template< class T >
RWGenMat<T>  outerProduct(const RWMathVec<T>& x, const RWMathVec<T>& y)
// outer product of x and y
{
    RWGenMat<T> Z(x.length(), y.length(), rwUninitialized);
    unsigned i;

    for (i = 0; i < y.length(); i++) {
        Z.col(i) = y[ i ] * x;
    }

    return Z;
}


template< class T>
RWMathVec<T> pow(const RWMathVec<T>& u, const RWMathVec<T>& v)
// u to the v power.
{
    register size_t i = v.length();
    u.lengthCheck(i);
    RWMathVec<T> temp(i, rwUninitialized);
    rw_math_transform(u.begin(), u.end(), v.begin(), temp.begin(), RW_pow<T, T, T>());
    return temp;
}


template <class T>
RWMathVec<T> rw_do_pow(const RWMathVec<T>& base, T expo)
// base to the expo power, vector to scalar.
{
    RWMathVec<T> temp(base.length(), rwUninitialized);
    RW_ASSERT(temp.data() != 0 || temp.length() == 0);
    typename RWMathVec<T>::const_iterator baseBegin = base.begin();
    typename RWMathVec<T>::const_iterator baseEnd = base.end();
    typename RWMathVec<T>::iterator tempIter = temp.begin();

    RW_pow<T, T, T> rwPow;

    while (baseBegin != baseEnd) {
        *tempIter++ = rwPow(*baseBegin++, expo);
    }

    return temp;
}

template <class T>
RWMathVec<T> pow(const RWMathVec<T>& x, T y)
{
    return rw_do_pow(x, y);
}

// need complex overload to prevent ambiguous function error
template <class T>
RWMathVec<std::complex<T> > pow(const RWMathVec<std::complex<T> >& x, std::complex<T> y)
{
    return rw_do_pow(x, y);
}


template <class T>
RWMathVec<T> rw_do_pow(T base, const RWMathVec<T>& expo)
// base to the expo power, scalar to vector.
{
    RWMathVec<T> temp(expo.length(), rwUninitialized);
    RW_ASSERT(temp.data() != 0 || temp.length() == 0);
    typename RWMathVec<T>::const_iterator expoBegin = expo.begin();
    typename RWMathVec<T>::const_iterator expoEnd = expo.end();
    typename RWMathVec<T>::iterator tempIter = temp.begin();

    RW_pow<T, T, T> rwPow;

    //while (expoBegin != expoEnd) *tempIter++ = rwPow(expo, *expoBegin++);
    while (expoBegin != expoEnd) {
        *tempIter++ = rwPow(base, *expoBegin++);
    }

    return temp;
}

template <class T>
RWMathVec<T> pow(T x, const RWMathVec<T>& y)
{
    return rw_do_pow(x, y);
}

// need complex overload to prevent ambiguous function error
template <class T>
RWMathVec<std::complex<T> > pow(std::complex<T> x, const RWMathVec<std::complex<T> >& y)
{
    return rw_do_pow(x, y);
}


template< class T>
RWMathVec<T> reverse(const RWMathVec<T>& s)
// Reverse vector elements --- pretty tricky!
{
    return s.slice(static_cast<unsigned>(s.length() - 1), s.length(), -1);
}


template< class T>
T sum(const RWMathVec<T>& s)
// Sum of a RWMathVec<T>
{
    typedef typename rw_numeric_traits<T>::calc_type calc_type;
    register size_t i = s.length();
    register T* sp = (T*)s.data();
    register calc_type t = calc_type(0);

    register int j = s.stride();
    while (i--) {
        t += *sp;
        sp += j;
    }
    return T(t);
}


//***************************************************
// Various functions with simple definitions:
//***************************************************

template< class T >
RWMathVec<T>    acos(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_acos<T, T>());
    return tmp;
}

template< class T >
RWMathVec<T>    asin(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_asin<T, T>());
    return tmp;
}

template< class T >
RWMathVec<T>    atan(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_atan<T, T>());
    return tmp;
}

template< class T >
RWMathVec<T>    ceil(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_ceil<T, T>());
    return tmp;
}

template< class T >
RWMathVec<T>    cos(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_cos<T, T>());
    return tmp;
}

template< class T >
RWMathVec<T>    cosh(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_cosh<T, T>());
    return tmp;
}

template< class T >
RWMathVec<T>    exp(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_exp<T, T>());
    return tmp;
}

template< class T >
RWMathVec<T>    floor(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_floor<T, T>());
    return tmp;
}

template< class T >
RWMathVec<T>    log(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_log<T, T>());
    return tmp;
}

template< class T >
RWMathVec<T>    log10(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_log10<T, T>());
    return tmp;
}

template< class T >
T               mean(const RWMathVec<T>& V)
{
    typename RWMathVec<T>::const_iterator p = V.begin(), stop = V.end();
    typename rw_numeric_traits<T>::calc_type t = T(0);
    while (p != stop) {
        t += *p;
        ++p;
    }

    return T(t / (typename rw_numeric_traits<T>::calc_type)V.length());
}

template< class T >
RWMathVec<T>    sin(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_sin<T, T>());
    return tmp;
}

template< class T >
RWMathVec<T>    sinh(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_sinh<T, T>());
    return tmp;
}


template< class T >
RWMathVec<T>    sqrt(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_sqrt<T, T>());
    return tmp;
}


template< class T >
RWMathVec<T>    tan(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_tan<T, T>());
    return tmp;
}


template< class T >
RWMathVec<T>    tanh(const RWMathVec<T>& V)
{
    RWMathVec<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_tanh<T, T>());
    return tmp;
}


//***************************************************************
// Expand an even real series into its full series.
//***************************************************************

template< class T >
RWMathVec<T> expandEven(const RWMathVec<T>& v)
{
    unsigned N = static_cast<unsigned>(v.length() - 1);
    RWMathVec<T> full(2 * N, rwUninitialized);

    // Lower half:
    full.slice(0, N + 1, 1) = v;
    // Upper half:
    full.slice(N + 1, N - 1, 1) = v.slice(N - 1, N - 1, -1);

    return full;
}

//*******************************************************
// Expand an odd series into its full series.
//*******************************************************

template< class T >
RWMathVec<T> expandOdd(const RWMathVec<T>& v)
{
    unsigned N = static_cast<unsigned>(v.length() + 1);
    size_t two(2); // to foil overly aggressive optimizers
    RWMathVec<T> full(two * N, rwUninitialized);

    // Lower half:
    full.slice(1, N - 1, 1) = v;
    // Upper half:
#if defined(RW_AIX_BUG_WORKAROUND)
    RWMathVec<T> temp = -v.slice(N - 2, N - 1, -1);
    full.slice(N + 1, N - 1, 1) = temp;
#else
    full.slice(N + 1, N - 1, 1) = -v.slice(N - 2, N - 1, -1);
#endif

    full((int)0) = T(0);
    full(N) = T(0);

    return full;
}

#endif
