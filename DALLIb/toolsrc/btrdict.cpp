/*
 * Definitions for B-Tree Dictionaries
 *
 * $Id: btrdict.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: btrdict.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:05:27  vriezen
 * Move all files to 6.1
 *
 * Revision 2.10  1994/03/04  01:04:16  jims
 * Override isEqual member function from RWCollectable to return
 * TRUE or FALSE based on operator==
 *
 * Revision 2.9  1994/01/13  04:59:41  jims
 * Check that key and value pointers are not the same address in removeAndDestroy
 *
 * Revision 2.8  1993/11/09  05:27:54  jims
 * Port to ObjectStore
 *
 * Revision 2.7  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.6  1993/09/09  02:39:12  keffer
 * Copy constructor and assignment operator now based on copyContentsTo().
 *
 * Revision 2.5  1993/07/03  23:55:58  keffer
 * Simplified and improved the clearAndDestroy() algorithm.
 *
 * Revision 2.4  1993/05/18  16:05:59  dealys
 * merged with btrdictd.cpp
 *
 * Revision 2.3  1993/03/01  16:22:07  keffer
 * Added text messages to PRECONDITION clauses.
 *
 * Revision 2.2  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 *    Rev 1.5   27 May 1992 18:08:56   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.4   04 Mar 1992 09:17:16   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.3   28 Oct 1991 09:23:50   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.2   08 Sep 1991 12:58:42   keffer
 * clearAndDestroy() now checks for already deleted keys and values.
 * 
 *    Rev 1.1   22 Aug 1991 10:55:24   keffer
 * RWBTreeDictStruct is no longer declared static
 * 
 */

#include "rw/btrdict.h"
#include "rw/collass.h"
#include "rw/idenset.h"
#include "defcol.h"
                     
RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: btrdict.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

RWDEFINE_COLLECTABLE2(RWBTreeDictionary, __RWBTREEDICTIONARY)

RWBTreeDictionary::RWBTreeDictionary() : RWBTree() { }                        

/*
 * Dictionary copy constructor.  Start with an empty RWBTree.
 * Then add copies of all items in the base class of self to it.
 * This will result in the associations being copied, but not the
 * collected items.
 */

RWBTreeDictionary::RWBTreeDictionary(const RWBTreeDictionary& btrdict)
 : RWBTree()
{
  RWPRECONDITION(entries()==0);
  btrdict.RWBTree::copyContentsTo(this);
  RWPOSTCONDITION(entries()==btrdict.entries());
}

/*
 * Similar strategy for the assignment operator.
 */
void
RWBTreeDictionary::operator=(const RWBTreeDictionary& btrdict)
{
  clear();
  btrdict.RWBTree::copyContentsTo(this);
  RWPOSTCONDITION(entries()==btrdict.entries());
}

RWBTreeDictionary::~RWBTreeDictionary()
{
  RWBTreeDictionary::clear();
}

/********************************************************
*							*
*	      Methods for Class RWBTreeDictionary	*
*							*
**********************************************************/

/*
 * The following is designed to allow examination of key and value
 * pairs without using a RWBTree Iterator (which we don't have).
 * Basically, we repackage the call to applyToKeyAndValue() as a call 
 * to the base class's apply(), using a special struct to hold the
 * necessary values.   This approach is recursive.
 */

struct RWBTreeDictStruct {
  RWapplyKeyAndValue	functionToApply;
  void*			clientData;
};

static void
applyAssociation(RWCollectable* c, void* capsule)
{
  struct RWBTreeDictStruct* temp	= (struct RWBTreeDictStruct*)capsule;
  RWCollectableAssociation* ca	= (RWCollectableAssociation*)c;
  (temp->functionToApply)(ca->key(), ca->value(), temp->clientData);
}

// Member function to examine keys and values
void	 
RWBTreeDictionary::applyToKeyAndValue(RWapplyKeyAndValue ap, void* x)
{
  RWPRECONDITION2( ap!=rwnil,"RWBTreeDictionary::applyToKeyAndValue(RWapplyKeyAndValue ap, void* x): nil function pointer" );
  RWBTreeDictStruct capsule;
  capsule.clientData		= x;
  capsule.functionToApply	= ap;
  RWBTree::apply(applyAssociation, &capsule);
}

void
RWBTreeDictionary::clear()
{
  RWBTree::apply(rwDestroy, 0);
  RWBTree::clear();
}

static void
addKeyAndValue(RWCollectable* key, RWCollectable* val, void* x)
{
  ((RWCollection*)x)->insert(key);
  ((RWCollection*)x)->insert(val);
}

/*
 * Deletes all keys AND values.
 */
void
RWBTreeDictionary::clearAndDestroy()
{
  RWIdentitySet nukeSet(entries()/4); // Lambda of approximately 8
  applyToKeyAndValue(addKeyAndValue, &nukeSet);
  nukeSet.clearAndDestroy();	// Nuke 'em
  clear();			// Will delete the associations.
}

/*
 * Returns matching key.
 */
RWCollectable*  
RWBTreeDictionary::find(const RWCollectable* key) const
{
  RWPRECONDITION2( key!=rwnil,"RWBTreeDictionary::find(const RWCollectable* key) const: nil pointer" );
  RWCollectableAssociation* a = (RWCollectableAssociation*)RWBTree::find(key);
  return a ? a->key() : rwnil;
}

/*
 * Returns matching key, puts value in second argument.
 */
RWCollectable*	
RWBTreeDictionary::findKeyAndValue(const RWCollectable* key, RWCollectable*& value) const
{
  RWPRECONDITION2( key!=rwnil,"RWBTreeDictionary::findKeyAndValue(const RWCollectable*, RWCollectable*& ) const: nil pointer" );
  RWCollectableAssociation* a = (RWCollectableAssociation*)RWBTree::find(key);
  RWCollectable* ret;
  if (a) {
    ret = a->key();
    value = a->value();
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
RWBTreeDictionary::findValue(const RWCollectable* key) const
{
  RWPRECONDITION2( key!=rwnil,"RWBTreeDictionary::findValue(const RWCollectable* key) const: nil pointer" );
  RWCollectableAssociation* a = (RWCollectableAssociation*)RWBTree::find(key);
  return a ? a->value() : rwnil;
}

/*
 * Given a key, replaces the associated value.  Returns the old value.
 * Returns nil if the value cannot be found.
 */
RWCollectable*
RWBTreeDictionary::findValue(const RWCollectable* key, RWCollectable* newValue) const
{
  RWPRECONDITION2( key!=rwnil,"RWBTreeDictionary::findValue(const RWCollectable* , RWCollectable* ) const: nil pointer" );
  RWCollectableAssociation* a = (RWCollectableAssociation*)RWBTree::find(key);
  return a ? a->value(newValue) : rwnil;
}

/*
 * Returns the key if successful.  If the key is already
 * in the Dictionary, then returns rwnil.
 */
RWCollectable*
RWBTreeDictionary::insertKeyAndValue(RWCollectable* key, RWCollectable* val)
{

  RWPRECONDITION2( key!=rwnil,"RWBTreeDictionary::insertKeyAndValue(RWCollectable* key, RWCollectable* val): nil pointer" );
  RWCollectableAssociation* a = new RWCollectableAssociation(key, val);
  RWCollectableAssociation* b = (RWCollectableAssociation*)RWBTree::insert(a);
  if ( a != b ) {
    delete a;			// Key already in the tree.
    return rwnil;
  }
  else
    return key;
}

RWBoolean
RWBTreeDictionary::isEqual(const RWCollectable* c) const
{
  if( c->isA() != __RWBTREEDICTIONARY ) return FALSE;
  return RWBTreeDictionary::operator==(*(const RWBTreeDictionary*)c);
}

/*
 * Removes the key and value from the dictionary and returns the key.
 */
RWCollectable*  
RWBTreeDictionary::remove(const RWCollectable* key)
{
  RWPRECONDITION2( key!=rwnil,"RWBTreeDictionary::remove(const RWCollectable* key): nil pointer" );
  RWCollectable* ret;
  RWCollectableAssociation* a = (RWCollectableAssociation*)RWBTree::remove(key);
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
RWBTreeDictionary::removeAndDestroy(const RWCollectable* key)
{
  RWPRECONDITION2( key!=rwnil,"RWBTreeDictionary::removeAndDestroy(const RWCollectable* key): nil pointer" );
  RWCollectableAssociation* c = (RWCollectableAssociation*)RWBTree::remove(key);
  if(c){
    RWCollectable* deadKeyMeat   = c->key();
    RWCollectable* deadValueMeat = c->value();

    if (deadKeyMeat != deadValueMeat) {
      delete deadKeyMeat;
      delete deadValueMeat;
    } 
    else {
      delete deadKeyMeat;
    }
    delete c;
  }
}

/*
 * Removes the key and value from the dictionary, returns the key, puts
 * the value in the second argument.
 */
RWCollectable*	 
RWBTreeDictionary::removeKeyAndValue(const RWCollectable* key, RWCollectable*& value)
{
  RWPRECONDITION2( key!=rwnil,"RWBTreeDictionary::removeKeyAndValue(const RWCollectable*, RWCollectable*&): nil pointer" );
  RWCollectableAssociation* a = (RWCollectableAssociation*)RWBTree::remove(key);
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
