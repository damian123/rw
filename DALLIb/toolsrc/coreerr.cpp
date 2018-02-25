/*
 * Definitions for Core.h++ error messages
 *
 * $Id: coreerr.cpp,v 6.5 1994/07/18 20:51:00 jims Exp $
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
 * $Log: coreerr.cpp,v $
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.3  1994/06/14  16:37:18  jims
 * RWCORE_XXX now a function that returns &RWCOREMSG_XXX so as not
 * to depend on static init to initialize address constants
 *
 * Revision 6.2  1994/05/16  16:57:51  jims
 * Use functions for DLL since can't export variables
 *
 * Revision 6.1  1994/04/15  19:05:47  vriezen
 * Move all files to 6.1
 *
 * Revision 2.8  1994/02/15  18:46:48  jims
 * Remove extraneous semicolons
 *
 * Revision 2.7  1993/10/29  16:37:55  alv
 * separated Math.h++ out, so Money.h++ can stand on its own
 *
 * Revision 2.5  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.4  1993/06/21  22:59:29  randall
 * undefined EOF
 *
 * Revision 2.3  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.2  1993/05/18  19:08:14  keffer
 * Ported to cfront V2.1.
 *
 */

#include "rw/coreerr.h"
#include "rw/generic.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: coreerr.cpp,v $ $Revision: 6.5 $ $Date: 1994/07/18 20:51:00 $");

#undef  EOF 
#undef  DECLARE_MSG
#define DECLARE_MSG(NAME, VALUE, MSG) \
        const RWCatMsg name2(RWCOREMSG_,NAME) = {RWCORE, VALUE, MSG};    \
        const RWMsgId  rwexport name2(RWCORE_,NAME)  \
                                  { return &name2(RWCOREMSG_,NAME); }

const char RWCORE[] = "rwcore6.0";

DECLARE_MSG(EOF,      0x1000, "[EOF] EOF on input")
DECLARE_MSG(GENERIC,  0x1001, "[GENERIC] Generic error number %d; %s")
DECLARE_MSG(INVADDR,  0x1002, "[INVADDR] Invalid address: %lx")
DECLARE_MSG(LOCK,     0x1003, "[LOCK] Unable to lock memory")
DECLARE_MSG(NOINIT,   0x1004, "[NOINIT] Memory allocated without being initialized")
DECLARE_MSG(NOMEM,    0x1005, "[NOMEM] No memory")
DECLARE_MSG(OPERR,    0x1006, "[OPERR] Could not open file %s")
DECLARE_MSG(OUTALLOC, 0x1007, "[OUTALLOC] Memory released with allocations still outstanding")
DECLARE_MSG(OVFLOW,   0x1008, "[OVFLOW] Overflow error -> \"%.*s\" <- (%u max characters)")
DECLARE_MSG(STREAM,   0x1009, "[STREAM] Bad input stream")
DECLARE_MSG(SYNSTREAM,0x100a, "[SYNSTREAM] Syntax error in input stream: expected %s, got %s")

