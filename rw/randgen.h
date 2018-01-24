#ifndef __RWRANDGEN_H__
#define __RWRANDGEN_H__


/***************************************************************************
 *
 * randgen.h - Declarations for RWRandGenerator Class.
 *
 * $Id: //math/13/rw/randgen.h#1 $
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
 * $Log: randgen.h,v $
 * Revision 1.4  1996/04/22 21:13:11  steves
 * Added declarations for the debugging functions
 * rwRandStateBuilds() and rwRandStateDeletes()
 *
 * Revision 1.3  1996/04/22  19:00:52  steves
 * Fixed some cosmetic stuff in the header.
 *
 * Revision 1.2  1996/04/22  18:30:24  steves
 * Added multiple include guards (oops).
 *
 **************************************************************************/



#include <rw/mathdefs.h>
#include <rw/dcomplex.h>      /* Looking for typedef for DComplex */
#include <rw/rwfile.h>
#include <rw/vstream.h>

class RWRandGeneratorState;   // Reference counted shared part of generator objects

/**
 * @ingroup statistics_classes
 * @brief Generates random numbers uniformly distributed
 * over the interval <tt>[0, 1]</tt>.
 *
 * Class RWRandGenerator generates random numbers uniformly distributed
 * over the interval <tt>[0, 1]</tt>. RWRandGenerator objects can be
 * independent or share state with each other. The Essential Math
 * Module contains a global default state that is shared by all
 * generators created with the default constructor. Using the default
 * generator state does not guarantee repeatable sequences, even
 * if you restart() it using a specific seed. This is because the
 * global state may be shared among several applications in a multithreaded
 * environment. There is no protection against multiple threads
 * of control using the global default state at the same time. To
 * achieve repeatable sequences, you must construct a generator
 * with a specific seed value and use the generator for random number
 * generation.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/randgen.h>
 * RWRandGenerator gen;
 * @endcode
 */
class RW_MATH_SYMBOLIC RWRandGenerator
{
private:
    // reference counted guts of the generator
    RWRandGeneratorState* state_;
public:
    /**
     * Constructs a generator with the shared global default state.
     */
    RWRandGenerator();                       // uses shared global random state

    /**
     * Constructs a generator with specified initial \a seed. The generator
     * has its own state.
     */
    RWRandGenerator(long seed);

    /**
     * Constructs a copy of \a x. The generators share state.
     */
    RWRandGenerator(const RWRandGenerator& x);

    ~RWRandGenerator();

    /**
     * Returns a random double in <tt>[0, 1]</tt>.
     */
    double operator()();

    long   seed() const;               // return seed used to initialize

    /**
     * Restarts the generator with a random seed.
     */
    void   restart();

    /**
     * Restarts the generator with the specified \a seed.
     */
    void   restart(long seed);

    /**
     * Sets state to argument's state.
     */
    void   reference(const RWRandGenerator& x);

    /**
     * Makes the state independent, but doesn't change it.
     */
    void   deepenShallowCopy();

    // not implemented, here to avoid the compiler building one
    /**
     * Sets state to argument's state.
     */
    RWRandGenerator& operator=(const RWRandGenerator& x);

    //  RWRandGeneratorState *state() const { return state_; }
    //  void   setState(RWRandGeneratorState *s);

    bool operator==(const RWRandGenerator& u) const;

    bool operator!=(const RWRandGenerator& u) const;

    /**
     * Outputs a summary of self's internal state on ostream \a s.
     */
    void printOn(std::ostream& s) const;

    /**
     * Inputs an internal state from istream \a s to self.
     */
    void scanFrom(std::istream& s);

    /**
     * Restores internal state of self from an RWFile.
     */
    void restoreFrom(RWFile&);      // Restore from binary

    /**
     * Restores internal state of self from a virtual stream.
     */
    void restoreFrom(RWvistream&);  // Restore from virtual stream

    /**
     * Stores internal state of self in a binary format to an RWFile.
     */
    void saveOn(RWFile&) const;     // Store using binary

    /**
     * Stores internal state of self to a virtual stream.
     */
    void saveOn(RWvostream&) const; // Store to virtual stream
};

#if defined(RW_DEBUG)
int RW_MATH_SYMBOLIC rwRandStateBuilds();
int RW_MATH_SYMBOLIC rwRandStateDeletes();
#endif

/**
 * @relates RWRandGenerator
 *
 * Outputs a summary of the internal state of \a rgen on ostream \a s.
 */
RW_MATH_SYMBOLIC std::ostream& operator << (std::ostream& s, const RWRandGenerator& rgen);

/**
 * @relates RWRandGenerator
 *
 * Inputs an internal state from istream \a s to \a rgen.
 */
RW_MATH_SYMBOLIC std::istream& operator >> (std::istream& s, RWRandGenerator& rgen);

#endif
