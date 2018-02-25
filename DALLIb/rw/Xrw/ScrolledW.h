#ifndef __RW_SCROLLED_WINDOW_H
#define __RW_SCROLLED_WINDOW_H
/*********************************************************************
*
*  $Id: ScrolledW.h,v 1.10 1993/10/22 22:05:14 keffer Exp $
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
*  File        : ScrolledW.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Scrolled Window class.
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

#include <rw/Xrw/Manager.h>

#if RWX11Version>=5
#include <rw/Xrw/ScrollWCB.h>
#endif
	 
/*********************************************************************
* RWScrolledWindow : Class object which represents the XmScrolledWindow
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWScrolledWindow : public RWManager
{
 public:
  RWScrolledWindow
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWScrolledWindow
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWScrolledWindow
    (
    RWController *,
    RWResourceObj * ro = 0
    );
  RWScrolledWindow();
  RWScrolledWindow
    (
    Widget,
    RWBoolean managed = False
    );  // Internal.
  ~RWScrolledWindow();

  void           createWidget                   (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  RWController * getClipWindow			(RWController **val = 0);
  void           setHorizontalScrollBar		(RWController *);
  RWController * getHorizontalScrollBar		(RWController **val = 0);
  unsigned char  getScrollBarDisplayPolicy	(unsigned char *val = 0);
  void           setScrollBarPlacement		(unsigned char);
  unsigned char  getScrollBarPlacement		(unsigned char *val = 0);
  void           setScrolledWindowMarginHeight	(Dimension);
  Dimension      getScrolledWindowMarginHeight	(Dimension *val = 0);
  void           setScrolledWindowMarginWidth	(Dimension);
  Dimension      getScrolledWindowMarginWidth	(Dimension *val = 0);
  unsigned char  getScrollingPolicy		(unsigned char *val = 0);
  void           setSpacing			(Dimension);
  Dimension      getSpacing			(Dimension *val = 0);
  void           setVerticalScrollBar		(RWController *);
  RWController * getVerticalScrollBar		(RWController **val = 0);
  unsigned char  getVisualPolicy		(unsigned char *val = 0);
  void           setWorkWindow			(RWController *);
  RWController * getWorkWindow			(RWController **val = 0);

#if RWX11Version>=5
  void           scrollVisible                  (RWController *, Dimension, Dimension);
  void           setTraverseObscuredCallback    (RWXtCallbackProc, RWXtPointer);
  void           setTraverseObscuredMethod      (RWApplication *, RWMethodPtr, void *);
  void           setTraverseObscuredCallback    (XtCallbackList);
#endif

//MOTIF MEMBER FUNCTIONS  
  void           setAreas                       (RWController *, 
						 RWController *, 
						 RWController *);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWSCROLLEDWINDOW;}

#if RWX11Version>=5
private:
  RWCallback * getCallbackObject (RWXtPointer);
#endif

};

#endif  /* End of file ScrolledW.h - Do not add beyond this point! */
