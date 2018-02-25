/*
 * Definitions for RWFileManager
 *
 * $Id: filemgr.cpp,v 6.6 1994/07/18 20:51:00 jims Exp $
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
 * Designed for backwards compatibility with old RWFileManager files.
 * The old version did allocations using type "unsigned".
 * The new version uses type "unsigned long".
 * Under DOS, the former is only 64k.
 * To provide backwards compatibility, RWFileManager looks at the node size
 * of old files then uses the appropriate manager.
 *
 ***************************************************************************
 *
 * $Log: filemgr.cpp,v $
 * Revision 6.6  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.5  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.4  1994/06/16  03:31:02  vriezen
 * Added RW_DEFINE_HOME and/or RW_DECLARE_HOME
 *
 * Revision 6.3  1994/05/16  17:31:07  jims
 * Allow file manager to be used on read-only files by using optional mode
 *
 * Revision 6.2  1994/04/15  21:08:12  vriezen
 * Backed out (until next release) RW_DEFINE_HOME and RW_DECLARE_HOME support.
 *
 * Revision 6.1  1994/04/15  19:06:36  vriezen
 * Move all files to 6.1
 *
 * Revision 2.15  1994/03/31  23:10:54  vriezen
 * > Add RW_DECLARE_HOME and RW_DEFINE_HOME for RWListManager
 *
 * Revision 2.14  1994/03/04  07:44:28  jims
 * Check to make sure RWFile sub-object isValid() before constructing
 * rest of RWFileManager object.
 *
 * Revision 2.13  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.12  1993/08/03  17:53:07  dealys
 * ported to MPW C++ 3.3
 *
 * Revision 2.11  1993/07/08  03:07:38  keffer
 * Corrected the node order for an old-style FileManager
 *
 * Revision 2.10  1993/07/04  00:40:20  keffer
 * Now initialized unused node members to keep Purify happy when
 * the nodes are written to disk.
 *
 * Revision 2.9  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.8  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.7  1993/04/09  23:37:09  keffer
 * Changed variable name in inner loop to avoid confusion with
 * variable of the same name in an outer loop.
 *
 * Revision 2.6  1993/04/09  23:09:25  keffer
 * Extensive rewrite.  Allocations now done in units of RWspace.
 *
 *
 */

#include "rw/filemgr.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"
#include <assert.h>

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: filemgr.cpp,v $ $Revision: 6.6 $ $Date: 1994/07/18 20:51:00 $");

#ifdef applec
# include <ioctl.h>
#endif

#ifdef RDEBUG
# include "rw/rstream.h"
#endif

// Minimum space allowed after a partial allocation.  Anything less
// will get sucked into the allocation.
#define MINSPACE 8

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              RWOldNode                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


const int oldNodeOrder = 10;            // Number of allocations in a node
const unsigned short oldMagicNumber = 0x1234;
typedef unsigned OldSpace;

struct RWOldNode
{
  unsigned short	magicNumber_;
  RWoffset		freeSpaceOffset_[oldNodeOrder];
  OldSpace		freeSpaceSize_  [oldNodeOrder];
  int			entries_; 	// Number of free spaces entries.
  RWoffset		next_;		// Next block of free space data.


  RWoffset		allocate(RWspace&);
  RWBoolean		deallocate(RWoffset, RWspace);
  void			attemptMerge(int);
  void			initialize(RWoffset);
  void			slideRight(int);
  void			slideLeft(int);

};  

// Allocate in this node if possible.  Returns the allocation
// if successful, RWNIL if not.
RWoffset
RWOldNode::allocate(RWspace& space)
{
  RWoffset loc = RWNIL;

  for(register int i=0; i<entries_; i++)
  {
    if (freeSpaceSize_[i] >= (OldSpace)space)
    {
      loc = freeSpaceOffset_[i];        // Location of the allocation
      freeSpaceSize_[i] -= (OldSpace)space; // Decrease amount of free space
      if (freeSpaceSize_[i] < MINSPACE) // Is it less than the minimum?
      {
        space += freeSpaceSize_[i];     // If yes, add on the remnant
        freeSpaceSize_[i] = 0;          // Amount left is now zero
	slideLeft(i);                   // close up the hole.
	entries_--;                     // One less entry.
      }
      else
        // Otherwise, remaining space has moved:
        freeSpaceOffset_[i] += (RWoffset)space;

      break;
    }
  }
  return loc;
}


// Record some deallocated space in this node.  Return TRUE if
// successful, FALSE otherwise.
RWBoolean
RWOldNode::deallocate(RWoffset loc, RWspace amount)
{
  // First try coalescing the space with an existing entry:
  // Note, this algorithm will not recognize adjacent free spaces
  // in *separate* free-list nodes.
  for ( register int i=0; i<entries_; i++)
  {
    // Try merging to right of existing space:
    if(loc == freeSpaceOffset_[i]+(RWoffset)freeSpaceSize_[i])
    {
      freeSpaceSize_[i] += (OldSpace)amount;
      attemptMerge(i);		// Coalesce with adjacent block if possible.
      return TRUE;
    }
    // Try merging to left of existing space:
    if(loc+(RWoffset)amount == freeSpaceOffset_[i])
    {
      freeSpaceOffset_[i] = loc;
      freeSpaceSize_[i] += (OldSpace)amount;
      // No need to attempt merge.  It would have been caught above.
      return TRUE;
    }
  }

  // Coalescing didn't work.
  // Try making a new entry.  This will only work if this node has
  // some space left in it.
  if(entries_ < oldNodeOrder)
  {
    // Find where to insert the entry in the sorted array:
    for(register int j=0; j<entries_; j++)
    {
      if( loc < freeSpaceOffset_[j] ) break;
    }
    slideRight(j);                      // Move other blocks out of the way
    freeSpaceSize_[j]   = (OldSpace)amount;
    freeSpaceOffset_[j] = loc;
    entries_++;
    return TRUE;
  }

  // Unable to record in this node.  Return FALSE.
  return FALSE;
  
}

// Attempt to merge two adjacent free space entries.
void
RWOldNode::attemptMerge(int i)
{
  if (i < entries_-1)
  {
    if (freeSpaceOffset_[i] + (RWoffset)freeSpaceSize_[i] ==
                                                 freeSpaceOffset_[i+1])
    {
      freeSpaceSize_[i] += freeSpaceSize_[i+1];
      slideLeft(i+1);
      entries_--;                       // One less entry
    }
  }
}
  

void
RWOldNode::initialize(RWoffset next)
{
  next_		= next;
  entries_ 	= 0;
  magicNumber_	= oldMagicNumber;
  for (int i=0; i<oldNodeOrder; i++)
  {
    freeSpaceOffset_[i] = 0;
    freeSpaceSize_[i]   = 0;
  }
}

void
RWOldNode::slideLeft(int i)
{
  for(register int j=i+1; j<entries_; j++)
  {
    freeSpaceSize_[j-1]   = freeSpaceSize_[j];
    freeSpaceOffset_[j-1] = freeSpaceOffset_[j];
  }
}
void
RWOldNode::slideRight(int i)
{
  RWPRECONDITION(entries_<oldNodeOrder);

  if (i>=entries_) return;
  for(register int j=entries_-1; j>=i; j--)
  {
    freeSpaceSize_[j+1]   = freeSpaceSize_[j];
    freeSpaceOffset_[j+1] = freeSpaceOffset_[j];
  }
}


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              RWNewNode                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


const int newNodeOrder = 32;            // Number of allocations in a node
const unsigned short newMagicNumber = 0x1235;
typedef RWspace NewSpace;

struct RWNewNode
{
  unsigned short	magicNumber_;
  RWoffset		freeSpaceOffset_[newNodeOrder];
  NewSpace		freeSpaceSize_  [newNodeOrder];
  int			entries_; 	// Number of free spaces entries.
  RWoffset		next_;		// Next block of free space data.


  RWoffset		allocate(RWspace&);
  RWBoolean		deallocate(RWoffset, RWspace);
  void			attemptMerge(int);
  void			initialize(RWoffset);
  void			slideRight(int);
  void			slideLeft(int);

};  

// Allocate in this node if possible.  Returns the allocation
// if successful, RWNIL if not.
RWoffset
RWNewNode::allocate(RWspace& space)
{
  RWoffset loc = RWNIL;

  for(register int i=0; i<entries_; i++)
  {
    if (freeSpaceSize_[i] >= (NewSpace)space)
    {
      loc = freeSpaceOffset_[i];        // Location of the allocation
      freeSpaceSize_[i] -= (NewSpace)space; // Decrease amount of free space
      if (freeSpaceSize_[i] < MINSPACE) // Is it less than the minimum?
      {
        space += freeSpaceSize_[i];     // If yes, add on the remnant
        freeSpaceSize_[i] = 0;          // Amount left is now zero
	slideLeft(i);                   // close up the hole.
	entries_--;                     // One less entry.
      }
      else
        // Otherwise, remaining space has moved:
        freeSpaceOffset_[i] += (RWoffset)space;

      break;
    }
  }
  return loc;
}


// Record some deallocated space in this node.  Return TRUE if
// successful, FALSE otherwise.
RWBoolean
RWNewNode::deallocate(RWoffset loc, RWspace amount)
{
  // First try coalescing the space with an existing entry:
  // Note, this algorithm will not recognize adjacent free spaces
  // in *separate* free-list nodes.
  for ( register int i=0; i<entries_; i++)
  {
    // Try merging to right of existing space:
    if(loc == freeSpaceOffset_[i]+(RWoffset)freeSpaceSize_[i])
    {
      freeSpaceSize_[i] += (NewSpace)amount;
      attemptMerge(i);		// Coalesce with adjacent block if possible.
      return TRUE;
    }
    // Try merging to left of existing space:
    if(loc+(RWoffset)amount == freeSpaceOffset_[i])
    {
      freeSpaceOffset_[i] = loc;
      freeSpaceSize_[i] += (NewSpace)amount;
      // No need to attempt merge.  It would have been caught above.
      return TRUE;
    }
  }

  // Coalescing didn't work.
  // Try making a new entry.  This will only work if this node has
  // some space left in it.
  if(entries_ < newNodeOrder)
  {
    // Find where to insert the entry in the sorted array:
    for(register int j=0; j<entries_; j++)
    {
      if( loc < freeSpaceOffset_[j] ) break;
    }
    slideRight(j);                      // Move other blocks out of the way
    freeSpaceSize_[j]   = (NewSpace)amount;
    freeSpaceOffset_[j] = loc;
    entries_++;
    return TRUE;
  }

  // Unable to record in this node.  Return FALSE.
  return FALSE;
  
}

// Attempt to merge two adjacent free space entries.
void
RWNewNode::attemptMerge(int i)
{
  if (i < entries_-1)
  {
    if (freeSpaceOffset_[i] + (RWoffset)freeSpaceSize_[i] ==
                                                 freeSpaceOffset_[i+1])
    {
      freeSpaceSize_[i] += freeSpaceSize_[i+1];
      slideLeft(i+1);
      entries_--;                       // One less entry
    }
  }
}
  

void
RWNewNode::initialize(RWoffset next)
{
  next_		= next;
  entries_ 	= 0;
  magicNumber_	= newMagicNumber;
  for (int i=0; i<newNodeOrder; i++)
  {
    freeSpaceOffset_[i] = 0;
    freeSpaceSize_[i]   = 0;
  }
}

void
RWNewNode::slideLeft(int i)
{
  for(register int j=i+1; j<entries_; j++)
  {
    freeSpaceSize_[j-1]   = freeSpaceSize_[j];
    freeSpaceOffset_[j-1] = freeSpaceOffset_[j];
  }
}
void
RWNewNode::slideRight(int i)
{
  RWPRECONDITION(entries_<newNodeOrder);

  if (i>=entries_) return;
  for(register int j=entries_-1; j>=i; j--)
  {
    freeSpaceSize_[j+1]   = freeSpaceSize_[j];
    freeSpaceOffset_[j+1] = freeSpaceOffset_[j];
  }
}


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                            RWListManager                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class RWListManager
{
  RW_DECLARE_HOME(RWListManager)
public:

  RWListManager(RWFileManager* f) : filemgr_(f) {;}
  virtual ~RWListManager() {;}

  virtual RWoffset	allocate(RWspace)    = 0;
  virtual void          deallocate(RWoffset) = 0;
#ifdef RDEBUG
  virtual RWoffset      walkFreeList(RWoffset&, int&, RWspace&) = 0;
  virtual void          summarize() = 0;
#endif

protected:

  RWoffset              allocateAtEnd(RWspace s)
    {return filemgr_->allocateAtEnd(s);}
  RWBoolean             deallocateFromEnd(RWoffset loc, RWspace s)
    {return filemgr_->deallocateFromEnd(loc, s);}
  RWoffset              rootOffset() const
    {return filemgr_->rootOffset();}
  RWBoolean             isRootNode() const
    {return here_ == filemgr_->rootOffset();}

  void                  readErr()
    {filemgr_->readErr();}
  void                  seekErr()
    {filemgr_->seekErr();}
  void                  writeErr()
    {filemgr_->writeErr();}

  RWFileManager*        filemgr_;
  RWoffset              here_;

};

  RW_DEFINE_HOME(RWListManager)

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                           RWOldListManager                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class RWOldListManager : public RWListManager
{

public:

  RWOldListManager(RWFileManager*, RWBoolean);
  virtual RWoffset	allocate(RWspace);
  virtual void          deallocate(RWoffset);
#ifdef RDEBUG
  virtual RWoffset      walkFreeList(RWoffset&, int&, RWspace&);
  virtual void          summarize();
#endif

private:

  RWBoolean             advance(RWoffset);
  RWBoolean             advance() {return readNode(node_.next_);}
  RWBoolean             readNode(RWoffset);
  void                  writeNode();
  void                  deleteNode();
  OldSpace              readHeader(RWoffset);
  void                  writeHeader(RWoffset, OldSpace);
  void                  addToFreeList(RWoffset, OldSpace);
  void                  extendFreeList(RWoffset, OldSpace);

  RWOldNode             node_;

};

RWOldListManager::RWOldListManager(RWFileManager* fm, RWBoolean newFile)
 : RWListManager(fm)
{
  if (newFile)
  {
    here_ = rootOffset();
    node_.initialize(rootOffset());
    writeNode();                        // Initialize with virgin node
  }
  else
  {
    readNode(rootOffset());             // Recover old node
  }
}
   
RWoffset
RWOldListManager::allocate(RWspace spaceRequested)
{
  // Add on size of the header and round up to 4 byte boundary:
  RWspace spaceRequired =
    (unsigned long)(spaceRequested + sizeof(OldSpace) + 3) & ~(unsigned long)3;

  // Remember starting location:
  RWoffset startLoc = here_;
  RWoffset ret = RWNIL;

  // Loop around the free list, looking for a node
  // that can handle the allocation:
  do
  {
    ret = node_.allocate(spaceRequired);
    if (ret != RWNIL ) break;
  }
  while (advance(startLoc));

  // If unsuccessful, allocate at the end of the file:
  if (ret == RWNIL)
    ret = allocateAtEnd(spaceRequired);
  else
  {
    // Delete the node if empty, unless it is the last node:
    if (node_.entries_ == 0
      && (!isRootNode() || node_.next_ != rootOffset()) )
      deleteNode();
    else
      writeNode();
  }
  
  // Record the size of the allocation in the header:
  if (ret != RWNIL)
  {
    writeHeader(ret, (OldSpace)spaceRequired);
    ret += sizeof(OldSpace);            // Adjust to what the user will see
  }
  return ret;
}

RWBoolean
RWOldListManager::advance(RWoffset startLoc)
{
  if (node_.next_ == startLoc) return FALSE;

  return readNode(node_.next_);
}

// Read the node at offset 'loc'
RWBoolean
RWOldListManager::readNode(RWoffset nodeLoc)
{
  here_ = nodeLoc;

  // Seek first, then read:
  if ( !filemgr_->SeekTo(nodeLoc)                    ) seekErr();
  if ( !filemgr_->Read((char*)&node_, sizeof(node_)) ) readErr();

  // Check the magic number:
  if ( node_.magicNumber_ != oldMagicNumber )
    RWTHROW(RWExternalErr(RWMessage(RWTOOL_MAGIC,
				    (long)node_.magicNumber_,
				    (long)oldMagicNumber) ));

  return TRUE;

}

void
RWOldListManager::writeNode()
{
    if ( !filemgr_->SeekTo(here_)                            ) seekErr();
    if ( !filemgr_->Write((const char*)&node_, sizeof(node_))) writeErr();
}

void
RWOldListManager::deleteNode()
{
  RWoffset index;

  // Is this the root node?
  if (isRootNode())
  {
    // We are deleting the root node.
    // Replace it with the next node.
    readNode(index = node_.next_);      // Read in next node
    here_ = rootOffset();               // Now it is the root node
  }
  else
  {
    // Not root node.
    index = here_;                      // Offset of node to be deleted
    RWoffset nextIndex = node_.next_;   // Offset of node after it

    // Starting with the root node,
    // search for the node before this node:
    readNode(rootOffset());             // Read root node
    while (node_.next_ != index)
      advance();

    node_.next_ = nextIndex;            // Point around deleted node
  }

  writeNode();		// Record changes

  // Add deleted space to the free space list
  addToFreeList(index, sizeof(RWOldNode));
}

void
RWOldListManager::writeHeader(RWoffset loc, OldSpace space)
{
  if (!filemgr_->SeekTo(loc)  ) seekErr();
  if (!filemgr_->Write(space) ) writeErr();
}

void
RWOldListManager::deallocate(RWoffset loc)
{
  // Subtract off the header:
  loc -= sizeof(OldSpace);

  // Get the size of the allocation
  OldSpace spaceFreed = readHeader(loc);

  // Add it to the free list:
  addToFreeList(loc, spaceFreed);
}


OldSpace
RWOldListManager::readHeader(RWoffset loc)
{
  OldSpace temp;
  
  if (!filemgr_->SeekTo(loc) ) seekErr();
  if (!filemgr_->Read(temp))   readErr();

  return temp;
}

// Add the allocation at location 'loc' and of size 'space' to
// the free list.
void
RWOldListManager::addToFreeList(RWoffset loc, OldSpace space)
{
  // Check to see if this allocation is at the end of the file.
  // If so, redefine end of data.
  if (deallocateFromEnd(loc, space)) return;

  // Remember starting location:
  RWoffset startLoc = here_;

  // Loop around the free list, looking for a node
  // that can handle the deallocation:
  do
  {
    // Can this node handle it?
    if (node_.deallocate(loc, space))
    {
      writeNode();	// Yes. Record the change
      return;
    }
  } while (advance(startLoc));

  // Nodes all full; create a new one:
  extendFreeList(loc, space);  
}

/*
 * Extend the free list by creating a new node.
 */
void
RWOldListManager::extendFreeList(RWoffset loc, OldSpace space)
{
  // We know the size of a free list node, hence we don't need to
  // store a header with it.  This allows us to allocate a little
  // less space:
  RWoffset nodeLoc = allocate(sizeof(node_) - sizeof(OldSpace));
  
  // Use the header space:
  nodeLoc -= sizeof(OldSpace);

  RWoffset oldNext  = node_.next_;

  // Splice the new node in after the current node
  node_.next_  = nodeLoc;
  writeNode();

  here_ = nodeLoc;
  node_.initialize(oldNext);

  // Because this is an empty node, the following must succeed:
  assert(node_.deallocate(loc, space));

  writeNode();
}


#ifdef RDEBUG
  
RWoffset
RWOldListManager::walkFreeList(RWoffset& nodeLoc, int& idx, RWspace& space)
{
  if (nodeLoc==0)
  {
    nodeLoc = rootOffset();
    idx = 0;
  }

  readNode(nodeLoc);

  if (idx >= node_.entries_)
  {
    if (node_.next_ == rootOffset()) return RWNIL;
    advance();
    nodeLoc = here_;
    idx = 0;
  }

  assert( node_.entries_ > 0);
  assert( idx < node_.entries_);
  
  space = node_.freeSpaceSize_[idx];
  return node_.freeSpaceOffset_[idx++];
}

  
void
RWOldListManager::summarize()
{

  // Start at the root node:
  readNode(rootOffset());

  do
  {
    cout << "*** Block starting at " << here_           << endl;
    cout << "*** which points to   " << node_.next_ << endl;
    cout << "      Location\tSpace\n";
    for(int i =0; i<node_.entries_; i++)
    {
      cout << "      " 
	   << node_.freeSpaceOffset_[i]
	   << "\t"
	   << node_.freeSpaceSize_[i]
	   << endl;
    }
  }
  while (advance(rootOffset()));
}

#endif  /* RDEBUG */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                           RWNewListManager                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class RWNewListManager : public RWListManager
{

public:

  RWNewListManager(RWFileManager*, RWBoolean);
  virtual RWoffset	allocate(RWspace);
  virtual void          deallocate(RWoffset);
#ifdef RDEBUG
  virtual RWoffset      walkFreeList(RWoffset&, int&, RWspace&);
  virtual void          summarize();
#endif

private:

  RWBoolean             advance(RWoffset);
  RWBoolean             advance() {return readNode(node_.next_);}
  RWBoolean             readNode(RWoffset);
  void                  writeNode();
  void                  deleteNode();
  NewSpace              readHeader(RWoffset);
  void                  writeHeader(RWoffset, NewSpace);
  void                  addToFreeList(RWoffset, NewSpace);
  void                  extendFreeList(RWoffset, NewSpace);

  RWNewNode             node_;

};

RWNewListManager::RWNewListManager(RWFileManager* fm, RWBoolean newFile)
 : RWListManager(fm)
{
  if (newFile)
  {
    here_ = rootOffset();
    node_.initialize(rootOffset());
    writeNode();                        // Initialize with virgin node
  }
  else
  {
    readNode(rootOffset());             // Recover old node
  }
}
   
RWoffset
RWNewListManager::allocate(RWspace spaceRequested)
{
  // Add on size of the header and round up to 4 byte boundary:
  RWspace spaceRequired =
    (unsigned long)(spaceRequested + sizeof(NewSpace) + 3) & ~(unsigned long)3;

  // Remember starting location:
  RWoffset startLoc = here_;
  RWoffset ret = RWNIL;

  // Loop around the free list, looking for a node
  // that can handle the allocation:
  do
  {
    ret = node_.allocate(spaceRequired);
    if (ret != RWNIL ) break;
  }
  while (advance(startLoc));

  // If unsuccessful, allocate at the end of the file:
  if (ret == RWNIL)
    ret = allocateAtEnd(spaceRequired);
  else
  {
    // Delete the node if empty, unless it is the last node:
    if (node_.entries_ == 0
      && (!isRootNode() || node_.next_ != rootOffset()) )
      deleteNode();
    else
      writeNode();
  }
  
  // Record the size of the allocation in the header:
  if (ret != RWNIL)
  {
    writeHeader(ret, (NewSpace)spaceRequired);
    ret += sizeof(NewSpace);            // Adjust to what the user will see
  }
  return ret;
}

RWBoolean
RWNewListManager::advance(RWoffset startLoc)
{
  if (node_.next_ == startLoc) return FALSE;

  return readNode(node_.next_);
}

// Read the node at offset 'loc'
RWBoolean
RWNewListManager::readNode(RWoffset nodeLoc)
{
  here_ = nodeLoc;

  // Seek first, then read:
  if ( !filemgr_->SeekTo(nodeLoc)                    ) seekErr();
  if ( !filemgr_->Read((char*)&node_, sizeof(node_)) ) readErr();
  
  // Check the magic number:
  if ( node_.magicNumber_ != newMagicNumber )
    RWTHROW(RWExternalErr(RWMessage(RWTOOL_MAGIC,
				    (long)node_.magicNumber_,
				    (long)newMagicNumber) ));

  return TRUE;

}

void
RWNewListManager::writeNode()
{
  if ( !filemgr_->SeekTo(here_)                            ) seekErr();
  if ( !filemgr_->Write((const char*)&node_, sizeof(node_))) writeErr();
}

void
RWNewListManager::deleteNode()
{
  RWoffset index;

  // Is this the root node?
  if (isRootNode())
  {
    // We are deleting the root node.
    // Replace it with the next node.
    readNode(index = node_.next_);      // Read in next node
    here_ = rootOffset();               // Now it is the root node
  }
  else
  {
    // Not root node.
    index = here_;                      // Offset of node to be deleted
    RWoffset nextIndex = node_.next_;   // Offset of node after it

    // Starting with the root node,
    // search for the node before this node:
    readNode(rootOffset());             // Read root node
    while (node_.next_ != index)
      advance();

    node_.next_ = nextIndex;            // Point around deleted node
  }

  writeNode();		// Record changes

  // Add deleted space to the free space list
  addToFreeList(index, sizeof(RWNewNode));
}

void
RWNewListManager::writeHeader(RWoffset loc, NewSpace space)
{
  if (!filemgr_->SeekTo(loc)  ) seekErr();
  if (!filemgr_->Write(space) ) writeErr();
  
}

void
RWNewListManager::deallocate(RWoffset loc)
{
  // Subtract off the header:
  loc -= sizeof(NewSpace);

  // Get the size of the allocation
  NewSpace spaceFreed = readHeader(loc);

  // Add it to the free list:
  addToFreeList(loc, spaceFreed);
}


NewSpace
RWNewListManager::readHeader(RWoffset loc)
{
  NewSpace temp;

  if (!filemgr_->SeekTo(loc) ) seekErr();
  if (!filemgr_->Read(temp)  ) readErr();
  
  return temp;
}

// Add the allocation at location 'loc' and of size 'space' to
// the free list.
void
RWNewListManager::addToFreeList(RWoffset loc, NewSpace space)
{
  // Check to see if this allocation is at the end of the file.
  // If so, redefine end of data.
  if (deallocateFromEnd(loc, space)) return;

  // Remember starting location:
  RWoffset startLoc = here_;

  // Loop around the free list, looking for a node
  // that can handle the deallocation:
  do
  {
    // Can this node handle it?
    if (node_.deallocate(loc, space))
    {
      writeNode();	// Yes. Record the change
      return;
    }
  } while (advance(startLoc));

  // Nodes all full; create a new one:
  extendFreeList(loc, space);  
}

/*
 * Extend the free list by creating a new node.
 */
void
RWNewListManager::extendFreeList(RWoffset loc, NewSpace space)
{
  // We know the size of a free list node, hence we don't need to
  // store a header with it.  This allows us to allocate a little
  // less space:
  RWoffset nodeLoc = allocate(sizeof(node_) - sizeof(NewSpace));
  
  // Use the header space:
  nodeLoc -= sizeof(NewSpace);

  RWoffset oldNext  = node_.next_;

  // Splice the new node in after the current node
  node_.next_  = nodeLoc;
  writeNode();

  here_ = nodeLoc;
  node_.initialize(oldNext);

  // Because this is an empty node, the following must succeed:
  assert(node_.deallocate(loc, space));

  writeNode();
}


#ifdef RDEBUG
  
RWoffset
RWNewListManager::walkFreeList(RWoffset& nodeLoc, int& idx, RWspace& space)
{
  if (nodeLoc==0)
  {
    nodeLoc = rootOffset();
    idx = 0;
  }

  readNode(nodeLoc);

  if (idx >= node_.entries_)
  {
    if (node_.next_ == rootOffset()) return RWNIL;
    advance();
    nodeLoc = here_;
    idx = 0;
  }

  assert( node_.entries_ > 0);
  assert( idx < node_.entries_);
  
  space = node_.freeSpaceSize_[idx];
  return node_.freeSpaceOffset_[idx++];
}

  
void
RWNewListManager::summarize()
{

  // Start at the root node:
  readNode(rootOffset());

  do
  {
    cout << "*** Block starting at " << here_           << endl;
    cout << "*** which points to   " << node_.next_ << endl;
    cout << "      Location\tSpace\n";
    for(int i =0; i<node_.entries_; i++)
    {
      cout << "      " 
	   << node_.freeSpaceOffset_[i]
	   << "\t"
	   << node_.freeSpaceSize_[i]
	   << endl;
    }
  }
  while (advance(rootOffset()));
}

#endif  /* RDEBUG */


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                      RWFileManager definitions                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/*
 * Definitions for RWFileManager mostly consists of a bunch of forwarding
 * functions to the abstract base class RWListManager.
 *
 */

RWFileManager::RWFileManager(const char* fname, const char* mode)
 : RWFile(fname, mode),
   filemgr_(rwnil),
   startOfData_(RWNIL),
   endOfData_(RWNIL)
{
  if( isValid() && Exists() )
  {
    // Is this file empty?
    if( IsEmpty() )
    {
      // Virgin file; use the new style manager:
      // (If mode is read-only - writeErr() will be called)
      startOfData_    = RWNIL;
      endOfData_      = rootOffset() + sizeof(RWNewNode);
      size_t nodeSize = sizeof(RWNewNode);
      if (!SeekToBegin()) seekErr();
      if (!Write(startOfData_)  ||
	  !Write(endOfData_)    ||
	  !Write(nodeSize) ) writeErr();
      filemgr_ = new RWNewListManager(this, TRUE);
    }

    else
    {
      // File is not empty.
      size_t nodeSize;

      // Recover header information:
      if (!SeekToBegin()) seekErr();
      if (!Read(startOfData_)   ||
	  !Read(endOfData_)     ||
	  !Read(nodeSize) ) readErr();

      // Use the node size to figure out if it is old style or
      // new style nodes:
      if (nodeSize == sizeof(RWNewNode))
        filemgr_ = new RWNewListManager(this, FALSE);
      else if (nodeSize == sizeof(RWOldNode))
        filemgr_ = new RWOldListManager(this, FALSE);
      else
      {
	RWTHROW(RWExternalErr(RWMessage(RWTOOL_FLIST,
					(long)sizeof(RWNewNode),
					(long)nodeSize) ));
      }
    }
  }
}  

RWFileManager::~RWFileManager()
{
  delete filemgr_;
}

RWoffset
RWFileManager::allocate(RWspace space)
{
  RWPRECONDITION(filemgr_ != rwnil);

  RWoffset ret = filemgr_->allocate(space);

  if (startOfData_ == RWNIL && ret != RWNIL)
  {
    startOfData_ = ret;
    if ( !SeekToBegin()       ) seekErr();
    if ( !Write(startOfData_) ) writeErr();
  }
  return ret;
}

void
RWFileManager::deallocate(RWoffset loc)
{
  RWPRECONDITION(filemgr_ != rwnil);

  // Can't deallocate first allocated block or RWNIL:
  if ( loc == RWNIL || loc == start() ) return;  

  filemgr_->deallocate(loc);
}

/*
 * Allocates space at the end of the file.
 */
RWoffset
RWFileManager::allocateAtEnd(RWspace spaceRequired)
{
  RWoffset ret = endOfData_;

  // Update and record new endOfData_:
  endOfData_ += spaceRequired;

  if ( !SeekTo(sizeof(RWoffset)) ) seekErr();
  if ( !Write(endOfData_) )        writeErr();

#ifdef applec
  if(ioctl(fileno(filep),FIOSETEOF,(long*)endOfData_) == -1) writeErr();
#endif

  return ret;
}

// Checks to see if space freed is last record.  If not, returns FALSE.
// If TRUE, redefines endOfData_ and writes new value to disk.
RWBoolean
RWFileManager::deallocateFromEnd(RWoffset loc, RWspace space)
{
  if (loc + (RWoffset)space >= endOfData_)	// If last record removed:
  {
    endOfData_ = loc;		                // Change end of data point.
    // Store end of data point.
    if ( !SeekTo(sizeof(RWoffset)) ) seekErr();
    if ( !Write(endOfData_) )        writeErr();
    return TRUE;
  }
  return FALSE;
}

void
RWFileManager::readErr()
{
  RWTHROW(RWFileErr(RWMessage( RWTOOL_READERR ),
		    GetStream(),
		    RWFileErr::readErr) );
}

void
RWFileManager::seekErr()
{
  RWTHROW(RWFileErr(RWMessage( RWTOOL_SEEKERR ),
		    GetStream(),
		    RWFileErr::seekErr) );
}

void
RWFileManager::writeErr()
{
  RWTHROW(RWFileErr(RWMessage( RWTOOL_WRITEERR ),
		    GetStream(),
		    RWFileErr::writeErr) );
}

RWoffset
RWFileManager::rootOffset() const
{
  return sizeof(startOfData_) + sizeof(endOfData_) + sizeof(size_t);
}

#ifdef RDEBUG

RWoffset
RWFileManager::walkFreeList(RWoffset& nodeLoc, int& idx, RWspace& space)
{
  RWPRECONDITION(filemgr_ != rwnil);
  return filemgr_->walkFreeList(nodeLoc, idx, space);
}

void
RWFileManager::summarize()
{
  RWPRECONDITION(filemgr_ != rwnil);

  cout << "Start of data is " << start()   << endl;
  cout << "End of data is   " << endData() << endl;

  filemgr_->summarize();
}

#endif  /* RDEBUG */
