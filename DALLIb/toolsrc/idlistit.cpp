/*
 * RWIsvDlistIterator 
 *
 * $Id: idlistit.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: idlistit.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:06:49  vriezen
 * Move all files to 6.1
 *
 * Revision 1.4  1993/12/06  20:19:24  jims
 * Modify ASSERT, PRECONDITION macros to explicitly compare pointers to rwnil
 *
 * Revision 1.3  1993/11/13  23:05:50  keffer
 * Fixed bug in RWIsvDlistIterator::operator-=(size_t n);
 *
 * Revision 1.2  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.1  1993/05/14  20:30:14  dealys
 * Initial revision
 *
 * 
 */

/*
 * Member data head_.next_ points to the actual start of
 * the list, or tail_ if there are no members of the list.
 * Member data head_.prev_ always points to itself.
 *
 * Member data tail_.prev_ points to the last member of
 * the list, or to head_ if there are no members of the list.
 * Member data tail_.next_ always points to itself.
 */

#include "rw/idlist.h"
#include "rw/toolerr.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: idlistit.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif



/********************************************************
*							*
*		RWIsvDlistIterator definitions		*
*							*
********************************************************/

/*
 * The variable "dhere_" points to the "current position" of
 * the iterator (sometimes called "point" or the "cursor 
 * position").  When the iterator is constructed, dhere_
 * points to the "head_" link of the list.  This link
 * points to the actual first link of the list.
 * Each call to operator++() advances the cursor one position.
 *
 */


/*
 * Advance iterator n links then return.
 */

RWIsvDlink*
RWIsvDlistIterator::operator+=(size_t n)
{
  while (n--)
    ++(*this);
  return dhere_ == &dlist_->tail_ ? rwnil : dhere_;
}

/*
 * Backup iterator n links then return.
 */

RWIsvDlink*
RWIsvDlistIterator::operator-=(size_t n)
{
  while (n--)
    --(*this);
  return dhere_ == &dlist_->head_ ? rwnil : dhere_;
}

RWBoolean
RWIsvDlistIterator::atFirst() const
{
  return isActive() && dhere_ == dlist_->head_.next_;
}

RWBoolean
RWIsvDlistIterator::atLast() const
{
  return isActive() && dhere_ == dlist_->tail_.prev_;
}

void
RWIsvDlistIterator::toFirst()
{
  dhere_ = dlist_->head_.next_;
}

void
RWIsvDlistIterator::toLast()
{
  dhere_ = dlist_->isEmpty() ? &dlist_->tail_ : dlist_->tail_.prev_;
}


/*
 * Insert the link pointed to by a after the current link.  There must
 * be a current link (that is, the iterator must still be active).
 */
void
RWIsvDlistIterator::insertAfterPoint(RWIsvDlink* a)
{
  RWPRECONDITION2(a!=rwnil, "RWIsvDlistIterator::insertAfterPoint(RWIsvDlink*): nil pointer");
  RWPRECONDITION2(a->next_==rwnil, "RWIsvDlistIterator::insertAfterPoint(RWIsvDlink*): Attempt to add intrusive link to more than one list");
  RWPRECONDITION2(dhere_ != &dlist_->tail_, "RWIsvDlistIterator::insertAfterPoint(RWIsvDlink*): Attempt to add link while iterator is inactive");

  dlist_->insertAfterLink(dhere_, a);
}	       

/*
 * Remove the link pointed to by the iterator.  The iterator must
 * be active.  The iterator is left pointing to the *previous* item in
 * the list.
 */

RWIsvDlink*
RWIsvDlistIterator::remove()
{
  // Check for inactive iterator:
  if (!isActive()) return rwnil;

  // Backup the iterator to the previous link:
  dhere_ = dhere_->prev_;

  // Now remove the link at the old position:
  return dlist_->removeReference(dhere_->next_);
}

// Reset the iterator to work with a new collection
void
RWIsvDlistIterator::reset(RWIsvDlist& s)
{
  dlist_ = &s;
  reset();
}
