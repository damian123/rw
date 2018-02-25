/*
 * Definitions for RWCollectable Dates
 *
 * $Id: ctdate.cpp,v 6.5 1994/07/18 20:51:00 jims Exp $
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
 * $Log: ctdate.cpp,v $
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.3  1994/06/20  16:39:22  nevis
 * Fixed out of line virtual dtor.
 *
 * Revision 6.2  1994/06/18  02:55:51  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.1  1994/04/15  19:06:07  vriezen
 * Move all files to 6.1
 *
 * Revision 2.3  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.2  1993/05/18  16:18:49  dealys
 * merged with ctdatdf.cpp
 *
 * Revision 2.1  1993/04/07  02:59:02  myersn
 * eliminate use of member operator==
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.1   28 Oct 1991 09:23:56   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 08:38:58   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/colldate.h"
#include "defcol.h"  

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: ctdate.cpp,v $ $Revision: 6.5 $ $Date: 1994/07/18 20:51:00 $");

RWDEFINE_COLLECTABLE2(RWCollectableDate, __RWCOLLECTABLEDATE)

RWCollectableDate::RWCollectableDate() : RWDate() { }               
RWCollectableDate::~RWCollectableDate() { }

int
RWCollectableDate::compareTo(const RWCollectable* c) const
{
  return RWDate::compareTo( (const RWDate*)(const RWCollectableDate*)c );
}

unsigned
RWCollectableDate::hash() const
{
  return RWDate::hash();
}

RWBoolean
RWCollectableDate::isEqual(const RWCollectable* c) const
{
  if( c->isA() != RWCollectableDate::isA() ) return FALSE;
  return *this == *(const RWCollectableDate*)c;
}

