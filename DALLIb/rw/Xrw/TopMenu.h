#ifndef __RW_TOPMENUH
#define __RW_TOPMENUH

/*********************************************************************
*
*  $Id: TopMenu.h,v 1.11 1994/01/11 00:23:12 cynthia Exp $
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
*  File        : TopMenu.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for a
*                menubar for creating pulldown menu systems
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#include <rw/Xrw/Menu.h>

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
*
*  name defaults to:     "RWTopMenu"
*
*********************************************************************/

class RWTopMenu : public RWMenuObject {
public:

  friend RWBoolean RWMenu::attachTo (RWTopMenu       * mainMenu, 
				     const char      * menuLabel,
				     char              mnemonic,
				     RWBoolean         helpMenu);
  

  RWTopMenu (RWView* parentView,
	     const char *name = "RWTopMenu");
  ~RWTopMenu();

  virtual void addOption      (const char      *buttonName, 
			       char              mnemonic    = '\0', 
			       RWApplication    *methodObj   = rwnil, 
			       RWMethodPtr       meth        = rwnil, 
			       RWXtPointer       userData    = rwnil,
			       const char        *ignore     = rwnil);

  // Add a cascading menu
  virtual RWBoolean addSubMenu(RWMenuObject    * childMenu,
			       const char      * menuLabel,
			       char              mnemonic = '\0');

  virtual RWBoolean  addHelpMenu(RWMenuObject    * childMenu,
				 const char      * helpLabel = "Help",
				 char              mnemonic  = 'H');

  virtual RWClassID isA() const     { return(__RWTOPMENU); }

protected:
  
  // Add a cascading menu
  virtual RWBoolean addPulldownMenu(RWMenuObject    * childMenu,
				    const RWCString & menuLabel,
				    char              mnemonic,
				    RWBoolean         helpMenu = FALSE);

};

#endif
