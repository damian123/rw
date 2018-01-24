/*
 * defs for RWLeastSqCh
 *
 *
 * $Id: //lapack/13/rw/lapack/lsch.cc#1 $
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

#include <rw/rwerr.h>
#include <rw/lapkerr.h>

template <class TypeT>
RWLeastSqCh<TypeT>::RWLeastSqCh() {}

template <class TypeT>
RWLeastSqCh<TypeT>::RWLeastSqCh(const RWGenMat<TypeT>& A)
{
    factor(A);
}

template <class TypeT>
void      RWLeastSqCh<TypeT>::factor(const RWGenMat<TypeT>& A)
{
    A_.reference(A);
    RWSymMat<TypeT> ATA = upperToSymMat(transposeProduct(A, A));
    decomp_.factor(ATA);
}
//specialize for DComplex:
template <>
inline void      RWLeastSqCh<DComplex>::factor(const RWGenMat<DComplex>& A)
{
    factor_specialized(A);
}

template <class TypeT>
void      RWLeastSqCh<TypeT>::factor_specialized(const RWGenMat<TypeT>& A)
{
    A_.reference(A);
    RWHermMat<DComplex> ATA = upperToHermMat(conjTransposeProduct(A, A));
    ATA.makeDiagonalReal();
    decomp_.factor(ATA);
}
//end of specialization

template <class TypeT>
RWMathVec<TypeT>    RWLeastSqCh<TypeT>::residual(const RWMathVec<TypeT>& data) const
{
    RWMathVec<TypeT> x = solve(data);
    return data - product(A_, x);
}

template <class TypeT>
typename rw_numeric_traits<TypeT>::norm_type       RWLeastSqCh<TypeT>::residualNorm(const RWMathVec<TypeT>& data) const
{
    if (data.length() != rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, data.length(), rows())));
    }
    return l2Norm(residual(data));
}

template <class TypeT>
RWMathVec<TypeT>    RWLeastSqCh<TypeT>::solve(const RWMathVec<TypeT>& data) const
{
    if (decomp_.fail()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_CANTSOLVELS)));
    }
    RWMathVec<TypeT> ATb = product(transpose(A_), data);
    return ::solve(decomp_, ATb);
}
//specialization for DComplex:
template <>
inline RWMathVec<DComplex>    RWLeastSqCh<DComplex>::solve(const RWMathVec<DComplex>& data) const
{
    return solve_specialized(data);
}

template <class TypeT>
RWMathVec<TypeT>    RWLeastSqCh<TypeT>::solve_specialized(const RWMathVec<TypeT>& data) const
{
    if (decomp_.fail()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_CANTSOLVELS)));
    }
    RWMathVec<DComplex> ATb = product(conj(transpose(A_)), data);
    return ::solve(decomp_, ATb);
}
//end of specialization
