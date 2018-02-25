/*
 * Definitions for RWSet --- hashed table lookups.
 *
 * $Id: rwset.cpp,v 6.6 1994/07/18 20:51:00 jims Exp $
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
 * $Log: rwset.cpp,v $
 * Revision 6.6  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.5  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.4  1994/06/18  00:55:05  myersn
 * add explicit out-of-line dtor.
 *
 * Revision 6.3  1994/06/16  03:31:02  vriezen
 * Added RW_DEFINE_HOME and/or RW_DECLARE_HOME
 *
 * Revision 6.2  1994/04/15  21:08:12  vriezen
 * Backed out (until next release) RW_DEFINE_HOME and RW_DECLARE_HOME support.
 *
 * Revision 6.1  1994/04/15  19:07:39  vriezen
 * Move all files to 6.1
 *
 * Revision 2.6  1994/03/23  20:41:38  vriezen
 * Add RW_DEFINE_HOME macros for otherwise homeless classes.
 *
 * Revision 2.5  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.4  1993/05/18  16:37:03  dealys
 * merged with rwsetdf.cpp
 *
 * Revision 2.3  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.2  1993/03/24  23:49:27  keffer
 * Indexing operations now used unsigned
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.10   25 May 1992 15:35:50   KEFFER
 * Optimized, reducing size.
 * 
 *    Rev 1.9   29 Apr 1992 14:50:58   KEFFER
 * Hashing now uses chaining to resolve collisions
 * 
 */

#include "rw/rwset.h"
#include "rw/slistcol.h"
#include "defcol.h"
#include "rw/toolerr.h"   

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: rwset.cpp,v $ $Revision: 6.6 $ $Date: 1994/07/18 20:51:00 $");

RWDEFINE_COLLECTABLE2(RWSet, __RWSET)

// Constructor for hash table of specified size:
RWSet::RWSet(size_t N) : RWHashTable(N) { }

RWSet::~RWSet() {}
                          

/************* Canonical collection class functions ****************/

/*
 * Offer a special definition of clearAndDestroy() for Sets that can
 * optimize on the fact that duplicates are not allowed.
 */
void
RWSet::clearAndDestroy()
{
  apply (rwDestroy, 0);
  RWSet::clear();
}

/*
 * Add an item to this table.  If it is already in the
 * table, then return the old instance. 
 */
RWCollectable*
RWSet::insert(RWCollectable* a)
{
  RWCollectable* ret;
  size_t idx = hashIndex(a);
  if (table_(idx)==rwnil || (ret=table_(idx)->find(a))==rwnil) {
    insertIndex(idx, a);
    ret = a;
  }
  return ret;
}

/*
 * Override occurrencesOf() to something more efficient:
 */
size_t
RWSet::occurrencesOf(const RWCollectable* a) const
{
  return find(a) ? 1 : 0;
}

/************************ Special RWSet functions ***************************/

/* override operator<= for set since it is easier */
RWBoolean
RWSet::operator<=(const RWSet& h) const
{
  RWCollectable* p;
  RWSetIterator iterator(*(RWSet*)this);	// Cast away constness
  while ((p=iterator())!=rwnil) {
    if (contains(p) && !h.contains(p)) return FALSE;
  }
  return TRUE;
}

/************************ RWSet utilities ****************************/

RWBoolean
RWSet::isEqual(const RWCollectable* c) const
{
  if( c->isA() != __RWSET ) return FALSE;
  return RWSet::operator==(*(const RWSet*)c);
}

/********************* Conversion functions ****************************/

RWSet
RWCollection::asSet() const
{
  RWSet aSet( entries()/2 );	// Pick a lambda of 2.0
  aSet += *this;
  return aSet;
}

/********************* RWSetIterator functions ****************************/

RW_DEFINE_HOME(RWSetIterator)
