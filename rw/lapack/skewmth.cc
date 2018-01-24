
/*
 * Math member functions for the matrix types
 *
 * $Id: //lapack/13/rw/lapack/skewmth.cc#1 $
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
#include <rw/lapack/symmat.h>

template <class TypeT>
/*static*/ void verifyMatch(const RWSkewMat<TypeT>& A, const RWSkewMat<TypeT>& B)
{
    if (A.rows() != B.rows() || A.cols() != B.cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATSIZE, A.rows(), A.cols(), B.rows(), B.cols())));
    }
}

template <class TypeT>
bool RWSkewMat<TypeT>::operator==(const RWSkewMat<TypeT>& X)
{
    if (n != X.n) {
        return false;
    }
    return vec == X.vec; // All elements in vec used in the matrix
}

template <class TypeT>
RWSkewMat<TypeT>& RWSkewMat<TypeT>::operator+=(const RWSkewMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec += m.vec;
    return *this;
}

template <class TypeT>
RWSkewMat<TypeT>& RWSkewMat<TypeT>::operator-=(const RWSkewMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec -= m.vec;
    return *this;
}

template <class TypeT>
RWSkewMat<TypeT>& RWSkewMat<TypeT>::operator*=(const RWSkewMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec *= m.vec;
    return *this;
}

template <class TypeT>
RWSkewMat<TypeT>& RWSkewMat<TypeT>::operator*=(TypeT x)
{
    vec *= x;
    return *this;
}

template <class TypeT>
RWSkewMat<TypeT>& RWSkewMat<TypeT>::operator/=(const RWSkewMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec /= m.vec;
    return *this;
}

template <class TypeT>
RWSkewMat<TypeT>& RWSkewMat<TypeT>::operator/=(TypeT x)
{
    vec /= x;
    return *this;
}




template <class TypeT>
RWSkewMat<TypeT> operator-(const RWSkewMat<TypeT>& A)
{
    RWMathVec<TypeT> temp(-(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWSkewMat<TypeT> operator+(const RWSkewMat<TypeT>& A)
{
    return A;
}


template <class TypeT>
RWSkewMat<TypeT> operator*(const RWSkewMat<TypeT>& A, const RWSkewMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec()*B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWSkewMat<TypeT> operator/(const RWSkewMat<TypeT>& A, const RWSkewMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() / B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWSkewMat<TypeT> operator+(const RWSkewMat<TypeT>& A, const RWSkewMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() + B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWSkewMat<TypeT> operator-(const RWSkewMat<TypeT>& A, const RWSkewMat<TypeT>& B)
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
RWSkewMat<TypeT> operator*(TypeT x, const RWSkewMat<TypeT>& A)
{
    return A * x;
}
#endif

template <class TypeT>
RWSkewMat<TypeT> operator/(const RWSkewMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec() / x);
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWSkewMat<TypeT> operator/(TypeT x, const RWSkewMat<TypeT>& A)
{
    RWMathVec<TypeT> temp(x / A.dataVec());
    return sameShapeMatrix(temp, A);
}

/*
template <class TypeT>
RWSymMat<double> abs(const RWSkewMat<TypeT>& A) {
    RWMathVec<TypeT> temp(abs(A.dataVec()));
    return RWSymMat<TypeT>(temp,A.rows(),A.cols());
}
RWSymMat<double> abs(const RWSkewMat<DComplex>& M);
RWSymMat<double> abs(const RWSkewMat<double>& M);
RWSymMat<float> abs(const RWSkewMat<float>& M);
RWSymMat<int> abs(const RWSkewMat<int>& M);
RWSymMat<SChar> abs(const RWSkewMat<SChar>& M);
*/

/* Sorry, no conj or arg function for SkewMat */
/*
RWSkewMat<double> real(const RWSkewMat<DComplex>& A) {
    RWMathVec<double> temp(real(A.dataVec()));
    return sameShapeRealMatrix(temp,A);
}

RWSkewMat<double> imag(const RWSkewMat<DComplex>& A) {
    RWMathVec<double> temp(imag(A.dataVec()));
    return sameShapeRealMatrix(temp,A);
}

RWSymMat<double> norm(const RWSkewMat<DComplex>& A) {
    RWMathVec<double> temp(norm(A.dataVec()));
    return RWSymMat<double>(temp,A.rows(),A.cols());
}
*/

template <class TypeT>
RWSkewMat<TypeT> transpose(const RWSkewMat<TypeT>& A)
{
    RWSkewMat<TypeT> Tm(-A);
    TypeT* index = Tm.data();     // Fix up the diagonal
    for (size_t k = 1; k <= Tm.rows(); index += (++k)) {
        (*index) *= -1;
    }
    return Tm;
}

template <class TypeT>
RWMathVec<TypeT> product(const RWSkewMat<TypeT>& A, const RWMathVec<TypeT>& x)
{
    if (A.cols() != x.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATVECPROD, A.className, A.rows(), A.cols(), x.length())));
    }
    RWMathVec<TypeT> y(A.rows(), rwUninitialized);
    const RWMathVec<TypeT>& v = A.dataVec();
    unsigned n = A.rows();
    y(0) = (TypeT)(0);
    for (unsigned i = 0; i < n; i++) {
        if (i > 0) {
            y(i) = -dot(v.slice(i * (i + 1) / 2, i, 1), x.slice(0, i, 1));
        }
        TypeT* index = (TypeT*)v.data() + ((i + 1) * (i + 2) / 2 - 1);
        for (unsigned k = i; k < n; index += (++k)) {
            y(i) += (*index) * x(k);
        }
    }
    return y;
}

template <class TypeT>
RWMathVec<TypeT> product(const RWMathVec<TypeT>& x, const RWSkewMat<TypeT>& A)
{
    if (A.rows() != x.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECMATPROD, x.length(), A.className, A.rows(), A.cols())));
    }
    RWMathVec<TypeT> y(A.cols(), (TypeT)(0));
    const RWMathVec<TypeT>& v = A.dataVec();
    unsigned n = A.rows();
    for (unsigned i = 0; i < n; i++) {
        y(i) = dot(v.slice(i * (i + 1) / 2, i + 1, 1), x.slice(0, i + 1, 1));
        TypeT* index = (TypeT*)v.data() + ((i + 2) * (i + 3) / 2 - 2);
        for (unsigned k = i + 1; k < n; index += (++k)) {
            y(i) -= (*index) * x(k);
        }
    }
    return y;
}



/*
 * Because of the templatized Math.h++ classes, automatic conversions
 * are no longer automatic.  Here we provide the product functions
 * to be backward compatible.
 */

template <class TypeT>
RWGenMat<TypeT> product(const RWSkewMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return product(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> product(const RWGenMat<TypeT>& A, const RWSkewMat<TypeT>& B)
{
    return product(A, RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> product(const RWSkewMat<TypeT>& A, const RWSkewMat<TypeT>& B)
{
    return product(RWGenMat<TypeT>(A), RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWSkewMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return transposeProduct(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWGenMat<TypeT>& A, const RWSkewMat<TypeT>& B)
{
    return transposeProduct(A, RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWSkewMat<TypeT>& A, const RWSkewMat<TypeT>& B)
{
    return transposeProduct(RWGenMat<TypeT>(A), RWGenMat<TypeT>(B));
}
