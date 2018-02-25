#ifndef __RW_FORM_H
#define __RW_FORM_H
/*********************************************************************
*
*  $Id: Form.h,v 1.9 1993/10/07 01:42:57 keffer Exp $
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
*  File        : Form.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Form and Form Dialog classes.
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

#include <rw/Xrw/BulletinB.h>

/*********************************************************************
* RWForm : Class object which represents the XmForm
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWForm : public RWBulletinBoard {
public:
  RWForm
    (
    RWController *, 
    const RWCString &,
    RWBoolean managed = False
    );
  RWForm
    (
    RWController *, 
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWForm
    (
    RWController *, 
    RWResourceObj * ro = 0
    );
  RWForm();
  RWForm
    (
    Widget, 
    RWBoolean managed = False
    );  // Internal.
  ~RWForm();

  void      createWidget         (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void      setFractionBase      (int);
  int       getFractionBase      (int *val = 0);
  void      setHorizontalSpacing (Dimension);
  Dimension getHorizontalSpacing (Dimension *val = 0);
  void      setRubberPositioning (Boolean);
  Boolean   getRubberPositioning (Boolean *val = 0);
  void      setVerticalSpacing   (Dimension);
  Dimension getVerticalSpacing   (Dimension *val = 0);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const { return __RWFORM; }
};

#endif


#ifndef __RW_FORM_DIALOG_H
#define __RW_FORM_DIALOG_H
/*********************************************************************
* RWFormDialog : Class object which represents the XmFormDialog
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWFormDialog : public RWForm {
public:
  RWFormDialog
    (
    RWController *, 
    const RWCString &,
    Boolean managed = False
    );
  RWFormDialog
    (
    RWController *, 
    const RWCString &,
    const RWResourceObj &,
    Boolean managed = False
    );
  RWFormDialog
    (
    RWController *, 
    RWResourceObj * ro = 0
    );

  ~RWFormDialog();

  void  createWidget (char *, RWBoolean managed = False);
  
// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const { return __RWFORMDIALOG; }
};

#endif  /* End of file Form.h - Do not add beyond this point! */


