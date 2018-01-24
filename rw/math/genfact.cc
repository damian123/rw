#ifndef __GENFACT_CC__
#define __GENFACT_CC__

/***************************************************************************
 *
 * genfact.cc
 *
 * $Id: //math/13/rw/math/genfact.cc#1 $
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
 **************************************************************************/

#include <rw/rwerr.h>
#include <rw/matherr.h>
#include <rw/math/numtrait.h>
#include <rw/math/ludecomp.h>
#include <rw/tools/pointer.h>


template <class T>
RWGenFact<T>::~RWGenFact() {}

template <class T>
RWGenFact<T>::RWGenFact()
{
    cond = -1;
    sing_eps = -1;
}

template <class T>
RWGenFact<T>::RWGenFact(const RWGenFact<T>& A)
    : factorization(A.factorization)
    , pivots(A.pivots)
{
    cond = A.cond;
    sing_eps = A.sing_eps;
}

template <class T>
RWGenFact<T>::RWGenFact(const RWGenMat<T>& A, bool estimateCondition)
// Use reference semantics, followed by deepenShallowCopy(), to work
// around a BCC V3.0 bug
    : factorization(A)
{
    factorization.deepenShallowCopy();
    dofactor(estimateCondition);
    sing_eps = -1;
}

template <class T>
void RWGenFact<T>::factor(const RWGenMat<T>& A, bool estimateCondition)
{
    RWGenMat<T> Acopy = A.copy();
    factorization.reference(Acopy);
    dofactor(estimateCondition);
}
template <class T>
void RWGenFact<T>::dofactor(bool estimateCondition)
{
    size_t n = factorization.rows();

    if (n != factorization.cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWMATH_NSQUARE, size_t(n), factorization.cols())));
    }
    if (factorization.rowStride() != 1 || (size_t)factorization.colStride() != factorization.rows()) {
        factorization.deepenShallowCopy();
    }

    RWPivotType info;    // For linpack
    RWTScopedPointer<T[]> work(0); // Set up a work array if linpack needs it
    if (n > 0 && estimateCondition) {
        work.reset(new T[n]);
    }

    pivots.resize(n);
    pivots.deepenShallowCopy();

    if (n > 0) {
        if (estimateCondition) {
            info = RWLUDecompMethod<T>::getConditionNumber((T*)factorization.data(),
                    static_cast<int>(n), static_cast<int>(n),
                    (RWPivotType*)pivots.data(), cond, work.get());
        }
        else {
            info = RWLUDecompMethod<T>::getFactorization((T*)factorization.data(),
                    static_cast<int>(n), static_cast<int>(n),
                    (RWPivotType*)pivots.data());
            cond = static_cast<norm_type>((info == 0) ? (-1) : 0);
        }
    }
}

template <class T>
bool RWGenFact<T>::fail() const
{
    if (factorization.rows() == 0) {
        return false;
    }
    if (sing_eps >= 0) {
        return (cond <= sing_eps) ? true : false;
    }
    if (norm_type(1) + cond == norm_type(1)) {
        return true;
    }
    return false;
}

template <class T>
bool RWGenFact<T>::isSingular() const
{
    if (sing_eps >= 0) {
        return (factorization.rows() > 0 && cond <= sing_eps) ? true : false;
    }

    return (factorization.rows() > 0 && norm_type(1) + cond == norm_type(1)) ? true : false;
}

template <class T>
typename RWGenFact<T>::norm_type RWGenFact<T>::condition() const
{
    if (cond < 0.0) {
        RWTHROW(RWInternalErr(RWMessage(RWMATH_NOCONDITION)));
    }
    return cond;
}

template <class T>
void RWGenFact<T>::setSingEps(typename RWGenFact<T>::norm_type eps)
{
    sing_eps = eps;
}

template <class T>
RWMathVec<T> RWGenFact<T>::solve(const RWMathVec<T>& b) const
{

    size_t n = factorization.rows();

    b.lengthCheck(n);
    if (fail()) {
        RWTHROW(RWInternalErr(RWMessage(RWMATH_CANTSOLVE)));
    }
    RWMathVec<T> x(b.copy());
    if (n > 0) {
        int job = 0;
        RWLUDecompMethod<T>::solveSystem((T*)factorization.data(),
                                         static_cast<int>(n), static_cast<int>(n),
                                         (RWPivotType*)pivots.data(), (T*)x.data(), job);
    }

    return x;
}

template <class T>
RWGenMat<T> RWGenFact<T>::solve(const RWGenMat<T>& B) const
{
    RWGenMat<T> X(B.rows(), B.cols(), rwUninitialized);
    for (unsigned i = static_cast<unsigned>(B.cols()); i--;) {
#if defined(RW_AIX_BUG_WORKAROUND)
        RWMathVec<T> temp = solve(B.col(i));
        X.col(i) = temp;
#else
        X.col(i) = solve(B.col(i));
#endif
    }
    return X;
}

template <class T>
void RWGenFact<T>::calldi(const RWGenMat<T>& A, int job, T* det) const
{
    int n = static_cast<int>(A.rows());

    RWTScopedPointer<T[]> work(new T[static_cast<size_t>(n)]);
    RWLUDecompMethod<T>::determinantInverse((T*)A.data(), n, n,
                                            (RWPivotType*)pivots.data(), *det, work.get(), job);
}

template <class T>
T RWGenFact<T>::determinant() const
{
    if (isSingular()) {
        return T(0);
    }
    if (fail()) {               // failure mode other than singularity
        RWTHROW(RWInternalErr(RWMessage(RWMATH_CANTSOLVE)));
    }
    size_t n = factorization.rows();
    if (n < 1) {
        return T(0);    // Explicit constructor necessary for Zortech's sake
    }
    T det;
    int job = 10;
    calldi(factorization, job, &det);

    return det;
}


template <class T>
RWGenMat<T> RWGenFact<T>::inverse() const
{
    if (factorization.rows() == 0) {
        return factorization;
    }
    if (fail()) {
        RWTHROW(RWInternalErr(RWMessage(RWMATH_CANTSOLVE)));
    }
    T det;
    int job = 1;
    RWGenMat<T> soln(factorization);
    soln.deepenShallowCopy();
    calldi(soln, job, &det);
    return soln;
}


template <class T>
T determinant(const RWGenMat<T>& A, typename RWGenFact<T>::norm_type sing_eps)
{
    RWGenFact<T> gf(A);
    gf.setSingEps(sing_eps);
    return gf.determinant();
}

#endif // __GENFACT_CC__
