#ifndef __RWFCOMPLEX_H__
#define __RWFCOMPLEX_H__

/***************************************************************************
 *
 * fcomplex.h - Provide an FComplex type by regular name
 *
 * If the standard complex<ftype> is available, provide
 *  typedef complex<float> FComplex;
 * Otherwise declare the class FComplex, provide some of the member
 * definitions.
 *
 * $Id: //math/13/rw/fcomplex.h#1 $
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

// fully compliant std::complex
#include <complex>

/**
 * @ingroup math_classes
 *
 * Convenience typedef for C++ Standard Library complex number of type
 * \c float.
 */
typedef std::complex<float> FComplex;

#ifdef RW_USING_SUNPERF
// needs this to repack returned values
inline FComplex RW_SPL_FCOMPLEX(rw_floatcomplex x)
{
    return FComplex(x.re, x.im);
}
#endif

#endif /* __RWFCOMPLEX_H__ */
