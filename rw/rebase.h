#ifndef RW_TOOLS_REBASE_H
#define RW_TOOLS_REBASE_H



/**********************************************************************
 *
 * Declaration for class RWREBaseClass
 * $Log: rebase.h,v $
 * Revision 7.3  1996/02/18 01:45:35  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 * Revision 7.2  1995/10/24 06:35:19  jims
 * Add guard against multiple inclusion
 * Revision 7.1  1995/10/24  06:27:36  jims
 * Move to revision 7.1
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rebase.h#1 $
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


/////////////////////////////////////////////////////////////////////////////
//
// RWREBaseClass
//
//  Stores enums shared by both RWTRegularExpression
//  and RWTRegularExpressionImp.
//
//  This is inherited by both RWTRegularExpression and RWTRegularExpressionImp
//  because RWTRegularExpressionImp uses some enums that are declared in
//  RWTRegularExpression but RWTRegularExpression has a member of type
//  RWTRegularExpressionImp. This can lead to a "chicken and egg" situation
//  that can confuse some compilers; hence this class.
//
/////////////////////////////////////////////////////////////////////////////

#include <rw/defs.h>

class RW_TOOLS_SYMBOLIC RWREBaseClass
{
public:

    enum statusType {
        OK = 0,
        NOT_SUPPORTED = 0x100,
        NO_MATCH, BAD_PATTERN, BAD_COLLATING_ELEMENT, BAD_CHAR_CLASS_TYPE,
        TRAILING_BACKSLASH, UNMATCHED_BRACKET, UNMATCHED_PARENTHESIS,
        UNMATCHED_BRACE, BAD_BRACE, BAD_CHAR_RANGE, OUT_OF_MEMORY, BAD_REPEAT
    };

    enum {MAX_NO_OF_SUBEXPRESSIONS = 1}; // Includes subexpr 0, the whole string
};

#endif  /* RW_TOOLS_REBASE_H */
