#ifndef __RW_TEXT_VERIFY_CB_H
#define __RW_TEXT_VERIFY_CB_H
/*********************************************************************
*
*  $Id: TextVCB.h,v 1.8 1993/10/07 01:42:57 keffer Exp $
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
*  File        : TextVCB.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : April 1991
*
*  Project     : View.h++
*
*  Description : This file contains the declaration of an View.h++ class
*                object which encapsulates the
*                XmTextVerifyCallbackStruct.
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
#include <Xm/Text.h>

/*********************************************************************
* RWTextVerifyCallback : Class which encapsulates the X-Windows
* XmTextVerifyCallbackStruct.
*********************************************************************/

class RWTextVerifyCallback : public RWAnyCallback
{
 public:
  RWTextVerifyCallback (XmTextVerifyCallbackStruct * cbs)
        : RWAnyCallback ((XmAnyCallbackStruct *)cbs) {}
  ~RWTextVerifyCallback() {}

  Boolean        getDoit()       { return shadow()->doit; }
  XmTextPosition getCurrInsert() { return shadow()->currInsert; }
  XmTextPosition getNewInsert()  { return shadow()->newInsert; }
  XmTextPosition getStartPos()   { return shadow()->startPos; }
  XmTextPosition getEndPos()     { return shadow()->endPos; }
  XmTextBlock    getText()       { return shadow()->text; }

  void setDoit       (Boolean b)        { shadow()->doit = b; }
  void setStartPos   (XmTextPosition p) { shadow()->startPos = p; }
  void setEndPos     (XmTextPosition p) { shadow()->endPos = p; }
  void setNewInsert  (XmTextPosition p) { shadow()->newInsert = p; }
  void setTextPtr    (char * ptr)       { shadow()->text->ptr = ptr; }
  void setTextLength (int len)          { shadow()->text->length = len; }
  void setTextFormat (XmTextFormat f)   { shadow()->text->format = f; }

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWTEXTVERIFYCALLBACK;}
  
private:
  XmTextVerifyCallbackStruct * shadow() 
        { return (XmTextVerifyCallbackStruct *)shadow_; }
};
#endif
