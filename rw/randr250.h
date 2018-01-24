#ifndef __RWRANDR250_H__
#define __RWRANDR250_H__

/***************************************************************************
 *
 * randr250.h - A generalized feedback shift register generator.
 *
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * Acknowledgment:  implements R250 random number generator, from S.
 * Kirkpatrick and E.  Stoll, Journal of Computational Physics, 40, p.
 * 517 (1981).
 *
 *
 *
 ***************************************************************************
 *
 * $Id: //math/13/rw/randr250.h#1 $
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

// Force doxygen into recognizing the class RWRandGenBasicR250 is called
// RWRandGenR250.  This is done to force the documented API to match
// what was done pre doxygen.
#if defined(DOXYGEN)
#  define RWRandGenBasicR250 RWRandGenR250
#endif

/**
 * @ingroup statistics_classes
 * @brief Generates random numbers uniformly distributed
 * over the interval <tt>[0, 1]</tt>.
 *
 * Class RWRandGenR250 generates random numbers uniformly distributed
 * over the interval <tt>[0, 1]</tt>, and may be used instead of class
 * RWRandGenerator as a template parameter to any of the distribution
 * generators derived from abstract base class \link RWTRand RWTRand<>
 * \endlink. RWRandGenR250 is a generalized feedback shift register
 * generator.
 *
 * Unlike class RWRandGenerator, each instance of RWRandGenR250
 * contains and maintains its own, independent state.  This can
 * make it easier to use in some situations.  The "internal state"
 * of an instance of RWRandGenR250 consists of a seed, an array
 * of unsigned longs known as the "state vector" and an integer
 * index into that state vector. (S.  Kirkpatrick and E.  Stoll,
 * Journal of Computational Physics, 40, p. 517 (1981).
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/randr250.h>
 * RWRandGenR250 gen;
 * @endcode
 */
class RW_MATH_GLOBAL RWRandGenBasicR250
{
private:

    static const unsigned long  N;

    // ---------- Generator state ----------

    // The generator's state consists of seed, table and table index

    struct state_t {
        unsigned long x[250];  // the array for the state vector
        int i;
    };

    unsigned long seed_;
    state_t state_;

    // ---------- Mersenne Twister functions ----------

    unsigned long int r250_get();

    double r250_get_double();

    void r250_set(unsigned long);

public:
    /**
     * Constructs a generator, arbitrarily seeded by a call to the \b time()
     * function.
     */
    RWRandGenBasicR250();

    /**
     * Constructs a generator with a specified \a seed.
     */
    RWRandGenBasicR250(unsigned long seed);

    /**
     * Constructs a copy of \a u.  The copy's state will equal the state of \a u
     * at the time the copy is created, but will not share the state of \a u.
     */
    RWRandGenBasicR250(const RWRandGenBasicR250& u);

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
     * Sets an instance of RWRandGenR250 equal to the state of instance
     * \a u.
     */
    RWRandGenBasicR250& operator=(const RWRandGenBasicR250& u);

    /**
     * Returns \c true for two distinct instances of RWRandGenR250 if
     * they will return identical sequences of values.
     */
    bool operator==(const RWRandGenBasicR250& u) const;

    /**
     * Returns \c true for two distinct instances of RWRandGenR250 if
     * their internal states differ. \"<tt>(x != x)</tt>\" returns \c false.
     */
    bool operator!=(const RWRandGenBasicR250& u) const;

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
 * @relates RWRandGenR250
 *
 * Outputs a summary of \a gen's internal state on ostream \a s.
 */
RW_MATH_SYMBOLIC std::ostream&
operator <<(std::ostream& s, const RWRandGenBasicR250& gen);

/**
 * @relates RWRandGenR250
 *
 * Inputs an internal state from istream \a s to \a gen.
 */
RW_MATH_SYMBOLIC std::istream&
operator >>(std::istream& s, RWRandGenBasicR250& gen);

#ifndef RW_USING_MKL
typedef RWRandGenBasicR250 RWRandGenR250;
#else
#  include <rw/rwtbrngen.h>
typedef RWTBRNGenerator<RWBRNG::R250> RWRandGenR250;
#endif

#endif  // __RWRANDR250_H__
