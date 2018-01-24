/*
 * Implementation of RWHermBandTriDiagDecomp
 *
 *
 * $Id: //lapack/13/rw/lapack/bandtd.cc#1 $
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

#include <rw/lapack/hbndmat.h>
#include <rw/lapack/lapkfunc.h>
#include <rw/lapack/latraits.h>
#include <rw/lapkerr.h>
#include <rw/math/rwalloc.h>
#include <rw/rwerr.h>

template <class TypeT>
RWBandTriDiagDecomp<TypeT>::RWBandTriDiagDecomp(const typename rw_linear_algebra_traits<TypeT>::banded_hermitian_type& A, bool keepQ)
{
    if (A.rows() > 0 && A.bandwidth() > 0) {
        char vect = (keepQ) ? 'V' : 'N';
        char uplo = 'U';   // Indicates to lapack that upper triangle is stored
        rw_lapack_int_t n = A.rows();
        rw_lapack_int_t kd = A.upperBandwidth();
        typename rw_linear_algebra_traits<TypeT>::banded_hermitian_type Acopy = A.copy();
        rw_lapack_int_t ldab = kd + 1;
        RWMathVec<rt> D(A.rows(), rwUninitialized);
        RWMathVec<rt> E(A.rows() - 1, rwUninitialized);
        if (keepQ) {
            Q.reshape(A.rows(), A.rows());
        }
        RWPRECONDITION(n >= 0);
        RWTWorkBuffer<TypeT> work(static_cast<size_t>(n));
        rw_lapack_int_t info;
        sbtrd(vect, uplo, n, kd, (TypeT*)Acopy.data(), ldab, (rt*)D.data(), (rt*)E.data(), (TypeT*)Q.data(), n, work.data(), info);
        RWPOSTCONDITION(info == 0);
        this->set(D, E);
    }
}

template <class TypeT>
RWMathVec<TypeT> RWBandTriDiagDecomp<TypeT>::transform(const RWMathVec<rt>& V) const
{
    return RWTriDiagDecomp<TypeT>::transform(V);
}

template <class TypeT>
RWGenMat<TypeT> RWBandTriDiagDecomp<TypeT>::transform(const RWGenMat<rt>& C) const
{
    if (C.cols() == 0 || C.rows() == 0) {
        return C;
    }
    else {
        if (C.rows() != this->rows()) {
            RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, C.rows(), this->rows())));
        }
        if (Q.rows() == 0) {
            RWTHROW(RWInternalErr(RWMessage(RWLAPK_QNOTCOMPUTED)));
        }
        return product(Q, C);
    }
}

//specialize for DComplex:
template <>
inline
RWGenMat<DComplex> RWBandTriDiagDecomp<DComplex>::transform(const RWGenMat<rt>& C) const
{
    return transformSpecialized(C);
}
template <class TypeT>
RWGenMat<DComplex> RWBandTriDiagDecomp<TypeT>::transformSpecialized(const RWGenMat<double>& C) const
{
    if (C.cols() == 0 || C.rows() == 0) {
        return RWConvertGenMat<double, DComplex>(C);
    }
    else {
        if (C.rows() != this->rows()) {
            RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, C.rows(), this->rows())));
        }
        if (Q.rows() == 0) {
            RWTHROW(RWInternalErr(RWMessage(RWLAPK_QNOTCOMPUTED)));
        }
        RWGenMat<DComplex> Ctemp = RWConvertGenMat<double, DComplex>(C);
        return product(Q, Ctemp);
    }
}

//end of specialization
