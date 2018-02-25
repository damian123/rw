#ifndef __RW_CONFIRMH
#define __RW_CONFIRMH

/*********************************************************************
*
*  $Id: Confirm.h,v 1.11 1994/01/10 23:52:01 cynthia Exp $
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
*  File        : Confirm.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definiton for a dialog
*                that asks the user a Yes or No question
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

/*
 * RWConfirmDialog
 *
 * $Id: Confirm.h,v 1.11 1994/01/10 23:52:01 cynthia Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 * Voice: (503) 754-3010	FAX: (503) 757-6650
 *
 * Copyright (C) 1993.
 * This software is subject to copyright protection under the laws of
 * the United States and other countries.
 *
 ***************************************************************************
 *
 */

#include <rw/Xrw/Notify.h>

class RWConfirmDialog : public RWNotifyDialog {

public:
  RWConfirmDialog(RWView        *parentView, 
		  const char    *name    = "RWConfirmDialog",
		  RWButtonConfig buttons = RWOkCancelButton);
  virtual ~RWConfirmDialog();

  virtual RWDialogReply getSelection () const;

  virtual RWClassID isA() const { return(__RWCONFIRMDIALOG); }

private:
  virtual void setIcon (RWNotifyStyle  ns,
			const char*    fg = rwnil,
			const char*    bg = rwnil)
	 { RWNotifyDialog::setIcon(ns, fg, bg); }
  
  
};

#endif
