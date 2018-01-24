#ifndef __RWRANDMTWIST_H__
#define __RWRANDMTWIST_H__

/***************************************************************************
 *
 * randmtwist.h - A uniform [0,1] random number generator class
 *                based upon Makoto Matsumoto and Takuji Nishimura's
 *                Mersenne Twister algorithm.
 *
 ***************************************************************************
 *
 * The Mersenne Twister algorithm produces a uniform distribution of
 * pseudo random numbers on the range [0,1], with a period of 2^19937-1.
 *
 * It is NOT a cryptographically secure generator.
 *
 * References:
 *
 * 1. Mersenne Twister Home Page:
 *    http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
 *
 * 2. M. Matsumoto and T. Nishimura,
 *    "Mersenne Twister: A 623-Dimensionally Equidistributed Uniform
 *    Pseudo-Random Number Generator",
 *    ACM Transactions on Modeling and Computer Simulation,
 *    Vol. 8, No. 1, January 1998, pp 3--30.
 *
 ***************************************************************************
 *
 * Acknowledgment:
 *
 * Sections of code preceded by a "// ---------- Mersenne Twister ..."
 * comment are direct or almost-direct copies of code available in
 * file "mt19937ar.c" at
 * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/CODES/mt19937ar.c
 * According to that webpage, the code is copyrighted but freely usable
 * ("BSD-license") for commercial purposes.
 *
 * Here is the copyright text from the mt19937ar.c source code:
 * ----------
 * Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. The names of its contributors may not be used to endorse or promote
 *      products derived from this software without specific prior written
 *      permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Any feedback is very welcome.
 * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
 * email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)

 * ----------
 * In this file, the above copyright applies for the following routines:
 *     void init_genrand(unsigned long s);
 *     unsigned long genrand_int32();
 *     double genrand_real1();
 *
 ***************************************************************************
 *
 * $Id: //math/13/rw/randmtwist.h#1 $
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

// Force doxygen into recognizing the class RWRandGenBasicMTwist is called
// RWRandGenMTwist.  This is done to force the documented API to match
// what was done pre doxygen.
#if defined(DOXYGEN)
#  define RWRandGenBasicMTwist RWRandGenMTwist
#endif

/**
 * @ingroup statistics_classes
 * @brief Generates random numbers uniformly distributed
 * over the interval <tt>[0, 1]</tt>.
 *
 * Class RWRandGenMTwist generates random numbers uniformly distributed
 * over the interval <tt>[0, 1]</tt>, and may be used instead of class
 * RWRandGenerator as a template parameter to any of the distribution
 * generators derived from abstract base class \link RWTRand RWTRand<>
 * \endlink. RWRandGenMTwist uses Takuji Nishimura and Makoto Matsumoto's
 * Mersenne Twister algorithm.  It is relatively fast, and has a
 * large period (2^19937-1).
 *
 * Unlike class RWRandGenerator, each instance of RWRandGenMTwist
 * contains and maintains its own, independent state.  This can
 * make it easier to use in some situations.  The "internal state"
 * of an instance of RWRandGenMTwist consists of a table of an unsigned
 * long seed, an array of unsigned longs known as the "state vector"
 * and an integer index into that state vector.
 *
 * Additional information about the Mersenne Twister algorithm is available
 * in the original article: M. Matsumoto and T. Nishimura,"Mersenne
 * Twister: A 623-dimensionally equidistributed uniform pseudorandom
 * number generator", ACM Trans. on Modeling and Computer Simulation
 * Vol. 8, No. 1, January pp.3-30 1998. There are also numerous resources
 * on the internet, including a website dedicated to the Mersenne Twister.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/randmtwist.h>
 * RWRandGenMTwist gen;
 * @endcode
 */
class RW_MATH_SYMBOLIC RWRandGenBasicMTwist
{
private:

    // ---------- Mersenne Twister Period parameters ----------

    // These are the absolute constants within which this implementation
    // of Mersenne Twister operates.  They were C-preprocessor macros in
    // the original mt19937ar.c source file.

    // For efficiency these are no longer static members, but now,
    // constants in randmtwist.cpp

    // static const unsigned long  N;
    // static const unsigned long  M;
    // static const unsigned long  MATRIX_A;    // constant vector a
    // static const unsigned long  UPPER_MASK;  // most significant w-r bits
    // static const unsigned long  LOWER_MASK;  // significant r bits


    // ---------- Generator state ----------

    // The generator's state consists of seed, table and table index

    unsigned long seed_;

    unsigned long mt[624];  // the array for the state vector (624 == N)
    int mti;                // mti == N+1 indicates mt[N] is not initialized


    // ---------- Mersenne Twister functions ----------

    //  Initialize mt[N] with a seed.
    void init_genrand(unsigned long s);

    //  Generate a random number on [0,0xffffffff]-interval
    unsigned long genrand_int32();

    //  Generate a random number on [0,1]-real-interval
    double genrand_real1();

public:
    /**
     * Constructs a generator, arbitrarily seeded by a call to the \b time()
     * function.
     */
    RWRandGenBasicMTwist();

    /**
     * Constructs a generator with a specified \a seed.
     */
    RWRandGenBasicMTwist(unsigned long seed);

    /**
     * Constructs a copy of \a u.  The copy's state will equal the state of \a u
     * at the time the copy is created, but will not share the state of \a u.
     */
    RWRandGenBasicMTwist(const RWRandGenBasicMTwist& u);

    /**
     * Returns a random double in <tt>[0,1]</tt>.
     */
    double operator()();               // returns a random double in [0,1]

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
     * Sets an instance of RWRandGenMTwist equal to the state of instance
     * \a u.
     */
    RWRandGenBasicMTwist& operator=(const RWRandGenBasicMTwist& u);

    /**
     * Returns \c true for two distinct instances of RWRandGenMTwist
     * if they will return identical sequences of values.
     */
    bool operator==(const RWRandGenBasicMTwist& u) const;

    /**
     * Returns \c true for two distinct instances of RWRandGenMTwist
     * if their internal states differ. \"<tt>(x != x)</tt>\" returns \c false.
     */
    bool operator!=(const RWRandGenBasicMTwist& u) const;

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
 * @relates RWRandGenMTwist
 *
 * Outputs a summary of \a gen's internal state on ostream \a s.
 */
RW_MATH_SYMBOLIC std::ostream&
operator <<(std::ostream& s, const RWRandGenBasicMTwist& gen);

/**
 * @relates RWRandGenMTwist
 *
 * Inputs an internal state from istream \a s to \a gen.
 */
RW_MATH_SYMBOLIC std::istream&
operator >>(std::istream& s, RWRandGenBasicMTwist& gen);

#ifndef RW_USING_MKL
typedef RWRandGenBasicMTwist RWRandGenMTwist;
#else
#  include <rw/rwtbrngen.h>
typedef RWTBRNGenerator<RWBRNG::MT19937> RWRandGenMTwist;
#endif // RW_USING_MKL

#endif  // __RWRANDMTWIST_H__
