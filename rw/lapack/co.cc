/*
 * Definitions for RWCODecomp
 *
 *
 * $Id: //lapack/13/rw/lapack/co.cc#1 $
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
#include <rw/lapkerr.h>
#include <rw/math/funcobj.h>
#include <rw/math/rwalloc.h>
#include <rw/rwerr.h>


template <class TypeT, class QRCalc>
RWCODecomp<TypeT, QRCalc>::RWCODecomp()
{
}

template <class TypeT, class QRCalc>
RWCODecomp<TypeT, QRCalc>::RWCODecomp(const RWCODecomp<TypeT, QRCalc>& x)
{
    *this = x;
}

template <class TypeT, class QRCalc>
RWCODecomp<TypeT, QRCalc>& RWCODecomp<TypeT, QRCalc>::operator=(const RWCODecomp<TypeT, QRCalc>& x)
{
    if (&x != this) {
        QRDecomp_ = x.QRDecomp_;
        Ztau_.reference(x.Ztau_);
        TZ_.reference(x.TZ_);
    }
    return *this;
}

template <class TypeT, class QRCalc>
void RWCODecomp<TypeT, QRCalc>::dofactor(double tol)
{
    RW_norm<TypeT, norm_type> normFuncObj;

    RW_LAPACK_INT_MAX_ASSERT(cols());
    rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, cols());
    int m;
    if (rows() < cols()) {
        RW_ASSERT(rows() <= RW_INT_MAX);
        m = RW_STATIC_CAST(int, rows());
    }
    else {
        RW_ASSERT(n <= RW_INT_MAX);
        m = RW_STATIC_CAST(int, n);
    }
    while (m > 0 && normFuncObj(QRDecomp_.QR_(m - 1, m - 1)) < tol) {
        m--;
    }
    TZ_.reference(QRDecomp_.QR_(RWSlice(0, m), RWAll));
    Ztau_.reshape(RW_STATIC_CAST(unsigned, m));
    rw_lapack_int_t info;
    tzrqf(m, n, TZ_.data(), TZ_.colStride(), Ztau_.data(), info);
    RWPOSTCONDITION(info == 0);
}


template <class TypeT, class QRCalc>
RWCODecomp<TypeT, QRCalc>::RWCODecomp(const RWGenMat<TypeT>& A, double tol)
    : QRDecomp_(A)
{
    dofactor(tol);
}

template <class TypeT, class QRCalc>
RWCODecomp<TypeT, QRCalc>::RWCODecomp(const RWQRDecomp<TypeT, QRCalc>& A, double tol)
    : QRDecomp_(A)
{
    QRDecomp_.QR_.deepenShallowCopy();
    dofactor(tol);
}

template <class TypeT, class QRCalc>
void RWCODecomp<TypeT, QRCalc>::factor(const RWGenMat<TypeT>& A, double tol)
{
    QRDecomp_.factor(A);
    dofactor(tol);
}

template <class TypeT, class QRCalc>
void RWCODecomp<TypeT, QRCalc>::factor(const RWQRDecomp<TypeT, QRCalc>& A, double tol)
{
    QRDecomp_ = A;
    QRDecomp_.QR_.deepenShallowCopy();
    dofactor(tol);
}

template <class TypeT, class QRCalc>
RWCODecomp<TypeT, QRCalc>::~RWCODecomp()
{
}

template <class TypeT, class QRCalc>
RWUpperTriMat<TypeT> RWCODecomp<TypeT, QRCalc>::T() const
{
    return toUpperTriMat(TZ_.operator()(RWAll, RWSlice(0, rank()))); // explicit op() to shut up sun 3.0.1 warning
}

// Generate Z as Z = I P1 P2 P3 ... Pn
//
template <class TypeT, class QRCalc>
RWGenMat<TypeT> RWCODecomp<TypeT, QRCalc>::Z() const
{
    RWGenMat<TypeT> Zmat(cols(), cols(), (TypeT)0); // initialize Z to identity matrix
    Zmat.diagonal() = (TypeT)1;
    if (cols() > rank()) {
        RW_LAPACK_INT_MAX_ASSERT(cols());
        rw_lapack_int_t m = RW_STATIC_CAST(rw_lapack_int_t, cols());
        rw_lapack_int_t n = m - rank() + 1;
        RWPRECONDITION(m >= 0);
        RWTWorkBuffer<TypeT> work(static_cast<size_t>(m));
        RWGenMat<TypeT> ZmatRight = Zmat(RWAll, RWToEnd(rank()));
        RWGenMat<TypeT> TZRight = TZ_.operator()(RWAll, RWToEnd(rank())); // explicit op() to shut up sun 3.0.1 warning
        TypeT* c2 = ZmatRight.data();
        rw_lapack_int_t ldc = Zmat.colStride();
        rw_lapack_int_t incv = TZRight.colStride();
        for (int i = rank(); i--;) {
            TypeT* v = TZRight.row(i).data();
            TypeT* c1 = Zmat.col(i).data();
            latzm('R', m, n, v, incv, Ztau_(i), c1, c2, ldc, work.data());
        }
    }
    return Zmat;
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWCODecomp<TypeT, QRCalc>::Tx(const RWMathVec<TypeT>& x) const
{
    if (x.length() != rank()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), rank())));
    }
    if (cols() > rank()) {
        RWUpperTriMat<TypeT> T = toUpperTriMat(TZ_.operator()(RWAll, RWSlice(0, rank()))); // explicit op() to shut up sun 3.0.1 warning
        return product(T, x);
    }
    else {      // T==R
        return QRDecomp_.Rx(x);
    }
}


template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWCODecomp<TypeT, QRCalc>::TTx(const RWMathVec<TypeT>& x) const
{
    if (x.length() != rank()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), rank())));
    }
    if (cols() > rank()) {
        RWUpperTriMat<TypeT> Tm = toUpperTriMat(TZ_.operator()(RWAll, RWSlice(0, rank()))); // explicit op() to shut up sun 3.0.1 warning
        return product(x, conj(Tm));
    }
    else {      // Tm==R
        return QRDecomp_.RTx(x);
    }
}



template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWCODecomp<TypeT, QRCalc>::Tinvx(const RWMathVec<TypeT>& x) const
{
    if (x.length() != rank()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), rank())));
    }
    RWMathVec<TypeT> y = x.copy();
    rw_lapack_int_t n = rank();
    rw_lapack_int_t lda = TZ_.colStride();
    rw_lapack_int_t one = 1;
    rw_lapack_int_t info = 0;
    trtrs('U', 'N', 'N', n, one, (TypeT*)TZ_.data(), lda, y.data(), n, info);
    RWPOSTCONDITION(info == 0);
    return y;
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWCODecomp<TypeT, QRCalc>::TTinvx(const RWMathVec<TypeT>& x) const
{
    if (x.length() != rank()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), rank())));
    }
    RWMathVec<TypeT> y = x.copy();
    rw_lapack_int_t n = rank();
    rw_lapack_int_t lda = TZ_.colStride();
    rw_lapack_int_t one = 1;
    rw_lapack_int_t info;
    trtrs('U', *lapk_param_traits<TypeT>::char_param, 'N', n, one, (TypeT*)TZ_.data(), lda, y.data(), n, info);

    RWPOSTCONDITION(info == 0);
    return y;
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWCODecomp<TypeT, QRCalc>::Zx(const RWMathVec<TypeT>& x) const
{
    if (x.length() != cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), cols())));
    }
    RWMathVec<TypeT> y = x.copy();
    if (cols() > rank()) {
        size_t temp = cols() - rank() + 1;
        RW_ASSERT(temp <= RW_LAPACK_INT_MAX);
        rw_lapack_int_t n = 1;
        rw_lapack_int_t m = RW_STATIC_CAST(rw_lapack_int_t, temp);
        TypeT work[1];
        RWGenMat<TypeT> TZRight = TZ_.operator()(RWAll, RWToEnd(rank())); // explicit op() to shut up sun 3.0.1 warning
        TypeT* c2 = y.data() + rank();
        RW_ASSERT(y.length() <= RW_LAPACK_INT_MAX);
        rw_lapack_int_t ldc = RW_STATIC_CAST(rw_lapack_int_t, y.length());
        rw_lapack_int_t incv = TZRight.colStride();
        for (unsigned i = 0; i < rank(); i++) {
            TypeT* c1 = &(y(i));
            TypeT* v = TZRight.row(i).data();
            latzm('L', m, n, v, incv, Ztau_(i), c1, c2, ldc, work);
        }
    }
    return y;
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWCODecomp<TypeT, QRCalc>::ZTx(const RWMathVec<TypeT>& x) const
{
    if (x.length() != cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), cols())));
    }
    // To compute Z'x, use relationship y' = x'Z
    RWMathVec<TypeT> y = conj(x);
    y.deepenShallowCopy();
    if (cols() > rank()) {
        rw_lapack_int_t m = 1;
        size_t temp = cols() - rank() + 1;
        RW_ASSERT(temp <= RW_LAPACK_INT_MAX);
        rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, temp);
        TypeT work[1];
        RWGenMat<TypeT> TZRight = TZ_.operator()(RWAll, RWToEnd(rank())); // explicit op() to shut up sun 3.0.1 warning
        TypeT* c2 = y.data() + rank();
        rw_lapack_int_t ldc = 1;
        rw_lapack_int_t incv = TZRight.colStride();
        for (int i = rank(); i--;) {
            TypeT* c1 = &(y(i));
            TypeT* v = TZRight.row(i).data();
            latzm('R', m, n, v, incv, Ztau_(i), c1, c2, ldc, work);
        }
    }
    return conj(y);
}

