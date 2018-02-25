#ifndef __RW_DRAWING_AREA_H
#define __RW_DRAWING_AREA_H
/*********************************************************************
*
*  $Id: DrawingA.h,v 1.12 1993/10/07 01:42:57 keffer Exp $
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
*  File        : DrawingA.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Drawing Area class.
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
#include <rw/Xrw/DrawACB.h>

/*********************************************************************
* RWDrawingArea : Class object which represents the XmDrawingArea
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWDrawingArea : public RWManager {
public:
  RWDrawingArea
    (
    RWController *, 
    const RWCString &,
    RWBoolean managed = False
    );
  RWDrawingArea
    (
    RWController *, 
    const RWCString &, 
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWDrawingArea
    (
    RWController *, 
    RWResourceObj * ro = 0
    );
  RWDrawingArea
    (
    Widget, 
    RWBoolean managed = False
    );  // Internal.
  ~RWDrawingArea();

  void          createWidget      (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS

  void          setExposeCallback (RWXtCallbackProc, RWXtPointer);
  void          setExposeMethod   (RWApplication *, RWMethodPtr, void *);
  void          setExposeCallback (XtCallbackList);
  void          setInputCallback  (RWXtCallbackProc, RWXtPointer);
  void          setInputMethod    (RWApplication *, RWMethodPtr, void *);
  void          setInputCallback  (XtCallbackList);
  void          setMarginHeight   (Dimension);
  Dimension     getMarginHeight   (Dimension *val = 0);
  void          setMarginWidth    (Dimension);
  Dimension     getMarginWidth    (Dimension *val = 0);
  void          setResizeCallback (RWXtCallbackProc, RWXtPointer);
  void          setResizeMethod   (RWApplication *, RWMethodPtr, void *);
  void          setResizeCallback (XtCallbackList);
  void          setResizePolicy   (unsigned char);
  unsigned char getResizePolicy   (unsigned char *val = 0);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const 
    {
    return __RWDRAWINGAREA;
    }
  
 private:
  RWCallback *getCallbackObject (RWXtPointer);
};

#endif  /* End of file DrawingA.h - Do not add beyond this point! */
