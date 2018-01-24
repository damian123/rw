#ifndef RW_TOOLS_TOOLERR_H
#define RW_TOOLS_TOOLERR_H 1



/**********************************************************************
 *
 * Error messages
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/toolerr.h#1 $
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
 * For each RWMsgId of the form RWTOOL_XXX there is an RWTOOLMSG_XXX
 *   of type RWCatMsg.
 * Instead of simply defining a variable: RWMsgId RWTOOL_XXX = &RWTOOLMSG_XXX,
 *   RWTOOL_XXX is #defined to be a function that returns &RWTOOLMSG_XXX.
 * This is done to side-step the problem of compilers (e.g. DEC)
 *   that generate pre-main code for the initialization of address constants.
 * It also serves us in the case of DLLs where variables cannot be exported.
 */

#ifndef RW_NO_RECURSIVE_MACRO_DEF
#  define RWTOOL_ALLOCOUT     RWTOOL_ALLOCOUT()
#  define RWTOOL_BADRE        RWTOOL_BADRE()
#  define RWTOOL_CRABS        RWTOOL_CRABS()
#  define RWTOOL_FLIST        RWTOOL_FLIST()
#  define RWTOOL_ID           RWTOOL_ID()
#  define RWTOOL_INDEX        RWTOOL_INDEX()
#  define RWTOOL_LOCK         RWTOOL_LOCK()
#  define RWTOOL_LONGINDEX    RWTOOL_LONGINDEX()
#  define RWTOOL_MAGIC        RWTOOL_MAGIC()
#  define RWTOOL_NEVECL       RWTOOL_NEVECL()
#  define RWTOOL_NOCREATE     RWTOOL_NOCREATE()
#  define RWTOOL_NOTALLOW     RWTOOL_NOTALLOW()
#  define RWTOOL_READERR      RWTOOL_READERR()
#  define RWTOOL_REF          RWTOOL_REF()
#  define RWTOOL_SEEKERR      RWTOOL_SEEKERR()
#  define RWTOOL_STREAM       RWTOOL_STREAM()
#  define RWTOOL_SUBSTRING    RWTOOL_SUBSTRING()
#  define RWTOOL_CONSTSUBSTRING    RWTOOL_CONSTSUBSTRING()
#  define RWTOOL_UNLOCK       RWTOOL_UNLOCK()
#  define RWTOOL_WRITEERR     RWTOOL_WRITEERR()
#  define RWTOOL_INDEXERR     RWTOOL_INDEXERR()
#  define RWTOOL_NPOSINDEX    RWTOOL_NPOSINDEX()
#  define RWTOOL_NOPERSIST    RWTOOL_NOPERSIST()
#  define RWTOOL_CRABSS       RWTOOL_CRABSS()
#  define RWTOOL_DTINVALIDOP  RWTOOL_DTINVALIDOP()
#  define RWTOOL_IRREGULARSEQ RWTOOL_IRREGULARSEQ()
#  define RWTOOL_TRUNCATEDSEQ RWTOOL_TRUNCATEDSEQ()
#  define RWTOOL_ILLEGALSEQ   RWTOOL_ILLEGALSEQ()
#  define RWTOOL_FATALERR     RWTOOL_FATALERR()
#else
#  define RWTOOL_ALLOCOUT     rwtool_ALLOCOUT()
#  define RWTOOL_BADRE        rwtool_BADRE()
#  define RWTOOL_CRABS        rwtool_CRABS()
#  define RWTOOL_FLIST        rwtool_FLIST()
#  define RWTOOL_ID           rwtool_ID()
#  define RWTOOL_INDEX        rwtool_INDEX()
#  define RWTOOL_LOCK         rwtool_LOCK()
#  define RWTOOL_LONGINDEX    rwtool_LONGINDEX()
#  define RWTOOL_MAGIC        rwtool_MAGIC()
#  define RWTOOL_NEVECL       rwtool_NEVECL()
#  define RWTOOL_NOCREATE     rwtool_NOCREATE()
#  define RWTOOL_NOTALLOW     rwtool_NOTALLOW()
#  define RWTOOL_READERR      rwtool_READERR()
#  define RWTOOL_REF          rwtool_REF()
#  define RWTOOL_SEEKERR      rwtool_SEEKERR()
#  define RWTOOL_STREAM       rwtool_STREAM()
#  define RWTOOL_SUBSTRING    rwtool_SUBSTRING()
#  define RWTOOL_CONSTSUBSTRING    rwtool_CONSTSUBSTRING()
#  define RWTOOL_UNLOCK       rwtool_UNLOCK()
#  define RWTOOL_WRITEERR     rwtool_WRITEERR()
#  define RWTOOL_INDEXERR     rwtool_INDEXERR()
#  define RWTOOL_NPOSINDEX    rwtool_NPOSINDEX()
#  define RWTOOL_NOPERSIST    rwtool_NOPERSIST()
#  define RWTOOL_CRABSS       rwtool_CRABSS()
#  define RWTOOL_IRREGULARSEQ rwtool_IRREGULARSEQ()
#  define RWTOOL_TRUNCATEDSEQ rwtool_TRUNCATEDSEQ()
#  define RWTOOL_DTINVALIDOP  rwtool_DTINVALIDOP()
#  define RWTOOL_ILLEGALSEQ   rwtool_ILLEGALSEQ()
#  define RWTOOL_FATALERR     rwtool_FATALERR()
#endif


extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_ALLOCOUT;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_BADRE;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_CRABS;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_FLIST;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_ID;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_INDEX;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_LOCK;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_LONGINDEX;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_MAGIC;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_NEVECL;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_NOCREATE;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_NOTALLOW;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_READERR;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_REF;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_SEEKERR;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_STREAM;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_SUBSTRING;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_CONSTSUBSTRING;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_UNLOCK;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_WRITEERR;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_INDEXERR;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_NPOSINDEX;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_NOPERSIST;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_CRABSS;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_DTINVALIDOP;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_IRREGULARSEQ;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_TRUNCATEDSEQ;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_ILLEGALSEQ;
extern RWMsgId RW_TOOLS_SYMBOLIC RWTOOL_FATALERR;

#endif  /*  RW_TOOLS_TOOLERR_H */
