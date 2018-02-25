#ifndef __RW_ROWCOLUMN_H
#define __RW_ROWCOLUMN_H
/*********************************************************************
*
*  $Id: RowColumn.h,v 1.17 1993/10/22 22:05:14 keffer Exp $
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
*  File        : RowColumn.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Row Column class.
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
#include <rw/Xrw/CascadeBG.h>
#include <rw/Xrw/LabelG.h>
#include <rw/Xrw/RowColCB.h>
	 
/*********************************************************************
* RWRowColumn : Class object which represents the XmRowColumn
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWRowColumn : public RWManager {
public:
  RWRowColumn
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWRowColumn
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWRowColumn
    (
    RWController *,
    RWResourceObj * ro = 0
    );
  RWRowColumn();
  RWRowColumn
    (
    Widget,
    RWBoolean managed = False
    );  // Internal.

  ~RWRowColumn();

  void           createWidget           (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void           setAdjustLast		(Boolean);
  Boolean        getAdjustLast		(Boolean *val = 0);
  void           setAdjustMargin	(Boolean);
  Boolean        getAdjustMargin	(Boolean *val = 0);
  void           setEntryAlignment 	(unsigned char);
  unsigned char  getEntryAlignment	(unsigned char *val = 0);
  void           setEntryBorder		(Dimension);
  Dimension      getEntryBorder		(Dimension *val = 0);
  void           setEntryCallback 	(RWXtCallbackProc, RWXtPointer);
  void           setEntryMethod 	(RWApplication *, RWMethodPtr, void *);
  void           setEntryCallback 	(XtCallbackList);
  void           setEntryClass		(WidgetClass);
  WidgetClass    getEntryClass		(WidgetClass *val = 0);
  void           setIsAligned		(Boolean);
  Boolean        getIsAligned		(Boolean *val = 0);
  void           setIsHomogeneous	(Boolean);
  Boolean        getIsHomogeneous	(Boolean *val = 0);
  void           setMapCallback 	(RWXtCallbackProc, RWXtPointer);
  void           setMapMethod 		(RWApplication *, RWMethodPtr, void *);
  void           setMapCallback 	(XtCallbackList);
  void           setMarginHeight	(Dimension);
  Dimension      getMarginHeight	(Dimension *val = 0);
  void           setMarginWidth		(Dimension);
  Dimension      getMarginWidth		(Dimension *val = 0);
  void           setMenuAccelerator	(String);
  String         getMenuAccelerator	(String *val = 0);
  void           setMenuHelpWidget	(RWController *);
  RWController * getMenuHelpWidget	(RWController **val = 0);
  void           setMenuHelpObject	(RWController *);
  RWController * getMenuHelpObject	(RWController **val = 0);
  void           setMenuHistory		(RWController *);
  RWController * getMenuHistory		(RWController **val = 0);
  void           setMenuPost		(String);
  String         getMenuPost		(String *val = 0);
  void           setNumColumns		(short);
  short          getNumColumns		(short *val = 0);
  void           setOrientation		(unsigned char);
  unsigned char  getOrientation		(unsigned char *val = 0);
  void           setPacking		(unsigned char);
  unsigned char  getPacking		(unsigned char *val = 0);
  void           setPopupEnabled	(Boolean);
  Boolean        getPopupEnabled	(Boolean *val = 0);
  void           setRadioAlwaysOne	(Boolean);
  Boolean        getRadioAlwaysOne	(Boolean *val = 0);
  void           setRadioBehavior	(Boolean);
  Boolean        getRadioBehavior	(Boolean *val = 0);
  void           setResizeHeight	(Boolean);
  Boolean        getResizeHeight	(Boolean *val = 0);
  void           setResizeWidth		(Boolean);
  Boolean        getResizeWidth		(Boolean *val = 0);
  unsigned char  getRowColumnType	(unsigned char *val = 0);
  void           setSpacing		(Dimension);
  Dimension      getSpacing		(Dimension *val = 0);
  void           setUnmapCallback 	(RWXtCallbackProc, RWXtPointer);
  void           setUnmapMethod 	(RWApplication *, RWMethodPtr, void *);
  void           setUnmapCallback 	(XtCallbackList);
  void           setWhichButton		(unsigned int);
  unsigned int   getWhichButton		(unsigned int *val = 0);

#if RWX11Version>=5
  void           setEntryVerticalAlignment (unsigned char);
  unsigned char  getEntryVerticalAlignment (unsigned char *val = 0);
  void           setTearOffModel (unsigned char);
  unsigned char  getTearOffModel (unsigned char * val = 0);
  void           setTearOffMenuActivateCallback (RWXtCallbackProc, RWXtPointer);
  void           setTearOffMenuActivateMethod (RWApplication *, RWMethodPtr, void *);
  void           setTearOffMenuActivateCallback (XtCallbackList);
  void           setTearOffMenuDeactivateCallback (RWXtCallbackProc, RWXtPointer);
  void           setTearOffMenuDeactivateMethod (RWApplication *, RWMethodPtr, void *);
  void           setTearOffMenuDeactivateCallback (XtCallbackList);
#endif
// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWROWCOLUMN;}
  
private:
  RWCallback * getCallbackObject (RWXtPointer);
};

#endif


#ifndef __RW_MENUBAR_H
#define __RW_MENUBAR_H
/*********************************************************************
* RWMenuBar : Class object which represents the XmMenuBar
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWCascadeButton;

class RWMenuBar : public RWRowColumn {
public:
  RWMenuBar
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWMenuBar
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWMenuBar
    (
    RWController *,
    RWResourceObj * ro = 0
    );

  ~RWMenuBar();

  void              createWidget (char *, RWBoolean managed = False);
  RWCascadeButton * addItem      (const RWCString &, char, const char * res = 0);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWMENUBAR;}  
};

#endif


#ifndef __RW_OPTION_MENU_H
#define __RW_OPTION_MENU_H
/*********************************************************************
* RWOptionMenu : Class object which represents the XmOptionMenu
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWOptionMenu : public RWRowColumn {
public:
  RWOptionMenu
    (
    RWController *, 
    const RWCString &,
    RWBoolean managed = False
    );
  RWOptionMenu
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWOptionMenu
    (
    RWController *,
    RWResourceObj * ro = 0
    );

  ~RWOptionMenu();

  void            createWidget    (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void            setLabelString  (RWXmString *);
  RWXmString *    getLabelString  (RWXmString **val = 0);
  void            setLabelString  (const RWCString &);
  RWCString       getLabelStringC (RWCString *val = 0);
  void            setMnemonic     (KeySym);
  KeySym          getMnemonic     (KeySym *val = 0);
  void            setSubMenuId    (RWController *);
  RWController *  getSubMenuId    (RWController **val = 0);
  
// MOTIF MEMBER FUNCTIONS  
  RWCascadeButtonGadget * optionButtonGadget ();
  RWLabelGadget *         optionLabelGadget  ();

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWOPTIONMENU;}  
  
 private:
};

#endif


#ifndef __RW_POPUP_MENU_H
#define __RW_POPUP_MENU_H
/*********************************************************************
* RWPopupMenu : Class object which represents the XmPopupMenu
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWPopupMenu : public RWRowColumn {
public:
  RWPopupMenu
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWPopupMenu
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWPopupMenu
    (
    RWController *,
    RWResourceObj * ro = 0
    );

  ~RWPopupMenu();

  void           createWidget        (char *, RWBoolean managed = False);

// MOTIF MEMBER FUNCTIONS  
  RWController * getPostedFromObject ();
  void           menuPosition        (XButtonPressedEvent *);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWPOPUPMENU;}  
};

#endif


#ifndef __RW_PULLDOWN_MENU_H
#define __RW_PULLDOWN_MENU_H
/*********************************************************************
* RWPulldownMenu : Class object which represents the XmPulldownMenu
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWPushButton;
class RWToggleButton;
class RWCascadeButton;
class RWSeparator;
class RWLabel;

class RWPulldownMenu : public RWRowColumn {
public:
  RWPulldownMenu
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWPulldownMenu
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWPulldownMenu
    (
    RWController *,
    RWResourceObj * ro = 0
    );

  ~RWPulldownMenu();

// MOTIF MEMBER FUNCTIONS  
  RWController * getPostedFromObject ();

// RW TOOLS MEMBER FUNCTIONS
  RWPushButton *    addPush (const RWCString &, 
			     char mn = 0, 
			     RWXtCallbackProc cbf = 0, 
			     RWXtPointer cd = 0);
  RWPushButton *    addPushM (const RWCString &, 
			      char mn = 0,
			      RWApplication * obj = 0,
			      RWMethodPtr cbf = 0, 
			      RWXtPointer cd = 0);
  RWToggleButton *  addToggle (const RWCString &,
			       char mn = 0,
			       RWXtCallbackProc cbf = 0,
			       RWXtPointer cd = 0);
  RWToggleButton *  addToggleM (const RWCString &,
				char mn = 0,
				RWApplication * obj = 0,
				RWMethodPtr cbf = 0,
				RWXtPointer cd = 0);
  RWCascadeButton * addCascade (const RWCString &, char mn = 0);
  RWSeparator *     addSeparator (unsigned char sep_type = XmSINGLE_LINE,
				  const char * res_name = 0);
  RWLabel *         addTitle (const RWCString &);

  void              setRadioButton (RWToggleButton *);
  void              createWidget   (char *, RWBoolean managed = False);

  virtual RWClassID isA () const {return __RWPULLDOWNMENU;}    
};

#endif


#ifndef __RW_RADIO_BOX_H
#define __RW_RADIO_BOX_H
/*********************************************************************
* RWRadioBox : Class object which represents the XmRadioBox
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWRadioBox : public RWRowColumn {
public:
  RWRadioBox
    (
    RWController *,
    const RWCString &,
    RWBoolean managed = False
    );
  RWRadioBox
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWRadioBox
    (
    RWController *,
    RWResourceObj * ro = 0
    );

  ~RWRadioBox();

  void  createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWRADIOBOX;}    
};

#endif  /* End of file RowColumn.h - Do not add beyond this point! */
