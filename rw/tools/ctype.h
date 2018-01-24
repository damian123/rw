#ifndef RW_TOOLS_CTYPE_H
#define RW_TOOLS_CTYPE_H

/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/ctype.h#1 $
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
 * Returns non-zero if the character \a c is a digit, zero otherwise. A
 * digit is defined as one of the following characters:
 *
 * <tt>'0' '1' '2' '3' '4' '5' '6' '7' '8' '9'</tt>
 *
 * @note
 * This function differs from the C Standard Library function in that
 * it does not take the current locale in to account. This function's
 * behavior is consistent with the "C" locale.
 */
inline int rw_isdigit_nolocale(int c)
{
    return ('0' <= c && c <= '9');
}

/**
 * @internal
 * @ingroup cstdlib
 *
 * Returns non-zero if the character \a c is whitespace, zero otherwise.
 * Whitespace is defined as one of the following characters:
 *
 * <tt>' ' '\\t' '\\n' '\\v' '\\f' '\\r'</tt>
 *
 * @note
 * This function differs from the C Standard Library function in that
 * it does not take the current locale in to account. This function's
 * behavior is consistent with the "C" locale.
 */
inline int rw_isspace_nolocale(int c)
{
    switch (c) {
        case ' ':
        case '\t':
        case '\n':
        case '\v':
        case '\f':
        case '\r':
            return 1;
        default:
            return 0;
    }
}

#endif
