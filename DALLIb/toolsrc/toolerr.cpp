/*
 * Definitions for Tools.h++ error messages
 *
 * $Id: toolerr.cpp,v 6.5 1994/07/18 20:51:00 jims Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 *
 * (c) Copyright 1989, 1990, 1991, 1992, 1993, 1994 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 52.227-7013.
 * 
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 ***************************************************************************
 *
 *
 ***************************************************************************
 *
 * $Log: toolerr.cpp,v $
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.3  1994/06/14  16:31:57  jims
 * RWTOOL_XXX now a function that returns &RWTOOLMSG_XXX so as not
 * to depend on static init to initialize address constant
 *
 * Revision 6.2  1994/05/16  17:41:03  jims
 * Change "_export" to "rwexport"
 *
 * Revision 6.1  1994/04/15  19:08:00  vriezen
 * Move all files to 6.1
 *
 * Revision 2.7  1994/03/02  19:20:24  jims
 * New message INDEXERR shows invalid index and number of items
 * in collection
 *
 * Revision 2.6  1994/02/15  18:46:48  jims
 * Remove extraneous semicolons
 *
 * Revision 2.5  1993/10/27  19:08:18  jims
 * For DLLs, RWTOOL_XXX must be an exported function, not an object
 *
 * Revision 2.4  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.3  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.2  1993/05/18  19:08:14  keffer
 * Ported to cfront V2.1.
 *
 * Revision 2.1  1993/05/18  00:40:21  keffer
 * Rewrote to use new messaging and exception handling facility.
 *
 */

#include "rw/toolerr.h"
#include "rw/generic.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: toolerr.cpp,v $ $Revision: 6.5 $ $Date: 1994/07/18 20:51:00 $");

#undef  DECLARE_MSG
#define DECLARE_MSG(NAME, VALUE, MSG) \
        const RWCatMsg name2(RWTOOLMSG_,NAME) = {RWTOOL, VALUE, MSG};    \
        const RWMsgId  rwexport name2(RWTOOL_,NAME)  \
                                  { return &name2(RWTOOLMSG_,NAME); }

const char RWTOOL[] = "rwtools6.0";

DECLARE_MSG(ALLOCOUT,  0x2000, "[ALLOCOUT] %s destructor called with allocation outstanding")
DECLARE_MSG(BADRE,     0x2001, "[BADRE] Attempt to use invalid regular expression")
DECLARE_MSG(CRABS,     0x2002, "[CRABS] RWFactory: attempting to create abstract class with ID %hu (0x%hx)")
DECLARE_MSG(FLIST,     0x2003, "[FLIST] Free list size error: expected %ld, got %ld")
DECLARE_MSG(ID,        0x2004, "[ID] Unexpected class ID %hu; should be %hu")
DECLARE_MSG(INDEX,     0x2005, "[INDEX] Index (%u) out of range [0->%u]")
DECLARE_MSG(LOCK,      0x2006, "[LOCK] Locked object deleted")
DECLARE_MSG(LONGINDEX, 0x2007, "[LONGINDEX] Long index (%lu) out of range [0->%lu]")
DECLARE_MSG(MAGIC,     0x2008, "[MAGIC] Bad magic number: %ld (should be %ld)")
DECLARE_MSG(NEVECL,    0x2009, "[NEVECL] Unequal vector lengths: %u versus %u")
DECLARE_MSG(NOCREATE,  0x200a, "[NOCREATE] RWFactory: no create function for class with ID %hu (0x%hx)")
DECLARE_MSG(NOTALLOW,  0x200b, "[NOTALLOW] Function not allowed for derived class")
DECLARE_MSG(READERR,   0x200c, "[READERR] Read error")
DECLARE_MSG(REF,       0x200d, "[REF] Bad persistence reference")
DECLARE_MSG(SEEKERR,   0x200e, "[SEEKERR] Seek error")
DECLARE_MSG(STREAM,    0x200f, "[STREAM] Bad input stream")
DECLARE_MSG(SUBSTRING, 0x2010, "[SUBSTRING] Illegal substring (%d, %u) from %u element RWCString")
DECLARE_MSG(UNLOCK,    0x2011, "[UNLOCK] Improper use of locked object")
DECLARE_MSG(WRITEERR,  0x2012, "[WRITEERR] Write error")
DECLARE_MSG(INDEXERR,  0x2013, "[INDEXERR] Illegal Index (%u) for collection with %u elements")

