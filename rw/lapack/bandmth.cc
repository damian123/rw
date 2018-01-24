
/*
 * Math member functions for the matrix types
 *
 *
 * $Id: //lapack/13/rw/lapack/bandmth.cc#1 $
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

#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

#include <rw/lapkerr.h>

template <class TypeT>
/*static*/ void verifyMatch(const RWBandMat<TypeT>& A, const RWBandMat<TypeT>& B)
{
    if (A.rows() != B.rows() || A.cols() != B.cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATSIZE, A.rows(), A.cols(), B.rows(), B.cols())));
    }
    if (A.bandwidth() != B.bandwidth() || A.upperBandwidth() != B.upperBandwidth())
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_BANDSIZE, A.bandwidth(), B.bandwidth(),
                                        A.upperBandwidth(), B.upperBandwidth())));
}

template <class TypeT>
bool RWBandMat<TypeT>::operator==(const RWBandMat<TypeT>& X)
{
    if (n != X.n || band != X.band || bandu != X.bandu) {
        return false;
    }
    unsigned b = bandwidth();
    unsigned lb = lowerBandwidth();
    unsigned ub = upperBandwidth();
    /*
     * This is a little tricky cause some of the data vector is unused.
     * First check the middle part of the vector.  Next check the bits
     * at the front that are part of the matrix, and finally check the
     * bits at the end
     */
    unsigned midstart = (ub == 0) ? 0 : (ub - 1) * b + 1;
    unsigned fixup = (lb == 0) ? ((ub == 0) ? 0U : 1U) : ((ub == 0) ? 1U : 2U);
    size_t midlen = size_t((n - ub - lb + fixup) * b - fixup);
    if (midlen > 0)
        if (vec.slice(midstart, midlen) != X.vec.slice(midstart, midlen)) {
            return false;
        }
    unsigned el, len;
    for (el = ub, len = b - ub ; el < n * b && len < (b - 1); el += b - 1, len++) {
        if (vec.slice(el, len) != X.vec.slice(el, len)) {
            return false;
        }
    }
    for (el = b * (n - lb + 1), len = b - 2 ; el < n * b; el += b, len--) {
        if (vec.slice(el, len) != X.vec.slice(el, len)) {
            return false;
        }
    }
    return true;
}

template <class TypeT>
RWBandMat<TypeT>& RWBandMat<TypeT>::operator+=(const RWBandMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec += m.vec;
    return *this;
}

template <class TypeT>
RWBandMat<TypeT>& RWBandMat<TypeT>::operator-=(const RWBandMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec -= m.vec;
    return *this;
}

template <class TypeT>
RWBandMat<TypeT>& RWBandMat<TypeT>::operator*=(const RWBandMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec *= m.vec;
    return *this;
}

template <class TypeT>
RWBandMat<TypeT>& RWBandMat<TypeT>::operator*=(TypeT x)
{
    vec *= x;
    return *this;
}

template <class TypeT>
RWBandMat<TypeT>& RWBandMat<TypeT>::operator/=(const RWBandMat<TypeT>& m)
{
    verifyMatch(*this, m);
    vec /= m.vec;
    return *this;
}

template <class TypeT>
RWBandMat<TypeT>& RWBandMat<TypeT>::operator/=(TypeT x)
{
    vec /= x;
    return *this;
}



template <class TypeT>
RWBandMat<TypeT> operator-(const RWBandMat<TypeT>& A)
{
    RWMathVec<TypeT> temp(-(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWBandMat<TypeT> operator+(const RWBandMat<TypeT>& A)
{
    return A;
}

template <class TypeT>
RWBandMat<TypeT> operator*(const RWBandMat<TypeT>& A, const RWBandMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec()*B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWBandMat<TypeT> operator/(const RWBandMat<TypeT>& A, const RWBandMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() / B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWBandMat<TypeT> operator+(const RWBandMat<TypeT>& A, const RWBandMat<TypeT>& B)
{
    verifyMatch(A, B);
    RWMathVec<TypeT> temp(A.dataVec() + B.dataVec());
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWBandMat<TypeT> operator-(const RWBandMat<TypeT>& A, const RWBandMat<TypeT>& B)
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
RWBandMat<TypeT> operator*(TypeT x, const RWBandMat<TypeT>& A)
{
    return A * x;
}
#endif


template <class TypeT>
RWBandMat<TypeT> operator/(const RWBandMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec() / x);
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
RWBandMat<TypeT> transpose(const RWBandMat<TypeT>&  A)
{
    RWBandMat<TypeT> TR(A.rows(), A.cols(), A.upperBandwidth(), A.lowerBandwidth());
    int l = A.lowerBandwidth();   // Getting these as ints avoids
    int u = A.upperBandwidth();   // conversion problems
    for (int i = (-l); i <= u; i++) {
        TR.diagonal(-i) = A.diagonal(i);
    }
    return TR;
}




template <class TypeT>
RWMathVec<TypeT> product(const RWBandMat<TypeT>& A, const RWMathVec<TypeT>& x)
{
    if (A.cols() != x.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATVECPROD, A.className, A.rows(), A.cols(), x.length())));
    }
    RWMathVec<TypeT> y(A.rows(), rwUninitialized);
    y = TypeT(0);
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
RWMathVec<TypeT> product(const RWMathVec<TypeT>& x, const RWBandMat<TypeT>& A)
{
    if (A.rows() != x.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECMATPROD, x.length(), A.className, A.rows(), A.cols())));
    }
    RWMathVec<TypeT> y(A.cols(), TypeT(0));
    int l = A.lowerBandwidth(); // Getting these as ints avoids
    int u = A.upperBandwidth(); // conversion problems
    for (int i = (-l); i <= u; i++) {
        int iabs = (i >= 0) ? i : (-i);
        RWMathVec<TypeT> yslice = y.slice((i > 0) ? iabs : 0, x.length() - iabs);
        RWMathVec<TypeT> xslice = x.slice((i < 0) ? iabs : 0, x.length() - iabs);
        /* The * in the next line is element by element multiplication */
        yslice += A.diagonal(i) * xslice;
    }
    return y;
}


/*
 * The product of two banded matrices is a banded matrix.  Here's the details:
 *
 * Say we have want C=AB, then Cij = sum z=0,...,n ( AizBzj )
 * Say A has lower bandwidth al and upper au, B's bandwidths are bl and bu
 * so that Aij=0 unless -al<=j-i<=au and similiar for B.
 *
 * Back to Cij = sum z=0,...,n AizBzj.  Define j=i+k, now we have
 *         Cij = sum z=0,...,n Aiz Bz,i+k
 *
 * Now replace z by i+w:
 *         Ci,i+k = sum w=-i,...,n-i Ai,i+w Bi+w,i+k
 *
 * Bi+w,i+k is zero unless -bl<=k-w<=bu ==> k-bu<=w<=k+bl
 * Ai,i+w is zero unless                     -al<=w<=au
 * so we can reduce the limits on the summation:
 *
 *    Ci,i+k = sum w=max(-i,k-bu,-al),...,min(n-i,k+bl,au) Ai,i+w Bi+w,i+k
 *
 * That's the formula we'll implement.  Now consider the bandwidth of C.
 * For i in the middle of the matrix (-i is small and n-i is big) then
 * the sums are reduced to one term in two cases:
 *   k-bu=au  ==>  k=au+bu
 *   k+bl=-al ==>  k=-al-bl
 * so the lower bandwidth of C is al+bl and the upper au+bu.
 *
 * Rather than code this up with slices and dot() functions, I simply
 * wrote the loops directly.  Since (hopefully) the bandwidth is
 * generally not that large, avoiding the overhead of function calls
 * may be worthwhile.  (plus it is easier this way)
 */
template <class TypeT>
RWBandMat<TypeT> product(const RWBandMat<TypeT>& A, const RWBandMat<TypeT>& B)
{
    unsigned n = A.cols();
    if (B.rows() != n) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATMATPROD, A.className, n, n, B.rows(), B.cols())));
    }
    int au = A.upperBandwidth();
    int al = A.lowerBandwidth();
    int bu = B.upperBandwidth();
    int bl = B.lowerBandwidth();
    RWBandMat<TypeT> AB((unsigned)n, (unsigned)n, unsigned(al + bl), unsigned(au + bu));
    for (int i = 0; i < static_cast<int>(n); i++) {
        int mink = (i > al + bl) ? (-al - bl) : (-i);
        int maxk = (i < static_cast<int>(n) - au - bu) ? (au + bu) : (n - i - 1);
        for (int k = mink; k <= maxk; k++) {
            int minw = (-i >= k - bu) ? (-i) : (k - bu);
            if (-al > minw) {
                minw = -al;
            }
            int maxw = (au <= k + bl) ? au : (k + bl);
            if (static_cast<int>(n) - i - 1 < maxw) {
                maxw = n - i - 1;
            }
            TypeT entry = (TypeT)(0);
            for (int w = minw; w <= maxw; w++) {
                entry += A.val(i, i + w) * B.val(i + w, i + k);
            }
            AB.set(i, i + k, entry);
        }
    }
    return AB;
}

template <class TypeT>
RWBandMat<TypeT> transposeProduct(const RWBandMat<TypeT>& A, const RWBandMat<TypeT>& B)
{
    return product(transpose(A), B);
}

// added by Omon - 06/15
/*
RWBandMat<DComplex> conjTransposeProduct(const RWBandMat<DComplex>& A, const RWBandMat<DComplex>& B)
{
    return product(conj(transpose(A)),B);
}
*/
//

/*
 * Because of the templatized Math.h++ classes, automatic conversions
 * are no longer automatic.  Here we provide the product functions
 * to be backward compatible.
 */

template <class TypeT>
RWGenMat<TypeT> product(const RWBandMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return product(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> product(const RWGenMat<TypeT>& A, const RWBandMat<TypeT>& B)
{
    return product(A, RWGenMat<TypeT>(B));
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWBandMat<TypeT>& A, const RWGenMat<TypeT>& B)
{
    return transposeProduct(RWGenMat<TypeT>(A), B);
}

template <class TypeT>
RWGenMat<TypeT> transposeProduct(const RWGenMat<TypeT>& A, const RWBandMat<TypeT>& B)
{
    return transposeProduct(A, RWGenMat<TypeT>(B));
}

//added by Omon - 06/15
/*
RWGenMat<DComplex> conjTransposeProduct(const RWBandMat<DComplex>& A, const RWGenMat<DComplex>& B)
{
   return conjTransposeProduct(RWGenMat<DComplex>(A),B);
}

RWGenMat<DComplex> conjTransposeProduct(const RWGenMat<DComplex>& A, const RWBandMat<DComplex>& B)
{
   return conjTransposeProduct(A,RWGenMat<DComplex>(B));
}
*/
//
#ifdef _WIN32
#  pragma warning(pop)
#endif
