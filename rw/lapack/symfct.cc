/*
 * Implementation of RWSymFact
 *
 * $Id: //lapack/13/rw/lapack/symfct.cc#1 $
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
//#include <rw/lapack.h>
#include <rw/rwerr.h>

#include <rw/lapkerr.h>
#include <rw/math/genmat.h>
#include <rw/lapack/lapkfunc.h>

template <class TypeT>
#if !defined(_MSC_VER)
const char* RWSymFact<TypeT>::className = template_name<TypeT>("RWSymFact");
#else
const char* RWSymFact<TypeT>::className = template_name<TypeT>("RWSymFact", (TypeT)0);
#endif

template <class TypeT>
RWSymFact<TypeT>::~RWSymFact()
{
}

template <class TypeT>
RWSymFact<TypeT>::RWSymFact() : pvts(0)
{
    info = -1;
}

template <class TypeT>
RWSymFact<TypeT>::RWSymFact(const RWSymFact<TypeT>& A)
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
RWSymFact<TypeT>::RWSymFact(const RWSymMat<TypeT>& A, bool estimateCondition)
// Use reference semantics, followed by deepenShallowCopy(), to work
// around a BCC V3.0 bug
    : factorization(A), pvts(0)
{
    factorization.deepenShallowCopy();
    dofactor(estimateCondition);
}

template <class TypeT>
void RWSymFact<TypeT>::factor(const RWSymMat<TypeT>& A, bool estimateCondition)
{
    RWSymMat<TypeT> Acopy = A.copy();
    factorization.reference(Acopy);
    dofactor(estimateCondition);
}

template <class TypeT>
void RWSymFact<TypeT>::dofactor(bool estimateCondition)
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;

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
        RWTWorkBuffer<rt> work(static_cast<size_t>(n));
        //Anorm = zlansp('1','U',n,factorization.data(),work);
        Anorm = lansp('1', 'U', n, factorization.data(), work.data());
    }

    if (n > 0) {
        sptrf('U', n, factorization.data(), pvts.data(), info);
        RWPOSTCONDITION(info >= 0);
    }
}

template <class TypeT>
bool RWSymFact<TypeT>::fail() const
{
    return (rows() > 0 && info != 0) ? true : false;
}


template <class TypeT>
bool RWSymFact<TypeT>::isSingular() const
{
    return (rows() > 0 && info != 0) ? true : false;
}

template <class TypeT>
typename rw_numeric_traits<TypeT>::norm_type RWSymFact<TypeT>::condition() const
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;

    rt Anormcopy = Anorm;      // Copy to stop warnings about ref parm
    rw_lapack_int_t localInfo;
    rw_lapack_int_t n = rows();
    rt rcond;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(4 * n)); // Some routines only require 2*n, but what the hell
    RWTWorkBuffer<rw_lapack_int_t> iwork(static_cast<size_t>(n));
    //zspcon('U',n,(TypeT*)factorization.data(),pvts,Anormcopy,rcond,work,localInfo);
    spcon('U', n, (TypeT*)factorization.data(), const_cast<rw_lapack_int_t*>(pvts.data()), Anormcopy, rcond, work.data(), iwork.data(), localInfo);
    RWPOSTCONDITION(localInfo == 0);
    return rcond;
}
/*
template <class TypeT>
TypeT RWSymFact<TypeT>::condition() const
{

    TypeT Anormcopy = Anorm;      // Copy to stop warnings about ref parm
    rw_lapack_int_t localInfo;
    rw_lapack_int_t n = rows();
    TypeT rcond;
    RWTWorkBuffer<TypeT> work(4 * n); // Some routines only require 2*n, but what the hell
    RWTWorkBuffer<rw_lapack_int_t> iwork(n);
    //zspcon('U',n,(TypeT*)factorization.data(),pvts,Anormcopy,rcond,work,localInfo);
    spcon('U',n,(TypeT*)factorization.data(),pvts,Anormcopy,rcond,work.data(),iwork.data(),localInfo);
    RWPOSTCONDITION(localInfo==0);
    return rcond;
}
*/

template <class TypeT>
RWMathVec<TypeT> RWSymFact<TypeT>::solve(const RWMathVec<TypeT>& b) const
{
    RWGenMat<TypeT> B(b, b.length(), 1);
    RWGenMat<TypeT> X = solve(B);
    RWPOSTCONDITION(X.cols() == 1);
    return X.col(0);
}

template <class TypeT>
RWGenMat<TypeT> RWSymFact<TypeT>::solve(const RWGenMat<TypeT>& B) const
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
        //zsptrs('U',n,nrhs,(TypeT*)factorization.data(),pvts,X.data(),n,localInfo);
        sptrs('U', n, nrhs, (TypeT*)factorization.data(), const_cast<rw_lapack_int_t*>(pvts.data()), X.data(), n, localInfo);
        RWPOSTCONDITION(localInfo == 0);
    }
    return X;
}

template <class TypeT>
TypeT RWSymFact<TypeT>::determinant() const
{
    // see linpack manual pg 5.16 for description of algorithm
    TypeT det = 1.0;
    for (unsigned i = factorization.rows(); i--;) {
        RWPRECONDITION(pvts[i] != 0);
        if (pvts[i] > 0) {
            det *= factorization.val(i, i); // one by one block
        }
        else {                            // two by two block
            RWPRECONDITION(i > 0 && pvts[i - 1] == pvts[i]);
            TypeT a = factorization.val(i, i); // .val stops sun warning
            TypeT b = factorization.val(i - 1, i);
            TypeT c = factorization.val(i - 1, i - 1);
            TypeT blockdet = a * c - b * b; // see linpack manual for a better way that avoids overflow and (as much) roundoff
            det *= blockdet;
            i--;
        }
    }
    return det;
}

RW_SUPPRESS_MEMORY_LEAK_WARNING
template <class TypeT>
RWSymMat<TypeT> RWSymFact<TypeT>::inverse() const
{
    if (rows() == 0) {
        return factorization;
    }
    if (fail()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_CANTSOLVE)));
    }
    RWSymMat<TypeT> soln(factorization);
    soln.deepenShallowCopy();
    rw_lapack_int_t n = rows();
    rw_lapack_int_t localInfo;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(n));
    //zsptri('U',n,soln.data(),pvts,work,localInfo);
    sptri('U', n, soln.data(), const_cast<rw_lapack_int_t*>(pvts.data()), work.data(), localInfo);
    RWPOSTCONDITION(localInfo == 0);
    return soln;
}
RW_RESTORE_MEMORY_LEAK_WARNING

/*
 * Global functions which provide alternate ways to call member functions.
 * These used to be inline, but this requires that you include more header
 * files on some compilers, and that is just not worth it.
 */
template <class TypeT>
RWMathVec<TypeT> solve(const RWSymFact<TypeT>& A, const RWMathVec<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
RWGenMat<TypeT> solve(const RWSymFact<TypeT>& A, const RWGenMat<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
TypeT  determinant(const RWSymFact<TypeT>& A)
{
    return A.determinant();
}

template <class TypeT>
RWSymMat<TypeT> inverse(const RWSymFact<TypeT>& A)
{
    return A.inverse();
}

//template <class TypeT>
//typename rw_numeric_traits<TypeT>::norm_type condition(const RWSymFact<TypeT>& A) { return A.condition(); }