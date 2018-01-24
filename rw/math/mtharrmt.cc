#ifndef __RWMATHARRMT_CC_
#define __RWMATHARRMT_CC_

/***************************************************************************
 *
 * mtharrmt.cc - Math definitions
 *
 * $Id: //math/13/rw/math/mtharrmt.cc#1 $
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

#include <rw/math/mtharray.h>
#include <rw/math/rwbla.h>
#include <rw/dcomplex.h>

#ifdef RW_MATH_NO_NEW_HEADER
#  include <stdlib.h>
#else
#  include <cstdlib>
#endif  /* Looking for abs(int) */

// Apply function returning norm_type to each element of vector slice
#ifndef RW_INLINE_TRAITS_TEMPLATES
template <class T>
RWMathArray<typename RWMathArray<T>::norm_type> RWMathArray<T>::apply2(mathFunType2 f) const
{
    RWMathArray<norm_type> temp(npts, rwUninitialized);
    typename RWMathArray<norm_type>::iterator dst = temp.begin();
    typename RWMathArray<T>::const_iterator src = this->begin();
    typename RWMathArray<T>::const_iterator end = this->end();

    for (; src != end; ++src, ++dst) {
        *dst = (*f)(*src);
    }
    return temp;
}
#endif

#if defined(RW_NATIVE_EXTENDED)
template <class T>
RWMathArray<T> RWMathArray<T>::apply(XmathFunTy f) const
// Apply function for compilers using extended types
{
    RWMathArray<T> temp(npts, rwUninitialized, RWDataView::COLUMN_MAJOR);
    register T* dp = temp.data();
    for (ArrayLooper l(npts, step); l; ++l) {
        register const T* sp = data() + l.start;
        register int i = l.length;
        register int j = l.stride;
        while (i--) {
            *dp++ = (*f)(*sp);
            sp += j;
        }
    }
    return temp;
}
#endif


template <class T>
RWMathArray<T> atan2(const RWMathArray<T>& u, const RWMathArray<T>& v)
// Arctangent of u/v.
{
    u.lengthCheck(v.length());
    RWMathArray<T> temp(v.length(), rwUninitialized, RWDataView::COLUMN_MAJOR);
    register T* dp = temp.data();
    RW_ASSERT(dp != 0 || temp.length() == 0);
    for (DoubleArrayLooper l(v.length(), u.stride(), v.stride()); l; ++l) {
        register const T* up = u.data() + l.start1;
        register const T* vp = v.data() + l.start2;
        register size_t i = l.length;
        register int uj = l.stride1;
        register int vj = l.stride2;
        while (i--) {
            *dp++ = static_cast<T>(atan2(*up, *vp));
            up += uj;
            vp += vj;
        }
    }
    return temp;
}

template <class T>
T dot(const RWMathArray<T>& u, const RWMathArray<T>& v)
// dot product: sum_ij...k (u_ij...k * v_ij...k)
{
    u.lengthCheck(v.length());
    T result = 0;
    T* x;
    T* y;
    for (DoubleArrayLooper l(v.length(), u.stride(), v.stride()); l; ++l) {
        x = u.data() + l.start1;
        y = v.data() + l.start2;
        if (l.stride1 < 0) {
            (x += (l.length - 1) * l.stride1);
        }
        if (l.stride2 < 0) {
            (y += (l.length - 1) * l.stride2);
        }
        result += RWBlas<T>::dot(l.length, x, l.stride1, y, l.stride2);
    }
    return result;
}

template <class T>
RWMathArray<T> dot(const RWMathVec<T>& V, const RWMathArray<T>& A)
// dot product: Bj...k = sum_i (Vi * Aij...k)
{
    int n = V.length();
    A.dimensionLengthCheck(0, n);
    RWToEnd jk(1);   // Indexes all components of a vector but the first
    RWMathArray<T> B(A.length()(jk), rwUninitialized, RWDataView::COLUMN_MAJOR);
    T* dp = B.data();
    T* x;
    T* y;
    for (ArrayLooper l(B.length(), A.stride()(jk)); l; ++l) {
        const T* sp = A.data() + l.start;
        for (int i = l.length; i--;) {
            x = V.data();
            y = sp;
            if (V.stride() < 0) {
                (x += (n - 1) * V.stride());
            }
            if (A.stride(0) < 0) {
                (y += (n - 1) * A.stride(0));
            }
            *dp = RWBlas<T>::dot(n, x, V.stride(), y, A.stride(0));
            sp += l.stride;
            dp++;
        }
    }
    return B;
}

template <class T>
RWMathArray<T> dot(const RWMathArray<T>& A, const RWMathVec<T>& V)
// dot product: Bi...j = sum_k (Ai...jk * Vk)
{
    int n = V.length();
    A.dimensionLengthCheck(0, n);
    RWSlice ij(0, n - 1); // Indexes all components of array index but the last
    RWMathArray<T> B(A.length()(ij), rwUninitialized, RWDataView::COLUMN_MAJOR);
    T* dp = B.data();
    T* x;
    T* y;
    for (ArrayLooper l(B.length(), A.stride()(ij)); l; ++l) {
        const T* sp = A.data() + l.start;
        for (int i = l.length; i--;) {
            x = V.data();
            y = sp;
            if (V.stride() < 0) {
                (x += (n - 1) * V.stride());
            }
            if (A.stride(0) < 0) {
                (y += (n - 1) * A.stride(0));
            }
            *dp = RWBlas<T>::dot(n, x, V.stride(), y, A.stride(0));
            sp += l.stride;
            dp++;
        }
    }
    return B;
}

template <class T>
RWIntVec maxIndex(const RWMathArray<T>& s)
// Index of maximum element.
{
    s.numPointsCheck("max needs at least", 1);
    unsigned p = static_cast<unsigned>(s.dimension());
    if (p < 1) {
        return s.length();    //s.length() is a length 0 vector
    }
    RWIntVec index(p, 0);
    RWIntVec maxIndex(p, 0);
    T max = s(maxIndex);

    // Loop through the array, using the bla routine to loop through the
    // innermost loop (first component of index vector).
    int strider = s.stride(0);

    size_t len = static_cast<unsigned>(s.length(0));
    while (index(p - 1) < s.length(p - 1)) {
        const T* ptr = s.data() + dot(index, s.stride());
        RW_ASSERT(len <= RW_LONG_MAX);
        long tempImax = RWBlas_util<T>::imax(static_cast<long>(len), (T*)ptr, strider);
        RW_LAPACK_INT_MAX_ASSERT(tempImax);
        int maxLocal = static_cast<int>(tempImax);
        if (ptr[maxLocal * strider] > max) {
            max = ptr[maxLocal * strider];
            index(0) = maxLocal;
            maxIndex = index;
        }
        index(0) = s.length(0);     // Skip over the zeroth component, then
        unsigned r = 0;                  // bump up the index.
        while (index(r) >= s.length(r) && ++r < p) {
            index(r - 1) = 0;
            ++(index(r));
        }
    }
    return maxIndex;
}

template <class T>
T maxValue(const RWMathArray<T>& s)
{
    s.numPointsCheck("maxValue needs at least", 1);
    T max = *(s.data());
    for (ArrayLooper l(s.length(), s.stride()); l; ++l) {
        RW_ASSERT(l.length <= RW_LONG_MAX);
        long localMaxIndex = RWBlas_util<T>::imax(static_cast<long>(l.length), (T*)s.data() + l.start, l.stride);
        T localMax = *(s.data() + l.start + localMaxIndex * l.stride);
        if (localMax > max) {
            max = localMax;
        }
    }
    return max;
}

template <class T>
RWIntVec minIndex(const RWMathArray<T>& s)
// Index of minimum element.
{
    s.numPointsCheck("min needs at least", 1);
    unsigned p = static_cast<unsigned>(s.dimension());
    if (p < 1) {
        return s.length();    //s.length() is a length 0 vector
    }
    RWIntVec index(p, 0);
    RWIntVec minIndex(p, 0);
    T min = s(minIndex);

    // Loop through the array, using the bla routine to loop through the
    // innermost loop (first component of index vector).
    int strider = s.stride(0);
    long len = s.length(0);
    while (index(p - 1) < s.length(p - 1)) {
        const T* ptr = s.data() + dot(index, s.stride());
        long tempImin = RWBlas_util<T>::imin(len, (T*)ptr, strider);
        RW_LAPACK_INT_MAX_ASSERT(tempImin);
        int minLocal = static_cast<int>(tempImin);
        if (ptr[minLocal * strider] < min) {
            min = ptr[minLocal * strider];
            index(0) = minLocal;
            minIndex = index;
        }
        index(0) = s.length(0);     // Skip over the zeroth component, then
        unsigned r = 0;             // bump up the index.
        while (index(r) >= s.length(r) && ++r < p) {
            index(r - 1) = 0;
            ++(index(r));
        }
    }
    return minIndex;
}

template <class T>
T minValue(const RWMathArray<T>& s)
{
    s.numPointsCheck("minValue needs at least", 1);
    T min = *(s.data());
    for (ArrayLooper l(s.length(), s.stride()); l; ++l) {
        int localMinIndex = RWBlas_util<T>::imin(l.length, (T*)s.data() + l.start, l.stride);
        T localMin = *(s.data() + l.start + localMinIndex * l.stride);
        if (localMin < min) {
            min = localMin;
        }
    }
    return min;
}


template <class T>
T prod(const RWMathArray<T>& s)
{
    T t = 1;
    for (ArrayLooper l(s.length(), s.stride()); l; ++l) {
        register T* sp = (T*)(s.data() + l.start);
        register size_t i = l.length;
        register int j = l.stride;
        while (i--) {
            t *= *sp;
            sp += j;
        }
    }
    return t;
}

template <class T>
RWMathArray<T> pow(const RWMathArray<T>& u, const RWMathArray<T>& v)
// u to the v power.
{
    u.lengthCheck(v.length());
    RWMathArray<T> temp(v.length(), rwUninitialized, RWDataView::COLUMN_MAJOR);
    register T* dp = temp.data();
    RW_ASSERT(dp != 0 || temp.length() == 0);
    for (DoubleArrayLooper l(v.length(), u.stride(), v.stride()); l; ++l) {
        register size_t i = l.length;
        register T* up = (T*)(u.data() + l.start1);
        register T* vp = (T*)(v.data() + l.start2);
        register int uj = l.stride1;
        register int vj = l.stride2;
        while (i--) {
            *dp++ = pow(*up, *vp);
            up += uj;
            vp += vj;
        }
    }
    return temp;
}


template <class T>
RWMathArray<T> rw_do_pow(const RWMathArray<T>& base, T expo)
// base to the expo power, Array to scalar.
{

    RWMathArray<T> temp(base.length(), rwUninitialized, RWDataView::COLUMN_MAJOR);
    typename RWMathArray<T>::const_iterator baseBegin = base.begin();
    typename RWMathArray<T>::const_iterator baseEnd = base.end();
    typename RWMathArray<T>::iterator tempIter = temp.begin();

    RW_pow<T, T, T> rwPow;

    while (baseBegin != baseEnd) {
        *tempIter++ = rwPow(*baseBegin++, expo);
    }

    return temp;
}

template <class T>
RWMathArray<T> pow(const RWMathArray<T>& x, T y)
{
    return rw_do_pow(x, y);
}

// need complex overload to prevent ambiguous function error
template <class T>
RWMathArray<std::complex<T> > pow(const RWMathArray<std::complex<T> >& x, std::complex<T> y)
{
    return rw_do_pow(x, y);
}


template <class T>
RWMathArray<T> rw_do_pow(T base, const RWMathArray<T>& expo)
// base to the expo power, scalar to Array.
{
    RWMathArray<T> temp(expo.length(), rwUninitialized, RWDataView::COLUMN_MAJOR);
    typename RWMathArray<T>::const_iterator expoBegin = expo.begin();
    typename RWMathArray<T>::const_iterator expoEnd = expo.end();
    typename RWMathArray<T>::iterator tempIter = temp.begin();

    RW_pow<T, T, T> rwPow;

    while (expoBegin != expoEnd) {
        *tempIter++ = rwPow(base, *expoBegin++);
    }

    return temp;
}

template <class T>
RWMathArray<T> pow(T x, const RWMathArray<T>& y)
{
    return rw_do_pow(x, y);
}

// need complex overload to prevent ambiguous function error
template <class T>
RWMathArray<std::complex<T> > pow(std::complex<T> x, const RWMathArray<std::complex<T> >& y)
{
    return rw_do_pow(x, y);
}


template <class T>
T sum(const RWMathArray<T>& s)
{
    T t = 0;
    for (ArrayLooper l(s.length(), s.stride()); l; ++l) {
        register T* sp = (T*)(s.data() + l.start);
        register size_t i = l.length;
        register int j = l.stride;
        while (i--) {
            t += *sp;
            sp += j;
        }
    }
    return t;
}

template <class T>
T mean(const RWMathArray<T>& s)
{
    T theSum = 0;
    for (ArrayLooper l(s.length(), s.stride()); l; ++l) {
        register T* sp = (T*)(s.data() + l.start);
        register size_t i = l.length;
        register int j = l.stride;
        while (i--) {
            theSum += *sp;
            sp += j;
        }
    }

    size_t numPoints = static_cast<unsigned>(prod(s.length()));
    if (theSum == T(0)) {
        return 0;    // Covers the case of zero points
    }
    return static_cast<T>(theSum / static_cast<T>(numPoints));
}


//***************************************************
// Various functions with simple definitions:
//***************************************************

template< class T >
RWMathArray<T>  acos(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_acos<T, T>());
    return tmp;
}

template< class T >
RWMathArray<T>  asin(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_asin<T, T>());
    return tmp;
}

template< class T >
RWMathArray<T>  atan(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_atan<T, T>());
    return tmp;
}

template< class T >
RWMathArray<T>  ceil(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_ceil<T, T>());
    return tmp;
}

template< class T >
RWMathArray<T>  cos(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_cos<T, T>());
    return tmp;
}

template< class T >
RWMathArray<T>  cosh(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_cosh<T, T>());
    return tmp;
}

template< class T >
RWMathArray<T>  exp(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_exp<T, T>());
    return tmp;
}

template< class T >
RWMathArray<T>  floor(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_floor<T, T>());
    return tmp;
}

template< class T >
RWMathArray<T>  log(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_log<T, T>());
    return tmp;
}

template< class T >
RWMathArray<T>  log10(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_log10<T, T>());
    return tmp;
}

template< class T >
RWMathArray<T>  sin(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_sin<T, T>());
    return tmp;
}

template< class T >
RWMathArray<T>  sinh(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_sinh<T, T>());
    return tmp;
}


template< class T >
RWMathArray<T>  sqrt(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_sqrt<T, T>());
    return tmp;
}


template< class T >
RWMathArray<T>  tan(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_tan<T, T>());
    return tmp;
}


template< class T >
RWMathArray<T>  tanh(const RWMathArray<T>& V)
{
    RWMathArray<T> tmp(V.length(), rwUninitialized);
    rw_math_transform(V.begin(), V.end(), tmp.begin(), RW_tanh<T, T>());
    return tmp;
}


/*
template <class T>
RWMathArray<T>  acos(const RWMathArray<T>& V) { return V.apply(::acos); }
template <class T>
RWMathArray<T>  asin(const RWMathArray<T>& V) { return V.apply(::asin); }
template <class T>
RWMathArray<T>  atan(const RWMathArray<T>& V) { return V.apply(::atan); }
template <class T>
RWMathArray<T>  ceil(const RWMathArray<T>& V) { return V.apply(::ceil); }

template <class T>
RWMathArray<T>  cos(const RWMathArray<T>& V) { return V.apply(::cos);  }
template <class T>
RWMathArray<T>  cosh(const RWMathArray<T>& V) { return V.apply(::cosh); }


template <class T>
RWMathArray<T>  exp(const RWMathArray<T>& V) { return V.apply(::exp); }

template <class T>
RWMathArray<T>  floor(const RWMathArray<T>& V) { return V.apply(::floor); }

template <class T>
RWMathArray<T>  log(const RWMathArray<T>& V) { return V.apply(::log); }

template <class T>
RWMathArray<T>  log10(const RWMathArray<T>& V) { return V.apply(::log10); }

template <class T>
RWMathArray<T>  sin(const RWMathArray<T>& V) { return V.apply(::sin); }

template <class T>
RWMathArray<T>  sinh(const RWMathArray<T>& V) { return V.apply(::sinh); }

template <class T>
RWMathArray<T>  tan(const RWMathArray<T>& V) { return V.apply(::tan); }

template <class T>
RWMathArray<T>  tanh(const RWMathArray<T>& V) { return V.apply(::tanh); }


template <class T>
RWMathArray<T>  sqrt(const RWMathArray<T>& V) { return V.apply(::sqrt); }
*/

#endif
