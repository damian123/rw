
/*
 * Definitions for doubly-linked list iterators.
 *
 * $Id: dlistit.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
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
 * $Log: dlistit.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/06/18  02:39:26  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.1  1994/04/15  19:06:31  vriezen
 * Move all files to 6.1
 *
 * Revision 1.3  1993/11/09  08:06:08  jims
 * Port to ObjectStore
 *
 * Revision 1.2  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.1  1993/05/14  19:32:30  dealys
 * Initial revision
 *
 * 
 */

#include "rw/dlist.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: dlistit.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif


/********************************************************
*							*
*		RWDlistIterator definitions		*
*							*
********************************************************/

RWDlistIterator::~RWDlistIterator() {}

void*
RWDlistIterator::operator+=(size_t n)
{
  void* p = key();
  while (n--)
    p = ++(*this);
  return p;
}

void*
RWDlistIterator::operator-=(size_t n)
{
  void* p = key();
  while (n--)
    p = --(*this);
  return p;
}

void*
RWDlistIterator::findNext(RWtestGeneric tst, const void* x)
{
  RWPRECONDITION( tst!=0 );
  void* p;
  while ( (p = (*this)()) != 0 )  // advances iterator
    if ( (*tst)(p, x) ) return p;
  return rwnil;
}

void*
RWDlistIterator::findNextReference(const void* a)
{
  void* p;
  while ( (p = (*this)()) != 0 ){	// This will advance iterator.
    if ( p==a )	 return p;		// Is this the victim?
  }
  return rwnil;
}

void*
RWDlistIterator::insertAfterPoint(void* a)
{
  RWIsvDlistIterator::insertAfterPoint(new RWPDlink(a));
  return a;
}

/*
 * Remove first occurrence where the tester returns true.
 */
void*
RWDlistIterator::removeNext(RWtestGeneric tst, const void* x)
{
  RWPRECONDITION( tst!=rwnil );

  void* p;
  while ( (p = ++(*this)) != rwnil )
  {
    if ( tst(p, x) )		// hit?
      return remove();
  }
  return rwnil;
}

/*
 * Remove first occurrence of the pointer "a".
 */
void*
RWDlistIterator::removeNextReference(const void* a)
{
  RWPRECONDITION( a!=rwnil );

  void* p;
  while ( (p = ++(*this)) != rwnil )
  {
    if (p==a)
      return remove();
  }
  return rwnil;
}

