#ifndef __RWSV_CC__
#define __RWSV_CC__

/*
 * Defs for RWSVDecomp
 *
 * $Id: //lapack/13/rw/lapack/sv.cc#1 $
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
 *
 */

#include <rw/lapkdefs.h>
#include <rw/lapack/lapkfunc.h>
#include <rw/rwerr.h>

#include <rw/lapkerr.h>
#include <rw/math/genmat.h>
#include <rw/math/mathvec.h>
#include <rw/math/numtrait.h>
#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

template <class TypeT, class SVDCalc>
RWSVDecomp<TypeT, SVDCalc>::RWSVDecomp()
{
    computedAll_ = true;
}

template <class TypeT, class SVDCalc>
RWSVDecomp<TypeT, SVDCalc>::RWSVDecomp(const RWSVDecomp<TypeT, SVDCalc>& x)
{
    *this = x;
}

template <class TypeT, class SVDCalc>
RWSVDecomp<TypeT, SVDCalc>::RWSVDecomp(const RWGenMat<TypeT>& X, norm_type tol)
{
    factor(X, tol);
}

template <class TypeT, class SVDCalc>
void RWSVDecomp<TypeT, SVDCalc>::factor(const RWGenMat<TypeT>& X, norm_type tol)
{
    this->computedAll_ = this->computeSVD(X, U_, VT_, sigma_, tol);
}

template <class TypeT, class SVDCalc>
RWSVDecomp<TypeT, SVDCalc>&
RWSVDecomp<TypeT, SVDCalc>::operator=(const RWSVDecomp<TypeT, SVDCalc>& x)
{
    if (&x != this) {
        sigma_.reference(x.sigma_);
        U_.reference(x.U_);
        VT_.reference(x.VT_);
        computedAll_ = x.computedAll_;
    }
    return *this;
}

template <class TypeT, class SVDCalc>
typename rw_numeric_traits<TypeT>::norm_type RWSVDecomp<TypeT, SVDCalc>::singularValue(int i) const
{
    if (i >= 0 && (unsigned)i < sigma_.length()) {
        return sigma_(i);
    }
    return 0;
}

template <class TypeT, class SVDCalc>
const RWMathVec<TypeT>    RWSVDecomp<TypeT, SVDCalc>::leftVector(int i) const
{
    if (i < 0 || (unsigned)i >= U_.cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_NOSV, i)));
    }
    return U_.col(i);
}

template <class TypeT, class SVDCalc>
const RWMathVec<TypeT>    RWSVDecomp<TypeT, SVDCalc>::rightVector(int i) const
{
    if (i < 0 || (unsigned)i >= VT_.rows()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_NOSV, i)));
    }
    return conj(VT_.row(i));
}


template <class TypeT, class SVDCalc>
void            RWSVDecomp<TypeT, SVDCalc>::truncate(norm_type tol)
{
    RWPRECONDITION(tol >= 0);
    unsigned r = (unsigned)sigma_.length();
    while (r > 0 && sigma_(r - 1) <= tol) {
        --r;
    }
    sigma_.resize(r);
}

template <class TypeT, class SVDCalc>
RWSVServer<TypeT, SVDCalc>::RWSVServer()
{
    computeAllLeftVecs_ = false;
    computeAllRightVecs_ = false;
    computeDefaultLeftVecs_ = true;
    computeDefaultRightVecs_ = true;
    numLeftVecs_ = 0;
    numRightVecs_ = 0;
    tol_ = 0;
}

template <class TypeT, class SVDCalc>
void        RWSVServer<TypeT, SVDCalc>::computeAllVectors()
{
    computeAllLeftVectors();
    computeAllRightVectors();
}

template <class TypeT, class SVDCalc>
void        RWSVServer<TypeT, SVDCalc>::computeAllLeftVectors()
{
    computeAllLeftVecs_ = true;
}

template <class TypeT, class SVDCalc>
void        RWSVServer<TypeT, SVDCalc>::computeAllRightVectors()
{
    computeAllRightVecs_ = true;
}

template <class TypeT, class SVDCalc>
void        RWSVServer<TypeT, SVDCalc>::computeDefaultVectors()
{
    computeDefaultLeftVectors();
    computeDefaultRightVectors();
}

template <class TypeT, class SVDCalc>
void        RWSVServer<TypeT, SVDCalc>::computeDefaultLeftVectors()
{
    computeAllLeftVecs_ = false;
    computeDefaultLeftVecs_ = true;
}

template <class TypeT, class SVDCalc>
void        RWSVServer<TypeT, SVDCalc>::computeDefaultRightVectors()
{
    computeAllRightVecs_ = false;
    computeDefaultRightVecs_ = true;
}

template <class TypeT, class SVDCalc>
void        RWSVServer<TypeT, SVDCalc>::computeVectors(unsigned n)
{
    computeLeftVectors(n);
    computeRightVectors(n);
}

template <class TypeT, class SVDCalc>
void        RWSVServer<TypeT, SVDCalc>::computeLeftVectors(unsigned n)
{
    computeAllLeftVecs_ = computeDefaultLeftVecs_ = false;
    numLeftVecs_ = n;
}

template <class TypeT, class SVDCalc>
void        RWSVServer<TypeT, SVDCalc>::computeRightVectors(unsigned n)
{
    computeAllRightVecs_ = computeDefaultRightVecs_ = false;
    numRightVecs_ = n;
}

template <class TypeT, class SVDCalc>
void        RWSVServer<TypeT, SVDCalc>::setTolerance(norm_type x)
{
    tol_ = x;
}

template <class TypeT, class SVDCalc>
RWSVDecomp<TypeT, SVDCalc> RWSVServer<TypeT, SVDCalc>::operator()(const RWGenMat<TypeT>& A) const
{
    RWSVDecomp<TypeT, SVDCalc> decomp;  // We'll return this someday

    // get number of right vectors to compute.
    int numRight = numRightVecs_;
    if (computeDefaultRightVecs_) {
        numRight = -1; // Compute the default number of right vectors
    }
    if ((numRight > 0 && numRight > (long)A.cols()) || computeAllRightVecs_) {
        numRight = (unsigned)A.cols();
    }

    // get number of left vectors to compute.
    int numLeft = numLeftVecs_;
    if (computeDefaultLeftVecs_) {
        numLeft = -1; // Compute the default number of left vectors.
    }
    if ((numLeft > 0 && numLeft > (long)A.rows()) || computeAllLeftVecs_) {
        numLeft = (unsigned)A.rows();
    }

    decomp.computedAll_ = this->computeSVD(A, decomp.U_, decomp.VT_, decomp.sigma_, tol_, numLeft, numRight);
    return decomp;
}

#ifdef _WIN32
#  pragma warning(pop)
#endif
#endif // __RWSV_CC__
