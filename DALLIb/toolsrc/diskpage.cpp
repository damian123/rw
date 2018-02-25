/*
 * RWDiskPageHeap definitions
 *
 * $Id: diskpage.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: diskpage.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:06:26  vriezen
 * Move all files to 6.1
 *
 * Revision 2.8  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.7  1993/08/20  19:47:18  keffer
 * Initialized unused handleMap_ values to keep Purify happy.
 *
 * Revision 2.6  1993/08/07  16:32:46  keffer
 * Destructor now checks for valid file pointer before fclose().
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
 *    Rev 1.4   29 May 1992 09:46:42   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.3   28 May 1992 16:24:12   KEFFER
 * Introduced RWPRECONDITION2 macro
 * 
 *    Rev 1.2   27 May 1992 18:09:00   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.1   19 May 1992 15:28:32   KEFFER
 * Increased /lib/cpp portability.
 * 
 *    Rev 1.0   11 Mar 1992 14:04:52   KEFFER
 * Initial revision.
 */

#include "rw/diskpage.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"
STARTWRAP
#include <stdio.h>
ENDWRAP

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: diskpage.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

const unsigned RWDiskPageHeap::initialPages_    = 128;
const unsigned RWDiskPageHeap::initialHandles_  = 128;
const unsigned RWDiskPageHeap::pageIncrement_   = 128;
const unsigned RWDiskPageHeap::handleIncrement_ = 128;

#ifdef RW_CRLF_CONVENTION
# define ACCESS "wb+"
#else
# define ACCESS "w+"
#endif

RWDiskPageHeap::RWDiskPageHeap(const char* filename, unsigned nbufs, unsigned pgsize) : 
  RWBufferedPageHeap(pgsize, nbufs),
  freePageMap_(initialPages_, FALSE),
  handleMap_(rwnil),
  handleStatus_(rwnil),
  nHandles_(initialHandles_),
  tempfp_(0)
{
  handleMap_ = new RWHandle[nHandles_];
  handleStatus_ = new HandleStatus[nHandles_];
  for(unsigned i=0; i<nHandles_; i++)
  {
    handleStatus_[i] = NotUsed;
    handleMap_[i] = 0;		// Initialize all slots to keep Purify happy
  }
  tempfp_ = filename ? fopen(filename, ACCESS) : tmpfile();
}

RWDiskPageHeap::~RWDiskPageHeap()
{
#ifdef RWDEBUG
  for(unsigned i=0; i<nHandles_; i++){
    if (handleStatus_[i] != NotUsed ) {
      RWTHROW(RWInternalErr(RWMessage(RWTOOL_ALLOCOUT, "RWDiskPageHeap")));
    }
  }
#endif
  if (tempfp_) fclose(tempfp_);
  delete handleStatus_;
  delete handleMap_;
}

RWHandle
RWDiskPageHeap::allocate()
{
  for (register unsigned slot=0; slot<nHandles_; slot++){
    if (handleStatus_[slot]==NotUsed) goto FoundSlot;
  }

  // Extend the handle maps ---
  resize(nHandles_+handleIncrement_);
  // (slot will be equal to the old nHandles_)

FoundSlot:

  handleStatus_[slot] = NoSwapSpace;
  return slot+1;		// NB: Add one: handle zero is never valid
}

void
RWDiskPageHeap::deallocate(RWHandle h)
{
  if (h==0) return;
  RWPRECONDITION2(handleValid(h), "RWDiskPageHeap::deallocate(RWHandle): Invalid handle");
  unsigned slot = h-1;

  if (handleStatus_[slot]==HasSwapSpace)
    freePageMap_.clearBit(handleMap_[slot]);

  handleStatus_[slot] = NotUsed;
  RWBufferedPageHeap::deallocate(h);
}

/************************************************
 *						*
 *		PROTECTED FUNCTIONS		*
 *						*
 ************************************************/

RWBoolean
RWDiskPageHeap::allocateDiskPage(RWHandle h)
{
  unsigned slot = h-1;
  RWPRECONDITION(handleStatus_[slot]==NoSwapSpace);

  unsigned ids;		// Disk slot.  This is the "slot" the page will occupy on disk
  unsigned maxSlot = freePageMap_.length();

  for (ids=0; ids<maxSlot; ids++){
    if (!freePageMap_.testBit(ids)) goto Free;
  }
  // No page is free.  Have to extend the bit map:
  freePageMap_.resize(maxSlot + pageIncrement_);
  ids = maxSlot;

Free:

  freePageMap_.setBit(ids);		// Mark the slot as in use
  handleMap_[slot]    = ids;		// Record it in the handle map
  handleStatus_[slot] = HasSwapSpace;	// Record status
  return TRUE;
}

RWBoolean
RWDiskPageHeap::handleValid(RWHandle h)
{
  if (h==0) return TRUE;
  unsigned slot = h-1;
  return slot < nHandles_ && handleStatus_[slot] != NotUsed;
}

RWoffset
RWDiskPageHeap::offsetOfHandle(RWHandle h)
{
  return (long)( (long)handleMap_[h-1] * (long)pageSize() );
}

RWBoolean
RWDiskPageHeap::swapIn(RWHandle h, void* p)
{
  RWPRECONDITION2(handleValid(h), "RWDiskPageHeap::swapIn(RWHandle,void*): Invalid handle");

  // If this page has never been swapped out, but the page buffer
  // is asking for it, then it has never been used.  Just return.
  if (handleStatus_[h-1]==NoSwapSpace) return TRUE;

  // Get the location of this page; seek to it and read.
  RWoffset offset = offsetOfHandle(h);
  if (fseek(tempfp_, offset, 0)) return FALSE;
  return fread((char*)p, pageSize(), 1, tempfp_) == pageSize();
}

RWBoolean
RWDiskPageHeap::swapOut(RWHandle h, void* p)
{
  RWPRECONDITION2(handleValid(h), "RWDiskPageHeap::swapOut(RWHandle,void*): Invalid handle");

  // Make sure this handle has a swap space
  if (handleStatus_[h-1]==NoSwapSpace)
    if (allocateDiskPage(h)==FALSE)
      return FALSE;

  // Find the location of the swap space for this handle:
  RWoffset offset = offsetOfHandle(h);

  // Seek and write it out:
  if (fseek(tempfp_, offset, 0)) return FALSE;
  return fwrite((char*)p, pageSize(), 1, tempfp_) == pageSize();
}

void
RWDiskPageHeap::resize(unsigned newN)
{
  unsigned* newHandleMap = new RWHandle[newN];
  HandleStatus* newHandleStatus = new HandleStatus[newN];

  for(register unsigned i=0; i<nHandles_; i++){
    newHandleMap[i] = handleMap_[i];		// Copy old info
    newHandleStatus[i] = handleStatus_[i];
  }
  for(i=nHandles_; i<newN; i++)			// Initialize new info
  {
    newHandleStatus[i] = NotUsed;
    newHandleMap[i] = 0;	// Keep Purify happy
  }
  delete handleStatus_;
  delete handleMap_;
  handleStatus_ = newHandleStatus;
  handleMap_ = newHandleMap;
  nHandles_ = newN;
}
