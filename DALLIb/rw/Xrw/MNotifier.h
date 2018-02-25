#ifndef __RW_MOUSENOTIFIERH
#define __RW_MOUSENOTIFIERH

/*********************************************************************
*
*  $Id: MNotifier.h,v 1.6 1994/01/11 00:05:29 cynthia Exp $
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
*  File        : MNotifier.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class declaration for 
*                MouseWrapper notification
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#include <rw/collect.h>
#include <rw/Xrw/App.h>

class RWMouseNotifier : public RWCollectable {
public:
  RWMouseNotifier
    (
     RWApplication * object, 
     RWWrapMethod    method,
     int             button,
     RWXtPointer     data = 0
     )
     : obj(object), meth(method), mouseButton(button), userData(data) {}

  virtual ~RWMouseNotifier() {}

  RWBoolean operator==
    (
     const RWCollectable *c
    )
    { return(isEqual(c)); }

  inline virtual RWBoolean isEqual
    (
     const RWCollectable *c
    ) const;

  const int       getMouseState ()  { return(mouseButton); }
  void            fireMethod ()     { (obj->*meth)(userData); }
  virtual RWClassID isA() const       { return(__RWMOUSENOTIFIER); }
  
protected:
  int              mouseButton;
  RWApplication *  obj;
  RWWrapMethod     meth;
  RWXtPointer      userData;

};

/****
*   Test equality, really
****/

inline RWBoolean RWMouseNotifier::isEqual
        (
	const RWCollectable *c
	) const
{
  const RWMouseNotifier *note = (const RWMouseNotifier*)c;
  
  /*For now, we ignore data for comparisons*/
  return ((obj == note->obj) && 
	  (meth == note->meth) &&
	  (mouseButton == note->mouseButton));

}


#endif /*__RW_MOUSENOTIFIERH*/
