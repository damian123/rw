
/*
 * Math member functions for the matrix types
 *
 * $Id: //lapack/13/rw/lapack/trdgmth.cc#1 $
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

/* This include is to avoid nesting limits in the Glock precompiler: */
#include <rw/defs.h>
#include <rw/rwerr.h>
#include <rw/lapkerr.h>

template <class TypeT>
/*static*/ void verifyMatch(const RWTriDiagMat<TypeT>& A, const RWTriDiagMat<TypeT>& B)
{
    if (A.rows() != B.rows() || A.cols() != B.cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATSIZE, A.rows(), A.cols(), B.rows(), B.cols())));
    }
}

template <class TypeT>
bool RWTriDiagMat<TypeT>::operator==(const RWTriDiagMat<TypeT>& X)
{
    if (n != X.n) {
        return false;
    }
    // The following two statements cannot be combined into one
    // because of a cfront limitation:
    if (n < 1) {
        return true;
    }
    return vec.slice(1, 3 * n - 2) == X.vec.slice(1, 3 * n - 2);
}

template <class TypeT>
RWTriDiagMat<TypeT>& RWTriDiagMat<TypeT>::operator+=(const RWTriDiagMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec += m.vec;
    return *this;
}

template <class TypeT>
RWTriDiagMat<TypeT>& RWTriDiagMat<TypeT>::operator-=(const RWTriDiagMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec -= m.vec;
    return *this;
}

template <class TypeT>
RWTriDiagMat<TypeT>& RWTriDiagMat<TypeT>::operator*=(const RWTriDiagMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec *= m.vec;
    return *this;
}

template <class TypeT>
RWTriDiagMat<TypeT>& RWTriDiagMat<TypeT>::operator*=(TypeT x)
{
    vec *= x;
    return *this;
}

template <class TypeT>
RWTriDiagMat<TypeT>& RWTriDiagMat<TypeT>::operator/=(const RWTriDiagMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec /= m.vec;
    return *this;
}

template <class TypeT>
RWTriDiagMat<TypeT>& RWTriDiagMat<TypeT>::operator/=(TypeT x)
{
    vec /= x;
    return *this;
}



template <class TypeT>
RWTriDiagMat<TypeT> operator-(const RWTriDiagMat<TypeT>& A)
{
    RWMathVec<TypeT> temp(-(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWTriDiagMat<TypeT> operator+(const RWTriDiagMat<TypeT>& A)
{
    return A;
}

template <class TypeT>
RWTriDiagMat<TypeT> operator*(const RWTriDiagMat<TypeT>& A, const RWTriDiagMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec()*B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWTriDiagMat<TypeT> operator/(const RWTriDiagMat<TypeT>& A, const RWTriDiagMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() / B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWTriDiagMat<TypeT> operator+(const RWTriDiagMat<TypeT>& A, const RWTriDiagMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() + B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWTriDiagMat<TypeT> operator-(const RWTriDiagMat<TypeT>& A, const RWTriDiagMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() - B.dataVec());
    return sameShapeMatrix(temp, A);
}


/*
 * Out-of-line version for compilers which can't
 * handle inlined temporaries:
 */
#ifdef RW_NO_INLINED_TEMP_DESTRUCTORS
template <class TypeT>
RWTriDiagMat<TypeT> operator*(TypeT x, const RWTriDiagMat<TypeT>& A)
{
    return A * x;
}
#endif

template <class TypeT>
RWTriDiagMat<TypeT> operator/(const RWTriDiagMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec() / x);
    return sameShapeMatrix(temp, A);
}


template <class TypeT>
RWTriDiagMat<TypeT> transpose(const RWTriDiagMat<TypeT>& A)
{
    RWTriDiagMat<TypeT> TM(A.rows(), A.cols());
    TM.diagonal(-1) = A.diagonal(1);
    TM.diagonal(0) = A.diagonal(0);
    TM.diagonal(1) = A.diagonal(-1);
    return TM;
}

template <class TypeT>
RWMathVec<TypeT> product(const RWTriDiagMat<TypeT>& A, const RWMathVec<TypeT>& x)
{
    if (A.cols() != x.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATVECPROD, A.className, A.rows(), A.cols(), x.length())));
    }
    RWMathVec<TypeT> y(A.rows(), rwUninitialized);
    y = A.diagonal(0) * x;
#if defined(RW_AIX_BUG_WORKAROUND)
    RWMathVec<TypeT> temp1 = A.diagonal(-1) * x.slice(0, x.length() - 1);
    RWMathVec<TypeT> temp2 = A.diagonal(1) * x.slice(1, x.length() - 1);
    y.slice(1, x.length() - 1) += temp1;
    y.slice(0, x.length() - 1) += temp2;
#else
    y.slice(1, x.length() - 1) += A.diagonal(-1) * x.slice(0, x.length() - 1);
    y.slice(0, x.length() - 1) += A.diagonal(1) * x.slice(1, x.length() - 1);
#endif
    return y;
}

template <class TypeT>
RWMathVec<TypeT> product(const RWMathVec<TypeT>& x, const RWTriDiagMat<TypeT>& A)
{
    if (A.rows() != x.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECMATPROD, x.length(), A.className, A.rows(), A.cols())));
    }
    RWMathVec<TypeT> y(A.cols(), rwUninitialized);
    y = A.diagonal(0) * x;
#if defined(RW_AIX_BUG_WORKAROUND)
    RWMathVec<TypeT> temp1 = A.diagonal(1) * x.slice(0, x.length() - 1);
    RWMathVec<TypeT> temp2 = A.diagonal(-1) * x.slice(1, x.length() - 1);
    y.slice(1, x.length() - 1) += temp1;
    y.slice(0, x.length() - 1) += temp2;
#else
    y.slice(1, x.length() - 1) += A.diagonal(1) * x.slice(0, x.length() - 1);
    y.slice(0, x.length() - 1) += A.diagonal(-1) * x.slice(1, x.length() - 1);
#endif
    return y;
}



/*
 * Because of the templatized Math.h++ classes, automatic conversions
 * are no longer automatic.  Here we provide the product functions
 * to be backward compatible.
 */

template <class TypeT>
RWGenMat<TypeT> product(const RWTriDiagMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return product(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> product(const RWGenMat<TypeT>& A, const RWTriDiagMat<TypeT>& B)
{
    return product(A, RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> product(const RWTriDiagMat<TypeT>& A, const RWTriDiagMat<TypeT>& B)
{
    return product(RWGenMat<TypeT>(A), RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWTriDiagMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return transposeProduct(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWGenMat<TypeT>& A, const RWTriDiagMat<TypeT>& B)
{
    return transposeProduct(A, RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWTriDiagMat<TypeT>& A, const RWTriDiagMat<TypeT>& B)
{
    return transposeProduct(RWGenMat<TypeT>(A), RWGenMat<TypeT>(B));
}

/*
RWGenMat<DComplex> conjTransposeProduct(const RWTriDiagMat<DComplex>& A, const RWGenMat<DComplex>& B)
{
   return conjTransposeProduct(RWGenMat<DComplex>(A),B);
}

RWGenMat<DComplex> conjTransposeProduct(const RWGenMat<DComplex>& A, const RWTriDiagMat<DComplex>& B)
{
   return conjTransposeProduct(A,RWGenMat<DComplex>(B));
}

RWGenMat<DComplex> conjTransposeProduct(const RWTriDiagMat<DComplex>& A, const RWTriDiagMat<DComplex>& B)
{
   return conjTransposeProduct(RWGenMat<DComplex>(A),RWGenMat<DComplex>(B));
}
*/
