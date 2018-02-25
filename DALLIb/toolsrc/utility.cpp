/*
 * Definitions for Rogue Wave Utility and obsolete Error Functions
 *
 * $Id: utility.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
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
 * $Log: utility.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/06/02  19:11:53  foote
 * Port to Symantec 7.0 on Macintosh
 *
 * Revision 6.1  1994/04/15  19:08:02  vriezen
 * Move all files to 6.1
 *
 * Revision 2.8  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.7  1993/08/06  04:33:45  randall
 * removed use of RW_CAFE_ALPHA
 *
 * Revision 2.6  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.5  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.3  1993/02/11  22:42:59  keffer
 * Added IBM xlC to the list of compilers without genericerror().
 *
 * Revision 2.2  1993/02/07  19:53:58  keffer
 * genericerror now exported from DLL.
 *
 * Revision 2.1  1992/11/26  19:50:28  myersn
 * port to Gnu gcc.  Also, simplify the dummy definition at the end.
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.9   30 May 1992 16:34:08   KEFFER
 * Inserted rwdummy1() to keep fussy librarians happy.
 * 
 *    Rev 1.7   28 May 1992 19:52:58   KEFFER
 * Ported to Metaware High-C
 * 
 *    Rev 1.6   24 Mar 1992 13:30:40   KEFFER
 * Ported to Microsoft C/C++ V7.0
 * 
 *    Rev 1.5   05 Nov 1991 14:07:04   keffer
 * Removed RWXXXXErr macros --- now done by error package
 * 
 *    Rev 1.4   17 Oct 1991 09:15:32   keffer
 * Changed include path to <rw/xxx.h>
 * 
 *    Rev 1.3   08 Oct 1991 13:45:18   keffer
 * Math V4.0 beta 2
 * 
 *    Rev 1.2   29 Jul 1991 14:41:00   keffer
 * Now #includes "rw/defs.h" before checking type of compiler.
 * 
 *    Rev 1.1   24 Jul 1991 12:56:20   keffer
 * Added pvcs keywords
 *
 */

#include "rw/defs.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: utility.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

/*
 * These guys declared "genericerror()", but
 * didn't bother offering a definition:
 */

#if defined(__TURBOC__) || defined(__ZTC__) || defined(__OREGON__) || defined(_MSC_VER) || defined(__HIGHC__) || defined(__GNUC__) || defined(__xlC__) && !defined(THINK_CPLUS)

#include "rw/rwerr.h"
#include "rw/coreerr.h"

int rwexport
genericerror(int ern, char* msg)
{
  RWTHROW( RWxmsg(RWMessage( RWCORE_GENERIC, ern, msg) ));
  return 1;
}
#endif

/********************************************************
 *							*
 *	Error Functions --- not used anymore, but	*
 *	included for backwards compatibility.		*
 *							*
 ********************************************************/

#ifdef V3_COMPATIBLE

STARTWRAP
#include <stdlib.h>
#include <stdio.h>
ENDWRAP

typedef void (*RWErrorHandler)(RWSeverity, const char*, const char*);

void
RWDefaultErrorRoutine(RWSeverity gravity, const char* routine, const char* comment)
{
  fputs("\n",stderr);
  fputs("** ",stderr);
  fputs(routine,stderr);
  fputs("\n",stderr);

  fputs("** ", stderr);
  fputs(comment, stderr);
  fputs("\n", stderr);

  switch (gravity) {
  case RWWARNING:
    fputs("** Processing continues.\n", stderr);
    break;
  case RWDEFAULT:
  case RWFATAL:
  default:
    fputs("** Processing terminated.\n", stderr);
    exit(gravity);
  };
}

RWErrorHandler RWErrorRoutine = RWDefaultErrorRoutine;

void
RWError(RWSeverity severity, const char* routine, const char* comment)
{
  (*RWErrorRoutine)(severity, routine, comment);
}

RWErrorHandler
setRWErrorHandler(RWErrorHandler routine)
{
  RWErrorHandler temp = RWErrorRoutine;
  RWErrorRoutine = routine;
  return temp;
}

#endif

// This definition is always compiled in case nothing else is,
// in order to quiet down some fussy librarians:
int rwUtilityDummy;
