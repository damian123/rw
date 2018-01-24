/*
 * defs for RWLeastSqQR
 *
 *
 * $Id: //lapack/13/rw/lapack/lsqr.cc#1 $
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

#include <rw/lapkerr.h>
#include <rw/rwerr.h>


template <class TypeT, class QRCalc>
RWLeastSqQR<TypeT, QRCalc>::RWLeastSqQR(): RWCODecomp<TypeT, QRCalc>()
{
    ;
}

template <class TypeT, class QRCalc>
RWLeastSqQR<TypeT, QRCalc>::RWLeastSqQR(const RWGenMat<TypeT>& A, double tol)
    : RWCODecomp<TypeT, QRCalc>(A, tol)
{
    ;
}

template <class TypeT, class QRCalc>
RWLeastSqQR<TypeT, QRCalc>::RWLeastSqQR(const RWQRDecomp<TypeT, QRCalc>& A, double tol)
    : RWCODecomp<TypeT, QRCalc>(A, tol)
{
    ;
}

template <class TypeT, class QRCalc>
RWLeastSqQR<TypeT, QRCalc>::RWLeastSqQR(const RWCODecomp<TypeT, QRCalc>& A)
    : RWCODecomp<TypeT, QRCalc>(A)
{
    ;
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWLeastSqQR<TypeT, QRCalc>::residual(const RWMathVec<TypeT>& data) const
{
    if (data.length() != this->rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, data.length(), this->rows())));
    }
    RWMathVec<TypeT> d = this->QTx(data);
    d(RWSlice(0, this->rank())) = (TypeT)0;
    return this->Qx(d);
}

template <class TypeT, class QRCalc>
typename rw_numeric_traits<TypeT>::norm_type RWLeastSqQR<TypeT, QRCalc>::residualNorm(const RWMathVec<TypeT>& data) const
{
    if (data.length() != this->rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, data.length(), this->rows())));
    }
    RWMathVec<TypeT> d = this->QTx(data);
    RWMathVec<TypeT> d2 = d(RWToEnd(this->rank()));
    return l2Norm(d2);
}

template <class TypeT, class QRCalc>
RWMathVec<TypeT> RWLeastSqQR<TypeT, QRCalc>::solve(const RWMathVec<TypeT>& data) const
{
    if (data.length() != this->rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, data.length(), this->rows())));
    }
    RWMathVec<TypeT> d = this->QTx(data);
    RWMathVec<TypeT> d1 = d(RWSlice(0, this->rank()));
    RWMathVec<TypeT> c(this->cols(), rwUninitialized);
    c(RWSlice(0, this->rank())) = this->Tinvx(d1);
    c(RWToEnd(this->rank())) = (TypeT)0;
    return this->Px(this->Zx(c));
}
