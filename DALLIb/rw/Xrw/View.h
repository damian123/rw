#ifndef __RW_VIEWH
#define __RW_VIEWH

/*********************************************************************
*
*  $Id: View.h,v 1.23 1994/01/11 00:23:12 cynthia Exp $
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
*  File        : View.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                abstract base class that represents the View component
*                of a Model-View-Controller architecture
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#ifndef __RWVIEWDEFS_H__
# include <rw/Xrw/viewdefs.h>
#endif

#ifdef RW_UNREASONABLE_NESTING
# include <rw/Xrw/Core.h>
#endif

#include <rw/Xrw/Form.h>
#include <rw/Xrw/appdefs.h>
#include <X11/Xlib.h>

class RWApplicationShell;
class RWForm;
class RWOrdered;
class RWPopupMenu;
class RWScrollBar;
class RWSet;
class RWShell;
class RWTopMenu;
class RWVendorShell;

/*********************************************************************
*  Resources for components of an RWView may be set in an
*  app-defaults file using strings composed of the "name" given in
*  the dialog constructor with identifying strings for each
*  component appended.  The names for each component are created
*  as follows:
*
*  Example: if name =    "myView":
*
*  COMPONENT              NAME   ID STRING       COMPONENT NAME
*  ---------              ----   ---------       --------------
*  view                 : name                   myView
*  topLevel             : name + "SHELL"         myViewSHELL
*  dialogManager        : name + "DM"            myViewDM
*  workArea             : name + "WA"            myViewWA
*  menuBar              : name + "MB"            myViewMB
*
*  name defaults to:     "RWView"
*
*********************************************************************/


class RWView : public RWApplication {
public:
  RWView ( unsigned int    argCount, 
	   char               **args,
	   const RWCString     &name,
	   RWApplicationShell  *top       = rwnil, 
	   RWBoolean            menu      = TRUE, 
	   RWBoolean            scroll    = TRUE, 
	   RWBoolean            back      = TRUE,
	   RWBoolean            autoStart = FALSE );

  RWView ( RWView        * topView, 
	   const RWCString    & name,
	   RWBoolean            menu      = TRUE, 
	   RWBoolean            scroll    = TRUE,
	   RWBoolean            back      = TRUE,
	   RWBoolean            autoStart = FALSE );

  RWView ();

  virtual ~RWView();

  void              start         (RWBoolean startChildren = FALSE);
#ifdef RWVIEW1_COMPATIBLE
  void              start         (RWView& otherView);
#endif
  virtual void      startView() {if (view) view->manage();}


  virtual void      stopView() {view->unmanage();}

  void          workAreaSize  (int xDim, int yDim);
  void          workAreaSize  (double xFrac, double yFrac);
  void          viewSize      (int xDim, int yDim);
  void          viewSize      (double xFrac, double yFrac);
  virtual void  showPopup     (RWApplication*, 
			       RWPopupMenu  *popup, 
			       XEvent       *event);
  void          setViewTitle  (const char * titleString);
  void          setChild      (RWView *kid);
  RWBoolean     removeChild   (RWView *kid);

  RWBoolean     addController (RWController*, 
			       int xPos = 0, 
			       int yPos = 0);

  RWTopMenu    *getMenu () const            { return menuBar; }
  RWForm       *getWorkArea () const        { return workArea; }
  RWController *getViewController () const  { return view; }
  RWView       *getParent () const          { return parent; }
  const RWOrdered    *getChildren () const        { return children; }
  const RWOrdered    *getComponents () const      { return constituents; }
  RWShell      *getTopLevel() const         { return topLevel; }

  RWBoolean     getChildren (RWSet& kids);
  RWBoolean     getComponents (RWSet& parts);

  void          writeTextAt (const char * text,
			     int          x,
			     int          y,
			     const char * textColor = rwnil,
			     const char * textFont  = rwnil);

  void          interceptClose();
  static  void  closeWindowCallback(Widget, XtPointer, XtPointer);
  virtual void  close(XmAnyCallbackStruct *) {;}  // Default is to do nothing

  virtual RWClassID isA() const { return(__RWVIEW); }
  
protected:
  RWView ( RWController *parent,
	   RWBoolean hasMenu,
	   const RWCString & name) ;
  RWView ( RWView * parentView, 
	   RWBoolean hasMenu,
	   const RWCString & name) ;

  RWForm       *getDialogManager  () const  { return dialogManager; }

#ifdef RWVIEW1_COMPATIBLE
  virtual void setPart (RWController *part, int xPos = 0, int yPos = 0);
#else
  void         setPart       (RWController *part);
#endif

  RWBoolean    removePart    (RWController *part);

  RWController *view;             //RWMainView or RWFormDialog
  RWShell      *topLevel;         //RWApplicationShell, RWTopLevelShell or
                                  //RWDialogShell
  RWForm       *dialogManager;    //Container for dialog components
  RWForm       *workArea;         //Where all action occurs
  RWTopMenu    *menuBar;          //If a menu bar is needed
  RWScrollBar  *hControl;         //Scrolling controllers
  RWScrollBar  *vControl;
  RWOrdered    *children;         //Child views
  RWOrdered    *constituents;     //Parts of this view
  RWView       *parent;           //Other than Motif parent widget
  RWBoolean     persistentScreen; //Save screen contents on scrolling?

  static void              wmCloseCallback     (Widget,        //shell
						RWView         *vw, 
						RWXtPointer    callData);

  virtual void              wmCloseMethod      (RWApplication*, //control
						RWXtPointer,    //clientData
						RWXtPointer);   //callData

private:
  void createMainView (RWShell *parentShell,
		       const RWCString & name,
		       RWBoolean menu, 
		       RWBoolean scroll, 
		       RWBoolean autoStart);

  void createDialogView (RWController *parent,
			 const RWCString & name,
			 RWBoolean hasMenu);

  void addWMCloseMethod(RWVendorShell *vShell);

  RWBoolean     ownTopLevel_;      //Does self own 'topLevel'?
  RWBoolean     mainLoopStarted_;  //Has XtAppMainLoop been called?
};


#endif /*  __RW_VIEWH  */
