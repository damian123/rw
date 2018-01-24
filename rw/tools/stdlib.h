#ifndef RW_TOOLS_STDLIB_H
#define RW_TOOLS_STDLIB_H

/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/stdlib.h#1 $
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

#include <rw/defs.h>

/**
 * @internal
 * @ingroup cstdlib
 *
 * Converts the string pointed to by \c str to a double, stopping at the
 * first character that is not a part of the double representation (see
 * below). If \c endptr is not null, it will be set the first character that
 * is not a part of the double representation. Leading whitespace is
 * consumed before the conversion begins.
 *
 * A valid double string representation consists of an optional plus \c '+'
 * or minus \c '-' sign, followed by a non-empty sequence of digits which
 * may include a decimal separator \c '.', followed by an optional exponent
 * string. An exponent string is represented as a string beginning with
 * either \c 'e' or \c 'E', followed by an optional plus \c '+' or minus \c
 * '-' sign, followed by a non-empty sequence of digits.
 *
 * @note
 * This function differs from the C Standard Library function in that
 * it does not take the current locale in to account. This function's
 * behavior is consistent with the "C" locale. This function does not
 * support parsing strings containing hexadecimal digits, or representations
 * of infinity and NaN.
 */
RW_TOOLS_SYMBOLIC double rw_strtod_nolocale(const char* str, char** endptr);

#endif
