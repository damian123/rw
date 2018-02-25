#ifndef __RW_LIST_H
#define __RW_LIST_H
/*********************************************************************
*
*  $Id: List.h,v 1.17 1993/10/22 22:05:14 keffer Exp $
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
*  File        : List.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Scrolled List and List classes.
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
#include <rw/Xrw/ListCB.h>

/*********************************************************************
* RWList : Class object which represents the XmList
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWList : public RWPrimitive {
public:
  RWList();
  RWList
    (
    RWController *, 
    const RWCString &,
    RWBoolean managed = False
    );
  RWList
    (
    RWController *, 
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
   RWList
     (
     RWController *, 
     RWResourceObj * ro = 0
     );
   RWList
     (
     Widget, 
     RWBoolean managed = False
     );      // Internal.

   ~RWList();

  void                  createWidget                 (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void                  setAutomaticSelection        (Boolean);
  Boolean               getAutomaticSelection        (Boolean *val = 0);
  void                  setDoubleClickInterval       (int);
  int                   getDoubleClickInterval       (int *val = 0);
  void                  setFontList                  (RWFontList *);
  RWFontList *          getFontList                  (RWFontList **val = 0);
  void                  setItemCount                 (int);
  int                   getItemCount                 (int *val = 0);
  void                  setItems                     (String);
  void                  setItems                     (RWXmStringList);
  void                  setItems                     (RWXmStringList, int);
  RWXmStringList        getItems                     (RWXmStringList * val = 0);
  void                  setItems                     (RWCStringOrderedVec &);
  void                  setItemsC                    (char **, int);
  void                  getItemsC                    (RWCStringOrderedVec &);
  RWCStringOrderedVec   getItemsC                    ();
  void                  setListMarginHeight          (Dimension);
  Dimension             getListMarginHeight          (Dimension *val = 0);
  void                  setListMarginWidth           (Dimension);
  Dimension             getListMarginWidth           (Dimension *val = 0);
  unsigned char         getListSizePolicy            (unsigned char *val = 0);
  void                  setListSpacing               (Dimension);
  Dimension             getListSpacing               (Dimension *val = 0);
  void                  setSelectedItemCount         (int);
  int                   getSelectedItemCount         (int *val = 0);
  void                  setSelectedItems             (RWXmStringList);
  void                  setSelectedItems             (RWXmStringList, int);
  void                  setSelectedItems             (RWCStringOrderedVec &);
  void                  setSelectedItemsC            (char **, int);
  void                  getSelectedItemsC            (RWCStringOrderedVec &);
  RWCStringOrderedVec   getSelectedItemsC            ();
  RWXmStringList        getSelectedItems             (RWXmStringList * val = 0);
  void                  setSelectionPolicy           (unsigned char);
  unsigned char         getSelectionPolicy           (unsigned char *val = 0);
  void                  setXmStringDirection         (XmStringDirection);
  XmStringDirection     getXmStringDirection         (XmStringDirection *val = 0);
  void                  setStringDirection           (RWXmStringDirection *);
  RWXmStringDirection * getStringDirection           (RWXmStringDirection **val = 0);
  void                  setTopItemPosition           (int);
  int                   getTopItemPosition           (int *val = 0);
  void                  setVisibleItemCount          (int);
  int                   getVisibleItemCount          (int *val = 0);

// CALLBACKS
  void                  setBrowseSelectionCallback   (RWXtCallbackProc, RWXtPointer);
  void                  setBrowseSelectionMethod     (RWApplication *, RWMethodPtr, void *);
  void                  setBrowseSelectionCallback   (XtCallbackList);
  void                  setDefaultActionCallback     (RWXtCallbackProc, RWXtPointer);
  void                  setDefaultActionMethod       (RWApplication *, RWMethodPtr, void *);
  void                  setDefaultActionCallback     (XtCallbackList);
  void                  setExtendedSelectionCallback (RWXtCallbackProc, RWXtPointer);
  void                  setExtendedSelectionMethod   (RWApplication *, RWMethodPtr, void *);
  void                  setExtendedSelectionCallback (XtCallbackList);
  void                  setMultipleSelectionCallback (RWXtCallbackProc, RWXtPointer);
  void                  setMultipleSelectionMethod   (RWApplication *, RWMethodPtr, void *);
  void                  setMultipleSelectionCallback (XtCallbackList);
  void                  setSingleSelectionCallback   (RWXtCallbackProc, RWXtPointer);
  void                  setSingleSelectionMethod     (RWApplication *, RWMethodPtr, void *);
  void                  setSingleSelectionCallback   (XtCallbackList);

// MOTIF MEMBER FUNCTIONS  
  void    addItem            (RWXmString *, int);
  void    addItem            (const RWCString &, int);
  void    addItems           (RWXmString *, int, int);
  void    addItems           (const RWCStringOrderedVec &, int);
  void    addItemsC          (char **, int, int);
  void    addItemUnselected  (RWXmString *, int);
  void    addItemUnselected  (const RWCString &, int);
  void    deleteAllItems     ();
  void    deleteItem         (RWXmString *);
  void    deleteItem         (const RWCString &);
  void    deleteItems        (RWXmString *, int);
  void    deleteItems        (const RWCStringOrderedVec &);
  void    deleteItemsC       (char **, int);
  void    deleteItemsPos     (int, int);
  void    deletePos          (int);
  void    deselectAllItems   ();
  void    deselectItem       (RWXmString *);
  void    deselectItem       (const RWCString &);
  void    deselectItemC      (char **);
  void    deselectPos        (int);
  Boolean getMatchPos        (RWXmString *, int **, int *);
  Boolean getMatchPos        (const RWCString &, int **, int *);
  Boolean getSelectedPos     (int **, int *);
  Boolean itemExists         (RWXmString *);
  Boolean itemExists         (const RWCString &);
  int     itemPos            (RWXmString *);
  int     itemPos            (const RWCString &);
  void    replaceItems       (RWXmString *, int, RWXmString *);
  void    replaceItems       (const RWCStringOrderedVec &, const RWCStringOrderedVec &);
  void    replaceItemsC      (char **, int, char **);
  void    replaceItemsPos    (RWXmString *, int, int);
  void    replaceItemsPos    (const RWCStringOrderedVec &, int);
  void    replaceItemsPosC   (char **, int, int);
  void    selectItem         (RWXmString *, Boolean);
  void    selectItem         (const RWCString &, Boolean);
  void    selectPos          (int, Boolean);
  void    setAddMode         (Boolean);
  void    setBottomItem      (RWXmString *);
  void    setBottomItem      (const RWCString &);
  void    setBottomPos       (int);
  void    setHorizPos        (int);
  void    setItem            (RWXmString *);
  void    setItem            (const RWCString &);
  void    setPos             (int);

#if RWX11Version>=5
  void    addItemsUnselected        (RWXmString *, int, int);
  void    addItemsUnselected        (const RWCStringOrderedVec &, int);
  void    addItemsUnselectedC       (char **, int, int);
  void    deletePositions           (int *, int);
  int     getKbdItemPos             ();
  Boolean posToBounds               (int, Position *, Position *, Dimension *, Dimension *);
  Boolean posSelected               (int);
  void    replaceItemsPosUnselected (RWXmString *, int, int);
  void    replaceItemsPosUnselected (const RWCStringOrderedVec &, int);
  void    replaceItemsPosUnselectedC(char **, int, int);
  void    replaceItemsUnselected    (RWXmString *, int, RWXmString *);
  void    replaceItemsUnselected    (const RWCStringOrderedVec &, const RWCStringOrderedVec &);
  void    replaceItemsUnselectedC   (char **, int, char **);
  void    replacePositions          (int *, RWXmString *, int);
  void    replacePositions          (int *, const RWCStringOrderedVec &);
  void    replacePositionsC         (int *, char **, int);
  Boolean setKbdItemPos             (int);
  void    updateSelectedList        ();
  int     yToPos                    (Position);
#endif

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWLIST;}
  
 private:
  RWCallback *getCallbackObject (RWXtPointer);
};

#endif


#ifndef __RW_SCROLLED_LIST_H
#define __RW_SCROLLED_LIST_H
/*********************************************************************
* RWScrolledList : Class object which represents the XmScrolledList
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWScrolledList : public RWList {
public:
  RWScrolledList
    (
    RWController *, 
    const RWCString &,
    RWBoolean managed = False
    );
  RWScrolledList
    (
    RWController *, 
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWScrolledList
    (
    RWController *, 
    RWResourceObj * ro = 0
    );
  ~RWScrolledList();

  void           createWidget                  (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void           setHorizontalScrollBar        (RWController *);
  RWController * getHorizontalScrollBar        (RWController **val = 0);
  void           setScrollBarDisplayPolicy     (unsigned char);
  unsigned char  getScrollBarDisplayPolicy     (unsigned char *val = 0);
  void           setScrollBarPlacement         (unsigned char);
  unsigned char  getScrollBarPlacement         (unsigned char *val = 0);
  void           setScrolledWindowMarginHeight (Dimension);
  Dimension      getScrolledWindowMarginHeight (Dimension *val = 0);
  void           setScrolledWindowMarginWidth  (Dimension);
  Dimension      getScrolledWindowMarginWidth  (Dimension *val = 0);
  void           setSpacing                    (Dimension);
  Dimension      getSpacing                    (Dimension *val = 0);
  void           setVerticalScrollBar          (RWController *);
  RWController * getVerticalScrollBar          (RWController **val = 0);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWSCROLLEDLIST;}
};

#endif  /* End of file List.h - Do not add beyond this point! */




