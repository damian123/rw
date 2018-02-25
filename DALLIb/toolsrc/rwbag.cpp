
/*
 * Definitions for RWBag --- an unordered collection with multiple occurrences.
 *
 * $Id: rwbag.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: rwbag.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:07:30  vriezen
 * Move all files to 6.1
 *
 * Revision 2.10  1994/01/13  07:53:50  jims
 * RWBag::remove now always returns victim unless not found.
 * Redefine removeAndDestroy to only destroy if no more occurrences.
 *
 * Revision 2.9  1993/11/09  09:37:36  jims
 * Port to ObjectStore
 *
 * Revision 2.8  1993/09/10  03:54:21  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.7  1993/05/17  20:20:26  dealys
 * merged with rwbagdf.cpp.
 *
 * Revision 2.6  1993/05/14  21:11:31  dealys
 * removed iterator code
 *
 * Revision 2.5  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.4  1993/02/08  17:19:59  keffer
 * Ported to SGI cfront.
 *
 * Revision 2.3  1993/02/07  18:48:30  keffer
 * Fixed bug in RWBag::clear().
 *
 * Revision 2.2  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.1  1992/11/05  18:54:28  keffer
 * Removed ^Z from end-of-file.
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.7   29 May 1992 09:46:54   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.6   27 May 1992 18:09:08   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.5   25 May 1992 15:36:10   KEFFER
 * Optimized size of returned RWBag in asBag().
 * 
 *    Rev 1.4   04 Mar 1992 09:17:26   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.3   28 Oct 1991 09:24:22   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.2   22 Aug 1991 10:47:18   keffer
 * Removed unnecessary local variable "key" in RWBag::clear().
 * 
 *    Rev 1.1   29 Jul 1991 14:22:00   keffer
 * Return type of asBag() is now RWBag.
 * 
 *    Rev 1.0   28 Jul 1991 08:39:34   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/defs.h"	/* Needed to avoid nestling limits in /lib/cpp */
#include "rw/rwbag.h"
#include "rw/collint.h"
#include "defcol.h"     

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: rwbag.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

RWDEFINE_COLLECTABLE2(RWBag, __RWBAG)

RWBag::RWBag(size_t N) :
  contents(N)
{
  totalEntries = 0;
}
                                                       

/************************************************
 *						*
 *		RWBag definitions		*
 *						*
 ************************************************/

/*
 * Copy constructor.  The copy constructor for the base class
 * will create a shallow copy of the old dictionary.  However,
 * it does *not* make copies of the RWCollectableInts used to keep
 * a tally of the number of times an item has been inserted.
 * This is done by the private function deepenTally().
 */
RWBag::RWBag(const RWBag& bag) :
  contents(bag.contents)
{
  totalEntries = bag.totalEntries;
  deepenTally();
}

RWBag::~RWBag()
{
  RWBag::clear();
}

void
RWBag::operator=(const RWBag& bag)
{
  contents = bag.contents;
  totalEntries = bag.totalEntries;
  deepenTally();
}

RWBoolean
RWBag::operator==(const RWBag& bag) const
{
  return totalEntries==bag.totalEntries && contents==bag.contents;
}

/**************** Definitions for Collection class functions **********************/

void
RWBag::apply(RWapplyCollectable ap, void* x)
{
  RWPRECONDITION( ap!=0 );
  RWBagIterator bi(*this);
  RWCollectable* item;
  while ( (item = bi()) != 0 )
    (*ap)(item, x);
}

void
RWBag::clear()
{
  RWHashDictionaryIterator hdi(contents);

  while ( (RWCollectable*)hdi() )   // Cast necessary for SGI compiler
  {
    RWCollectable* d = hdi.value(); // Temporary necessary for cfront.
    delete d;
  }
  contents.clear();
  totalEntries = 0;
}

void
RWBag::clearAndDestroy()
{
  contents.clearAndDestroy();
  totalEntries = 0;
}

RWCollectable*
RWBag::find( const RWCollectable* c ) const
{
  return contents.find(c);
}

RWCollectable*
RWBag::insert(RWCollectable* c)
{
  return insertWithOccurrences(c, 1);
}

/*
 * Insert "n" occurrences of an item into the bag.
 */
RWCollectable*
RWBag::insertWithOccurrences(RWCollectable* c, size_t n)
{
  RWCollectable* temp;
  RWCollectable* key = contents.findKeyAndValue(c, temp);

  if ( key==rwnil ) {
    // Not found.  Totally new entry.
    contents.insertKeyAndValue(c, new RWCollectableInt(n) );
  }
  else {
    RWCollectableInt* tally = (RWCollectableInt*)temp;
    tally->value(tally->value()+n);	// Increment tally.
    c = key;				// Return the old key.
  }
  totalEntries += n;			// Increment total tally.
  return c;
}

RWBoolean
RWBag::isEqual(const RWCollectable* c) const
{
  if( c->isA() != RWBag::isA() ) return FALSE;
  return RWBag::operator==( *(const RWBag*)c );
}

size_t
RWBag::occurrencesOf(const RWCollectable* c) const
{
  RWCollectable* temp;
  return contents.findKeyAndValue(c, temp) ? ((RWCollectableInt*)temp)->value() : 0;
}

/*
 * Remove an occurrence of an item and return it.  
 * Return nil if not found.
 */
RWCollectable*
RWBag::remove(const RWCollectable* c)
{
  RWCollectable* temp;
  RWCollectable* key = contents.findKeyAndValue(c, temp);

  // Was key in the dictionary?
  if (key) {
    // Yes.  Get its tally.
    RWCollectableInt* tally = (RWCollectableInt*)temp;
    if (tally->value()==1){	// Only one occurrence?
      contents.remove(c);	// If yes, remove it & delete the tally.
      delete tally;
    }
    else {
      tally->value(tally->value()-1); // More than one occurrence.  Decrement.
    }
    totalEntries--;			// Decrement total entries.
  }
  return key;
}

/*
 * removeAndDestroy only actually deletes the removed object
 * if no more occurrences remain in the bag.
 */
void RWBag::removeAndDestroy(const RWCollectable* target)
{
  RWCollectable* victim = remove(target);
  
  // Only destroy if not being used anymore:
  if (victim && occurrencesOf(victim) == 0)
    delete victim;
}
  
/************************* RWCollectable functions *****************************/

/*
 * NB: as a future enhancement, might consider supplying a 
 * definition for RWBag::saveGuts() and RWBag::restoreGuts(),
 * rather than inheriting the definitions from Collection.
 * The latter store every occurrence of an item (rather than
 * the total tally), which is bound to take up more disk space.
 */

/************************* Special functions **********************************/
RWBag
RWCollection::asBag() const
{
  RWBag aBag( entries()/2 );	// Pick a lambda of 2.0
  aBag += *this;
  return aBag;
}

void
RWBag::resize(size_t N)
{
  contents.resize(N);
}

void
RWBag::deepenTally()
{
  RWHashDictionaryIterator hdi(contents);

  while ( (RWCollectable*)hdi() )	// Cast necessary for SGI compiler
  {
    const RWCollectableInt* tally = (const RWCollectableInt*)hdi.value();
    hdi.value(new RWCollectableInt(*tally));
  }
}

