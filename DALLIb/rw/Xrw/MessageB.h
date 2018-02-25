#ifndef __RW_MESSAGE_BOX_H
#define __RW_MESSAGE_BOX_H
/*********************************************************************
*
*  $Id: MessageB.h,v 1.13 1993/10/22 22:05:14 keffer Exp $
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
*  File        : MessageB.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Message Box, Message Dialog, Information Dialog,
*                Question Dialog, Warning Dialog, Error Dialog,
*                and Working Dialog classes.
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
* RWMessageBox : Class object which represents the XmMessageBox
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWMessageBox : public RWBulletinBoard {
public:
  RWMessageBox
    (
    RWController *, 
    const RWCString &,
    RWBoolean managed = False
    );
  RWMessageBox
    (
    RWController *, 
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWMessageBox
    (
    RWController *, 
    RWResourceObj * ro = 0
    );
  RWMessageBox();
  RWMessageBox
    (
    Widget, 
    RWBoolean managed = False
    );  // Internal.

  ~RWMessageBox();

  void          createWidget          (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void          setCancelCallback     (RWXtCallbackProc, RWXtPointer);
  void          setCancelMethod       (RWApplication *, RWMethodPtr, void *);
  void          setCancelCallback     (XtCallbackList);
  void          setCancelLabelString  (RWXmString *);
  RWXmString *  getCancelLabelString  (RWXmString **val = 0);
  void          setCancelLabelString  (const RWCString &);
  RWCString     getCancelLabelStringC (RWCString *val = 0);
  void          setDefaultButtonType  (unsigned char);
  unsigned char getDefaultButtonType  (unsigned char *val = 0);
  void          setDialogType         (unsigned char);
  unsigned char getDialogType         (unsigned char *val = 0);
  void          setHelpLabelString    (RWXmString *);
  RWXmString *  getHelpLabelString    (RWXmString **val = 0);
  void          setHelpLabelString    (const RWCString &);
  RWCString     getHelpLabelStringC   (RWCString *val = 0);
  void          setMessageAlignment   (unsigned char);
  unsigned char getMessageAlignment   (unsigned char *val = 0);
  void          setMessageString      (RWXmString *);
  RWXmString *  getMessageString      (RWXmString **val = 0);
  void          setMessageString      (const RWCString &);
  RWCString     getMessageStringC     (RWCString *val = 0);
  void          setMinimizeButtons    (Boolean);
  Boolean       getMinimizeButtons    (Boolean *val = 0);
  void          setOkCallback         (RWXtCallbackProc, RWXtPointer);
  void          setOkMethod           (RWApplication *, RWMethodPtr, void *);
  void          setOkCallback         (XtCallbackList);
  void          setOkLabelString      (RWXmString *);
  RWXmString *  getOkLabelString      (RWXmString **val = 0);
  void          setOkLabelString      (const RWCString &);
  RWCString     getOkLabelStringC     (RWCString *val = 0);
  void          setSymbolPixmap       (Pixmap);
  Pixmap        getSymbolPixmap       (Pixmap *val = 0);

// MOTIF MEMBER FUNCTIONS  
  RWController * getChild (unsigned char);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWMESSAGEBOX;}
  
 protected:
};

#endif


#ifndef __RW_MESSAGE_DIALOG_H
#define __RW_MESSAGE_DIALOG_H
/*********************************************************************
* RWMessageDialog : Class object which represents the XmMessageDialog
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWMessageDialog : public RWMessageBox {
public:
  RWMessageDialog
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWMessageDialog
    (
    RWController *,
    const RWCString &, 
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWMessageDialog
    (
    RWController *,
    RWResourceObj * ro = 0
    );

  ~RWMessageDialog();

  void  createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWMESSAGEDIALOG;}
};

#endif


#ifndef __RW_INFORMATION_DIALOG_H
#define __RW_INFORMATION_DIALOG_H
/*********************************************************************
* RWInformationDialog : Class object which represents the XmInformationDialog
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWInformationDialog : public RWMessageBox {
public:
  RWInformationDialog
    (
    RWController *, 
    const RWCString &, 
    RWBoolean managed = False
    );
  RWInformationDialog
    (
    RWController *, 
    const RWCString &, 
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWInformationDialog
    (
    RWController *, 
    RWResourceObj * ro = 0
    );

  ~RWInformationDialog();

  void  createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const { return __RWINFORMATIONDIALOG; }
};

#endif


#ifndef __RW_QUESTION_DIALOG_H
#define __RW_QUESTION_DIALOG_H
/*********************************************************************
* RWQuestionDialog : Class object which represents the XmQuestionDialog
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWQuestionDialog : public RWMessageBox {
public:
  RWQuestionDialog
    (
    RWController *,
    const RWCString &, 
    RWBoolean managed = False
    );
  RWQuestionDialog
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWQuestionDialog
    (
    RWController *,
    RWResourceObj * ro = 0
    );

  ~RWQuestionDialog();

  void  createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWQUESTIONDIALOG;}
};

#endif


#ifndef __RW_WARNING_DIALOG_H
#define __RW_WARNING_DIALOG_H
/*********************************************************************
* RWWarningDialog : Class object which represents the XmWarningDialog
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWWarningDialog : public RWMessageBox {
public:
  RWWarningDialog
    (
    RWController *, 
    const RWCString &,
    RWBoolean managed = False
    );
  RWWarningDialog
    (
    RWController *, 
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWWarningDialog
    (
    RWController *, 
    RWResourceObj * ro = 0
    );

  ~RWWarningDialog();

  void  createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWWARNINGDIALOG;}
};

#endif


#ifndef __RW_ERROR_DIALOG_H
#define __RW_ERROR_DIALOG_H
/*********************************************************************
* RWErrorDialog : Class object which represents the XmErrorDialog
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWErrorDialog : public RWMessageBox {
public:
  RWErrorDialog
    (
    RWController *, 
    const RWCString &, 
    RWBoolean managed = False
    );
  RWErrorDialog
    (
    RWController *, 
    const RWCString &, 
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWErrorDialog
    (
    RWController *, 
    RWResourceObj * ro = 0
    );

  ~RWErrorDialog();

  void  createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const { return __RWERRORDIALOG; }
};

#endif


#ifndef __RW_WORKING_DIALOG_H
#define __RW_WORKING_DIALOG_H
/*********************************************************************
* RWWorkingDialog : Class object which represents the XmWorkingDialog
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWWorkingDialog : public RWMessageBox {
public:
  RWWorkingDialog
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWWorkingDialog
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWWorkingDialog
    (
    RWController *,
    RWResourceObj * ro = 0
    );

  ~RWWorkingDialog();

  void  createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWWORKINGDIALOG;}
};
#endif

#if RWX11Version>=5
# ifndef __RW_TEMPLATE_DIALOG_H
# define __RW_TEMPLATE_DIALOG_H
/*********************************************************************
* RWTemplateDialog : Class object which represents the XmTemplateDialog
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWTemplateDialog : public RWMessageBox {
public:
  RWTemplateDialog
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWTemplateDialog
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWTemplateDialog
    (
    RWController *,
    RWResourceObj * ro = 0
    );

  ~RWTemplateDialog();

  void  createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWTEMPLATEDIALOG;}
};

# endif
#endif  /* End of file MessageB.h - Do not add beyond this point! */
