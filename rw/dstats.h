#ifndef __RWDSTATS_H__
#define __RWDSTATS_H__

/***************************************************************************
 *
 * dstats.h - Declarations for global statistics functions.
 *
 * $Id: //math/13/rw/dstats.h#1 $
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
 *
 **************************************************************************/
#include <rw/mathdefs.h>
#include <rw/mathwind.h>

template< class T >
class RWMathVec;

class RW_MATH_SYMBOLIC RWFactorial
{
public:
    RWFactorial(): top_(0U) {
        A_[0] = 1.;
    }

    double operator()(size_t n);

private:
    size_t top_;
    double A_[33];
};

/**
 * @ingroup statistics_classes
 *
 * @section header_file Header File
 * @code
 * #include <rw/dstats.h>
 * @endcode
 *
 * Returns the \c Beta function:
 *
 * \f[
 * Beta(z,w) = \int\limits_0^1t^{z-1}(1-t)^{w-1}dt
 * \f]
 */
double            RW_MATH_SYMBOLIC beta(double w, double z);

/**
 * @ingroup statistics_classes
 *
 * @section header_file Header File
 * @code
 * #include <rw/dstats.h>
 * @endcode
 *
 * The function binomialPF() returns the binomial probability coefficient.
 * If an event has a probability \a p of occurring and we make
 * \a N tries, binomialPF() returns the probability that the
 * event will occur \a m times:
 *
 * \f[
 * \frac{N!}{(N-m)!m!}p^m(1-p)^{N-m}
 * \f]
 */

double            RW_MATH_SYMBOLIC binomialPF(size_t m, size_t N, double p);

/**
 * @ingroup statistics_classes
 *
 * @section header_file Header File
 * @code
 * #include <rw/dstats.h>
 * @endcode
 *
 * Returns the exponential probability function. The quantity \a x / \a a
 * has the probability distribution <i>a exp(-ax)</i>.
 */
double            RW_MATH_SYMBOLIC exponentialPF(double x, double a);

/**
 * @ingroup statistics_classes
 *
 * @section header_file Header File
 * @code
 * #include <rw/dstats.h>
 * @endcode
 *
 * Returns the factorial of \a n, which is written <i>n!</i>.
 * Stores previously calculated factorials internally for speed.
 */
inline double     RW_MATH_SYMBOLIC factorial(size_t n)
{
    RWFactorial f;
    return f(n);
}

/**
 * @ingroup statistics_classes
 *
 * @section header_file Header File
 * @code
 * #include <rw/dstats.h>
 * @endcode
 *
 * Returns the Gaussian probability function for Gaussian mean \a m
 * and standard deviation \a s:
 *
 * \f[
 *  \text{Z}(x) = \frac{1}{s\sqrt{2\pi}} \exp \left ( \frac{(x-m)^2}{2s^2} \right )
 * \f]
 */
double            RW_MATH_SYMBOLIC gaussianPF(double x, double m, double s);

/**
 * @relates RWMathVec
 *
 * @section header_file Header File
 * @code
 * #include <rw/dstats.h>
 * @endcode
 *
 * Returns the kurtosis of a vector of doubles:
 *
 * \f[
 *  \frac{1}{N}\sum_{i=0}^{N-1}\left ( \frac{x_i - \langle x \rangle}{\sigma} \right )^4 - 3
 * \f]
 *
 * where <i>\<x\></i> is the mean of the vector and \f$ \sigma \f$
 * is its standard deviation (that is, the square root of the variance).
 * The -3 term is designed to make the kurtosis 0 for a normal
 * distribution.
 */
double            RW_MATH_SYMBOLIC kurtosis(const RWMathVec<double>&);

/**
 * @ingroup statistics_classes
 *
 * @section header_file Header File
 * @code
 * #include <rw/dstats.h>
 * @endcode
 *
 * Returns the natural log of the gamma function \f$ \Gamma(x) \f$, where:
 *
 * \f[
 * \Gamma (x) = \int_{0}^{\infty} \text{t}^{x-1} \text{e}^{-t} \text{dt}
 * \f]
 */
double            RW_MATH_SYMBOLIC logGamma(double x);

/**
 * @ingroup statistics_classes
 *
 * @section header_file Header File
 * @code
 * #include <rw/dstats.h>
 * @endcode
 *
 * Returns the Lorentzian probability function, where \a m is
 * the mean of the distribution and \a w is the full width at
 * half maximum of the distribution.
 */
double            RW_MATH_SYMBOLIC lorentzianPF(double x, double m, double w);

/**
 * @ingroup statistics_classes
 *
 * @section header_file Header File
 * @code
 * #include <rw/dstats.h>
 * @endcode
 *
 * The Poisson distribution gives the probability of a certain integer
 * number of unit rate Poisson random events occurring in a given
 * time interval. The Poisson distribution represents an approximation
 * to the binomial distribution for the special case where the average
 * number of events is very much smaller than the possible number.
 * The function poissonPF() returns the probability that a Poisson
 * random event with Poisson mean \a m will occur, given \a n observations:
 *
 * \f[
 * \frac{m^n e}{n!} \frac{m^n e^{-m}}{n!}
 * \f]
 */
double            RW_MATH_SYMBOLIC poissonPF(size_t n, double m);

/**
 * @ingroup statistics_classes
 *
 * @section header_file Header File
 * @code
 * #include <rw/dstats.h>
 * @endcode
 *
 * Returns an estimate of the machine roundoff error in units of
 * \a x, typically 1. This is frequently used to determine whether
 * a number is <i>near 0</i> relative to other numbers of order
 * \a x.
 */
double            RW_MATH_SYMBOLIC rwEpslon(double x);

/**
 * @relates RWMathVec
 *
 * @section header_file Header File
 * @code
 * #include <rw/dstats.h>
 * @endcode
 *
 * Returns a sorted vector, smallest to largest.
 */
RWMathVec<double> RW_MATH_SYMBOLIC sort(const RWMathVec<double>&);

RWMathVec<size_t> RW_MATH_SYMBOLIC sort(const RWMathVec<size_t>&);
RWMathVec<int>    RW_MATH_SYMBOLIC sort(const RWMathVec<int>&);

/**
 * @relates RWMathVec
 *
 * @section header_file Header File
 * @code
 * #include <rw/dstats.h>
 * @endcode
 *
 * Returns the skewness of a vector of doubles:
 *
 * \f[
 * \frac{1}{N}\sum_{i=0}^{N-1}\left ( \frac{x_{i}-\left \langle x \right \rangle}{\sigma} \right )^3
 * \f]
 *
 * where <i>\<x\></i> is the mean of the vector and \f$ \sigma \f$
 * is its standard deviation, that is, the square root of the variance.
 */
double            RW_MATH_SYMBOLIC skewness(const RWMathVec<double>&);

#endif /*__RWDSTATS_H__*/
