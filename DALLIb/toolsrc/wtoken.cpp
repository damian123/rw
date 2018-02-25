/*
 * Definitions for the RWWTokenizer class
 *
 * $Id: wtoken.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: wtoken.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:08:11  vriezen
 * Move all files to 6.1
 *
 * Revision 1.11  1993/09/10  20:05:33  keffer
 * Repositioned RW_RCSID macro.
 *
 * Revision 1.10  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.9  1993/08/09  03:55:22  myersn
 * tolerate nulls.
 *
 * Revision 1.8  1993/05/29  18:48:24  keffer
 * Renamed or added a dummy variable in case nothing gets compiled
 *
 * Revision 1.7  1993/05/24  19:12:22  keffer
 * Ported to xlC compiler.
 *
 * Revision 1.6  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 1.5  1993/03/22  23:12:26  keffer
 * Make indices unsigned instead of int.
 *
 * Revision 1.4  1993/02/16  19:11:39  myersn
 * install #ifndef RW_NO_WSTR protection.
 *
 * Revision 1.2  1993/02/11  20:50:52  myersn
 * change W" \t\n" to {(wchar_t)' ',(wchar_t'\t',...}; compilers still broken.
 *
 * Revision 1.1  1993/02/11  02:02:04  myersn
 * Initial revision
 *
 */

#include "rw/compiler.h"
#ifndef RW_NO_WSTR

#include "rw/wtoken.h"
#include "rwwchar.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: wtoken.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

RWWTokenizer::RWWTokenizer(const RWWString& s)
{
  theString = &s;
  place = rwnil;
}

RWWSubString
RWWTokenizer::operator()(const wchar_t* ws)
{
  RWPRECONDITION( ws!=0 );
  wchar_t const* eos = theString->data() + theString->length();

  // Check to see if this the first time through...
  if (place==rwnil)
    place = theString->data();	// Initialize 'place'

  size_t extent = 0;
  while (1) {
    if (place >= eos) return (*theString)(RW_NPOS,0);
    place += wcsspn(place, ws);
    extent = wcscspn(place, ws);
    if (extent) break;
    ++place; // skip null
  }
  size_t start = place - theString->data();
  place += extent;		// Advance the placeholder

  return (*theString)(start, extent);
}

RWWSubString
RWWTokenizer::operator()()
{
  // Sun CC 3.0 doesn't support W" \t\n".
  static const wchar_t white[4] =
    { (wchar_t)' ', (wchar_t)'\t', (wchar_t)'\n', 0 };
  return operator()(white);
}

#else
// This is to quiet fussy libraries if nothing gets compiled:
int rwDummy_wtoken_cpp;
#endif /* RW_NO_WSTR */
