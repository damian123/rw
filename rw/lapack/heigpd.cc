/*
 * Implementation of RWHermPDQREigServer
 *
 * Compute eigenvalues/vectors of a positive definite tridiagonal matrix.
 *
 *
 * $Id: //lapack/13/rw/lapack/heigpd.cc#1 $
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
#include <rw/math/rwalloc.h>


template <class TypeT>
RWHermPDQREigServer<TypeT>::RWHermPDQREigServer(bool vecs)
{
    computeVecs_ = vecs;
}

template <class TypeT>
RWSymEigDecomp<double> RWHermPDQREigServer<TypeT>::decompose(const RWTriDiagDecomp<TypeT>& decomp)
{
    char compz = computeVecs_ ? 'I' : 'N';
    rw_lapack_int_t n = decomp.rows();
    RWMathVec<double> d = decomp.diagonal().copy();
    RWMathVec<double> e = decomp.offDiagonal().copy();
    unsigned zsize = computeVecs_ ? decomp.rows() : 0;
    RWGenMat<double> Z(zsize, zsize, rwUninitialized);
    rw_lapack_int_t ldz = computeVecs_ ? n : 1;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<double> work(static_cast<size_t>(4 * n + 1)); // Needs only 4*n-4, but this way we avoid <0 checks
    rw_lapack_int_t info;
    //dpteqr(compz,n,d.data(),e.data(),Z.data(),ldz,work,info);
    pteqr(compz, n, d.data(), e.data(), Z.data(), ldz, work.data(), info);
    RWPOSTCONDITION(info >= 0);

    RWSymEigDecomp<double> eig;
    if (info > 0) {
        this->computedAll(eig) = false;
    }
    if (info == 0) {
        RWHermPDQREigServer<TypeT>::n(eig) = decomp.rows();
        (this->lambda(eig)).reference(d);
        (this->P(eig)).reference(Z);
    }
    return eig;
}

template <class TypeT>
bool RWHermPDQREigServer<TypeT>::computeEigenVectors() const
{
    return computeVecs_;
}

template <class TypeT>
void RWHermPDQREigServer<TypeT>::computeEigenVectors(bool x)
{
    computeVecs_ = x;
}
