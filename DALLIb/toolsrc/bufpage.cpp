/*
 * RWBufferedPageHeap definitions
 *
 * $Id: bufpage.cpp,v 6.5 1994/07/18 20:51:00 jims Exp $
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
 * $Log: bufpage.cpp,v $
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.3  1994/06/16  03:31:02  vriezen
 * Added RW_DEFINE_HOME and/or RW_DECLARE_HOME
 *
 * Revision 6.2  1994/04/15  21:08:12  vriezen
 * Backed out (until next release) RW_DEFINE_HOME and RW_DECLARE_HOME support.
 *
 * Revision 6.1  1994/04/15  19:05:41  vriezen
 * Move all files to 6.1
 *
 * Revision 2.10  1994/03/31  21:55:53  vriezen
 * Add RW_DEFINE_HOME macros for otherwise homeless classes.
 *
 * Revision 2.9  1994/02/15  06:02:38  jims
 * Port to cxx under OSF on DEC Alpha
 *
 * Revision 2.8  1994/02/15  05:54:14  jims
 * No longer assume size_t is same size as unsigned
 *
 * Revision 2.7  1993/11/09  05:27:54  jims
 * Port to ObjectStore
 *
 * Revision 2.6  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.5  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.4  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.3  1993/03/24  23:49:27  keffer
 * Indexing operations now used unsigned
 *
 * Revision 2.1  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.4   29 May 1992 09:46:40   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.3   28 May 1992 17:31:26   KEFFER
 * NULL -> rwnil.
 * 
 *    Rev 1.2   28 May 1992 16:24:12   KEFFER
 * Introduced RWPRECONDITION2 macro
 * 
 *    Rev 1.1   27 May 1992 18:08:56   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.0   11 Mar 1992 14:04:52   KEFFER
 * Initial revision.
 */

#include "rw/bufpage.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: bufpage.cpp,v $ $Revision: 6.5 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

RWBufferedPageHeap::RWBufferedPageHeap(unsigned pgsize, unsigned nbufs) :
  RWVirtualPageHeap(pgsize),
  nBuffers_(nbufs)
{
  RWPRECONDITION2(nbufs, "RWBufferedPageHeap::RWBufferedPageHeap(): Number of buffers must be greater than zero");
  RWPRECONDITION2(pgsize,"RWBufferedPageHeap::RWBufferedPageHeap(): Page size must be greater than zero");

  buffers_    = new RWvoid[nbufs];
  handles_    = new RWHandle[nbufs];
  lockCounts_ = new short[nbufs];
  age_        = new unsigned[nbufs];
  dirty_      = new RWBoolean[nbufs];

  if (buffers_ && handles_ && lockCounts_ && age_) {
    for (unsigned i=0; i<nbufs; i++) {
      // Here's where the actual buffer space is allocated:
      buffers_[i] = new char[pgsize];	
      if (!buffers_[i]) {
        nBuffers_ = i;		// If no memory, reduce the number of buffers
	break;
      }
      handles_[i]    = 0;	// Zero handle is never valid; use to mark an unused slot
      lockCounts_[i] = 0;	// Number of outstanding locks
      age_[i]        = 0;	// Age in use counts
      dirty_[i]      = FALSE;	// Initially, buffer is not dirty; avoids initial swap
    }
  }
  else {
    delete dirty_;
    delete age_;
    delete lockCounts_;
    delete handles_;
    delete buffers_;
    buffers_ = rwnil;
  }
}

RWBufferedPageHeap::~RWBufferedPageHeap()
{
  for (unsigned i=0; i<nBuffers_; i++) {
    if (lockCounts_[i] > 0) {
      RWTHROW(RWInternalErr(RWMessage(RWTOOL_LOCK)));
    }
  }

  if (buffers_) {
    for (unsigned j=0; j<nBuffers_; j++) {
      delete buffers_[j];
    }
  }
  delete dirty_;
  delete age_;
  delete lockCounts_;
  delete handles_;
  delete buffers_;
}

/*
 * This function gives us a chance to purge the given handle from
 * the buffers.
 */
void
RWBufferedPageHeap::deallocate(RWHandle h)
{
  size_t islot = findHandle(h);
  if (islot!=RW_NPOS) handles_[islot] = 0;
}

/*
 * Mark the indicated page as "dirty".  This requires first finding it,
 * then marking it.  If the slot it occupies is then ever needed, the 
 * buffer will be "swapped out" before being recycled.
 */

void
RWBufferedPageHeap::dirty(RWHandle h)
{
  RWPRECONDITION2(isValid(), "RWBufferedPageHeap::dirty(RWHandle): RWBufferedPageHeap is invalid");
  RWPRECONDITION2(h, "RWBufferedPageHeap::dirty(RWHandle): Handle must be nonzero");
  size_t islot = findHandle(h);
  if (islot==RW_NPOS || lockCounts_[islot]==0) {
    RWTHROW(RWInternalErr(RWMessage(RWTOOL_UNLOCK)));
  }
  dirty_[islot] = TRUE;
}

/*
 * Lock the page with the indicated handle.
 */

void*
RWBufferedPageHeap::lock(RWHandle h)
{
  RWPRECONDITION2(isValid(), "RWBufferedPageHeap::lock(RWHandle): RWBufferedPageHeap is invalid");
  RWPRECONDITION2(h, "RWBufferedPageHeap::lock(RWHandle): Handle must be nonzero");

  size_t islot = ageAndFindHandle(h);
  if (islot == RW_NPOS) {
    if ( (islot = swapPageIn(h)) ==RW_NPOS ) return NULL;
  }
  lockCounts_[islot]++;
  age_[islot] = 0;
  return buffers_[islot];
}

/*
 * Unlock the given handle.  The handle must have already been locked.
 */

void
RWBufferedPageHeap::unlock(RWHandle h)
{
  RWPRECONDITION2(isValid(), "RWBufferedPageHeap::unlock(RWHandle): RWBufferedPageHeap is invalid");
  RWPRECONDITION2(h, "RWBufferedPageHeap::unlock(RWHandle): Handle must be nonzero");
  size_t islot = findHandle(h);
  if (islot==RW_NPOS || lockCounts_[islot]==0) {
    RWTHROW(RWInternalErr(RWMessage(RWTOOL_UNLOCK)));
  }
  lockCounts_[islot]--;
}

/************************************************
 *						*
 *		PROTECTED FUNCTIONS		*
 *						*
 ************************************************/

size_t
RWBufferedPageHeap::ageAndFindHandle(RWHandle h)
{
  size_t islot = RW_NPOS;
  for (register unsigned i=0; i<nBuffers_; i++){
    if (handles_[i]==h) islot = i;
    age_[i]++;		// Age all buffers
  }
  return islot;
}

size_t
RWBufferedPageHeap::findHandle(register RWHandle h)
{
  for (register unsigned islot=0; islot<nBuffers_; islot++)
    if (handles_[islot]==h) return islot;
  return RW_NPOS;
}

size_t
RWBufferedPageHeap::findUnusedSlot()
{
  for (register unsigned islot=0; islot<nBuffers_; islot++)
    if (handles_[islot]==0) return islot;
  return RW_NPOS;
}

size_t
RWBufferedPageHeap::swapPageIn(RWHandle h)
{
  size_t islot = findUnusedSlot();
  if (islot==RW_NPOS)  islot = swapOutLRUSlot();

  if (islot!=RW_NPOS) {
    swapIn(h, buffers_[islot]);		// Virtual function provided by specializing class
    handles_[islot]    = h;
    dirty_[islot]      = FALSE;
    lockCounts_[islot] = 0;
    age_[islot]        = 0;
  }
  return islot;
}

/*
 * Search for the least recently used slot.  If the buffer in it is
 * "dirty", then it should be swapped out before being used.
 *
 * Under some circumstances, this algorithm could be improved by 
 * preferring "clean" slots, even though they might not be as old
 * as some other "dirty" slots.  This would avoid the swap out.
 */

size_t
RWBufferedPageHeap::swapOutLRUSlot()
{
  if (nBuffers_==0) return RW_NPOS;

  size_t islot  = RW_NPOS;
  unsigned maxAge = 0;

  for (register unsigned i=0; i<nBuffers_; i++) {
    if (lockCounts_[i]==0 && age_[i] >= maxAge)
      {maxAge=age_[islot=i];}
  }

  if (islot!=RW_NPOS && dirty_[islot]) 
    swapOut(handles_[islot], buffers_[islot]);

  return islot;
}


RW_DEFINE_HOME(RWVirtualPageHeap)
