/*
 * Definitions for the cache manager RWCacheManager.
 *
 * $Id: cacheman.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: cacheman.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:05:45  vriezen
 * Move all files to 6.1
 *
 * Revision 2.6  1994/02/15  05:56:04  jims
 * No longer assume that size_t is same size as unsigned
 *
 * Revision 2.5  1993/12/31  21:40:39  griswolf
 * corrected destructor's delete usage
 *
 * Revision 2.4  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.3  1993/03/23  02:21:40  keffer
 * Eliminated int to unsigned conversions.
 *
 * Revision 2.2  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.1  1992/10/31  23:46:03  keffer
 * Check for BSD changed to RW_NON_ANSI_HEADERS
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.4   29 May 1992 09:46:40   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.3   27 May 1992 18:08:58   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.2   04 Mar 1992 09:17:18   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.1   28 Oct 1991 09:23:52   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 08:38:50   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/cacheman.h"
#include "rw/rwfile.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: cacheman.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

STARTWRAP
#include <sys/types.h>
#include <fcntl.h>

#if defined(RW_NON_ANSI_HEADERS)
#  include <memory.h>		/* Looking for memcpy() */
#else
#  include <string.h>
#endif
ENDWRAP

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

/*
 * Construct a cache manager for blocks of size blocksz and mxblks buffers
 */
RWCacheManager::RWCacheManager(RWFile* file, unsigned blocksz, unsigned mxblks)
{
  theFile_ = file;
  maxblocks_ = mxblks;
  nused_ = 0;
  blocksize_ = blocksz;
  buff_ = new char[blocksize_*maxblocks_];
  diskAddrs_ = new RWoffset[maxblocks_];
  useCounts_ = new unsigned[maxblocks_];
}

RWCacheManager::~RWCacheManager()
{
  flush();
  RWVECTOR_DELETE(maxblocks_) useCounts_;
  RWVECTOR_DELETE(maxblocks_) diskAddrs_;
  RWVECTOR_DELETE(blocksize_*maxblocks_) buff_;
}

RWBoolean
RWCacheManager::flush()
{
  // Don't flush if openend in RO mode.
  if(theFile_->isReadOnly())
    return TRUE;
  
  // Because we use "write through", this is all that's necessary:
  return theFile_->Flush();
}

void
RWCacheManager::invalidate()
{
  nused_ = 0;
}

RWBoolean
RWCacheManager::read(RWoffset locn, void* dat)
{
  RWPRECONDITION( dat!=rwnil );
  size_t islot = ageAndFindSlot(locn);

  if(islot == RW_NPOS){
    /*
     * Not in buffer;  we'll have to read it in from disk.
     * Get a free slot.
     */
    if( (islot = getFreeSlot()) == RW_NPOS ) return FALSE;
    diskAddrs_[islot] = locn;
    if( !theFile_->SeekTo(locn) || 
        !theFile_->Read( buff_+islot*blocksize_, blocksize_ ) )
      return FALSE;
  }
  useCounts_[islot] = 0;
  memcpy(dat, buff_+islot*blocksize_, blocksize_);
  return TRUE;
}

RWBoolean
RWCacheManager::write(RWoffset locn, void* dat)
{
  RWPRECONDITION( dat!=rwnil );
  size_t islot = ageAndFindSlot(locn);

  if(islot == RW_NPOS){
    /*
     * Not in buffer; find a free slot.
     */
    if( (islot = getFreeSlot()) == RW_NPOS ) return FALSE;
    diskAddrs_[islot] = locn;
  }

  useCounts_[islot] = 0;
  memcpy(buff_+islot*blocksize_, dat, blocksize_);

  // Don't write if openend in RO mode.
  if(theFile_->isReadOnly())
    return TRUE;
  else
    return theFile_->SeekTo(locn) &&
      theFile_->Write( buff_+islot*blocksize_, blocksize_ );
}

/************************************************
 *		Private functions		*
 ************************************************/
size_t
RWCacheManager::ageAndFindSlot(RWoffset locn)
{
  size_t islot = RW_NPOS;
  for(register unsigned i = 0; i<nused_; i++){
    if(diskAddrs_[i] == locn) islot = i;
    useCounts_[i]++;		// Age the blocks
  }
  return islot;
}

RWBoolean
RWCacheManager::flush(unsigned)
{
  // Don't flush if openend in RO mode.
  if(theFile_->isReadOnly())
    return TRUE;
  
  // Because we use "write through", this is all that's necessary:
  return theFile_->Flush();
}

size_t
RWCacheManager::getFreeSlot()
{
  size_t islot;

  if(nused_ < maxblocks_){
    islot = nused_++;		// Found an unused slot.
  }
  else {
    // No free slots; get the Least Recently Used block
    islot = LRU();
    if( !flush(islot) ) islot = RW_NPOS;
  }
  return islot;
}

size_t
RWCacheManager::LRU() const
{
  size_t islot = 0;
  unsigned maxCount = useCounts_[0];
  for(register unsigned i=1; i<nused_; i++){
    if(useCounts_[i] > maxCount){ maxCount=useCounts_[islot=i]; }
  }
  return islot;
}
