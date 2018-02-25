/*
 * Definitions for RWReference
 *
 * $Id: ref.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * Implementation of reference-counting garbage-collection primitives for
 * multi-threaded environment.
 *
 ***************************************************************************
 *
 * $Log: ref.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:06:32  vriezen
 * Move all files to 6.1
 *
 * Revision 1.7  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.6  1993/09/01  03:40:34  myersn
 * change implementation semantics to allow easy static initialization.
 *
 * Revision 1.5  1993/05/29  18:48:24  keffer
 * Renamed or added a dummy variable in case nothing gets compiled
 *
 * Revision 1.4  1993/05/25  18:41:14  keffer
 * Added bogus definition rwUtilityDummy2 to quiet fussy librarians.
 *
 * Revision 1.3  1993/04/12  12:11:22  jims
 * Now uses RWMutex class for locking
 *
 * Revision 1.2  1993/02/03  19:58:44  myersn
 * cache value of refcount while locked in removeReference, as return value.
 *
 * Revision 1.1  1993/01/29  20:17:32  myersn
 * Initial revision
 */

#include "rw/ref.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: ref.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#if defined(RW_MULTI_THREAD)
#include "rw/mutex.h"

void
RWReference::addReference(RWMutex& mutex)
{
  mutex.acquire();
  ++refs_;
  mutex.release();
}

unsigned
RWReference::removeReference(RWMutex& mutex)
{
  mutex.acquire();
  unsigned int result = refs_--;
  mutex.release();
  return result;
}

#else

// This definition is compiled in case nothing else is,
// in order to quiet down some fussy librarians:
int rwDummy_ref_cpp;

#endif

