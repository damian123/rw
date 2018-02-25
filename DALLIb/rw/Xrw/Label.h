#ifndef __RW_LABEL_H
#define __RW_LABEL_H
/*********************************************************************
*
*  $Id: Label.h,v 1.12 1994/04/25 23:30:07 steves Exp $
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
*  File        : Label.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Label class.
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

/*********************************************************************
* RWLabel : Class object which represents the XmLabel
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWLabel : public RWPrimitive {
public:
  RWLabel
    (
    RWController *, 
    const RWCString &,
    RWBoolean managed = False
    );
  RWLabel
    (
    RWController *, 
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWLabel
    (
    RWController *, 
    RWResourceObj * ro = 0
    );
  RWLabel();
  RWLabel
    (
    Widget, 
    RWBoolean managed = False
    );   // Internal.

  ~RWLabel();

  void                  createWidget              (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void                  setAccelerator            (const RWCString &);
  RWCString             getAccelerator            (RWCString *val = 0);
  void                  setAcceleratorText        (RWXmString *);
  RWXmString *          getAcceleratorText        (RWXmString **val = 0);
  void                  setAcceleratorText        (const RWCString &);
  RWCString             getAcceleratorTextC       (RWCString *val = 0);
  void                  setAlignment              (unsigned char);
  unsigned char         getAlignment              (unsigned char *val = 0);
  void                  setFontList               (RWFontList *);
  RWFontList *          getFontList               (RWFontList **val = 0);
  void                  setLabelInsensitivePixmap (Pixmap);
  Pixmap                getLabelInsensitivePixmap (Pixmap *val = 0);
  void                  setLabelPixmap            (Pixmap);
  Pixmap                getLabelPixmap            (Pixmap *val = 0);
  void                  setLabelString            (RWXmString *);
  RWXmString *          getLabelString            (RWXmString **val = 0);
  void                  setLabelString            (const RWCString &);
  RWCString             getLabelStringC           (RWCString *val = 0);
  void                  setLabelType              (unsigned char);
  unsigned char         getLabelType              (unsigned char *val = 0);
  void                  setMarginBottom           (Dimension);
  Dimension             getMarginBottom           (Dimension *val = 0);
  void                  setMarginHeight           (Dimension);
  Dimension             getMarginHeight           (Dimension *val = 0);
  void                  setMarginLeft             (Dimension);
  Dimension             getMarginLeft             (Dimension *val = 0);
  void                  setMarginRight            (Dimension);
  Dimension             getMarginRight            (Dimension *val = 0);
  void                  setMarginTop              (Dimension );
  Dimension             getMarginTop              (Dimension  *val = 0);
  void                  setMarginWidth            (Dimension);
  Dimension             getMarginWidth            (Dimension *val = 0);
  void                  setMnemonic               (KeySym);
  KeySym                getMnemonic               (KeySym *val = 0);
  void                  setMnemonicCharSet        (String);
  String                getMnemonicCharSet        (String *val = 0);
  void                  setRecomputeSize          (Boolean);
  Boolean               getRecomputeSize          (Boolean *val = 0);
  void                  setXmStringDirection      (XmStringDirection);
  XmStringDirection     getXmStringDirection      (XmStringDirection *val = 0);
  void                  setStringDirection        (RWXmStringDirection *);
  RWXmStringDirection * getStringDirection        (RWXmStringDirection **val = 0);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const { return __RWLABEL; }
  
 private:
};

#endif  /* End of file Label.h - Do not add beyond this point! */

