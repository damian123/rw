#ifndef __RWLSQFIT_H__
#define __RWLSQFIT_H__

/***************************************************************************
 *
 * lsqfit.h - Declarations for class LeastSqFit: linear least squares fit.
 *
 * $Id: //math/13/rw/lsqfit.h#1 $
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
 *
 **************************************************************************/

#include <rw/dvec.h>         /* Included for backward compatibility */
#include <rw/math/mathvec.h>

/**
 * @ingroup statistics_classes
 *
 * @brief Constructs a linear least squares fit to
 * a straight line from input data.
 *
 * The class LeastSqFit constructs a linear least squares fit to
 * a straight line from input data, with or without weighting factors.
 * This class is of precision double. Once a LeastSqFit is constructed,
 * various methods are provided to interrogate it.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lsqfit.h>
 * #include <rw/math/mathvec.h>
 * RWMathVec<double> x, y;
 * LeastSqFit lsf(x, y);
 * @endcode
 */
class RW_MATH_SYMBOLIC LeastSqFit
{
private:
    size_t               npts;

protected:
    double               LSq_slope;
    double               LSq_intercept;
    double               SD_slope;
    double               SD_intercept;
    double               correlation_coeff;

public:
    /**
     * Constructs a linear least squares fit to the data in vectors
     * \a x and \a y, without weights. Similar indices of the vectors
     * <i>x(i)</i> and <i>y(i) </i>are assumed to represent <i>(x,y)</i>
     * pairs. The vectors need not be the same length; trailing unmatched
     * points are ignored.
     */
    LeastSqFit(const RWMathVec<double>& x, const RWMathVec<double>& y);

    /**
     * Constructs a linear least squares fit to the data in vectors
     * \a x and \a y, using a set of known standard deviations for the
     * \a y values. Similar indices of the vectors <i>x(i) </i>and <i>y(i)</i>
     * are assumed to represent <i>(x,y)</i> pairs. These values are
     * weighted according to the \a y standard deviations given as
     * \f$ 1/sigmay(i)^{2} \f$. The vectors need not be
     * the same length; trailing unmatched points are ignored.
     */
    LeastSqFit(const RWMathVec<double>& x, const RWMathVec<double>& y,
               const RWMathVec<double>& sigmay);

    /**
     * Returns the sample linear correlation coefficient of the fit.
     */
    double               correlationCoeff() const {
        return correlation_coeff;
    }

    /**
     * Returns the intercept of the line.
     */
    double               intercept() const {
        return LSq_intercept;
    }

    /**
     * Returns the standard deviation of the calculated intercept. If
     * the standard deviations of the data is given, the standard deviation
     * for the intercept is calculated appropriately. In the unweighted
     * case, the standard deviation is estimated by factoring in the
     * variance of the data from the calculated line.
     */
    double               interceptStandardDev() const {
        return SD_intercept;
    }

    /**
     * Returns the slope of the line.
     */
    double               slope() const {
        return LSq_slope;
    }

    /**
     * Returns the standard deviation of the calculated slope. If the
     * standard deviations of the data are given, the standard deviation
     * for the slope is calculated appropriately. In the unweighted
     * case, the standard deviation is estimated by factoring in the
     * variance of the data from the calculated line.
     */
    double               slopeStandardDev() const {
        return SD_slope;
    }

    /**
     * Using the linear fit, returns the value \c x corresponding to
     * \a y.
     */
    double               xPosition(double y) const {
        return (y - LSq_intercept) / LSq_slope;
    }

    /**
     * Using the linear fit, returns the value \c y corresponding to
     * \a x.
     */
    double               yPosition(double x) const {
        return x * LSq_slope + LSq_intercept;
    }

    // Storage requirements.
    size_t               binaryStoreSize() const;

    /**
     * Outputs a summary of the linear fit \a f on \b std::ostream \a s.
     */
    friend RW_MATH_SYMBOLIC std::ostream&  operator<<(std::ostream& s, const LeastSqFit& f);
};

#endif /*__RWLSQFIT_H__*/
