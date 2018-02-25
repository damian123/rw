/*
 * Definitions for the abstract base class RWCollection
 *
 * $Id: ctclass.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
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
 * It would be nice if this class could carry a default destructor that called
 * clear() of the deriving class, because nearly every collection class does so.
 * But, the language says that virtual functions in a destructor resolve to
 * the class itself, or farther down the DAG.  The net result is that
 * RWCollection::clear() would get called every time.  See E&S, p. 294.  
 *
 * The code for clearAndDestroy() is complicated by collections that allow
 * duplicates.  It must check to see if an object has already been deleted.
 * It does this by maintaining an IdentitySet of already deleted objects.
 *
 ***************************************************************************
 *
 * $Log: ctclass.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/06/20  19:15:01  nevis
 * Added out of line virtual dtor.
 *
 * Revision 6.1  1994/04/15  19:06:04  vriezen
 * Move all files to 6.1
 *
 * Revision 2.7  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.6  1993/09/09  02:39:32  keffer
 * Added member function copyContentsTo().
 *
 * Revision 2.5  1993/07/03  23:55:58  keffer
 * Simplified and improved the clearAndDestroy() algorithm.
 *
 * Revision 2.4  1993/05/18  00:47:20  keffer
 * Replaced RDEBUG check with an assertion.
 *
 * Revision 2.3  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.2  1993/03/17  21:21:15  keffer
 * Return type of binaryStoreSize is now RWspace
 *
 * Revision 2.1  1992/11/05  03:47:45  keffer
 * Removed ^Z from the end of file.
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.5   25 May 1992 15:30:52   KEFFER
 * CAPACITY and RESIZE are now non-const, allowing them to be changed.
 * shallowStoreSize -> recursiveStoreSize;
 * RWIdentitySet deadObjects optimized.
 * 
 *    Rev 1.4   04 Mar 1992 09:17:20   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.3   05 Nov 1991 14:05:46   keffer
 * Removed RWXXXXErr macros --- now done by error package
 * 
 *    Rev 1.2   28 Oct 1991 09:23:54   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.1   22 Aug 1991 10:56:36   keffer
 * RWCollectionTestStruct is no longer declared static
 * 
 *    Rev 1.0   28 Jul 1991 08:38:56   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/idenset.h"
#include <assert.h>

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: ctclass.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

// Default initial collection size (where applicable):
size_t RWCollection::DEFAULT_CAPACITY	= RWDEFAULT_CAPACITY;
size_t RWCollection::DEFAULT_RESIZE	= RWDEFAULT_RESIZE;

void
rwDestroy(RWCollectable* s, void*)
{
  delete s;
}

RWCollection::~RWCollection() {;}

void
RWCollection::clearAndDestroy()
{
  RWIdentitySet nukeSet(entries()/4); // Lambda of 4
  nukeSet += (*this);
  nukeSet.clearAndDestroy();	// All objects are now unique.  Nuke 'em

  clear();
}

RWBoolean		
RWCollection::contains(const RWCollectable* c) const
{
  return find(c) != rwnil;
}

void
RWCollection::removeAndDestroy(const RWCollectable* a)
{
  RWCollectable* c = remove(a);
  delete c;
}

static void
addBinaryStoreSize(RWCollectable* c, void* n)
{
  *(RWspace*)n += c->recursiveStoreSize();
}

RWspace
RWCollection::binaryStoreSize() const
{
  RWspace tot = sizeof(size_t);
  // Cast done to suppress unfounded "const" warnings:
  ((RWCollection*)this)->apply(addBinaryStoreSize, &tot);
  return tot;
}

static void
addItemTo(RWCollectable* item, void* x)
{
  ((RWCollection*)x)->insert(item);
}

static void
copyItemTo(RWCollectable* item, void* x)
{
  ((RWCollection*)x)->insert(item->copy());
}

static void
removeItemFrom(RWCollectable* item, void* x)
{
  ((RWCollection*)x)->remove(item);
}

void
RWCollection::operator+=(const RWCollection& col)
{
  ((RWCollection&)col).apply(::addItemTo, this);
}

void
RWCollection::operator-=(const RWCollection& col)
{
    ((RWCollection&)col).apply(::removeItemFrom, this);
}

void
RWCollection::addContentsTo(RWCollection* collection) const
{
  RWPRECONDITION( collection!=rwnil );
  // Cast necessary to suppress unfounded cfront warnings:
  ((RWCollection*)this)->apply(::addItemTo, collection);
}

void
RWCollection::copyContentsTo(RWCollection* collection) const
{
  RWPRECONDITION( collection!=rwnil );
  // Cast necessary to suppress unfounded cfront warnings:
  ((RWCollection*)this)->apply(::copyItemTo, collection);
}

struct RWCollectionTestStruct {
  RWtestCollectable	functionToApply;
  RWCollection*		collection;
  void*			clientData;
};

static void
addIfTrue(RWCollectable* c, void* capsule)
{
  register struct RWCollectionTestStruct* temp = (struct RWCollectionTestStruct*)capsule;
  if(temp->functionToApply(c, temp->clientData)) temp->collection->insert(c);
}
  
RWCollection*
RWCollection::select(RWtestCollectable tstFun, void* x) const
{
  RWPRECONDITION( tstFun!=0 );
  RWCollectionTestStruct capsule;

  capsule.clientData = x;
  capsule.functionToApply = tstFun;
  capsule.collection = (RWCollection*)newSpecies();

  ((RWCollection*)this)->apply(addIfTrue, &capsule);

  return capsule.collection;
}
