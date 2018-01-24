/*
 * Implementation of RWPDBandFact
 *
 * $Id: //lapack/13/rw/lapack/pdbdfct.cc#1 $
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
#include <rw/lapack/latraits.h>
#include <rw/lapack/pdbdfct.h>
#include <rw/lapkdefs.h>
#include <rw/lapkerr.h>
#include <rw/math/genmat.h>
#include <rw/math/rwalloc.h>
#include <rw/rwerr.h>

template <class TypeT>
#if !defined(_MSC_VER)
const char* RWPDBandFact<TypeT>::className = template_name<TypeT>("RWPDBandFact");
#else
const char* RWPDBandFact<TypeT>::className = template_name<TypeT>("RWPDBandFact", (TypeT)0);
#endif

template <class TypeT>
RWPDBandFact<TypeT>::~RWPDBandFact()
{
}

template <class TypeT>
RWPDBandFact<TypeT>::RWPDBandFact()
{
    info = -1;
}

template <class TypeT>
RWPDBandFact<TypeT>::RWPDBandFact(const RWPDBandFact<TypeT>& A)
    : factorization(A.factorization)
{
    info = A.info;
    Anorm = A.Anorm;
}

template <class TypeT>
RWPDBandFact<TypeT>::RWPDBandFact(const typename rw_linear_algebra_traits<TypeT>::banded_hermitian_type& A, bool estimateCondition)
// Use reference semantics, followed by deepenShallowCopy(), to work
// around a BCC V3.0 bug
    : factorization(A)
{
    factorization.deepenShallowCopy();
    dofactor(estimateCondition);
}

template <class TypeT>
void RWPDBandFact<TypeT>::factor(const typename rw_linear_algebra_traits<TypeT>::banded_hermitian_type& A, bool estimateCondition)
{
    typename rw_linear_algebra_traits<TypeT>::banded_hermitian_type Acopy = A.copy();
    factorization.reference(Acopy);
    dofactor(estimateCondition);
}

template <class TypeT>
void RWPDBandFact<TypeT>::dofactor(bool estimateCondition)
{
    typedef typename rw_numeric_traits<TypeT>::norm_type ret_type;
    info = 0; // for size zero matrices, need to set this explicitly
    rw_lapack_int_t n = rows();

    // Error (sanity) checking
    RWPRECONDITION(factorization.dataVec().stride() == 1);


    // If we care to ever find the condition number, we need to know
    // the 1-norm of the matrix A.  Compute it while the member data
    // factorization is still a copy of A, and not yet it's factorization.
    Anorm = -1;
    if (n > 0 && estimateCondition) {
        rw_lapack_int_t kd = factorization.halfBandwidth();
        rw_lapack_int_t ldab = kd + 1;
        RWTWorkBuffer<ret_type> work(static_cast<size_t>(n));
        Anorm = lansb('1', 'U', n, kd, factorization.data(), ldab, work.data());
    }

    if (n > 0) {
        rw_lapack_int_t kd = factorization.halfBandwidth();
        rw_lapack_int_t ldab = kd + 1;
        pbtrf('U', n, kd, factorization.data(), ldab, info);
        RWPOSTCONDITION(info >= 0);
    }
}

//specialize for DComplex:
template <>
inline void RWPDBandFact<DComplex>::dofactor(bool estimateCondition)
{
    dofactorSpecialized(estimateCondition);
}

template <class TypeT>
void RWPDBandFact<TypeT>::dofactorSpecialized(bool estimateCondition)
{
    info = 0; // for size zero matrices, need to set this explicitly
    rw_lapack_int_t n = rows();

    // Error (sanity) checking
    RWPRECONDITION(factorization.dataVec().stride() == 1);
    for (int r = RW_STATIC_CAST(int, n); r--;) {
        if (RW_MATH_SL_STD_GLOBAL(imag)(factorization.val(r, r)) != 0.0) {
            RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTHERM)));
        }
    }


    // If we care to ever find the condition number, we need to know
    // the 1-norm of the matrix A.  Compute it while the member data
    // factorization is still a copy of A, and not yet it's factorization.
    Anorm = -1;
    if (n > 0 && estimateCondition) {
        rw_lapack_int_t kd = factorization.halfBandwidth();
        rw_lapack_int_t ldab = kd + 1;
        RWPRECONDITION(n >= 0);
        RWTWorkBuffer<double> work(static_cast<size_t>(n));
        Anorm = lansb('1', 'U', n, kd, factorization.data(), ldab, work.data());
    }

    if (n > 0) {
        rw_lapack_int_t kd = factorization.halfBandwidth();
        rw_lapack_int_t ldab = kd + 1;
        pbtrf('U', n, kd, factorization.data(), ldab, info);
        RWPOSTCONDITION(info >= 0);
    }
}
// end of specialization

template <class TypeT>
bool RWPDBandFact<TypeT>::fail() const
{
    return (rows() > 0 && info != 0) ? true : false;
}

template <class TypeT>
bool RWPDBandFact<TypeT>::isPD() const
{
    return (rows() > 0 && info != 0) ? false : true;
}

template <class TypeT>
bool RWPDBandFact<TypeT>::isSingular() const
{
    return (rows() > 0 && info != 0) ? true : false;
}

template <class TypeT>
typename rw_numeric_traits<TypeT>::norm_type RWPDBandFact<TypeT>::condition() const
{
    typedef typename rw_numeric_traits<TypeT>::norm_type ret_type;

    ret_type Anormcopy = Anorm;      // Copy to stop warnings about ref parm
    rw_lapack_int_t localInfo;
    rw_lapack_int_t n = rows();
    ret_type rcond;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(4 * n)); // Some routines only require 2*n, but what the hell
    rw_lapack_int_t kd = factorization.halfBandwidth();
    rw_lapack_int_t ldab = kd + 1;
    RWTWorkBuffer<rw_lapack_int_t> work2(static_cast<size_t>(n));
    pbcon('U', n, kd, (TypeT*)factorization.data(), ldab, Anormcopy, rcond, work.data(), work2.data(), localInfo);
    RWPOSTCONDITION(localInfo == 0);
    return rcond;
}

// specialize for DComplex:
template <>
inline rw_numeric_traits<DComplex>::norm_type RWPDBandFact<DComplex>::condition() const
{
    return conditionSpecialized();
}

template <class TypeT>
double RWPDBandFact<TypeT>::conditionSpecialized() const
{
    typedef rw_numeric_traits<DComplex>::norm_type ret_type;

    ret_type Anormcopy = Anorm;      // Copy to stop warnings about ref parm
    rw_lapack_int_t localInfo;
    rw_lapack_int_t n = rows();
    ret_type rcond;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<DComplex> work(static_cast<size_t>(4 * n)); // Some routines only require 2*n, but what the hell
    rw_lapack_int_t kd = factorization.halfBandwidth();
    rw_lapack_int_t ldab = kd + 1;
    RWTWorkBuffer<double> work2(static_cast<size_t>(n)); // <-- specializing because of this difference
    //zpbcon('U',n,kd,(TypeT*)factorization.data(),ldab,Anormcopy,rcond,work,work2,localInfo);
    pbcon('U', n, kd, (DComplex*)factorization.data(), ldab, Anormcopy, rcond, work.data(), work2.data(), localInfo);
    RWPOSTCONDITION(localInfo == 0);
    return rcond;
}
// end of specialization

template <class TypeT>
RWMathVec<TypeT> RWPDBandFact<TypeT>::solve(const RWMathVec<TypeT>& b) const
{
    RWGenMat<TypeT> B(b, b.length(), 1);
    RWGenMat<TypeT> X = solve(B);
    RWPOSTCONDITION(X.cols() == 1);
    return X.col(0);
}

template <class TypeT>
RWGenMat<TypeT> RWPDBandFact<TypeT>::solve(const RWGenMat<TypeT>& B) const
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
        rw_lapack_int_t kd = factorization.halfBandwidth();
        rw_lapack_int_t ldab = kd + 1;
        //zpbtrs('U',n,kd,nrhs,(TypeT*)factorization.data(),ldab,X.data(),n,localInfo);
        pbtrs('U', n, kd, nrhs, (TypeT*)factorization.data(), ldab, X.data(), n, localInfo);
        RWPOSTCONDITION(localInfo == 0);
    }
    return X;
}

template <class TypeT>
TypeT RWPDBandFact<TypeT>::determinant() const
{
    TypeT rootdet = 1.0;
    for (int i = rows(); i--;) {
        rootdet *= factorization.val(i, i);    // using val() rather than just op()() shuts up a SUN warning
    }
    return rootdet * rootdet;
}




/*
 * Global functions which provide alternate ways to call member functions.
 * These used to be inline, but this requires that you include more header
 * files on some compilers, and that is just not worth it.
 */
template <class TypeT>
RWMathVec<TypeT> solve(const RWPDBandFact<TypeT>& A, const RWMathVec<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
RWGenMat<TypeT> solve(const RWPDBandFact<TypeT>& A, const RWGenMat<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
TypeT  determinant(const RWPDBandFact<TypeT>& A)
{
    return A.determinant();
}
