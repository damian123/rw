/*
 * Implementation of DComplexBandFact
 *
 *
 * $Id: //lapack/13/rw/lapack/bandfct.cc#1 $
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
#include <rw/rwerr.h>

#include <rw/lapkerr.h>
#include <rw/math/genmat.h>
#include <rw/lapack/lapkfunc.h>

template <class TypeT>
#if !defined(_MSC_VER)
const char* RWBandFact<TypeT>::className = template_name<TypeT>("RWBandFact");
#else
const char* RWBandFact<TypeT>::className = template_name<TypeT>("RWBandFact", (TypeT)0);
#endif

template <class TypeT>
RWBandFact<TypeT>::~RWBandFact()
{
}

template <class TypeT>
RWBandFact<TypeT>::RWBandFact() : pvts(0)
{
    info = -1;
}

template <class TypeT>
RWBandFact<TypeT>::RWBandFact(const RWBandFact<TypeT>& A)
    : factorization(A.factorization), pvts(0)
{
    info = A.info;
    Anorm = A.Anorm;
    unsigned n = A.rows();
    if (n > 0) {
        RWTWorkBuffer<rw_lapack_int_t> tmp(n);
        while (n--) {
            pvts[n] = A.pvts[n];
        }
        pvts.swap(tmp);
    }
}

template <class TypeT>
RWBandFact<TypeT>::RWBandFact(const RWBandMat<TypeT>& A, bool estimateCondition)
    : factorization(A.rows(), A.cols(), A.lowerBandwidth(), A.lowerBandwidth() + A.upperBandwidth()), pvts(0)
{
    factorization = A;
    dofactor(estimateCondition);
}

template <class TypeT>
void RWBandFact<TypeT>::factor(const RWBandMat<TypeT>& A, bool estimateCondition)
{
    RWBandMat<TypeT> F(A.rows(), A.cols(), A.lowerBandwidth(), A.lowerBandwidth() + A.upperBandwidth());
    F = A;
    factorization.reference(F);
    dofactor(estimateCondition);
}

template <class TypeT>
void RWBandFact<TypeT>::dofactor(bool estimateCondition)
{
    info = 0; // for size zero matrices, need to set this explicitly
    rw_lapack_int_t n = rows();

    // Error (sanity) checking
    RWPRECONDITION(factorization.dataVec().stride() == 1);

    RWTWorkBuffer<rw_lapack_int_t>(static_cast<size_t>(n)).swap(pvts);

    // If we care to ever find the condition number, we need to know
    // the 1-norm of the matrix A.  Compute it while the member data
    // factorization is still a copy of A, and not yet it's factorization.
    Anorm = -1;
    if (n > 0 && estimateCondition) {
        rw_lapack_int_t kl = factorization.lowerBandwidth();
        rw_lapack_int_t ku = factorization.upperBandwidth();
        rw_lapack_int_t b = factorization.bandwidth();
        //Anorm = zlangb('1',n,kl,ku,factorization.data(),b,0);
        Anorm = langb('1', n, kl, ku, factorization.data(), b, 0);
    }

    if (n > 0) {
        rw_lapack_int_t b = factorization.bandwidth();
        rw_lapack_int_t l = factorization.lowerBandwidth();
        rw_lapack_int_t u = factorization.upperBandwidth() - factorization.lowerBandwidth();
        //zgbtrf(n,n,l,u,factorization.data(),b,pvts,info);
        gbtrf(n, n, l, u, factorization.data(), b, pvts.data(), info);
        RWPOSTCONDITION(info >= 0);
    }
}

template <class TypeT>
bool RWBandFact<TypeT>::fail() const
{
    return (rows() > 0 && info != 0) ? true : false;
}


template <class TypeT>
bool RWBandFact<TypeT>::isSingular() const
{
    return (rows() > 0 && info != 0) ? true : false;
}

template <class TypeT>
typename rw_numeric_traits<TypeT>::norm_type RWBandFact<TypeT>::condition() const
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;

    typedef typename rw_linear_algebra_traits<TypeT>::work_type argt;

    rt Anormcopy = Anorm;      // Copy to stop warnings about ref parm
    rw_lapack_int_t localInfo;
    rw_lapack_int_t n = rows();
    rt rcond;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(4 * n)); // Some routines only require 2*n, but what the hell
    rw_lapack_int_t b = factorization.bandwidth();
    rw_lapack_int_t l = factorization.lowerBandwidth();
    rw_lapack_int_t u = factorization.upperBandwidth() - factorization.lowerBandwidth();
    RWTWorkBuffer<argt> work2(static_cast<size_t>(n));
    gbcon('1', n, l, u, (TypeT*)factorization.data(), b, const_cast<rw_lapack_int_t*>(pvts.data()), Anormcopy, rcond, work.data(), work2.data(), localInfo);
    RWPOSTCONDITION(localInfo == 0);
    return rcond;
}

template <class TypeT>
RWMathVec<TypeT> RWBandFact<TypeT>::solve(const RWMathVec<TypeT>& b) const
{
    RWGenMat<TypeT> B(b, b.length(), 1);
    RWGenMat<TypeT> X = solve(B);
    RWPOSTCONDITION(X.cols() == 1);
    return X.col(0);
}

template <class TypeT>
RWGenMat<TypeT> RWBandFact<TypeT>::solve(const RWGenMat<TypeT>& B) const
{
    RWGenMat<TypeT> X(B.copy());
    rw_lapack_int_t n = rows();
    if (B.rows() != static_cast<size_t>(n)) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, B.rows(), n)));
    }
    if (fail()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_CANTSOLVE)));
    }
    RW_ASSERT(X.cols() <= RW_LAPACK_INT_MAX);
    rw_lapack_int_t nrhs = RW_STATIC_CAST(rw_lapack_int_t, X.cols());
    rw_lapack_int_t localInfo;
    if (n > 0) {
        rw_lapack_int_t b = factorization.bandwidth();
        rw_lapack_int_t l = factorization.lowerBandwidth();
        rw_lapack_int_t u = factorization.upperBandwidth() - factorization.lowerBandwidth();
        //zgbtrs('N',n,l,u,nrhs,(DComplex*)factorization.data(),b,pvts,X.data(),n,localInfo);
        gbtrs('N', n, l, u, nrhs, (TypeT*)factorization.data(), b, const_cast<rw_lapack_int_t*>(pvts.data()), X.data(), n, localInfo);
        RWPOSTCONDITION(localInfo == 0);
    }
    return X;
}

template <class TypeT>
TypeT RWBandFact<TypeT>::determinant() const
{
    int numExchanges = 0;
    for (int i = rows(); i--;) {
        if (pvts[static_cast<size_t>(i)] != i + 1) {
            numExchanges++;
        }
    }
    TypeT detLU = prod(factorization.diagonal());
    return (numExchanges % 2) ? -detLU : detLU;
}


/*
 * Global functions which provide alternate ways to call member functions.
 * These used to be inline, but this requires that you include more header
 * files on some compilers, and that is just not worth it.
 */
template <class TypeT>
RWMathVec<TypeT> solve(const RWBandFact<TypeT>& A, const RWMathVec<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
RWGenMat<TypeT> solve(const RWBandFact<TypeT>& A, const RWGenMat<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
TypeT  determinant(const RWBandFact<TypeT>& A)
{
    return A.determinant();
}
