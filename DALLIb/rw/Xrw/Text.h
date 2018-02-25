#ifndef __RW_TEXT_H
#define __RW_TEXT_H
/*********************************************************************
*
*  $Id: Text.h,v 1.17 1993/10/22 22:09:09 keffer Exp $
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
*  File        : Text.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Text class.
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

#include <rw/Xrw/ScrolledW.h>
#include <rw/Xrw/Primitive.h>
#include <rw/Xrw/TextVCB.h>

#include <Xm/Text.h>

/*********************************************************************
* RWText : Class object which represents the XmText
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWText : public RWPrimitive {
public:
  RWText();
  RWText
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWText
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWText
    (
    RWController *,
    RWResourceObj * ro = 0
    );
  ~RWText();
  RWText
    (
    Widget,
    RWBoolean managed = False
    );   // Internal

  void             createWidget                 (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void             setActivateCallback		(RWXtCallbackProc, RWXtPointer);
  void             setActivateMethod		(RWApplication *, RWMethodPtr, void *);
  void             setActivateCallback		(XtCallbackList);
  void             setAutoShowCursorPosition	(Boolean);
  Boolean          getAutoShowCursorPosition	(Boolean *val = 0);
  void             setCursorPosition		(XmTextPosition);
  XmTextPosition   getCursorPosition		(XmTextPosition *val = 0);
  void             setEditable			(Boolean);
  Boolean          getEditable			(Boolean *val = 0);
  void             setEditMode			(int);
  int              getEditMode			(int *val = 0);
  void             setFocusCallback		(RWXtCallbackProc, RWXtPointer);
  void             setFocusMethod		(RWApplication *, RWMethodPtr, void *);
  void             setFocusCallback		(XtCallbackList);
  void             setGainPrimaryCallback	(RWXtCallbackProc, RWXtPointer);
  void             setGainPrimaryMethod		(RWApplication *, RWMethodPtr, void *);
  void             setGainPrimaryCallback	(XtCallbackList);
  void             setLosePrimaryCallback	(RWXtCallbackProc, RWXtPointer);
  void             setLosePrimaryMethod		(RWApplication *, RWMethodPtr, void *);
  void             setLosePrimaryCallback	(XtCallbackList);
  void             setLosingFocusCallback	(RWXtCallbackProc, RWXtPointer);
  void             setLosingFocusMethod		(RWApplication *, RWMethodPtr, void *);
  void             setLosingFocusCallback	(XtCallbackList);
  void             setMarginHeight		(Dimension);
  Dimension        getMarginHeight		(Dimension *val = 0);
  void             setMarginWidth		(Dimension);
  Dimension        getMarginWidth		(Dimension *val = 0);
  void             setMaxLength			(int);
  int              getMaxLength			(int *val = 0);
  void             setModifyVerifyCallback	(RWXtCallbackProc, RWXtPointer);
  void             setModifyVerifyMethod	(RWApplication *, RWMethodPtr, void *);
  void             setModifyVerifyCallback	(XtCallbackList);
  void             setMotionVerifyCallback	(RWXtCallbackProc, RWXtPointer);
  void             setMotionVerifyMethod	(RWApplication *, RWMethodPtr, void *);
  void             setMotionVerifyCallback	(XtCallbackList);
  void             setSource			(XmTextSource);
  XmTextSource     getSource			(XmTextSource *val = 0);
  void             setTopCharacter		(XmTextPosition);
  XmTextPosition   getTopCharacter		(XmTextPosition *val = 0);
  void             setValue			(const RWCString &);
  RWCString        getValue			(RWCString *val = 0);
  void             setValueChangedCallback	(RWXtCallbackProc, RWXtPointer);
  void             setValueChangedMethod	(RWApplication *, RWMethodPtr, void *);
  void             setValueChangedCallback	(XtCallbackList);
  void             setVerifyBell		(Boolean);
  Boolean          getVerifyBell		(Boolean *val = 0);
  void             setPendingDelete		(Boolean);
  Boolean          getPendingDelete		(Boolean *val = 0);
  void             setSelectionArray		(XmTextScanType *);
  XmTextScanType * getSelectionArray		(XmTextScanType **val = 0);
  void             setSelectionArrayCount	(int);
  int              getSelectionArrayCount	(int *val = 0);
  void             setSelectThreshold		(int);
  int              getSelectThreshold		(int *val = 0);
  void             setBlinkRate			(int);
  int              getBlinkRate			(int *val = 0);
  void             setColumns			(short);
  short            getColumns			(short *val = 0);
  void             setCursorPositionVisible	(Boolean);
  Boolean          getCursorPositionVisible	(Boolean *val = 0);
  void             setFontList			(RWFontList *);
  RWFontList *     getFontList			(RWFontList **val = 0);
  void             setResizeHeight		(Boolean);
  Boolean          getResizeHeight		(Boolean *val = 0);
  void             setResizeWidth		(Boolean);
  Boolean          getResizeWidth		(Boolean *val = 0);
  void             setRows			(short);
  short            getRows			(short *val = 0);
  void             setWordWrap			(Boolean);
  Boolean          getWordWrap			(Boolean  *val = 0);

#if RWX11Version>=5
  void             setModifyVerifyCallbackWcs	(RWXtCallbackProc, RWXtPointer);
  void             setModifyVerifyMethodWcs	(RWApplication *, RWMethodPtr, void *);
  void             setModifyVerifyCallbackWcs	(XtCallbackList);
  void             setValueWcs			(wchar_t *);
  wchar_t *        getValueWcs			(wchar_t **val = 0);
#endif

// MOTIF MEMBER FUNCTIONS  
  void             clearSelection		(Time);
  Boolean          Copy				(Time);
  Boolean          cut				(Time);
  int              getBaseline			();
  Boolean          textGetEditable		();
  XmTextPosition   getInsertionPosition		();
  XmTextPosition   getLastPosition		();
  int              textGetMaxLength		();
  RWCString        getSelection			();
  Boolean          getSelectionPosition		(XmTextPosition*, XmTextPosition*);
  XmTextSource     textGetSource		();
  RWCString        getString			();
  XmTextPosition   textGetTopCharacter		();
  void             insert			(XmTextPosition, const RWCString &);
  Boolean          paste			();
  Boolean          posToXY			(XmTextPosition, Position *, Position *);
  Boolean          remove			();
  void             replace			(XmTextPosition, XmTextPosition, const RWCString &);
  void             scroll			(int);
  void             setAddMode			(Boolean);
  void             textSetEditable		(Boolean);
  void             setHighlight			(XmTextPosition, XmTextPosition, XmHighlightMode);
  void             setInsertionPosition		(XmTextPosition);
  void             textSetMaxLength		(int);
  void             setSelection			(XmTextPosition, XmTextPosition, Time);
  void             setSource			(XmTextSource, XmTextPosition, XmTextPosition);
  void             setString			(const RWCString &);
  void             textSetTopCharacter		(XmTextPosition);
  void             showPosition			(XmTextPosition);
  XmTextPosition   xyToPos			(Position, Position);

#if RWX11Version>=5
  void             disableRedisplay             ();
  void             enableRedisplay              ();
  Boolean          findString                   (XmTextPosition, const RWCString &,
						 XmTextDirection, XmTextPosition *);
  int              getSubString                 (XmTextPosition, int, int, RWCString &);
  Boolean          findStringWcs                (XmTextPosition, wchar_t *,
						 XmTextDirection, XmTextPosition *);
  wchar_t *        getSelectionWcs              ();
  wchar_t *        getStringWcs                 ();
  int              getSubStringWcs              (XmTextPosition, int, int, wchar_t *);
  void             insertWcs                    (XmTextPosition, wchar_t *);
  void             replaceWcs                   (XmTextPosition, XmTextPosition, wchar_t *);
  void             setStringWcs                 (wchar_t *);
#endif

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWTEXT;}

 private:
  RWCallback *   getCallbackObject (RWXtPointer);
};

#endif


#ifndef __RW_SCROLLED_TEXT_H
#define __RW_SCROLLED_TEXT_H
/*********************************************************************
* RWScrolledText : Class object which represents the XmScrolledText
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWScrolledText : public RWText
{
 public:
  RWScrolledText
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWScrolledText
    (
    RWController *,
    const RWCString &,
    RWResourceObj &,
    RWBoolean managed = False
    );
  RWScrolledText
    (
    RWController *,
    RWResourceObj * ro = 0
    );
  ~RWScrolledText();

  void           createWidget           (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  Boolean        getScrollHorizontal	(Boolean *val = 0);
  Boolean        getScrollLeftSide	(Boolean *val = 0);
  Boolean        getScrollTopSide	(Boolean *val = 0);
  Boolean        getScrollVertical	(Boolean *val = 0);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWSCROLLEDTEXT;}
};

#endif  /* End of file Text.h - Do not add beyond this point! */
