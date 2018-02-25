#ifndef __RW_MENUITEM
#define __RW_MENUITEM

/*********************************************************************
*
*  $Id: MenuItem.h,v 1.5 1994/01/11 00:05:29 cynthia Exp $
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
*  File        : MenuItem.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for a
*                convenience object for menu creation
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#include <rw/cstring.h>
#include <rw/Xrw/Acceltr.h>
#include <rw/Xrw/App.h>

enum RWMenuItemType { RWMenuLabel,
                      RWMenuOption,
                      RWMenuToggleOption,
                      RWMenuPixmapOption,
                      RWMenuSeparator,
                      RWMenuSubMenu,
                      RWMenuNone };

class RWMenu;

class RWMenuItem 
{
public:
  RWMenuItem();                            //Default Constructor
  RWMenuItem(const RWMenuItem& menuItem);  //Copy constructor

  RWMenuItem (const RWCString  &label);    //Label menu item

  RWMenuItem (const RWCString  &label,     //Option or Toggle Option menu item
              char              mnemonic,
              RWApplication    *obj, 
              RWMethodPtr       method, 
              RWXtPointer       clientData,
              const char       *accelString = rwnil,
              RWMenuItemType    type        = RWMenuOption);

  RWMenuItem (const RWCString  &pixPath,     //Pixmap Option menu item
              RWApplication    *obj, 
              RWMethodPtr       method, 
              RWXtPointer       clientData,
	      const char       *fgColor     = "black",
	      const char       *bgColor     = "white",
              const char       *accelString = rwnil);
  
  RWMenuItem (RWMenuItemType    type);     //For creating Separator menu item
  
  RWMenuItem (const RWCString  &label,     //SubMenu menu item
              char              mnemonic,
              RWMenu           *subMenu,
              const char       *accelString = rwnil);

  virtual ~RWMenuItem();

  RWMenuItem& operator=(const RWMenuItem& rhs);      //Assignment operator

  RWBoolean operator==(const RWMenuItem& rhs) const; //Equivalence operator

  RWMenuItemType       getItemType      () const   { return itemType_;    }
  const RWCString     &getLabel         () const   { return label_;       }
  char                 getMnemonic      () const   { return mnemonic_;    }
  RWApplication       *getObject        () const   { return obj_;         }
  RWMethodPtr          getMethod        () const   { return method_;      }
  RWXtPointer          getClientData    () const   { return clientData_;  }
  const RWAccelerator *getAccelerator   () const   { return accelerator_; }
  const RWMenu        *getSubMenu       () const   { return subMenu_;     }
  const RWCString     &getPixmapPath    () const   { return pixmapPath_;  }
  const RWCString     &getPixForeground () const   { return pixmapFg_;    }
  const RWCString     &getPixBackground () const   { return pixmapBg_;    }
  RWBoolean            contains         (const RWMenu *menu) const;

  virtual RWClassID isA() const { return(__RWMENUITEM); }

private:

  RWMenuItemType   itemType_;    //Type of menu item
  RWCString        label_;       //Label of the menu
  char             mnemonic_;    //Mnemonic for the label
  RWApplication *  obj_;         //Object on which to fire method
  RWMethodPtr      method_;      //Method to fire on object
  RWXtPointer      clientData_;  //Client data to pass to method
  RWAccelerator *  accelerator_; //Menu Accelerator
  RWMenu        *  subMenu_;     //SubMenu for cascading menu system
  RWCString        pixmapPath_;  //Pixmap of the menu
  RWCString        pixmapFg_;    //Pixmap foreground color
  RWCString        pixmapBg_;    //Pixmap background color 

};

#endif
