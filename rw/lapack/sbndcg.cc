
/*
 * The conversion functions to/from general matrices.
 *
 * $Id: //lapack/13/rw/lapack/sbndcg.cc#1 $
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
#include <rw/math/genmat.h>

#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

template <class TypeT>
RWSymBandMat<TypeT>::operator RWGenMat<TypeT>() const
{
    RWGenMat<TypeT> temp(rows(), cols(), TypeT(0));
    for (int k = halfBandwidth(); k >= 0; k--) {
        RWMathVec<TypeT> d = diagonal(k);
        temp.diagonal(-k) = d;
        temp.diagonal(k) = d;
    }
    return temp;
}

template <class TypeT>
RWSymBandMat<TypeT> toSymBandMat(const RWGenMat<TypeT>& S, unsigned bandu)
{
    unsigned n = (unsigned)S.cols();
    RWSymBandMat<TypeT> A((unsigned)S.rows(), n, bandu);
    A.diagonal(0) = S.diagonal(0);
    for (int k = 1; (unsigned)k <= bandu; k++) {
        RWMathVec<TypeT> Adiag = A.diagonal(k);
        Adiag = S.diagonal(k);
        Adiag += S.diagonal(-k);
        Adiag /= 2;
    }
    return A;
}

#ifdef _WIN32
#  pragma warning(pop)
#endif
