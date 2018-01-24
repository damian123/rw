/*
 * Implementation of RWHermQREigServer
 *
 * Uses Lapack's QR/QL algorithm to compute eigenvalues/vectors of
 * a tridiagonal matrix.
 *
 *
 * $Id: //lapack/13/rw/lapack/heigqr.cc#1 $
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
RWHermQREigServer<TypeT>::RWHermQREigServer(bool vecs)
{
    computeVecs_ = vecs;
}

template <class TypeT>
RWSymEigDecomp<double> RWHermQREigServer<TypeT>::decompose(const RWTriDiagDecomp<TypeT>& decomp)
{
    char compz = computeVecs_ ? 'I' : 'N';
    rw_lapack_int_t n = decomp.rows();
    RWMathVec<double> d = decomp.diagonal().copy();
    RWMathVec<double> e(decomp.rows(), rwUninitialized);  // lapack needs a length n (not n-1) off diagonal
    if (n > 0) {
        e(RWSlice(0, decomp.rows() - 1)) = decomp.offDiagonal();
    }
    unsigned zsize = computeVecs_ ? decomp.rows() : 0;
    RWGenMat<double> Z(zsize, zsize, rwUninitialized);
    rw_lapack_int_t ldz = computeVecs_ ? n : 1;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<double> work(computeVecs_ ? static_cast<size_t>(2 * n + 1) : 0); // Needs only 2*n-2, but this way we avoid <0 checks
    rw_lapack_int_t info;
    //dsteqr(compz,n,d.data(),e.data(),Z.data(),ldz,work,info);
    steqr(compz, n, d.data(), e.data(), Z.data(), ldz, work.data(), info);
    RWPOSTCONDITION(info >= 0);

    RWSymEigDecomp<double> eig;
    RWHermQREigServer<TypeT>::n(eig) = decomp.rows();
    (this->lambda(eig)).reference(d);
    (this->P(eig)).reference(Z);
    if (info > 0) {
        unsigned numComputed = (unsigned)info;   // Make an explicit unsigned to avoid warnings on SUN
        (this->lambda(eig)).resize(numComputed);
        (this->P(eig)).resize(decomp.rows(), numComputed);
        this->computedAll(eig) = false;
    }
    return eig;
}

template <class TypeT>
bool RWHermQREigServer<TypeT>::computeEigenVectors() const
{
    return computeVecs_;
}

template <class TypeT>
void RWHermQREigServer<TypeT>::computeEigenVectors(bool x)
{
    computeVecs_ = x;
}
