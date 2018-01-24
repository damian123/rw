/*
 * Definitions for RWSVDDivConqCalc
 * $Id: //lapack/13/rw/lapack/svddccalc.cc#1 $
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
#include <rw/math/funcobj.h>
#include <rw/math/genmat.h>
#include <rw/math/mathvec.h>
#include <rw/math/rwalloc.h>

#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

template <class TypeT>
bool RWSVDDivConqCalc<TypeT>::computeSVD(const RWGenMat<TypeT>&    A,
        RWGenMat<TypeT>&          U,
        RWGenMat<TypeT>&          VT,
        RWMathVec<norm_type>& sigma,
        norm_type             tolerance,
        int                   numLeftVecs,
        int                   numRightVecs)
{
    // Make a deep copy of the input matrix to pass to the LAPACK routine
    // as it will be destroyed by the routine.
    RWGenMat<TypeT> Acopy = A.copy();

    // set up parameters for calling gesdd
    RW_ASSERT(Acopy.rows() <= RW_LAPACK_INT_MAX);
    RW_ASSERT(Acopy.cols() <= RW_LAPACK_INT_MAX);
    rw_lapack_int_t m = RW_STATIC_CAST(rw_lapack_int_t, Acopy.rows());
    rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, Acopy.cols());
    rw_lapack_int_t minMN = n < m ? n : m;

    char jobz;
    bool computeDefaultNumVecs = (numRightVecs < 0 && numLeftVecs < 0);
    if (numRightVecs == 0 && numLeftVecs == 0) {
        U.reshape(RW_STATIC_CAST(size_t, m), 0);
        VT.reshape(0, RW_STATIC_CAST(size_t, n));
        jobz = 'N';
    }
    else if (computeDefaultNumVecs ||
             (numRightVecs <= minMN && numLeftVecs <= minMN)) {
        jobz = 'S';
        U.reshape(RW_STATIC_CAST(size_t, m), RW_STATIC_CAST(size_t, minMN));
        VT.reshape(RW_STATIC_CAST(size_t, minMN), RW_STATIC_CAST(size_t, n));
    }
    else { // Compute all
        jobz = 'A';
        U.reshape(RW_STATIC_CAST(size_t, m), RW_STATIC_CAST(size_t, m));
        VT.reshape(RW_STATIC_CAST(size_t, n), RW_STATIC_CAST(size_t, n));
    }

    sigma.reshape(RW_STATIC_CAST(size_t, minMN));
    bool success = callGesdd(jobz, Acopy, sigma, U, VT);
    if (!success) {
        U.reshape(RW_STATIC_CAST(size_t, m), 0);
        VT.reshape(0, RW_STATIC_CAST(size_t, n));
        return false;
    }

    // Truncate in accordance with the specified tolerance.
    RWPRECONDITION(tolerance >= 0);
    size_t r = sigma.length();
    while (r > 0 && sigma((unsigned)r - 1) <= tolerance) {
        --r;
    }
    sigma.resize(r);
    if (numLeftVecs < 0) {
        if (U.cols() > r) {
            U.resize(U.rows(), r);
        }
    }
    else if ((unsigned)numLeftVecs < U.cols()) {
        U.resize(U.rows(), (unsigned)numLeftVecs);
    }
    if (numRightVecs < 0) {
        if (VT.rows() > r) {
            VT.resize(r, VT.cols());
        }
    }
    else if ((unsigned)numRightVecs < VT.rows()) {
        VT.resize((unsigned)numRightVecs, VT.cols());
    }

    return true;
}

template<>
inline bool RWSVDDivConqCalc<DComplex>::callGesdd(char                  jobz,
        RWGenMat<DComplex>    A,
        RWMathVec<double>& sigma,
        RWGenMat<DComplex>&   U,
        RWGenMat<DComplex>&   VT)
{
    return callGesddCpxSpecl(jobz, A, sigma, U, VT);
}


template< class TypeT >
bool RWSVDDivConqCalc<TypeT>::callGesdd(char                  jobz,
                                        RWGenMat<TypeT>           A,
                                        RWMathVec<norm_type>& sigma,
                                        RWGenMat<TypeT>&          U,
                                        RWGenMat<TypeT>&          VT)
{
    rw_lapack_int_t info;
    size_t liwork = 8 * (A.cols() < A.rows() ? A.cols() : A.rows());
    RWTWorkBuffer<rw_lapack_int_t> iwork(liwork);
    rw_lapack_int_t lda = A.colStride();
    // strides may be zero if jobz == 'N': compute no singular vectors. But
    // gesdd requires that leading dimensions be >= 1.
    rw_lapack_int_t ldu = (U.colStride() > 0 ? U.colStride() : 1);
    rw_lapack_int_t ldvt = (VT.colStride() > 0 ? VT.colStride() : 1);
    RW_ASSERT(A.rows() <= RW_LAPACK_INT_MAX);
    RW_ASSERT(A.cols() <= RW_LAPACK_INT_MAX);
    rw_lapack_int_t m = RW_STATIC_CAST(rw_lapack_int_t, A.rows());
    rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, A.cols());

    // Query for optimal workspace length.
    rw_lapack_int_t lwork = -1;
    TypeT workLength[1];

    // SPL has a bug in its xgesdd routine. It returns a work length of 1
    // when do workspace queries for jobz == 'N'. So, for SPL we just
    // retrieve the workspace length for computing everything which seems
    // to work.
#  if defined(RW_USING_SUNPERF)
    rw_lapack_int_t ldvt_temp = m < n ? m : n;
    gesdd('S', m, n, A.data(), lda, sigma.data(), U.data(), ldu,
          VT.data(), ldvt_temp, workLength, lwork, iwork.data(), info);
#  else
    gesdd(jobz, m, n, A.data(), lda, sigma.data(), U.data(), ldu,
          VT.data(), ldvt, workLength, lwork, iwork.data(), info);
#  endif
    lwork = (rw_lapack_int_t)workLength[0];

    // Allocate work array and do the work.
    RWPRECONDITION(lwork >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    gesdd(jobz, m, n, A.data(), A.colStride(), sigma.data(),
          U.data(), ldu, VT.data(), ldvt, work.data(),
          lwork, iwork.data(), info);
    RWPOSTCONDITION(info >= 0);
    RWPOSTCONDITION(lwork >= (rw_lapack_int_t)work[0]);
    if (info < 0) {
        return false;
    }
    return true;
}

template< class TypeT >
bool RWSVDDivConqCalc<TypeT>::callGesddCpxSpecl(char                  jobz,
        RWGenMat<TypeT>           A,
        RWMathVec<norm_type>& sigma,
        RWGenMat<TypeT>&          U,
        RWGenMat<TypeT>&          VT)
{
    rw_lapack_int_t info;
    RW_ASSERT(A.rows() <= RW_LAPACK_INT_MAX);
    RW_ASSERT(A.cols() <= RW_LAPACK_INT_MAX);
    rw_lapack_int_t m = RW_STATIC_CAST(rw_lapack_int_t, A.rows());
    rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, A.cols());
    rw_lapack_int_t minMN = n < m ? n : m;
    // strides may be zero if jobz == 'N': compute no singular vectors. But
    // gesdd requires that leading dimensions be >= 1.
    rw_lapack_int_t ldu = (U.colStride() > 0 ? U.colStride() : 1U);
    rw_lapack_int_t ldvt = (VT.colStride() > 0 ? VT.colStride() : 1U);
    rw_lapack_int_t lda = A.colStride();
    RWPRECONDITION(minMN >= 0);
    RWTWorkBuffer<rw_lapack_int_t> iwork(static_cast<size_t>(8 * minMN));
    rw_lapack_int_t lrwork = (jobz == 'N' ? 7 * minMN : 5 * minMN * (minMN + 1));
    RWPRECONDITION(lrwork >= 0);
    RWTWorkBuffer<double> rwork(static_cast<size_t>(lrwork));
    RW_real<TypeT, double> realPart;

    // Query for optimal workspace length.
    rw_lapack_int_t lwork = -1;
    TypeT workLength[1];
    gesdd(jobz, m, n, A.data(), lda, sigma.data(), U.data(), ldu,
          VT.data(), ldvt, workLength, lwork, rwork.data(), iwork.data(), info);

    // Allocate work array and do the work.
    lwork = (rw_lapack_int_t)realPart(workLength[0]);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    gesdd(jobz, m, n, A.data(), lda, sigma.data(), U.data(), ldu,
          VT.data(), ldvt, work.data(), lwork, rwork.data(), iwork.data(), info);
    RWPOSTCONDITION(info >= 0);
    if (info < 0) {
        return false;
    }
    return true;
}

#ifdef _WIN32
#  pragma warning(pop)
#endif
