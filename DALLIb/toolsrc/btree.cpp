
/*
 * Definitions for B-Trees.
 *
 * $Id: btree.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: btree.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:05:29  vriezen
 * Move all files to 6.1
 *
 * Revision 2.7  1993/11/09  05:27:54  jims
 * Port to ObjectStore
 *
 * Revision 2.6  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.5  1993/05/18  16:10:49  dealys
 * merged with btreedf.cpp
 *
 * Revision 2.4  1993/04/09  23:15:26  keffer
 * Renamed auto variable "tempKey" to "temp" to avoid confusion with
 * member data of the same name.
 *
 * Revision 2.3  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.2  1993/03/20  22:02:30  keffer
 * Eliminated int to unsigned type conversions.
 *
 * Revision 2.1  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.5   29 May 1992 09:46:38   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.4   27 May 1992 18:08:56   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.3   22 May 1992 17:09:46   KEFFER
 * Changed the names of some global variables to reduce potential collisions.
 * 
 *    Rev 1.2   04 Mar 1992 09:17:16   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.1   28 Oct 1991 09:23:52   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 08:38:48   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

/*
 * Algorithms are based on programs presented in Chapter 6 of: 
 *
 * L. Ammeraal, Programs and Data Structures in C
 * John Wiley & Sons
 * Chichester, NY, 1989, 206 pp.
 * ISBN 0-471-91751-6  
 * 
 * (or see also Knuth: Vol. 3)
 */

#include "rw/btree.h"
#include "defcol.h" 

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: btree.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

enum returnStatus {more, success, ignored};

RWDEFINE_COLLECTABLE2(RWBTree, __RWBTREE)

RWBTree::RWBTree()
{
  root = rwnil;
  tempNode = rwnil;
  tempKey = rwnil;
}                                           

/*********************************************************
*					 		  *
*	      Methods for Class RWBTreeNode		  *
*							  *
**********************************************************/

RWBTreeNode::RWBTreeNode(RWCollectable* k)
{
  initialize();
  key[0]  = k;
  counter = 1;
}

RWBTreeNode::RWBTreeNode()
{
  initialize();
}

void
RWBTreeNode::initialize()
{
  counter = 0;
  for (register unsigned i = 0; i< rworder2; i++) {
    next[i] = rwnil;
    key[i]  = rwnil;
  }
  next[rworder2] = rwnil;  
}

// binary search of node for key a
//    returns:	0 for a <= all keys 
//		1 for a > all keys
//		r, where  key[r-1] < a <= key[r]
unsigned
RWBTreeNode::binarySearch(const RWCollectable* a)	const
{
  unsigned n = counter-1;
  if ( key[0]->compareTo(a) >= 0 ) return 0;
  if ( key[n]->compareTo(a) < 0 ) return counter;
  unsigned l = 0;
  unsigned r = n;
  while (r-l > 1) {
    unsigned i = r+l >> 1;     // bitwise divide by two
    if ( key[i]->compareTo(a) >= 0 )
      r=i;
    else
      l=i;
  }
  return r;
}

/*
 * Count the number of items in self and all daughters.
 */
void 
RWBTreeNode::siz(size_t& nitems) const
{
  /* Take a look in each of my daughters: */
  for (register size_t i=0; i < counter+1; i++)
    if( next[i] ) next[i]->siz(nitems);

  /* Now count myself: */
  nitems += counter;
}

/*
 * Returns TRUE if all keys in self and daughters 
 * are in the RWBTree bt.
 */
RWBoolean
RWBTreeNode::subSetOf(const RWBTree& bt) const
{
  // Check all daughters:
  for(unsigned i=0; i<counter+1; i++)
    if( next[i] && !next[i]->subSetOf(bt) ) return FALSE;

  // Check all keys:
  for(i=0; i<counter; i++)
    if( !bt.contains(key[i]) ) return FALSE;

  return TRUE;
}
    
/*********************************************************
*					 		  *
*	      Methods for Class RWBTree			  *
*							  *
**********************************************************/

RWBTree::RWBTree(const RWBTree& bt)
{
  root = rwnil;
  tempNode = rwnil;
  tempKey = rwnil;
  *this += bt;
}
  
RWBTree::~RWBTree()
{
  RWBTree::clear();
}

void
RWBTree::operator=(const RWBTree& bt)
{
  RWBTree::clear();
  *this += bt;
}

/*
 * Is self a subset of bt?  Note that the empty collection
 * is a subset of any collection.
 */
RWBoolean
RWBTree::operator<=(const RWBTree& bt) const
{
  return root ? root->subSetOf(bt) : TRUE;
}

RWBoolean
RWBTree::operator==(const RWBTree& bt) const
{
  return RWBTree::entries()==bt.RWBTree::entries() && operator<=(bt);
}

unsigned
RWBTree::height() const
{
  if (isEmpty()) return 0;	// Empty tree.
  unsigned h=1;			// At least root node.
  RWBTreeNode* temp=root;
  while( temp->next[0] ){
    temp=temp->next[0];
    h++;
  }
  return h;
}

void
RWBTree::apply(RWapplyCollectable ap, void* x)
{
   apl(root, ap, x);
}
  
// delete all nodes
void 
RWBTree::clear()
{
   del(root);
   root=rwnil;
}

// Number of items in tree:
size_t 
RWBTree::entries() const
{
   size_t nitems = 0;
  if(root) root->siz(nitems);
   return nitems;
}

/*
 * Find an item that compares equal.  Uses the
 * virtual function compareTo() to zero in on the item.
 */
RWCollectable*
RWBTree::find(const RWCollectable* k) const
{
  unsigned i;
  RWBTreeNode* temp = root; 
  while ( temp != rwnil ) {
    i = temp->binarySearch(k);	// Search current node.
    if (i < temp->counter && temp->key[i]->compareTo(k) == 0 )
       return temp->key[i];	// Found it!
    temp = temp->next[i];	// Follow tree to next node.
  }
  return rwnil;	// not found.
}

/*
 * Add an item to the tree.  Return it if successful.  
 * If it is already in the tree, then return the old instance.
 */
RWCollectable*
RWBTree::insert(RWCollectable* k)
{
  int status = ins(k, root);	    // Recursive function to attempt insertion.
  if (status == success) return k;  // Successful uncomplicated insertion.
  else if (status == ignored) return tempKey;	// Item already present--ignore
  RWBTreeNode* newNode = new RWBTreeNode(tempKey); // Create new root node.
  newNode->next[0] = root;			// Assign pointers to children.
  newNode->next[1] = tempNode;
  root = newNode;
  return k;
}
      
RWBoolean
RWBTree::isEqual(const RWCollectable* c) const
{
  if( c->isA() != RWBTree::isA() ) return FALSE;
  return operator==( *(const RWBTree*)c );
}

size_t
RWBTree::occurrencesOf(const RWCollectable* a) const
{
  return find(a) ? 1 : 0;	// This works because no duplicates are allowed
}

RWCollectable*
RWBTree::remove(const RWCollectable* k)
{
  RWCollectable* victim = 0;
  
  // Check for uncomplicated removal:
  if ( rem(k, root, victim) == more ){
    // If underflow, decrease the height of the tree:
    RWBTreeNode* newRoot = root->next[0];
    delete root; 
    root = newRoot;
  }
  return victim;
}

/********************************************************
*							*
*	      Protected methods for Class RWBTree		*
*							*
*********************************************************/

// Recursive function to examine all member items:
void 
RWBTree::apl(RWBTreeNode* t, RWapplyCollectable ap, void* x) 
{
   if(t) {
     for (unsigned i=0; i < t->counter; i++){
       apl(t->next[i], ap, x);	// First examine all children to left...
       (*ap)(t->key[i], x);	//    then the key.
     }
     apl(t->next[t->counter], ap, x); // Finally, do the last daughter.
   }
}

void
RWBTree::del(RWBTreeNode* t)
{
  unsigned i;		    
  if (t) { 
    unsigned n = t->counter+1;
    for (i=0; i<n; i++) del(t->next[i]);
    delete t;
  }
}

// Recursive function to insert "k" in tree rooted at b:
int
RWBTree::ins(RWCollectable* k, RWBTreeNode* b)
{
  unsigned i, j;
  int status;

  /*
   * Check if b is a pointer in a leaf.  
   * If it is, you can't put the item in here. 
   */
  if (b == rwnil){
    tempNode = rwnil;
    tempKey = k;
    return more;
  }

  i = b->binarySearch(k);		// Search node.

  if (i < b->counter && b->key[i]->compareTo(k)==0 ){
    tempKey = b->key[i];		// Duplicate key found.  Remember where it is.
    return ignored;			// Signal the problem.
  }

  status = ins(k, b->next[i]);		// Recursive call to ins()
  if (status!=more) return status;	// Successful uncomplicated insertion.

  /*
   * Insertion in subtree did not completely succeed;
   * try to insert the overflow in the current node:
   */
  if (b->counter < rworder2) {
    /*
     * There's room in this node.  Figure out where to put the
     * overflow, then slide everything over.
     */
    i = b->binarySearch(tempKey);
    for (j=b->counter; j>i; j--) {
       b->key[j]    = b->key[j-1];
       b->next[j+1] = b->next[j];
    }
    /* Slide in the overflow in the proper slot: */
    b->key[i]    = tempKey;
    b->next[i+1] = tempNode;
    b->counter++;
    return success;		// Signal all is OK.
  }

  /*
   * No room in current node.  We'll have to split it.
   * Pass item key[rworder] (which is in the middle of the augmented
   * sequence) back up the tree for someone else to deal with it.
   * Also, pass the address of the new node back up. 
   */
  RWCollectable* lastKey;
  RWBTreeNode* lastPointer;

  if (i == rworder2) {
     lastKey     = tempKey;
     lastPointer = tempNode;
  } 
  else {
     lastKey     = b->key[rworder2-1];
     lastPointer = b->next[rworder2];
     for (j=rworder2-1; j>i; j--) {
	 b->key[j]    = b->key[j-1];
	 b->next[j+1] = b->next[j];
     }
     b->key[i]    = tempKey;
     b->next[i+1] = tempNode;
  }

  tempNode = new RWBTreeNode();	// Get a new node.
  tempNode->counter = rworder;	// Set its counter.
  tempKey = b->key[rworder];	// Get the key that will get passed back up.
  b->counter = rworder;		// Set the new counter.

  /* Transfer the info over to the new node: */
  for (j=0; j<rworder-1; j++) {
    tempNode->key[j]  = b->key[j+rworder+1];
    tempNode->next[j] = b->next[j+rworder+1];
  }

  tempNode->next[rworder-1] = b->next[rworder2];
  tempNode->key[rworder-1]  = lastKey;
  tempNode->next[rworder]   = lastPointer;
  return more;
}
  
/* Recursive function to remove key "a" from tree rooted at b: */
int 
RWBTree::rem(const RWCollectable* a, RWBTreeNode* b, RWCollectable*& victim)
{
  if (b == rwnil) return ignored;	// No node.
  
  unsigned     		i,j;
  unsigned     		leftCounter, rightCounter;
  RWCollectable*	temp;
  RWBTreeNode* 		leftNode;
  RWBTreeNode* 		rightNode;
  
  i = b->binarySearch(a);	// Search current node.
  
  if (b->next[0] == rwnil) {	// b is a terminal node.
    if (i == b->counter || b->key[i]->compareTo(a) > 0 )
      return ignored;                                   
    victim = b->key[i];
    for (j=i+1; j<b->counter; j++) {	// Remove item.
      b->key[j-1]  = b->key[j];    
      b->next[j]   = b->next[j+1];
    }
    --b->counter;	// decrement field index
    return b->counter >= (b==root ? 1 : rworder);
  }
  
  // b is not a terminal node.
  
  temp        =  b->key[i];
  leftNode    =  b->next[i];
  leftCounter =  leftNode->counter;
  if( i < b->counter && temp->compareTo(a) == 0 ) { // found it!
    
    // Go to left child, then follow rightmost branches to a terminal node.
    
    RWBTreeNode* tempNode1 = b->next[i];
    unsigned tempCounter = tempNode1->counter;
    RWBTreeNode* tempNode2;
    while (tempNode2 = tempNode1->next[tempCounter], tempNode2 != rwnil) {
      tempNode1   = tempNode2;
      tempCounter = tempNode1->counter;
    }
    
    // Exchange item in b with last item in terminal node.
    
    b->key[i]   =  tempNode1->key[tempCounter-1];
    tempNode1->key[tempCounter-1]  =  temp;
    temp    =  b->key[i];
  }
  
  // Now find and remove a.
  
  int status = rem(a, leftNode, victim);    // Recursive function to attempt removal.
  if (status != more) return status;      // Successful removal.
  
  // Underflow:  Borrow item from right sibling if possible, otherwise borrow 
  //   from left. If underflow occurs in  sibling too, merge nodes.
  if (i == b->counter) {   // Borrow from left sibling.
    
    temp      = b->key[i-1];
    leftNode  = b->next[i-1];
    leftCounter = leftNode->counter;
    
    rightNode = b->next[i];
  }
  else 
    rightNode = b->next[i+1];	     //Borrow from right sibling. 
  rightCounter = rightNode->counter;
  
  if (i == b->counter) {   // Borrow from left sibling.
    rightNode->next[rightCounter+1] =  rightNode->next[rightCounter];
    for (j=rightCounter; j>0; j--) {
      rightNode->key[j]  = rightNode->key[j-1];
      rightNode->next[j] = rightNode->next[j-1];
    }
    ++rightNode->counter;
    rightNode->key[0]  = temp;
    rightNode->next[0] = leftNode->next[leftCounter];
    b->key[i-1]  = leftNode->key[leftCounter-1];
    temp         = leftNode->key[leftCounter-1];
    if (--leftNode->counter >= rworder) return success;
  }  
  else {      // Borrow from right sibling.
     if (rightCounter > rworder)	{     // Merging not necessary.
         leftNode->key[rworder-1]  = temp;
	  leftNode->next[rworder]   = rightNode->next[0];
	  b->key[i]   = rightNode->key[0];
	  temp        = rightNode->key[0];
	  ++leftNode->counter;
	  --rightNode->counter;
	  unsigned c = rightNode->counter;
	  for (j=0; j<c; j++) {
	      rightNode->next[j] =  rightNode->next[j+1];
	      rightNode->key[j]  =  rightNode->key[j+1];
	  }
	  rightNode->next[c] =  rightNode->next[c+1];
	  return success;
     }
  }

  // If you get to here, nodes must be merged.
  leftNode->key[rworder-1]  = temp;
  leftNode->next[rworder]   = rightNode->next[0];
  for (j=0; j<rworder; j++) {
      leftNode->next[rworder+j+1] =  rightNode->next[j+1];
      leftNode->key[rworder+j]   =  rightNode->key[j];
  }

  leftNode->counter =  rworder2;
  delete rightNode;

  for (j=i+1; j<b->counter; j++) {  // fix up parent node
      b->key[j-1]  = b->key[j];
      b->next[j]   = b->next[j+1];
  }
  return --b->counter >= (b==root ? 1 : rworder);
}

