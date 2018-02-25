/*
 * Definitions for RWLocale stream operations.
 *
 * $Id: lostream.cpp,v 6.7 1994/07/18 20:51:00 jims Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 * (503) 754-3010   Fax: (503) 757-6650  email: support@roguewave.com
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
 * $Log: lostream.cpp,v $
 * Revision 6.7  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.6  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.5  1994/06/16  00:02:14  vriezen
 * Fix typo
 *
 * Revision 6.4  1994/06/15  23:34:21  vriezen
 * Remove static init code.
 *
 * Revision 6.3  1994/06/15  21:13:01  vriezen
 * Remove the LAST (?) static init code from Tools--other than RWFactory related
 * items.  (localeIndex)
 *
 * Revision 6.2  1994/06/02  18:41:01  foote
 * Port to Symantec 7.0 on Macintosh
 *
 * Revision 6.1  1994/04/15  19:07:02  vriezen
 * Move all files to 6.1
 *
 * Revision 1.7  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.6  1993/08/06  04:32:21  randall
 * removed use of RW_CAFE_ALPHA.
 *
 * Revision 1.5  1993/04/15  02:14:05  myersn
 * add RWLocale::unimbue(ios&), guard against entire lack of ios::xalloc().
 *
 * Revision 1.5  1993/04/15  02:06:44  myersn
 * add RWLocale::unimbue(), guard against entire lack of ios::xalloc() support.
 *
 * Revision 1.4  1993/04/12  21:54:31  myersn
 * add RW_IOS_XALLOC_BROKEN guards around use of ios::pword().
 *
 * Revision 1.3  1993/04/12  11:50:46  jims
 * Added work-around for Sun Cafe Alpha problem with MT and ios::xalloc init
 *
 * Revision 1.2  1993/04/09  23:19:29  myersn
 * declare RWLocale::imbue() const.
 *
// Revision 1.1  1993/04/06  17:10:55  myersn
// Initial revision
//
 */

#include <iostream.h>
#include "rw/locale.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: lostream.cpp,v $ $Revision: 6.7 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_IOS_XALLOC_BROKEN

// the magic cookie to pass to stream.pword():
static int       localeIndexValue;
static RWBoolean localeIndexSet;

static int localeIndex() {
  if (!localeIndexSet) {
    localeIndexValue = ios::xalloc();
    localeIndexSet = TRUE;
  }
  return localeIndexValue;
}


#endif /* RW_IOS_XALLOC_BROKEN */

const RWLocale&
RWLocale::of(ios& s)
{
#ifndef RW_IOS_XALLOC_BROKEN

  const RWLocale* loc = (const RWLocale*) s.pword(localeIndex());
  if (loc)
    return *loc;
  else
#endif
    return RWLocale::global();
}

#ifndef RW_IOS_XALLOC_BROKEN

const RWLocale*
RWLocale::imbue(ios& s) const
{
  void*& p = s.pword(localeIndex());
  const RWLocale* loc = (const RWLocale*) p;
  p = (void*) this;
  return loc;
}

const RWLocale*
RWLocale::unimbue(ios& s)
{
  void*& p = s.pword(localeIndex());
  const RWLocale* loc = (const RWLocale*) p;
  p = 0;
  return loc;
}

#endif /* RW_IOS_XALLOC_BROKEN */
