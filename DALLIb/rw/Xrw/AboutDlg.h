#ifndef __RW_ABOUTDLGH
#define __RW_ABOUTDLGH

/*********************************************************************
*
*  $Id: AboutDlg.h,v 1.12 1994/01/11 03:19:58 cynthia Exp $
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
*  File        : AboutDlg.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for a
*                dialog used to display information "about" an
*                application to the user
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#include <rw/Xrw/Dialog.h>

/*********************************************************************
*  Resources for components of an RWAboutDialog may be set in an
*  app-defaults file using strings composed of the "name" given in
*  the dialog constructor with identifying strings for each
*  component appended.  The names for each component are created
*  as follows:
*
*  Example: if name =    "myDialog":
*
*  COMPONENT              NAME   ID STRING       COMPONENT NAME
*  ---------              ----   ---------       --------------
*  pixmapDisplay_       : name + "PIX"           myDialogPIX
*  iconDisplay_         : name + "ICON"          myDialogICON
*
*  name defaults to:     "RWAboutDialog"
*
*  See Dialoh.h for identifying strings of inherited components.
*********************************************************************/

class RWCString;

class RWAboutDialog : public RWDialogView {

public:

// Constructors and Destructors
  RWAboutDialog  (RWView * parent_view, 
						const char* name = "RWAboutDialog",
						RWButtonConfig buttons = RWOkButton);
  virtual ~RWAboutDialog  ();

  virtual RWDialogReply getResponse (RWDialogStyle modal = RWModalDialog)
	                           {return getResponseToDialog(modal);}

// Virtual Functions
  virtual void    setColor    (const char* color);
  virtual void    setPixmap   (const char* newPixmapName,
										 const char* fg = rwnil,
										 const char* bg = rwnil)
	 { addPixmapLabel(pixmapDisplay_, newPixmapName, pixmapPath_, fg, bg); }
  virtual void    setIcon     (const char* newIconName,
										 const char* fg = rwnil,
										 const char* bg = rwnil)
	 { addPixmapLabel(iconDisplay_, newIconName, iconPath_, fg, bg); }

  virtual RWClassID isA         () const { return(__RWABOUTDIALOG); }

protected:

  RWLabel *    pixmapDisplay_;
  RWLabel *    iconDisplay_;
  RWCString    pixmapPath_;
  RWCString    iconPath_;
  
private:
  void    addPixmapLabel (RWLabel* lbl,
								  const RWCString & newPixmapName,
								  RWCString & pixmapName,
								  const char* fg,
								  const char* bg);
  
};

#endif
