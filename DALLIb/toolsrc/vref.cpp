
/*
 * RWVirtualRef: definitions
 *
 * $Id: vref.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: vref.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:08:04  vriezen
 * Move all files to 6.1
 *
 * Revision 2.8  1993/11/09  09:37:36  jims
 * Port to ObjectStore
 *
 * Revision 2.7  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.6  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.5  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.4  1993/04/12  12:16:09  jims
 * Removed mutex_init and mutex_destroy (now done by ctor and dtor)
 *
 * Revision 2.3  1993/01/29  20:07:31  myersn
 * make MT-safe via different interface to RWReference base class
 *
 * Revision 2.1  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.6   09 Sep 1992 19:38:34   KEFFER
 * Modifed to reflect new RWReference starting reference count.
 * 
 *    Rev 1.5   04 Jun 1992 14:49:08   KEFFER
 * ported to Glock under Unix.
 * 
 *    Rev 1.4   30 May 1992 14:57:16   KEFFER
 * Extra type casts to suppress warnings.
 * 
 *    Rev 1.3   29 May 1992 09:47:00   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.2   28 May 1992 16:24:16   KEFFER
 * Introduced RWPRECONDITION2 macro
 * 
 *    Rev 1.1   27 May 1992 18:09:14   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.0   11 Mar 1992 14:04:54   KEFFER
 * Initial revision.
 */

#include "rw/vref.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: vref.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

RWVirtualRef::RWVirtualRef(long length, size_t elementSize, RWVirtualPageHeap* heap)
:  RWReference(1)
,  length_(length)
,   baseIndex_(0)
,   myHeap_(heap)
,   handles_(rwnil)
{
  nPerPage_ = heap->pageSize() / elementSize;
  init();
}

RWVirtualRef::RWVirtualRef(const RWVirtualRef& v)
  :  RWReference(1),
     length_(v.length_),
     baseIndex_(0),
     nPerPage_(v.nPerPage_),
     myHeap_(v.myHeap_),
     handles_(rwnil)
{
  init();
}

RWVirtualRef::~RWVirtualRef()
{
  RWPRECONDITION2(references()==0, "RWVirtualRef::~RWVirtualRef(): Destructor called with ref. count != 0");
  for(RWPageSlot i=0; i<nSlots_; i++){
    myHeap_->deallocate(handles_[i]);
  }
  delete handles_;
}

void
RWVirtualRef::augmentLeft(long amount)
{
  RWPageSlot extraSlots = (RWPageSlot)((amount + nPerPage_ - 1) / nPerPage_);
  RWPageSlot newSlots = nSlots_ + extraSlots;
  RWHandle* newHandles = new RWHandle[newSlots];
  for(RWPageSlot i=0; i<extraSlots; i++)
    newHandles[i] = myHeap_->allocate();
  for(i=extraSlots; i<newSlots; i++)
    newHandles[i] = handles_[i-extraSlots];
  delete handles_;
  handles_ = newHandles;
  nSlots_ = newSlots;
  baseIndex_ += extraSlots * nPerPage_;
}

void
RWVirtualRef::augmentRight(long amount)
{
  RWPageSlot extraSlots = (RWPageSlot)((amount + nPerPage_ - 1) / nPerPage_);
  RWPageSlot newSlots = nSlots_ + extraSlots;
  RWHandle* newHandles = new RWHandle[newSlots];
  for (RWPageSlot i=0; i<nSlots_; i++)
    newHandles[i] = handles_[i];
  for (i=nSlots_; i<newSlots; i++)
    newHandles[i] = myHeap_->allocate();
  delete handles_;
  handles_ = newHandles;
  nSlots_ = newSlots;
}

/*
 * Find the slot and subscript within that slot where a given index
 * can be found.
 */
void
RWVirtualRef::findLocation(long idx, RWPageSlot& slot, unsigned& rem) const
{
  if (idx<0 || idx >= length_)
  {
    RWTHROW( RWBoundsErr(RWMessage( RWTOOL_LONGINDEX,
				    (long)idx,
				    (long)(length_-1) ) ));
  }
  slot = (RWPageSlot)((idx + baseIndex_) / nPerPage_);
  rem  =   (unsigned)((idx + baseIndex_) % nPerPage_);
}

void
RWVirtualRef::init()
{
  nSlots_ = (RWPageSlot)((length_ + nPerPage_ - 1) / nPerPage_);

  handles_ = new RWHandle[nSlots_];

  for(RWPageSlot i=0; i<nSlots_; i++)
    handles_[i] = myHeap_->allocate();	// Allocate a page for each slot
}

void 
RWVirtualRef::slide(long start, long delta)
{
  if (delta==0) return;

  if (start + delta == 0L) {
    // PRUNE LEFT:
    // In this case, the vector is being slid to the left such
    // that the element at index "start" will become index zero.
    // Just change the base index.
    baseIndex_ += start;
  }

  else if (start==0) {
    // PREPEND:
    // In this case, the vector is being augmented on the left.
#ifdef RWDEBUG
    assert( delta>=0 );
#endif
    if (delta>baseIndex_)
      augmentLeft(delta-baseIndex_);	// Add space if there isn't enough
    baseIndex_ -= delta;
  }

  else if (start==length()) {

    if (delta>0) {
      // APPEND:
      // In this case, the vector is being augmented on the right.
      if (length()+delta > capacity())
        augmentRight(length()+delta-capacity());
    }
    else {
      // PRUNE RIGHT:
      // In this case, the end of the vector is being trimmed.
      // Just change the length which will add excess capacity.
    }

  }

  else {

    // Oh dear.  They're not asking for anything simple.  Resize, then
    // pass the request on to the specializing class.
    long N = length_-start;
    if(delta>0) {augmentRight(delta); length_ += delta; }
    conformalCopy(start+delta, *this, start, N);
    if(delta<0) length_ += delta;
    return;
  }
  length_ += delta;
}

void
RWVirtualRef::setSlice(long start1, long extent1,
                           RWVirtualRef& v, long start2, long extent2)
{
  long delta = extent2-extent1;
  slide(start1+extent1, delta);
  conformalCopy(start1, v, start2, extent2);
}

void
RWVirtualRef::reshape(long newLength)
{
  slide(length_, newLength-length_);
}
