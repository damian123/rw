#ifndef __RW_ERROR_H
#define __RW_ERROR_H
/*********************************************************************
*
*  $Id: Error.h,v 1.7 1993/10/07 01:42:57 keffer Exp $
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
*  File        : Error.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : February 1991
*
*  Project     : View.h++
*
*  Description : This file contains the C++ #defines and class
*                definitions for View.h++ class error handling.
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

#include <rw/compiler.h>

#include <rw/Xrw/App.h>
#include <rw/Xrw/TypeId.h>
#include <rw/rstream.h>

/*********************************************************************
* #defines
*********************************************************************/

#define MAX_ERRORS                11
#define COMMAND_GET_CHILD         0
#define FILE_SB_GET_CHILD         1
#define MESSAGE_BOX_GET_CHILD     2
#define SELECTION_BOX_GET_CHILD   3
#define CREATE_OBJECT_FAILURE     4
#define INVALID_COLOR_NAME        5
#define CANT_ALLOCATE_COLOR       6
#define RESOURCE_BUFFER_OVERFLOW  7
#define NO_RESOURCE_BUFFER        8
#define OUT_OF_MEMORY             9
#define DYN_ARG_ARRAY_OVERFLOW    10


static const char * const error[MAX_ERRORS] = {
  "invalid 'child' argument to function 'commandGetChild'.",
  "invalid 'child' argument to function 'fileSelectionBoxGetChild'.",
  "invalid 'child' argument to function 'messageBoxGetChild'.",
  "invalid 'child' argument to function 'selectionBoxGetChild'.",
  "invalid 'widget' argument to internal function 'widgetToControllerObj'.",
  "invalid color name in function 'strToPix'.",
  "function 'strToPix' cannot allocate color in colormap.",
  "resource buffer overflow.  Use 'setValues' / 'getValues' to set the resource values and flush the resource buffer.",
  "attempted to set more than one resource value without a resource buffer.  Use 'setBufferResources(True);' to create a resource buffer.",
  "out of memory!  Unable to allocate resource argument array.",
  "can't grow dynamic argument array!  Array size has reached maximum size."
  };


/*********************************************************************
* RWErrorMess : Class that implements error handling for all View.h++ class
* objects.
*********************************************************************/

class RWErrorMess : public RWApplication {
public:
  static int  message (const int);
  
// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWERRORMESS;}
};
  
#endif  /* End of file Error.h - Do not add beyond this point! */


