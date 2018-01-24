#ifndef RW_TOOLS_LIMITS_H
#define RW_TOOLS_LIMITS_H

/**********************************************************************
 *
 * Tools-specific limits
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/limits.h#1 $
 *
 **********************************************************************
 *
 * Copyright (c) 1989-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 **********************************************************************/

#include <rw/config/rwconfig_tls.h>      /* Set compiler-specific flags */

#include <limits.h>

// 64-bit integral limits specific to Tools and historically preferred
// (other Tools-specific limits defined in the config generated header)

#if defined(RW_LP64) || defined(RW_ILP64)

// 64-bit long int available
#  define RW_INT64_MAX  RW_LONG_MAX
#  define RW_INT64_MIN  RW_LONG_MIN

#  define RW_UINT64_MAX  RW_ULONG_MAX
#  define RW_UINT64_MIN  RW_ULONG_MIN

#elif defined(RW_ILP32LL) || defined(RW_LP64LL) || defined(RW_LLP64)

#  define RW_INT64_MAX  RW_LLONG_MAX
#  define RW_INT64_MIN  RW_LLONG_MIN

#  define RW_UINT64_MAX  RW_ULLONG_MAX
#  define RW_UINT64_MIN  RW_ULLONG_MIN

#else

#  error No 64-bit integral type available.

#endif // 64 bit integer limits

#endif // RW_TOOLS_LIMITS_H
