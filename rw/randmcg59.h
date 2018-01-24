#ifndef __RWRANDMCG59_H__
#define __RWRANDMCG59_H__

/***************************************************************************
 *
 * randmcg59.h - A 59-bit multiplicative congruential generator.
 *
 ***************************************************************************
 *
 * $Id: //math/13/rw/randmcg59.h#1 $
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
 **************************************************************************/

#include <rw/mathdefs.h>

#include <rw/randmlcg.h>

// Force doxygen into recognizing the class RWRandGenBasicMCG59 is called
// RWRandGenMCG59.  This is done to force the documented API to match
// what was done pre doxygen.
#if defined(DOXYGEN)
#  define RWRandGenBasicMCG59 RWRandGenMCG59
#endif

/**
 * @ingroup statistics_classes
 * @brief Generates random numbers uniformly distributed
 * over the interval <tt>[0, 1]</tt>.
 *
 * Class RWRandGenMCG59  generates random numbers uniformly distributed
 * over the interval <tt>[0, 1]</tt>, and may be used instead of class
 * RWRandGenerator as a template parameter to any of the distribution
 * generators derived from abstract base class \link RWTRand RWTRand<>
 * \endlink.  RWRandGenMCG59 is a 59-bit multiplicative congruential
 * generator.
 *
 * \par
 *
 *   \f$ x_{n} = ax_{n-1}(mod (m)) \f$ \n
 *   \f$ u_{n} = x_{n}/m \f$ \n
 *   \f$ a = 13^{13}, m = 2^{59} \f$ \n
 *
 * Unlike class RWRandGenerator, each instance of RWRandGenMCG59
 * contains and maintains its own, independent state. This can make
 * it easier to use in some situations. The "internal state" of
 * an instance of RWRandGenMCG59 consists of only a seed.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/randmcg59.h>
 * RWRandGenMCG59 gen;
 * @endcode
 */
class RW_MATH_GLOBAL RWRandGenBasicMCG59 : public RWRandGenBasicMLC
{
public:
    /**
     * Constructs a generator, arbitrarily seeded by a call to the \b time()
     * function.
     */
    RWRandGenBasicMCG59() {}

    /**
     * Constructs a generator with a specified \a seed.
     */
    RWRandGenBasicMCG59(unsigned long seed): RWRandGenBasicMLC(seed) {}

    /**
     * Constructs a copy of \a u.  The copy's state will equal the state of \a u
     * at the time the copy is created, but will not share the state of \a u.
     */
    RWRandGenBasicMCG59(const RWRandGenBasicMCG59& u): RWRandGenBasicMLC(u) {}

    /**
     * Returns a random double in <tt>[0,1]</tt>.
     */
    double operator()();
};


#ifndef RW_USING_MKL
typedef RWRandGenBasicMCG59 RWRandGenMCG59;
#else
#  include <rw/rwtbrngen.h>
typedef RWTBRNGenerator<RWBRNG::MCG59> RWRandGenMCG59;
#endif // RW_USING_MKL

#endif  // __RWRANDMCG59_H__
