/*
 * Definitions for binary tree iterator class.
 *
 * $Id: bintrit.cpp,v 6.5 1994/07/19 01:16:13 jims Exp $
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
 *
 * $Log: bintrit.cpp,v $
 * Revision 6.5  1994/07/19  01:16:13  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.3  1994/06/18  02:39:26  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.2  1994/05/05  19:57:04  vriezen
 * Remove duplicate definitions of RWBinaryTree::printChildren and ::operator<<(ostream&, const RWBinaryTree&);
 *
 * Revision 6.1  1994/04/15  19:05:15  vriezen
 * Move all files to 6.1
 *
 * Revision 1.3  1993/11/09  04:25:14  jims
 * Moved definition of class RWTreeNode back to header file
 *
 * Revision 1.2  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.1  1993/05/14  18:37:02  dealys
 * Initial revision
 *
 *
 */

#include "rw/bintree.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: bintrit.cpp,v $ $Revision: 6.5 $ $Date: 1994/07/19 01:16:13 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif
   
/****************************************************************
 *								*
 *		RWBinaryTreeIterator class definitions		*
 *								*
 ****************************************************************/

/*
 * Implementing an iterator for a binary tree is not trivial.
 * The main complication is that you must maintain a stack of
 * visited parent nodes to remember where you are in the tree
 */

RWBinaryTreeIterator::RWBinaryTreeIterator(const RWBinaryTree& b)
{
  tree = &b;
  here = rwnil;
}

RWBinaryTreeIterator::~RWBinaryTreeIterator() {}

RWCollectable*
RWBinaryTreeIterator::findNext(const RWCollectable* a)
{
  RWPRECONDITION2(a!=rwnil,"RWBinaryTreeIterator::findNext(const RWCollectable*): nil pointer");

  int compare;
  
  if ( tree->isEmpty() )return rwnil;
  
  /* Test for first time through: */
  if ( here == rwnil ) {
restart: ;
    stack.clear();		// Make sure stack is cleared.
    here = tree->root;		// Initialize "here" to root.
    compare = -here->e->compareTo(a);
    if ( compare == 0 )return here->e; // Is root the target?
  } else {			    
    /* Not first time through.  Do a test. */
    compare = -here->e->compareTo(a);
    /* Three possiblities...
     *   the given value is less than the current value; it won't
     *   be found.  Just return. */
    if ( compare < 0 ) {
      reset();
      return rwnil;
    }

    /*   the give value is greater than the current value, start all over.*/
    if ( compare > 0 ) goto restart;

    /*   else, the given value is the same as the current value;
     *   If you think about it, it must be in the right-hand branch.
     *   Just carry on.  If it exists, we'll find it. */

  }
  
  
  /*
   * The objective is to advance to the next node that compares to zero.
   */
  
  do {
    stack.push((RWTreeNode*)here);
    // Which way do we branch?
    here = compare < 0 ? here->left : here->right;
    // Is there a "next object" at all?
    if ( here==rwnil ) {
      reset();
      return rwnil;
    }
    compare = -here->e->compareTo(a);
  } while (compare);
  
  return here->e;
}

RWCollectable*
RWBinaryTreeIterator::key() const
{
  return here ? here->e : rwnil;
}

/*
 * Advance and return "next" object.
 */
RWCollectable*
RWBinaryTreeIterator::operator()()
{
  if ( tree->isEmpty() )return rwnil;

  /* Test for first time through: */
  if ( here==rwnil ) {
    stack.clear();		// Clear stack.
    here = tree->root;		// Set here to root node.
    /* Descend the left branch all the way to the bottom;
       this will be the "first" member of the tree. */
    descendLeft();
  }

/*
 * The pointer "here" points to the "current node".  The overall
 * objective is to find the "next largest node".  Because the binary tree
 * is sorted, this next node can be in only one of two places.  (1) If
 * the current node has a right-hand branch, then it must be in the
 * "smallest valued" node in that right-hand branch.  Descend the branch all the
 * way to the bottom, taking all left-hand branches.  (2) If the current
 * node does not have a right hand branch, then the next node must be a
 * parent.  But, it can only be a parent such that we are in a left-hand
 * branch of that parent (otherwise the current node would have a larger
 * value).  Hence, unwind the stack, looking for a parent such that we are not
 * in its right-hand branch.  This parent will be the "next value". 
 */

  // Is there a right-hand branch for the current node?
  else if ( here->right ) {
    // Yes.  Push its root onto the stack and descend it.
    stack.push((RWTreeNode*)here);
    here = here->right;
    descendLeft();
  }
  else {
    /* No right-hand branch for the current node.
     * Unwind the stack, looking for a parent where we are the
     * left-hand daughter. */
    RWTreeNode* daughter;
    do {
      daughter = (RWTreeNode*) here;	// Save what will be the daughter.
      here = stack.pop();		// Get the parent.
      if ( here == rwnil ) return rwnil;	// stack empty?
    } while ( here->right == daughter );// Continue while we are right-hand daughter
  }
  return here->e;
}

void
RWBinaryTreeIterator::reset()
{
  stack.clear();
  here = rwnil;
}

/**************** RWBinaryTreeIterator Utilities ******************/

/*
 * Descend the left branch as far as we can go.
 * Leaves "here" pointing to the leaf and all parents
 * on the stack.
 */

void
RWBinaryTreeIterator::descendLeft()
{
  while ( here->left ) {
    stack.push((RWTreeNode*)here);
    here = here->left;
  }
}


