/*
 * RWDate I/O
 *
 * $Id: rwdateio.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
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
 * Parts of this code have been taken from "The NIH Class Library",
 * a public domain C++ class library written by Keith Gorlen, of the
 * National Institute of Health.
 *
 * $Log: rwdateio.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/05/16  17:39:28  jims
 * Port to Win32 DLL
 *
 * Revision 6.1  1994/04/15  19:07:34  vriezen
 * Move all files to 6.1
 *
 * Revision 2.14  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.13  1993/04/06  17:47:42  keffer
 * Made operator>>() exportable.
 *
 * Revision 2.12  1993/04/06  17:08:07  myersn
 * use RWLocale::of(s) for operator >> and << formatting.
 *
 * Revision 2.11  1993/02/17  03:12:30  keffer
 * Changed const notation to follow style guide
 *
 * Revision 2.10  1993/02/04  20:26:23  jims
 * Added rwexport to operator<<(ostream&, const RWDate&)
 *
 * Revision 2.9  1993/01/29  21:51:24  myersn
 * extern asString() interface.
 *
 * Revision 2.8  1993/01/26  21:21:16  myersn
 * remove redundant declaration, eliminate warning.
 *
 * Revision 2.7  1992/12/02  02:53:02  myersn
 * remove use of strftime because it fails for dates before 1970.
 *
 * Revision 2.6  1992/12/01  04:21:46  myersn
 * make parseFrom() use locale::stringToDate().
 *
 * Revision 2.5  1992/11/27  21:01:11  myersn
 * added RWZone interface
 *
 * Revision 2.4  1992/11/20  02:55:42  myersn
 * adjustments for support of locales, time zones, and struct tm
 *
 * Revision 2.3  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
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
 *    Rev 1.8   07 Jun 1992 13:14:26   KEFFER
 * Ported to Liant; required adding RWDate:: scope to howToPrint.
 * 
 *    Rev 1.7   29 May 1992 12:44:44   KEFFER
 * Introduced CPP_ANSI_RECURSION macro; ported to Glock under Unix.
 * 
 *    Rev 1.6   27 May 1992 18:09:08   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.5   04 Mar 1992 10:26:54   KEFFER
 * Changed RWString to RWCString
 * 
 *    Rev 1.4   13 Nov 1991 11:49:08   keffer
 * Static variables now maintained by an instance manager
 * 
 *    Rev 1.3   29 Oct 1991 13:58:56   keffer
 * Improved error handling.  Removed static buffers.
 * 
 *    Rev 1.2   28 Oct 1991 09:24:24   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.1   09 Oct 1991 18:09:26   keffer
 * Ported to Zortech V3.0
 * 
 */

#include "rw/rwdate.h"
#include "rw/cstring.h"
#include "rw/vstream.h"
#include "rw/rwfile.h"
#include "rw/locale.h"
STARTWRAP
#include <ctype.h>
ENDWRAP

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: rwdateio.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

RWCString
RWDate::asString(
  char format,
  const RWLocale& locale) const
{
  struct tm tmbuf;
  extract(&tmbuf);
  return locale.asString(&tmbuf, format);
}

// Typedefs are the easiest way to make more compilers happy
// about where the "rwexport" should be:
typedef ostream& ostreamRef;
typedef istream& istreamRef;

istreamRef rwexport
operator>>(istream& s, RWDate& d)
{
  d.parseFrom(s, RWLocale::of(s));
  return s;
}

void
RWDate::parseFrom(istream& s, const RWLocale& locale)
{
  RWCString dateStr;
  dateStr.readLine(s);
  struct tm tmbuf;
  if (!locale.stringToDate(dateStr, &tmbuf)) julnum = 0;
  else julnum = RWDate(&tmbuf).julnum;
}

ostreamRef rwexport
operator<<(ostream& s, const RWDate& d)
{
  s << d.asString('x', RWLocale::of(s));
  return s;
}

void
RWDate::saveOn(RWvostream& s) const
{
  s << julnum;
}

void
RWDate::saveOn(RWFile& file) const
{
  file.Write(julnum);
}

void
RWDate::restoreFrom(RWvistream& s) 
{
  s >> julnum;
}

void
RWDate::restoreFrom(RWFile& file) 
{
  file.Read(julnum);
}
