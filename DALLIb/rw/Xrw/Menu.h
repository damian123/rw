#ifndef __RW_MENUH
#define __RW_MENUH

/*********************************************************************
*
*  $Id: Menu.h,v 1.12 1994/01/11 00:05:29 cynthia Exp $
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
*  File        : Menu.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                representation of pulldown menus, popup menus, and
*                sunmenus.
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#include <rw/Xrw/MenuObj.h>
#include <rw/Xrw/MenuArg.h>

/*********************************************************************
*  Resources for components of an RWTopMenu may be set in an
*  app-defaults file using strings composed of the "name" given in
*  the constructor with identifying strings for each component appended.
*  The names for each component are created as follows:
*
*  Example: if name =    "myMenu":
*
*  COMPONENT              NAME   ID STRING       COMPONENT NAME
*  ---------              ----   ---------       --------------
*  <cascade buttons>    : name + "MENUCB"        myMenuMENUCB
*  <separators>         : name + "MENUSEP"       myMenuMENUSEP
*  <labels>             : name + "MENULBL"       myMenuMENULBL
*  <push buttons>       : name + "MENUPB"        myMenuMENUPB
*  <toggle buttons>     : name + "MENUTB"        myMenuMENUTB
*
*  name defaults to:     "RWMenu"
*
*********************************************************************/

class RWTopMenu;

class RWMenu : public RWMenuObject {
public:
  RWMenu(const char *name = "RWMenu");
  ~RWMenu();
  
  virtual void addLabel        (const char *label);

  virtual void addOption       (const char       *label, 
				char              mnemonic    = '\0', 
				RWApplication    *obj         = rwnil, 
				RWMethodPtr       method      = rwnil, 
				RWXtPointer       clientData  = rwnil,
				const char       *accelString = rwnil);

  virtual void addToggleOption (const char       *label,
				char              mnemonic,
				RWApplication    *obj, 
				RWMethodPtr       method, 
				RWXtPointer       clientData  = rwnil,
				const char       *accelString = rwnil);

  virtual void addPixmapOption (const char       *pixPath,
				RWApplication    *obj, 
				RWMethodPtr       method, 
				RWXtPointer       clientData  = rwnil,
				const char       *fgColor     = RWDefaultFGColor,
				const char       *bgColor     = RWDefaultColor,
				const char       *accelString = rwnil);

  void         addSeparator();

  // Add a cascading menu
  virtual RWBoolean  addSubMenu(RWMenuObject    * childMenu,
				const char      * menuLabel,
				char              mnemonic = '\0');

  // Attach the menu to an upper level object
  RWBoolean attachTo (RWTopMenu       * mainMenu, 
		      const char      * menuLabel,
		      char              mnemonic = '\0',
#ifdef RWVIEW1_COMPATIBLE
		      const char      * resName = "",
#endif
		      RWBoolean         helpMenu = FALSE);

  RWBoolean attachTo (RWMenu          * upperMenu, 
		      const char      * menuLabel, 
		      char              mnemonic = '\0'
#ifdef RWVIEW1_COMPATIBLE
		      ,
		      const char      * resName = ""
#endif
		      );

  RWBoolean attachTo (RWView          * parentView
#ifdef RWVIEW1_COMPATIBLE
		      , 
		      const char      * resName = ""
#endif
		      );

  //-- General Set and Get Methods

  // return the top level menu bar
  RWTopMenu   *getTopMenu() const { return(topMenu_); }


  RWBoolean setMethodAt (int            pos, 
			 RWApplication *obj,
			 RWMethodPtr    method, 
			 RWXtPointer    userData = rwnil );

  // sets accelerator key
  RWBoolean setAcceleratorAt (int            pos, 
			      RWAccelerator* accel);

  // sets accel, char data
  RWBoolean setAcceleratorAt (int               pos, 
			      const char      * text );

  virtual RWBoolean  isMember (const RWMenuObject* mem) const
	 { return (menuValues_->isMember((RWMenu*)mem)); }
  
  virtual RWClassID isA() const { return(__RWMENU); }
  
protected:
  // Add a cascading menu
  virtual RWBoolean addPulldownMenu(RWMenuObject    * childMenu,
				    const RWCString & menuLabel,
				    char              mnemonic,
				    RWBoolean         ignore   = FALSE);

  //Make the underlying controllers
  void createControllers(); 

  RWTopMenu   *topMenu_;	   // menu bar (RWTopMenu) this menu is attached to
  RWMenuArg   *menuValues_;   // Menu argument container


private:
  void                 createLabel        (int pos);
  void                 createOption       (int pos);
  void                 createToggleOption (int pos);
  void                 createPixmapOption (int pos);
  void                 createSeparator    ();
  void                 createSubMenu      (int pos);
  
};

#endif
