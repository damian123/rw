
/*
 * Math member functions for the matrix types
 *
 *
 * $Id: //lapack/13/rw/lapack/hbndmth.cc#1 $
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
/*static*/ void verifyMatch(const RWHermBandMat<TypeT>& A, const RWHermBandMat<TypeT>& B)
{
    if (A.rows() != B.rows() || A.cols() != B.cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATSIZE, A.rows(), A.cols(), B.rows(), B.cols())));
    }
    if (A.bandwidth() != B.bandwidth() || A.upperBandwidth() != B.upperBandwidth())
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_BANDSIZE, A.bandwidth(), B.bandwidth(),
                                        A.upperBandwidth(), B.upperBandwidth())));
}

template <class TypeT>
bool RWHermBandMat<TypeT>::operator==(const RWHermBandMat<TypeT>& X)
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
    int lastStart = hb * hb;
    size_t lastLen = (n - hb + 1) * (hb + 1) - 1;
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
RWHermBandMat<TypeT>& RWHermBandMat<TypeT>::operator+=(const RWHermBandMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec += m.vec;
    return *this;
}

template <class TypeT>
RWHermBandMat<TypeT>& RWHermBandMat<TypeT>::operator-=(const RWHermBandMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec -= m.vec;
    return *this;
}

template <class TypeT>
RWHermBandMat<TypeT>& RWHermBandMat<TypeT>::operator*=(const RWHermBandMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec *= m.vec;
    return *this;
}

template <class TypeT>
RWHermBandMat<TypeT>& RWHermBandMat<TypeT>::operator*=(TypeT x)
{
    vec *= x;
    return *this;
}

template <class TypeT>
RWHermBandMat<TypeT>& RWHermBandMat<TypeT>::operator/=(const RWHermBandMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec /= m.vec;
    return *this;
}

template <class TypeT>
RWHermBandMat<TypeT>& RWHermBandMat<TypeT>::operator/=(TypeT x)
{
    vec /= x;
    return *this;
}




template <class TypeT>
RWHermBandMat<TypeT> operator-(const RWHermBandMat<TypeT>& A)
{
    RWMathVec<TypeT> temp(-(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWHermBandMat<TypeT> operator+(const RWHermBandMat<TypeT>& A)
{
    return A;
}

template <class TypeT>
RWHermBandMat<TypeT> operator*(const RWHermBandMat<TypeT>& A, const RWHermBandMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec()*B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWHermBandMat<TypeT> operator/(const RWHermBandMat<TypeT>& A, const RWHermBandMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() / B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWHermBandMat<TypeT> operator+(const RWHermBandMat<TypeT>& A, const RWHermBandMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() + B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWHermBandMat<TypeT> operator-(const RWHermBandMat<TypeT>& A, const RWHermBandMat<TypeT>& B)
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
RWHermBandMat<TypeT> operator*(TypeT x, const RWHermBandMat<TypeT>& A)
{
    return A * x;
}
#endif

template <class TypeT>
RWHermBandMat<TypeT> operator/(const RWHermBandMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec() / x);
    return sameShapeMatrix(temp, A);
}





template <class TypeT>
RWSymBandMat<double> abs(const RWHermBandMat<TypeT>& A)
{
    RWMathVec<double> temp(abs(A.dataVec()));
    return RWSymBandMat<double>(temp, A.rows(), A.cols(), A.halfBandwidth());
}



/* Sorry, no imag, arg, functions for HermBandMat */

template <class TypeT>
RWHermBandMat<TypeT> conj(const RWHermBandMat<TypeT>& A)
{
    RWMathVec<TypeT> temp(conj(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWSymBandMat<double> real(const RWHermBandMat<TypeT>& A)
{
    RWMathVec<double> temp(real(A.dataVec()));
    return RWSymBandMat<double>(temp, A.rows(), A.cols(), A.halfBandwidth());
}

template <class TypeT>
RWSymBandMat<double> norm(const RWHermBandMat<TypeT>& A)
{
    RWMathVec<double> temp(norm(A.dataVec()));
    return RWSymBandMat<double>(temp, A.rows(), A.cols(), A.halfBandwidth());
}

template <class TypeT>
RWHermBandMat<TypeT> transpose(const RWHermBandMat<TypeT>& A)
{
    RWHermBandMat<TypeT> TM(conj(A));
    unsigned n = A.rows();
    unsigned m = A.halfBandwidth();
    RWMathVec<TypeT> diag = TM.dataVec().slice(m, n, m + 1);
    diag = conj(diag);
    return TM;
}

template <class TypeT>
RWMathVec<TypeT> product(const RWHermBandMat<TypeT>& A, const RWMathVec<TypeT>& x)
{
    if (A.cols() != x.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATVECPROD, A.className, A.rows(), A.cols(), x.length())));
    }
    RWMathVec<TypeT> y(A.rows(), rwUninitialized);
    unsigned n = A.rows();
    unsigned m = A.lowerBandwidth();
    y = A.dataVec().slice(m, n, m + 1);
    y *= x;
    for (unsigned i = 1; i <= m; i++) {
        RWMathVec<TypeT> diag = A.dataVec().slice(m + i * m, n - i, m + 1);
        RWMathVec<TypeT> yslice = y.slice(0, x.length() - i);
        RWMathVec<TypeT> xslice = x.slice(i, x.length() - i);
        yslice += diag * xslice;
        RWMathVec<TypeT> yslice2 = y.slice(i, x.length() - i);
        RWMathVec<TypeT> xslice2 = x.slice(0, x.length() - i);
        yslice2 += conj(diag) * xslice2;
    }
    return y;
}

template <class TypeT>
RWMathVec<TypeT> product(const RWMathVec<TypeT>& x, const RWHermBandMat<TypeT>& A)
{
    if (A.rows() != x.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECMATPROD, x.length(), A.className, A.rows(), A.cols())));
    }
    unsigned n = A.rows();
    unsigned m = A.lowerBandwidth();
    RWMathVec<TypeT> y(A.dataVec().slice(m, n, m + 1));
    y.deepenShallowCopy();
    y *= x;
    for (unsigned i = 1; i <= m; i++) {
        RWMathVec<TypeT> diag = A.dataVec().slice(m + i * m, n - i, m + 1);
        RWMathVec<TypeT> yslice = y.slice(0, x.length() - i);
        RWMathVec<TypeT> xslice = x.slice(i, x.length() - i);
        yslice += conj(diag) * xslice;
        RWMathVec<TypeT> yslice2 = y.slice(i, x.length() - i);
        RWMathVec<TypeT> xslice2 = x.slice(0, x.length() - i);
        yslice2 += diag * xslice2;
    }
    return y;
}



/*
 * Because of the templatized Math.h++ classes, automatic conversions
 * are no longer automatic.  Here we provide the product functions
 * to be backward compatible.
 */

template <class TypeT>
RWGenMat<TypeT> product(const RWHermBandMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return product(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> product(const RWGenMat<TypeT>& A, const RWHermBandMat<TypeT>& B)
{
    return product(A, RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> product(const RWHermBandMat<TypeT>& A, const RWHermBandMat<TypeT>& B)
{
    return product(RWGenMat<TypeT>(A), RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWHermBandMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return transposeProduct(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWGenMat<TypeT>& A, const RWHermBandMat<TypeT>& B)
{
    return transposeProduct(A, RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWHermBandMat<TypeT>& A, const RWHermBandMat<TypeT>& B)
{
    return transposeProduct(RWGenMat<TypeT>(A), RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> conjTransposeProduct(const RWHermBandMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return conjTransposeProduct(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> conjTransposeProduct(const RWGenMat<TypeT>& A, const RWHermBandMat<TypeT>& B)
{
    return conjTransposeProduct(A, RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> conjTransposeProduct(const RWHermBandMat<TypeT>& A, const RWHermBandMat<TypeT>& B)
{
    return conjTransposeProduct(RWGenMat<TypeT>(A), RWGenMat<TypeT>(B));
}
