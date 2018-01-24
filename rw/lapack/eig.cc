/*
 * Implementation of RWEigDecomp
 *
 *
 * $Id: //lapack/13/rw/lapack/eig.cc#1 $
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


#include <rw/lapack/eigsrv.h>
#include <rw/rwerr.h>
#include <rw/lapkerr.h>

#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

template <class TypeT>
class RWSchurEigServer;

template <class TypeT>
RWEigDecomp<TypeT>::RWEigDecomp()
{
    n = 0;
    computedAll = accurate = true;
}


template <class TypeT>
RWEigDecomp<TypeT>::RWEigDecomp(const RWEigDecomp<double>& x)
    : n(x.n),
      lambda(x.lambda),
      P(x.P),
      Q(x.Q),
      computedAll(x.computedAll),
      accurate(x.accurate)
{
}

//specialize for DComplex:
template <>
inline RWEigDecomp<DComplex>::RWEigDecomp(const RWEigDecomp<double>& x)
    : n(x.n),
      lambda(x.lambda),
      P(RWGenMat<DComplex>(RWConvertGenMat<double, DComplex>(x.P))),
      Q(RWGenMat<DComplex>(RWConvertGenMat<double, DComplex>(x.Q))),
      computedAll(x.computedAll),
      accurate(x.accurate)
{
}
//end of specialization



template <class TypeT>
RWEigDecomp<TypeT>::RWEigDecomp(const RWGenMat<TypeT>& A, bool computeVecs)
{
    factor(A, computeVecs);
}

template <class TypeT>
void RWEigDecomp<TypeT>::factor(const RWGenMat<TypeT>& A, bool computeVecs)
{
    RWSchurEigServer<TypeT> server;
    server.computeLeftEigenVectors(computeVecs);
    server.computeRightEigenVectors(computeVecs);
    *this = server(A);
}

template <class TypeT>
RWEigDecomp<TypeT>& RWEigDecomp<TypeT>::operator=(const RWEigDecomp<TypeT>& A)
{
    if (&A != this) {
        n = A.n;
        computedAll = A.computedAll;
        accurate = A.accurate;
        lambda.reference(A.lambda);
        P.reference(A.P);
        Q.reference(A.Q);
    }
    return *this;
}

template <class TypeT>
DComplex       RWEigDecomp<TypeT>::eigenValue(int i) const
{
    if (i < 0 || (size_t)i >= lambda.length()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_NOEIG, i)));
    }
    return lambda(i);
}

template <class TypeT>
const  RWMathVec<DComplex>   RWEigDecomp<TypeT>::leftEigenVector(int i) const
{
    if (i < 0 || (size_t)i >= P.cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_NOEIG, i)));
    }
    RWPOSTCONDITION(lambda.length() > (size_t)i); // if this isn't true something went wrong
    DComplex eval = lambda(i);
    if (RW_MATH_SL_STD_GLOBAL(imag)(eval) == 0) {
        return RWConvertMathVec<TypeT, DComplex>(P.col(i));      // A real eigenvalue
    }
    else {                   // A complex eigenvalue
        if (i > 0 && lambda(i - 1) == RW_MATH_SL_STD_GLOBAL(conj)(eval)) {
            return RWMathVec<DComplex>(P.col(i - 1), -P.col(i));
        }
        else {
            RWPOSTCONDITION((size_t)i < lambda.length() && lambda(i + 1) == RW_MATH_SL_STD_GLOBAL(conj)(eval)); // if this fails the evals aren't a pair
            return RWMathVec<DComplex>(P.col(i), P.col(i + 1));
        }
    }
}


//specialize for DComplex:
template <>
inline const RWMathVec<DComplex>    RWEigDecomp<DComplex>::leftEigenVector(int i) const
{
    if (i < 0 || (size_t)i >= P.cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_NOEIG, i)));
    }
    RWPOSTCONDITION(lambda.length() > (size_t)i); // if this isn't true something went wrong
    return P.col(i);
}
//end of specialization


template <class TypeT>
const RWGenMat<DComplex> RWEigDecomp<TypeT>::leftEigenVectors() const
{
    //RWGenMat<DComplex> vecs = RWConvertGenMat<TypeT,DComplex>(P);
    RWGenMat<DComplex> vecs = RWGenMat<DComplex>(RWConvertGenMat<TypeT, DComplex>(P));

    for (int i = (int)vecs.cols(); i--;) { // fix up complex conjugate pairs of vals
        if (RW_MATH_SL_STD_GLOBAL(imag)(lambda(i)) != 0) {
            real(vecs.col(i)) = P.col(i - 1);
            imag(vecs.col(i)) = -P.col(i);
            i--;
            imag(vecs.col(i)) = P.col(i + 1);
        }
    }
    return vecs;
}

//specialize for DComplex:
template <>
inline const RWGenMat<DComplex> RWEigDecomp<DComplex>::leftEigenVectors() const
{
    return P;
}
//end of specialization

template <class TypeT>
const RWMathVec<DComplex>    RWEigDecomp<TypeT>::rightEigenVector(int i) const
{
    if (i < 0 || (size_t)i >= Q.cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_NOEIG, i)));
    }
    RWPOSTCONDITION(lambda.length() > (size_t)i); // if this isn't true something went wrong
    DComplex eval = lambda(i);
    if (RW_MATH_SL_STD_GLOBAL(imag)(eval) == 0) {
        return RWConvertMathVec<TypeT, DComplex>(Q.col(i));      // A real eigenvalue
    }
    else {                   // A complex eigenvalue
        if (i > 0 && lambda(i - 1) == RW_MATH_SL_STD_GLOBAL(conj)(eval)) {
            return RWMathVec<DComplex>(Q.col(i - 1), -Q.col(i));
        }
        else {
            RWPOSTCONDITION((size_t)i < lambda.length() && lambda(i + 1) == RW_MATH_SL_STD_GLOBAL(conj)(eval)); // if this fails the evals aren't a pair
            return RWMathVec<DComplex>(Q.col(i), Q.col(i + 1));
        }
    }
}


//specialize for DComplex:
template <>
inline const RWMathVec<DComplex>    RWEigDecomp<DComplex>::rightEigenVector(int i) const
{
    if (i < 0 || (size_t)i >= Q.cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_NOEIG, i)));
    }
    RWPOSTCONDITION(lambda.length() > (size_t)i); // if this isn't true something went wrong
    return Q.col(i);
}
//end of specialization


template <class TypeT>
const RWGenMat<DComplex> RWEigDecomp<TypeT>::rightEigenVectors() const
{
    //RWGenMat<DComplex> vecs = RWConvertGenMat<TypeT,DComplex>(Q);
    RWGenMat<DComplex> vecs = RWGenMat<DComplex>(RWConvertGenMat<TypeT, DComplex>(Q));

    for (int i = (int)vecs.cols(); i--;) { // fix up complex conjugate pairs of vals
        if (RW_MATH_SL_STD_GLOBAL(imag)(lambda(i)) != 0) {
            real(vecs.col(i)) = Q.col(i - 1);
            imag(vecs.col(i)) = -Q.col(i);
            i--;
            imag(vecs.col(i)) = Q.col(i + 1);
        }
    }
    return vecs;
}

//specialize for DComplex:
template <>
inline const RWGenMat<DComplex> RWEigDecomp<DComplex>::rightEigenVectors() const
{
    return Q;
}
//end of specialization

template <class TypeT>
bool RWEigDecomp<TypeT>::good() const
{
    return computedAll && accurate;
}

template <class TypeT>
bool RWEigDecomp<TypeT>::inaccurate() const
{
    return computedAll && !accurate;
}

template <class TypeT>
bool RWEigDecomp<TypeT>::fail() const
{
    return !computedAll;
}

#ifdef _WIN32
#  pragma warning(pop)
#endif
