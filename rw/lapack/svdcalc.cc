/*
 * Defs for RWSVDCalc
 *
 * $Id: //lapack/13/rw/lapack/svdcalc.cc#1 $
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
 */

#include <rw/lapack/lapkfunc.h>
#include <rw/math/funcobj.h>
#include <rw/math/genmat.h>
#include <rw/math/mathvec.h>
#include <rw/math/rwalloc.h>

template <class TypeT>
bool RWSVDCalc<TypeT>::computeSVD(const RWGenMat<TypeT>&          A,
                                  RWGenMat<TypeT>&          U,
                                  RWGenMat<TypeT>&          VT,
                                  RWMathVec<norm_type>& sigma,
                                  norm_type             tolerance,
                                  int                   numLeftVecs,
                                  int                   numRightVecs)
{
    // Extracts the real part of not-necessarily-complex numbers
    RW_real<TypeT, norm_type> realPart;

    // First set up parameters for and then call gebrd to generate
    // bidiagonal reduction of A
    RW_ASSERT(A.rows() <= RW_LAPACK_INT_MAX);
    RW_ASSERT(A.cols() <= RW_LAPACK_INT_MAX);
    rw_lapack_int_t m = RW_STATIC_CAST(rw_lapack_int_t, A.rows());
    rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, A.cols());
    RWGenMat<TypeT> B = A.copy();
    rw_lapack_int_t lda = B.colStride();
    rw_lapack_int_t maxrank = m < n ? m : n;
    RWMathVec<norm_type> d(RW_STATIC_CAST(size_t, maxrank), rwUninitialized);
    RWMathVec<norm_type> e(RW_STATIC_CAST(size_t, maxrank), rwUninitialized);  // one longer than necessary to avoid <0 checks
    RWMathVec<TypeT> tauq(RW_STATIC_CAST(size_t, maxrank), rwUninitialized);
    RWMathVec<TypeT> taup(RW_STATIC_CAST(size_t, maxrank), rwUninitialized);

    rw_lapack_int_t info;
    // Query for optimal work array length
    rw_lapack_int_t lwork = -1;
    TypeT workLength[1];
    gebrd(m, n, B.data(), lda, d.data(), e.data(), tauq.data(), taup.data(), workLength, lwork, info);

    // Allocate work array and do the work. Tack on another 3*n T's so that the
    // work array will always be big enough for xbdsqr to use.  We save a hit
    // on the free store this way.
    lwork = (rw_lapack_int_t)realPart(workLength[0]) + 3 * n;
    RWPRECONDITION(lwork >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    gebrd(m, n, B.data(), lda, d.data(), e.data(), tauq.data(), taup.data(), work.data(), lwork, info);

    RWPOSTCONDITION(info == 0);

    // This condition is probably always true since we queried for optimal
    // work size before hand, but you can never be too careful.
    RWPOSTCONDITION((rw_lapack_int_t)realPart(work[0]) <= lwork);

    // Compute the SVD of B.  If we need any left singular vectors,
    // compute Q, the left vecs of B, if we need any right vectors,
    // compute PT, the right vecs of B.
    char uplo = (m >= n) ? 'U' : 'L';
    rw_lapack_int_t npt = 0;
    rw_lapack_int_t nrq = 0;
    rw_lapack_int_t ldpt = 1;
    rw_lapack_int_t ldq = 1;
    if (numLeftVecs != 0) {
        U.reshape(RW_STATIC_CAST(size_t, maxrank), RW_STATIC_CAST(size_t, maxrank));
        U = (TypeT)0;
        U.diagonal() = (TypeT)1;
        nrq = maxrank;
        ldq = maxrank;
    }
    if (numRightVecs != 0) {
        VT.reshape(RW_STATIC_CAST(size_t, maxrank), RW_STATIC_CAST(size_t, maxrank));
        VT = (TypeT)0;
        VT.diagonal() = (TypeT)1;
        npt = maxrank;
        ldpt = maxrank;
    }
    // In the complex case, work needs to be a real array here.  I just
    // put a cast in, since it should always be OK to case a pointer to
    // a (C-style) array of complex to an array of reals
    bdsqr(uplo, maxrank, npt, nrq, 0, d.data(), e.data(), VT.data(), ldpt, U.data(), ldq, (TypeT*)0, 1, (norm_type*)work.data(), info);
    RWPOSTCONDITION(info >= 0);
    if (info != 0) {
        U.reshape(B.rows(), 0);
        VT.reshape(0, B.cols());
        return false;
    }

    sigma.reference(d);

    // Truncate in accordance with the specified tolerance.
    RWPRECONDITION(tolerance >= 0);
    RW_ASSERT(sigma.length() <= RW_INT_MAX);
    unsigned r = (unsigned)sigma.length();
    while (r > 0 && sigma(r - 1) <= tolerance) {
        --r;
    }
    sigma.resize(r);

    // fill in the singular values
    int rank = RW_STATIC_CAST(int, sigma.length()); // this cast is safe because sigma was resized above
    // compute the left singular vectors, if required
    int p = (numLeftVecs < 0 ? rank : numLeftVecs);
    RW_ASSERT(A.rows() < RW_INT_MAX);
    if (p > RW_STATIC_CAST(int, A.rows())) { // Sanity check for brain dead users.
        p = RW_STATIC_CAST(int, A.rows());
    }
    if (p > 0) {
        U.resize(A.rows(), RW_STATIC_CAST(size_t, p));
        RW_ASSERT(maxrank <= RW_INT_MAX);
        for (int i = RW_STATIC_CAST(int, maxrank); i < p; i++) {
            U(i, i) = TypeT(1); // set the new part of U to identity
        }
        RW_ASSERT(U.rows() <= RW_LAPACK_INT_MAX);
        RW_ASSERT(U.cols() <= RW_LAPACK_INT_MAX);
        RW_ASSERT(B.cols() <= RW_LAPACK_INT_MAX);
        rw_lapack_int_t urows = RW_STATIC_CAST(rw_lapack_int_t, U.rows());
        rw_lapack_int_t ucols = RW_STATIC_CAST(rw_lapack_int_t, U.cols());
        rw_lapack_int_t bcols = RW_STATIC_CAST(rw_lapack_int_t, B.cols());
        mbr('Q', 'L', 'N', urows, ucols, bcols, B.data(), B.colStride(),
            tauq.data(), U.data(), U.colStride(), work.data(), lwork, info);
        RWPOSTCONDITION(info == 0);
    }
    else {
        U.reshape(B.rows(), 0);  // don't want any left vecs
    }

    // compute the right singular vectors, if required
    int q = (numRightVecs < 0 ? rank : numRightVecs);
    RW_ASSERT(A.cols() <= RW_INT_MAX);
    if (q > RW_STATIC_CAST(int, A.cols())) {
        q = RW_STATIC_CAST(int, A.cols());
    }
    if (q > 0) {
        VT.resize(RW_STATIC_CAST(size_t, q), B.cols());
        for (int i = RW_STATIC_CAST(int, maxrank); i < q; i++) {
            VT(i, i) = TypeT(1); // set the new part of VT to identity
        }
        RW_ASSERT(VT.rows() <= RW_LAPACK_INT_MAX);
        RW_ASSERT(VT.cols() <= RW_LAPACK_INT_MAX);
        RW_ASSERT(B.rows() <= RW_LAPACK_INT_MAX);
        rw_lapack_int_t vtrows = RW_STATIC_CAST(rw_lapack_int_t, VT.rows());
        rw_lapack_int_t vtcols = RW_STATIC_CAST(rw_lapack_int_t, VT.cols());
        rw_lapack_int_t brows = RW_STATIC_CAST(rw_lapack_int_t, B.rows());
        mbr('P', 'R', *lapk_param_traits<TypeT>::char_param, vtrows, vtcols, brows,
            B.data(), B.colStride(), taup.data(), VT.data(), VT.colStride(),
            work.data(), lwork, info);
        RWPOSTCONDITION(info == 0);
    }
    else {
        VT.reshape(0, B.cols()); // don't want any right vecs
    }

    return true;
}

