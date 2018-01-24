#ifndef RW_CURRENCY_DEC52_H
#define RW_CURRENCY_DEC52_H

/*--------------------------------------------------------------
 *
 * Type definitions provided for backward compatibility, plus
 * initialization variables to cause proper initialization of
 * global variables.
 *
 * $Id: //money/13/rw/dec52.h#1 $
 *
 * Copyright (c) 1993-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 *--------------------------------------------------------------*/

/* for backward compatibility */
#ifndef RW_MONEY_DEC52_H
#  define RW_MONEY_DEC52_H
#endif

#include <rw/currency/mpint.h>
#include <rw/currency/mp1int.h>
#include <rw/currency/decimal.h>

typedef RWDecimal< RWMultiPrecisionInt<1> > RWDecimal52;
typedef RWDecimalInexactErr< RWMultiPrecisionInt<1> > RWDecimal52InexactErr;
typedef RWDecimalOverflowErr< RWMultiPrecisionInt<1> > RWDecimal52OverflowErr;

#endif  // RW_CURRENCY_DEC52_H
