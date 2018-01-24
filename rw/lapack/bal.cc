
/*
 * defs for balancing transformation and decomposition classes
 *
 * $Id: //lapack/13/rw/lapack/bal.cc#1 $
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

#include <rw/lapkdefs.h>
#include <rw/lapack/lapkfunc.h>
#include <rw/rwerr.h>
#include <rw/lapkerr.h>

template <class TypeT>
RWBalanceTransform<TypeT>::RWBalanceTransform()
{
    job_ = 'N';
    ilo_ = ihi_ = 0;
}

template <class TypeT>
RWBalanceTransform<TypeT>&
RWBalanceTransform<TypeT>::operator=(const RWBalanceTransform<TypeT>& A)
{
    if (this != &A) {
        job_ = A.job_;
        ilo_ = A.ilo_;
        ihi_ = A.ihi_;
        scale_.reference(A.scale_);
    }
    return *this;
}

template <class TypeT>
void RWBalanceTransform<TypeT>::init(RWGenMat<TypeT>* A, bool permute, bool scale)
{
    if (A->rows() != A->cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, A->rows(), A->cols())));
    }
    RWPRECONDITION(A->rowStride() == 1);
    // Subtract one from the other and verify the result is 0.  Let the compiler
    // figure out how to reconcile the signed and unsigned types
    RWPRECONDITION((A->colStride() - A->rows()) == 0);
    scale_.reshape(A->rows());
    job_ = (permute ? (scale ? 'B' : 'P') : (scale ? 'S' : 'N'));
    rw_lapack_int_t info;
    RW_ASSERT(A->rows() <= RW_LAPACK_INT_MAX);
    rw_lapack_int_t arows = RW_STATIC_CAST(rw_lapack_int_t, A->rows());
    gebal(job_, arows, A->data(), arows, ilo_, ihi_, scale_.data(), info);
    RWPOSTCONDITION(info == 0);
}

template <class TypeT>
RWGenMat<TypeT> RWBalanceTransform<TypeT>::transform(const RWGenMat<TypeT>& X) const
{
    if (X.rows() != rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_BALANCE, X.rows(), rows())));
    }
    RWGenMat<TypeT> Y = X.copy();
    if (rows() > 0) {
        rw_lapack_int_t info;
        RW_LAPACK_INT_MAX_ASSERT(rows());
        RW_ASSERT(Y.cols() <= RW_LAPACK_INT_MAX);
        RW_ASSERT(Y.rows() <= RW_LAPACK_INT_MAX);
        rw_lapack_int_t r = RW_STATIC_CAST(rw_lapack_int_t, rows());
        rw_lapack_int_t ycols =  RW_STATIC_CAST(rw_lapack_int_t, Y.cols());
        rw_lapack_int_t yrows =  RW_STATIC_CAST(rw_lapack_int_t, Y.rows());
        gebak(job_, 'R', r, ilo_, ihi_, (double*)scale_.data(), ycols, Y.data(), yrows, info);
        RWPOSTCONDITION(info == 0);
    }
    return Y;
}

template <class TypeT>
RWGenMat<TypeT> RWBalanceTransform<TypeT>::invTransform(const RWGenMat<TypeT>& X) const
{
    if (X.rows() != rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_BALANCE, X.rows(), rows())));
    }
    RWGenMat<TypeT> Y = X.copy();
    if (rows() > 0) {
        rw_lapack_int_t info;
        RW_LAPACK_INT_MAX_ASSERT(rows());
        RW_ASSERT(Y.cols() <= RW_LAPACK_INT_MAX);
        RW_ASSERT(Y.rows() <= RW_LAPACK_INT_MAX);
        rw_lapack_int_t r = RW_STATIC_CAST(rw_lapack_int_t, rows());
        rw_lapack_int_t ycols =  RW_STATIC_CAST(rw_lapack_int_t, Y.cols());
        rw_lapack_int_t yrows =  RW_STATIC_CAST(rw_lapack_int_t, Y.rows());
        gebak(job_, 'L', r, ilo_, ihi_, (double*)scale_.data(), ycols, Y.data(), yrows, info);
        RWPOSTCONDITION(info == 0);
    }
    return Y;
}

template <class TypeT>
RWMathVec<TypeT> RWBalanceTransform<TypeT>::transform(const RWMathVec<TypeT>& x) const
{
    RWGenMat<TypeT> Y = transform(RWGenMat<TypeT>(x, x.length(), 1));
    RWPOSTCONDITION(Y.cols() == 1);
    return Y.col(0);
}

template <class TypeT>
RWMathVec<TypeT> RWBalanceTransform<TypeT>::invTransform(const RWMathVec<TypeT>& x) const
{
    RWGenMat<TypeT> Y = invTransform(RWGenMat<TypeT>(x, x.length(), 1));
    RWPOSTCONDITION(Y.cols() == 1);
    return Y.col(0);
}

template <class TypeT>
RWBalanceDecomp<TypeT>::RWBalanceDecomp()
{
}

template <class TypeT>
RWBalanceDecomp<TypeT>::RWBalanceDecomp(const RWGenMat<TypeT>& A, bool permute, bool scale)
{
    factor(A, permute, scale);
}

template <class TypeT>
void RWBalanceDecomp<TypeT>::factor(const RWGenMat<TypeT>& A, bool permute, bool scale)
{
    C_.reference(A.copy());
    B_.init(&C_, permute, scale);
    RWPOSTCONDITION(rows() == A.rows());
    RWPOSTCONDITION(B_.rows() == A.rows());
    RWPOSTCONDITION(C_.rows() == A.rows());
}
