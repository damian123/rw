
/*
 * Definitions for RWHashTable --- chained hash table.
 *
 * $Id: hashtab.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: hashtab.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:06:43  vriezen
 * Move all files to 6.1
 *
 * Revision 2.14  1993/12/09  16:36:24  jims
 * ObjectStore version: Fix mistyped parens in operator=()
 *
 * Revision 2.13  1993/11/09  08:37:30  jims
 * Port to ObjectStore
 *
 * Revision 2.12  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.11  1993/05/17  18:57:22  dealys
 * merged with hashtbdf.cpp
 *
 * Revision 2.10  1993/05/14  20:15:42  dealys
 * removed iterator code
 *
 * Revision 2.9  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.8  1993/04/01  00:52:59  myersn
 * change GVector mention to RWGVector.
 *
 * Revision 2.7  1993/03/24  23:49:27  keffer
 * Indexing operations now used unsigned
 *
 * Revision 2.6  1993/02/07  21:31:49  keffer
 * Provided copy constructor for iterator
 *
 * Revision 2.5  1993/01/29  21:52:49  keffer
 * *** empty log message ***
 *
 * Revision 2.4  1993/01/28  22:43:13  keffer
 * Optimized insertions slightly.
 *
 * Revision 2.2  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.1  1992/11/15  00:27:03  keffer
 * Added explicit "this->" dereference to suppress unfounded
 * cfront 3.0 warnings.
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.8   29 May 1992 11:51:36   KEFFER
 * 0u -> 0
 * 
 *    Rev 1.7   29 May 1992 09:46:48   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.6   28 May 1992 15:19:58   KEFFER
 * Suppressed unfounded warnings from Metaware High-C++.
 * 
 *    Rev 1.5   27 May 1992 18:09:04   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.4   25 May 1992 15:27:04   KEFFER
 * Optimized boundary checking.
 * 
 *    Rev 1.3   22 May 1992 17:02:20   KEFFER
 * Simplified algorithms
 * 
 *    Rev 1.0   29 Apr 1992 14:50:54   KEFFER
 * Hashing now uses chaining to resolve collisions
 *
 */

#include "rw/hashtab.h"
#include "rw/slistcol.h"
#include "defcol.h"             

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: hashtab.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

#define BUCKET         RWSlistCollectables
#define BUCKETITERATOR RWSlistCollectablesIterator  

RWDEFINE_COLLECTABLE2(RWHashTable, __RWHASHTABLE)

// Default constructor: N defaults to RWCollectable::DEFAULT_CAPACITY (=64)
RWHashTable::RWHashTable(size_t N) :
  table_(N ? N : 2,rwnil),
  nitems_(0)
{
}
 
/*
 * got to put this somewhere; might as well be here:
 */
implement(RWGVector,RWSlistCollectablesP) 

// Destructor:
RWHashTable::~RWHashTable()
{
  clear();
}

// Copy constructor: makes a shallow copy
RWHashTable::RWHashTable(const RWHashTable& v) :
  table_(v.table_.length(), rwnil),
  nitems_(v.nitems_)
{
  size_t N = table_.length();
  for(size_t i=0; i<N; i++)
    if (v.table_(i))
      table_(i) = new BUCKET(*v.table_(i));	// Copy will be made
}

/*
 * Assignment operator: sets self to a shallow copy of
 * the right hand side.
 */
RWHashTable&
RWHashTable::operator=(const RWHashTable& v)
{
  if (&v != this) {
    clear();			// Clear the slate

    size_t N = v.table_.length();	// Adopt the argument's length
    table_.reshape(N);

    // Now iterate through v's table, inserting a copy of each chain:
    for (size_t i=0; i<N; i++){
      table_(i) = v.table_(i) ? new BUCKET(*v.table_(i)) : rwnil;
    }
    nitems_ = v.nitems_;
  }
  return *this;
}

/************* Canonical collection class functions ****************/

// Operate on all entries.
void
RWHashTable::apply(RWapplyCollectable ap, void* x)
{
  size_t N = table_.length();
  for (size_t i=0; i<N; i++) 
    if(table_(i))
       table_(i)->apply(ap,x);
}

// clear all entries
void 
RWHashTable::clear()  
{
  for(size_t i = 0; i < table_.length(); i++)
  {
#ifdef __ATT__
    BUCKET* p = table_(i);	// Temporary necessary for cfront
    delete p;			// clears the list
#else
    delete table_(i);		// clears the list
#endif
    table_(i) = rwnil;		// indicates no data
  }
  nitems_ = 0;
}

// Find the first occurrence of "a":
RWCollectable*
RWHashTable::find(const RWCollectable* a) const
{
  RWPRECONDITION(a!=rwnil);
  BUCKET* p = this->table_(hashIndex(a));
  RWCollectable* t = p ? p->find(a) : rwnil;
  RWPOSTCONDITION(t==rwnil || t->isEqual(a));
  return t;
}

/*
 * Add an item to this table. Allow duplicates.
 */

RWCollectable*
RWHashTable::insert(RWCollectable* a)
{
  size_t idx = hashIndex(a);

  // Allocate a chain if one doesn't exist already:
  if(table_(idx) == rwnil)
    table_(idx) = new BUCKET;

  nitems_++;

  // Now insert into the chain:
  return table_(idx)->insert(a);
}

/*
 * Return the number of items that are equal to the item pointed
 * to by "a".  Because all such instances will be in the same bucket,
 * we need only search that bucket.
 */
size_t
RWHashTable::occurrencesOf(const RWCollectable* a) const
{
   BUCKET* p = this->table_(hashIndex(a));
   return p ? p->occurrencesOf(a) : 0;
}

/*
 * Remove and return one occurrence of a; if a is not in table return 0.
 */
RWCollectable*
RWHashTable::remove(const RWCollectable* a)   
{
  BUCKET* p = table_(hashIndex(a));
  RWCollectable* c = p ? p->remove(a) : rwnil;
  if (c)
    --nitems_;
  RWPOSTCONDITION(c==rwnil || c->isEqual(a));
  return c;
}

/****************************************************************
 *								*
 *			RWHashTable				*
 *			protected functions			*
 *								*
 ****************************************************************/


/*
 *  Resize adjusts the number of buckets, not the number of items.
 *  If number of items ~= (2/3)* number of slots,
 *  then the probability that the next insert will find an empty 
 *  slot is almost .5 (which indicates that about 1/2 the existing data 
 *  are in chains of at least length 2). We will use 3*items/2 as our
 *  default value. For small tables, we will force to 16 slots.
 */

void
RWHashTable::resize(size_t N)
{
#ifdef RWDEBUG
  size_t oldTally = entries();
#endif
  if (N==0) N = nitems_ < 12 ? 16 : nitems_*3/2 ;

  // Save old values -- NB: this involves a copy:
  RWGVector(RWSlistCollectablesP) tempTable =table_;
  table_.reshape(N);		// Resize internal table
  table_  = rwnil;		// Zero it
  nitems_ = 0;

  // Now iterate through the old collection, inserting each item
  for (size_t i=0; i<tempTable.length(); i++){
    if (tempTable(i)) {
      BUCKETITERATOR next(*tempTable(i));
      RWCollectable* p;
      while ( (p=next())!=rwnil )
        insert(p);
      BUCKET* ch = tempTable(i);		// Temporary necessary for cfront
      delete ch;
    }
  }
  RWPOSTCONDITION(oldTally==entries());
}

/*
 * Insert the item "a" at the index "idx":
 */
RWCollectable*
RWHashTable::insertIndex(size_t idx, RWCollectable* a)
{
  // Allocate a chain if one doesn't exist already:
  if(table_(idx) == rwnil) 
    table_(idx) = new BUCKET;
  nitems_++;
  // Now insert into the chain:
  return table_(idx)->insert(a);
}

