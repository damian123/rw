/*
 * Implementation of RWHermEigDecomp
 *
 *
 * $Id: //lapack/13/rw/lapack/hermeig.cc#1 $
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

//#include <rw/lapack/hermeig.h>
#include <rw/lapack/heigsrv.h>
#include <rw/rwerr.h>
#include <rw/lapkerr.h>

#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

//template <class TypeT>
//typedef RWHermQREigServer<TypeT> DefaultServer;

template <class TypeT>
RWHermEigDecomp<TypeT>::RWHermEigDecomp()
{
    n = 0;
    computedAll = accurate = true;
}

template <class TypeT>
RWHermEigDecomp<TypeT>::RWHermEigDecomp(const RWSymEigDecomp<double>& x)
    : n(x.n),
      lambda(x.lambda),
      P(RWGenMat<TypeT>(RWConvertGenMat<double, TypeT>(x.P))),
      computedAll(x.computedAll),
      accurate(x.accurate)
{
}

template <class TypeT>
RWHermEigDecomp<TypeT>::RWHermEigDecomp(const RWHermMat<TypeT>& A, bool computeVecs)
{
    factor(A, computeVecs);
}

template <class TypeT>
RWHermEigDecomp<TypeT>::RWHermEigDecomp(const RWHermBandMat<TypeT>& A, bool computeVecs)
{
    factor(A, computeVecs);
}

template <class TypeT>
void RWHermEigDecomp<TypeT>::factor(const RWHermMat<TypeT>& A, bool computeVecs)
{
    //DefaultServer server;
    RWHermQREigServer<TypeT> server;
    server.computeEigenVectors(computeVecs);
    *this = server(A);
}

template <class TypeT>
void RWHermEigDecomp<TypeT>::factor(const RWHermBandMat<TypeT>& A, bool computeVecs)
{
    //DefaultServer server;
    RWHermQREigServer<TypeT> server;
    server.computeEigenVectors(computeVecs);
    *this = server(A);
}

template <class TypeT>
RWHermEigDecomp<TypeT>& RWHermEigDecomp<TypeT>::operator=(const RWHermEigDecomp<TypeT>& A)
{
    if (&A != this) {
        n = A.n;
        computedAll = A.computedAll;
        accurate = A.accurate;
        lambda.reference(A.lambda);
        P.reference(A.P);
    }
    return *this;
}

template <class TypeT>
double       RWHermEigDecomp<TypeT>::eigenValue(int i) const
{
    if (i < 0 || (size_t)i >= lambda.length()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_NOEIG, i)));
    }
    return lambda(i);
}

template <class TypeT>
const RWMathVec<TypeT>    RWHermEigDecomp<TypeT>::eigenVector(int i) const
{
    if (i < 0 || (size_t)i >= P.cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_NOEIG, i)));
    }
    return P.col(i);
}

template <class TypeT>
bool RWHermEigDecomp<TypeT>::good() const
{
    return computedAll && accurate;
}

template <class TypeT>
bool RWHermEigDecomp<TypeT>::inaccurate() const
{
    return computedAll && !accurate;
}

template <class TypeT>
bool RWHermEigDecomp<TypeT>::fail() const
{
    return !computedAll;
}

#ifdef _WIN32
#  pragma warning(pop)
#endif
