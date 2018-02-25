#ifndef __RW_DROP_SITE_CB_H
#define __RW_DROP_SITE_CB_H
/*********************************************************************
*
*  $Id: DropSiteCB.h,v 1.7 1993/09/30 19:40:10 keffer Exp $
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
*  File        : DropSiteCB.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : January 1993
*
*  Project     : View.h++
*
*  Description : This file contains the declaration of an View.h++ class
*                object which encapsulates the Drag and Drop callback
*                structures
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
#include <rw/Xrw/DragCon.h>  
#include <Xm/DropSMgr.h>

/*********************************************************************
* RWDragProcCallback : Class which encapsulates the X-Windows
* XmDragProcCallbackStruct.
*********************************************************************/

class RWDragProcCallback : public RWAnyCallback {
public:
  RWDragProcCallback (XmDragProcCallbackStruct * cbs)
        : RWAnyCallback ((XmAnyCallbackStruct *)cbs) {}
  ~RWDragProcCallback () {}

  Time            getTimeStamp()      { return shadow()->timeStamp; }
  unsigned char   getOperation()      { return shadow()->operation; }
  unsigned char   getOperations()     { return shadow()->operations; }
  unsigned char   getDropSiteStatus() { return shadow()->dropSiteStatus; }
  Position        getX()              { return shadow()->x; }
  Position        getY()              { return shadow()->y; }
  RWDragContext * getDragContext()    { return new RWDragContext (shadow()->dragContext); }
  Boolean         getAnimate()        { return shadow()->animate; }

  void setDropSiteStatus (unsigned char dss) { shadow()->dropSiteStatus = dss; }
  void setOperation (unsigned char op)       { shadow()->operation = op; }
  void setOperations (unsigned char ops)     { shadow()->operations = ops; }
  void setAnimate (Boolean b)                { shadow()->animate = b; }
  
private:
  XmDragProcCallbackStruct * shadow() 
        { return (XmDragProcCallbackStruct *)shadow_; }
};

/*********************************************************************
* RWDropProcCallback : Class which encapsulates the Motif
* XmDropProcCallbackStruct.
*********************************************************************/

class RWDropProcCallback : public RWAnyCallback {
public:
  RWDropProcCallback (XmDropProcCallbackStruct * cbs)
        : RWAnyCallback ((XmAnyCallbackStruct *)cbs) {}
  ~RWDropProcCallback () {}

  Time            getTimeStamp()      { return shadow()->timeStamp; }
  unsigned char   getOperation()      { return shadow()->operation; }
  unsigned char   getOperations()     { return shadow()->operations; }
  unsigned char   getDropSiteStatus() { return shadow()->dropSiteStatus; }
  Position        getX()              { return shadow()->x; }
  Position        getY()              { return shadow()->y; }
  RWDragContext * getDragContext()    { return new RWDragContext (shadow()->dragContext); }
  unsigned char   getDropAction()     { return shadow()->dropAction; }

  void setDropSiteStatus (unsigned char dss) { shadow()->dropSiteStatus = dss; }
  void setOperation (unsigned char op)       { shadow()->operation = op; }
  void setOperations (unsigned char ops)     { shadow()->operations = ops; }
  void setDropAction (unsigned char da)      { shadow()->dropAction = da; }
  
private:
  XmDropProcCallbackStruct * shadow() 
        { return (XmDropProcCallbackStruct *)shadow_; }
};


#endif
