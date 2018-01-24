#ifndef __RWRANDMLCG_H__
#define __RWRANDMLCG_H__

/***************************************************************************
 *
 * randmlcg.h - A baseclass for a multiplicative linear congruential
 * random number generator.
 *
 ***************************************************************************
 *
 * $Id: //math/13/rw/randmlcg.h#1 $
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

/**
 * @ingroup statistics_classes
 *
 * @brief Abstract base class for classes that generate random numbers
 * uniformly distributed over the interval <tt>[0,1]</tt>.
 *
 * Abstract base class for classes that generate random numbers
 * uniformly distributed over the interval <tt>[0, 1]</tt>.
 */
class RW_MATH_GLOBAL RWRandGenBasicMLC
{
private:

    // ---------- Generator state ----------
    // The generator's state consists of the seed.

    long long seed_;

protected:
    double doRandom(long long A, long long M) {
        unsigned long long tmp = static_cast<unsigned long long>(seed_ * A);
        seed_ = tmp % M;
        return RW_STATIC_CAST(double, seed_) / RW_STATIC_CAST(double, M);
    }

public:

    // ---------- Constructors ----------
    RWRandGenBasicMLC();                // default init, seed = time(0)
    RWRandGenBasicMLC(unsigned long seed) { // specify the seed
        seed_ = seed;
    }
    RWRandGenBasicMLC(const RWRandGenBasicMLC& u): seed_(u.seed_) {}

    // ---------- Destructor ----------
    virtual ~RWRandGenBasicMLC() {};

    // ---------- Member functions ----------

    virtual double operator()() = 0;   // returns a random double in [0,1]

    unsigned long seed() const {       // return seed used to initialize
        return RW_STATIC_CAST(unsigned long, seed_);
    }
    void restart();                    // restart with a random seed
    void restart(unsigned long seed) { // restart with specified seed
        seed_ = seed;
    }

    RWRandGenBasicMLC& operator=(const RWRandGenBasicMLC& u) {
        if (this != &u) {
            seed_ = u.seed_;
        }
        return *this;
    }

    bool operator==(const RWRandGenBasicMLC& u) const {
        return seed_ == u.seed_;
    }
    bool operator!=(const RWRandGenBasicMLC& u) const {
        return seed_ != u.seed_;
    }


    /**
     * Outputs a summary of self's internal state on ostream \a s.
     */
    void printOn(std::ostream& s) const;  // outstrm table, seed, index

    /**
     * Inputs an internal state from istream \a s to self.
     */
    void scanFrom(std::istream& s);       // instrm table, seed, index

    /**
     * Restores internal state of self from a RWFile.
     */
    void restoreFrom(RWFile& file);                // Restore from binary

    /**
     * Restores internal state of self from a virtual stream.
     */
    void restoreFrom(RWvistream& s);               // Restore from virtual stream

    /**
     * Stores internal state of self a binary format to an RWFile.
     */
    void saveOn(RWFile& file) const;               // Store using binary

    /**
     * Stores internal state of self to a virtual stream.
     */
    void saveOn(RWvostream& s) const;              // Store to virtual stream
};

/**
 * @relates RWRandGenBasicMLC
 *
 * Outputs a summary of the internal state of \a gen on ostream \a s.
 */
RW_MATH_SYMBOLIC std::ostream&
operator <<(std::ostream& s, const RWRandGenBasicMLC& gen);

/**
 * @relates RWRandGenBasicMLC
 *
 * Inputs an internal state from istream \a s to \a gen.
 */
RW_MATH_SYMBOLIC std::istream&
operator >>(std::istream& s, RWRandGenBasicMLC& gen);

#endif  // __RWRANDMLCG_H__
