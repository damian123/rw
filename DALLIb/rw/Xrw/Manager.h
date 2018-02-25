#ifndef __RW_MANAGER_H
#define __RW_MANAGER_H
/*********************************************************************
*
*  $Id: Manager.h,v 1.9 1993/10/22 22:05:14 keffer Exp $
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
*  File        : Manager.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Manager class.  This C++ class represents both
*                OSF/Motif widget classes XmConstraint and XmManager.
*                From a C++ perspective, there was no need to
*                implement both of these because they are not directly
*                instantiated.  Subclasses merely need access to their
*                resources; the proper inheritance of which are handled
*                by Motif.  Member functions to access the combined
*                XmConstraint and XmManager resources are provided on
*                this class object.
*                
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

/*********************************************************************
*
* #include files
*
*********************************************************************/

#include <rw/Xrw/Composite.h>


/*********************************************************************
* RWManager : Class object which represents the Manager
* widget in the OSF/Motif widget set.  This is an internal class
* which is not meant to be instantiated directly.
*********************************************************************/

class RWManager : public RWComposite
{
 public:
  RWManager();         // Internal.  
  RWManager
    (
    Widget, 
    Boolean managed = False
    );                 // Internal.

  ~RWManager();

// RESOURCE MEMBER FUNCTIONS
  void                  setBottomShadowColor  (const RWCString &);
  void                  setBottomShadowColor  (Pixel);
  Pixel                 getBottomShadowColor  (Pixel *val = 0);
  void                  setBottomShadowPixmap (Pixmap);
  Pixmap                getBottomShadowPixmap (Pixmap *val = 0);
  void                  setForeground         (const RWCString &);
  void                  setForeground         (Pixel);
  Pixel                 getForeground         (Pixel *val = 0);
  void                  setHelpCallback       (RWXtCallbackProc, RWXtPointer);
  void                  setHelpMethod         (RWApplication *, RWMethodPtr, void *);
  void                  setHelpCallback       (XtCallbackList);
  void                  setHighlightColor     (const RWCString &);
  void                  setHighlightColor     (Pixel);
  Pixel                 getHighlightColor     (Pixel *val = 0);
  void                  setHighlightPixmap    (Pixmap);
  Pixmap                getHighlightPixmap    (Pixmap *val = 0);
  void                  setNavigationType     (XmNavigationType);
  XmNavigationType      getNavigationType     (XmNavigationType *val = 0);
  void                  setShadowThickness    (Dimension);
  Dimension             getShadowThickness    (Dimension *val = 0);
  XmStringDirection     getXmStringDirection  (XmStringDirection *val = 0);
  RWXmStringDirection * getStringDirection    (RWXmStringDirection **val = 0);
  void                  setTopShadowColor     (const RWCString &);
  void                  setTopShadowColor     (Pixel);
  Pixel                 getTopShadowColor     (Pixel *val = 0);
  void                  setTopShadowPixmap    (Pixmap);
  Pixmap                getTopShadowPixmap    (Pixmap *val = 0);
  void                  setTraversalOn        (Boolean);
  Boolean               getTraversalOn        (Boolean *val = 0);
  void                  setUnitType           (unsigned char);
  unsigned char         getUnitType           (unsigned char *val = 0);
  void                  setUData              (RWXtPointer);
  RWXtPointer           getUserData           (void *val = 0);
  RWXtPointer           getUData              (RWXtPointer *val = 0);

#if RWX11Version>=5
  void                  setInitialFocus       (RWController *);
  RWController *        getInitialFocus       (RWController **val = 0);
#endif

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWMANAGER;}
  
 private:
};

#endif  /* End of file Manager.h - Do not add beyond this point! */


