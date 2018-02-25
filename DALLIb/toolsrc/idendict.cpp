
/*
 * Definitions for RWIdentityDictionary
 *
 * $Id: idendict.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
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
 * This class is very similar similar to a regular hashed dictionary.
 * The only difference is that an item is found by requiring that it
 * be "identical to" the key, rather than "equal to".  This is done by
 * using RWCollectableIDAssociations in place of RWCollectableAssociations.
 *
 * Accomplishing this can be done very simply by just overriding the
 * virtual function "newAssociation()".
 *
 ***************************************************************************
 *
 * $Log: idendict.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/06/18  01:56:54  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.1  1994/04/15  19:06:45  vriezen
 * Move all files to 6.1
 *
 * Revision 2.5  1994/03/04  01:04:16  jims
 * Override isEqual member function from RWCollectable to return
 * TRUE or FALSE based on operator==
 *
 * Revision 2.4  1993/11/09  08:37:30  jims
 * Port to ObjectStore
 *
 * Revision 2.3  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.2  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.1  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.6   29 May 1992 09:46:50   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.5   27 May 1992 18:09:04   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.4   25 May 1992 15:37:12   KEFFER
 * Optimized, reducing size, by using virtual functions.
 * 
 *    Rev 1.3   29 Apr 1992 14:50:56   KEFFER
 * Hashing now uses chaining to resolve collisions
 * 
 *    Rev 1.1   28 Oct 1991 09:24:14   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 08:39:28   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/defs.h"	/* necessary to avoid nesting limits in /lib/cpp */
#include "rw/idendict.h"
#include "rw/collass.h"
#include "defcol.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: idendict.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

RWDEFINE_COLLECTABLE2(RWIdentityDictionary, __RWIDENTITYDICTIONARY)

RWIdentityDictionary::RWIdentityDictionary(size_t N) :
  RWHashDictionary(N)
{
}

RWIdentityDictionary::~RWIdentityDictionary() {}

/*
 * Protected virtual function to find the association for a given
 * key, or return nil if there is no such association.
 */
RWCollectableAssociation*
RWIdentityDictionary::findAssociation(const RWCollectable* key) const
{
  RWCollectableIDAssociation assoc((RWCollectable*)key,rwnil);
  return (RWCollectableAssociation*)RWSet::find(&assoc);
}


RWCollectableAssociation*
RWIdentityDictionary::newAssociation(RWCollectable* k, RWCollectable* v) const
{
  return new RWCollectableIDAssociation(k,v);
}


/* Protected virtual function to remove and return the association
 * for a given key, or return nil if there is no such association.
 */
RWCollectableAssociation*
RWIdentityDictionary::removeAssociation(const RWCollectable* key)
{
  RWCollectableIDAssociation assoc((RWCollectable*)key,rwnil);
  return (RWCollectableAssociation*)RWSet::remove(&assoc);
}

RWBoolean
RWIdentityDictionary::isEqual(const RWCollectable* c) const
{
  if( c->isA() != __RWIDENTITYDICTIONARY ) return FALSE;
  return operator==(*(const RWIdentityDictionary*)c);
}
