#ifndef __RWAPPDEFS_H__
#define __RWAPPDEFS_H__

/*********************************************************************
*
*  $Id: appdefs.h,v 1.7 1994/01/11 00:23:12 cynthia Exp $
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
*  File        : appdefs.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ general declarations for
*                the application class source code
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#ifndef __RWVIEWDEFS_H__
# include <rw/Xrw/viewdefs.h>
#endif

#include <Xm/Xm.h>

/************************************************************************
* RWDialog  enum(s)
************************************************************************/

enum RWButtonsShown {
  OKButton                         = 1, 
  CancelButton                     = 2, 
  ApplyButton                      = 4,
  OtherButton                      = 8
  };

enum RWButtonConfig {
  RWOkButton                       = OKButton,
  RWOkCancelButton                 = OKButton | CancelButton,
  RWOkApplyCancelButton            = OKButton | ApplyButton  | CancelButton,
  RWOkApplyCancelOtherButton       = OKButton | ApplyButton  |
                                                CancelButton | OtherButton,
  RWOkOtherButton                  = OKButton | OtherButton,
  RWOkCancelOtherButton            = OKButton | CancelButton | OtherButton,
  RWNoButton                       = 0
  };

enum RWDialogReply {
  RWOk                             = 0, 
  RWCancel, 
  RWApply, 
  RWOther,
  RWIgnore
  };

enum RWDialogStyle {
  RWModalDialog                    = 1,   //modal-synchronous
  RWAsyncModalDialog,                     //modal-asynchronous
  RWNonModalDialog                        //non-modal-asynchronous
  };


/************************************************************************
* RWButtonDialog  enum(s)
************************************************************************/

enum RWButtonStyle {
  RWRadioButtonStyle,
  RWToggleButtonStyle
  };
  

/************************************************************************
* Label enum(s)
************************************************************************/

enum RWLabelAlignment {
  RWAlignmentBeginning             = XmALIGNMENT_BEGINNING,           // 0
  RWAlignmentCenter                = XmALIGNMENT_CENTER,              // 1
  RWAlignmentEnd                   = XmALIGNMENT_END                  // 2
  };

enum RWLabelType {
  RWPixmap                         = XmPIXMAP,                        // 1
  RWString                         = XmSTRING                         // 2
  };


/************************************************************************
* Menu/RowColumn enum(s)
************************************************************************/

enum RWOrientation {
  RWNoOrientation                  = XmNO_ORIENTATION,                // 0
  RWVertical                       = XmVERTICAL,                      // 1
  RWHorizontal                     = XmHORIZONTAL                     // 2
  };


/************************************************************************
* List enum(s) and variables
************************************************************************/

static const int RWDefaultVisibleItemCount = -1;

enum RWSelectionPolicy {
  RWSingleSelect                   = XmSINGLE_SELECT,                 // 0
  RWMultipleSelect                 = XmMULTIPLE_SELECT,               // 1
  RWExtendedSelect                 = XmEXTENDED_SELECT,               // 2
  RWBrowseSelect                   = XmBROWSE_SELECT,                 // 3
  RWDefaultSelect                  = 256
  };


#endif
