/*
 * Implementation of RWHermSomeEigServer and RWHermRangeEigServer
 *
 *
 * $Id: //lapack/13/rw/lapack/heigbis.cc#1 $
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
#include <rw/lapkerr.h>
#include <rw/math/rwalloc.h>
#include <rw/rwerr.h>

#ifdef RW_MATH_NO_NEW_HEADER
#  include <float.h>
#else
#  include <cfloat>
#endif  /* looking for max and min values to initialize RangeEigServers */
#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

template <class TypeT>
RWHermSomeEigServer<TypeT>::RWHermSomeEigServer(bool computeVecs)
    : range((const RWSlice&)RWAll)  // Sun seems to need the explicit cast
{
    computeVecs_ = computeVecs;
    tolerance_ = 0;
}

template <class TypeT>
double               RWHermSomeEigServer<TypeT>::setTolerance(double t)
{
    double temp = tolerance_;
    tolerance_ = t;
    return temp;
}

template <class TypeT>
RWSlice RWHermSomeEigServer<TypeT>::setRange(const RWSlice& x)
{
    RWSlice temp = range;
    range = x;
    return temp;
}

template <class TypeT>
bool RWHermSomeEigServer<TypeT>::computeEigenVectors() const
{
    return computeVecs_;
}

template <class TypeT>
void              RWHermSomeEigServer<TypeT>::computeEigenVectors(bool x)
{
    computeVecs_ = x;
}

template <class TypeT>
RWSymEigDecomp<double>   RWHermSomeEigServer<TypeT>::decompose(const RWTriDiagDecomp<TypeT>& decomp)
{
    RWSymEigDecomp<double> eig;
    RWHermEigServer<TypeT>::n(eig) = decomp.rows();

    // Put together a few things both the lapack routines can use
    rw_lapack_int_t n = decomp.rows();
    RWMathVec<double> d = decomp.diagonal().copy();
    RWMathVec<double> e(decomp.rows(), rwUninitialized);  // lapack needs a length n (not n-1) off diagonal
    if (n > 0) {
        e(RWSlice(0, decomp.rows() - 1)) = decomp.offDiagonal();
    }
    RWMathVec<double> w(decomp.rows(), rwUninitialized); // eigenvalues in here
    rw_lapack_int_t m;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<rw_lapack_int_t> iblock(static_cast<size_t>(n));
    RWTWorkBuffer<rw_lapack_int_t> isplit(static_cast<size_t>(n));
    RWTWorkBuffer<double> work(static_cast<size_t>(5 * n));
    RWTWorkBuffer<rw_lapack_int_t> iwork(static_cast<size_t>(3 * n));
    rw_lapack_int_t info;

    // determine range of interest, and make sure it is sensible.
    rw_lapack_int_t il = range.begin() + 1;
    rw_lapack_int_t iu = il + range.stride() * (range.len(decomp.rows()) - 1);
    if (range.stride() < 0) {
        rw_lapack_int_t temp = il;
        il = iu;
        iu = temp;
    }
    if (iu > (rw_lapack_int_t)decomp.rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOEIG, iu)));
    }

    // compute eigenvalues
    if (iu >= il) { // ie skip if we don't want any eigenvalues
        rw_lapack_int_t nsplit;
        double junk;
        //dstebz('I','E',n,junk,junk,il,iu,tolerance_,d.data(),e.data(),m,nsplit,w.data(),iblock,isplit,work,iwork,info);
        stebz('I', 'E', n, junk, junk, il, iu, tolerance_, d.data(), e.data(), m, nsplit, w.data(), iblock.data(), isplit.data(), work.data(), iwork.data(), info);
        RWPOSTCONDITION(info >= 0);
        RWPOSTCONDITION(m <= iu - il + 1);
        if (m < iu - il + 1) {
            this->computedAll(eig) = false;
        }
        if (info == 1 || info == 3) {
            this->accurate(eig) = false;
        }
        // toss out some if some weirdo used a non-unit stride, reverse
        // for negative stride
        if (range.stride() < 0) {
            w(RWRange(int(w.length() - 1), 0)) = w;    // reverse
        }
        int str = range.stride();
        if (str < 0) {
            str = (-str);
        }
        if (str > 1) {
            m = (m + str - 1) / str;
            w(RWSlice(0, (unsigned)m)) = w(RWToEnd(0, str));
        }
        (this->lambda(eig)).reference(w(RWSlice(0, (unsigned)m)));

        // compute eigenvectors
        if (computeVecs_ && m >= 1) {
            (this->P(eig)).reshape(decomp.rows(), (unsigned)m);
            //dstein(n,d.data(),e.data(),m,w.data(),iblock,isplit,P(eig).data(),n,work,iwork,iwork+n,info);
            stein(n, d.data(), e.data(), m, w.data(), iblock.data(), isplit.data(), (this->P(eig)).data(), n, work.data(), iwork.data(), iwork.data() + n, info);
            RWPOSTCONDITION(info >= 0);
            if (info > 0) {
                this->accurate(eig) = false;
            }
        }
    }

    return eig;
}

/********************************************
 * Implementation of RWHermRangeEigServer *
 ********************************************/

template <class TypeT>
RWHermRangeEigServer<TypeT>::RWHermRangeEigServer(bool computeVecs)
{
    computeVecs_ = computeVecs;
    tolerance_ = 0;
    small_ = -DBL_MAX;
    large_ = DBL_MAX;
}

template <class TypeT>
void              RWHermRangeEigServer<TypeT>::setRange(double x, double y)
{
    small_ = x;
    large_ = y;
}

template <class TypeT>
double               RWHermRangeEigServer<TypeT>::setSmall(double x)
{
    double temp = small_;
    small_ = x;
    return temp;
}

template <class TypeT>
double               RWHermRangeEigServer<TypeT>::setLarge(double x)
{
    double temp = large_;
    large_ = x;
    return temp;
}

template <class TypeT>
double               RWHermRangeEigServer<TypeT>::setTolerance(double t)
{
    double temp = tolerance_;
    tolerance_ = t;
    return temp;
}

template <class TypeT>
bool RWHermRangeEigServer<TypeT>::computeEigenVectors() const
{
    return computeVecs_;
}

template <class TypeT>
void              RWHermRangeEigServer<TypeT>::computeEigenVectors(bool x)
{
    computeVecs_ = x;
}

template <class TypeT>
RWSymEigDecomp<double>   RWHermRangeEigServer<TypeT>::decompose(const RWTriDiagDecomp<TypeT>& decomp)
{
    RWSymEigDecomp<double> eig;
    RWHermEigServer<TypeT>::n(eig) = decomp.rows();

    // Put together a few things both the lapack routines can use
    rw_lapack_int_t n = decomp.rows();
    RWMathVec<double> d = decomp.diagonal().copy();
    RWMathVec<double> e(decomp.rows(), rwUninitialized);  // lapack needs a length n (not n-1) off diagonal
    if (n > 0) {
        e(RWSlice(0, decomp.rows() - 1)) = decomp.offDiagonal();
    }
    RWMathVec<double> w(decomp.rows(), rwUninitialized); // eigenvalues in here
    rw_lapack_int_t m;
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<rw_lapack_int_t> iblock(static_cast<size_t>(n));
    RWTWorkBuffer<rw_lapack_int_t> isplit(static_cast<size_t>(n));
    RWTWorkBuffer<double> work(static_cast<size_t>(5 * n));
    RWTWorkBuffer<rw_lapack_int_t> iwork(static_cast<size_t>(3 * n));
    rw_lapack_int_t info;

    // compute eigenvalues
    if (large_ > small_) { // ie skip if we don't want any eigenvalues
        rw_lapack_int_t nsplit;
        rw_lapack_int_t junk;
        //dstebz('V','E',n,small_,large_,junk,junk,tolerance_,d.data(),e.data(),m,nsplit,w.data(),iblock,isplit,work,iwork,info);
        stebz('V', 'E', n, small_, large_, junk, junk, tolerance_, d.data(), e.data(), m, nsplit, w.data(), iblock.data(), isplit.data(), work.data(), iwork.data(), info);
        RWPOSTCONDITION(info >= 0);
        if (info == 4) {
            this->computedAll(eig) = false;
        }
        if (info == 1 || info == 3) {
            this->accurate(eig) = false;
        }
        (this->lambda(eig)).reference(w(RWSlice(0, (unsigned)m)));

        // compute eigenvectors
        if (computeVecs_ && m >= 1) {
            (this->P(eig)).reshape(decomp.rows(), (unsigned)m);
            //dstein(n,d.data(),e.data(),m,w.data(),iblock,isplit,P(eig).data(),n,work,iwork,iwork+n,info);
            stein(n, d.data(), e.data(), m, w.data(), iblock.data(), isplit.data(), (this->P(eig)).data(), n, work.data(), iwork.data(), iwork.data() + n, info);
            RWPOSTCONDITION(info >= 0);
            if (info > 0) {
                this->accurate(eig) = false;
            }
        }
    }

    return eig;
}

#ifdef _WIN32
#  pragma warning(pop)
#endif
