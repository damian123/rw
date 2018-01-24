#ifndef __MATMATH_CC__
#define __MATMATH_CC__

/***************************************************************************
 *
 * matmath.cc
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

#include <rw/math/genmat.h>
#include <rw/math/mathvec.h>
#include <rw/math/rwbla.h>
#include <rw/math/numtrait.h>
#include <rw/math/funcobj.h>


#define REGISTER register

#ifndef RW_INLINE_FUNC_PTR_ARG
// Apply function returning T to each element
template <class T>
RWGenMat<T>
RWGenMat<T>::apply(typename RWGenMat<T>::mathFunType f) const
{
    RWGenMat<T> temp(nrows, ncols, rwUninitialized);
    typename RWGenMat<T>::const_iterator it;
    typename RWGenMat<T>::iterator it_temp;
    typename RWGenMat<T>::const_iterator last = end();
    for (it = begin(), it_temp = temp.begin(); it < last; ++it, ++it_temp) {
        *it_temp = (*f)(*it);
    }
    return temp;
}
#endif

#ifndef RW_INLINE_TRAITS_TEMPLATES
template <class T>
RWGenMat<typename RWGenMat<T>::norm_type> RWGenMat<T>::apply2(typename RWGenMat<T>::mathFunType2 f) const
{
    RWGenMat<norm_type> temp(rows(), cols(), rwUninitialized);
    register norm_type* dp = (norm_type*)temp.data();
    typename RWGenMat<T>::const_iterator it;
    typename RWGenMat<norm_type>::iterator it_temp;
    typename RWGenMat<T>::const_iterator last = end();
    for (it = begin(), it_temp = temp.begin(); it < last; ++it, ++it_temp) {
        *it_temp = (*f)(*it);
    }
    return temp;
}
#endif

#if defined(RW_NATIVE_EXTENDED)
template <class T>
RWGenMat<T>
RWGenMat<T>::apply(RWGenMat<T>::XmathFunType f) const
// Apply function for compilers using extended types
{
    RWGenMat<T> temp(nrows, ncols, rwUninitialized);
    RWGenMat<T>::const_iterator it;
    RWGenMat<T>::iterator it_temp;
    RWGenMat<T>::const_iterator last = end();
    for (it = begin(), it_temp = temp.begin(); it < last; ++it, ++it_temp) {
        *it_temp = (*f)(*it);
    }
    return temp;
}
#endif


template <class T>
RWGenMat<T> atan2(const RWGenMat<T>& u, const RWGenMat<T>& v)
// Arctangent of u/v.
{
    u.lengthCheck(v.rows(), v.cols());
    RWGenMat<T> w(u.rows(), u.cols(), rwUninitialized);
    rw_math_transform(u.begin(), u.end(), v.begin(), w.begin(), RW_atan2<T, T, T>());
    return w;
}

template <class T>
T dot(const RWGenMat<T>& u, const RWGenMat<T>& v)
// dot product: sum_ij (u_ij * v_ij)
{
    u.lengthCheck(v.rows(), v.cols());
    T result = 0;
    typename RWGenMat<T>::const_iterator it_u, it_v;
    typename RWGenMat<T>::const_iterator last = u.end();
    const T* udata;
    const T* vdata;
    for (it_u = u.begin(), it_v = v.begin(); it_u < last; INC_ROW(it_u), INC_ROW(it_v)) {
        T partialResult;
        udata = &*it_u;
        vdata = &*it_v;
        if (it_u.stride() < 0) {
            (udata += (it_u.row_length() - 1) * it_u.stride());
        }
        if (it_v.stride() < 0) {
            (vdata += (it_u.row_length() - 1) * it_v.stride());
        }

        partialResult = RWBlas<T>::dot((int)it_u.row_length(), udata, it_u.stride(), vdata, it_v.stride());
        result += partialResult;
    }
    return result;
}

template <class T>
void maxIndex(const RWGenMat<T>& s, int* maxi, int* maxj)
// Index of maximum element.
{
    size_t len = s.rows() * s.cols();
    s.numPointsCheck("max needs at least", 1);
    long tempImax;
    if (static_cast<size_t>(s.colStride()) == s.rowStride()*s.rows()) {  // Column major order
        RW_ASSERT(len <= RW_LONG_MAX);
        tempImax = RWBlas_util<T>::imax(static_cast<long>(len), (T*)s.data(), s.rowStride());
        RW_LAPACK_INT_MAX_ASSERT(tempImax);
        int maxindex = static_cast<int>(tempImax);
        *maxj = static_cast<int>(maxindex / s.rows());
        *maxi = static_cast<int>(maxindex % s.rows());
        return;
    }
    if (static_cast<size_t>(s.rowStride()) == s.colStride()*s.cols()) {  // Row major order
        RW_ASSERT(len <= RW_LONG_MAX);
        tempImax = RWBlas_util<T>::imax(static_cast<long>(len), (T*)s.data(), s.colStride());
        RW_LAPACK_INT_MAX_ASSERT(tempImax);
        int maxindex = static_cast<int>(tempImax);
        *maxi = static_cast<int>(maxindex / s.cols());
        *maxj = static_cast<int>(maxindex % s.cols());
        return;
    }
    *maxi = *maxj = 0;                        // Matrix is not contiguous
    T themax = s(0, 0);
    if (s.colStride() > s.rowStride()) {
        RW_ASSERT(s.cols() <= RW_INT_MAX);
        RW_ASSERT(s.rows() <= RW_LONG_MAX);
        for (int j = 0; j < static_cast<int>(s.cols()); j++) { // Do a column at a time
            tempImax = RWBlas_util<T>::imax(static_cast<long>(s.rows()), (T*)(s.data() + j * s.colStride()), s.rowStride());
            RW_LAPACK_INT_MAX_ASSERT(tempImax);
            int i = static_cast<int>(tempImax);
            if (s(i, j) > themax) {
                themax = s(i, j);
                *maxi = i;
                *maxj = j;
            }
        }
    }
    else {
        RW_ASSERT(s.cols() <= RW_LONG_MAX);
        RW_ASSERT(s.rows() <= RW_INT_MAX);
        for (int i = 0; i < static_cast<int>(s.rows()); i++) { // Do a row at a time
            tempImax = RWBlas_util<T>::imax(static_cast<long>(s.cols()), (T*)(s.data() + i * s.rowStride()), s.colStride());
            RW_LAPACK_INT_MAX_ASSERT(tempImax);
            int j = static_cast<int>(tempImax);
            if (s(i, j) > themax) {
                themax = s(i, j);
                *maxi = i;
                *maxj = j;
            }
        }
    }
}

template <class T>
T maxValue(const RWGenMat<T>& s)
{
    int i, j;
    maxIndex(s, &i, &j);
    return s(i, j);
}

template <class T>
void minIndex(const RWGenMat<T>& s, int* mini, int* minj)
// Index of minimum element.
{
    size_t len = s.rows() * s.cols();
    s.numPointsCheck("min needs at least", 1);
    long tempImin;
    if (static_cast<size_t>(s.colStride()) == s.rowStride()*s.rows()) {  // Column major order
        RW_ASSERT(len <= RW_LONG_MAX);
        tempImin = RWBlas_util<T>::imin(static_cast<long>(len), (T*)s.data(), s.rowStride());
        RW_LAPACK_INT_MAX_ASSERT(tempImin);
        int minindex = static_cast<int>(tempImin);
        *minj = static_cast<int>(minindex / s.rows());
        *mini = static_cast<int>(minindex % s.rows());
        return;
    }
    if (static_cast<size_t>(s.rowStride()) == s.colStride()*s.cols()) {  // Row major order
        RW_ASSERT(len <= RW_LONG_MAX);
        tempImin = RWBlas_util<T>::imin(static_cast<long>(len), (T*)s.data(), s.colStride());
        RW_LAPACK_INT_MAX_ASSERT(tempImin);
        int minindex = static_cast<int>(tempImin);
        *mini = static_cast<int>(minindex / s.cols());
        *minj = static_cast<int>(minindex % s.cols());
        return;
    }
    *mini = *minj = 0;                        // Matrix is not contiguous
    T themin = s(0, 0);
    if (s.colStride() > s.rowStride()) {
        RW_ASSERT(s.cols() <= RW_INT_MAX);
        RW_ASSERT(s.rows() <= RW_LONG_MAX);
        for (int j = 0; j < static_cast<int>(s.cols()); j++) { // Do a column at a time
            tempImin = RWBlas_util<T>::imin(static_cast<long>(s.rows()), (T*)(s.data() + j * s.colStride()), s.rowStride());
            RW_LAPACK_INT_MAX_ASSERT(tempImin);
            int i = static_cast<int>(tempImin);
            if (s(i, j) < themin) {
                themin = s(i, j);
                *mini = i;
                *minj = j;
            }
        }
    }
    else {
        RW_ASSERT(s.cols() <= RW_LONG_MAX);
        RW_ASSERT(s.rows() <= RW_INT_MAX);
        for (int i = 0; i < static_cast<int>(s.rows()); i++) { // Do a row at a time
            tempImin = RWBlas_util<T>::imin(static_cast<long>(s.cols()), (T*)(s.data() + i * s.rowStride()), s.colStride());
            RW_LAPACK_INT_MAX_ASSERT(tempImin);
            int j = static_cast<int>(tempImin);
            if (s(i, j) < themin) {
                themin = s(i, j);
                *mini = i;
                *minj = j;
            }
        }
    }
}

template <class T>
T minValue(const RWGenMat<T>& s)
{
    int i, j;
    minIndex(s, &i, &j);
    return s(i, j);
}


template <class T>
T prod(const RWGenMat<T>& s)
{
    REGISTER T t = 1;
    typename RWGenMat<T>::const_iterator it;
    typename RWGenMat<T>::const_iterator last = s.end();
    for (it = s.begin(); it < last; ++it) {
        t *= *it;
    }
    return t;
}

template <class T>
RWGenMat<T> pow(const RWGenMat<T>& u, const RWGenMat<T>& v)
// u to the v power.
{
    u.lengthCheck(v.rows(), v.cols());
    RWGenMat<T> temp(v.rows(), v.cols(), rwUninitialized);
    rw_math_transform(u.begin(), u.end(), v.begin(), temp.begin(), RW_pow<T, T, T>());
    return temp;
}

template <class T>
RWGenMat<T> rw_do_pow(const RWGenMat<T>& base, T expo)
{
    // base to the expo power, Matrices to scalar.
    RWGenMat<T> temp(base.rows(), base.cols(), rwUninitialized);
    typename RWGenMat<T>::const_iterator baseBegin = base.begin();
    typename RWGenMat<T>::const_iterator baseEnd = base.end();
    typename RWGenMat<T>::iterator tempIter = temp.begin();

    RW_pow<T, T, T> rwPow;

    while (baseBegin != baseEnd) {
        *tempIter++ = rwPow(*baseBegin++, expo);
    }

    return temp;
}

template <class T>
RWGenMat<T> pow(const RWGenMat<T>& x, T y)
{
    return rw_do_pow(x, y);
}

// need complex overload to prevent ambiguous function error
template <class T>
RWGenMat<std::complex<T> > pow(const RWGenMat<std::complex<T> >& x, std::complex<T> y)
{
    return rw_do_pow(x, y);
}


template <class T>
RWGenMat<T> rw_do_pow(T base, const RWGenMat<T>& expo)
// base to the expo power,scalar to Matrices.
{
    RWGenMat<T> temp(expo.rows(), expo.cols(), rwUninitialized);
    typename RWGenMat<T>::const_iterator expoBegin = expo.begin();
    typename RWGenMat<T>::const_iterator expoEnd = expo.end();
    typename RWGenMat<T>::iterator tempIter = temp.begin();

    RW_pow<T, T, T> rwPow;

    while (expoBegin != expoEnd) {
        *tempIter++ = rwPow(base, *expoBegin++);
    }

    return temp;
}

template <class T>
RWGenMat<T> pow(T x, const RWGenMat<T>& y)
{
    return rw_do_pow(x, y);
}

// need complex overload to prevent ambiguous function error
template <class T>
RWGenMat<std::complex<T> > pow(std::complex<T> x, const RWGenMat<std::complex<T> >& y)
{
    return rw_do_pow(x, y);
}

template <class T>
T sum(const RWGenMat<T>& s)
{
    typename rw_numeric_traits<T>::calc_type t = 0;
    typename RWGenMat<T>::const_iterator it = s.begin();
    typename RWGenMat<T>::const_iterator last = s.end();
    RW_ASSERT(s.data() != 0 || it == last);
    for (; it < last; ++it) {
        t += *it;
    }
    return T(t);
}

template <class T>
T mean(const RWGenMat<T>& s)
{
    T theSum = sum(s);
    T numPoints = static_cast<T>(s.rows() * s.cols());
    if (theSum == T(0)) {
        return T(0);    // Covers the case of zero points
    }
    return theSum / numPoints;
}


template <class T>
RWGenMat<T> transpose(const RWGenMat<T>& A)
{
    return A.slice(0, 0, A.cols(), A.rows(), 0, 1, 1, 0);
}

template <class T>
RWMathVec<T> product(const RWGenMat<T>& A, const RWMathVec<T>& x)
{
    RWMathVec<T> y(A.rows(), rwUninitialized);
    product(A, x, y);
    return y;
}

template <class T>
void product(const RWGenMat<T>& A, const RWMathVec<T>& x, RWMathVec<T>& y)
{
    size_t m = 0;
    size_t n = 0;
    size_t numRows = A.rows();
    size_t numCols = A.cols();
    char Atrans;
    long ldA = 0;
    x.lengthCheck(numCols);
    y.lengthCheck(numRows);

    // First check to see if matrix has "compact stride". This means that
    // either the row stride is 1 and the column stride is equal to the
    // number of rows (COLUMN_MAJOR storage), or the column stride is 1 and
    // the row stride is equal to the number of columns (ROW_MAJOR storage).
    // If this is the case we can call the BLAS routine directly which is
    // MUCH faster.

    if (A.rowStride() == 1 && A.colStride() == (int)numRows) {
        RW_ASSERT(numRows <= RW_LONG_MAX);
        Atrans = 'N';
        m = numRows;
        n = numCols;
        ldA = static_cast<long>(numRows);
    }
    else if (A.colStride() == 1 && A.rowStride() == (int)numCols) {
        RW_ASSERT(numCols <= RW_LONG_MAX);
        Atrans = 'T';
        m = numCols;
        n = numRows;
        ldA = static_cast<long>(numCols);
    }

    if (ldA != 0) {
        y = T(0);
        RWBlas<T>::gemv(Atrans, static_cast<long>(m), static_cast<long>(n), 1, (T*)A.data(), ldA, (T*)x.data(), x.stride(), 1,
                        (T*)y.data(), 1);
        return;
    }

    T* xdata;
    const T* Aptr = A.data();
    T* Adata;

    for (int i = 0; i < (int)numRows; i++) {
        xdata = (T*) x.data();
        Adata = (T*) Aptr;
        if (x.stride() < 0) {
            (xdata += numCols * x.stride());
        }
        if (A.colStride() < 0) {
            (Adata += numCols * A.colStride());
        }
        y(i) = RWBlas<T>::dot((int)numCols, xdata, x.stride(), Adata, A.colStride());
        Aptr += A.rowStride();
    }
}

template <class T>
RWGenMat<T> transposeProduct(const RWGenMat<T>& A, const RWGenMat<T>& B)
{
    return product(transpose(A), B);
}

template <class T>
void transposeProduct(const RWGenMat<T>& A, const RWGenMat<T>& B, RWGenMat<T>& AB)
{
    product(transpose(A), B, AB);
}


template <class T>
RWMathVec<T> product(const RWMathVec<T>& x, const RWGenMat<T>& A)
{
    return product(transpose(A), x);
}

template <class T>
void product(const RWMathVec<T>& x, const RWGenMat<T>& A, RWMathVec<T>& y)
{
    product(transpose(A), x, y);
}

template <class T>
RWGenMat<T> product(const RWGenMat<T>& A, const RWGenMat<T>& B)
{
    RWGenMat<T> AB(A.rows(), B.cols(), rwUninitialized);
    product(A, B, AB);
    return AB;
}

template <class T>
void product(const RWGenMat<T>& A, const RWGenMat<T>& B, RWGenMat<T>& AB)
{
    char Atrans, Btrans;
    long ldA = 0, ldB = 0;
    size_t m = A.rows();
    size_t n = B.cols();
    size_t r = A.cols();  // == B.rows() if A,B conform
    B.rowCheck(r);
    AB.lengthCheck(m, n);

    // First check to see if matrices have "compact stride". This means that
    // either the row stride is 1 and the column stride is equal to the
    // number of rows (COLUMN_MAJOR storage), or the column stride is 1 and
    // the row stride is equal to the number of columns (ROW_MAJOR storage).
    // If this is the case we can call the BLAS routine directly which is
    // MUCH faster.
    RW_ASSERT(m <= RW_LONG_MAX);
    RW_ASSERT(r <= RW_LONG_MAX);
    RW_ASSERT(n <= RW_LONG_MAX);
    if (A.rowStride() == 1 && A.colStride() == (int)m) {
        ldA = static_cast<long>(m);
        Atrans = 'N';
    }
    else if (A.colStride() == 1 && A.rowStride() == (int)r) {
        ldA = static_cast<long>(r);
        Atrans = 'T';
    }
    if (B.rowStride() == 1 && B.colStride() == (int)B.rows()) {
        RW_ASSERT(B.rows() <= RW_LONG_MAX);
        ldB = static_cast<long>(B.rows());
        Btrans = 'N';
    }
    else if (B.colStride() == 1 && B.rowStride() == (int)n) {
        ldB = static_cast<long>(n);
        Btrans = 'T';
    }

    if (ldA != 0 && ldB != 0) {
        AB = T(0);
        RW_ASSERT(AB.rows() <= RW_LONG_MAX);
        RWBlas<T>::gemm(Atrans, Btrans, static_cast<long>(m), static_cast<long>(n), static_cast<long>(r), 1, (T*)A.data(), ldA,
                        (T*)B.data(), ldB, 1, (T*)AB.data(), static_cast<long>(AB.rows()));
        return;
    }

    RWMathVec<T> Arow(r, rwUninitialized);
    for (int i = 0; (size_t)i < m; i++) {
        Arow = A.row(i);  // Compact this row to minimize page faults
        const T* Bptr = B.data();
        T* Bdata;
        for (int j = 0; (size_t)j < n; j++) {
            Bdata = (T*) Bptr;
            if (B.rowStride() < 0) {
                (Bdata += r * B.rowStride());
            }
            AB(i, j) = RWBlas<T>::dot((int)r, (T*)Arow.data(), 1, Bdata, B.rowStride());
            Bptr += B.colStride();
        }
    }
    return;
}

/*
 * Various functions with simple definitions:
 */
template <class T>
RWGenMat<T> acos(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_acos<T, T>());
    return N;
}

template <class T>
RWGenMat<T> asin(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_asin<T, T>());
    return N;
}

template <class T>
RWGenMat<T> atan(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_atan<T, T>());
    return N;
}

template <class T>
RWGenMat<T> ceil(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_ceil<T, T>());
    return N;
}

template <class T>
RWGenMat<T> cos(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_cos<T, T>());
    return N;
}

template <class T>
RWGenMat<T> cosh(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_cosh<T, T>());
    return N;
}

template <class T>
RWGenMat<T> exp(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_exp<T, T>());
    return N;
}

template <class T>
RWGenMat<T> floor(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_floor<T, T>());
    return N;
}

template <class T>
RWGenMat<T> log(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    typename RWGenMat<T>::const_iterator beginM = M.begin();
    typename RWGenMat<T>::const_iterator endM = M.end();
    rw_math_transform(beginM, endM, N.begin(), RW_log<T, T>());
    return N;
}

template <class T>
RWGenMat<T> log10(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_log10<T, T>());
    return N;
}

template <class T>
RWGenMat<T> sin(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_sin<T, T>());
    return N;
}

template <class T>
RWGenMat<T> sinh(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_sinh<T, T>());
    return N;
}

template <class T>
RWGenMat<T> tan(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_tan<T, T>());
    return N;
}

template <class T>
RWGenMat<T> tanh(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_tanh<T, T>());
    return N;
}

RWGenMat<double> RW_MATH_SYMBOLIC arg(const RWGenMat<DComplex>& M);
RWGenMat<double> RW_MATH_SYMBOLIC imag(const RWGenMat<DComplex>& M);
RWGenMat<double> RW_MATH_SYMBOLIC real(const RWGenMat<DComplex>& M);
RWGenMat<DComplex> RW_MATH_SYMBOLIC conj(const RWGenMat<DComplex>& M);
RWGenMat<double> RW_MATH_SYMBOLIC norm(const RWGenMat<DComplex>& M);
RWGenMat<double> RW_MATH_SYMBOLIC abs(const RWGenMat<DComplex>& M);
RWGenMat<double> RW_MATH_SYMBOLIC abs(const RWGenMat<double>& M);
RWGenMat<float> RW_MATH_SYMBOLIC abs(const RWGenMat<float>& M);
RWGenMat<int> RW_MATH_SYMBOLIC abs(const RWGenMat<int>& M);
RWGenMat<SChar> RW_MATH_SYMBOLIC abs(const RWGenMat<SChar>& M);

template <class T>
RWGenMat<T> sqrt(const RWGenMat<T>& M)
{
    RWGenMat<T> N(M.rows(), M.cols(), rwUninitialized);
    rw_math_transform(M.begin(), M.end(), N.begin(), RW_sqrt<T, T>());
    return N;
}

/**** Norm functions ****/

double RW_MATH_SYMBOLIC l1Norm(const RWGenMat<DComplex>& x);
double RW_MATH_SYMBOLIC l1Norm(const RWGenMat<double>& x);
float RW_MATH_SYMBOLIC l1Norm(const RWGenMat<float>& x);
double RW_MATH_SYMBOLIC linfNorm(const RWGenMat<DComplex>& x);
double RW_MATH_SYMBOLIC linfNorm(const RWGenMat<double>& x);
float RW_MATH_SYMBOLIC linfNorm(const RWGenMat<float>& x);
double RW_MATH_SYMBOLIC maxNorm(const RWGenMat<DComplex>& x);
double RW_MATH_SYMBOLIC maxNorm(const RWGenMat<double>& x);
float RW_MATH_SYMBOLIC maxNorm(const RWGenMat<float>& x);
double RW_MATH_SYMBOLIC frobNorm(const RWGenMat<DComplex>& x);
double RW_MATH_SYMBOLIC frobNorm(const RWGenMat<double>& x);
float RW_MATH_SYMBOLIC frobNorm(const RWGenMat<float>& x);

#endif // __MATMATH_CC__
