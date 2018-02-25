#ifndef __RW_DRAGCONTEXT_H
#define __RW_DRAGCONTEXT_H
/*********************************************************************
*
*  $Id: DragCon.h,v 1.8 1993/10/18 17:51:48 keffer Exp $
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
*  File        : DragCon.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1992
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                drag context class.
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

/*********************************************************************
*
* #include files
*
********************************************************************/

#include <rw/Xrw/Core.h>
#include <rw/Xrw/DragIcon.h>

class RWConvertCB;

/*********************************************************************
* RWDragContext : Class object which represents the DragContext
* widget in the OSF/Motif widget set.  This is an internal class
* which is not meant to be instantiated directly.
*********************************************************************/

class RWDragContext : public RWCore {
public:
  RWDragContext
	(
	RWController * obj,
	XEvent *       event,
	RWResourceObj  resObj
	);
  RWDragContext
	(
	RWController * obj,
	XEvent *       event
	);
  RWDragContext
	(
	Widget
	);
  ~RWDragContext();

// RESOURCE MEMBER FUNCTIONS
  void                     setBlendModel (unsigned char);
  unsigned char            getBlendModel (unsigned char * val = 0);
  void                     setCursorBackground (const RWCString &);
  void                     setCursorBackground (Pixel);
  Pixel                    getCursorBackground (Pixel * val = 0);
  void                     setCursorForeground (const RWCString &);
  void                     setCursorForeground (Pixel);
  Pixel                    getCursorForeground (Pixel * val = 0);
  void                     setDragDropFinishCallback (RWXtCallbackProc, RWXtPointer);
  void                     setDragDropFinishMethod (RWApplication *, RWMethodPtr, void *);
  void                     setDragDropFinishCallback (XtCallbackList);
  void                     setDragMotionCallback (RWXtCallbackProc, RWXtPointer);
  void                     setDragMotionMethod (RWApplication *, RWMethodPtr, void *);
  void                     setDragMotionCallback (XtCallbackList);
  unsigned char		   getDragOperations (unsigned char * val = 0);
  void                     setDropFinishCallback (RWXtCallbackProc, RWXtPointer);
  void                     setDropFinishMethod (RWApplication *, RWMethodPtr, void *);
  void                     setDropFinishCallback (XtCallbackList);
  void                     setDropSiteEnterCallback (RWXtCallbackProc, RWXtPointer);
  void                     setDropSiteEnterMethod (RWApplication *, RWMethodPtr, void *);
  void                     setDropSiteEnterCallback (XtCallbackList);
  void                     setDropSiteLeaveCallback (RWXtCallbackProc, RWXtPointer);
  void                     setDropSiteLeaveMethod (RWApplication *, RWMethodPtr, void *);
  void                     setDropSiteLeaveCallback (XtCallbackList);
  void                     setDropStartCallback (RWXtCallbackProc, RWXtPointer);
  void                     setDropStartMethod (RWApplication *, RWMethodPtr, void *);
  void                     setDropStartCallback (XtCallbackList);
  Atom *                   getExportTargets (Atom ** val = 0);
  void                     setIncremental (Boolean);
  Boolean                  getIncremental (Boolean * val = 0);
  void                     setInvalidCursorForeground (const RWCString &);
  void                     setInvalidCursorForeground (Pixel);
  Pixel                    getInvalidCursorForeground (Pixel * val = 0);
  void                     setNoneCursorForeground (const RWCString &);
  void                     setNoneCursorForeground (Pixel);
  Pixel                    getNoneCursorForeground (Pixel * val = 0);
  void                     setNumExportTargets (Cardinal);
  Cardinal                 getNumExportTargets (Cardinal * val = 0);
  void                     setOperationChangedCallback (RWXtCallbackProc, RWXtPointer);
  void                     setOperationChangedMethod (RWApplication *, RWMethodPtr, void *);
  void                     setOperationChangedCallback (XtCallbackList);
  void                     setOperationCursorIcon (RWDragIcon *);
  RWDragIcon *             getOperationCursorIcon (RWDragIcon ** val = 0);
  void                     setSourceCursorIcon (RWDragIcon *);
  RWDragIcon *             getSourceCursorIcon (RWDragIcon ** val = 0);
  void                     setSourcePixmapIcon (RWDragIcon *);
  RWDragIcon *             getSourcePixmapIcon (RWDragIcon ** val = 0);
  void                     setStateCursorIcon (RWDragIcon *);
  RWDragIcon *             getStateCursorIcon (RWDragIcon ** val = 0);
  void                     setTopLevelEnterCallback (RWXtCallbackProc, RWXtPointer);
  void                     setTopLevelEnterMethod (RWApplication *, RWMethodPtr, void *);
  void                     setTopLevelEnterCallback (XtCallbackList);
  void                     setTopLevelLeaveCallback (RWXtCallbackProc, RWXtPointer);
  void                     setTopLevelLeaveMethod (RWApplication *, RWMethodPtr, void *);
  void                     setTopLevelLeaveCallback (XtCallbackList);
  void                     setValidCursorForeground (const RWCString &);
  void                     setValidCursorForeground (Pixel);
  Pixel                    getValidCursorForeground (Pixel * val = 0);

  void                     setConvertProc (RWConvertPtr, RWXtPointer);
  void                     setConvertMethod (RWApplication *, RWConvertMethodPtr, RWXtPointer);

  void                     setClientData (RWXtPointer cd); 
  RWXtPointer              getClientData (RWXtPointer * val = 0);
 
  Atom                     addExportTarget (Atom);
  Atom                     addExportTarget (const RWCString &);

  RWAtomOrderedVec *       getExportList()      { return exportList_; }
  RWConvertCB *            getCallbackStruct () { return callbackStruct; }
  Widget                   getDragWidget ()     { return dragWidget_; }

// MEMBER FUNCTIONS
  inline void            dragCancel   ();  

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const 
	{
	return __RWDRAGCONTEXT;
	}
  
private:
  RWCallback * getCallbackObject (RWXtPointer);

  Widget                   dragWidget_;
  RWXtPointer              client_data_;
  RWAtomOrderedVec *       exportList_;
  RWConvertCB *            callbackStruct;
};

#endif  /* End of file DragContext.h - Do not add beyond this point! */
