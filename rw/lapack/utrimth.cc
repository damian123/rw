
/*
 * Math member functions for the matrix types
 *
 * $Id: //lapack/13/rw/lapack/utrimth.cc#1 $
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
#include <rw/lapack/ltrimat.h>

template <class TypeT>
/*static*/ void verifyMatch(const RWUpperTriMat<TypeT>& A, const RWUpperTriMat<TypeT>& B)
{
    if (A.rows() != B.rows() || A.cols() != B.cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATSIZE, A.rows(), A.cols(), B.rows(), B.cols())));
    }
}

template <class TypeT>
bool RWUpperTriMat<TypeT>::operator==(const RWUpperTriMat<TypeT>& X)
{
    if (n != X.n) {
        return false;
    }
    return vec == X.vec; // All elements in vec used in the matrix
}

template <class TypeT>
RWUpperTriMat<TypeT>& RWUpperTriMat<TypeT>::operator+=(const RWUpperTriMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec += m.vec;
    return *this;
}

template <class TypeT>
RWUpperTriMat<TypeT>& RWUpperTriMat<TypeT>::operator-=(const RWUpperTriMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec -= m.vec;
    return *this;
}

template <class TypeT>
RWUpperTriMat<TypeT>& RWUpperTriMat<TypeT>::operator*=(const RWUpperTriMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec *= m.vec;
    return *this;
}

template <class TypeT>
RWUpperTriMat<TypeT>& RWUpperTriMat<TypeT>::operator*=(TypeT x)
{
    vec *= x;
    return *this;
}

template <class TypeT>
RWUpperTriMat<TypeT>& RWUpperTriMat<TypeT>::operator/=(const RWUpperTriMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec /= m.vec;
    return *this;
}

template <class TypeT>
RWUpperTriMat<TypeT>& RWUpperTriMat<TypeT>::operator/=(TypeT x)
{
    vec /= x;
    return *this;
}



template <class TypeT>
RWUpperTriMat<TypeT> operator-(const RWUpperTriMat<TypeT>& A)
{
    RWMathVec<TypeT> temp(-(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWUpperTriMat<TypeT> operator+(const RWUpperTriMat<TypeT>& A)
{
    return A;
}

template <class TypeT>
RWUpperTriMat<TypeT> operator*(const RWUpperTriMat<TypeT>& A, const RWUpperTriMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec()*B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWUpperTriMat<TypeT> operator/(const RWUpperTriMat<TypeT>& A, const RWUpperTriMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() / B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWUpperTriMat<TypeT> operator+(const RWUpperTriMat<TypeT>& A, const RWUpperTriMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() + B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWUpperTriMat<TypeT> operator-(const RWUpperTriMat<TypeT>& A, const RWUpperTriMat<TypeT>& B)
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
RWUpperTriMat<TypeT> operator*(TypeT x, const RWUpperTriMat<TypeT>& A)
{
    return A * x;
}
#endif

template <class TypeT>
RWUpperTriMat<TypeT> operator/(const RWUpperTriMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec() / x);
    return sameShapeMatrix(temp, A);
}

/*
template <class TypeT>
RWUpperTriMat<double> abs(const RWUpperTriMat<TypeT>& A) {
    RWMathVec<double> temp(abs(A.dataVec()));
    return sameShapeMatrix(temp,A);
}
RWUpperTriMat<double> abs(const RWUpperTriMat<DComplex>& M);
RWUpperTriMat<double> abs(const RWUpperTriMat<double>& M);
RWUpperTriMat<float> abs(const RWUpperTriMat<float>& M);
RWUpperTriMat<int> abs(const RWUpperTriMat<int>& M);
RWUpperTriMat<SChar> abs(const RWUpperTriMat<SChar>& M);
*/

//******these were added by Omon:
/*
RWUpperTriMat<DComplex> conj(const RWUpperTriMat<DComplex>& A) {
    RWMathVec<DComplex> temp(conj(A.dataVec()));
    return sameShapeMatrix(temp,A);
}

RWUpperTriMat<double> real(const RWUpperTriMat<DComplex>& A) {
    RWMathVec<double> temp(real(A.dataVec()));
    return sameShapeRealMatrix(temp,A);
}

RWUpperTriMat<double> imag(const RWUpperTriMat<DComplex>& A) {
    RWMathVec<double> temp(imag(A.dataVec()));
    return sameShapeRealMatrix(temp,A);
}

RWUpperTriMat<double> norm(const RWUpperTriMat<DComplex>& A) {
    RWMathVec<double> temp(norm(A.dataVec()));
    return sameShapeRealMatrix(temp,A);
}

RWUpperTriMat<double> arg(const RWUpperTriMat<DComplex>& A) {
    RWMathVec<double> temp(arg(A.dataVec()));
    return sameShapeRealMatrix(temp,A);
}

// ******/


template <class TypeT>
RWLowerTriMat<TypeT> transpose(const RWUpperTriMat<TypeT>& A)
{
    return RWLowerTriMat<TypeT>(A.dataVec(), A.rows(), A.cols());
}

template <class TypeT>
RWMathVec<TypeT> product(const RWUpperTriMat<TypeT>& A, const RWMathVec<TypeT>& x)
{
    if (A.cols() != x.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATVECPROD, A.className, A.rows(), A.cols(), x.length())));
    }
    RWMathVec<TypeT> y(A.rows(), rwUninitialized);
    const RWMathVec<TypeT>& v = A.dataVec();
    int n = A.rows();
    for (int i = 0; i < n; i++) {
        TypeT* index = (TypeT*)v.data() + ((i + 1) * (i + 2) / 2 - 1);
        y(i) = (*index) * x(i);
        index += i + 1;
        for (int k = i + 1; k < n; index += (++k)) {
            y(i) += (*index) * x(k);
        }
    }
    return y;
}

template <class TypeT>
RWMathVec<TypeT> product(const RWMathVec<TypeT>& x, const RWUpperTriMat<TypeT>& A)
{
    if (A.rows() != x.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECMATPROD, x.length(), A.className, A.rows(), A.cols())));
    }
    return product(transpose(A), x);
}



/*
 * Because of the templatized Math.h++ classes, automatic conversions
 * are no longer automatic.  Here we provide the product functions
 * to be backward compatible.
 */
template <class TypeT>
RWGenMat<TypeT> product(const RWUpperTriMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return product(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> product(const RWGenMat<TypeT>& A, const RWUpperTriMat<TypeT>& B)
{
    return product(A, RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> product(const RWUpperTriMat<TypeT>& A, const RWUpperTriMat<TypeT>& B)
{
    return product(RWGenMat<TypeT>(A), RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWUpperTriMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return transposeProduct(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWGenMat<TypeT>& A, const RWUpperTriMat<TypeT>& B)
{
    return transposeProduct(A, RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWUpperTriMat<TypeT>& A, const RWUpperTriMat<TypeT>& B)
{
    return transposeProduct(RWGenMat<TypeT>(A), RWGenMat<TypeT>(B));
}

/*
RWGenMat<DComplex> conjTransposeProduct(const RWUpperTriMat<DComplex>& A, const RWGenMat<DComplex>& B)
{
   return conjTransposeProduct(RWGenMat<DComplex>(A),B);
}

RWGenMat<DComplex> conjTransposeProduct(const RWGenMat<DComplex>& A, const RWUpperTriMat<DComplex>& B)
{
   return conjTransposeProduct(A,RWGenMat<DComplex>(B));
}

RWGenMat<DComplex> conjTransposeProduct(const RWUpperTriMat<DComplex>& A, const RWUpperTriMat<DComplex>& B)
{
   return conjTransposeProduct(RWGenMat<DComplex>(A),RWGenMat<DComplex>(B));
}
*/
