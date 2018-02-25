#ifndef __RW_CORE_H
#define __RW_CORE_H
/*********************************************************************
*
*  $Id: Core.h,v 1.10 1993/10/17 22:02:56 keffer Exp $
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
*  File        : Core.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                core class.  This C++ class represents both
*                OSF/Motif widget classes WindowObj and Core.
*                From a C++ perspective, there was no need to
*                implement both of these because they are not directly
*                instantiated.  Subclasses merely need access to their
*                resources; the proper inheritance of which are handled
*                by Motif.  Member functions to access the combined
*                WindowObj and Core resources are provided on
*                this class object.
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

#include <rw/Xrw/Control.h>


/*********************************************************************
* RWCore : Class object which represents the core
* widget in the OSF/Motif widget set.  This is an internal class
* which is not meant to be instantiated directly.
*********************************************************************/

class RWCore : public RWController
{
public:
  RWCore();  // Internal.  
  RWCore
  (
   Widget, 
   Boolean managed = False
   );       // Internal.
  
  virtual ~RWCore();
  
// RESOURCE MEMBER FUNCTIONS
  void           setAccelerators      (XtTranslations);
  XtTranslations getAccelerators      (XtTranslations *val = 0);
  void           setBackground        (const RWCString &);
  void           setBackground        (Pixel);
  Pixel          getBackground        (Pixel *val = 0);
  void           setBackgroundPixmap  (Pixmap);
  Pixmap         getBackgroundPixmap  (Pixmap *val = 0);
  void           setBorderColor       (const RWCString &);
  void           setBorderColor       (Pixel);
  Pixel          getBorderColor       (Pixel *val = 0);
  void           setBorderPixmap      (Pixmap);
  Pixmap         getBorderPixmap      (Pixmap *val = 0);
  Colormap       getColormap          (Colormap *val = 0);
  int            getDepth             (int *val = 0);
  void           setMappedWhenManaged (Boolean);
  Boolean        getMappedWhenManaged (Boolean *val = 0);
  Screen *       getScreen            (Screen ** val = 0);
  void           setTranslations      (XtTranslations);
  XtTranslations getTranslations      (XtTranslations *val = 0);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const 
    {
    return __RWCORE;
    }
  
 private:
};

#endif  /* End of file Core.h - Do not add beyond this point! */
