/*
 * RWIsvDlist: Definitions for intrusive doubly-linked lists.
 *
 * $Id: idlist.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: idlist.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:06:48  vriezen
 * Move all files to 6.1
 *
 * Revision 2.12  1994/03/02  19:20:24  jims
 * New message INDEXERR shows invalid index and number of items
 * in collection
 *
 * Revision 2.11  1994/01/04  23:04:17  jims
 * Added note to precondition message warning about non-debug code linked
 * to debug version of library
 *
 * Revision 2.10  1993/12/06  20:19:24  jims
 * Modify ASSERT, PRECONDITION macros to explicitly compare pointers to rwnil
 *
 * Revision 2.9  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.8  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.7  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.6  1993/05/14  20:30:14  dealys
 * removed iterator code
 *
 * Revision 2.5  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.4  1993/03/24  23:49:27  keffer
 * Indexing operations now used unsigned
 *
 * Revision 2.3  1993/01/29  20:09:26  keffer
 * Added precondition in removeReference()
 *
 * Revision 2.2  1993/01/23  00:34:51  keffer
 * Performance enhancements; simplified; flatter inheritance tree.
 *
 *
 *    Rev 1.2   28 May 1992 16:24:14   KEFFER
 * Introduced RWPRECONDITION2 macro
 * 
 *    Rev 1.1   25 May 1992 15:38:12   KEFFER
 * 
 *    Rev 1.0   15 Apr 1992 19:48:38   KEFFER
 * Initial revision.
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
#include "rw/rwerr.h"
#include "rw/toolerr.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: idlist.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

/********************************************************
*							*
*		RWIsvDlist definitions			*
*							*
********************************************************/


/*
 * Construct a list with one link, given by "a".  In the "debug" version,
 * the "next" pointer in "a" must be nil, signalling that it is not being
 * used in any other list.
 */
RWIsvDlist::RWIsvDlist(RWIsvDlink* a)
{
  RWPRECONDITION2(a!=rwnil, "RWIsvDlist::RWIsvDlist(RWIsvDlink*): Attempt to construct with nil pointer");
  RWPRECONDITION2(a->next_==rwnil, "RWIsvDlist::RWIsvDlist(RWIsvDlink*): Attempt to add intrusive link to more than one list");

  head_.next_ = tail_.prev_ = a;
  a->next_    = tail_.next_ = &tail_;
  a->prev_    = head_.prev_ = &head_;
  nitems_     = 1;
}

/*
 * Find link "i".  The index "i" must be in range.
 */
RWIsvDlink*
RWIsvDlist::at(size_t i) const
{
  if(i>=entries())
    RWTHROW( RWBoundsErr(RWMessage(RWTOOL_INDEXERR,
				   (unsigned)i,
				   (unsigned)entries()) ));

  register RWIsvDlink* link = head_.next_;
  while (i--) link = link->next_;
  return link;
}

/*
 * Clear the list.  The debug implementation walks the list, setting each
 * "next" pointer to nil.  This allows other routines to detect whether
 * a link is being inserted into more than one collection.
 */
void   
RWIsvDlist::clear()
{
#ifdef RWDEBUG
  register RWIsvDlink* link = first();
  while (link)
  {
    link = link->clear();
  }
#endif
  init();
}

RWBoolean
RWIsvDlist::containsReference(const RWIsvDlink* a) const
{
  RWPRECONDITION2(a!=rwnil, "RWIsvDlist::containsReference(const RWIsvDlink*): nil pointer");
  RWPRECONDITION(head_.next_ != rwnil);

  if (a)
  {
    const RWIsvDlink* cursor = head_.next_;
    // Cast away constness to suppress unwarranted cfront warning:
    while (cursor != &((RWIsvDlist*)this)->tail_)
    {
      if (cursor == a) return TRUE;
      cursor = cursor->next_;
    }
  }
  return FALSE;
}


/*
 * Put link "a" at position "i".  The link formerly at
 * position "i" becomes link "i+1".
 */
void
RWIsvDlist::insertAt(size_t i, RWIsvDlink* a)
{
  RWPRECONDITION2(a!=rwnil, "RWIsvDlist::insertAt(size_t,RWIsvDlink*): nil pointer");
  RWPRECONDITION2(a->next_==rwnil, "RWIsvDlist::insertAt(size_t,RWIsvDlink*): Attempt to add intrusive link to more than one list");

  if(i>entries())
    RWTHROW( RWBoundsErr(RWMessage(RWTOOL_INDEXERR, (unsigned)i, (unsigned)entries()) ));

  RWIsvDlink* prev = i ? at(i-1) : &head_;

  RWASSERT(prev!=rwnil);

  insertAfterLink(prev, a);
}


/*
 * Remove and return the link at position "i".
 */
RWIsvDlink*
RWIsvDlist::removeAt(size_t i)
{
  if(i>=entries())
    RWTHROW( RWBoundsErr(RWMessage(RWTOOL_INDEXERR, (unsigned)i, (unsigned)entries()) ));

  return removeReference(at(i));
}


/*
 * Remove and return the link with address "a".  Returns nil if the
 * link is nil.
 */

RWIsvDlink*
RWIsvDlist::removeReference(RWIsvDlink* a)
{
  if (!a)
    return rwnil;

  RWPRECONDITION2(a != &head_ && a != &tail_, "RWIsvDlist::removeReference(RWIsvDlink*): Internal error");

  a->prev_->next_ = a->next_;
  a->next_->prev_ = a->prev_;

#ifdef RWDEBUG
  a->next_ = rwnil;
#endif

  nitems_--;

  return a;
}

/********************************************************
*							*
*		RWIsvDlist protected functions		*
*							*
********************************************************/

void
RWIsvDlist::init()
{
  head_.next_ = tail_.next_ = &tail_;
  head_.prev_ = tail_.prev_ = &head_;
  nitems_     = 0;
}


/*
 * Protected function to insert an object. 
 * Link "a" is inserted to the right of link "sl".
 * Neither "a" nor "sl" can be nil.
 */
void
RWIsvDlist::insertAfterLink(RWIsvDlink* sl, RWIsvDlink* a)
{
  RWPRECONDITION2(sl!=rwnil, "RWIsvDlist::insertAfterLink(RWIsvDlink*, RWIsvDlink*): Internal error: attempt to add link after nil link");
  RWPRECONDITION2(a!=rwnil, "RWIsvDlist::insertAfterLink(RWIsvDlink*, RWIsvDlink*): Attempt to add nil link");
  RWPRECONDITION2(a->next_==rwnil, "RWIsvDlist::insertAfterLink(RWIsvDlink*, RWIsvDlink*): Attempt to add intrusive link to more than one list\n(NOTE: May be caused by not defining RWDEBUG when compiling modules to be linked with debug version of library.)");

  a->next_  = sl->next_;
  a->prev_  = sl;
  a->next_->prev_ = sl->next_ = a;
  ++nitems_;
}
