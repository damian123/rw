
/*
 * Definitions for RWPtrVector
 *
 * $Id: pvector.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: pvector.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:07:23  vriezen
 * Move all files to 6.1
 *
 * Revision 1.9  1994/03/02  19:20:24  jims
 * New message INDEXERR shows invalid index and number of items
 * in collection
 *
 * Revision 1.8  1993/12/09  23:53:21  jims
 * ObjectStore version: realign heading
 *
 * Revision 1.7  1993/11/09  09:01:23  jims
 * Port to ObjectStore
 *
 * Revision 1.6  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.5  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 1.4  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 1.3  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 1.2  1993/03/24  23:49:27  keffer
 * Indexing operations now used unsigned
 *
 * Revision 1.1  1993/02/17  18:26:24  keffer
 * Initial revision
 *
 *
 ***************************************************************************
 */

#include "rw/pvector.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: pvector.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

RWPtrVector::RWPtrVector(register size_t n, void* p)
{
  register RWvoid* dst = array_ = new RWvoid[npts_=n];
  while (n--) *dst++ = p;
}


RWPtrVector::RWPtrVector(const RWPtrVector& a)
{
  register size_t i= npts_ = a.npts_;
  register RWvoid* dst = array_ = new RWvoid[i];
  register RWvoid* src = a.array_;
  while (i--) *dst++ = *src++;
}

RWPtrVector&
RWPtrVector::operator=(const RWPtrVector& a)
{
  if(array_ != a.array_){
    RWVECTOR_DELETE(npts_) array_;	/* Disconnect from old array_ */
    register size_t i = npts_ = a.npts_;
    register RWvoid* dst = array_ = new RWvoid[i];
    register RWvoid* src = a.array_;
    while (i--) *dst++ = *src++;
  }
  return *this;
}
										
RWPtrVector&
RWPtrVector::operator=(void* p)
{
  for (register size_t i=0; i<npts_; i++)
    array_[i] = p;
  return *this;
}

void
RWPtrVector::reshape(size_t N)
{
  if (N==npts_) return;
  RWvoid* newArray = new RWvoid[N];
  register size_t i = (N<=npts_) ? N:npts_;
  register RWvoid* src = array_;
  register RWvoid* dst = newArray;
  while (i--) *dst++ = *src++;
  RWVECTOR_DELETE(npts_) array_;
  array_ = newArray;
  npts_ = N;
}

void
RWPtrVector::resize(size_t N)
{
  size_t oldN = length();
  reshape(N);
  for (register size_t i = oldN; i<N; i++)
    array_[i] = rwnil;
}

void
RWPtrVector::boundsCheck(size_t i) const
{
  if (i>=npts_)
    RWTHROW( RWBoundsErr(RWMessage(RWTOOL_INDEXERR, (unsigned)i, (unsigned)npts_) ));
}
