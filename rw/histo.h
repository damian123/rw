#ifndef __RWHISTO_H__
#define __RWHISTO_H__

/***************************************************************************
 *
 * histo.h - Declarations for Histogram Class.
 *
 * $Id: //math/13/rw/histo.h#1 $
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

#include <rw/ivec.h>           /* Included for backward compatibility */
#include <rw/dvec.h>           /* Included for backward compatibility */
#include <rw/math/mathvec.h>
#include <rw/rstream.h>

/**
 * @ingroup statistics_classes
 *
 * @brief Constructs and maintains a histogram of input data.
 *
 * The class Histogram constructs and maintains a histogram of input
 * data. The count is stored as a public \link RWMathVec RWMathVec<int>
 * \endlink base class. It can be used anywhere an \link RWMathVec
 * RWMathVec<int> \endlink can be used.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/histo.h>
 * #include <rw/math/mathvec.h>
 * RWMathVec<double> v;
 * unsigned n;
 * Histogram H(n, v);
 * @endcode
 */
class RW_MATH_GLOBAL Histogram : public RWMathVec<int>
{
private:
    size_t                totalCounts_;
    size_t                nbins_;
    RWMathVec<double>     binValues_;
    size_t                largerValues_;
    size_t                smallerValues_;

public:
    /**
     * Constructs a histogram with 0 bins.
     */
    Histogram();

    /**
     * Constructs a histogram from the data in \a v, with \a nbins bins
     * of data. The bins are of equal size and are scaled with the maximum
     * and minimum data in \a v. The count in the histogram is initialized
     * with the contents of \a v.
     */
    Histogram(size_t nbins, const RWMathVec<double>& v);

    /**
     * Constructs a histogram with no initial data, with bin boundaries
     * defined by the data in \a b. The number of bins is \c b.length()-1.
     */
    Histogram(const RWMathVec<double>& b);

    /**
     * Constructs a histogram with no initial data, with \a nbins bins.
     * The bins are of equal size and are scaled with \a max and \a
     * min.
     */
    Histogram(size_t nbins, double min, double max);

    /**
     * Copy constructor. The new histogram does \e not reference
     * the old histogram's data.
     */
    Histogram(const Histogram&);

    /**
     * Returns the total accumulated number of data values that are
     * greater than the largest bin.
     */
    size_t            larger() const {
        return largerValues_;
    }

    /**
     * Returns the total accumulated number of data values that are
     * less than the smallest bin.
     */
    size_t            smaller() const {
        return smallerValues_;
    }

    /**
     * Returns the number of bins.
     */
    size_t            bins() const {
        return nbins_;
    }

    /**
     * Returns the total accumulated number of data values in the histogram.
     * This the sum of all the bin counts, plus the number of data values
     * that are smaller than the smallest bin, and the number of data
     * values that are larger than the largest bin.
     */
    size_t          totalCount() const {
        return totalCounts_;
    }

    /**
     * Returns the boundaries of the histogram's bins as a vector.
     */
    RWMathVec<double> binBoundaries() const {
        return binValues_.copy();
    }

    /**
     * Adds data value \a a to the histogram.
     */
    void                  addCount(double a);

    /**
     * Adds the contents of the vector \a v to the histogram.
     */

    void                  addCount(const RWMathVec<double>& v);

    /**
     * Removes all data values from the histogram. Each bin count, including
     * larger and smaller counts, is set to 0. The bin boundaries are
     * retained. Returns a reference to self.
     */
    Histogram&      resetCounts();

    /**
     * Assignment operator. Self is a copy of the data in \a h.
     */
    Histogram&  operator=(const Histogram& h);

    // Storage requirements.
    size_t                binaryStoreSize() const;

    /**
     * Outputs a summary of the histogram \a h on \b std::ostream \a s.
     */
    friend RW_MATH_SYMBOLIC std::ostream&  operator<<(std::ostream& s, const Histogram& h);

};

#endif /*__RWHISTO_H__*/
