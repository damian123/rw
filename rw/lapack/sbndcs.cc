
/*
 * Functions for shape conversion for DoubleSymBandMat, except
 * conversion to/from square matrices.
 *
 * $Id: //lapack/13/rw/lapack/sbndcs.cc#1 $
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
#include <rw/lapkerr.h>
#include <rw/rwerr.h>
#include <rw/lapack/bandmat.h>

#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

template <class TypeT>
RWBandMat<TypeT>::RWBandMat(const RWSymBandMat<TypeT>& A)
    : vec(A.bandwidth()*A.rows(), rwUninitialized)
{
    n = A.rows();
    band = A.bandwidth();
    bandu = A.upperBandwidth();
    for (int i = 0; i <= bandu; i++) {
        RWMathVec<TypeT> d = A.diagonal(i);
        diagonal(i) = d;
        if (i > 0) {
            diagonal(-i) = d;
        }
    }
}

template <class TypeT>
RWSymBandMat<TypeT> toSymBandMat(const RWBandMat<TypeT>& S)
{
    unsigned n = S.rows();
    unsigned l = S.lowerBandwidth();
    unsigned u = S.upperBandwidth();
    if (l != u) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSYM)));
    }
    RWSymBandMat<TypeT> A(n, n, u);
    if (n >= 1) {
        A.diagonal(0) = S.diagonal(0);
    }
    for (int i = (u < n) ? u : n; i >= 1; i--) {
        RWMathVec<TypeT> Adiag(A.diagonal(i));
        Adiag = S.diagonal(i);      // Avoid generating temporaries
        Adiag += S.diagonal(-i);
        Adiag /= 2;
    }
    return A;
}

#ifdef _WIN32
#  pragma warning(pop)
#endif
