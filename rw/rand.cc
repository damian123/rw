#ifndef __RAND_CC__
#define __RAND_CC__

#include <rw/mathdefs.h>

#ifdef RW_MATH_NO_NEW_HEADER
#  include <math.h>
#else
#  include <cmath>
#endif
#include <rw/dstats.h>

/***********************************************************************
*
* Abstract base class RWTRand
*
* $Id: //math/13/rw/rand.cc#1 $
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
***********************************************************************
*
* $Log: rand.cc,v $
* Revision 1.9  1996/05/13 20:37:28  steves
* Put the copyrite macro inside of a comment block.
*
* Revision 1.8  1996/04/30  18:39:46  steves
* Added multiple include guards.
* IBM port
*
* Revision 1.7  1996/04/24 20:38:35  steves
* Changed name of random number classes from RWRand* to RWTRand*
*
* Revision 1.6  1996/04/22  21:09:49  steves
* added copyrite years macro
*
***********************************************************************/

// Constructors ---------------------------
template< class Generator >
RWTRand<Generator>::RWTRand()
{
    ;
}

template< class Generator >
RWTRand<Generator>::RWTRand(const RWTRand<Generator>& r)
    : RWRandInterface(r)
    , generator_(r.generator_)
{
    ;
}

template< class Generator >
RWTRand<Generator>::RWTRand(const Generator& x)
    : generator_(x)
{
    ;
}


// Destructor -------------------------------
template< class Generator >
RWTRand<Generator>::~RWTRand()
{
    ;
}


// Member Functions ------------------------


// Set the underlying random number generator
template< class Generator >
void RWTRand<Generator>::setGenerator(const Generator& x)
{
    generator_ = x;
}

// Set the underlying random number generator
template< class Generator >
void RWTRand<Generator>::setGenerator(const RWTRand<Generator>& r)
{
    generator_ = r.generator_;
}



//***********************************************************************
//
// Class RWTRandUniform
//
//***********************************************************************

// Constructors --------------------------
template< class Generator >
RWTRandUniform<Generator>::RWTRandUniform()
    : min_(0), max_(1)
{
    ;
}

template< class Generator >
RWTRandUniform<Generator>::RWTRandUniform(double a, double b)
{
    if (a > b) {
        max_ = a;
        min_ = b;
    }
    else {
        max_ = b;
        min_ = a;
    }
}


template< class Generator >
RWTRandUniform<Generator>::RWTRandUniform(const Generator& gen, double a, double b)
    : RWTRand<Generator>(gen)
{
    if (a > b) {
        max_ = a;
        min_ = b;
    }
    else {
        max_ = b;
        min_ = a;
    }
}


template< class Generator >
RWTRandUniform<Generator>::RWTRandUniform(const RWTRandUniform<Generator>& x)
    : RWTRand<Generator>(x)
{
    min_ = x.min_;
    max_ = x.max_;
}

template< class Generator >
double RWTRandUniform<Generator>::_doRandom()
{
    double initialDeviate = this->generator_();
    double deviateInRange = (max_ - min_) * initialDeviate + min_;
    if (deviateInRange > max_) {  // Check for round off error.
        return max_;
    }
    if (deviateInRange < min_) {
        return min_;
    }

    return deviateInRange;
}

template< class Generator >
size_t RWTRandUniform<Generator>::_doRandomArray(double* out, size_t n)
{
    for (; n; --n, ++out) {
        *out = _doRandom();
    }
    return n;
}

template< class Generator >
double RWTRandUniform<Generator>::operator()()
{
    return _doRandom();
}

template< class Generator >
size_t RWTRandUniform<Generator>::operator()(double* out, size_t n)
{
    return _doRandomArray(out, n);
}

template< class Generator >
double RWTRandUniform<Generator>::lowBound() const
{
    return min_;
}

template< class Generator >
double RWTRandUniform<Generator>::highBound() const
{
    return max_;
}

template< class Generator >
void RWTRandUniform<Generator>::setRange(double a, double b)
{
    if (a < b) {
        min_ = a;
        max_ = b;
    }
    else {
        min_ = b;
        max_ = a;
    }
}


//***********************************************************************
//
// Class RWTRandNormal
//
//***********************************************************************

// Constructors -----------------------------------
template< class Generator >
RWTRandNormal<Generator>::RWTRandNormal()
    : mean_(0), variance_(1), haveOne_(0)
{
    stdDeviation_ = RWMTH_STD::sqrt(variance_);
}

template< class Generator >
RWTRandNormal<Generator>::RWTRandNormal(double mean, double variance)
    : mean_(mean), variance_(variance), haveOne_(0)
{
    stdDeviation_ = sqrt(variance_);
}

template< class Generator >
RWTRandNormal<Generator>::RWTRandNormal(const RWTRandNormal<Generator>& x)
    : RWTRand<Generator>(x), mean_(x.mean_), variance_(x.variance_),
      stdDeviation_(x.stdDeviation_), randNum_(x.randNum_), haveOne_(x.haveOne_)
{
    ;
}

template< class Generator >
RWTRandNormal<Generator>::RWTRandNormal(const Generator& gen, double m, double v)
    : RWTRand<Generator>(gen), mean_(m), variance_(v), haveOne_(0)
{
    stdDeviation_ = sqrt(variance_);
}

template< class Generator >
double RWTRandNormal<Generator>::_doRandom()
{
    if (haveOne_) {
        haveOne_ = 0;
        return randNum_;
    }

    // pick (v1,v2) in the unit circle
    double v1, v2, rsquared;
    do {
        v1 = 2 * (this->generator_()) - 1.0; // -1 <= v1 <= 1
        v2 = 2 * (this->generator_()) - 1.0; // -1 <= v2 <= 1
        rsquared = v1 * v1 + v2 * v2;
    }
    while (rsquared >= 1.0 || rsquared == 0.0);

    double factor = RWMTH_STD::sqrt(-2.0 * RWMTH_STD::log(rsquared) / rsquared);
    randNum_ = mean_ + stdDeviation_ * v1 * factor;
    haveOne_ = 1;

    return mean_ + stdDeviation_ * v2 * factor;
}

template< class Generator >
size_t RWTRandNormal<Generator>::_doRandomArray(double* out, size_t n)
{
    for (; n; --n, ++out) {
        *out = _doRandom();
    }
    return n;
}

template< class Generator >
size_t RWTRandNormal<Generator>::operator()(double* out, size_t n)
{
    return _doRandomArray(out, n);
}

template< class Generator >
double RWTRandNormal<Generator>::operator()()
{
    return _doRandom();
}

template< class Generator >
void RWTRandNormal<Generator>::setVariance(double variance)
{
    variance_ = variance;
    stdDeviation_ = sqrt(variance_);
}


template< class Generator >
double RWTRandNormal<Generator>::lowBound() const
{
    return mean_ - 2.58 * sqrt(variance_);
}

template< class Generator >
double RWTRandNormal<Generator>::highBound() const
{
    return mean_ + 2.58 * stdDeviation_;
}

//***********************************************************************
//
// Class RWTRandExponential
//
//***********************************************************************

// Constructors -----------------------------------
template< class Generator >
RWTRandExponential<Generator>::RWTRandExponential(double lambda)
{
    lambda_ = lambda;
}

template< class Generator >
RWTRandExponential<Generator>::RWTRandExponential(const RWTRandExponential<Generator>& x)
    : RWTRand<Generator>(x)
{
    lambda_ = x.lambda_;
}

template< class Generator >
RWTRandExponential<Generator>::RWTRandExponential(const Generator& gen, double lambda)
    : RWTRand<Generator>(gen)
{
    lambda_ = lambda;
}

template< class Generator >
double RWTRandExponential<Generator>::_doRandom()
{
    double r = 2 * (this->generator_()) - 1.0; // -1 <= r  <= 1
    double xi = (r + 1.0) / 2.0;      // 0  <= xi <= 1
    return - RWMTH_STD::log(xi) / lambda_;
}


template< class Generator >
size_t RWTRandExponential<Generator>::_doRandomArray(double* out, size_t n)
{
    for (; n; --n, ++out) {
        *out = _doRandom();
    }
    return n;
}

template< class Generator >
size_t RWTRandExponential<Generator>::operator()(double* out, size_t n)
{
    return _doRandomArray(out, n);
}

template< class Generator >
double RWTRandExponential<Generator>::operator()()
{
    return _doRandom();
}

template< class Generator >
double RWTRandExponential<Generator>::lowBound() const
{
    return 0;
}

template< class Generator >
double RWTRandExponential<Generator>::highBound() const
{
    return 4.605170219 / lambda_; // ln(0.01) = -4.6...
}

//***********************************************************************
//
// Class RWTRandGamma
//
//***********************************************************************

// Constructors ---------------------------------------
template< class Generator >
RWTRandGamma<Generator>::RWTRandGamma(int order)
    : order_(order)
{
    ;
}

template< class Generator >
RWTRandGamma<Generator>::RWTRandGamma(const RWTRandGamma<Generator>& g)
    : RWTRand<Generator>(g), order_(g.order_)
{
    ;
}

template< class Generator >
RWTRandGamma<Generator>::RWTRandGamma(const Generator& gen, int order)
    : RWTRand<Generator>(gen), order_(order)
{
    ;
}

//------------------------------------------------------

template< class Generator >
size_t RWTRandGamma<Generator>::operator()(double* out, size_t n)
{
    return _doRandomArray(out, n);
}

template< class Generator >
double RWTRandGamma<Generator>::operator()()
{
    return _doRandom();
}


template< class Generator >
size_t RWTRandGamma<Generator>::_doRandomArray(double* out, size_t n)
{
    for (; n; --n, ++out) {
        *out = this->operator()();
    }
    return n;
}

template< class Generator >
double RWTRandGamma<Generator>::_doRandom()
{
    double x;
    if (order_ < 6) { // direct method
        x = 1.;
        for (int j = 0; j < order_; j++) {
            x *= this->generator_();
        }

        return - RWMTH_STD::log(x);
    }

    else { // rejection method
        double v1, v2, y, s, e;
        int am;
        do {
            do {
                do {
                    v1 = 2.* (this->generator_()) - 1.; // pick two uniform
                    v2 = 2.* (this->generator_()) - 1.; // numbers from -1 to 1
                }
                while (v1 * v1 + v2 * v2 > 1.);

                y = v2 / v1;
                am = order_ - 1;
                s = RWMTH_STD::sqrt(2.*am + 1.);
                x = s * y + am;
            }
            while (x <= 0.);

            e = (1. + y * y) * RWMTH_STD::exp(am * RWMTH_STD::log(x / am) - s * y);

        }
        while (this->generator_() > e);

        return x;
    }
}


//***********************************************************************
//
// Class RWTRandPoisson
//
//***********************************************************************

// Constructors ---------------------------------------
template< class Generator >
RWTRandPoisson<Generator>::RWTRandPoisson(double mean)
    : mean_(mean)
{
    initPrivateData();
}


template< class Generator >
RWTRandPoisson<Generator>::RWTRandPoisson(const RWTRandPoisson<Generator>& g)
    : RWTRand<Generator>(g), mean_(g.mean_)
{
    initPrivateData();
}

template< class Generator >
RWTRandPoisson<Generator>::RWTRandPoisson(const Generator& gen, double mean)
    : RWTRand<Generator>(gen), mean_(mean)
{
    initPrivateData();
}


template< class Generator >
void RWTRandPoisson<Generator>::setMean(double m)
{
    mean_ = m;
    initPrivateData();
}

template< class Generator >
void RWTRandPoisson<Generator>::initPrivateData()
{
    if (mean_ < 12.) {
        g_     = RWMTH_STD::exp(-mean_);
        sq_    = 0.;
        lmean_ = 0.;
    }
    else {
        sq_    = RWMTH_STD::sqrt(2.*mean_);
        lmean_ = RWMTH_STD::log(mean_);
        g_     = mean_ * lmean_ - logGamma(mean_ + 1.);
    }
}

// Return a random number selected from an poisson distribution
// with mean: mean_

template< class Generator >
double RWTRandPoisson<Generator>::_doRandom()
{
    double em, t;
    if (mean_ < 12.) {
        em = -1.;
        t  = 1.;
        do {
            em += 1.;
            t  *= this->generator_();
        }
        while (t > g_);
    }
    else {
        double y;
        do {
            do {
                y  = RWMTH_STD::tan(RW_M_PI * (this->generator_()));
                em = sq_ * y + mean_;
            }
            while (em < 0.);
            em = RWMTH_STD::floor(em);
            t  = .9 * (1. + y * y) * RWMTH_STD::exp(em * lmean_ - logGamma(em + 1.) - g_);
        }
        while (this->generator_() > t);
    }
    return em;
}

template< class Generator >
size_t RWTRandPoisson<Generator>::_doRandomArray(double* out, size_t n)
{
    for (; n; --n, ++out) {
        *out = _doRandom();
    }
    return n;
}

template< class Generator >
size_t RWTRandPoisson<Generator>::operator()(double* out, size_t n)
{
    return _doRandomArray(out, n);
}

template< class Generator >
double RWTRandPoisson<Generator>::operator()()
{
    return _doRandom();
}

//***********************************************************************
//
// Class RWTRandBinomial
//
//***********************************************************************

// Constructors ---------------------------------------------------------
template< class Generator >
RWTRandBinomial<Generator>::RWTRandBinomial(size_t N, double probability)
    : n_(N), p_(probability)
{
    initData();
}

template< class Generator >
RWTRandBinomial<Generator>::RWTRandBinomial(const Generator& gen, size_t N,
        double probability)
    : RWTRand<Generator>(gen), n_(N), p_(probability)
{
    initData();
}


template< class Generator >
RWTRandBinomial<Generator>::RWTRandBinomial(const RWTRandBinomial<Generator>& r)
    : RWTRand<Generator>(r), n_(r.n_), p_(r.p_), prb_(r.prb_), g_(r.g_), am_(r.am_), pc_(r.pc_),
      plog_(r.plog_), pclog_(r.pclog_), sq_(r.sq_)
{
    ;
}

// Member Functions --------------------------------------------------------
template< class Generator >
void RWTRandBinomial<Generator>::initData()
{
    if (p_ <= .5) {
        prb_ = p_;
    }
    else {
        prb_ = 1 - p_;
    }

    if (n_ < 25) {
        g_     = 0.;
        am_    = 0.;
        pc_    = 0.;
        sq_    = 0.;
        plog_  = 0.;
        pclog_ = 0.;
    }
    else {
        am_ = n_ * prb_;
        if (am_ < 1.) {
            g_ = RWMTH_STD::exp(-am_);
            pc_    = 0.;
            sq_    = 0.;
            plog_  = 0.;
            pclog_ = 0.;
        }
        else {
            g_  = logGamma(double(n_ + 1));
            pc_ = 1. - prb_;
            sq_ = RWMTH_STD::sqrt(2.*am_ * pc_);
            plog_ = RWMTH_STD::log(prb_);
            pclog_ = RWMTH_STD::log(pc_);
        }
    }
}

template< class Generator >
void RWTRandBinomial<Generator>::setN(size_t N)
{
    n_ = N;
    initData();
}

template< class Generator >
void RWTRandBinomial<Generator>::setP(double probability)
{
    p_   = probability;
    initData();
}

// Return a random number selected from a binomial distribution
// of (n_) trials each of probability (p_)

template< class Generator >
double RWTRandBinomial<Generator>::_doRandom()
{
    double temp;
    double t;
    size_t j;
    if (n_ < 25) {
        temp = 0.;
        for (size_t i = 0; i < n_; i++) {
            if (this->generator_() < prb_) {
                temp += 1.;
            }
        }
    }
    else if (am_ < 1.) {
        t = 1.;
        for (j = 0; j < n_ + 1; j++) {
            t *= this->generator_();
            if (t < g_) {
                break;
            }
        }
        temp = (double)j;
    }
    else {
        double y, em;
        do {
            do {
                y = RWMTH_STD::tan(RW_M_PI * (this->generator_()));
                em = sq_ * y + am_;
            }
            while (em < 0. || em >= n_ + 1);
            em = RWMTH_STD::floor(em);
            t = 1.2 * sq_ * (1. + y * y) *
                RWMTH_STD::exp(g_ - logGamma(em + 1.) - logGamma(n_ - em + 1.)
                               + em * plog_ + (n_ - em) * pclog_);
        }
        while (this->generator_() > t);
        temp = em;
    }
    if (p_ != prb_) {
        return (n_ - temp);
    }
    else {
        return temp;
    }
}

template< class Generator >
size_t RWTRandBinomial<Generator>::_doRandomArray(double* out, size_t n)
{
    for (; n; --n, ++out) {
        *out = _doRandom();
    }
    return n;
}

template< class Generator >
size_t RWTRandBinomial<Generator>::operator()(double* out, size_t n)
{
    return _doRandomArray(out, n);
}

template< class Generator >
double RWTRandBinomial<Generator>::operator()()
{
    return _doRandom();
}

#endif // __RAND_CC__
