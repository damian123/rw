#ifndef __RWRAND_H__
#define __RWRAND_H__

/***************************************************************************
 *
 * rand.h - Random number generator classes
 *
 * $Id: //math/13/rw/rand.h#1 $
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
 ***************************************************************************
 *
 * These random number classes are new to math.h++ v5.1.  The old classes
 * are available in math.h++ 5.1 in rand*.h.  They will be phased out.
 *
 *
 *   RWTRand:             abstract base class for distribution generators
 *   RWTRandUniform:    random numbers from a uniform distribution
 *   RWTRandNormal:     random numbers from a normal distribution
 *   RWTRandExponential:random numbers from an exponential distribution
 *   RWTRandGamma:      random numbers from a gamma distribution
 *   RWTRandPoisson:    random integers from a Poisson distribution
 *   RWTRandBinomial:   random integers from a binomial distribution
 *
 **************************************************************************/

#include <rw/mathdefs.h>
#include <rw/mathwind.h>
#include <rw/randgen.h>
#include <rw/rwtbrngen.h>

/**
 * @ingroup statistics_classes
 * @brief Abstract base class for \link RWTRand RWTRand<Generator> \endlink
 *
 * Abstract base class for \link RWTRand RWTRand<Generator>\endlink.
 */
class RW_MATH_SYMBOLIC RWRandInterface
{
public:
    /**
     * Function call operator. Defined by derived classes to return
     * a random number from the appropriate probability distribution.
     */
    virtual double operator()() = 0;

    // fill an array of random deviates
    virtual size_t operator()(double*, size_t) = 0;

    virtual ~RWRandInterface() {};
};

/**
 * @ingroup statistics_classes
 * @brief Abstract base class from which the random number generator
 * classes derive.
 *
 * Class \link RWTRand RWTRand<Generator> \endlink is the abstract
 * base class for the random number generator classes. The classes
 * derived from \link RWTRand RWTRand<Generator> \endlink generate random
 * numbers from specific probability distributions. Each of these derived
 * classes generates a random number from a uniform distribution on the
 * interval [0, 1], and from this deviate derives a random number
 * generator from the appropriate probability distribution. The
 * template parameter \c Generator is a function object that provides
 * this uniform [0, 1] deviate. Specifically, \c Generator is presumed
 * to be an object that overloads the function call operator to
 * return a uniform random number of type \c double between 0 and
 * 1.
 *
 * The class Generator must have the following semantics, where
 * \c gen is of type Generator:
 *
 * <table>
 * <tr>
 * <td><b>Default Constructor:</b> </td>
 * <td>\c Generator \c gen; </td>
 * </tr>
 * <tr>
 * <td><b>Function Call Operator:</b> </td>
 * <td>
 * \c double \c x \c = \c gen(); \c  \c // \c 0 \c <= \c x
 * \c <= \c 1 \c or \c 0 \c < \c x \c < \c 1
 * </td>
 * </tr>
 * </table>
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rand.h>
 * @endcode
 */
template< class Generator >
class RWTRand : public RWRandInterface
{
protected:
    // not private since derived classes use the generator
    Generator generator_;
public:
    /**
     * Uses the default constructor of class Generator to initialize
     * the underlying uniform random number generator.
     */
    RWTRand();

    /**
     * Initializes the underlying uniform random number generator with
     * \a g.
     */
    RWTRand(const Generator& g);

    /**
     * Initializes the underlying uniform random number generator with
     * \a x's.
     */
    RWTRand(const RWTRand<Generator>& x);

    virtual ~RWTRand();

    /**
     * Returns the underlying uniform random number generator as a \c
     * const reference.
     */
    const Generator& generator() const {
        return generator_;
    }

    /**
     * Returns a reference to the underlying uniform random number generator.
     */
    Generator& generator() {
        return generator_;
    }

    /**
     * Sets the underlying uniform random number generator to \a x.
     */
    void setGenerator(const Generator& x);

    /**
     * Sets the underlying uniform random number generator to the underlying
     * uniform random number generator of \a x.
     */
    void setGenerator(const RWTRand<Generator>& x);
};


/**
 * @ingroup statistics_classes
 * @brief Used to generate random numbers from a uniform distribution
 * in an interval [a, b].
 *
 * Class \link RWTRandUniform RWTRandUniform<Generator> \endlink
 * generates random numbers from a uniform distribution in an interval
 * [a, b]. The algorithm used to generate the random numbers begins
 * with random numbers generated from a uniform distribution on
 * the interval [0, 1]. Thus, the speed and statistical properties
 * of the uniform number generator may be changed by varying the
 * Generator template parameter responsible for generating the uniformly
 * distributed numbers.
 *
 * The class Generator must be a function object whose function
 * call operator returns a uniform random double between 0 and 1.
 * The class RWRandGenerator may be used.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rand.h>
 * RWTRandUniform<Generator> gen;
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <rw/rand.h>
 * #include <iostream.h>
 * #include "myrand.h"
 *
 * int main() {
 * // Create a uniform generator on [0, 1] with an
 * // initial seed value.
 *   RWRandGenerator uniform(123456L);
 *
 * // Create a uniform generator on [-1, 1] using the above
 * // generator.
 *   RWTRandUniform<RWRandGenerator> uniform(uniform, -1.0, 1.0);
 *
 * // Create a uniform generator on [0 100] using class
 * // RWRandGenerator with a random seed.
 *   RWTRandUniform<RWRandGenerator> uniformOne(0.0, 100.0);
 *
 * // Create a uniform generator on [3.14, 345.67] using a
 * // class MyDoubleRand to generate the uniform [0, 1] deviates.
 *   RWTRandUniform<MyDoubleRand> myUniform( 3.14, 345.67 );
 *
 * // Print a few values.
 *   for ( int j = 0; j < 10; j++ )
 *   {
 *     cout << "uniform [-1, 1] = " << uniform();
 *     cout << "\nuniform [0, 100] = " << uniformOne();
 *     cout << "\nuniform [3.14, 345.67] = " << myUniform()
 *          << endl;
 *   }
 *
 * // Restart one of the generators with another seed value.
 *   ( uniformOne.generator() ).restart( 654321L);
 *
 *   return 0;
 * }
 * @endcode
 *
 */
template< class Generator >
class RWTRandUniform : public RWTRand<Generator>
{
private:
    double _doRandom();
    size_t _doRandomArray(double*, size_t);

public:
    /**
     * Constructs a generator with default range [0, 1].
     */
    RWTRandUniform();

    /**
     * Constructs a generator with range [\a a, \a b].
     */
    RWTRandUniform(double a, double b);

    /**
     * Constructs self as a copy of \a g.
     */
    RWTRandUniform(const RWTRandUniform<Generator>& g);

    /**
     * Constructs a generator with underlying uniform [0, 1] generator
     * \a g and range [\a a, \a b]
     */
    RWTRandUniform(const Generator& g, double a = 0, double b = 1);

    /**
     * Returns the next random number in the sequence.
     */
    virtual double operator()();

    /**
     * Assigns random numbers to \a n elements in \a out. Returns the
     * number of random numbers generated.
     */
    virtual size_t operator()(double* out, size_t n);

    /**
     * Returns the lower range value.
     */
    double lowBound() const;

    /**
     * Returns the upper range value.
     */
    double highBound() const;

    /**
     * Sets the range to [\a a, \a b].
     */
    void   setRange(double a, double b);

private:
    double min_, max_ ; // values generated are in [min_,max_]
};


/**
 * @ingroup statistics_classes
 * @brief Used to generate random numbers from a normal distribution.
 *
 * Class \link RWTRandNormal RWTRandNormal<Generator> \endlink generates
 * random numbers from a normal distribution:
 *
 * \f[
 * f(x) = \frac{1}{\sigma \sqrt{2 \pi}}\text{exp}
 * \begin{bmatrix}
 * -\frac{(x - \mu )^2)}{2 \sigma ^2}
 * \end{bmatrix}
 * , - \infty < \text{x} < \infty
 * \f]
 *
 * where \f$ \mu\f$ is the mean, and \f$ \sigma ^2\f$
 * is the variance. The algorithm used to generate the random numbers
 * begins with random numbers generated from a uniform distribution
 * on the interval [0, 1]. Thus, the speed and statistical properties
 * of the normal random numbers may be varied by varying the Generator
 * template parameter, which is responsible for generating these
 * uniformly distributed numbers.
 *
 * The class Generator must be a function object whose function
 * call operator returns a uniform random double between 0 and 1.
 * The class RWRandGenerator supplied with the library may be used.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rand.h>
 * RWTRandNormal<Generator> gen;
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/rand.h>
 * #include <iostream.h>
 * #include "myrand.h"
 *
 * int main() {
 * // Create a uniform generator on [0, 1] with an
 * // initial seed value.
 *   RWRandGenerator uniform(123456L);
 *
 * // Create a normal generator with mean 75 and variance 100
 * // using the above uniform generator for generator.
 *   RWTRandNormal<RWRandGenerator> normal(uniform, 75.0, 100.0);
 *
 * // Create a normal generator with mean 0 and variance 1
 * // using class RWRandGenerator.
 *   RWTRandNormal<RWRandGenerator> normalZeroOne;
 *
 * // Create a normal generator with mean -3.14 and variance
 * // 345.67 using a class MyDoubleRand to generate the
 * // uniform [0, 1] deviates.
 *   RWTRandNormal<MyDoubleRand> myRandNormal( -3.14, 345.67 );
 *
 * // Print a few values.
 *   for ( int j = 0; j < 10; j++ )
 *   {
 *     cout << "normal(75,100) = " << normal();
 *     cout << "\normal(0,1) = " << normalZeroOne();
 *     cout << "normal(-3.14,345.67) = " << myRandNormal() << endl;
 * }
 *
 * // Restart one of the generators with another seed value
 * // using the RWRandGenerator method restart().
 *   ( normalZeroOne.generator() ).restart( 654321L);
 *
 *   return 0;
 * }
 * @endcode
 *
 */
template< class Generator >
class RWTRandNormal : public RWTRand<Generator>
{
private:
    double mean_, variance_;
    double stdDeviation_;  // used in calculations
    double randNum_;
    int haveOne_;

    double _doRandom();
    size_t _doRandomArray(double*, size_t);


public:
    /**
     * Constructs a generator with mean 0 and variance 1.
     */
    RWTRandNormal();

    /**
     * Constructs a generator with the \a mean specified by the first parameter
     * and \a variance specified by the second parameter.
     */
    RWTRandNormal(double mean, double variance);

    /**
     * Constructs self as a copy of \a g.
     */
    RWTRandNormal(const RWTRandNormal<Generator>& g);

    /**
     * Constructs a generator with underlying uniform [0, 1] generator
     * \a g and the specified \a mean and \a variance.
     */
    RWTRandNormal(const Generator& g, double mean = 0, double variance = 1);

    /**
     * Returns the next random number in the sequence.
     */
    virtual double operator()();

    /**
     * Assigns random numbers to \a n elements in \a out. Returns the
     * number of random numbers generated.
     */
    virtual size_t operator()(double* out, size_t n);

    double lowBound() const;   // lower boundary of > 99% of distribution
    double highBound() const;  // lower boundary of > 99% of distribution

    /**
     * Sets the mean of the distribution to \a m.
     */
    void   setMean(double m) {
        mean_ = m;
    }

    /**
     * Sets the variance of the distribution to \a v.
     */
    void   setVariance(double v);

    /**
     * Returns the mean of the distribution.
     */
    double mean() const {
        return mean_;
    }

    /**
     * Returns the variance of the distribution.
     */
    double variance() const {
        return variance_;
    }
};


/**
 * @ingroup statistics_classes
 * @brief Used to generate random numbers from an exponential distribution.
 *
 * Class \link RWTRandExponential RWTRandExponential<Generator>
 * \endlink generates random numbers from an exponential distribution:
 *
 * \f[
 * g(w) =
 * \left\{\begin{matrix}
 *  \lambda e^{-\lambda \omega } , & 0 < w \\
 *  0 \hfill, & w \leq 0
 * \end{matrix}\right.
 * \f]
 *
 * The algorithm used to generate the random numbers begins with
 * random numbers generated from a uniform distribution on the interval
 * [0, 1]. Thus, the speed and statistical properties of the exponentially
 * distributed random numbers may be changed by varying the Generator
 * template parameter responsible for generating these uniformly
 * distributed numbers.
 *
 * The class Generator must be a function object whose function
 * call operator returns a uniform random double between 0 and 1.
 * The class RWRandGenerator supplied with the library may be used.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rand.h>
 * RWTRandExponential<Generator> gen;
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/rand.h>
 * #include <iostream.h>
 * #include "myrand.h"
 *
 * int main() {
 * // Create a uniform generator on [0, 1] with an
 * // initial seed value.
 *   RWRandGenerator uniform(123456L);
 *
 * // Create an exponential generator with lambda = 4.5
 * // using the above uniform generator.
 *   RWTRandExponential<RWRandGenerator> exp(uniform, 4.5);
 *
 * // Create an exponential generator with lambda = 1 using class
 * // RWRandGenerator.
 *   RWTRandExponential<RWRandGenerator> expOne;
 *
 * // Create an exponential generator with lambda 3.14 using
 * // a class MyDoubleRand to generate the uniform [0, 1] deviates.
 *   RWTRandExponential<MyDoubleRand> myExp(3.14);
 *
 * // Print a few values.
 *   for ( int j = 0; j < 10; j++ )
 *   {
 *     cout << "exponential lambda = 4.5 = " << exp();
 *     cout << "\nexponential lambda = 1 = " << expOne();
 *     cout << "\nexponential lambda = 3.14 = " << myExp() << endl;
 * }
 *
 * // Restart one of the generators with another seed value
 * // using the RWRandGenerator method restart().
 *   ( expOne.generator() ).restart( 654321L);
 *
 *   return 0;
 * }
 * @endcode
 *
 */
template< class Generator >
class RWTRandExponential : public RWTRand<Generator>
{
private:
    double lambda_;   // 1/lambda is mean time between events

    double _doRandom();
    size_t _doRandomArray(double*, size_t);


public:
    /**
     * Constructs a generator with lambda = \a lam.
     */
    RWTRandExponential(double lam = 1);

    /**
     * Constructs self as a copy of \a g.
     */
    RWTRandExponential(const RWTRandExponential<Generator>& g);

    /**
     * Constructs a generator with underlying uniform [0, 1] generator
     * \a g lambda=\a lam.
     */
    RWTRandExponential(const Generator& g, double lam = 1);

    /**
     * Returns the next random number in the sequence.
     */
    virtual double operator()();

    /**
     * Assigns random numbers to \a n elements in \a out. Returns the
     * number of random numbers generated.
     */
    virtual size_t operator()(double* out, size_t n);

    /**
     * Returns the lower bound of 99% of the distribution.
     */
    double lowBound() const;

    /**
     * Returns the upper bound of 99% of the distribution.
     */
    double highBound() const;

    /**
     * Returns the value of lambda for the distribution.
     */
    double lambda() const {
        return lambda_;
    }

    /**
     * Sets the value of \c lambda for the distribution to \a l.
     */
    void   setLambda(double l) {
        lambda_ = l;
    }
};


/**
 * @ingroup statistics_classes
 * @brief Used to generate random numbers from a gamma distribution.
 *
 * Class \link RWTRandGamma RWTRandGamma<Generator> \endlink generates
 * random numbers from a gamma distribution:
 *
 * \f[
 * f(x) = \frac{x^{a-1}e^{-x}}{\Gamma (a)}, x > 0
 * \f]
 *
 * where \c a is the order. The algorithm used to generate the random
 * numbers begins with random numbers generated from a uniform distribution
 * on the interval [0, 1]. Thus, the speed and statistical properties
 * of the gamma random number generator may be varied by varying
 * the Generator template parameter, which is responsible for generating
 * the uniformly distributed numbers.
 *
 * The class Generator must be a function object whose function
 * call operator returns a uniform random double between 0 and 1.
 * The class RWRandGenerator supplied with the library may be used.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rand.h>
 * RWTRandGamma<Generator> gen;
 * @endcode
 * @section example Example
 *
 * @code
 * #include <rw/rand.h>
 * #include <iostream.h>
 * #include "myrand.h"
 *
 * int main() {
 * // Create a uniform generator on [0, 1] with an
 * // initial seed value.
 *   RWRandGenerator uniform(123456L);
 *
 * // Create a gamma generator with order = 4
 * // using the above uniform generator for generator.
 *   RWTRandGamma<RWRandGenerator> gamma(uniform, 4);
 *
 * // Create an exponential generator with order = 1
 * // using class RWRandGenerator.
 *   RWTRandGamma<RWRandGenerator> gammaOne;
 *
 * // Create a gamma generator with order 31 using a class
 * // MyDoubleRand to generate the uniform [0, 1] deviates.
 *   RWTRandGamma<MyDoubleRand> myGamma(31);
 *
 * // Print a few values.
 *   for ( int j = 0; j < 10; j++ )
 *   {
 *     cout << "gamma order = 4 = " << gamma();
 *     cout << "\ngamma order = 1 = " << gammaOne();
 *     cout << "\ngamma order = 31 = " << myGamma() << endl;
 * }
 *
 * // Restart one of the generators with another seed value
 * // using the RWRandGenerator method restart().
 *   ( gammaOne.generator() ).restart( 654321L);
 *
 *   return 0;
 * }
 * @endcode
 *
 */
template< class Generator >
class RWTRandGamma : public RWTRand<Generator>
{
private:
    int order_;          // order is number of events which occur

    double _doRandom();
    size_t _doRandomArray(double*, size_t);

public:
    /**
     * Constructs a generator with order = \a ord.
     */
    RWTRandGamma(int ord = 1);

    /**
     * Constructs self as a copy of \a g.
     */
    RWTRandGamma(const RWTRandGamma<Generator>& g);

    /**
     * Constructs a generator with underlying uniform [0, 1] generator
     * \a g  order = \c ord.
     */
    RWTRandGamma(const Generator& g, int ord = 1);

    /**
     * Returns the next random number in the sequence.
     */
    virtual double operator()();

    /**
     * Assigns random numbers to \a n elements in \a out. Returns the
     * number of random numbers generated.
     */
    virtual size_t operator()(double* out, size_t n);

    /**
     * Sets the value of lambda for the distribution to \a l.
     */
    void setOrder(int order) {
        order_ = order;
    }

    /**
     * Returns the value of order for the distribution.
     */
    int  order() const {
        return order_;
    }
};


/**
 * @ingroup statistics_classes
 * @brief Used to generate random numbers from a Poisson distribution.
 *
 * Class \link RWTRandPoisson RWTRandPoisson<Generator> \endlink
 * generates random numbers from a Poisson distribution:
 *
 * \f[
 * f(x) = \left\{\begin{matrix}
 *  \frac{\mu^xe^{- \mu}}{x!} & \text{x = 0, 1, 2, ...} \\
 *  \text{0} & \text{otherwise}
 * \end{matrix}\right.
 * \f]
 *
 * where \f$ \mu\f$ is the mean. The algorithm used to generate the
 * random numbers begins with random numbers generated from a uniform
 * distribution on the interval [0, 1]. Thus, the speed and statistical
 * properties of the Poisson random number generator may be varied
 * by varying the Generator template parameter, which is responsible
 * for generating the uniformly distributed numbers.
 *
 * The class Generator must be a function object whose function
 * call operator returns a uniform random double between 0 and 1.
 * The class RWRandGenerator supplied with the library may be used.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rand.h>
 * RWTRandPoisson<Generator> gen;
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/rand.h>
 * #include <iostream.h>
 * #include "myrand.h"
 *
 * int main() {
 * // Create a uniform generator on [0, 1] with an
 * // initial seed value.
 *   RWRandGenerator uniform(123456L);
 *
 * // Create a Poisson generator with mean = 4
 * // using the above uniform generator for generator.
 *   RWTRandPoisson<RWRandGenerator> poisson(uniform, 4);
 *
 * // Create a Poisson generator with mean = 1 using class
 * // RWRandGenerator.
 *   RWTRandPoisson<RWRandGenerator> poissonOne;
 *
 * // Create a poisson generator with mean 31 using a class
 * // MyDoubleRand to generate the uniform [0, 1] deviates.
 *   RWTRandPoisson<MyDoubleRand> myPoisson(31);
 *
 * // Print a few values
 *   for ( int j = 0; j < 10; j++ )
 *   {
 *     cout << "poisson mean = 4 = " << gamma();
 *     cout << "\npoisson mean = 1 = " << gammaOne();
 *     cout << "\npoisson mean = 31 = " << myGamma() << endl;
 * }
 *
 * // Restart one of the generators with another seed value
 * // using the RWRandGenerator method restart().
 *   ( poissonOne.generator() ).restart( 654321L);
 *
 *   return 0;
 * }
 * @endcode
 *
 */
template< class Generator >
class RWTRandPoisson : public RWTRand<Generator>
{
private:
    double mean_;           // mean of the distribution
    double g_, lmean_, sq_;  // Private data used by the algorithm
    void initPrivateData();

    double _doRandom();
    size_t _doRandomArray(double*, size_t);


public:
    /**
     * Constructs a generator with mean = \a m.
     */
    RWTRandPoisson(double m = 1);

    /**
     * Constructs self as a copy of \a g.
     */
    RWTRandPoisson(const RWTRandPoisson<Generator>& g);

    /**
     * Constructs a generator with underlying uniform [0, 1] generator
     * \a g and mean \a m.
     */
    RWTRandPoisson(const Generator& g, double m = 1);

    /**
     * Returns the next random number in the sequence.
     */
    virtual double operator()();

    /**
     * Assigns random numbers to \a n elements in \a out. Returns the
     * number of random numbers generated.
     */
    virtual size_t operator()(double* out, size_t n);

    /**
     * Returns the value of mean for the distribution.
     */
    double mean() const {
        return mean_;
    }

    /**
     * Sets the value of the mean of the distribution to \a m.
     */
    void   setMean(double m);
};


/**
 * @ingroup statistics_classes
 * @brief Used to generate random numbers from a binomial distribution.
 *
 * Class \link RWTRandBinomial RWTRandBinomial<Generator> \endlink
 * generates random numbers from a binomial distribution:
 *
 * \f[
 * \begin{array}{l l l}
 *  f(x) & = \binom{n}{x} p^x(1-p)^{n-x}, & \text{x = 0, 1, ..., n} \\
 *       & = \text{0} \hfill,              & \text{elsewhere}
 * \end{array}
 * \f]
 *
 * where \c n is the number of trials, and \c p is the probability
 * of success. The algorithm used to generate the random numbers
 * begins with random numbers generated from a uniform distribution
 * on the interval [0, 1]. Thus, the speed and statistical properties
 * of the binomial random number generator may be changed by varying
 * the Generator template parameter responsible for generating the
 * uniformly distributed numbers.
 *
 * The class Generator must be a function object whose function
 * call operator returns a uniform random double between 0 and 1.
 * The class RWRandGenerator supplied with the library may be used.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rand.h>
 * RWTRandBinomial<Generator> gen;
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/rand.h>
 * #include <iostream.h>
 * #include "myrand.h"
 *
 * int main() {
 * // Create a uniform generator on [0, 1] with an
 * // initial seed value.
 *   RWRandGenerator uniform(123456L);
 *
 * // Create a binomial generator with number of trials 25 and
 * // probability of success = 0.34 using the above uniform
 * // generator.
 * RWTRandBinomial<RWRandGenerator> binomial(uniform, 25, 0.34);
 *
 * // Create a binomial generator with number of trials = 1 and
 * // probability of success = .5 using class RWRandGenerator.
 *   RWTRandBinomial<RWRandGenerator> binomialOne;
 *
 * // Create a binomial generator with number of trials = 31
 * // and probability of success = .27 using a class
 * // MyDoubleRand to generate the uniform [0, 1] deviates.
 *   RWTRandBinomial<MyDoubleRand> myBinomial(31, .27);
 *
 * // Print a few values
 *   for ( int j = 0; j < 10; t+j )
 *   {
 *     cout << "binomial n=25, p=.34 = " << binomial();
 *     cout << "\nbinomial n=1, p=0.5 = " << binomialOne();
 *     cout << "\nbinomial n=31 and p=.27 = "
 *          << myBinomial() << endl;
 * }
 *
 * // Restart one of the generators with another seed value
 * // using the RWRandGenerator method restart()
 *   ( binomialOne.generator() ).restart( 654321L);
 *
 *   return 0;
 * }
 * @endcode
 *
 */
template< class Generator >
class RWTRandBinomial : public RWTRand<Generator>
{
private:
    size_t n_; // number of trials
    double p_;   // probability of success

    // Private data used in the algorithms.
    double prb_, g_, am_, pc_, plog_, pclog_, sq_;

    void initData(); // Initializes the above data.
    double _doRandom();
    size_t _doRandomArray(double*, size_t);


public:
    /**
     * Constructs a generator with the number of trials = \a n and the
     * probability of success = \a p.
     */
    RWTRandBinomial(size_t n = 1, double p = 0.5);

    /**
     * Constructs self as a copy of \a g.
     */
    RWTRandBinomial(const RWTRandBinomial<Generator>& g);

    /**
     * Constructs a generator with underlying uniform [0, 1] generator
     * \a g, number of trials = \a n, and probability of success = \a
     * p.
     */
    RWTRandBinomial(const Generator& g, size_t n = 1, double p = 0.5);

    /**
     * Returns the next random number in the sequence.
     */
    virtual double operator()();

    /**
     * Assigns random numbers to \a n elements in \a out. Returns the
     * number of random numbers generated.
     */
    virtual size_t operator()(double* out, size_t n);

    /**
     * Returns the number of trials for the distribution.
     */
    int            n() const {
        return static_cast<unsigned>(n_);
    }

    /**
     * Returns the probability of success for the distribution.
     */
    double         p() const {
        return p_;
    }

    /**
     * Sets the number of trials for the distribution to \a n.
     */
    void           setN(size_t n);

    /**
     * Sets the probability of success for the distribution to \a p.
     */
    void           setP(double p);
};


//**********************************************************************
//
// Typedefs for version 5.1 backward compatibility
//
//**********************************************************************
typedef RWTRand<RWRandGenerator>            RWRand;
typedef RWTRandUniform<RWRandGenerator>     RWRandUniform;
typedef RWTRandExponential<RWRandGenerator> RWRandExponential;
typedef RWTRandNormal<RWRandGenerator>      RWRandNormal;
typedef RWTRandBinomial<RWRandGenerator>    RWRandBinomial;
typedef RWTRandPoisson<RWRandGenerator>     RWRandPoisson;
typedef RWTRandGamma<RWRandGenerator>       RWRandGamma;

#ifdef RW_USING_MKL
#  if !defined(_MSC_VER) || (_MSC_VER > 1200)
//specializations to use the MKLs random number generator
template<>
double RW_MATH_SYMBOLIC RWTRandUniform<RWTBRNGenerator<RWBRNG::MCG31> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandUniform<RWTBRNGenerator<RWBRNG::MCG31> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandUniform<RWTBRNGenerator<RWBRNG::R250> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandUniform<RWTBRNGenerator<RWBRNG::R250> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandUniform<RWTBRNGenerator<RWBRNG::MRG32K3A> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandUniform<RWTBRNGenerator<RWBRNG::MRG32K3A> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandUniform<RWTBRNGenerator<RWBRNG::MCG59> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandUniform<RWTBRNGenerator<RWBRNG::MCG59> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandNormal<RWTBRNGenerator<RWBRNG::MCG31> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandNormal<RWTBRNGenerator<RWBRNG::MCG31> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandNormal<RWTBRNGenerator<RWBRNG::R250> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandNormal<RWTBRNGenerator<RWBRNG::R250> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandNormal<RWTBRNGenerator<RWBRNG::MRG32K3A> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandNormal<RWTBRNGenerator<RWBRNG::MRG32K3A> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandNormal<RWTBRNGenerator<RWBRNG::MCG59> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandNormal<RWTBRNGenerator<RWBRNG::MCG59> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandBinomial<RWTBRNGenerator<RWBRNG::MCG31> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandBinomial<RWTBRNGenerator<RWBRNG::MCG31> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandBinomial<RWTBRNGenerator<RWBRNG::R250> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandBinomial<RWTBRNGenerator<RWBRNG::R250> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandBinomial<RWTBRNGenerator<RWBRNG::MRG32K3A> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandBinomial<RWTBRNGenerator<RWBRNG::MRG32K3A> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandBinomial<RWTBRNGenerator<RWBRNG::MCG59> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandBinomial<RWTBRNGenerator<RWBRNG::MCG59> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandExponential<RWTBRNGenerator<RWBRNG::MCG31> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandExponential<RWTBRNGenerator<RWBRNG::MCG31> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandExponential<RWTBRNGenerator<RWBRNG::R250> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandExponential<RWTBRNGenerator<RWBRNG::R250> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandExponential<RWTBRNGenerator<RWBRNG::MRG32K3A> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandExponential<RWTBRNGenerator<RWBRNG::MRG32K3A> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandExponential<RWTBRNGenerator<RWBRNG::MCG59> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandExponential<RWTBRNGenerator<RWBRNG::MCG59> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandPoisson<RWTBRNGenerator<RWBRNG::MCG31> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandPoisson<RWTBRNGenerator<RWBRNG::MCG31> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandPoisson<RWTBRNGenerator<RWBRNG::R250> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandPoisson<RWTBRNGenerator<RWBRNG::R250> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandPoisson<RWTBRNGenerator<RWBRNG::MRG32K3A> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandPoisson<RWTBRNGenerator<RWBRNG::MRG32K3A> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandPoisson<RWTBRNGenerator<RWBRNG::MCG59> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandPoisson<RWTBRNGenerator<RWBRNG::MCG59> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandGamma<RWTBRNGenerator<RWBRNG::MCG31> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandGamma<RWTBRNGenerator<RWBRNG::MCG31> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandGamma<RWTBRNGenerator<RWBRNG::R250> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandGamma<RWTBRNGenerator<RWBRNG::R250> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandGamma<RWTBRNGenerator<RWBRNG::MRG32K3A> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandGamma<RWTBRNGenerator<RWBRNG::MRG32K3A> >::_doRandomArray(double* out, size_t n);
template<>
double RW_MATH_SYMBOLIC RWTRandGamma<RWTBRNGenerator<RWBRNG::MCG59> >::_doRandom();
template<>
size_t RW_MATH_SYMBOLIC RWTRandGamma<RWTBRNGenerator<RWBRNG::MCG59> >::_doRandomArray(double* out, size_t n);
#  endif
#endif
#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/rand.cc>
#endif

#endif // __RWRAND_H__
