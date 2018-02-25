#ifndef __RW_SCROLL_BAR_H
#define __RW_SCROLL_BAR_H
/*********************************************************************
*
*  $Id: ScrollBar.h,v 1.12 1993/10/07 01:42:57 keffer Exp $
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
*  File        : ScrollBar.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                ScrollBar class.
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

#include <rw/Xrw/Primitive.h>
#include <rw/Xrw/ScrollBCB.h>
	 
/*********************************************************************
* RWScrollBar : Class object which represents the XmScrollBar
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWScrollBar : public RWPrimitive
{
 public:
  RWScrollBar
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWScrollBar 
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWScrollBar 
    (
    RWController *,
    RWResourceObj * ro = 0
    );
  RWScrollBar
    (
    Widget,
    RWBoolean managed = False
    );  // Internal.

  ~RWScrollBar();

  void          createWidget              (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void          setDecrementCallback	  (RWXtCallbackProc, RWXtPointer);
  void          setDecrementMethod	  (RWApplication *, RWMethodPtr, void *);
  void          setDecrementCallback	  (XtCallbackList);
  void          setDragCallback		  (RWXtCallbackProc, RWXtPointer);
  void          setDragMethod		  (RWApplication *, RWMethodPtr, void *);
  void          setDragCallback		  (XtCallbackList);
  void          setIncrement		  (int);
  int           getIncrement		  (int *val = 0);
  void          setIncrementCallback	  (RWXtCallbackProc, RWXtPointer);
  void          setIncrementMethod	  (RWApplication *, RWMethodPtr, void *);
  void          setIncrementCallback	  (XtCallbackList);
  void          setInitialDelay		  (int);
  int           getInitialDelay		  (int *val = 0);
  void          setMaximum		  (int);
  int           getMaximum		  (int *val = 0);
  void          setMinimum		  (int);
  int           getMinimum		  (int *val = 0);
  void          setOrientation		  (unsigned char);
  unsigned char getOrientation		  (unsigned char *val = 0);
  void          setPageDecrementCallback  (RWXtCallbackProc, RWXtPointer);
  void          setPageDecrementMethod	  (RWApplication *, RWMethodPtr, void *);
  void          setPageDecrementCallback  (XtCallbackList);
  void          setPageIncrement	  (int);
  int           getPageIncrement	  (int *val = 0);
  void          setPageIncrementCallback  (RWXtCallbackProc, RWXtPointer);
  void          setPageIncrementMethod	  (RWApplication *, RWMethodPtr, void *);
  void          setPageIncrementCallback  (XtCallbackList);
  void          setProcessingDirection	  (unsigned char);
  unsigned char getProcessingDirection	  (unsigned char *val = 0);
  void          setRepeatDelay		  (int);
  int           getRepeatDelay		  (int *val = 0);
  void          setShowArrows		  (Boolean);
  Boolean       getShowArrows		  (Boolean *val = 0);
  void          setSliderSize		  (int);
  int           getSliderSize		  (int *val = 0);
  void          setToBottomCallback	  (RWXtCallbackProc, RWXtPointer);
  void          setToBottomMethod	  (RWApplication *, RWMethodPtr, void *);
  void          setToBottomCallback	  (XtCallbackList);
  void          setToTopCallback	  (RWXtCallbackProc, RWXtPointer);
  void          setToTopMethod		  (RWApplication *, RWMethodPtr, void *);
  void          setToTopCallback	  (XtCallbackList);
  void          setTroughColor		  (Pixel);
  Pixel         getTroughColor		  (Pixel *val = 0);
  void          setValue		  (int);
  int           getValue		  (int *val = 0);
  void          setValueChangedCallback	  (RWXtCallbackProc, RWXtPointer);
  void          setValueChangedMethod	  (RWApplication *, RWMethodPtr, void *);
  void          setValueChangedCallback	  (XtCallbackList);

// MOTIF MEMBER FUNCTIONS  
  void          getValues                 (int *, int *, int *, int *);
  void          setValues                 (int, int, int, int, Boolean);
  RWCallback *  getCallbackObject         (RWXtPointer);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWSCROLLBAR;}
  
 private:
};

#endif  /* End of file ScrollBar.h - Do not add beyond this point! */
