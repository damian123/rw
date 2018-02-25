#ifndef __RW_CONVERT_CB_H
#define __RW_CONVERT_CB_H
/*********************************************************************
*
*  $Id: ConvertCB.h,v 1.5 1993/09/30 19:40:10 keffer Exp $
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
*  File        : ConvertCB.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the declaration of a utility
*                object which is used to support firing global
*                functions as events on View.h++ object.
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

#include <rw/Xrw/App.h>
#include <rw/Xrw/DragCon.h>
#include <rw/Xrw/CBInfo.h>

  
/*********************************************************************
* RWEventCB : A utility object used to allow global functions to
* be fired as events on View.h++ objects.  
*********************************************************************/
  
class RWConvertCB : public RWCallbackInfo {
public:
  RWConvertCB
    (
     RWDragContext * dc,
     RWConvertPtr    cb,
     void *          client_data
    )
    : RWCallbackInfo (0, client_data),
      fp_(cb), mp_(0), dc_(dc)  {;}
  RWConvertCB
    (
     RWDragContext *    dc,
     RWApplication *    conto,
     RWConvertMethodPtr cb,
     void *             client_data
    )
    : RWCallbackInfo (conto, client_data),
      fp_(0), mp_(cb), dc_(dc)  {;}

  RWConvertPtr       getConvertPtr()     { return fp_; }
  RWConvertMethodPtr getConvertMethod () { return mp_; }
  RWApplication *    getObject()         { return controllerObj; }
  RWDragContext *    getDragContext ()   { return dc_; }

  void fireCallback (RWXtPointer) {;}

  Boolean RWConvertCB::fireConvertMethod
        (
	RWDragContext * dc,
	Atom *          selection,
	Atom *          target,
	Atom *          type_return,
	XtPointer *     value_return,
	unsigned long * length_return,
	int *           format_return,
	unsigned long * max_length,
	RWConvertCB *   cbcb,
	XtRequestId *   request_id
	 );

 private:
  RWConvertPtr       fp_;     // function to fire
  RWDragContext *    dc_;
  RWConvertMethodPtr mp_;     // method to fire

};

#endif
