
/*
 * The conversion functions to/from general matrices.
 *
 *
 * $Id: //lapack/13/rw/lapack/hermcg.cc#1 $
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


template <class TypeT>
RWHermMat<TypeT>::operator RWGenMat<TypeT>() const
{
    RWGenMat<TypeT> temp(rows(), cols(), TypeT(0));
    for (unsigned j = 0; j < rows(); j++) {
        for (register unsigned i = 0; i <= j; i++) {
            TypeT x = val(i, j);
            temp(j, i) = RW_MATH_SL_STD_GLOBAL(conj)(x);
            temp(i, j) = x;
        }
    }
    return temp;
}

template <class TypeT>
RWHermMat<TypeT> toHermMat(const RWGenMat<TypeT>& S)
{
    unsigned n = (unsigned)S.cols();
    RWHermMat<TypeT> A((unsigned)S.rows(), n);
    for (unsigned j = 0; j < n; j++) {
        for (unsigned i = 0; i < j; i++) {
            A.bcset(i, j, (S.val(i, j) + RW_MATH_SL_STD_GLOBAL(conj)(S.val(j, i))) / TypeT(2, 0));
        }
        A.bcset(j, j, S.val(j, j)); // We don't force imaginary part to zero
    }
    return A;
}

template <class TypeT>
RWHermMat<TypeT> upperToHermMat(const RWGenMat<TypeT>& S)
{
    unsigned n = (unsigned)S.cols();
    RWHermMat<TypeT> A(n, n);
    for (unsigned j = 0; j < n; j++) {
        for (unsigned i = 0; i <= j; i++) {                // This loop should be in a bla call for nonsmall j
            A.set(i, j, S.val(i, j));
        }
    }
    return A;
}

template <class TypeT>
RWHermMat<TypeT> lowerToHermMat(const RWGenMat<TypeT>& S)
{
    unsigned n = S.cols();
    RWHermMat<TypeT> A(n, n);
    for (unsigned j = 0; j < n; j++) {
        for (unsigned i = j; i < n; i++) {                // This loop should be in a bla call for nonsmall j
            A.set(i, j, S.val(i, j));
        }
    }
    return A;
}
