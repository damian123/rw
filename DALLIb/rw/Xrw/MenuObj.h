#ifndef __RW_MENUSYSH
#define __RW_MENUSYSH

/*********************************************************************
*
*  $Id: MenuObj.h,v 1.11 1994/01/11 00:05:29 cynthia Exp $
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
*  File        : MenuObj.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the 
*                abstract base class from which all application class 
*                menus are derived
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#include <rw/Xrw/Primitive.h>
#include <rw/Xrw/RowColumn.h>
#include <rw/cstring.h>

class RWView;
class RWMenuObject;
class RWOrdered;
class RWHashDictionary;

/*********************************************************************
*  Resources for components of an RWMenuObject may be set in an
*  app-defaults file using strings composed of the "name" given in
*  the constructor with identifying strings for each component appended.
*  The names for each component are created as follows:
*
*  Example: if name =    "myMenu":
*
*  COMPONENT              NAME   ID STRING       COMPONENT NAME
*  ---------              ----   ---------       --------------
*  menu_                : name + "MENU"          myMenuMENU
*
*  name defaults to:     "RWMenuObject"
*
*********************************************************************/

class RWMenuObject : public RWApplication {
public:
  RWMenuObject();
  RWMenuObject(const RWCString & name);
  RWMenuObject(RWView* parentView, const RWCString & name);
  virtual ~RWMenuObject();
	
  virtual RWBoolean setMnemonicAt  (int pos,            // hilight letter
				    char mnemonic);
  
  virtual void setSensitive        (RWBoolean);

  virtual RWBoolean setSensitiveAt (int pos,            // enable or disable
				    RWBoolean flag);
  
  virtual void setColor            (const RWCString & color);	// set color of menu
  virtual void setColor            (const char * background,	// set color of menu
				    const char * foreground = rwnil);

  virtual RWBoolean setColorAt     (int pos,            // set option color
				    const RWCString & color);

  virtual RWBoolean setColorAt     (int pos,            // set option color
				    const char * background,
				    const char * foreground = rwnil);
  
  void setRadioBehavior(RWBoolean val);


  RWBoolean    getRadioBehavior()      { return radioBehavior_; }

  RWPrimitive *getControllerAt    (int i) const;        // controller at pos i

  virtual RWRowColumn *getMenuControl() const            // rowcol controller
                                        { return(menu_); }
  RWMenuObject*getMenuAt      (int) const;               // menu at position i

  RWView      *getView        () const  { return(ownerView_); }  // return the view for this menu

  virtual RWBoolean  deleteSubMenu  (RWMenuObject * child);

  virtual void       addOption (const char * buttonName, 
				char              mnemonic    = '\0', 
				RWApplication    *methodObj   = rwnil, 
				RWMethodPtr       meth        = rwnil, 
				RWXtPointer       userData    = rwnil,
				const char        *accel      = rwnil) = 0;

  virtual RWBoolean addSubMenu(RWMenuObject    * childMenu,
			       const char      * menuLabel,
			       char              mnemonic = '\0') = 0;

  virtual RWBoolean  isMember (const RWMenuObject* mem) const;

  const RWCString    getName  () const { return menuName_; }

  virtual RWClassID isA() const { return(__RWMENUOBJECT); }
	
protected:
  virtual RWBoolean addPulldownMenu(RWMenuObject    * childMenu,
				    const RWCString & menuLabel,
				    char              mnemonic = '\0',
				    RWBoolean         helpMenu = FALSE) = 0;

  void     createPulldownMenu (RWController *parent,
			       const char   *name);

  void     createPopupMenu    (RWController *parent,
			       const char   *name);

  void     setView            (RWView* v)     { ownerView_ = v; } //assign view

  RWOrdered        *options_;      //ordered collection of menu button controls
  RWHashDictionary *subMenuTable_; //table of pulldowns indexed by option name
  RWView           *ownerView_;    //view to which this menu belongs
  RWRowColumn      *menu_;         //controller representing the menu
  RWCString         menuName_;     //name of menu - for setting resources


private:
  RWBoolean         radioBehavior_; // should toggle buttons behave
                                    //    as radio buttons
  
};

#endif
