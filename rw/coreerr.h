#ifndef RW_TOOLS_COREERR_H
#define RW_TOOLS_COREERR_H



/**********************************************************************
 *
 * Error messages for Core.h++
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/coreerr.h#1 $
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
#include <rw/message.h>

/*
 * RWMsgId is a typedef for RWCatMsg*.
 * For each RWMsgId of the form RWCORE_XXX their corresponds an RWCOREMSG_XXX
 *   of type RWCatMsg.
 * Instead of simply defining a variable: RWMsgId RWCORE_XXX = &RWCOREMSG_XXX,
 *   RWCORE_XXX is #defined to be a function that returns &RWCOREMSG_XXX.
 * This is done to side-step the problem of compilers (e.g. DEC)
 *   that generate pre-main code for the initialization of address constants.
 * It also serves us in the case of DLLs where variables cannot be exported.
 */

#ifndef RW_NO_RECURSIVE_MACRO_DEF
#  define RWCORE_RWEOF        RWCORE_RWEOF()
#  define RWCORE_GENERIC    RWCORE_GENERIC()
#  define RWCORE_INVADDR    RWCORE_INVADDR()
#  define RWCORE_LOCK       RWCORE_LOCK()
#  define RWCORE_NOINIT     RWCORE_NOINIT()
#  define RWCORE_NOMEM      RWCORE_NOMEM()
#  define RWCORE_OPERR      RWCORE_OPERR()
#  define RWCORE_OUTALLOC   RWCORE_OUTALLOC()
#  define RWCORE_OVFLOW     RWCORE_OVFLOW()
#  define RWCORE_STREAM     RWCORE_STREAM()
#  define RWCORE_SYNSTREAM  RWCORE_SYNSTREAM()
#  define RWCORE_NULLPTR    RWCORE_NULLPTR()
#else
#  define RWCORE_RWEOF        rwcore_RWEOF()
#  define RWCORE_GENERIC    rwcore_GENERIC()
#  define RWCORE_INVADDR    rwcore_INVADDR()
#  define RWCORE_LOCK       rwcore_LOCK()
#  define RWCORE_NOINIT     rwcore_NOINIT()
#  define RWCORE_NOMEM      rwcore_NOMEM()
#  define RWCORE_OPERR      rwcore_OPERR()
#  define RWCORE_OUTALLOC   rwcore_OUTALLOC()
#  define RWCORE_OVFLOW     rwcore_OVFLOW()
#  define RWCORE_STREAM     rwcore_STREAM()
#  define RWCORE_SYNSTREAM  rwcore_SYNSTREAM()
#  define RWCORE_NULLPTR    rwcore_NULLPTR()
#endif


extern RW_TOOLS_SYMBOLIC RWMsgId RWCORE_RWEOF;
extern RW_TOOLS_SYMBOLIC RWMsgId RWCORE_GENERIC;
extern RW_TOOLS_SYMBOLIC RWMsgId RWCORE_INVADDR;
extern RW_TOOLS_SYMBOLIC RWMsgId RWCORE_LOCK;
extern RW_TOOLS_SYMBOLIC RWMsgId RWCORE_NOINIT;
extern RW_TOOLS_SYMBOLIC RWMsgId RWCORE_NOMEM;
extern RW_TOOLS_SYMBOLIC RWMsgId RWCORE_OPERR;
extern RW_TOOLS_SYMBOLIC RWMsgId RWCORE_OUTALLOC;
extern RW_TOOLS_SYMBOLIC RWMsgId RWCORE_OVFLOW;
extern RW_TOOLS_SYMBOLIC RWMsgId RWCORE_STREAM;
extern RW_TOOLS_SYMBOLIC RWMsgId RWCORE_SYNSTREAM;
extern RW_TOOLS_SYMBOLIC RWMsgId RWCORE_NULLPTR;


#endif /* RW_TOOLS_COREERR_H */
