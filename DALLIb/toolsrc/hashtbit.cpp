
/*
 * Definitions for RWHashTableIterator.
 *
 * $Id: hashtbit.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: hashtbit.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:06:44  vriezen
 * Move all files to 6.1
 *
 * Revision 1.3  1993/11/09  08:37:30  jims
 * Port to ObjectStore
 *
 * Revision 1.2  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.1  1993/05/14  20:15:42  dealys
 * Initial revision
 *
 *
 */

#include "rw/hashtab.h"
#include "rw/slistcol.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: hashtbit.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

#define BUCKET         RWSlistCollectables  
#define BUCKETITERATOR RWSlistCollectablesIterator


/****************************************************************
 *								*
 *			RWHashTableIterator			*
 *								*
 ****************************************************************/


RWHashTableIterator::RWHashTableIterator(RWHashTable& h) :
 myHash_(&h),
 idx_(RW_NPOS),
 iterator_(rwnil)
{
  reset();
}

RWHashTableIterator::RWHashTableIterator(const RWHashTableIterator& h) :
 myHash_(h.myHash_),
 idx_(h.idx_)
{
  iterator_ = h.iterator_ ? new BUCKETITERATOR(*h.iterator_) : rwnil;
}

RWHashTableIterator::~RWHashTableIterator()
{
   delete iterator_;
}

RWHashTableIterator&
RWHashTableIterator::operator=(const RWHashTableIterator& h)
{
  if (this != &h)
  {
    delete iterator_;
    myHash_   = h.myHash_;
    idx_      = h.idx_;
    iterator_ = h.iterator_ ? new BUCKETITERATOR(*h.iterator_) : rwnil;
  }
  return *this;
}

RWCollectable*
RWHashTableIterator::operator()()
{
  RWCollectable* p=rwnil;
  while (iterator_ && (p=(*iterator_)())==rwnil )
    nextIterator();
  return p;
}

RWCollectable*
RWHashTableIterator::findNext(const RWCollectable* a)
{
  RWCollectable* p=rwnil;
  while (iterator_ && (p=iterator_->findNext(a))==rwnil )
    nextIterator();
  return p;
}

RWCollectable*
RWHashTableIterator::remove()
{
  RWCollectable* p = iterator_ ? iterator_->remove() : rwnil;
  if (p) --myHash_->nitems_;
  return p;
}

RWCollectable*
RWHashTableIterator::removeNext(const RWCollectable* a)
{
  RWCollectable* p=rwnil;
  while (iterator_ && (p=iterator_->removeNext(a))==rwnil )
    nextIterator();
  if (p) --myHash_->nitems_;
  return p;
}

void
RWHashTableIterator::reset()
{
  delete iterator_;
  iterator_ = rwnil;

  for (idx_=0; idx_ < myHash_->buckets(); idx_++)
  {
    if (myHash_->table_(idx_))
    {
      iterator_ = new BUCKETITERATOR(*myHash_->table_(idx_));
      return;
    }
  }
}

RWCollectable*
RWHashTableIterator::key() const
{
   return iterator_ ? iterator_->key() : rwnil;
}

void
RWHashTableIterator::nextIterator()
{
  delete iterator_;		// Invalidate old iterator
  iterator_ = rwnil;

  while (++idx_ < myHash_->buckets())
  {
    if (myHash_->table_(idx_))
    {
      iterator_ = new BUCKETITERATOR(*myHash_->table_(idx_));
      return;
    }
  }
}

