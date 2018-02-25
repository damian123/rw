
/*
 * RWSlist: Definitions for singly-linked lists of pointers.
 *
 * $Id: slist.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
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
 * $Log: slist.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/06/18  01:04:36  myersn
 * move dtor out-of-line.
 *
 * Revision 6.1  1994/04/15  19:07:50  vriezen
 * Move all files to 6.1
 *
 * Revision 2.11  1993/11/09  09:37:36  jims
 * Port to ObjectStore
 *
 * Revision 2.10  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.9  1993/05/14  21:20:30  dealys
 * removed iterator code
 *
 * Revision 2.8  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.7  1993/03/24  23:49:27  keffer
 * Indexing operations now used unsigned
 *
 * Revision 2.6  1993/03/14  16:57:30  keffer
 * Added additional RWPRECONDITION clauses.
 *
 * Revision 2.5  1993/02/11  22:48:30  keffer
 * Ported to IBM xlC compiler
 *
 * Revision 2.4  1993/01/25  18:23:08  keffer
 * Simplified, improving performance
 *
 * 
 *    Rev 1.9   29 May 1992 09:46:56   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.8   27 May 1992 18:09:10   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.7   25 May 1992 15:38:32   KEFFER
 * added function applyReference().
 * 
 *    Rev 1.6   04 Mar 1992 09:17:28   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.5   18 Feb 1992 19:29:40   KEFFER
 * Iterator is now left at *previous* link after remove().
 * 
 *    Rev 1.4   05 Nov 1991 14:06:46   keffer
 * Removed RWXXXXErr macros --- now done by error package
 * 
 *    Rev 1.3   28 Oct 1991 09:24:28   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.2   22 Aug 1991 11:01:52   keffer
 * Suppress warning in findLeft()
 * 
 *    Rev 1.0   28 Jul 1991 08:39:44   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 * 
 */

#include "rw/slist.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: slist.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

/********************************************************
*							*
*		RWSlist definitions			*
*							*
********************************************************/

RWSlist::RWSlist(const RWSlist& s)
{
  // Construct an iterator, casting away constness to avoid warnings
  // (which we promise to honor anyway):
  RWSlistIterator iter((RWSlist&) s);
  void* p;
  while (rwnil != (p = iter()))
    append(p);
  RWPOSTCONDITION(s.entries() == entries());
}

RWSlist::~RWSlist() { clear(); }

RWSlist&
RWSlist::operator=(const RWSlist& s)
{
  if (this!=&s)
  {
    clear();			// Get rid of old links.
    // Construct an iterator, casting away "constness"
    // (which we promise to honor anyway):
    RWSlistIterator iter((RWSlist&)s);
    void *p;
    while (rwnil != (p = iter()))
      append(p);
  }
  RWPOSTCONDITION(s.entries()==entries());
  return *this;
}

void*
RWSlist::append(void* a)
{
  insertAfterLink(lastLink(), new RWPSlink(a));
  return a;
}

void
RWSlist::apply(RWapplyGeneric ap, void *x)
{
  RWPRECONDITION(ap!=rwnil);
  RWPSlink* link = firstLink();
  while (link != tailLink())
  {
    (*ap)(link->info_, x);
    link = link->next();
  }
}

void
RWSlist::applyReference(void (*apRef)(void*&, void*), void* x)
{
  RWPRECONDITION(apRef!=rwnil);
  RWPSlink* link = firstLink();
  while (link != tailLink())
  {
    (*apRef)(link->info_, x);
    link = link->next();
  }
}

void
RWSlist::clear()
{
  RWPSlink *n;
  RWPSlink *link = firstLink();
  while (link != tailLink() )
  {
    n = link->next();
    delete link;
    link = n;
  }
  RWIsvSlist::init();
}

void*
RWSlist::find(RWtestGeneric tst, const void* x) const
{
  RWPRECONDITION(tst!=rwnil);
  RWPSlink* link = firstLink();
  while (link != tailLink())
  {
    if ((*tst)(link->info_, x))
      return link->info_;
    link = link->next();
  }
  return rwnil;
}

void*
RWSlist::findReference(const void* x) const
{
  RWPSlink* link = firstLink();
  while (link != tailLink())
  {
    if (link->info_ == x)
      return link->info_;
    link = link->next();
  }
  return rwnil;
}

size_t
RWSlist::index(RWtestGeneric tst, const void* x) const
{
  RWPRECONDITION(tst!=rwnil);
  size_t index = 0;
  RWPSlink* link = firstLink();
  while (link != tailLink())
  {
    if ((*tst)(link->info_, x))
      return index;
    link = link->next();
    ++index;
  }
  return RW_NPOS;
}

// For backwards compatibility:
void*
RWSlist::insertAfter(int i, void* a)
{
  RWPRECONDITION(i>=0 && i<entries());
  RWIsvSlist::insertAfterLink(RWIsvSlist::at(i), new RWPSlink(a));
  return a;
}

void*
RWSlist::insertAt(size_t i, void* a)
{
  RWPRECONDITION(i<=entries());
  RWIsvSlist::insertAt(i, new RWPSlink(a));
  return a;
}

size_t 
RWSlist::occurrencesOf(RWtestGeneric tst, const void* x) const
{
  RWPRECONDITION(tst!=rwnil);
  size_t count = 0;
  RWPSlink* link = firstLink();
  while (link != tailLink())
  {
    if ((*tst)(link->info_, x))
      ++count;
    link = link->next();
  }
  return count;
}

size_t
RWSlist::occurrencesOfReference(const void* x) const
{
  size_t count = 0;
  RWPSlink* link = firstLink();
  while (link != tailLink())
  {
    if (x == link->info_)
      ++count;
    link = link->next();
  }
  return count;
}

void*
RWSlist::prepend(void* a)
{
  insertAfterLink(headLink(), new RWPSlink(a));
  return a;
}

// Remove and return first link for which the tester == 1.
// Return rwnil if unsuccessful.
void*
RWSlist::remove(RWtestGeneric tst, const void* x)
{
  RWPRECONDITION(tst!=rwnil);
  // Look for previous link:
  RWPSlink* prev = headLink();
  while (prev != lastLink())
  {
    if ((*tst)(prev->next()->info_,x))
    {
      return peel(removeRight(prev));
    }
    // else
    prev = prev->next();
  }
  return rwnil;
}

void*
RWSlist::removeReference(const void* a)
{
  // Look for previous link:
  RWPSlink* prev = headLink();
  while (prev != lastLink())
  {
    if (prev->next()->info_ == a)
    {
      return peel(removeRight(prev));
    }
    // else
    prev = prev->next();
  }
  return rwnil;
}

// RWSlist protected functions

void*
RWSlist::peel(RWPSlink* link)
{
  if (!link)
    return rwnil;
  void *ret = link->info_;
  delete link;
  return ret;
}


