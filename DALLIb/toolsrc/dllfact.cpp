/*
 * Create function for standard Rogue Wave objects.  Used by the Windows DLL only!
 *
 * $Id: dllfact.cpp,v 6.5 1994/07/18 20:51:00 jims Exp $
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
 * The function defined herein is used only by the DLL.  A standard RWFactory
 * is not necessary because all objects are known and available at link time.
 * There is no space savings by only including objects used by the user.
 * Furthermore, the memory allocated by the Factory would belong to the allocating
 * application.  When it exited the memory would be blown away!
 * Hence, using a giant switch statement results in time and space savings as
 * well as being more robust.
 *
 ***************************************************************************
 *
 * $Log: dllfact.cpp,v $
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.3  1994/06/10  17:39:19  jims
 * Now used for shared libraries
 *
 * Revision 6.2  1994/05/16  17:06:28  jims
 * Port to Win32 DLL
 *
 * Revision 6.1  1994/04/15  20:33:00  vriezen
 * Move all files to 6.1
 *
 * Revision 2.2  1993/06/05  21:36:22  keffer
 * Added case statements for __RWHASHTABLE and __RWCOLLECTABLEIDASSOCIATION.
 *
 * Revision 2.1  1993/03/13  02:36:40  keffer
 * ClassID->RWClassID
 *
 * Revision 2.0  1992/10/23  03:36:49  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.3   18 Mar 1992 11:44:32   KEFFER
 * Changed nil to rwnil.
 * 
 *    Rev 1.2   12 Nov 1991 09:09:30   keffer
 * 
 */

// Only used for building DLLs and shared libraries

#if defined(_RWBUILDDLL) || defined(_RWBUILDSHARED)

#include "rw/bintree.h"
#include "rw/btrdict.h"
#include "rw/btree.h"
#include "rw/collass.h"
#include "rw/colldate.h"
#include "rw/collint.h"
#include "rw/collstr.h"
#include "rw/colltime.h"
#include "rw/dlistcol.h"
#include "rw/hashdict.h"
#include "rw/idendict.h"
#include "rw/idenset.h"
#include "rw/ordcltn.h"
#include "rw/queuecol.h"
#include "rw/rwbag.h"
#include "rw/rwset.h"
#include "rw/seqcltn.h"
#include "rw/slistcol.h"
#include "rw/sortvec.h"
#include "rw/stackcol.h"
#include "dllfact.h"

RWCollectable*
rwCreateFromDLLFactory(RWClassID id)
{
  RWCollectable* ret;

  switch(id) {
  case __RWBAG:
    ret = new RWBag;
    break;    
  case __RWBINARYTREE:
    ret = new RWBinaryTree;
    break;    
  case __RWBTREE:
    ret = new RWBTree;
    break;    
  case __RWBTREEDICTIONARY:
    ret = new RWBTreeDictionary;
    break;    
  case __RWCOLLECTABLE:
    ret = new RWCollectable;
    break;    
  case __RWCOLLECTABLEASSOCIATION:
    ret = new RWCollectableAssociation;
    break;    
  case __RWCOLLECTABLEIDASSOCIATION:
    ret = new RWCollectableAssociation;
    break;
  case __RWCOLLECTABLEDATE:
    ret = new RWCollectableDate;
    break;    
  case __RWCOLLECTABLEINT:
    ret = new RWCollectableInt;
    break;    
  case __RWCOLLECTABLESTRING:
    ret = new RWCollectableString;
    break;    
  case __RWCOLLECTABLETIME:
    ret = new RWCollectableTime;
    break;    
  case __RWDLISTCOLLECTABLES:
    ret = new RWDlistCollectables;
    break;    
  case __RWHASHDICTIONARY:
    ret = new RWHashDictionary;
    break;    
  case __RWHASHTABLE:
    ret = new RWHashTable;
    break;
  case __RWIDENTITYDICTIONARY:
    ret = new RWIdentityDictionary;
    break;    
  case __RWIDENTITYSET:
    ret = new RWIdentitySet;
    break;    
  case __RWORDERED:
    ret = new RWOrdered;
    break;    
  case __RWSET:
    ret = new RWSet;
    break;    
  case __RWSLISTCOLLECTABLES:
    ret = new RWSlistCollectables;
    break;    
  case __RWSLISTCOLLECTABLESQUEUE:
    ret = new RWSlistCollectablesQueue;
    break;    
  case __RWSLISTCOLLECTABLESSTACK:
    ret = new RWSlistCollectablesStack;
    break;    
  case __RWSORTEDVECTOR:
    ret = new RWSortedVector;
    break;    
  default:
    ret = rwnil;
  };

  return ret;
}

#else /* neither dll nor shared */

// This is to quiet fussy librarians if nothing gets compiled:
int rwDummy_dllfact_cpp;

#endif /* _RWBIULDDLL || _RWBUILDSHARED */

