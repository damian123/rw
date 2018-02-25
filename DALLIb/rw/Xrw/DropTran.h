#ifndef __RW_DROPTRANSFER_H
#define __RW_DROPTRANSFER_H
/*********************************************************************
*
*  $Id: DropTran.h,v 1.9 1993/10/07 01:42:57 keffer Exp $
*
*  COPYRIGHT   : (c) Copyright 1991, Rogue Wave Software, Inc.
*                ALL RIGHTS RESERVED
*
*                The software and information contained herein
*                are proprietary to, and comprise valuable trade
*                secrets of, Rogue Wave Software, Inc., which
*                intends to preserve as trade secrets such software
*                and information.  This software is furnished pursuant
*                to a written license agreement and may be used,
*                copied, transmitted, and stored only in accordance
*                with the terms of such license and with the inclusion
*                of the above copyright notice.  This software and
*                information or any other copies thereof may not be
*                provided or otherwise made available to any other
*                person.
*  
*  NOTICE      : Notwithstanding any other lease or license that may
*                pertain to, or accompany the delivery of, this
*                computer software and information, the rights of the
*                Government regarding its use, reproduction and
*                disclosure are as set forth in Section 52.227-19 of
*                the FARS Computer Software-Restricted Rights clause.
*  
*  RESTRICTED 
*  RIGHTS 
*  NOTICE      : Use, duplication, or disclosure by the Government
*                is subject to restrictions as set forth in
*                subparagraph (c)(1)(ii) of the Rights in Technical
*                Data and Computer Software clause at DFARS
*                52.227-7013.
*  
*  RESTRICTED
*  RIGHTS
*  LEGEND      : This computer software and information is distributed
*                with "restricted rights."  Use, duplication or
*                disclosure is subject to restrictions as set forth
*                in NASA FAR SUP 18-52.227-79 (April 1985)
*                "Commercial Computer Software-Restricted Rights
*                (April 1985)."  If the Clause at 18-52.227-74
*                "Rights in Data General" is specified in the
*                contract, then the "Alternate III" clause applies.
*
*                (c) Copyright 1991, Rogue Wave Software, Inc.
*                ALL RIGHTS RESERVED
*
*  TRADEMARKS  : Rogue Wave is a trademark of
*                   Rogue Wave Software, Inc.
*                OSF/Motif is a trademark of
*                   Open Software Foundation, Inc.
*                Motif is a trademark of
*                   Open Software Foundation, Inc.
*                X Window System is a trademark of
*                   the Massachusetts Institute of Technology
*
*  Rogue Wave  : Rogue Wave Software, Inc.
*                P.O. Box 2328
*                Corvallis, OR 97330
*                (503) 754-2311
*
*  ------------------------------------------------------------------
*
*  File        : DropTran.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1992
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                XmDropTransfer class.
*
*
*  Revision History:
*
*********************************************************************/

/*********************************************************************
*
* #include files
*
*********************************************************************/

#include <rw/Xrw/Control.h>
#include <rw/Xrw/DropSiteCB.h>
#include <Xm/DragDrop.h>

/*********************************************************************
* RWDropTransfer : View.h++ class object that encapsulates the 
* XmDropTransfer data type in OSF/Motif.
*********************************************************************/

class RWDropTransferEntryRec : public RWCollectable {
public:
  RWDropTransferEntryRec (Atom a, RWXtPointer cd) 
        { entry.target = a; entry.client_data = cd; }
  RWDropTransferEntryRec & operator=(RWDropTransferEntryRec const & e) 
        { entry.target = e.entry.target; 
	  entry.client_data = e.entry.client_data; 
	  return *this; 
	}
  Atom getTarget() { return entry.target; }
  RWXtPointer getClientData() { return entry.client_data; }  
private:
  XmDropTransferEntryRec entry;
};


class RWDropTransfer : public RWController {
public:
  RWDropTransfer 
	(
	RWController *,
	RWResourceObj,
	RWXtPointer cb = 0
	);
  RWDropTransfer 
	(
	RWController *,
	RWXtPointer cb = 0
	);
  RWDropTransfer
        (
	Widget,
	RWResourceObj,
	RWXtPointer cb = 0
	);
  RWDropTransfer
        (
	Widget,
	RWXtPointer cb = 0
	);
  RWDropTransfer
        (
	RWDropProcCallback *,
	const RWOrdered &,
	RWXtPointer cb = 0
	);
  ~RWDropTransfer ();

  void                      setDropTransfers (const RWOrdered &);
  RWOrdered *               getDropTransfers (RWOrdered ** val = 0);
  void 			    setIncremental (Boolean);
  Boolean 		    getIncremental (Boolean * val = 0);
  void			    setNumDropTransfers (Cardinal);
  Cardinal		    getNumDropTransfers (Cardinal * val = 0);
  void			    setTransferProc (RWXtSelectionCallbackProc);
  RWXtSelectionCallbackProc getTransferProc (RWXtSelectionCallbackProc * val = 0);
  void                      setTransferStatus (unsigned char);
  unsigned char             getTransferStatus (unsigned char * val = 0);

// RW TOOLS MEMBER FUNCTIONS
  void add (XmDropTransferEntryRec *, Cardinal);
  
  virtual RWClassID isA () const {return __RWDROPTRANSFER;}
  
  RWXtSelectionCallbackProc dropTransferCB;
  RWOrdered * transferList;

private:
  void  setDropTransfers (XmDropTransferEntryRec *);
};

#endif
