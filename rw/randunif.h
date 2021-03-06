#ifndef __RWRANDUNIF_H__
#define __RWRANDUNIF_H__

/***************************************************************************
 *
 * randunif.h
 * Declarations for RandUniform: Uniformly distributed random numbers.
 *
 * $Id: //math/13/rw/randunif.h#1 $
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
#include <rw/mathwind.h>

template< class T >
class RWGenMat;

template< class T >
class RWMathVec;

class RW_MATH_SYMBOLIC RandUniform
{
private:
    long                  table[55];
    int                   index;
protected:
    long                  seeder;         // Random number generator seed.
public:
    RandUniform();                        // Initialize a generator with clock time.
    RandUniform(long s) {
        reseed(s);   // Initialize a generator with seed.
    }

    long                  seed() const {
        return seeder;
    }
    void                  reseed(long);                   // Reseed with given seed

    double                randValue();                    // Return a random number.
    void                  randValue(double*, size_t n);   // Return an array of random numbers.
    RWMathVec<double>     randValue(size_t n);            // Return a vector of n random numbers.
    RWGenMat<double>      randValue(size_t nr, size_t nc);        // Return a matrix of random numbers.
};

#endif /*__RWRANDUNIF_H__*/
