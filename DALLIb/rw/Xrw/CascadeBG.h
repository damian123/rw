#ifndef __RW_CASCADE_BUTTON_GADGET_H
#define __RW_CASCADE_BUTTON_GADGET_H
/*********************************************************************
*
*  $Id: CascadeBG.h,v 1.11 1993/10/07 01:42:57 keffer Exp $
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
*  File        : CascadeBG.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Cascade Button Gadget class.
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

/*********************************************************************
* #include files
*********************************************************************/

#include <rw/Xrw/LabelG.h>

class RWPulldownMenu;

/*********************************************************************
* RWCascadeButtonGadget : Class object which represents the
* XmCascadeButtonGadget widget in the OSF/Motif widget set.
*********************************************************************/

class RWCascadeButtonGadget : public RWLabelGadget {
public:
  RWCascadeButtonGadget
    (
     RWController *    parent, 
     const RWCString & name, 
     RWBoolean         managed = False
    );
  RWCascadeButtonGadget
    (
     RWController *, 
     const RWCString &,
     const RWResourceObj &,
     RWBoolean managed = False
    );
  RWCascadeButtonGadget
    (
     RWController *, 
     RWResourceObj * = 0
    );
  RWCascadeButtonGadget
    (
     Widget, 
     RWBoolean managed = False
     );  // Internal.

  ~RWCascadeButtonGadget();

  void             createWidget         (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void             setActivateCallback  (RWXtCallbackProc, RWXtPointer);
  void             setActivateMethod    (RWApplication *, RWMethodPtr, void *);
  void             setActivateCallback  (XtCallbackList);
  void             setCascadePixmap     (Pixmap);
  Pixmap           getCascadePixmap     (Pixmap *val = 0);
  void             setCascadingCallback (RWXtCallbackProc, RWXtPointer);
  void             setCascadingMethod   (RWApplication *, RWMethodPtr, void *);
  void             setCascadingCallback (XtCallbackList);
  void             setMappingDelay      (int);
  int              getMappingDelay      (int *val = 0);
  void             setSubMenuId         (RWPulldownMenu *);
  RWPulldownMenu * getSubMenuId         (RWPulldownMenu **val = 0);

// MOTIF MEMBER FUNCTIONS
  void           highlight            (Boolean);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const { return __RWCASCADEBUTTONGADGET; }
};

#endif  /* End of file CascadeBG.h - Do not add beyond this point! */
