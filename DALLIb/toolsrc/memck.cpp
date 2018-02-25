/*
 * MEMCK definitions
 *
 * $Id: memck.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: memck.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:07:14  vriezen
 * Move all files to 6.1
 *
 * Revision 2.5  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.4  1993/07/09  21:00:58  jims
 * Add rwexport to overloaded new and delete
 *
 * Revision 2.2  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.1  1992/11/15  22:07:11  keffer
 * Removed use of macro NL
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.5   08 Jun 1992 12:16:38   KEFFER
 * Ported to Liant under Unix.
 * 
 *    Rev 1.4   30 May 1992 16:35:20   KEFFER
 * Inserted rwdummy2() to keep fussy librarians happy.
 * 
 *    Rev 1.2   28 May 1992 11:01:06   KEFFER
 * Moved the special version of RWMemoryPool::new to memck.cpp.
 * 
 *    Rev 1.1   28 May 1992 10:55:36   KEFFER
 * 
 *    Rev 1.0   27 May 1992 18:18:58   KEFFER
 * Initial revision.
 */

/*
 * Proceed only if the file is being compiled in "RWMEMCK" mode.
 */
#ifdef RWMEMCK


#include "rw/memck.h"
#include "rw/mempool.h"
#undef new
#include <stdlib.h>

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: memck.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

const int  PREPAD      = 8;	// Number of bytes before the data area
const int  POSTPAD     = 8;	// Number of bytes after the data area
const char PREPADCHAR  = 0xff;	// Character with which to pad before data
const char POSTPADCHAR = 0xfe;	// Character with which to pad after data
const char DATACHAR    = 0xfd;	// Character with which to initialize data
const int  MAGIC       = 0x1234;// Magic number

// Round a memory allocation up so it divisible by 4:
inline size_t roundAlloc(size_t sz) {return (sz+3) & ~3;}

union Align {		/* Used to insure proper alignment */
  char			data;
  long			dummy;
};

class RWMemAlloc : public RWMemLink {
friend RWMemoryCheck;
  int			magic;	// Magic number
  size_t		size;	// Allocation size, not counting header & trailer
  const char*		file;	// zero for unknown
  int			line;
  char			prepad[PREPAD];
  union Align		dataUnion;	// Start of data area

  // Effectively followed by:
  // char		data[size-sizeof(Align)];
  // char		postpad[POSTPAD];

public:		// Member functions:

  RWBoolean		checkIntegrity(RWBoolean checkData);
  void*			dataStart()	{return (void*)&dataUnion.data;}
  static RWMemAlloc*	getThis(void*);	// Given data area, return "this"
  RWBoolean		magicOK()	{return magic==MAGIC;}
  void*			postpadStart()	{return (void*)((char*)dataStart() + size);}
  void			printStatsOn(ostream&);
  void			scribble();
};

// Statics:

const MAXCHECK = 16;
static RWMemoryCheck* stack[MAXCHECK];
static int nStack = 0;

//////////////////////////////////////////////////////////////////////////
//									//
//		RWMemoryCheck DEFINITIONS				//
//									//
//////////////////////////////////////////////////////////////////////////

#ifdef RW_GLOBAL_ENUMS
RWMemoryCheck::RWMemoryCheck(memckType checks, ostream& str) :
#else
RWMemoryCheck::RWMemoryCheck(RWMemoryCheck::memckType checks, ostream& str) :
#endif
  rpstream_(str),
  checks_(checks)
{
  allocList_.next = rwnil;
  freeList_.next  = rwnil;
  stack[nStack++] = this;	// Push myself onto the allocation stack
}

RWMemoryCheck::~RWMemoryCheck()
{
  if (checks_ & checkDestroy) {
    walkList(TRUE);
  }
  releaseFree();		// Release my free list
  stack[--nStack] = rwnil;	// Pop myself off the allocation stack
}

void*
RWMemoryCheck::allocate(size_t sz, const char* file, int line)
{
  if (sz<=0) sz = 1;
  sz = roundAlloc(sz);	// Take care of alignment hassles

  size_t totalloc = sizeof(RWMemAlloc) + sz - sizeof(Align) + POSTPAD;
  RWMemAlloc* p = (RWMemAlloc*)malloc(totalloc);

  p->size = sz;		// Fill in ... allocation size;
  p->file = file;	//         ... where it came from;
  p->line = line;	//         ... line number;
  p->magic = MAGIC;	//         ... magic number.

  p->scribble();	// Initialize prepad, data, and postpad areas

  p->next = allocList_.next;	// Thread onto outstanding list
  allocList_.next = p;
  return p->dataStart();
}

void
RWMemoryCheck::deallocate(void* q)
{
  if (q==rwnil) return;	// Never an error to delete the nil pointer

  RWMemAlloc* p = RWMemAlloc::getThis(q);
  RWMemAlloc* ck = removeFromAllocList(p);

  // Was it found on the allocation list?
  if (ck) {
    // Yup.  Check its integrity
    if (!p->checkIntegrity(FALSE)) {
      rpstream_ << "\nMEMCK: Wild pointer.\n";
      p->printStatsOn(rpstream_);
    }

    /* If requested, add it to the free list: */
    if (checks_ & retainFree) {
      p->scribble();		// Scribble on it.
      p->next = freeList_.next;	// Thread on to free list.
      freeList_.next = p;
    }
    else {
      free(p);			// Otherwise, release to the operating system
    }
  }

  else {

    /* It wasn't found on the list of outstanding allocations.
       Try to figure out what's wrong: is it on the free list? */

    if (checks_ & retainFree && onFreeList(ck))
      rpstream_ << "\nMEMCK: Memory freed twice.\n";

    else
      rpstream_ << "\nMEMCK: Memory freed that wasn't allocated.\n";

    p->printStatsOn(rpstream_);
  }
}

/*
 * Walk the free list, returning everything to the operating system.
 */
void
RWMemoryCheck::releaseFree()
{
  RWMemAlloc* link = freeList_.next;
  while (link) {
    RWMemAlloc* next = link->next;
    free(link);
    link = next;
  }
}

/*
 * Remove the memory allocation pointed to by "p" from the list
 * of outstanding blocks.
 */
RWMemAlloc*
RWMemoryCheck::removeFromAllocList(RWMemAlloc* p)
{
  RWMemAlloc* link = (RWMemAlloc*)&allocList_;
  while (link->next) {
    if (link->next==p){			// Found it
      link->next = link->next->next;
      return p;
    }
    link = link->next;
  }
  return rwnil;		// Not on the allocation list.
}

RWBoolean
RWMemoryCheck::onFreeList(RWMemAlloc* p)
{
  RWMemAlloc* link = (RWMemAlloc*)&freeList_;
  while (link) {
    if (link==p) return TRUE;
    link = link->next;
  }
  return FALSE;
}

/*
 * Walk the free and outstanding lists, checking the integrity
 * of each allocation.
 */
void
RWMemoryCheck::walkList(RWBoolean reportAlloc)
{
  // First do the free list:
  RWMemAlloc* link = freeList_.next;
  while (link) {
    if( link->checkIntegrity(TRUE)==FALSE ) {
      rpstream_ << "\nMEMCK: Use after deletion.\n";
      link->printStatsOn(rpstream_);
    }      
    link = link->next;
  }

  // Now do the outstanding list:
  link = allocList_.next;
  while (link) {
    if( link->checkIntegrity(FALSE)==FALSE ) {
      rpstream_ << "\nMEMCK: Wild pointer.\n";
      link->printStatsOn(rpstream_);
    }
    if (reportAlloc) {
      rpstream_ << "\nMEMCK: Outstanding allocation.\n";
      link->printStatsOn(rpstream_);
    }
    link = link->next;
  }
}

//////////////////////////////////////////////////////////////////////////
//									//
//			RWMemAlloc DEFINITIONS				//
//									//
//////////////////////////////////////////////////////////////////////////

RWBoolean
RWMemAlloc::checkIntegrity(RWBoolean checkData)
{
  register i;
  char* q;

  // Check the magic number to give some semblance of sanity:
  if (!magicOK()) return FALSE;

  /* Check the PREPAD area: */
  for (i=0; i<PREPAD; i++)
    if (prepad[i] != (char)PREPADCHAR)
      return FALSE;

  /* Optionally, check the data area: */
  if (checkData) {
    q = (char*)dataStart();
    for (i=0; i<size; i++)
      if (q[i] != (char)DATACHAR)
        return FALSE;
  }

  q = (char*)postpadStart();
  for (i=0; i<POSTPAD; i++)
    if (q[i] != (char)POSTPADCHAR)
      return FALSE;

  return TRUE;	// Passes
}

// Given a pointer to the data area, this static function calculates
// the "this" pointer associated with it.
RWMemAlloc*
RWMemAlloc::getThis(void* q)
{
  RWMemAlloc* p = (RWMemAlloc*)(
      (char*)q			// Starting point
      + sizeof(Align)		// start of data area
      - sizeof(RWMemAlloc)	// subtract off header
    );
  return p;
}

void
RWMemAlloc::printStatsOn(ostream& strm)
{
  strm << "       Memory block at " << dataStart();
  if (magicOK()){
    if (file) strm << "; allocated in file " << file << " line " << line;
  }
  else {
      strm << "; (has bad magic number)";
  }
  strm << endl;
}

void
RWMemAlloc::scribble()
{
  memset(prepad,         PREPADCHAR,  PREPAD);
  memset(dataStart(),    DATACHAR,    size);
  memset(postpadStart(), POSTPADCHAR, POSTPAD);
}

/****************************************************************
 ****************************************************************
 *								*
 *		Override global "new's" and "delete's"		*
 *								*
 ****************************************************************
 ****************************************************************/

void* rwexport
operator new(size_t sz){
 return nStack ? stack[nStack-1]->allocate(sz, rwnil, 0) : malloc(sz);
}

void* rwexport
operator new(size_t sz, const char* filename, int line) {
  return nStack ? stack[nStack-1]->allocate(sz, filename, line) : malloc(sz);
}

void rwexport
operator delete(void* p) {
  if (nStack)
    stack[nStack-1]->deallocate(p);
  else
    free(p);
}

/****************************************************************
 ****************************************************************
 *								*
 *		Add a special case "new" for RWMemoryPool	*
 *								*
 ****************************************************************
 ****************************************************************/

void*
RWMemoryPool::operator new(size_t sz, const char* filename, int line)
{
  return ::new(filename, line) char[sz];
}

#else	/* Not in RWMEMCK mode. */

int rwdummy2(){return 0;}	// For linkers that get upset when they see
				// a no-op file

#endif
