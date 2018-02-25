#ifndef __RW_SELECTION_BOX_H
#define __RW_SELECTION_BOX_H
/*********************************************************************
*
*  $Id: SelectioB.h,v 1.14 1993/10/22 22:05:14 keffer Exp $
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
*  File        : SelectioB.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Selection Box, Prompt Dialog, and Selection Dialog
*                classes.
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
#include <rw/Xrw/PushB.h>
#include <rw/Xrw/List.h>
#include <rw/Xrw/Text.h>
#include <rw/Xrw/Separator.h>
#include <rw/Xrw/SelBCB.h>

/*********************************************************************
* RWSelectionBox : Class object which represents the XmSelectionBox
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWSelectionBox : public RWBulletinBoard
{
 public:
  RWSelectionBox
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWSelectionBox
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWSelectionBox
    (
    RWController *,
    RWResourceObj * ro = 0
    );
  RWSelectionBox();
  RWSelectionBox
    (
    Widget,
    RWBoolean managed = False
    );  // Internal.
  ~RWSelectionBox();

  void                createWidget              (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void                setApplyCallback		(RWXtCallbackProc, RWXtPointer);
  void                setApplyMethod		(RWApplication *, RWMethodPtr, void *);
  void                setApplyCallback		(XtCallbackList);
  void                setApplyLabelString	(RWXmString *);
  RWXmString *        getApplyLabelString	(RWXmString **val = 0);
  void                setApplyLabelString	(const RWCString &);
  RWCString           getApplyLabelStringC    	(RWCString *val = 0);
  void                setCancelCallback		(RWXtCallbackProc, RWXtPointer);
  void                setCancelMethod		(RWApplication *, RWMethodPtr, void *);
  void                setCancelCallback		(XtCallbackList);
  void                setCancelLabelString	(RWXmString *);
  RWXmString *        getCancelLabelString	(RWXmString **val = 0);
  void                setCancelLabelString	(const RWCString &);
  RWCString           getCancelLabelStringC	(RWCString *val = 0);
  unsigned char       getDialogType		(unsigned char *val = 0);
  void                setHelpLabelString	(RWXmString *);
  RWXmString *        getHelpLabelString	(RWXmString **val = 0);
  void                setHelpLabelString	(const RWCString &);
  RWCString           getHelpLabelStringC	(RWCString *val = 0);
  void                setListItemCount		(int);
  int                 getListItemCount		(int *val = 0);
  void                setListItems		(RWXmStringList);
  void                setListItems		(RWXmStringList, int);
  RWXmStringList      getListItems		(RWXmStringList *val = 0);
  void                setListItems		(RWCStringOrderedVec &);
  void                setListItemsC		(String *, int);
  void                getListItemsC		(RWCStringOrderedVec &);
  RWCStringOrderedVec getListItemsC		();
  void                setListLabelString	(RWXmString *);
  RWXmString *        getListLabelString	(RWXmString **val = 0);
  void                setListLabelString	(const RWCString &);
  RWCString           getListLabelStringC	(RWCString *val = 0);
  void                setListVisibleItemCount	(int);
  int                 getListVisibleItemCount	(int *val = 0);
  void                setMinimizeButtons	(Boolean);
  Boolean             getMinimizeButtons	(Boolean *val = 0);
  void                setMustMatch		(Boolean);
  Boolean             getMustMatch		(Boolean *val = 0);
  void                setNoMatchCallback 	(RWXtCallbackProc, RWXtPointer);
  void                setNoMatchMethod 		(RWApplication *, RWMethodPtr, void *);
  void                setNoMatchCallback 	(XtCallbackList);
  void                setOkCallback 		(RWXtCallbackProc, RWXtPointer);
  void                setOkMethod 		(RWApplication *, RWMethodPtr, void *);
  void                setOkCallback 		(XtCallbackList);
  void                setOkLabelString		(RWXmString *);
  RWXmString *        getOkLabelString		(RWXmString **val = 0);
  void                setOkLabelString		(const RWCString &);
  RWCString           getOkLabelStringC		(RWCString *val = 0);
  void                setSelectionLabelString	(RWXmString *);
  RWXmString *        getSelectionLabelString	(RWXmString **val = 0);
  void                setSelectionLabelString	(const RWCString &);
  RWCString           getSelectionLabelStringC	(RWCString *val = 0);
  void                setTextColumns		(short);
  short               getTextColumns		(short *val = 0);
  void                setTextString		(RWXmString *);
  RWXmString *        getTextString		(RWXmString **val = 0);
  void                setTextString		(const RWCString &);
  RWCString           getTextStringC		(RWCString *val = 0);

#if RWX11Version>=5
  void           setChildPlacement		(unsigned char);
  unsigned char  getChildPlacement		(unsigned char *val = 0);
#endif

// MOTIF MEMBER FUNCTIONS  
  RWController * getChild 			(unsigned char);

  RWPushButton * getApplyButton ();
  RWPushButton * getCancelButton ();
  RWPushButton * getDefaultButton ();
  RWPushButton * getHelpButton ();
  RWPushButton * getOkButton ();
  RWList *       getList ();
  RWLabel *      getListLabel ();
  RWLabel *      getSelectionLabel ();
  RWSeparator *  getSeparator ();
  RWText *       getText ();
  RWController * getWorkArea ();

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWSELECTIONBOX;}
  
 private:
  RWCallback * getCallbackObject (RWXtPointer);
};

#endif


#ifndef __RW_PROMPT_DIALOG_H
#define __RW_PROMPT_DIALOG_H
/*********************************************************************
* RWPromptDialog : Class object which represents the XmPromptDialog
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWPromptDialog : public RWSelectionBox
{
 public:
  RWPromptDialog
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWPromptDialog
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWPromptDialog
    (
    RWController *,
    RWResourceObj * ro = 0
    );
  ~RWPromptDialog();

  void  createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWPROMPTDIALOG;}
};

#endif


#ifndef __RW_SELECTION_DIALOG_H
#define __RW_SELECTION_DIALOG_H
/*********************************************************************
* RWSelectionDialog : Class object which represents the XmSelectionDialog
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWSelectionDialog : public RWSelectionBox
{
 public:
  RWSelectionDialog
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWSelectionDialog
    (
    RWController *, 
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWSelectionDialog
    (
    RWController *, 
    RWResourceObj * ro = 0
    );
  ~RWSelectionDialog();

  void  createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWSELECTIONDIALOG;}
};

#endif  /* End of file SelectioB.h - Do not add beyond this point! */
