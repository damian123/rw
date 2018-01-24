#ifndef __RWRANDBINO_H__
#define __RWRANDBINO_H__

/**************************************************************************
 *
 * randbino.h
 * Declarations for RandBinomial: Binomially distributed random numbers.
 *
 * $Id: //math/13/rw/randbino.h#1 $
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

#include <rw/randunif.h>

class RW_MATH_SYMBOLIC RandBinomial : public RandUniform
{
private:                // Private data used in the algorithms.
    double                prb, g, am, pc, plog, pclog, sq;
protected:
    size_t                tries;
    double                prob;
public:
    RandBinomial(size_t Ntries, double probability);
    RandBinomial(size_t n, size_t Ntries, double probability);

    size_t                trials() const {
        return tries;
    }
    double                probability() const {
        return prob;
    }
    void                  setTrials(size_t Ntries);
    void                  setProbability(double probability);

    double                randValue();                 // Return a random number.
    void                  randValue(double*, size_t n);// Return array of same
    RWMathVec<double>     randValue(size_t n);         // Return a vector of same
    RWGenMat<double>      randValue(size_t nr, size_t nc); // Return a matrix...
};

#endif /*__RWRANDBINO_H__*/
