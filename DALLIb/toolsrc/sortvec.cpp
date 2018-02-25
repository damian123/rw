/*
 * Definitions for RWSortedVector.
 *
 * $Id: sortvec.cpp,v 6.5 1994/07/18 20:51:00 jims Exp $
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
 * $Log: sortvec.cpp,v $
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.3  1994/06/18  01:24:09  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.2  1994/06/02  23:35:26  foote
 * removed extern "C" from declaration of comparison function
 *
 * Revision 6.1  1994/04/15  19:07:53  vriezen
 * Move all files to 6.1
 *
 * Revision 2.11  1994/03/04  01:04:16  jims
 * Override isEqual member function from RWCollectable to return
 * TRUE or FALSE based on operator==
 *
 * Revision 2.10  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.9  1993/08/05  10:25:36  jims
 * Remove "Always true" condition from POSTCONDITION to avoid
 * compiler warning
 *
 * Revision 2.8  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.7  1993/05/18  16:39:32  dealys
 * merged with sortvecd.cpp
 *
 * Revision 2.6  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
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
 * Revision 2.1  1992/11/15  00:31:14  keffer
 * Added explicit "this->" dereference to suppress unfounded
 * cfront 3.0 warnings.
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.4   04 Mar 1992 09:17:30   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.3   13 Nov 1991 11:50:12   keffer
 * No longer return nilCollectable.
 * 
 *    Rev 1.2   05 Nov 1991 14:06:52   keffer
 * Removed RWXXXXErr macros --- now done by error package
 * 
 *    Rev 1.1   28 Oct 1991 09:24:30   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 08:39:48   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

/*
 * Member function index() is redefined from RWOrdered, allowing many
 * function defined in RWOrdered to still work, even for this class.
 * Examples: find(), remove().
 */

#include "rw/sortvec.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"
#include "defcol.h"
                         
RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: sortvec.cpp,v $ $Revision: 6.5 $ $Date: 1994/07/18 20:51:00 $");

RWDEFINE_COLLECTABLE2(RWSortedVector, __RWSORTEDVECTOR)

RWSortedVector::RWSortedVector(size_t size) : RWOrdered(size) { }           

RWSortedVector::~RWSortedVector() {}

  static int
  comparison(const void* a, const void* b)
  {
    return (*(RWCollectable**)a)->compareTo(*(RWCollectable**)b);
  }

#ifndef RW_NO_CONST_OVERLOAD
/*
 * This function is allowed because it returns a pointer to a 
 * constant object --- the object can't be changed which would disrupt
 * the internal ordering of objects.
 */
const RWCollectable*
RWSortedVector::at(size_t inx) const
{
  const RWCollectable* temp = (*this)[inx];
  return temp;
}
#endif

/*
 * Use a binary search to find the position of an object.
 */
size_t
RWSortedVector::index(const RWCollectable* p) const
{
  size_t idx;
  if (!RWbsearch(&p,			      // The key
		 (RWCollectable**)vec.data(), // The candidates
		 entries(),		      // Number of candidates
		 sizeof(RWCollectable*),      // Size of each candidate
		 comparison,		      // Comparison function
		 idx) )			      // Returned index
    return RW_NPOS;		// Not in collection


  // The item is in the collection.
  // Search downwards, looking for the first instance:

  while (idx && this->vec(idx-1)->compareTo(p)==0)
    --idx;

  RWPOSTCONDITION(idx<entries());
  RWPOSTCONDITION(this->vec(idx)->compareTo(p)==0);

  return idx;
}

RWCollectable*
RWSortedVector::insert(RWCollectable* p)
{
  size_t idx;
  if (RWbsearch(&p,
		(RWCollectable**)vec.data(),
		entries(),
		sizeof(RWCollectable*),
		comparison,
		idx) )
  {
    // A matching item was found.  Insert after the
    // last equal item.
    while (idx<entries() && this->vec(idx)->compareTo(p)==0)
      ++idx;
  }
  else
  {
    // No matching item found.  Search upward
    // for the first item greater than the value
    // and insert before it.
    while (idx<entries() && this->vec(idx)->compareTo(p) < 0)
      ++idx;
  }
  return RWOrdered::insertAt(idx, p);
}

RWBoolean
RWSortedVector::isEqual(const RWCollectable* c) const
{
  if( c->isA() != __RWSORTEDVECTOR ) return FALSE;
  return RWSortedVector::operator==(*(const RWSortedVector*)c);
}

size_t
RWSortedVector::occurrencesOf(const RWCollectable* p) const
{
  size_t total = 0;
  size_t idx = index(p);
  if(idx != RW_NPOS)
  {
    // Keep counting until we find an object that does not compare equal:
    while( idx<nitems && this->vec(idx)->compareTo(p)==0 ) {total++; idx++;}
  }

  return total;
}
/********************************************************
 *							*
 *		Disallowed from RWOrdered		*
 *							*
 ********************************************************/

RWCollectable*
RWSortedVector::append(RWCollectable*)
{
  RWTHROW( RWInternalErr(RWMessage( RWTOOL_NOTALLOW) ));
  return rwnil;
}

/* 
 * This function is disallowed because it would be dangerous to use
 * it as an lvalue: the internal sort order of objects could be changed.
 */

RWCollectable*&
RWSortedVector::at(size_t)
{
  RWTHROW( RWInternalErr(RWMessage( RWTOOL_NOTALLOW) ));
  // These statements should never be reached and are just to 
  // keep the compiler happy
  static RWCollectable* retval = rwnil;
  return retval;	
}

// For backwards compatiblity:
RWCollectable*
RWSortedVector::insertAfter(int, RWCollectable*)
{
  RWTHROW( RWInternalErr(RWMessage( RWTOOL_NOTALLOW) ));
  return rwnil;
}

RWCollectable*
RWSortedVector::insertAt(size_t, RWCollectable*)
{
  RWTHROW( RWInternalErr(RWMessage( RWTOOL_NOTALLOW) ));
  return rwnil;
}

RWCollectable*
RWSortedVector::prepend(RWCollectable*)
{
  RWTHROW( RWInternalErr(RWMessage( RWTOOL_NOTALLOW) ));
  return rwnil;
}

