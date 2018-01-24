#ifndef __RWDCOMPLEX_H__
#define __RWDCOMPLEX_H__

/***************************************************************************
 *
 * dcomplex.h - Standard header file for double precision complex.
 *
 * $Id: //math/13/rw/dcomplex.h#1 $
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
 * Because there are so many variations of "Complex" types, this
 * header file attempts to standardize what the user sees.  If you
 * include this header file and use type "DComplex" (for double
 * precision complex) you generally need not worry about what
 * compiler you are using: This header will select the header file
 * that is appropriate to your platform, and typedef DComplex to
 * whatever double precision complex type is supported.
 *
 ***************************************************************************/

#include <rw/mathdefs.h>

// If we are using a recognizable version of the Standard Library, use the
// standard complex class provided therein.
#include <complex>

#if defined(RW_MULTI_THREAD) && defined(_DEBUG) && defined(_WIN32) && \
    defined(__INTEL_COMPILER) && (RWSTDLIBTYPE == 2) && _MSC_VER < 1700
// if /MDd option is used, icl doesn't generate a copy
// constructor for std::complex<double>
class DComplex: public std::complex<double>
{
public:
    DComplex() {}
    DComplex(const DComplex& d) {
        *this = d;
    }
    DComplex(const std::complex<double>& d): std::complex<double>(d) { }
    DComplex(const double r) : std::complex<double>(r) {}
    DComplex(const double& r, const double& i) : std::complex<double>(r, i) {}
};
#else
/**
 * @ingroup math_classes
 *
 * Convenience typedef for C++ Standard Library complex number of type
 * \c double.
 */
typedef std::complex<double> DComplex;
#endif
typedef DComplex(*CmathFunTy)(const DComplex&);
typedef double(*CmathFunTy2)(const DComplex&);

#ifdef RW_USING_SUNPERF
// needs this to repack returned values
inline DComplex RW_SPL_DCOMPLEX(rw_doublecomplex x)
{
    return DComplex(x.re, x.im);
}
#endif

#endif /* __RWDCOMPLEX_H__ */
