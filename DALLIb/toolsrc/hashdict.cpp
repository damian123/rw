
/*
 * Definitions for Hashed Dictionaries.
 *
 * $Id: hashdict.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * General strategy.  A dictionary stores keys and an associated value.  
 * We put these together in a special small class called a
 * RWCollectableAssociation which is created off the heap.  The association
 * is then stored in the base class hash table.  Because we created these
 * associations off the heap, this class is responsible for making sure
 * they get deleted at the proper time.
 *
 * Generally, the use of the association is transparent to the user.
 * This differs from the Smalltalk strategy.
 *
 ***************************************************************************
 *
 * $Log: hashdict.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:06:39  vriezen
 * Move all files to 6.1
 *
 * Revision 2.14  1994/03/03  00:16:45  jims
 * Override operator== so that the correct (non-virtual) operator<=
 * gets called
 *
 * Revision 2.13  1994/01/13  05:00:15  jims
 * Check that key and value pointers are not the same address in removeAndDestroy
 *
 * Revision 2.12  1993/11/09  08:06:08  jims
 * Port to ObjectStore
 *
 * Revision 2.11  1993/09/15  00:43:49  keffer
 * Corrected problem with RWHashDictionary::isEqual().
 *
 * Revision 2.10  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.9  1993/09/09  02:39:12  keffer
 * Copy constructor and assignment operator now based on copyContentsTo().
 *
 * Revision 2.8  1993/08/21  22:10:21  keffer
 * Changed return type of select().
 *
 * Revision 2.7  1993/07/03  23:55:58  keffer
 * Simplified and improved the clearAndDestroy() algorithm.
 *
 * Revision 2.6  1993/05/17  19:11:08  dealys
 * merged with hshdctdf.cpp
 *
 * Revision 2.5  1993/05/14  19:51:40  dealys
 * removed iterator code
 *
 * Revision 2.4  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.3  1993/03/20  22:59:43  keffer
 * Eliminated int to unsigned type conversion.
 *
 * Revision 2.1  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.8   29 May 1992 09:46:48   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.7   27 May 1992 18:09:04   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.6   25 May 1992 15:34:08   KEFFER
 * Optimized, reducing size.
 * 
 *    Rev 1.4   29 Apr 1992 14:50:52   KEFFER
 * Hashing now uses chaining to resolve collisions
 * 
 *    Rev 1.2   28 Oct 1991 09:24:12   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.1   08 Sep 1991 12:59:00   keffer
 * clearAndDestroy() now checks for already deleted keys and values.
 * 
 *    Rev 1.0   28 Jul 1991 08:39:24   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/hashdict.h"
#include "rw/collass.h"
#include "rw/idenset.h"
#include "rw/slistcol.h"
#include "defcol.h"   

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: hashdict.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

RWDEFINE_COLLECTABLE2(RWHashDictionary, __RWHASHDICTIONARY)

RWHashDictionary::RWHashDictionary(size_t N) : RWSet(N) { }       

RWHashDictionary::~RWHashDictionary()
{
  RWHashDictionary::clear();
}

/*
 * Dictionary copy constructor.  Start with an empty RWSet.
 * Then add copies of all items in the base class of self to it.
 * This will result in the associations being copied, but not the
 * collected items.
 */

RWHashDictionary::RWHashDictionary(const RWHashDictionary& d) :
  RWSet(0)
{
  RWPRECONDITION(entries()==0);
  d.RWSet::copyContentsTo(this);
  RWPOSTCONDITION(entries()==d.entries());
}

/*
 * Similar strategy for the assignment operator.
 */
void
RWHashDictionary::operator=(const RWHashDictionary& d)
{
  clear();		// Clear the playing field
  d.RWSet::copyContentsTo(this);
  RWPOSTCONDITION(entries()==d.entries());
}

RWBoolean
RWHashDictionary::operator<=(const RWHashDictionary& dict) const
{
  // easy case first
  if(entries() > dict.entries()) return FALSE;
  
  RWHashDictionaryIterator hdi(*(RWHashDictionary*)this);
  RWCollectable *key1, *value2;

  /*
   * Iterate over self.  For each key in self, find the corresponding
   * key in dict.  If the key does not occur in dict, return FALSE.
   * If the key occurs, check to see if the values match.
   */
  while ( (key1=hdi()) != 0 ){
    if ( !dict.findKeyAndValue(key1, value2) ) return FALSE;
    if ( !hdi.value()->isEqual(value2) ) return FALSE;
  }
  return TRUE;
}

RWBoolean
RWHashDictionary::operator==(const RWHashDictionary& h) const
{
  return nitems_==h.nitems_ && operator<=(h);
}

/********************************************************
*							*
*	      Methods for Class RWHashDictionary	*
*							*
*********************************************************/

void
RWHashDictionary::applyToKeyAndValue(RWapplyKeyAndValue ap, void* x)
{
  RWPRECONDITION( ap!=0 );
  RWSetIterator hi(*this);	// NB: Use RWSetIterator *not* RWHashDictionaryIterator
  RWCollectableAssociation*  ca;

  /* Iterate through the collection, extracting keys and values: */
  while( (ca = (RWCollectableAssociation*)hi()) != 0 )
    (*ap)(ca->key(), ca->value(), x);
}

void
RWHashDictionary::clear()
{
  RWSet::clearAndDestroy();	// Will delete associations held by base class
}

/*
 * Deletes all keys AND values.
 */
void
RWHashDictionary::clearAndDestroy()
{
  RWIdentitySet nukeSet(entries()/4); // Lambda of approximately 8
  RWHashDictionaryIterator hi(*this);
  RWCollectable* key;

  while( (key = hi()) != 0 )
  {
    nukeSet.insert(key);
    nukeSet.insert(hi.value());
  }
  nukeSet.clearAndDestroy();	// Nuke 'em

  clear();			// Will delete the associations
}

/*
 * Returns matching key.
 */
RWCollectable*
RWHashDictionary::find(const RWCollectable* key) const
{
  RWCollectableAssociation* a = findAssociation(key);
  return a ? a->key() : rwnil;
}

/*
 * Returns matching key, puts value in second argument.
 */
RWCollectable*
RWHashDictionary::findKeyAndValue(const RWCollectable* key, RWCollectable*& value) const
{
  RWCollectableAssociation* a = findAssociation(key);
  RWCollectable* ret;
  if (a) {
    ret = a->key();
    value =  a->value();
  }
  else{
    ret = rwnil;
    value = rwnil;
  }
  return ret;
}

/*
 * Given a key, returns the associated value.
 */
RWCollectable*
RWHashDictionary::findValue(const RWCollectable* key) const
{
  RWCollectableAssociation* a = findAssociation(key);
  return a ? a->value() : rwnil;
}

/*
 * Given a key, replaces the associated value.  Returns the old value.
 * Returns rwnil if the value cannot be found.
 */
RWCollectable*
RWHashDictionary::findValue(const RWCollectable* key, RWCollectable* newValue) const
{
  RWCollectableAssociation* a = findAssociation(key);
  return a ? a->value(newValue) : rwnil;
}

/*
 * Returns the key if successful.  If the key is already
 * in the Dictionary, then returns rwnil.
 */
RWCollectable*
RWHashDictionary::insertKeyAndValue(RWCollectable* key, RWCollectable* val)
{
  RWCollectableAssociation* a = newAssociation(key, val);
  RWCollectableAssociation* b = (RWCollectableAssociation*)RWSet::insert(a);
  if ( a != b ) {
    delete a;			// Key already in the hash table.
    return rwnil;
  }
  else
    return key;
}

RWBoolean
RWHashDictionary::isEqual(const RWCollectable* c) const
{
  if( c->isA() != __RWHASHDICTIONARY ) return FALSE;
  return RWHashDictionary::operator==(*(const RWHashDictionary*)c);
}

RWCollection*
RWHashDictionary::select(RWtestCollectable t, void* x) const
{
   RWHashDictionary *sd = (RWHashDictionary*)RWCollection::select(t, x);
   sd->copyAssociations();
   return sd;
}

/*
 * Removes the key and value from the dictionary and returns the key.
 */
RWCollectable*
RWHashDictionary::remove(const RWCollectable* key)
{
  RWCollectable* ret;
  RWCollectableAssociation* a = removeAssociation(key);
  if(a){
    ret = a->key();
    delete a;
  }
  else
    ret = rwnil;
  return ret;
}

/*
 * Removes the key and value from a dictionary AND deletes both of them.
 */
void
RWHashDictionary::removeAndDestroy(const RWCollectable* key)
{
  RWCollectableAssociation* a = removeAssociation(key);
  if(a){
    RWCollectable* deadKeyMeat   = a->key();
    RWCollectable* deadValueMeat = a->value();

    if (deadKeyMeat != deadValueMeat) {
      delete deadKeyMeat;
      delete deadValueMeat;
    } 
    else {
      delete deadKeyMeat;
    }
    delete a;
  }
}

/*
 * Removes the key and value from the dictionary, returns the key, puts
 * the value in the second argument.
 */
RWCollectable*
RWHashDictionary::removeKeyAndValue(const RWCollectable* key, RWCollectable*& value)
{
  RWCollectableAssociation* a = removeAssociation(key);
  RWCollectable* ret;
  if (a) {
    ret = a->key();
    value = a->value();
    delete a;
  }
  else{
    ret = rwnil;
    value = rwnil;
  }
  return ret;
}

/****************************************************************
 *								*
 *			RWHashDictionary			*
 *			protected functions			*
 *								*
 ****************************************************************/

static void replaceWithCopy(void*& a, void*)
{
  a = ((RWCollectable*)a)->copy();
}

/* 
 * Make a (shallow) copy of each association.  Note that this version
 * will work for both value and identity associations.
 */
void
RWHashDictionary::copyAssociations()
{
  for (size_t i=0; i<buckets(); i++) {
    if (table_(i)) {
      table_(i)->applyReference(replaceWithCopy,0);
    }
  }
}

/*
 * Protected virtual function to find the association for a given
 * key, or return nil if there is no such association.
 */
RWCollectableAssociation*
RWHashDictionary::findAssociation(const RWCollectable* key) const
{
  return (RWCollectableAssociation*)RWSet::find(key);
}

/*
 * Protected virtual function to return a new association of the type
 * appropriate for this kind of dictionary.
 */
RWCollectableAssociation*
RWHashDictionary::newAssociation(RWCollectable* k, RWCollectable* v) const
{
  return new RWCollectableAssociation(k,v);
}

/* Protected virtual function to remove and return the association
 * for a given key, or return nil if there is no such association.
 */
RWCollectableAssociation*
RWHashDictionary::removeAssociation(const RWCollectable* key)
{
  return (RWCollectableAssociation*)RWSet::remove(key);
}

