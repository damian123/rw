/*
 * defs for RWLeastSqSV
 *
 *
 * $Id: //lapack/13/rw/lapack/lssv.cc#1 $
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

#include <rw/math/numtrait.h>
#include <rw/lapkerr.h>
#include <rw/rwerr.h>

template< class TypeT >
class TypeToType
{
public:
    TypeToType() {
        ;
    }
    typedef TypeT OriginalType;
};


template <class TypeT, class SVDCalc>
RWLeastSqSV<TypeT, SVDCalc>::RWLeastSqSV() : RWSVDecomp<TypeT, SVDCalc>() {}

template <class TypeT, class SVDCalc>
RWLeastSqSV<TypeT, SVDCalc>::RWLeastSqSV(const RWGenMat<TypeT>& A, double tol)
    : RWSVDecomp<TypeT, SVDCalc>(A, tol)
{
    ;
}

template <class TypeT, class SVDCalc>
RWLeastSqSV<TypeT, SVDCalc>::RWLeastSqSV(const RWSVDecomp<TypeT, SVDCalc>& A, double tol)
    : RWSVDecomp<TypeT, SVDCalc>(A)
{
    this->truncate(tol);
}


template <class TypeT, class SVDCalc>
/*static*/ RWGenMat<TypeT> U(const RWLeastSqSV<TypeT, SVDCalc>& x)
{
    if (x.numLeftVectors() < x.rank()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOSV, x.rank())));
    }
    return x.leftVectors()(RWAll, RWSlice(0, x.rank()));
}

template <class TypeT, class SVDCalc>
/*static*/ RWGenMat<TypeT> V(const RWLeastSqSV<TypeT, SVDCalc>& x)
{
    if (x.numRightVectors() < x.rank()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOSV, x.rank())));
    }
    return x.rightVectors()(RWAll, RWSlice(0, x.rank()));
}


template <class TypeT, class SVDCalc>
RWMathVec<TypeT> RWLeastSqSV<TypeT, SVDCalc>::solve(const RWMathVec<TypeT>& data) const
{
    if (data.length() != this->rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, data.length(), this->rows())));
    }
    RWMathVec<TypeT> x = product(adjoint(U(*this)), data);
    //x /= this->singularValues();
    for (unsigned i = 0; i < this->singularValues().length(); i++) {
        x[i] /= TypeT(this->singularValues()[i]);
    }
    return product(V(*this), x);
}


template <class TypeT, class SVDCalc>
RWMathVec<TypeT> RWLeastSqSV<TypeT, SVDCalc>::residual(const RWMathVec<TypeT>& data) const
{
    if (data.length() != this->rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, data.length(), this->rows())));
    }
    RWMathVec<TypeT> x = solve(data);
    RWMathVec<TypeT> VTx = product(adjoint(V(*this)), x);
    //VTx *= this->singularValues();
    for (unsigned i = 0; i < this->singularValues().length(); i++) {
        VTx[i] *= TypeT(this->singularValues()[i]);
    }
    RWMathVec<TypeT> Ax = product(U(*this), VTx);
    return data - Ax;
}



template <class TypeT, class SVDCalc>
typename rw_numeric_traits<TypeT>::norm_type RWLeastSqSV<TypeT, SVDCalc>::residualNorm(const RWMathVec<TypeT>& data) const
{
    if (data.length() != this->rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, data.length(), this->rows())));
    }
    RWMathVec<TypeT> r = residual(data);
    return l2Norm(r);
}
