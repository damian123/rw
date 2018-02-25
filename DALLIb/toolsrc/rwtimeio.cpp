/*
 * RWTime I/O
 *
 * $Id: rwtimeio.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
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
 * $Log: rwtimeio.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/05/16  17:40:24  jims
 * Port to Win32 DLL
 *
 * Revision 6.1  1994/04/15  19:07:45  vriezen
 * Move all files to 6.1
 *
 * Revision 2.13  1994/03/04  06:47:08  jims
 * #include rw/defs.h first to help GNU 2.5.8
 *
 * Revision 2.12  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.11  1993/04/06  17:08:07  myersn
 * use RWLocale::of(s) for operator >> formatting.
 *
 * Revision 2.10  1993/03/25  05:56:33  myersn
 * add zone argument to call of RWLocale::asString in RWTime::asString.
 *
 * Revision 2.9  1993/02/17  03:12:30  keffer
 * Changed const notation to follow style guide
 *
 * Revision 2.8  1993/02/04  20:24:20  jims
 * Added rwexport to operator<<(ostream&, RWTime&)
 *
 * Revision 2.7  1993/01/29  20:20:37  myersn
 * extend RWTime::asString() interface.
 *
 *
 * Revision 2.4  1992/11/27  21:01:11  myersn
 * added RWZone interface
 *
 * Revision 2.3  1992/11/20  02:55:42  myersn
 * adjustments for support of locales, time zones, and struct tm
 *
 * Revision 2.2  1992/11/15  21:51:11  keffer
 * Removed old V1.2 stream support
 *
 * Revision 2.1  1992/11/14  00:15:39  myersn
 * fold in interface to struct tm and RWLocale
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.9   29 May 1992 12:41:58   KEFFER
 * Ported to Glock under Unix.
 * 
 *    Rev 1.8   04 Mar 1992 10:26:56   KEFFER
 * Changed RWString to RWCString
 * 
 *    Rev 1.7   14 Nov 1991 10:08:12   keffer
 * 
 *    Rev 1.6   13 Nov 1991 11:49:06   keffer
 * Static variables now maintained by an instance manager
 * 
 *    Rev 1.5   05 Nov 1991 14:06:44   keffer
 * Removed RWXXXXErr macros --- now done by error package
 * 
 *    Rev 1.4   29 Oct 1991 14:00:04   keffer
 * Added military time (24 hour clock) print option.
 * 
 *    Rev 1.3   28 Oct 1991 09:24:28   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.2   09 Oct 1991 18:09:28   keffer
 * Ported to Zortech V3.0
 * 
 *    Rev 1.1   24 Sep 1991 18:51:30   keffer
 * msg scratch space now comes off the stack
 * 
 */

// including "rw/defs.h" first seems to help GNU 2.5.8 compile this module:
#include "rw/defs.h"      
#include "rw/rwtime.h"
#include "rw/cstring.h"
#include "rw/vstream.h"
#include "rw/rwfile.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: rwtimeio.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

RWCString
RWTime::asString(
  char format,
  const RWZone& zone,
  const RWLocale& locale) const
{
  struct tm tmbuf;
  extract(&tmbuf, zone);
  RWCString result;
  if (format == '\0') {
    result = locale.asString(&tmbuf, 'x', zone);
    result += " ";
    format = 'X';
  }
  result += locale.asString(&tmbuf, format, zone);
  return result;
}

// Typedefs are the easiest way to make more compilers happy
// about where the "rwexport" should be:
typedef ostream& ostreamRef;

ostreamRef rwexport
operator<<(ostream& s, const RWTime& t)
{
  s << t.asString('\0', RWZone::local(), RWLocale::of(s));
  return s;
}

void
RWTime::restoreFrom(RWvistream& s)
{
  s >> sec;
}

void
RWTime::restoreFrom(RWFile& file)
{
  file.Read(sec);
}

void
RWTime::saveOn(RWvostream& s) const
{
  s << sec;
}

void
RWTime::saveOn(RWFile& file) const
{
  file.Write(sec);
}
