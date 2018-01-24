/*
 * Definitions for RWQRDecomp and RWQRDecompServer
 *
 * $Id: //lapack/13/rw/lapack/qr.cc#1 $
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
#include <rw/lapack/utrimat.h>
#include <rw/lapkerr.h>
#include <rw/math/funcobj.h>
#include <rw/math/numtrait.h>
#include <rw/math/rwalloc.h>
#include <rw/rwerr.h>
#include <string.h>

template <class TypeT, class QRCalc>
RWQRDecomp<TypeT, QRCalc>::RWQRDecomp()
{
}

template <class TypeT, class QRCalc>
RWQRDecomp<TypeT, QRCalc>::RWQRDecomp(const RWQRDecomp<TypeT, QRCalc>& x)
{
    *this = x;
}

template <class TypeT, class QRCalc>
RWQRDecomp<TypeT, QRCalc>::RWQRDecomp(const RWGenMat<TypeT>& A)
{
    factor(A);
}

template <class TypeT, class QRCalc>
RWQRDecomp<TypeT, QRCalc>& RWQRDecomp<TypeT, QRCalc>::operator=(const RWQRDecomp<TypeT, QRCalc>& x)
{
    if (&x != this) {
        tau_.reference(x.tau_);
        QR_.reference(x.QR_);
        pivots_.reference(x.pivots_);
    }
    return *this;
}



template <class TypeT, class QRCalc>
void RWQRDecomp<TypeT, QRCalc>::factor(const RWGenMat<TypeT>& A)
{
    size_t numCols = A.cols();
    RWMathVec<RWPivotType> temp(numCols, 0);
    pivots_.reference(temp);
    QR_.reference(A.copy());
    this->computeQR(QR_, tau_, pivots_.data());
}

template <class TypeT, class QRCalc>
RWQRDecomp<TypeT, QRCalc>::~RWQRDecomp()
{
}


template <class TypeT, class QRCalc>
RWGenMat<TypeT>      RWQRDecomp<TypeT, QRCalc>::P() const
{
    RWGenMat<TypeT> Pmat(cols(), cols(), (TypeT)0);
    if (pivots_.length() != 0) {
        for (int j = cols(); j--;) {
            Pmat((int)(pivots_[j] - 1), j) = TypeT(1);
        }
    }
    else {
        Pmat.diagonal() = (TypeT)1;
    }
    return Pmat;
}

template <class TypeT, class QRCalc>
RWGenMat<TypeT> RWQRDecomp<TypeT, QRCalc>::R() const
{
    int m = (cols() < rows()) ? cols() : rows();
    // Obfuscate the following line, in order to shut up a spurious
    // sun CC 3.0.1 warning
    // DComplexGenMat Rmat = QR_(RWSlice(0,m),RWAll);
    RWGenMat<TypeT> Rmat = QR_.operator()(RWSlice(0, m), RWAll);
    Rmat.deepenShallowCopy();
    for (int i = m; --i > 0;) { // Loop indexes i=m-1,...,1
        Rmat.diagonal(-i) = (TypeT)0;
    }
    return Rmat;
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT>         RWQRDecomp<TypeT, QRCalc>::Rdiagonal() const
{
    return QR_.diagonal().copy();
}

template <class TypeT, class QRCalc>
RWGenMat<TypeT>      RWQRDecomp<TypeT, QRCalc>::Q() const
{
    // Extracts the real part of not-necessarily-complex numbers
    RW_real<TypeT, norm_type> realPart;
    rw_lapack_int_t m = rows();
    rw_lapack_int_t n = cols();
    rw_lapack_int_t k = (m < n) ? m : n;
    RWGenMat<TypeT> Qmat(QR_);
    Qmat.resize((unsigned)m, (unsigned)m);
    Qmat.deepenShallowCopy();

    rw_lapack_int_t info;
    // Query for optimal work length
    rw_lapack_int_t lwork = -1;
    TypeT workLength[1];
    orgqr(m, m, k, Qmat.data(), m, (TypeT*)tau_.data(), workLength, lwork, info);
    // Allocate the work array and do the work.
    lwork = (rw_lapack_int_t)realPart(workLength[0]);

    RWPRECONDITION(lwork >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    orgqr(m, m, k, Qmat.data(), m, (TypeT*)tau_.data(), work.data(), lwork, info);

    RWPOSTCONDITION(info == 0);
    RWPOSTCONDITION((rw_lapack_int_t)realPart(work[0]) <= lwork);

    return Qmat;
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWQRDecomp<TypeT, QRCalc>::Px(const RWMathVec<TypeT>& x) const
{
    if (x.length() != cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), cols())));
    }
    if (pivots_.length() != 0) {
        RWMathVec<TypeT> y(x.length(), rwUninitialized);
        for (int i = int(x.length()); i--;) {
            y((int)pivots_[i] - 1) = x(i);
        }
        return y;
    }
    else {
        return x.copy();
    }
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWQRDecomp<TypeT, QRCalc>::PTx(const RWMathVec<TypeT>& x) const
{
    if (x.length() != cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), cols())));
    }
    if (pivots_.length() != 0) {
        RWMathVec<TypeT> y(x.length(), rwUninitialized);
        for (int i = (unsigned)x.length(); i--;) {
            y(i) = x((int)pivots_[i] - 1);
        }
        return y;
    }
    else {
        return x.copy();
    }
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWQRDecomp<TypeT, QRCalc>::Rx(const RWMathVec<TypeT>& x) const
{
    if (x.length() != cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), cols())));
    }
    int m = (cols() < rows()) ? cols() : rows();
    // explicit call to operator() in next line shuts up sun v3.0.1 warning
    RWGenMat<TypeT> R = QR_.operator()(RWSlice(0, m), RWAll);   // Also has entries for Q
    RWPRECONDITION(R.rows() <= R.cols());
    if (R.cols() > R.rows()) {
        RWUpperTriMat<TypeT> R1 = toUpperTriMat(R(RWAll, RWSlice(0, m)));
        RWGenMat<TypeT> R2 = R(RWAll, RWToEnd(m));
        return product(R1, x(RWSlice(0, m))) + product(R2, x(RWToEnd(m)));
    }
    else {
        RWUpperTriMat<TypeT> R1 = toUpperTriMat(R);
        return product(R1, x);
    }
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWQRDecomp<TypeT, QRCalc>::RTx(const RWMathVec<TypeT>& x) const
{
    int m = (cols() < rows()) ? cols() : rows();
    if (x.length() != static_cast<size_t>(m)) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), m)));
    }
    // explicit call to operator() in next line shuts up sun v3.0.1 warning
    RWGenMat<TypeT> R = QR_.operator()(RWSlice(0, m), RWAll);   // Also has entries for Q
    RWPRECONDITION(R.rows() <= R.cols());
    if (R.cols() > R.rows()) {
        RWMathVec<TypeT> y(cols(), rwUninitialized);
        RWUpperTriMat<TypeT> R1 = toUpperTriMat(R(RWAll, RWSlice(0, m)));
        RWGenMat<TypeT> R2 = R(RWAll, RWToEnd(m));
#if defined(RW_AIX_BUG_WORKAROUND)
        RWMathVec<TypeT> temp1 = product(x, conj(R1));
        RWMathVec<TypeT> temp2 = product(x, conj(R2));
        y(RWSlice(0, m)) = temp1;
        y(RWToEnd(m)) = temp2;
#else
        y(RWSlice(0, m)) = product(x, conj(R1));
        y(RWToEnd(m)) = product(x, conj(R2));
#endif
        return y;
    }
    else {
        RWUpperTriMat<TypeT> R1 = conj(toUpperTriMat(R));
        return product(x, R1);
    }
}
//end of specialization

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWQRDecomp<TypeT, QRCalc>::Rinvx(const RWMathVec<TypeT>& x) const
{
    if (x.length() != cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), cols())));
    }
    if (cols() > rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_RSINGULAR)));
    }
    RWMathVec<TypeT> y = x.copy();
    rw_lapack_int_t n = cols();
    rw_lapack_int_t m = rows();
    rw_lapack_int_t one = 1;
    rw_lapack_int_t info;
    trtrs('U', 'N', 'N', n, one, (TypeT*)QR_.data(), m, y.data(), n, info);
    RWPOSTCONDITION(info >= 0);
    if (info > 0) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_RSINGULAR)));
    }
    return y;
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWQRDecomp<TypeT, QRCalc>::RTinvx(const RWMathVec<TypeT>& x) const
{
    if (x.length() != cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), cols())));
    }
    if (cols() > rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_RSINGULAR)));
    }
    RWMathVec<TypeT> y = x.copy();
    rw_lapack_int_t n = cols();
    rw_lapack_int_t m = rows();
    rw_lapack_int_t one = 1;
    rw_lapack_int_t info;

    trtrs('U', *lapk_param_traits<TypeT>::char_param, 'N', n, one, (TypeT*)QR_.data(), m, y.data(), n, info);

    RWPOSTCONDITION(info >= 0);
    if (info > 0) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_RSINGULAR)));
    }
    return y;
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWQRDecomp<TypeT, QRCalc>::Qx(const RWMathVec<TypeT>& x) const
{
    RW_real<TypeT, norm_type> realPart;
    if (x.length() != rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), rows())));
    }
    rw_lapack_int_t m = rows();
    rw_lapack_int_t n = cols();
    rw_lapack_int_t k = (m < n) ? m : n;
    RWMathVec<TypeT> y = x.copy();
    rw_lapack_int_t one = 1;
    rw_lapack_int_t info;

    // Query for optimal lwork
    rw_lapack_int_t lwork = -1;
    TypeT workLength[1];
    mqr('L', 'N', m, one, k, (TypeT*)QR_.data(), m, (TypeT*)tau_.data(), y.data(), m, workLength, lwork, info);
    // Allocate the work array and do the work
    lwork = (rw_lapack_int_t)realPart(workLength[0]);

    RWPRECONDITION(lwork >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    mqr('L', 'N', m, one, k, (TypeT*)QR_.data(), m, (TypeT*)tau_.data(), y.data(), m, work.data(), lwork, info);
    RWPOSTCONDITION(info == 0);
    RWPOSTCONDITION((rw_lapack_int_t)realPart(work[0]) <= lwork);

    return y;
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWQRDecomp<TypeT, QRCalc>::QTx(const RWMathVec<TypeT>& x) const
{
    RW_real<TypeT, norm_type> realPart;

    if (x.length() != rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, x.length(), rows())));
    }
    rw_lapack_int_t m = rows();
    rw_lapack_int_t n = cols();
    rw_lapack_int_t k = (m < n) ? m : n;
    RWMathVec<TypeT> y = x.copy();
    rw_lapack_int_t one = 1;
    rw_lapack_int_t info;

    // Query for optimal work length
    rw_lapack_int_t lwork = -1;
    TypeT workLength[1];
    mqr('L', *lapk_param_traits<TypeT>::char_param, m, one, k, (TypeT*)QR_.data(), m, (TypeT*)tau_.data(), y.data(), m, workLength, lwork, info);
    // Allocate the work array and do the work
    lwork = (rw_lapack_int_t)realPart(workLength[0]);

    RWPRECONDITION(lwork >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(lwork));
    mqr('L', *lapk_param_traits<TypeT>::char_param, m, one, k, (TypeT*)QR_.data(), m, (TypeT*)tau_.data(), y.data(), m, work.data(), lwork, info);

    RWPOSTCONDITION(info == 0);
    RWPOSTCONDITION((rw_lapack_int_t)realPart(work[0]) <= lwork);

    return y;
}

template <class TypeT, class QRCalc>
RWQRDecompServer<TypeT, QRCalc>::RWQRDecompServer()
{
    pivot_ = true;
}

template <class TypeT, class QRCalc>
void RWQRDecompServer<TypeT, QRCalc>::setPivoting(bool x)
{
    pivot_ = x;
}

template <class TypeT, class QRCalc>
void RWQRDecompServer<TypeT, QRCalc>::setInitialIndex(int i)
{
    if (i < 0) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_CANTFREEZE, i)));
    }
    for (int j = (unsigned)initial_.length(); j--;) {
        if (initial_(j) == i) {
            return;    // already frozen
        }
    }
    initial_.resize(initial_.length() + 1);
    initial_((unsigned)initial_.length() - 1) = i;
}

template <class TypeT, class QRCalc>
void RWQRDecompServer<TypeT, QRCalc>::setFreeIndex(int i)
{
    if (i < 0) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_CANTFREE, i)));
    }
    for (int j = (unsigned)initial_.length(); j--;) {
        if (initial_(j) == i) {
            int l = (unsigned)initial_.length() - j - 1;
            if (l > 0) {
                initial_(RWSlice(j, l)) = initial_(RWSlice(j + 1, l));
            }
            initial_.resize(initial_.length() - 1);
        }
    }
}

template <class TypeT, class QRCalc>
RWQRDecomp<TypeT, QRCalc> RWQRDecompServer<TypeT, QRCalc>::operator()(const RWGenMat<TypeT>& A) const
{
    size_t m = A.rows();
    size_t n = A.cols();
    RWQRDecomp<TypeT, QRCalc> decomp;
    decomp.QR_.reference(A.copy());
    decomp.tau_.reshape(m < n ? m : n);
    if (pivot_) {
        RWMathVec<RWPivotType> temp(n, 0);

        size_t len = initial_.length();
        for (unsigned i = 0; i < len; i++) {
            int index = initial_(i);
            RWPRECONDITION(index >= 0);
            if ((unsigned)index >= n) {
                RWTHROW(RWInternalErr(RWMessage(RWLAPK_CANTFREEZE, index)));
            }
            temp[index] = 1;
        }
        decomp.pivots_.reference(temp);
    }
    else {
        decomp.pivots_.reshape(0);
    }
    RWPivotType* pivots = NULL;
    if (decomp.pivots_.length() != 0) {
        pivots = decomp.pivots_.data();
    }
    bool success = this->computeQR(decomp.QR_, decomp.tau_, pivots);
    RW_UNUSED(success);
    RWPOSTCONDITION(success);
    return decomp;
}
