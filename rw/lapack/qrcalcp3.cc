#ifndef __RWQRCALCP3_CC__
#define __RWQRCALCP3_CC__

/*
 * Definitions for RWQRCalcP3 class
 *
 * $Id: //lapack/13/rw/lapack/qrcalcp3.cc#1 $
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
 */

#include <rw/lapack/lapkfunc.h>
#include <rw/math/genmat.h>
#include <rw/math/mathvec.h>
#include <rw/math/rwalloc.h>

template < class TypeT >
bool RWQRCalcP3<TypeT>::call_geqp3(RWGenMat<TypeT>& A, RWMathVec<TypeT>& tau, rw_lapack_int_t* const pivots)
{
    RW_ASSERT(A.rows() <= RW_LAPACK_INT_MAX);
    RW_ASSERT(A.cols() <= RW_LAPACK_INT_MAX);
    rw_lapack_int_t m = RW_STATIC_CAST(rw_lapack_int_t, A.rows());
    rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, A.cols());
    rw_lapack_int_t info;
    rw_lapack_int_t lwork = -1;

    TypeT workLength[1];
    geqp3(m, n, A.data(), m, pivots, tau.data(), workLength, lwork, info);
    lwork = (rw_lapack_int_t)workLength[0];
    RWPRECONDITION(lwork >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    geqp3(m, n, A.data(), m, pivots, tau.data(), work.data(), lwork, info);
    return(info == 0 ? true : false);
}

template <>
inline bool RWQRCalcP3<DComplex>::call_geqp3(RWGenMat<DComplex>& A, RWMathVec<DComplex>& tau, rw_lapack_int_t* const pivots)
{
    return call_geqp3_specl(A, tau, pivots);
}

template< class TypeT >
bool RWQRCalcP3<TypeT>::call_geqp3_specl(RWGenMat<TypeT>& A, RWMathVec<TypeT>& tau, rw_lapack_int_t* const pivots)
{
    RW_real<TypeT, norm_type> realPart;
    RW_ASSERT(A.rows() <= RW_LAPACK_INT_MAX);
    RW_ASSERT(A.cols() <= RW_LAPACK_INT_MAX);
    rw_lapack_int_t m = RW_STATIC_CAST(rw_lapack_int_t, A.rows());
    rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, A.cols());
    rw_lapack_int_t info;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<norm_type> rwork(static_cast<size_t>(2 * n));
    rw_lapack_int_t lwork = -1;
    TypeT workLength[1];

    geqp3(m, n, A.data(), m, pivots, tau.data(), workLength, lwork, rwork.data(), info);
    lwork = (rw_lapack_int_t)realPart(workLength[0]);
    RWPRECONDITION(lwork >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    geqp3(m, n, A.data(), m, pivots, tau.data(), work.data(), lwork, rwork.data(), info);
    return(info == 0 ? true : false);
}



// On entry, A is the M-by-N matrix for which the QR decomposition is being
// computed. On exit, the upper triangle of the array contains the
// min(M,N)-by-N upper triangular matrix R; the elements below the diagonal,
// together with the vector tau, represent the orthogonal matrix Q as a
// product of min(m,n) elementary reflectors.
//
// If pivots != null, then on entry, if pivots[i] != 0, the i-th column
// of A is permuted to the front of A*P (a leading column); if pivots(i) == 0,
// the i-th column of A is a free column. On exit, if pivots[i] = k, then the
// i-th column of A*P was the k-th column of A.
// If pivots == null, pivoting is not done.
template< class TypeT >
bool RWQRCalcP3<TypeT>::_computeQR(RWGenMat<TypeT>& A, RWMathVec<TypeT>& tau, rw_lapack_int_t* const pivots)
{
    RW_real<TypeT, norm_type> realPart;
    RW_ASSERT(A.rows() <= RW_LAPACK_INT_MAX);
    RW_ASSERT(A.cols() <= RW_LAPACK_INT_MAX);
    rw_lapack_int_t m = RW_STATIC_CAST(rw_lapack_int_t, A.rows());
    rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, A.cols());
    rw_lapack_int_t info;
    size_t minMN = m < n ? RW_STATIC_CAST(size_t, m) : RW_STATIC_CAST(size_t, n);
    if (tau.length() != minMN) {
        tau.reshape(minMN);
    }
    if (pivots != rwnil) {
        return call_geqp3(A, tau, pivots);
    }

    // Query for optimal work length
    rw_lapack_int_t lwork = -1;
    TypeT workLength[1];
    geqrf(m, n, A.data(), m, tau.data(), workLength, lwork, info);

    // Allocate the work array and do the work.
    lwork = (rw_lapack_int_t)realPart(workLength[0]);
    RWPRECONDITION(lwork >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    geqrf(m, n, A.data(), m, tau.data(), work.data(), lwork, info);
    RWPOSTCONDITION(info == 0);
    RWPOSTCONDITION((rw_lapack_int_t)realPart(work[0]) <= lwork);
    return(info == 0 ? true : false);
}

template< class TypeT >
bool RWQRCalcP3<TypeT>::computeQR(RWGenMat<TypeT>& A, RWMathVec<TypeT>& tau, int* const pivots)
{
#if RW_SIZEOF_INT == RW_SIZEOF_LAPACK_INT
    return _computeQR(A, tau, reinterpret_cast<rw_lapack_int_t*>(pivots));
#else
    if (0 == pivots) {
        return _computeQR(A, tau, 0);
    }

    RWTWorkBuffer<rw_lapack_int_t> tmpPivots(A.cols());
    for (size_t i = 0; i < A.cols(); i++) {
        tmpPivots[i] = static_cast<rw_lapack_int_t>(pivots[i]);
    }
    bool retVal = _computeQR(A, tau, tmpPivots.data());
    for (size_t i = 0; i < A.cols(); i++) {
        pivots[i] = static_cast<int>(tmpPivots[i]);
    }
    return retVal;
#endif
}

template< class TypeT >
bool RWQRCalcP3<TypeT>::computeQR(RWGenMat<TypeT>& A, RWMathVec<TypeT>& tau, long* const pivots)
{
#if RW_SIZEOF_LONG_INT == RW_SIZEOF_LAPACK_INT
    return _computeQR(A, tau, reinterpret_cast<rw_lapack_int_t*>(pivots));
#else
    if (0 == pivots) {
        return _computeQR(A, tau, 0);
    }

    RWTWorkBuffer<rw_lapack_int_t> tmpPivots(A.cols());
    for (size_t i = 0; i < A.cols(); i++) {
        tmpPivots[i] = static_cast<rw_lapack_int_t>(pivots[i]);
    }
    bool retVal = _computeQR(A, tau, tmpPivots.data());
    for (size_t i = 0; i < A.cols(); i++) {
        pivots[i] = static_cast<long>(tmpPivots[i]);
    }
    return retVal;
#endif
}

#endif // __RWQRCALCP3_CC__
