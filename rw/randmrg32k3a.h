#ifndef __RWRANDMRG32K3A_H__
#define __RWRANDMRG32K3A_H__

/***************************************************************************
 *
 * randmrg32k3a.h - A combined multiple recursive generator with two components
 * of order 3.  This file is an implementation of Pierre L'Ecuyer's MRG32k3a
 * pseudo random number generator.
 *
 ***************************************************************************
 *
 * $Id: //math/13/rw/randmrg32k3a.h#1 $
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
#include <rw/rwfile.h>
#include <rw/vstream.h>

// Force doxygen into recognizing the class RWRandGenBasicMRG32K3A is called
// RWRandGenMRG32K3A.  This is done to force the documented API to match
// what was done pre doxygen.
#if defined(DOXYGEN)
#  define RWRandGenBasicMRG32K3A RWRandGenMRG32K3A
#endif

/**
 * @ingroup statistics_classes
 * @brief Generates random numbers uniformly distributed
 * over the interval <tt>[0, 1]</tt>.
 *
 * Class RWRandGenMRG32K3A generates random numbers uniformly distributed
 * over the interval <tt>[0, 1]</tt>, and may be used instead of class
 * RWRandGenerator as a template parameter to any of the distribution
 * generators derived from abstract base class \link RWTRand RWTRand<>
 * \endlink. RWRandGenMRG32K3A  is a combined multiple recursive
 * generator with two components of order 3.
 *
 * \par
 *
 *  \f$ x_{n} = a_{11}x_{n-1} + a_{12}x_{n-2} + a_{13}x_{n-3}( mod (m_{1}) ) \f$ \n
 *  \f$ y_{n} = a_{21}y_{n-1} + a_{22}y_{n-2} + a_{23}y_{n-3}( mod (m_{2}) ) \f$ \n
 *  \f$ z_{n} = x_{n} - y_{n}(mod (m_{1})) \f$ \n
 *  \f$ u_{n} = z_{n}/m_{1} \f$ \n
 *  \f$ a_{11}= 0, a_{12}= 1403580, a_{13} = -810728, m_{1} = 2^{32}- 209 \f$ \n
 *
 * Unlike class RWRandGenerator, each instance of RWRandGenMRG32K3A
 * contains and maintains its own, independent state. This can make
 * it easier to use in some situations. The "internal state" of
 * an instance of RWRandGenMRG32K3A  consists of  a seed and a series
 * of six integers.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/randmrg32k3a.h>
 * RWRandGenMRG32K3A gen;
 * @endcode
 */
class RW_MATH_SYMBOLIC RWRandGenBasicMRG32K3A
{
private:
    /* Constants defined in randmrg32k3a.cpp
      static const long int m1;
      static const long int m2;
      static const long int a2;
      static const long int qa2;
      static const long int ra2;
      static const long int a3;
      static const long int qa3;
      static const long int ra3;
      static const long int b1;
      static const long int qb1;
      static const long int rb1;
      static const long int b3;
      static const long int qb3;
      static const long int rb3;
    */

    // ---------- Generator state ----------

    struct cmrg_state_t {
        long int x1, x2, x3;        /* first component */
        long int y1, y2, y3;        /* second component */
    };

    cmrg_state_t state_;
    unsigned long seed_;

    unsigned long int cmrg_get();
    double cmrg_get_double();
    void cmrg_set(unsigned long int s);
    unsigned long LCG(unsigned long n) {
        return ((69069 * n) & 0xffffffffUL);
    }

public:

    /**
     * Constructs a generator, arbitrarily seeded by a call to the \b time()
     * function.
     */
    RWRandGenBasicMRG32K3A();

    /**
     * Constructs a generator with a specified \a seed.
     */
    RWRandGenBasicMRG32K3A(unsigned long seed);

    /**
     * Constructs a copy of \a u.  The copy's state will equal the state of \a u
     * at the time the copy is created, but will not share the state of \a u.
     */
    RWRandGenBasicMRG32K3A(const RWRandGenBasicMRG32K3A& u);

    /**
     * Returns a random double in <tt>[0,1]</tt>.
     */
    double operator()();

    /**
     * Returns the seed used to initialize the generator.
     */
    unsigned long seed() const;

    /**
     * Restarts the generator, arbitrarily seeding it by a call to the
     * \b time() function.
     */
    void restart();

    /**
     * Restarts the generator with specified \a seed.
     */
    void restart(unsigned long seed);

    /**
     * Sets an instance of RWRandGenMRG32K3A equal to the state of instance
     * \a u.
     */
    RWRandGenBasicMRG32K3A& operator=(const RWRandGenBasicMRG32K3A& u);

    /**
     * Returns \c true for two distinct instances of RWRandGenMRG32K3A
     * if they will return identical sequences of values.
     */
    bool operator==(const RWRandGenBasicMRG32K3A& u) const;

    /**
     * Returns \c true for two distinct instances of RWRandGenMRG32K3A
     * if their internal states differ. \"<tt>(x != x)</tt>\" returns \c false.
     */
    bool operator!=(const RWRandGenBasicMRG32K3A& u) const;

    /**
     * Outputs a summary of self's internal state on ostream \a s.
     */
    void printOn(std::ostream& s) const;  // outstrm table, seed, index

    /**
     * Inputs an internal state from istream \a s to self.
     */
    void scanFrom(std::istream& s);       // instrm table, seed, index

    /**
     * Restores internal state of self from an RWFile.
     */
    void restoreFrom(RWFile&);       // Restore from binary

    /**
     * Restores internal state of self from a virtual stream.
     */
    void restoreFrom(RWvistream&);   // Restore from virtual stream

    /**
     * Stores internal state of self in a binary format to an RWFile.
     */
    void saveOn(RWFile&) const;      // Store using binary

    /**
     * Stores internal state of self to a virtual stream.
     */
    void saveOn(RWvostream&) const;  // Store to virtual stream

};

/**
 * @relates RWRandGenMRG32K3A
 *
 * Outputs a summary of \a gen's internal state on ostream \a s.
 */
RW_MATH_SYMBOLIC std::ostream&
operator <<(std::ostream& s, const RWRandGenBasicMRG32K3A& gen);

/**
 * @relates RWRandGenMRG32K3A
 *
 * Inputs an internal state from istream \a s to \a gen.
 */
RW_MATH_SYMBOLIC std::istream&
operator >>(std::istream& s, RWRandGenBasicMRG32K3A& gen);

#ifndef RW_USING_MKL
typedef RWRandGenBasicMRG32K3A RWRandGenMRG32K3A;
#else
#  include <rw/rwtbrngen.h>

typedef RWTBRNGenerator<RWBRNG::MRG32K3A> RWRandGenMRG32K3A;
#endif // RW_USING_MKL

#endif  // __RWRANDMRG32K3A_H__
