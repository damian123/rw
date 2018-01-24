/*
 * RWHessenbergDecomp - Hessenberg decomposition of a matrix
 *
 * This decomposition is usually formed as a precursor to computing
 * the eigenvalues and/or Schur decomposition of a non-symmetric matrix.
 *
 * This class includes an optional balancing transformation in addition
 * to the standard Hessenberg decomposition.
 *
 *
 * $Id: //lapack/13/rw/lapack/hess.cc#1 $
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
#include <rw/math/rwalloc.h>

#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

template <class TypeT>
RWHessenbergDecomp<TypeT>::RWHessenbergDecomp()
{
}

template <class TypeT>
RWHessenbergDecomp<TypeT>::RWHessenbergDecomp(const RWGenMat<TypeT>& A, bool permute, bool scale)
{
    factor(A, permute, scale);
}

template <class TypeT>
RWHessenbergDecomp<TypeT>::RWHessenbergDecomp(const RWBalanceDecomp<TypeT>& A)
{
    factor(A);
}

template <class TypeT>
void RWHessenbergDecomp<TypeT>::factor(const RWGenMat<TypeT>& A, bool permute, bool scale)
{
    RWBalanceDecomp<TypeT> BCB(A, permute, scale);
    factor(BCB);
}

template <class TypeT>
void RWHessenbergDecomp<TypeT>::factor(const RWBalanceDecomp<TypeT>& decomp)
{
    RW_real<TypeT, norm_type> realPart;
    B_ = decomp.B();
    H_.reference(decomp.C());
    H_.deepenShallowCopy();
    tau_.reshape(decomp.rows());
    rw_lapack_int_t info;
    RW_LAPACK_INT_MAX_ASSERT(decomp.rows());
    rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, decomp.rows());

    // Query for optimal size of work array
    rw_lapack_int_t lwork = -1;
    TypeT workLength[1];
    gehrd(n, B_.lowIndex(), B_.highIndex(), H_.data(), n, tau_.data(), workLength, lwork, info);

    // Allocate work array and do the work
    lwork = (rw_lapack_int_t)realPart(workLength[0]);

    RWPRECONDITION(lwork >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    gehrd(n, B_.lowIndex(), B_.highIndex(), H_.data(), n, tau_.data(), work.data(), lwork, info);

    RWPOSTCONDITION(info == 0);
    RWPOSTCONDITION((rw_lapack_int_t)realPart(work[0]) <= lwork);
}

template <class TypeT>
RWGenMat<TypeT>   RWHessenbergDecomp<TypeT>::B() const
{
    RWGenMat<TypeT> I(rows(), cols(), (TypeT)0);
    I.diagonal() = (TypeT)1;
    return B_.transform(I);
}

template <class TypeT>
RWGenMat<TypeT>   RWHessenbergDecomp<TypeT>::Q() const
{
    RW_real<TypeT, norm_type> realPart;
    RWGenMat<TypeT> theQ = H_.copy();
    RW_LAPACK_INT_MAX_ASSERT(rows());
    rw_lapack_int_t m = RW_STATIC_CAST(rw_lapack_int_t, rows());
    rw_lapack_int_t info;

    // Query for optimal work space array length
    rw_lapack_int_t lwork = -1;
    TypeT workLength[1];
    ghr(m, B_.lowIndex(), B_.highIndex(), theQ.data(), rows(), (TypeT*)tau_.data(), workLength, lwork, info);

    // Allocate the work array and do the work.
    lwork = (rw_lapack_int_t)realPart(workLength[0]);

    RWPRECONDITION(lwork >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    ghr(m, B_.lowIndex(), B_.highIndex(), theQ.data(), rows(), (TypeT*)tau_.data(), work.data(), lwork, info);

    RWPOSTCONDITION(info == 0);
    RWPOSTCONDITION((rw_lapack_int_t)realPart(work[0]) <= lwork);

    return theQ;
}

template <class TypeT>
RWGenMat<TypeT>   RWHessenbergDecomp<TypeT>::H()                     const
{
    RWGenMat<TypeT> A = H_.copy();
    for (int i = 2; (unsigned)i < rows(); i++) {
        A.diagonal(-i) = (TypeT)0;
    }
    return A;
}

template <class TypeT>
RWMathVec<TypeT>      RWHessenbergDecomp<TypeT>::Bx(const RWMathVec<TypeT>& x)       const
{
    return B_.transform(x);
}

template <class TypeT>
RWMathVec<TypeT>      RWHessenbergDecomp<TypeT>::BInvTx(const RWMathVec<TypeT>& x)   const
{
    return B_.invTransform(x);
}

template <class TypeT>
RWMathVec<TypeT>      RWHessenbergDecomp<TypeT>::Qx(const RWMathVec<TypeT>& x)      const
{
    RWGenMat<TypeT> X(x, x.length(), 1);
    RWGenMat<TypeT> theQX = QX(X);
    RWPOSTCONDITION(theQX.cols() == 1);
    return theQX.col(0);
}

template <class TypeT>
RWMathVec<TypeT>      RWHessenbergDecomp<TypeT>::QTx(const RWMathVec<TypeT>& x)      const
{
    RWGenMat<TypeT> X(x, x.length(), 1);
    RWGenMat<TypeT> theQTX = QTX(X);
    RWPOSTCONDITION(theQTX.cols() == 1);
    return theQTX.col(0);
}

template <class TypeT>
RWGenMat<TypeT>   RWHessenbergDecomp<TypeT>::BX(const RWGenMat<TypeT>& x)    const
{
    return B_.transform(x);
}

template <class TypeT>
RWGenMat<TypeT>   RWHessenbergDecomp<TypeT>::BInvTX(const RWGenMat<TypeT>& x)const
{
    return B_.invTransform(x);
}

template <class TypeT>
RWGenMat<TypeT>   RWHessenbergDecomp<TypeT>::QX(const RWGenMat<TypeT>& X)   const
{
    RW_real<TypeT, norm_type> realPart;
    RWGenMat<TypeT> C = X.copy();
    rw_lapack_int_t info;

    // Query for optimal work array length
    rw_lapack_int_t lwork = -1;
    RW_ASSERT(C.rows() <= RW_LAPACK_INT_MAX);
    RW_ASSERT(C.cols() <= RW_LAPACK_INT_MAX);
    RW_ASSERT(H_.rows() <= RW_LAPACK_INT_MAX);
    rw_lapack_int_t crows = RW_STATIC_CAST(rw_lapack_int_t, C.rows());
    rw_lapack_int_t ccols = RW_STATIC_CAST(rw_lapack_int_t, C.cols());
    rw_lapack_int_t hrows = RW_STATIC_CAST(rw_lapack_int_t, H_.rows());
    TypeT workLength[1];
    mhr('L', 'N', crows, ccols, B_.lowIndex(), B_.highIndex(), (TypeT*)H_.data(),
        hrows, (TypeT*)tau_.data(), C.data(), crows, workLength, lwork, info);

    // Allocate the work array and do the work
    lwork = (rw_lapack_int_t)realPart(workLength[0]);

    RWPRECONDITION(lwork >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    mhr('L', 'N', crows, ccols, B_.lowIndex(), B_.highIndex(), (TypeT*)H_.data(),
        hrows, (TypeT*)tau_.data(), C.data(), crows, work.data(), lwork, info);

    RWPOSTCONDITION(info == 0);
    RWPOSTCONDITION((rw_lapack_int_t)realPart(work[0]) <= lwork);
    return C;
}

template <class TypeT>
RWGenMat<TypeT>   RWHessenbergDecomp<TypeT>::QTX(const RWGenMat<TypeT>& X)   const
{
    RW_real<TypeT, norm_type> realPart;
    RWGenMat<TypeT> C = X.copy();
    rw_lapack_int_t info;
    char trans = 'C';

    // Query for optimal work array length
    rw_lapack_int_t lwork = -1;
    TypeT workLength[1];
    RW_ASSERT(C.rows() <= RW_LAPACK_INT_MAX);
    RW_ASSERT(C.cols() <= RW_LAPACK_INT_MAX);
    RW_ASSERT(H_.rows() <= RW_LAPACK_INT_MAX);
    rw_lapack_int_t crows = RW_STATIC_CAST(rw_lapack_int_t, C.rows());
    rw_lapack_int_t ccols = RW_STATIC_CAST(rw_lapack_int_t, C.cols());
    rw_lapack_int_t hrows = RW_STATIC_CAST(rw_lapack_int_t, H_.rows());
    mhr('L', trans, crows, ccols, B_.lowIndex(), B_.highIndex(), (TypeT*)H_.data(), hrows,
        (TypeT*)tau_.data(), C.data(), crows, workLength, lwork, info);

    // Allocate the work array and do the work.
    lwork = (rw_lapack_int_t)realPart(workLength[0]);

    RWPRECONDITION(lwork >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    mhr('L', trans, crows, ccols, B_.lowIndex(), B_.highIndex(), (TypeT*)H_.data(), hrows,
        (TypeT*)tau_.data(), C.data(), crows, work.data(), lwork, info);

    RWPOSTCONDITION(info == 0);
    RWPOSTCONDITION((rw_lapack_int_t)realPart(work[0]) <= lwork);
    return C;
}

#ifdef _WIN32
#  pragma warning(pop)
#endif
