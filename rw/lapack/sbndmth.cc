
/*
 * Math member functions for the matrix types
 *
 * $Id: //lapack/13/rw/lapack/sbndmth.cc#1 $
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

#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

template <class TypeT>
/*static*/ void verifyMatch(const RWSymBandMat<TypeT>& A, const RWSymBandMat<TypeT>& B)
{
    if (A.rows() != B.rows() || A.cols() != B.cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATSIZE, A.rows(), A.cols(), B.rows(), B.cols())));
    }
    if (A.bandwidth() != B.bandwidth() || A.upperBandwidth() != B.upperBandwidth())
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_BANDSIZE, A.bandwidth(), B.bandwidth(),
                                        A.upperBandwidth(), B.upperBandwidth())));
}

template <class TypeT>
bool RWSymBandMat<TypeT>::operator==(const RWSymBandMat<TypeT>& X)
{
    if (n != X.n || bandu != X.bandu) {
        return false;
    }
    unsigned hb = halfBandwidth();
    if (hb == 0) {
        return (vec == X.vec);    // A diagonal matrix
    }
    /*
     * This is a little tricky cause some of the data vector is unused.
     * First check the last part of the data vector, then check the bits
     * at the beginning that are used.
     */
    unsigned lastStart = hb * hb;
    unsigned lastLen = (n - hb + 1) * (hb + 1) - 1;
    if (n >= hb)
        if (vec.slice(lastStart, lastLen) != X.vec.slice(lastStart, lastLen)) {
            return false;
        }
    unsigned el, len;
    for (el = hb, len = 1 ; el < vec.length() && len < hb ; el += hb, len++) {
        if (vec.slice(el, len) != X.vec.slice(el, len)) {
            return false;
        }
    }
    return true;
}

template <class TypeT>
RWSymBandMat<TypeT>& RWSymBandMat<TypeT>::operator+=(const RWSymBandMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec += m.vec;
    return *this;
}

template <class TypeT>
RWSymBandMat<TypeT>& RWSymBandMat<TypeT>::operator-=(const RWSymBandMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec -= m.vec;
    return *this;
}

template <class TypeT>
RWSymBandMat<TypeT>& RWSymBandMat<TypeT>::operator*=(const RWSymBandMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec *= m.vec;
    return *this;
}

template <class TypeT>
RWSymBandMat<TypeT>& RWSymBandMat<TypeT>::operator*=(TypeT x)
{
    vec *= x;
    return *this;
}

template <class TypeT>
RWSymBandMat<TypeT>& RWSymBandMat<TypeT>::operator/=(const RWSymBandMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec /= m.vec;
    return *this;
}

template <class TypeT>
RWSymBandMat<TypeT>& RWSymBandMat<TypeT>::operator/=(TypeT x)
{
    vec /= x;
    return *this;
}



template <class TypeT>
RWSymBandMat<TypeT> operator-(const RWSymBandMat<TypeT>& A)
{
    RWMathVec<TypeT> temp(-(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWSymBandMat<TypeT> operator+(const RWSymBandMat<TypeT>& A)
{
    return A;
}

template <class TypeT>
RWSymBandMat<TypeT> operator*(const RWSymBandMat<TypeT>& A, const RWSymBandMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec()*B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWSymBandMat<TypeT> operator/(const RWSymBandMat<TypeT>& A, const RWSymBandMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() / B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWSymBandMat<TypeT> operator+(const RWSymBandMat<TypeT>& A, const RWSymBandMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() + B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWSymBandMat<TypeT> operator-(const RWSymBandMat<TypeT>& A, const RWSymBandMat<TypeT>& B)
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
RWSymBandMat<TypeT> operator*(TypeT x, const RWSymBandMat<TypeT>& A)
{
    return A * x;
}
#endif

template <class TypeT>
RWSymBandMat<TypeT> operator/(const RWSymBandMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec() / x);
    return sameShapeMatrix(temp, A);
}



/*
template <class TypeT>
RWSymBandMat<double> abs(const RWSymBandMat<TypeT>& A) {
    RWMathVec<double> temp(abs(A.dataVec()));
    return sameShapeMatrix(temp,A);
}
//
RWSymBandMat<double> abs(const RWSymBandMat<DComplex>& M);
RWSymBandMat<double> abs(const RWSymBandMat<double>& M);
RWSymBandMat<float> abs(const RWSymBandMat<float>& M);
RWSymBandMat<int> abs(const RWSymBandMat<int>& M);
RWSymBandMat<SChar> abs(const RWSymBandMat<SChar>& M);
*/
//******these were added by Omon:
/*
RWSymBandMat<DComplex> conj(const RWSymBandMat<DComplex>& A) {
    RWMathVec<DComplex> temp(conj(A.dataVec()));
    return sameShapeMatrix(temp,A);
}

RWSymBandMat<double> real(const RWSymBandMat<DComplex>& A) {
    RWMathVec<double> temp(real(A.dataVec()));
    return sameShapeRealMatrix(temp,A);
}

RWSymBandMat<double> imag(const RWSymBandMat<DComplex>& A) {
    RWMathVec<double> temp(imag(A.dataVec()));
    return sameShapeRealMatrix(temp,A);
}

RWSymBandMat<double> norm(const RWSymBandMat<DComplex>& A) {
    RWMathVec<double> temp(norm(A.dataVec()));
    return sameShapeRealMatrix(temp,A);
}

RWSymBandMat<double> arg(const RWSymBandMat<DComplex>& A) {
    RWMathVec<double> temp(arg(A.dataVec()));
    return sameShapeRealMatrix(temp,A);
}

// ******/

template <class TypeT>
RWSymBandMat<TypeT> transpose(const RWSymBandMat<TypeT>& A)
{
    return A;
}

template <class TypeT>
RWMathVec<TypeT> product(const RWSymBandMat<TypeT>& A, const RWMathVec<TypeT>& x)
{
    if (A.cols() != x.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATVECPROD, A.className, A.rows(), A.cols(), x.length())));
    }
    RWMathVec<TypeT> y(A.rows(), rwUninitialized);
    y = (TypeT)(0);
    int l = A.lowerBandwidth(); // Getting these as ints avoids
    int u = A.upperBandwidth(); // conversion problems
    for (int i = (-l); i <= u; i++) {
        int iabs = (i >= 0) ? i : (-i);
        RWMathVec<TypeT> yslice = y.slice((i < 0) ? iabs : 0, x.length() - iabs);
        RWMathVec<TypeT> xslice = x.slice((i > 0) ? iabs : 0, x.length() - iabs);
        /* The * in the next line is element by element multiplication */
        yslice += A.diagonal(i) * xslice;
    }
    return y;
}

template <class TypeT>
RWMathVec<TypeT> product(const RWMathVec<TypeT>& x, const RWSymBandMat<TypeT>& A)
{
    if (A.rows() != x.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECMATPROD, x.length(), A.className, A.rows(), A.cols())));
    }
    return product(A, x);
}



/*
 * Because of the templatized Math.h++ classes, automatic conversions
 * are no longer automatic.  Here we provide the product functions
 * to be backward compatible.
 */
template <class TypeT>
RWGenMat<TypeT> product(const RWSymBandMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return product(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> product(const RWGenMat<TypeT>& A, const RWSymBandMat<TypeT>& B)
{
    return product(A, RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> product(const RWSymBandMat<TypeT>& A, const RWSymBandMat<TypeT>& B)
{
    return product(RWGenMat<TypeT>(A), RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWSymBandMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return transposeProduct(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWGenMat<TypeT>& A, const RWSymBandMat<TypeT>& B)
{
    return transposeProduct(A, RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWSymBandMat<TypeT>& A, const RWSymBandMat<TypeT>& B)
{
    return transposeProduct(RWGenMat<TypeT>(A), RWGenMat<TypeT>(B));
}

// these were added by Omon :
/*
RWGenMat<DComplex> conjTransposeProduct(const RWSymBandMat<DComplex>& A, const RWGenMat<DComplex>& B)
{
   return conjTransposeProduct(RWGenMat<DComplex>(A),B);
}

RWGenMat<DComplex> conjTransposeProduct(const RWGenMat<DComplex>& A, const RWSymBandMat<DComplex>& B)
{
   return conjTransposeProduct(A,RWGenMat<DComplex>(B));
}

RWGenMat<DComplex> conjTransposeProduct(const RWSymBandMat<DComplex>& A, const RWSymBandMat<DComplex>& B)
{
   return conjTransposeProduct(RWGenMat<DComplex>(A),RWGenMat<DComplex>(B));
}
// */

#ifdef _WIN32
#  pragma warning(pop)
#endif
