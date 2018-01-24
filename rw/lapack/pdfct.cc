/*
 * Implementation of RWPDFact
 *
 * $Id: //lapack/13/rw/lapack/pdfct.cc#1 $
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
const char* RWPDFact<TypeT>::className = template_name<TypeT>("RWPDFact");
#else
const char* RWPDFact<TypeT>::className = template_name<TypeT>("RWPDFact", (TypeT)0);
#endif

template <class TypeT>
RWPDFact<TypeT>::~RWPDFact()
{
}

template <class TypeT>
RWPDFact<TypeT>::RWPDFact()
{
    info = -1;
}

template <class TypeT>
RWPDFact<TypeT>::RWPDFact(const RWPDFact<TypeT>& A)
    : factorization(A.factorization)
{
    info = A.info;
    Anorm = A.Anorm;
}

template <class TypeT>
RWPDFact<TypeT>::RWPDFact(const typename RWPDFact<TypeT>::fact_matrix& A, bool estimateCondition)
// Use reference semantics, followed by deepenShallowCopy(), to work
// around a BCC V3.0 bug
    : factorization(A)
{
    factorization.deepenShallowCopy();
    dofactor(estimateCondition);
}

template <class TypeT>
void RWPDFact<TypeT>::factor(const typename RWPDFact<TypeT>::fact_matrix& A, bool estimateCondition)
{
    typename RWPDFact<TypeT>::fact_matrix Acopy = A.copy();
    factorization.reference(Acopy);
    dofactor(estimateCondition);
}

template <class TypeT>
void RWPDFact<TypeT>::dofactor(bool estimateCondition)
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;

    info = 0; // for size zero matrices, need to set this explicitly
    rw_lapack_int_t n = rows();

    // Error (sanity) checking
    RWPRECONDITION(factorization.dataVec().stride() == 1);

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
        //zpptrf('U',n,factorization.data(),info);
        pptrf('U', n, factorization.data(), info);
        RWPOSTCONDITION(info >= 0);
    }
}

//specialize the above method for DComplex:
template <>
inline void RWPDFact<DComplex>::dofactor(bool estimateCondition)
{
    dofactorSpecialized(estimateCondition);
}

template <class TypeT>
void RWPDFact<TypeT>::dofactorSpecialized(bool estimateCondition)
{
    typedef double rt;

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
        RWTWorkBuffer<rt> work(static_cast<size_t>(n));
        //Anorm = zlansp('1','U',n,factorization.data(),work);
        Anorm = lansp('1', 'U', n, factorization.data(), work.data());
    }

    if (n > 0) {
        //zpptrf('U',n,factorization.data(),info);
        pptrf('U', n, factorization.data(), info);
        RWPOSTCONDITION(info >= 0);
    }
}

template <class TypeT>
bool RWPDFact<TypeT>::fail() const
{
    return (rows() > 0 && info != 0) ? true : false;
}

template <class TypeT>
bool RWPDFact<TypeT>::isPD() const
{
    return (rows() > 0 && info != 0) ? false : true;
}

template <class TypeT>
bool RWPDFact<TypeT>::isSingular() const
{
    return (rows() > 0 && info != 0) ? true : false;
}

template <class TypeT>
typename rw_numeric_traits<TypeT>::norm_type RWPDFact<TypeT>::condition() const
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
    typedef typename rw_linear_algebra_traits<TypeT>::work_type argt;

    rt Anormcopy = Anorm;      // Copy to stop warnings about ref parm
    rw_lapack_int_t localInfo;
    rw_lapack_int_t n = rows();
    rt rcond;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(4 * n)); // Some routines only require 2*n, but what the hell
    RWTWorkBuffer<argt> work2(static_cast<size_t>(n));
    //zppcon('U',n,(TypeT*)factorization.data(),Anormcopy,rcond,work,work2,localInfo);
    ppcon('U', n, (TypeT*)factorization.data(), Anormcopy, rcond, work.data(), work2.data(), localInfo);
    RWPOSTCONDITION(localInfo == 0);
    return rcond;
}

template <class TypeT>
RWMathVec<TypeT> RWPDFact<TypeT>::solve(const RWMathVec<TypeT>& b) const
{
    RWGenMat<TypeT> B(b, b.length(), 1);
    RWGenMat<TypeT> X = solve(B);
    RWPOSTCONDITION(X.cols() == 1);
    return X.col(0);
}

template <class TypeT>
RWGenMat<TypeT> RWPDFact<TypeT>::solve(const RWGenMat<TypeT>& B) const
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
        //zpptrs('U',n,nrhs,(TypeT*)factorization.data(),X.data(),n,localInfo);
        pptrs('U', n, nrhs, (TypeT*)factorization.data(), X.data(), n, localInfo);
        RWPOSTCONDITION(localInfo == 0);
    }
    return X;

}

template <class TypeT>
TypeT RWPDFact<TypeT>::determinant() const
{
    TypeT rootdet = 1.0;
    for (int i = rows(); i--;) {
        rootdet *= factorization.val(i, i);    // using val() rather than just op()() shuts up a SUN warning
    }
    return rootdet * rootdet;
}

template <class TypeT>
typename rw_linear_algebra_traits<TypeT>::hermitian_type RWPDFact<TypeT>::inverse() const
{
    if (rows() == 0) {
        return factorization;
    }
    if (fail()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_CANTSOLVE)));
    }
    typename rw_linear_algebra_traits<TypeT>::hermitian_type soln(factorization);
    soln.deepenShallowCopy();
    rw_lapack_int_t n = rows();
    rw_lapack_int_t localInfo;
    //zpptri('U',n,soln.data(),localInfo);
    pptri('U', n, soln.data(), localInfo);
    RWPOSTCONDITION(localInfo == 0);
    return soln;
}


/*
 * Global functions which provide alternate ways to call member functions.
 * These used to be inline, but this requires that you include more header
 * files on some compilers, and that is just not worth it.
 */
template <class TypeT>
RWMathVec<TypeT> solve(const RWPDFact<TypeT>& A, const RWMathVec<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
RWGenMat<TypeT> solve(const RWPDFact<TypeT>& A, const RWGenMat<TypeT>& b)
{
    return A.solve(b);
}
template <class TypeT>
TypeT  determinant(const RWPDFact<TypeT>& A)
{
    return A.determinant();
}

template <class TypeT>
typename rw_linear_algebra_traits<TypeT>::hermitian_type inverse(const RWPDFact<TypeT>& A)
{
    return A.inverse();
}
