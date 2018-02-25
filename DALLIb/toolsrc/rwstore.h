#ifndef __RWSTORETABLE_H__
#define __RWSTORETABLE_H__

/*
 * Definitions for internal classes used to store collectables
 *
 * $Id: rwstore.h,v 6.2 1994/07/12 21:18:27 vriezen Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 * Voice: (503) 754-2311	FAX: (503) 757-7350
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
 * $Log: rwstore.h,v $
 * Revision 6.2  1994/07/12  21:18:27  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:07:40  vriezen
 * Move all files to 6.1
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.3   13 Nov 1991 11:53:42   keffer
 * Typedefs RWReadTable to an ordered collection.
 * 
 *    Rev 1.1   28 Oct 1991 09:24:26   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 09:23:06   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */


/**
 *******************  RWStoreTable declarations ***************************
 **/

#include "rw/rwset.h"

class RWExport RWStoreTable : public RWSet {
public:
  RWStoreTable();
  ~RWStoreTable();
  RWBoolean		add(const RWCollectable*, int&);
};

class RWExport RWStoreEntry : public RWCollectable {
friend class RWStoreTable;
  const RWCollectable*	item;
  int			objectNumber;
public:
  RWStoreEntry(const RWCollectable* c, int n){item=c; objectNumber=n;}
  unsigned		hash() const;
  RWBoolean		isEqual(const RWCollectable* c) const;
};

/**
 *******************  RWReadTable declarations ***************************
 **/

// For the read table, just use an ordered collection and typedef it:
#include "rw/ordcltn.h"
typedef RWOrdered RWReadTable;

#endif	/* __RWSTORETABLE_H__ */
