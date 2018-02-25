#ifndef __RW_DYNARGARR_H
#define __RW_DYNARGARR_H
/*********************************************************************
*
*  $Id: DynArgArr.h,v 1.7 1993/10/22 22:21:34 keffer Exp $
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
*  File        : DynArgArr.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : October 1991
*
*  Project     : View.h++
*
*  Description : This file contains the declaration of a Dynamic
*                Argument Array of Arg elements.  It is used
*                internally to facilitate resource argument
*                buffering.  It is not intended for use by the
*                end user.
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

#include <rw/Xrw/viewdefs.h>
STARTWRAP
#include <string.h>
ENDWRAP

#include <X11/Xutil.h>
#if RWX11Version>=5
#include <X11/Xosdefs.h>
#else
#include <X11/Xos.h>
#endif
#include <Xm/Xm.h>

  
/*********************************************************************
* #defines
*********************************************************************/

#define MAX_ARG_ARRAY_SIZE 100


/*********************************************************************
* RWDynamicArgArray : Dynamic resource argument array.  Used
* internally to facilitate resource argument buffering.
*********************************************************************/
  
class RWDynamicArgArray
{
 public:
  RWDynamicArgArray                // Constructor.
    (
     unsigned asize = 1
    );
  RWDynamicArgArray                // Copy Constructor.
    (
     const RWDynamicArgArray &
    );
  RWDynamicArgArray& operator=     // Assignment Operator.
    (
     const RWDynamicArgArray &
    );

  ~RWDynamicArgArray();            // Destructor.

  long     length     ();               // Return the "length" of the array.
  Boolean  resize     (unsigned);       // Grow/shrink the array.
  Arg &    operator[] (unsigned index); // Return Arg element or resize.
  Arg *    operator&  () const;         // Return address of array.

 private:
  unsigned numElements;            // Number of elements in argArray.
  Arg *    argArray;               // Array of Arg structures.
  Arg      dummy;                  // Return on error condition.
};

#endif  /* End of file DynArgArr.h - Do not add beyond this point! */ 
