/*
 * Definitions for the RWCTokenizer class
 *
 * $Id: ctoken.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
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
 ***************************************************************************
 *
 * $Log: ctoken.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/06/22  22:24:08  vriezen
 * Bug# 553.  Use value semantics on constructor
 *
 * Revision 6.1  1994/04/15  19:06:13  vriezen
 * Move all files to 6.1
 *
 * Revision 1.8  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.7  1993/07/29  16:02:01  myersn
 * simplify logic.
 *
 * Revision 1.6  1993/07/29  07:34:19  myersn
 * tolerate nulls.
 *
 * Revision 1.5  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 1.4  1993/03/20  22:55:13  keffer
 * Eliminated int to unsigned conversion.
 *
 * Revision 1.3  1993/02/14  05:23:03  myersn
 * use size_t for indices, not int (and RW_NPOS rather than -1 for bad index).
 *
 * Revision 1.2  1993/02/11  01:50:34  myersn
 * tolerate nulls.
 *
 * Revision 1.1  1993/02/11  00:50:13  myersn
 * Initial revision
 *
 * Revision 2.2  1992/11/14  23:23:38  keffer
 * Now includes header file <string.h> (which used to be included
 * by "rw/cstring.h".
 *
 * Revision 2.1  1992/11/05  18:59:55  keffer
 * Removed ^Z from end-of-file.
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.3   04 Mar 1992 09:17:30   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.2   28 Oct 1991 09:24:32   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.1   22 Aug 1991 15:04:48   keffer
 * operator()() no longer takes a default argument, as per the ARM.
 * 
 *    Rev 1.0   28 Jul 1991 08:39:52   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/ctoken.h"
#include <string.h>

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: ctoken.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

RWCTokenizer::RWCTokenizer(const RWCString& s) : theString(s), place(rwnil) 
{
}

RWCSubString
RWCTokenizer::operator()(const char* ws)
{
  RWPRECONDITION( ws!=0 );

  char const* eos = theString.data() + theString.length();

  if (place==rwnil)		// the first time through?
    place = theString.data();	// Initialize 'place'

  size_t extent = 0;
  while (1) {
    if (place >= eos) return theString(RW_NPOS,0);
    place += strspn(place, ws);
    extent = strcspn(place, ws);
    if (extent) break;
    ++place; // skip null
  }
  size_t start = place - theString.data();
  place += extent;		// Advance the placeholder

  return theString(start, extent);
}

RWCSubString
RWCTokenizer::operator()()
  { return operator()(" \t\n"); }
