/*
 * Definitions for binary tree class.
 *
 * $Id: bintree.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * General note: One must be careful what you compare to what.
 * In general, the items in the collection shoudl be compared to
 * the key, rather than vice versa.  I.e., use
 *     t->e->compareTo(a);
 * rather than
 *     a->compareTo(t->e);
 * This is because the items in the collection may be supersets
 * of the key: it is up to them to decide what their key is.
 *
 * To clarify the code below, we actually use 
 *     -(t->e->compareTo(a))
 * so that the sense of the compare is to go left if less than
 * zero.
 *
 ***************************************************************************
 *
 * $Log: bintree.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:04:54  vriezen
 * Move all files to 6.1
 *
 * Revision 2.11  1993/11/06  05:12:20  jims
 * Port to ObjectStore
 * Move declaration for RWTreeNode back to header file
 *
 * Revision 2.10  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.9  1993/05/17  18:39:41  dealys
 * merged with bintrdf.cpp
 *
 * Revision 2.8  1993/05/14  18:37:02  dealys
 * removed iterator code.
 *
 * Revision 2.7  1993/04/14  18:39:16  keffer
 * Moved declaration for RWTreeNode into the .cpp file.
 * Now does level saveGuts(), removing the need for a balance()
 * on restore.
 *
 * Revision 2.6  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.5  1993/04/01  00:25:07  myersn
 * add prefix RW to mention of GQueue.
 *
 * Revision 2.4  1993/03/20  17:39:10  keffer
 * Avoided int to unsigned conversion.
 *
 * Revision 2.3  1993/02/27  00:18:12  keffer
 * Added text messages to PRECONDITION clauses.
 *
 * Revision 2.2  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.1  1992/11/05  03:29:29  keffer
 * Changed return type of RWCollection::asSortedCollection() to RWBinaryTree.
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 *    Rev 1.4   29 May 1992 09:46:34   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.3   27 May 1992 18:08:54   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.2   04 Mar 1992 09:17:14   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.1   28 Oct 1991 09:23:48   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 08:38:42   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */


#include "rw/bintree.h"
#include "defcol.h"       

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: bintree.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

declare(RWGQueue, RWTreeNode)

RWDEFINE_COLLECTABLE2(RWBinaryTree, __RWBINARYTREE)


RWBinaryTree::RWBinaryTree()
{
  root=rwnil;
}                                                     


/****************************************************************
 *								*
 *			RWBinaryTree				*
 *								*
 ****************************************************************/

// This copy constructor constructs a balanced binary tree from a binary tree.
RWBinaryTree::RWBinaryTree(const RWBinaryTree& bt) 
{
  root = rwnil;
  *this += bt;
  balance();
}

RWBinaryTree::~RWBinaryTree()
{
  RWBinaryTree::clear();
}

void
RWBinaryTree::operator=(const RWBinaryTree& bt)
{
  RWBinaryTree::clear();
  *this += bt;
  balance();
}

RWBoolean
RWBinaryTree::operator==(const RWBinaryTree& bt) const
{
  return RWBinaryTree::entries() == bt.RWBinaryTree::entries() && operator<=(bt);
}

/*
 * Return TRUE if self is a subset of the argument.
 * Because duplicates are allowed, each item in self
 * must be present the same number of times or less.
 */
RWBoolean
RWBinaryTree::operator<=(const RWBinaryTree& arg) const
{
  RWBinaryTreeIterator argIt(arg);
  RWBinaryTreeIterator myIt(*this);
  RWCollectable* item;
  /* This works because these are sorted collections: */
  while( (item = myIt()) != 0)
    if(!argIt.findNext(item)) return FALSE;
  return TRUE;
}

/*
 * Operate on all nodes using the user-supplied function ap.
 */

void 
RWBinaryTree::apply(RWapplyCollectable ap, void* x)
{
  RWPRECONDITION2( ap!=0, "RWBinaryTree::apply(RWapplyCollectable, void*): nil function pointer" );
  applyChildren(root, ap, x);
}

// Protected function to control tree traversal 
void
RWBinaryTree::applyChildren(const RWTreeNode* t, RWapplyCollectable ap, void* x)
{
  if (t) {
    applyChildren(t->left,ap,x);
    (*ap)(t->e,x);
    applyChildren(t->right,ap,x);
  }
}

// Balance a binary tree.
void
RWBinaryTree::balance() 
{
  if (isEmpty()) return;

  RWBinaryTreeIterator nextItem(*this);    // Allocate an iterator.
  RWCollectable* theItem;
  RWCollectable* thePreviousItem;

  RWGQueue(RWCollectable) 	treeQueue;
  RWGQueue(RWCollectable) 	duplicateQueue;

  theItem = nextItem();   // Get the first item.
  thePreviousItem = theItem; 
  treeQueue.append(theItem);

  while ( ( theItem = nextItem() ) != rwnil ) {
    if (theItem->compareTo(thePreviousItem)) {  // Non-duplicate.
        treeQueue.append(theItem);
        thePreviousItem = theItem;  // Update "previous item" pointer.
    }
    else     // Duplicate.
    	duplicateQueue.append(theItem);
  }
  clear();  // Clear the original tree.
  while ( (theItem = treeQueue.get()) != 0 )  insert(theItem);
  balanceUnique();
  while ( (theItem = duplicateQueue.get()) != 0 )  insert(theItem);
}

// Protected function to balance a tree with no duplicate elements.
void
RWBinaryTree::balanceUnique()
{
  RWCollectable* 	theItem;
  RWGQueue(RWCollectable) 	treeQueue;
  RWBinaryTreeIterator 	nextItem(*this);
  while ( (theItem = nextItem()) != 0 ) treeQueue.append(theItem);
  clear();
  size_t  n = treeQueue.entries();
  root = balanceChildren(n, treeQueue);
}

RWTreeNode*
RWBinaryTree::balanceChildren(size_t n, RWGQueue(RWCollectable)& gq)
{
  if (n==0) return rwnil;
  size_t nleft   = (n - 1) >> 1;
  size_t nright  = n - nleft - 1;
  
  RWTreeNode* p = new RWTreeNode(rwnil);
  p->left     = balanceChildren(nleft, gq);
  p->e        = gq.get();
  p->right    = balanceChildren(nright, gq);
  return p;
}

void
RWBinaryTree::clear()
{
  deleteChildren(root);
  root = rwnil;
}

void
RWBinaryTree::deleteChildren(RWTreeNode* t)
{
  if(t){
    deleteChildren(t->left);
    deleteChildren(t->right);
    delete t;
  }
}

size_t
RWBinaryTree::entries() const
{
  size_t total=0;
  countChildren(root, total);
  return total;
}

void
RWBinaryTree::countChildren(const RWTreeNode* t, size_t& total) const
{
  if(t){
    countChildren(t->left, total);
    countChildren(t->right, total);
    total++;			// Count self.
  }
}

RWCollectable*  
RWBinaryTree::find(const RWCollectable* a) const
{
  RWPRECONDITION2(a!=rwnil,"RWBinaryTree::find(const RWCollectable*): nil pointer");
  RWTreeNode* t = root;
  while(t) {
    int compare = -t->e->compareTo(a);
    if ( compare == 0 ) return t->e; 		// Found it!
    t =  compare < 0  ? t->left : t->right;	// Binary branch.
  }
  return rwnil;            // no match found
}

// Add an item, return it.
RWCollectable*
RWBinaryTree::insert(RWCollectable* a)
{
  RWPRECONDITION2(a!=rwnil,"RWBinaryTree::insert(RWCollectable*): nil pointer");
  if (isEmpty()) 
    // Insert in root if tree is empty:
    root = new RWTreeNode(a);	
  else {
  
    int compare;
    RWTreeNode* p;		// Place holder for parent.
    RWTreeNode* t = root;		// Start at root.
    do {			// Continue branching, looking for a 
      p = t;			//   nil node in the proper spot.
      compare = -t->e->compareTo(a);
      t = compare<0 ? t->left : t->right;
    } while (t);

    RWTreeNode* temp = new RWTreeNode(a);
    if (compare <  0)		// Add node to tree on the proper branch.
      p->left = temp;
    else 
      p->right = temp;
  }
  return a;
}

RWBoolean
RWBinaryTree::isEqual(const RWCollectable* a) const
{
  RWPRECONDITION2(a!=rwnil,"RWBinaryTree::isEqual(const RWCollectable*): nil pointer");
  if( a->isA() != RWBinaryTree::isA() ) return FALSE;
  return RWBinaryTree::operator==( *(const RWBinaryTree*)a );
}

void
RWBinaryTree::levelApply(RWapplyCollectable fun, void* x)
{
  RWGQueue(RWTreeNode) tq(root);
  RWTreeNode *node;
  while(rwnil != (node = tq.get()))
  {
    if(rwnil != node->left)  tq.insert(node->left);
    if(rwnil != node->right) tq.insert(node->right);
    (*fun)(node->e, x);
  }
}

size_t
RWBinaryTree::occurrencesOf(const RWCollectable* a) const
{
  RWPRECONDITION2(a!=rwnil,"RWBinaryTree::occurrencesOf(const RWCollectable*): nil pointer");
  size_t count = 0;
  RWTreeNode* t = root;
  while(t) {
    int compare = -t->e->compareTo(a);
    if ( compare < 0 )
      t = t->left;		// Branch left.
    else {
      // Branch right.  Check to see if we found one.
      if (compare == 0) count++;	// Found one.
      t = t->right;
    }
  }
  return count;
}

// Remove node and return pointer to item for which compare function = 0
RWCollectable*  
RWBinaryTree::remove(const RWCollectable* a)
{
  RWPRECONDITION2(a!=rwnil,"RWBinaryTree::remove(const RWCollectable*): nil pointer");

  RWTreeNode* p = rwnil;		// Parent of victim.
  RWTreeNode* x = root;		// Start of search.

  /* Search for the node, remembering its parent: */
  while (x) {
    int compare = -x->e->compareTo(a);
    if ( compare == 0 )		// Find it?
      return deleteNode(x, p);
    p = x;			// Remember parent.
    x = compare < 0 ? x->left : x->right; // Take proper branch.
  }
  return rwnil;			// Not found.
}

/* Delete node "x" with parent "p": */
RWCollectable*
RWBinaryTree::deleteNode(RWTreeNode* x, RWTreeNode* p)
{
  RWTreeNode* d = x;		// Remember node to be deleted.

  // Rearrange tree nodes:
  if (x->right == rwnil)		// Three possibilities...
    x = x->left;		// Note: if deleting the last node, x->left == nil.
  else if (x->right->left == rwnil) {
    x = x->right;
    x->left = d->left;
  } 
  else {
    RWTreeNode* c = x->right;
    while (c->left->left) c = c->left; // Descend down left branch.
    x = c->left;
    c->left = x->right;
    x->left = d->left;
    x->right = d->right;
  }

  // If it has a parent (it is not the root node), attach to it properly.
  if (p!=rwnil)	{
    if ( d->e->compareTo(p->e) <  0) 
      p->left = x;
    else
      p->right = x;
  }
  else		// Replace the root node. 
    root = x;	// If deleting the last remaining node, this leaves root == nil.

  RWCollectable* ret = d->e;
  delete d;                             // Delete node
  return ret;
}

/************************************************
 *						*
 *		Misc. definitions		*
 *						*
 ************************************************/

RWBinaryTree
RWCollection::asSortedCollection() const
{
  RWBinaryTree aSortedCollection;
  aSortedCollection += *this;
  return aSortedCollection;	// Painfully expensive, but unavoidable, copy.
}



#ifdef RDEBUG

#include "rw/rstream.h"
#include "rw/collint.h"

/**************** RWBinaryTree Debugging Utilities ******************/


/*
 * Included only if debugging.  Note, assumes that
 * all members are RWCollectableInt's.  I.e., this is NOT a general
 * testing utility.
 */

ostream&
operator<<(ostream& s, const RWBinaryTree& bt)
{
  unsigned level = 0;
  bt.printChildren(bt.root, s, level, '-');
  return s;
}

void
RWBinaryTree::printChildren(const RWTreeNode* t, ostream& s, unsigned& level, char parent) const
{
  if(t){
    level++;
    printChildren(t->right, s, level, '/');
    if(t->e->isA() == __RWCOLLECTABLEINT){	// This supplies some small measure of protection
      for(int i = 0; i<level; i++)  s << "  ";
      s << parent << " " << ((RWCollectableInt*)(t->e))->value() << endl;
    }
    printChildren(t->left, s, level, '\\');
    level--;
  }
}
#endif

