/*
 * Implementation of RWSymRFQREigServer
 *
 * Uses Lapack's root free QR algorithm to compute eigenvalues of
 * a symmetric matrix.
 *
 * $Id: //lapack/13/rw/lapack/seigrf.cc#1 $
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

//#include <rw/lapack/seigsrv.h>
#include <rw/lapack/td.h>
#include <rw/lapack/lapkfunc.h>


template <class TypeT>
RWSymRFQREigServer<TypeT>::RWSymRFQREigServer()
{
}

template <class TypeT>
RWSymEigDecomp<TypeT> RWSymRFQREigServer<TypeT>::decompose(const RWTriDiagDecomp<TypeT>& decomp)
{
    rw_lapack_int_t n = decomp.rows();
    RWMathVec<TypeT> d = decomp.diagonal().copy();
    RWMathVec<TypeT> e(decomp.rows(), rwUninitialized);  // lapack needs a length n (not n-1) off diagonal
    if (n > 0) {
        e(RWSlice(0, decomp.rows() - 1)) = decomp.offDiagonal();
    }
    rw_lapack_int_t info;
    //dsterf(n,d.data(),e.data(),info);
    sterf(n, d.data(), e.data(), info);
    RWPOSTCONDITION(info >= 0);

    RWSymEigDecomp<TypeT> eig;
    RWSymEigServer<TypeT>::n(eig) = decomp.rows();
    this->lambda(eig).reference(d);
    if (info > 0) {
        this->lambda(eig).resize((unsigned)info);  // explicit cast to avoid warnings on SUN
        this->computedAll(eig) = false;
    }
    return eig;
}

template <class TypeT>
bool RWSymRFQREigServer<TypeT>::computeEigenVectors() const
{
    return false;
}
