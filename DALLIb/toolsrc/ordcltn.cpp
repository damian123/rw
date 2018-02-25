/*
 * Definitions for RWOrdered (OrderedCollection)
 *
 * $Id: ordcltn.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 * Voice: (503) 754-3010	FAX: (503) 757-6650
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
 * $Log: ordcltn.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/06/18  01:46:41  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.1  1994/04/15  19:07:20  vriezen
 * Move all files to 6.1
 *
 * Revision 2.13  1994/03/04  01:04:16  jims
 * Override isEqual member function from RWCollectable to return
 * TRUE or FALSE based on operator==
 *
 * Revision 2.12  1994/03/02  19:20:24  jims
 * New message INDEXERR shows invalid index and number of items
 * in collection
 *
 * Revision 2.11  1994/01/13  04:37:21  jims
 * Add boundsCheck(ipt) to RWOrdered::removeAt(size_t ipt)
 *
 * Revision 2.10  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.9  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.8  1993/05/18  15:57:03  dealys
 * merged with orddf.cpp
 *
 * Revision 2.7  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.6  1993/05/14  21:01:16  dealys
 * removed iterator code
 *
 * Revision 2.5  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.4  1993/03/24  23:49:27  keffer
 * Indexing operations now used unsigned
 *
 * Revision 2.3  1993/01/25  18:17:44  keffer
 * RW_NO_CONST_OVERLOADS->RW_NO_CONST_OVERLOAD
 *
 * Revision 2.2  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.1  1992/11/15  00:02:30  keffer
 * Added explicit "this->" dereference to suppress unfounded
 * cfront 3.0 warnings.
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.4   04 Mar 1992 09:17:24   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.3   05 Nov 1991 14:06:22   keffer
 * Removed RWXXXXErr macros --- now done by error package
 * 
 *    Rev 1.2   28 Oct 1991 09:24:22   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.1   29 Jul 1991 14:20:42   keffer
 * Return type of asOrderedCollection() is now RWOrdered.
 * 
 *    Rev 1.0   28 Jul 1991 08:39:32   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/ordcltn.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"
#include "defcol.h"          

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: ordcltn.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

RWDEFINE_COLLECTABLE2(RWOrdered, __RWORDERED)

RWOrdered::RWOrdered(size_t size) :
  vec(size, rwnil)
{
  nitems = 0;
}                                                       

RWOrdered::~RWOrdered() {}

RWBoolean
RWOrdered::operator==(const RWOrdered& rwo) const
{
  if( nitems!=rwo.nitems )return FALSE;

  for( register size_t i=0; i<nitems; i++ )
    if( !this->vec(i)->isEqual(rwo.vec(i))) return FALSE;

  return TRUE;
}

RWCollectable*
RWOrdered::append(RWCollectable* c)
{
  return RWOrdered::insertAt(nitems, c);
}

void
RWOrdered::apply(RWapplyCollectable ap, void* x)
{
  RWPRECONDITION( ap!=0 );
  for( register size_t i = 0; i<nitems; i++ )
    (*ap)(vec(i), x);
}

RWCollectable*&
RWOrdered::at(size_t inx)	
{
  return (*this)[inx];		// This will do bounds checking
}

#ifndef RW_NO_CONST_OVERLOAD
const RWCollectable*
RWOrdered::at(size_t inx) const
{
  const RWCollectable* temp = (*(RWOrdered*)this)[inx];
  return temp;
}
#endif

void
RWOrdered::clear()
{
  nitems = 0;
  if(vec.length() > DEFAULT_CAPACITY)	// If vector has grown, shrink it back down
	vec.resize(DEFAULT_CAPACITY);
}

RWCollectable*
RWOrdered::find(const RWCollectable* c) const
{
  size_t indx = index(c);
  return indx==RW_NPOS ? rwnil : this->vec(indx);
}

/*
 * Returns first element, or nil if there is no first element.
 */
RWCollectable*
RWOrdered::first() const
{
  return nitems>0 ? this->vec(0) : rwnil;
}

size_t
RWOrdered::index(const RWCollectable* c) const
{
  for(register size_t i=0; i<nitems; i++)
    if(this->vec(i)->isEqual(c)) return i;
  return RW_NPOS;
}

RWCollectable*
RWOrdered::insert(RWCollectable* c)
{
  return RWOrdered::insertAt(nitems, c);
}

// For backwards compatiblity:
RWCollectable*
RWOrdered::insertAfter(int ipt, RWCollectable* c)
{
  return insertAt((size_t)(ipt+1), c);
}

RWCollectable*
RWOrdered::insertAt(size_t ipt, RWCollectable* c)
{
  if(ipt>nitems )
    RWTHROW( RWBoundsErr(RWMessage(RWTOOL_INDEX, (unsigned)ipt, (unsigned)nitems) ));

  // Check for overflow:
  if(nitems>=vec.length())
    vec.resize(vec.length()+DEFAULT_RESIZE);

  // Slide right:
  for(register size_t i=nitems; i>ipt; i--)
    vec(i) = vec(i-1);

  nitems++;
  return vec(ipt) = c;
}

RWBoolean
RWOrdered::isEqual(const RWCollectable* c) const
{
  if( c->isA() != __RWORDERED ) return FALSE;
  return operator==(*(const RWOrdered*)c);
}

RWCollectable*
RWOrdered::last() const
{
  return nitems>0 ? this->vec(nitems-1) : rwnil;
}

size_t
RWOrdered::occurrencesOf(const RWCollectable* c) const
{
  register size_t total = 0;
  size_t idx = index(c);
  if(idx!=RW_NPOS)
  {
    for(register size_t i = idx; i<nitems; i++)
      if(this->vec(i)->isEqual(c)) total++;
  }
  return total;
}

RWCollectable*
RWOrdered::remove(const RWCollectable* c)
{
  size_t i = index(c);
  return i==RW_NPOS ? rwnil : removeAt(i);
}

RWCollectable*
RWOrdered::prepend(RWCollectable* c)
{
  return RWOrdered::insertAt(0,c);
}

void
RWOrdered::resize(size_t N)
{
  if( N>=nitems ) vec.resize(N);
}

/******************** Miscellaneous functions *****************************/

RWOrdered
RWCollection::asOrderedCollection() const
{
  RWOrdered temp(entries());
  temp += *this;
  return temp;
}

void
RWOrdered::boundsCheck(size_t ipt) const
{
  if(ipt>=nitems )
    RWTHROW( RWBoundsErr(RWMessage(RWTOOL_INDEXERR, (unsigned)ipt, (unsigned)nitems) ));
}

RWCollectable*
RWOrdered::removeAt(size_t ipt)
{
  boundsCheck(ipt);
  RWCollectable* victim = vec(ipt);
  // Slide left:
  for(register size_t i=ipt; i<nitems-1; i++)
    vec(i) = vec(i+1);
  nitems--;
  return victim;
}

