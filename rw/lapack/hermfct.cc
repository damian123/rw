/*
 * Implementation of RWHermFact
 *
 *
 * $Id: //lapack/13/rw/lapack/hermfct.cc#1 $
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
#include <rw/math/genmat.h>

#if defined(RW_GCC_VERSION) && (RW_GCC_VERSION >= 40300 && RW_GCC_VERSION < 40900)
// gcc 4.3 and 4.4 are not able to use pragmas to silence integer
// overflow warnings use this helper function instead.
template<class T>
__attribute__((noinline)) T rw_silence_gcc_strict_overflow(T i)
{
    return i;
}
// gcc 4.3 and 4.4 generate warnings regarding overflow/underflow of integer.
// warning: assuming signed overflow does not occur when assuming that (X - c) <= X is always true.
// The code in this file is safe and we can safely silence this warning.
#define RW_GCC_INT_OVERFLOW(i) rw_silence_gcc_strict_overflow(i)
#else
#define RW_GCC_INT_OVERFLOW(i) i
#endif

template <class TypeT>
#if !defined(_MSC_VER)
const char* RWHermFact<TypeT>::className = template_name<TypeT>("RWHermFact");
#else
const char* RWHermFact<TypeT>::className = template_name<TypeT>("RWHermFact", (TypeT)0);
#endif

template <class TypeT>
RWHermFact<TypeT>::~RWHermFact()
{
}

template <class TypeT>
RWHermFact<TypeT>::RWHermFact() : pvts(0)
{
    info = -1;
}

template <class TypeT>
RWHermFact<TypeT>::RWHermFact(const RWHermFact<TypeT>& A)
    : factorization(A.factorization), pvts(0)
{
    info = A.info;
    Anorm = A.Anorm;
    size_t n = A.rows();
    if (n > 0) {
        RWTWorkBuffer<rw_lapack_int_t> tmp(n);
        while (n--) {
            pvts[n] = A.pvts[n];
        }
        pvts.swap(tmp);
    }
}

template <class TypeT>
RWHermFact<TypeT>::RWHermFact(const RWHermMat<TypeT>& A, bool estimateCondition)
// Use reference semantics, followed by deepenShallowCopy(), to work
// around a BCC V3.0 bug
    : factorization(A), pvts(0)
{
    factorization.deepenShallowCopy();
    dofactor(estimateCondition);
}

template <class TypeT>
void RWHermFact<TypeT>::factor(const RWHermMat<TypeT>& A, bool estimateCondition)
{
    RWHermMat<TypeT> Acopy = A.copy();
    factorization.reference(Acopy);
    dofactor(estimateCondition);
}

template <class TypeT>
void RWHermFact<TypeT>::dofactor(bool estimateCondition)
{
    info = 0; // for size zero matrices, need to set this explicitly
    rw_lapack_int_t n = rows();

    // Error (sanity) checking
    RWPRECONDITION(factorization.dataVec().stride() == 1);
    for (int r = static_cast<int>(n); r--;) {
        if (RW_MATH_SL_STD_GLOBAL(imag)(factorization.val(r, r)) != 0.0) {
            RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTHERM)));
        }
    }

    RWTWorkBuffer<rw_lapack_int_t>(static_cast<size_t>(n)).swap(pvts);

    // If we care to ever find the condition number, we need to know
    // the 1-norm of the matrix A.  Compute it while the member data
    // factorization is still a copy of A, and not yet it's factorization.
    Anorm = -1;
    if (n > 0 && estimateCondition) {
        RWTWorkBuffer<double> work(static_cast<size_t>(n));
        Anorm = lanhp('1', 'U', n, factorization.data(), work.data());
    }

    if (n > 0) {
        hptrf('U', n, factorization.data(), pvts.data(), info);
        RWPOSTCONDITION(info >= 0);
    }
}

template <class TypeT>
bool RWHermFact<TypeT>::fail() const
{
    return (rows() > 0 && info != 0) ? true : false;
}


template <class TypeT>
bool RWHermFact<TypeT>::isSingular() const
{
    return (rows() > 0 && info != 0) ? true : false;
}

template <class TypeT>
double RWHermFact<TypeT>::condition() const
{
    double Anormcopy = Anorm;      // Copy to stop warnings about ref parm
    rw_lapack_int_t localInfo;
    rw_lapack_int_t n = rows();
    double rcond;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(4 * n)); // Some routines only require 2*n, but what the hell
    hpcon('U', n, (TypeT*)factorization.data(), const_cast<rw_lapack_int_t*>(pvts.data()), Anormcopy, rcond, work.data(), localInfo);
    RWPOSTCONDITION(localInfo == 0);
    return rcond;
}

template <class TypeT>
RWMathVec<TypeT> RWHermFact<TypeT>::solve(const RWMathVec<TypeT>& b) const
{
    RWGenMat<TypeT> B(b, b.length(), 1);
    RWGenMat<TypeT> X = solve(B);
    RWPOSTCONDITION(X.cols() == 1);
    return X.col(0);
}

template <class TypeT>
RWGenMat<TypeT> RWHermFact<TypeT>::solve(const RWGenMat<TypeT>& B) const
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
        hptrs('U', n, nrhs, (TypeT*)factorization.data(), const_cast<rw_lapack_int_t*>(pvts.data()), X.data(), n, localInfo);
        RWPOSTCONDITION(localInfo == 0);
    }
    return X;
}

template <class TypeT>
TypeT RWHermFact<TypeT>::determinant() const
{
    // see linpack manual pg 5.16 for description of algorithm
    TypeT det = 1.0;
    RW_ASSERT(factorization.rows() <= RW_INT_MAX);
    for (int i = RW_STATIC_CAST(int, factorization.rows()); i--;) {
        RWPRECONDITION(pvts[static_cast<size_t>(i)] != 0);
        if (pvts[static_cast<size_t>(i)] > 0) {
            det *= factorization.val(i, i); // one by one block
        }
        else {                            // two by two block
            RWPRECONDITION(i > 0 && pvts[static_cast<size_t>(i - 1)] == pvts[static_cast<size_t>(i)]);
            TypeT a = factorization.val(i, i); // .val stops sun warning
            int x = RW_GCC_INT_OVERFLOW(i - 1);
            TypeT b = factorization.val(x, i);
            TypeT c = factorization.val(x, x);
            TypeT blockdet = a * c - TypeT(norm(b), 0); // see linpack manual for a better way that avoids overflow and (as much) roundoff
            det *= blockdet;
            i--;
        }
    }
    return det;
}

template <class TypeT>
RWHermMat<TypeT> RWHermFact<TypeT>::inverse() const
{
    if (rows() == 0) {
        return factorization;
    }
    if (fail()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_CANTSOLVE)));
    }
    RWHermMat<TypeT> soln(factorization);
    soln.deepenShallowCopy();
    rw_lapack_int_t n = rows();
    rw_lapack_int_t localInfo;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(n));
    hptri('U', n, soln.data(), const_cast<rw_lapack_int_t*>(pvts.data()), work.data(), localInfo);
    RWPOSTCONDITION(localInfo == 0);
    return soln;
}


/*
 * Global functions which provide alternate ways to call member functions.
 * These used to be inline, but this requires that you include more header
 * files on some compilers, and that is just not worth it.
 */
template <class TypeT>
RWMathVec<TypeT> solve(const RWHermFact<TypeT>& A, const RWMathVec<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
RWGenMat<TypeT> solve(const RWHermFact<TypeT>& A, const RWGenMat<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
TypeT  determinant(const RWHermFact<TypeT>& A)
{
    return A.determinant();
}

template <class TypeT>
RWHermMat<TypeT> inverse(const RWHermFact<TypeT>& A)
{
    return A.inverse();
}

template <class TypeT>
double condition(const RWHermFact<TypeT>& A)
{
    return A.condition();
}
