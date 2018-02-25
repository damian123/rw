#ifndef __RW_RESOURCEOBJ_H
#define __RW_RESOURCEOBJ_H
/*********************************************************************
*
*  $Id: ResObj.h,v 1.18 1993/10/22 22:05:14 keffer Exp $
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
*  File        : ResObj.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : October 1991
*
*  Project     : View.h++
*
*  Description : This file contains the class declaration for
*                a resource class that is used to set and get
*                resource values.
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

#include <rw/Xrw/DynArgArr.h>
#include <rw/Xrw/String.h>

#if RWX11Version>=5
#include <Xm/DragDrop.h>
#endif

class RWController;
class RWPulldownMenu;
class RWFontList;

#if RWX11Version>=5
class RWDragIcon;
#endif

/*********************************************************************
* RWResourceObj : This file contains the class declaration for
*                 a resource class that is used to set and get
*                 resource values.
*********************************************************************/

class RWResourceObj {
public:
  RWResourceObj();
  ~RWResourceObj();

  Arg * getArgs   () const { return &dargs; }
  int   getArgCnt () const { return argcnt; }
  void  reset()            { argcnt = 0; }

  // RWBulletinBoard resources 
  void setAutoUnmanage           (Boolean);
  void setTextTranslations       (XtTranslations);

  // RWManager resources
  void setXmStringDirection      (XmStringDirection);
  void setStringDirection        (RWXmStringDirection *);

  // RWSelectionBox resources
  void setDialogType             (unsigned char);
  void setTextAccelerators       (XtAccelerators);

  // RWRowColumn resources
  void setRowColumnType          (unsigned char);
  void setSubMenuId              (RWPulldownMenu *);

  // RWCore resources
  void setColormap               (Colormap);
  void setDepth                  (int);
  void setScreen                 (Screen *);

  // RWList resources
  void setListSizePolicy         (unsigned char);

  // RWMainWindow
  void setCommandWindowLocation  (unsigned char);
  
  // RWScrolledText resource
  void setScrollHorizontal       (Boolean);
  void setScrollLeftSide         (Boolean);
  void setScrollTopSide          (Boolean);
  void setScrollVertical         (Boolean);

  // RWScrolledWindow resource
  void setScrollBarDisplayPolicy (unsigned char);
  void setScrollingPolicy        (unsigned char);
  void setVisualPolicy           (unsigned char);

// RWMenuShell resources
  void setDefaultFontList        (RWFontList *);

#if RWX11Version>=5
// RWXmScreen resources
  void setDarkThreshold          (int);
  void setForegroundThreshold    (int);
  void setLightThreshold         (int);
  void setMenuCursor             (String);

// XmDisplay resources
#if 0
  void setDefaultVirtualBindings     (String);
#endif
  void setDragInitiatorProtocolStyle (unsigned char);
  void setDragReceiverProtocolStyle  (unsigned char);

// RWDragIcon resources
  void setAttachment (unsigned char);
  void setHeight (Dimension);
  void setHotX (Position);
  void setHotY (Position);
  void setMask (Pixmap);
  void setOffsetX (Position);
  void setOffsetY (Position);
  void setPixmap (Pixmap);
  void setWidth (Dimension);

// RWDragContext resources
  void setBlendModel             (unsigned char);
  void setConvertProc            (XtConvertSelectionIncrProc);
  void setClientData             (XtPointer);
  void setCursorBackground       (const RWCString &, RWController *);
  void setCursorBackground       (Pixel);
  void setCursorForeground       (const RWCString &, RWController *);
  void setCursorForeground       (Pixel);
  void setDragOperations         (unsigned char);
  void setExportTargets          (Atom *);
  void setInvalidCursorForeground(const RWCString &, RWController *);
  void setInvalidCursorForeground(Pixel);
  void setNoneCursorForeground   (const RWCString &, RWController *);
  void setNoneCursorForeground   (Pixel);
  void setNumExportTargets       (Cardinal);
  void setOperationCursorIcon    (RWDragIcon *);
  void setSourceCursorIcon       (RWDragIcon *);
  void setSourcePixmapIcon       (RWDragIcon *);
  void setStateCursorIcon        (RWDragIcon *);
  void setValidCursorForeground  (const RWCString &, RWController *);
  void setValidCursorForeground  (Pixel);

// RWDropSite resources
  void setDropSiteType		 (unsigned char);
  void setAnimationMask          (Pixmap);
  void setAnimationPixmap        (Pixmap);
  void setAnimationPixmapDepth   (int);
  void setAnimationStyle         (unsigned char);
  void setDragProc               (void *);
  void setDropProc               (void *);
  void setDropRectangles         (XRectangle *);
  void setDropSiteActivity       (unsigned char);
  void setDropSiteOperations     (unsigned char);
  void setImportTargets          (Atom *);
  void setNumDropRectangles      (Cardinal);
  void setNumImportTargets       (Cardinal);

// RWDropTransfer resources
  void setDropTransfers          (XmDropTransferEntry);    // DO NOT USE
  void setNumDropTransfers       (Cardinal);               // DO NOT USE
  void setIncremental            (Boolean);
  void setTransferProc           (void *);                 // DO NOT USE
  void setTransferStatus         (unsigned char);
#endif

// All Callback resources
  void setActivateCallback          (XtCallbackList);
  void setArmCallback               (XtCallbackList);
  void setDisarmCallback            (XtCallbackList);
  void setFocusCallback             (XtCallbackList);
  void setMapCallback               (XtCallbackList);
  void setUnmapCallback             (XtCallbackList);  
  void setCascadingCallback         (XtCallbackList);
  void setCommandChangedCallback    (XtCallbackList);
  void setCommandEnteredCallback    (XtCallbackList);
  void setExposeCallback            (XtCallbackList);
  void setInputCallback             (XtCallbackList);
  void setResizeCallback            (XtCallbackList);
  void setHelpCallback              (XtCallbackList);
  void setBrowseSelectionCallback   (XtCallbackList);
  void setDefaultActionCallback     (XtCallbackList);
  void setExtendedSelectionCallback (XtCallbackList);
  void setMultipleSelectionCallback (XtCallbackList);
  void setSingleSelectionCallback   (XtCallbackList);
  void setCancelCallback            (XtCallbackList);
  void setOkCallback                (XtCallbackList);
//  void setDestroyCallback (XtCallbackList);
  void setEntryCallback             (XtCallbackList);
  void setDragCallback              (XtCallbackList);
  void setValueChangedCallback      (XtCallbackList);
  void setDecrementCallback         (XtCallbackList);
  void setIncrementCallback         (XtCallbackList);
  void setPageDecrementCallback     (XtCallbackList);
  void setPageIncrementCallback     (XtCallbackList);
  void setToBottomCallback          (XtCallbackList);
  void setToTopCallback             (XtCallbackList);
  void setApplyCallback             (XtCallbackList);
  void setNoMatchCallback           (XtCallbackList);
  void setPopdownCallback           (XtCallbackList);
  void setPopupCallback             (XtCallbackList);
  void setLosingFocusCallback       (XtCallbackList);
  void setModifyVerifyCallback      (XtCallbackList);
  void setMotionVerifyCallback      (XtCallbackList);
  void setGainPrimaryCallback       (XtCallbackList);
  void setLosePrimaryCallback       (XtCallbackList);

#if RWX11Version>=5
  void setDragDropFinishCallback    (XtCallbackList);
  void setDragMotionCallback        (XtCallbackList);
  void setDropFinishCallback        (XtCallbackList);
  void setDropSiteEnterCallback     (XtCallbackList);
  void setDropSiteLeaveCallback     (XtCallbackList);
  void setDropStartCallback         (XtCallbackList);
  void setOperationChangedCallback  (XtCallbackList);
  void setTopLevelEnterCallback     (XtCallbackList);
  void setTopLevelLeaveCallback     (XtCallbackList);
#endif

// Member functions to set resources by name.
  void setArg (String, void*);
  void setArg (String, RWController *);
  void setArg (String, int);
  void setArg (String, unsigned int);
  void setArg (String, unsigned long);
  void setArg (String, unsigned short);
  void setArg (String, unsigned char);
  
 private:
  RWDynamicArgArray  dargs;
  int                argcnt;

  Boolean checkResourceBufferSize ();
};


#endif  /* End of file ResObj.h - Do not add beyond this point! */
