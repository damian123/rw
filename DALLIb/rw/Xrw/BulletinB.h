#ifndef __RW_BULLETIN_BOARD_H
#define __RW_BULLETIN_BOARD_H
/*********************************************************************
*  $Id: BulletinB.h,v 1.13 1993/10/07 01:42:57 keffer Exp $
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
*  File        : BulletinB.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Bulletin Board and Bulletin Board Dialog classes.
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

/*********************************************************************
* RWBulletinBoard : Class object which represents the XmBulletinBoard
* widget in the OSF/Motif widget set.  It is used as a container for
* other widgets (i.e., a workarea).
*********************************************************************/

class RWBulletinBoard : public RWManager
{
 public:
  RWBulletinBoard 
	(
	RWController *, 
	const RWCString &,
	RWBoolean managed = False
	);
  RWBulletinBoard 
	(
	RWController *, 
	const RWCString &,
	const RWResourceObj &,
	RWBoolean managed = False
	);
  RWBulletinBoard 
	(
	RWController *, 
	RWResourceObj * ro = 0
	);
  RWBulletinBoard ();
  RWBulletinBoard 
	(
	Widget, 
	RWBoolean managed = False
	);  // Internal.
  ~RWBulletinBoard ();

  void          createWidget       (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void 		setAllowOverlap    (Boolean);
  Boolean 	getAllowOverlap    (Boolean *val = 0);
  Boolean 	getAutoUnmanage    (Boolean *val = 0);
  void 		setButtonFontList  (RWFontList *);
  RWFontList   *getButtonFontList  (RWFontList **val = 0);
  void 		setCancelButton    (RWController *);
  RWController *getCancelButton    (RWController **val = 0);
  void 		setDefaultButton   (RWController *);
  RWController *getDefaultButton   (RWController **val = 0);
  void 		setDefaultPosition (Boolean);
  Boolean 	getDefaultPosition (Boolean *val = 0);
  void 		setDialogStyle     (unsigned char);
  unsigned char getDialogStyle     (unsigned char *val = 0);
  void 		setDialogTitle     (RWXmString *);
  void 		setDialogTitle     (const RWCString &);
  RWXmString   *getDialogTitle     (RWXmString **val = 0);
  RWCString     getDialogTitleC    (RWCString * val = 0);
  void 		setFocusCallback   (RWXtCallbackProc, RWXtPointer);
  void 		setFocusMethod     (RWApplication *, RWMethodPtr, void *);
  void 		setFocusCallback   (XtCallbackList);
  void 		setLabelFontList   (RWFontList *);
  RWFontList   *getLabelFontList   (RWFontList **val = 0);
  void 		setMapCallback     (RWXtCallbackProc, RWXtPointer);
  void 		setMapMethod       (RWApplication *, RWMethodPtr, void *);
  void 		setMapCallback     (XtCallbackList);
  void 		setMarginHeight    (Dimension);
  Dimension 	getMarginHeight    (Dimension *val = 0);
  void 		setMarginWidth     (Dimension);
  Dimension     getMarginWidth     (Dimension *val = 0);
  void 		setNoResize        (Boolean);
  Boolean 	getNoResize        (Boolean *val = 0);
  void 		setResizePolicy    (unsigned char);
  unsigned char getResizePolicy    (unsigned char *val = 0);
  void 		setShadowType      (unsigned char);
  unsigned char getShadowType      (unsigned char *val = 0);
  void 		setTextFontList    (RWFontList *);
  RWFontList   *getTextFontList    (RWFontList **val = 0);
  void 		setUnmapCallback   (RWXtCallbackProc, RWXtPointer);  
  void 		setUnmapMethod     (RWApplication *, RWMethodPtr, void *);  
  void 		setUnmapCallback   (XtCallbackList);  

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const 
	{
	return __RWBULLETINBOARD;
	}
};
#endif


#ifndef __RW_BULLETIN_BOARD_DIALOG_H
#define __RW_BULLETIN_BOARD_DIALOG_H
/*********************************************************************
* RWBulletinBoardDialog : Class object which represents the 
* XmBulletinBoardDialog widget in the OSF/Motif widget set.  It is 
* used as a container for other widgets (i.e., a workarea).
*********************************************************************/

class RWBulletinBoardDialog : public RWBulletinBoard
{
 public:
  RWBulletinBoardDialog 
	(
	RWController *, 
	const RWCString &,
	RWBoolean managed = False
	);
  RWBulletinBoardDialog 
	(
	RWController *, 
	const RWCString &, 
	const RWResourceObj &,
	RWBoolean managed = False
	);
  RWBulletinBoardDialog 
	(
	RWController *, 
	RWResourceObj * ro = 0
	);
  ~RWBulletinBoardDialog ();

  void createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const 
	{
	return __RWBULLETINBOARDDIALOG;
	}
};

#endif  /* End of file BulletinB.h - Do not add beyond this point! */
