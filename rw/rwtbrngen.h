#ifndef __RWTBRNGEN_H__
#define __RWTBRNGEN_H__


/***************************************************************************
 *
 * rwtbrngen.h - Declarations for RWTBRNGenerator Class.
 *
 * $Id: //math/13/rw/rwtbrngen.h#1 $
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
 */

#include <rw/mathdefs.h>

#ifdef RW_USING_MKL
// We really should include mkl_vsl_defines.h here, but that will require a new characterization.
#  define VSL_BRNG_SHIFT      20
#  define VSL_BRNG_INC        (1<<VSL_BRNG_SHIFT)

#  define VSL_BRNG_MCG31      (VSL_BRNG_INC)
#  define VSL_BRNG_R250       (VSL_BRNG_MCG31    +VSL_BRNG_INC)
#  define VSL_BRNG_MRG32K3A   (VSL_BRNG_R250     +VSL_BRNG_INC)
#  define VSL_BRNG_MCG59      (VSL_BRNG_MRG32K3A +VSL_BRNG_INC)
#  define VSL_BRNG_WH         (VSL_BRNG_MCG59    +VSL_BRNG_INC)
#  define VSL_BRNG_SOBOL      (VSL_BRNG_WH       +VSL_BRNG_INC)
#  define VSL_BRNG_NIEDERR    (VSL_BRNG_SOBOL    +VSL_BRNG_INC)
#  define VSL_BRNG_MT19937    (VSL_BRNG_NIEDERR  +VSL_BRNG_INC)
#  define VSL_BRNG_MT2203     (VSL_BRNG_MT19937  +VSL_BRNG_INC)


typedef void* VSLStreamStatePtr;

struct RW_MATH_SYMBOLIC RWBRNG {
    enum GeneratorType { MCG31 = VSL_BRNG_MCG31,
                         R250 = VSL_BRNG_R250,
                         MRG32K3A = VSL_BRNG_MRG32K3A,
                         MCG59 = VSL_BRNG_MCG59,
                         MT19937 = VSL_BRNG_MT19937,
                         MT2203 = VSL_BRNG_MT2203
                       };
};

class RW_MATH_SYMBOLIC RWBRNGeneratorBase
{
private:
    VSLStreamStatePtr stream_;            // Pointer to MKL random number stream
    int method_;
    long seed_;
    unsigned int g_;

protected:
    // protected assignment operator so that the base class can't be assigned
    // to an instance using a different generator type.
    RWBRNGeneratorBase& operator=(const RWBRNGeneratorBase&);

public:
    RWBRNGeneratorBase(RWBRNG::GeneratorType gen = RWBRNG::MCG31);
    RWBRNGeneratorBase(long seed, RWBRNG::GeneratorType gen = RWBRNG::MCG31); // specify the seed
    RWBRNGeneratorBase(const RWBRNGeneratorBase&);
    virtual ~RWBRNGeneratorBase();

    double operator()();                     // returns a random double in [0,1]

    const VSLStreamStatePtr& streamStatePtr() { // returns the VSLStreamStatePtr
        return stream_;
    }

    long   seed() const;                    // return seed used to initialize
    int    method() const;                    // return the method passed to the generator
    int    method(int method);                    // set the method to the argument passed and return previous value
    void   restart();                             // restart with a random seed
    void   restart(long seed);                    // restart with specified seed

    bool operator==(const RWBRNGeneratorBase& u) const;
    bool operator!=(const RWBRNGeneratorBase& u) const;

    // I/O
    void printOn(std::ostream& s) const;
    void scanFrom(std::istream& s);
    void restoreFrom(RWFile&);      // Restore from binary
    void restoreFrom(RWvistream&);  // Restore from virtual stream
    void saveOn(RWFile&) const;     // Store using binary
    void saveOn(RWvostream&) const; // Store to virtual stream
};

template <RWBRNG::GeneratorType G>
class RWTBRNGenerator : public RWBRNGeneratorBase
{
public:
    RWTBRNGenerator(): RWBRNGeneratorBase(G) {}
    RWTBRNGenerator(long seed): RWBRNGeneratorBase(seed, G) {}
    RWTBRNGenerator(const RWTBRNGenerator<G>&);
    ~RWTBRNGenerator() {}

    RWTBRNGenerator<G>& operator=(const RWTBRNGenerator<G>& x) {
        RWBRNGeneratorBase::operator=(x);
        return *this;
    }
};

RW_MATH_SYMBOLIC std::ostream& operator<<(std::ostream& s, const RWBRNGeneratorBase& rgen);
RW_MATH_SYMBOLIC std::istream& operator>>(std::istream& s, RWBRNGeneratorBase& rgen);

#  ifdef RW_MATH_COMPILE_INSTANTIATE
#    include <rw/rwtbrngen.cc>
#  endif

#endif

#endif
