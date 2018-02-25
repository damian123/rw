/*
 * Definitions for RWCollectableAssociation
 *
 * $Id: ctass.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
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
 * $Log: ctass.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/06/18  02:55:51  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.1  1994/04/15  19:06:02  vriezen
 * Move all files to 6.1
 *
 * Revision 2.5  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.4  1993/07/03  23:55:58  keffer
 * Simplified and improved the clearAndDestroy() algorithm.
 *
 * Revision 2.3  1993/05/18  16:15:20  dealys
 * merged with ctassdf.cpp
 *
 * Revision 2.2  1993/03/17  21:21:15  keffer
 * Return type of binaryStoreSize is now RWspace
 *
 * Revision 2.1  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.9   17 Jul 1992 11:06:46   KEFFER
 * Fixed error in binaryStoreSize calculation.
 * 
 *    Rev 1.8   29 May 1992 09:46:40   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.7   28 May 1992 15:35:30   KEFFER
 * Introduced RWhashAddress() for identity hashes.
 * 
 *    Rev 1.6   27 May 1992 18:08:58   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.5   25 May 1992 15:28:56   KEFFER
 * One less function call in RWCollectableIDAssociation::hash().
 * 
 *    Rev 1.4   29 Apr 1992 14:50:50   KEFFER
 * Hashing now uses chaining to resolve collisions
 * 
 *    Rev 1.3   04 Mar 1992 09:17:20   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.2   28 Oct 1991 09:23:54   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 08:38:52   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/collass.h"
#include "defcol.h"      

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: ctass.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

RWDEFINE_COLLECTABLE2(RWCollectableAssociation,   __RWCOLLECTABLEASSOCIATION)
RWDEFINE_COLLECTABLE2(RWCollectableIDAssociation, __RWCOLLECTABLEIDASSOCIATION)

RWCollectableAssociation::RWCollectableAssociation() :
  ky(rwnil),
  val(rwnil)
{
}
 
RWCollectableAssociation::~RWCollectableAssociation() {}

RWCollectableIDAssociation::RWCollectableIDAssociation() :
    RWCollectableAssociation()
{
}
                           
RWCollectableIDAssociation::~RWCollectableIDAssociation() {}

RWCollectable*
RWCollectableAssociation::value(RWCollectable* newValue)
{
  register RWCollectable* temp = val;
  val = newValue;
  return temp;
}

RWspace
RWCollectableAssociation::binaryStoreSize() const
{
  return ky->recursiveStoreSize() + val->recursiveStoreSize();
}

int
RWCollectableAssociation::compareTo(const RWCollectable* a) const
{
  RWPRECONDITION( a!=rwnil );
  return -a->compareTo(ky);
}

unsigned
RWCollectableAssociation::hash() const
{
  return ky->hash();
}

RWBoolean
RWCollectableAssociation::isEqual(const RWCollectable* a) const
{
  RWPRECONDITION( a!=rwnil );
  return a->isEqual(ky);
}

/***************** RWCollectableIDAssociation members ****************/

unsigned
RWCollectableIDAssociation::hash() const
{
  return RWhashAddress((void*)ky);
}

int
RWCollectableIDAssociation::compareTo(const RWCollectable* a) const
{
   return (key() < a) ? -1 : (key() == a) ? 0 : 1;
}

RWBoolean
RWCollectableIDAssociation::isEqual(const RWCollectable* a) const
{
  RWPRECONDITION( a!=rwnil );
  if(a->isA() == __RWCOLLECTABLEIDASSOCIATION)
     return ((RWCollectableIDAssociation*)a)->key() == ky;
  else
     return a == ky;
}

