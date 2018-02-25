#ifndef __RW_DRAG_CONTEXT_CB_H
#define __RW_DRAG_CONTEXT_CB_H
/*********************************************************************
*
*  $Id: DragConCB.h,v 1.7 1993/09/30 19:40:10 keffer Exp $
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
*  File        : DragConCB.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : January 1993
*
*  Project     : View.h++
*
*  Description : This file contains the declaration of an View.h++ class
*                object which encapsulates the 
*                XmDragDropFinishCallbackStruct and the
*                XmDragMotionCallbackStruct.
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

#include <rw/Xrw/AnyCB.h>  
#include <Xm/DragC.h>

/*********************************************************************
* RWDragDropFinishCallback : Class which encapsulates the X-Windows
* XmDragDropFinishCallbackStruct.
*********************************************************************/

class RWDragDropFinishCallback : public RWAnyCallback {
public:
  RWDragDropFinishCallback (XmDragDropFinishCallbackStruct * cbs)
        : RWAnyCallback ((XmAnyCallbackStruct *)cbs) {}
  ~RWDragDropFinishCallback () {}

  Time  getTimeStamp() { return shadow()->timeStamp; }

private:
  XmDragDropFinishCallbackStruct * shadow() 
        { return (XmDragDropFinishCallbackStruct *)shadow_; }
};

/*********************************************************************
* RWOperationChangedCallback : Class which encapsulates the X-Windows
* XmOperationChangedCallbackStruct.
*********************************************************************/

class RWOperationChangedCallback : public RWAnyCallback {
public:
  RWOperationChangedCallback (XmOperationChangedCallbackStruct * cbs)
        : RWAnyCallback ((XmAnyCallbackStruct *)cbs) {}
  ~RWOperationChangedCallback () {}

  Time           getTimeStamp()      { return shadow()->timeStamp; }
  unsigned char  getOperation()      { return shadow()->operation; }
  unsigned char  getOperations()     { return shadow()->operations; }
  unsigned char  getDropSiteStatus() { return shadow()->dropSiteStatus; }

private:
  XmOperationChangedCallbackStruct * shadow() 
        { return (XmOperationChangedCallbackStruct *)shadow_; }
};

/*********************************************************************
* RWDragMotionCallback : Class which encapsulates the X-Windows
* XmDragMotionCallbackStruct.
*********************************************************************/

class RWDragMotionCallback : public RWAnyCallback {
public:
  RWDragMotionCallback (XmDragMotionCallbackStruct * cbs)
        : RWAnyCallback ((XmAnyCallbackStruct *)cbs) {}
  ~RWDragMotionCallback () {}

  Time           getTimeStamp()      { return shadow()->timeStamp; }
  unsigned char  getOperation()      { return shadow()->operation; }
  unsigned char  getOperations()     { return shadow()->operations; }
  unsigned char  getDropSiteStatus() { return shadow()->dropSiteStatus; }
  Position       getX()              { return shadow()->x; }
  Position       getY()              { return shadow()->y; }

private:
  XmDragMotionCallbackStruct * shadow() 
        { return (XmDragMotionCallbackStruct *)shadow_; }
};


/*********************************************************************
* RWDropFinishCallback : Class which encapsulates the X-Windows
* XmDropFinishCallbackStruct.
*********************************************************************/

class RWDropFinishCallback : public RWAnyCallback {
public:
  RWDropFinishCallback (XmDropFinishCallbackStruct * cbs)
        : RWAnyCallback ((XmAnyCallbackStruct *)cbs) {}
  ~RWDropFinishCallback () {}

  Time           getTimeStamp()        { return shadow()->timeStamp; }
  unsigned char  getOperation()        { return shadow()->operation; }
  unsigned char  getOperations()       { return shadow()->operations; }
  unsigned char  getDropSiteStatus()   { return shadow()->dropSiteStatus; }
  unsigned char  getDropAction()       { return shadow()->dropAction; }
  unsigned char  getCompletionStatus() { return shadow()->completionStatus; }

  void setCompletionStatus (unsigned char cs) { shadow()->completionStatus = cs; }

private:
  XmDropFinishCallbackStruct * shadow() 
        { return (XmDropFinishCallbackStruct *)shadow_; }
};

/*********************************************************************
* RWDropSiteEnterCallback : Class which encapsulates the X-Windows
* XmDropSiteEnterCallbackStruct.
*********************************************************************/

class RWDropSiteEnterCallback : public RWAnyCallback {
public:
  RWDropSiteEnterCallback (XmDropSiteEnterCallbackStruct * cbs)
        : RWAnyCallback ((XmAnyCallbackStruct *)cbs) {}
  ~RWDropSiteEnterCallback () {}

  Time           getTimeStamp()      { return shadow()->timeStamp; }
  unsigned char  getOperation()      { return shadow()->operation; }
  unsigned char  getOperations()     { return shadow()->operations; }
  unsigned char  getDropSiteStatus() { return shadow()->dropSiteStatus; }
  Position       getX()              { return shadow()->x; }
  Position       getY()              { return shadow()->y; }

private:
  XmDropSiteEnterCallbackStruct * shadow() 
        { return (XmDropSiteEnterCallbackStruct *)shadow_; }
};

/*********************************************************************
* RWDropSiteLeaveCallback : Class which encapsulates the X-Windows
* XmDropSiteLeaveCallbackStruct.
*********************************************************************/

class RWDropSiteLeaveCallback : public RWAnyCallback {
public:
  RWDropSiteLeaveCallback (XmDropSiteLeaveCallbackStruct * cbs)
        : RWAnyCallback ((XmAnyCallbackStruct *)cbs) {}
  ~RWDropSiteLeaveCallback () {}

  Time  getTimeStamp() { return shadow()->timeStamp; }

private:
  XmDropSiteLeaveCallbackStruct * shadow() 
        { return (XmDropSiteLeaveCallbackStruct *)shadow_; }
};

/*********************************************************************
* RWDropStartCallback : Class which encapsulates the X-Windows
* XmDropStartCallbackStruct.
*********************************************************************/

class RWDropStartCallback : public RWAnyCallback {
public:
  RWDropStartCallback (XmDropStartCallbackStruct * cbs)
        : RWAnyCallback ((XmAnyCallbackStruct *)cbs) {}
  ~RWDropStartCallback () {}

  Time           getTimeStamp()      { return shadow()->timeStamp; }
  unsigned char  getOperation()      { return shadow()->operation; }
  unsigned char  getOperations()     { return shadow()->operations; }
  unsigned char  getDropSiteStatus() { return shadow()->dropSiteStatus; }
  Position       getX()              { return shadow()->x; }
  Position       getY()              { return shadow()->y; }
  unsigned char  getDropAction ()    { return shadow()->dropAction; }

  void           setDropAction (unsigned char da) { shadow()->dropAction = da; }

private:
  XmDropStartCallbackStruct * shadow() 
        { return (XmDropStartCallbackStruct *)shadow_; }
};


/*********************************************************************
* RWTopLevelLeaveCallback : Class which encapsulates the X-Windows
* XmTopLevelLeaveCallbackStruct.
*********************************************************************/

class RWTopLevelLeaveCallback : public RWAnyCallback {
public:
  RWTopLevelLeaveCallback (XmTopLevelLeaveCallbackStruct * cbs)
        : RWAnyCallback ((XmAnyCallbackStruct *)cbs) {}
  ~RWTopLevelLeaveCallback () {}

  Time      getTimeStamp() { return shadow()->timeStamp; }
  Screen *  getScreen()    { return shadow()->screen; }
  Window    getWindow()    { return shadow()->window; }

private:
  XmTopLevelLeaveCallbackStruct * shadow() 
        { return (XmTopLevelLeaveCallbackStruct *)shadow_; }
};


/*********************************************************************
* RWTopLevelEnterCallback : Class which encapsulates the X-Windows
* XmTopLevelEnterCallbackStruct.
*********************************************************************/

class RWTopLevelEnterCallback : public RWAnyCallback {
public:
  RWTopLevelEnterCallback (XmTopLevelEnterCallbackStruct * cbs)
        : RWAnyCallback ((XmAnyCallbackStruct *)cbs) {}
  ~RWTopLevelEnterCallback () {}

  Time           getTimeStamp()         { return shadow()->timeStamp; }
  Screen *       getScreen()            { return shadow()->screen; }
  Window         getWindow()            { return shadow()->window; }
  Position       getX()                 { return shadow()->x; }
  Position       getY()                 { return shadow()->y; }
  unsigned char  getDragProtocolStyle() { return shadow()->dragProtocolStyle; }

private:
  XmTopLevelEnterCallbackStruct * shadow() 
        { return (XmTopLevelEnterCallbackStruct *)shadow_; }
};


#endif
