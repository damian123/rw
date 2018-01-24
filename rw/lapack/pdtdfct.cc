/*
 * Implementation of RWPDTriDiagFact
 *
 * $Id: //lapack/13/rw/lapack/pdtdfct.cc#1 $
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
#include <rw/lapkdefs.h>
#include <rw/lapkerr.h>
#include <rw/math/genmat.h>
#include <rw/math/rwalloc.h>
#include <rw/rwerr.h>

template <class TypeT>
#if !defined(_MSC_VER)
const char* RWPDTriDiagFact<TypeT>::className = template_name<TypeT>("RWPDTriDiagFact");
#else
const char* RWPDTriDiagFact<TypeT>::className = template_name<TypeT>("RWPDTriDiagFact", (TypeT)0);
#endif

template <class TypeT>
RWPDTriDiagFact<TypeT>::~RWPDTriDiagFact()
{
}

template <class TypeT>
RWPDTriDiagFact<TypeT>::RWPDTriDiagFact()
{
    info = -1;
}

template <class TypeT>
RWPDTriDiagFact<TypeT>::RWPDTriDiagFact(const RWPDTriDiagFact<TypeT>& A)
    : d(A.d), e(A.e)
{
    info = A.info;
    Anorm = A.Anorm;
}

template <class TypeT>
RWPDTriDiagFact<TypeT>::RWPDTriDiagFact(const RWTriDiagMat<TypeT>& A, bool estimateCondition)
// Initialize in the body so we can be careful of negative lengths
{
    RWPRECONDITION(A.diagonal(-1) == A.diagonal(1));
    d.reference(A.diagonal().copy());
    if (A.rows() > 1) {
        e.reference(A.diagonal(1).copy());
    }
    dofactor(estimateCondition);
}


// specialize for DComplex:
template <>
inline RWPDTriDiagFact<DComplex>::RWPDTriDiagFact(const RWTriDiagMat<DComplex>& A, bool estimateCondition)
// Initialize in the body so we can be careful of negative lengths
{
    RWPRECONDITION(A.diagonal(-1) == conj(A.diagonal(1)));
    d.reference(real(A.diagonal()).copy());
    if (A.rows() > 1) {
        e.reference(A.diagonal(1).copy());
    }
    dofactor(estimateCondition);
}
// end of specialization

template <class TypeT>
void RWPDTriDiagFact<TypeT>::factor(const RWTriDiagMat<TypeT>& A, bool estimateCondition)
{
    RWPRECONDITION(A.diagonal(-1) == A.diagonal(1));
    d.reference(A.diagonal().copy());
    if (A.rows() > 1) {
        e.reference(A.diagonal(1).copy());
    }
    else {
        e.reshape(0);
    }
    dofactor(estimateCondition);
}


// specialize for DComplex:
template <>
inline void RWPDTriDiagFact<DComplex>::factor(const RWTriDiagMat<DComplex>& A, bool estimateCondition)
{
    factorSpecialized(A, estimateCondition);
}


template <class TypeT>
void RWPDTriDiagFact<TypeT>::factorSpecialized(const RWTriDiagMat<DComplex>& A, bool estimateCondition)
{
    RWPRECONDITION(A.diagonal(-1) == conj(A.diagonal(1)));
    d.reference(real(A.diagonal()).copy());
    if (A.rows() > 1) {
        e.reference(A.diagonal(1).copy());
    }
    else {
        e.reshape(0);
    }
    info = 0; // for size zero matrices, need to set this explicitly
    rw_lapack_int_t n = rows();
    // If we care to ever find the condition number, we need to know
    // the 1-norm of the matrix A.  Compute it while the member data
    // factorization is still a copy of A, and not yet it's factorization.
    Anorm = -1;
    if (n > 0 && estimateCondition) {
        //Anorm = zlanht('1',n,d.data(),e.data());
        Anorm = lanht('1', n, d.data(), e.data());
    }

    if (n > 0) {
        //zpttrf(n,d.data(),e.data(),info);
        pttrf(n, d.data(), e.data(), info);
        RWPOSTCONDITION(info >= 0);
    }
}
//end of specialization

template <class TypeT>
void RWPDTriDiagFact<TypeT>::dofactor(bool estimateCondition)
{
    info = 0; // for size zero matrices, need to set this explicitly
    rw_lapack_int_t n = rows();

    // Error (sanity) checking


    // If we care to ever find the condition number, we need to know
    // the 1-norm of the matrix A.  Compute it while the member data
    // factorization is still a copy of A, and not yet it's factorization.
    Anorm = -1;
    if (n > 0 && estimateCondition) {
        Anorm = lanst('1', n, d.data(), e.data());
    }

    if (n > 0) {
        pttrf(n, d.data(), e.data(), info);
        RWPOSTCONDITION(info >= 0);
    }
}

template <class TypeT>
bool RWPDTriDiagFact<TypeT>::fail() const
{
    return (rows() > 0 && info != 0) ? true : false;
}

template <class TypeT>
bool RWPDTriDiagFact<TypeT>::isPD() const
{
    return (rows() > 0 && info != 0) ? false : true;
}

template <class TypeT>
bool RWPDTriDiagFact<TypeT>::isSingular() const
{
    return (rows() > 0 && info != 0) ? true : false;
}

template <class TypeT>
typename rw_numeric_traits<TypeT>::norm_type RWPDTriDiagFact<TypeT>::condition() const
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
    rt Anormcopy = Anorm;      // Copy to stop warnings about ref parm
    rw_lapack_int_t localInfo;
    rw_lapack_int_t n = rows();
    rt rcond;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<rt> work(static_cast<size_t>(4 * n)); // Some routines only require 2*n, but what the hell
    //double *work2 = new double [n];
    //zptcon(n,(double*)d.data(),(TypeT*)e.data(),Anormcopy,rcond,work2,localInfo);
    ptcon(n, (rt*)d.data(), (TypeT*)e.data(), Anormcopy, rcond, work.data(), localInfo);
    RWPOSTCONDITION(localInfo == 0);
    return rcond;
}

template <class TypeT>
RWMathVec<TypeT> RWPDTriDiagFact<TypeT>::solve(const RWMathVec<TypeT>& b) const
{
    RWGenMat<TypeT> B(b, b.length(), 1);
    RWGenMat<TypeT> X = solve(B);
    RWPOSTCONDITION(X.cols() == 1);
    return X.col(0);
}

template <class TypeT>
RWGenMat<TypeT> RWPDTriDiagFact<TypeT>::solve(const RWGenMat<TypeT>& B) const
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
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
        //zpttrs('U',n,nrhs,(double*)d.data(),(TypeT*)e.data(),X.data(),n,localInfo);
        pttrs('U', n, nrhs, (rt*)d.data(), (TypeT*)e.data(), X.data(), n, localInfo);
        RWPOSTCONDITION(localInfo == 0);
    }
    return X;
}

template <class TypeT>
TypeT RWPDTriDiagFact<TypeT>::determinant() const
{
    return prod(d);
}


/*
 * Global functions which provide alternate ways to call member functions.
 * These used to be inline, but this requires that you include more header
 * files on some compilers, and that is just not worth it.
 */
template <class TypeT>
RWMathVec<TypeT> solve(const RWPDTriDiagFact<TypeT>& A, const RWMathVec<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
RWGenMat<TypeT> solve(const RWPDTriDiagFact<TypeT>& A, const RWGenMat<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
TypeT           determinant(const RWPDTriDiagFact<TypeT>& A)
{
    return A.determinant();
}
