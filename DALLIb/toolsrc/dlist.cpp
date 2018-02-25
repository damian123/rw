
/*
 * Definitions for doubly-linked lists.
 *
 * $Id: dlist.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: dlist.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:06:29  vriezen
 * Move all files to 6.1
 *
 * Revision 2.11  1993/11/09  08:06:08  jims
 * Port to ObjectStore
 *
 * Revision 2.10  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.9  1993/05/14  19:32:30  dealys
 * removed iterator code
 *
 * Revision 2.8  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.7  1993/03/24  23:49:27  keffer
 * Indexing operations now used unsigned
 *
 * Revision 2.6  1993/02/11  22:48:30  keffer
 * Ported to IBM xlC compiler
 *
 * Revision 2.5  1993/01/29  20:10:24  keffer
 * Optimized removeNextReference().
 *
 * Revision 2.4  1993/01/25  18:23:08  keffer
 * Simplified, improving performance
 *
 * Revision 2.3  1993/01/16  02:40:40  keffer
 * Uses new <rw/compiler.h> directives.
 *
 *    Rev 1.3   27 May 1992 18:09:02   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.2   04 Mar 1992 09:17:22   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.1   28 Oct 1991 09:24:06   keffer
 * Include file path now <rw/xxx.h>
 * 
 */

#include "rw/dlist.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: dlist.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif


/********************************************************
*							*
*		RWDlist definitions			*
*							*
********************************************************/

RWDlist::RWDlist(const RWDlist& s) 
{
  RWDlistIterator iter((RWDlist&)s); // cast away, but honor const on s
  void* p;
  while (0 != (p = iter()))
    append(p);
  RWPOSTCONDITION(s.entries() == entries());
}

RWDlist&
RWDlist::operator=(const RWDlist& s)
{
  if (this!=&s)
  {
    clear();			// Get rid of old links.
    // Construct an iterator, casting away "constness"
    // (which we promise to honor anyway):
    RWDlistIterator iter((RWDlist&)s);
    void* p;
    while (rwnil != (p = iter()))
      append(p);
  }
  RWPOSTCONDITION(s.entries() == entries());
  return *this;
}

void*
RWDlist::append(void* a)
{
  insertAfterLink(lastLink(), new RWPDlink(a));
  return a;
}

void
RWDlist::apply(RWapplyGeneric tst, void* x)
{
  RWPDlink* link = firstLink();
  while (link != tailLink())
  {
    (*tst)(link->info_, x);
    link = link->next();
  }
}

void
RWDlist::applyReference(void (*apRef)(void*&, void*), void* x)
{
  RWPDlink* link = firstLink();
  while (link != tailLink())
  {
    (*apRef)(link->info_, x);
    link = link->next();
  }
}

void
RWDlist::clear()
{
  RWPDlink *n;
  RWPDlink *link = firstLink();
  while (link != tailLink() )
  {
    n = link->next();
    delete link;
    link = n;
  }
  RWIsvDlist::init();
}

void*
RWDlist::find(RWtestGeneric tst, const void* x) const
{
  RWPDlink* link = firstLink();
  while (link != tailLink())
  {
    if ((*tst)(link->info_, x))
      return link->info_;
    link = link->next();
  }
  return rwnil;
}

void*
RWDlist::findReference(const void* x) const
{
  RWPDlink* link = firstLink();
  while (link != tailLink())
  {
    if (link->info_ == x)
      return link->info_;
    link = link->next();
  }
  return rwnil;
}

size_t
RWDlist::index(RWtestGeneric tst, const void* x) const
{
  RWPRECONDITION(tst!=rwnil);
  size_t index = 0;
  RWPDlink* link = firstLink();
  while (link != tailLink())
  {
    if ((*tst)(link->info_, x))
      return index;
    link = link->next();
    ++index;
  }
  return RW_NPOS;
}

void*
RWDlist::insertAfter(int i, void* a)
{
  RWPRECONDITION(i>=0 && i<entries());
  RWIsvDlist::insertAfterLink(RWIsvDlist::at(i), new RWPDlink(a));
  return a;
}

void*
RWDlist::insertAt(size_t i, void* a)
{
  RWPRECONDITION(i<=entries());
  RWIsvDlist::insertAt(i, new RWPDlink(a));
  return a;
}

size_t
RWDlist::occurrencesOf(RWtestGeneric tst, const void* x) const
{
  RWPRECONDITION(tst!=rwnil);
  size_t count = 0;
  RWPDlink* link = firstLink();
  while (link != tailLink())
  {
    if ((*tst)(link->info_, x))
      ++count;
    link = link->next();
  }
  return count;
}

size_t
RWDlist::occurrencesOfReference(const void* x) const
{
  size_t count = 0;
  RWPDlink* link = firstLink();
  while (link != tailLink())
  {
    if (x == link->info_)
      ++count;
    link = link->next();
  }
  return count;
}

void*
RWDlist::prepend(void* a)
{
  insertAfterLink(headLink(), new RWPDlink(a));
  return a;
}

// Remove and return first link for which the tester == 1.
// Return rwnil if unsuccessful.
void*
RWDlist::remove(RWtestGeneric tst, const void* x)
{
  RWPRECONDITION(tst!=rwnil);
  RWPDlink* link = firstLink();
  while (link != tailLink())
  {
    if ((*tst)(link->info_,x))
    {
      return peel((RWPDlink*)RWIsvDlist::removeReference(link));
    }
    link = link->next();
  }
  return rwnil;
}

/*
 * Remove first occurrence of the link containing the pointer "a".
 */
void*
RWDlist::removeReference(const void* a)
{
  RWPDlink* link = firstLink();
  while (link != tailLink())
  {
    if (link->info_ == a)
    {
      return peel((RWPDlink*)RWIsvDlist::removeReference(link));
    }
    link = link->next();
  }
  return rwnil;
}

// RWDlist protected methods

void*
RWDlist::peel(RWPDlink* link)
{
  if (!link)
    return rwnil;
  void *ret = link->info_;
  delete link;
  return ret;
}

