#ifndef __RWTRDGFCT_CC__
#define __RWTRDGFCT_CC__

/*
 * Implementation of RWTriDiagFact
 *
 * $Id: //lapack/13/rw/lapack/trdgfct.cc#1 $
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

#include <rw/lapkdefs.h>
#include <rw/lapack/trdgfct.h>
//#include <rw/lapack.h>
#include <rw/rwerr.h>

#include <rw/lapkerr.h>
#include <rw/math/genmat.h>
#include <rw/lapack/lapkfunc.h>

template <class TypeT>
#if !defined(_MSC_VER)
const char* RWTriDiagFact<TypeT>::className = template_name<TypeT>("RWTriDiagFact");
#else
const char* RWTriDiagFact<TypeT>::className = template_name<TypeT>("RWTriDiagFact", (TypeT)0);
#endif

template <class TypeT>
RWTriDiagFact<TypeT>::~RWTriDiagFact()
{
}

template <class TypeT>
RWTriDiagFact<TypeT>::RWTriDiagFact() : pvts(0)
{
    info = -1;
}

template <class TypeT>
RWTriDiagFact<TypeT>::RWTriDiagFact(const RWTriDiagFact<TypeT>& A)
    : dl(A.dl), d(A.d), du(A.du), d2(A.d2), pvts(0)
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
RWTriDiagFact<TypeT>::RWTriDiagFact(const RWTriDiagMat<TypeT>& A, bool estimateCondition) : pvts(0)
    // Initialize in the body so we can be careful of negative lengths
{
    d.reference(A.diagonal().copy());
    if (A.rows() > 1) {
        dl.reference(A.diagonal(-1).copy());
        du.reference(A.diagonal(1).copy());
        d2.reshape(A.rows() - 1);  // Only use n-2 elements, but this simplifies the code by eliminating if statements
    }
    dofactor(estimateCondition);
}

template <class TypeT>
void RWTriDiagFact<TypeT>::factor(const RWTriDiagMat<TypeT>& A, bool estimateCondition)
{
    d.reference(A.diagonal().copy());
    if (A.rows() > 1) {
        dl.reference(A.diagonal(-1).copy());
        du.reference(A.diagonal(1).copy());
        d2.reshape(A.rows() - 1);  // Only use n-2 elements, but this simplifies the code by eliminating if statements
    }
    else {
        dl.reshape(0);
        du.reshape(0);
        d2.reshape(0);
    }
    d2.reshape(A.rows() - 2);
    dofactor(estimateCondition);
}

template <class TypeT>
void RWTriDiagFact<TypeT>::dofactor(bool estimateCondition)
{
    info = 0; // for size zero matrices, need to set this explicitly
    rw_lapack_int_t n = rows();

    // Error (sanity) checking

    RWTWorkBuffer<rw_lapack_int_t>(static_cast<size_t>(n)).swap(pvts);

    // If we care to ever find the condition number, we need to know
    // the 1-norm of the matrix A.  Compute it while the member data
    // factorization is still a copy of A, and not yet it's factorization.
    Anorm = -1;
    if (n > 0 && estimateCondition) {
        //Anorm = zlangt('1',n,dl.data(),d.data(),du.data());
        Anorm = langt('1', n, dl.data(), d.data(), du.data());
    }

    if (n > 0) {
        //zgttrf(n,dl.data(),d.data(),du.data(),d2.data(),pvts,info);
        gttrf(n, dl.data(), d.data(), du.data(), d2.data(), pvts.data(), info);
        RWPOSTCONDITION(info >= 0);
    }
}

template <class TypeT>
bool RWTriDiagFact<TypeT>::fail() const
{
    return (rows() > 0 && info != 0) ? true : false;
}


template <class TypeT>
bool RWTriDiagFact<TypeT>::isSingular() const
{
    return (rows() > 0 && info != 0) ? true : false;
}

template <class TypeT>
typename rw_numeric_traits<TypeT>::norm_type RWTriDiagFact<TypeT>::condition() const
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;

    rt Anormcopy = static_cast<rt>(Anorm);      // Copy to stop warnings about ref parm
    rw_lapack_int_t localInfo;
    rw_lapack_int_t n = rows();
    rt rcond;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(4 * n)); // Some routines only require 2*n, but what the hell
    RWTWorkBuffer<rw_lapack_int_t> iwork(static_cast<size_t>(n));
    //zgtcon('1',n,(TypeT*)dl.data(),(TypeT*)d.data(),(TypeT*)du.data(),(TypeT*)d2.data(),pvts,Anormcopy,rcond,work,localInfo);
    gtcon('1', n, (TypeT*)dl.data(), (TypeT*)d.data(), (TypeT*)du.data(), (TypeT*)d2.data(), const_cast<rw_lapack_int_t*>(pvts.data()), Anormcopy, rcond, work.data(), iwork.data(), localInfo);
    RWPOSTCONDITION(localInfo == 0);
    return rcond;
}



template <class TypeT>
RWMathVec<TypeT> RWTriDiagFact<TypeT>::solve(const RWMathVec<TypeT>& b) const
{
    RWGenMat<TypeT> B(b, b.length(), 1);
    RWGenMat<TypeT> X = solve(B);
    RWPOSTCONDITION(X.cols() == 1);
    return X.col(0);
}

template <class TypeT>
RWGenMat<TypeT> RWTriDiagFact<TypeT>::solve(const RWGenMat<TypeT>& B) const
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
        //zgttrs('N',n,nrhs,(TypeT*)dl.data(),(TypeT*)d.data(),(TypeT*)du.data(),(TypeT*)d2.data(),pvts,X.data(),n,localInfo);
        gttrs('N', n, nrhs, (TypeT*)dl.data(), (TypeT*)d.data(), (TypeT*)du.data(), (TypeT*)d2.data(), const_cast<rw_lapack_int_t*>(pvts.data()), X.data(), n, localInfo);
        RWPOSTCONDITION(localInfo == 0);
    }
    return X;
}

template <class TypeT>
TypeT RWTriDiagFact<TypeT>::determinant() const
{
    int numExchanges = 0;
    for (int i = rows(); i--;) {
        if (pvts[static_cast<size_t>(i)] != i + 1) {
            numExchanges++;
        }
    }
    TypeT detLU = prod(d);
    return (numExchanges % 2) ? -detLU : detLU;
}


/*
 * Global functions which provide alternate ways to call member functions.
 * These used to be inline, but this requires that you include more header
 * files on some compilers, and that is just not worth it.
 */
template <class TypeT>
RWMathVec<TypeT> solve(const RWTriDiagFact<TypeT>& A, const RWMathVec<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
RWGenMat<TypeT> solve(const RWTriDiagFact<TypeT>& A, const RWGenMat<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
TypeT  determinant(const RWTriDiagFact<TypeT>& A)
{
    return A.determinant();
}

#endif // __RWTRDGFCT_CC__
