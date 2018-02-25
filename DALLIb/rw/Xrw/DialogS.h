#ifndef __RW_DIALOG_SHELL_H
#define __RW_DIALOG_SHELL_H
/*********************************************************************
*
*  $Id: DialogS.h,v 1.8 1993/10/07 01:42:57 keffer Exp $
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
*  File        : DialogS.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Dialog Shell class.
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
*********************************************************************/

#include <rw/Xrw/TranShell.h>


/*********************************************************************
* RWDialogShell : Class object which represents the DialogShell
* widget class in X-Windows and the OSF/Motif widget set.
*********************************************************************/

class RWDialogShell : public RWTransientShell {
public:
  RWDialogShell ();
  RWDialogShell
    (
    RWController *    parent, 
    const RWCString & name, 
    RWBoolean         manage_it = False
    );
  RWDialogShell
    (
    RWController *, 
    const RWCString &,
    const RWResourceObj & pre_inst_resources,
    RWBoolean             managed = False
    );
  RWDialogShell
    (
    RWController *, 
    RWResourceObj * pre_inst_resources = 0
    );
  RWDialogShell
    (
    RWController *, 
    const RWCString &,
    ArgList    argv, 
    Cardinal   argc,
    RWBoolean  managed = False
    );
  RWDialogShell
    (
    Widget, 
    RWBoolean managed = False
    );  // Internal.
  ~RWDialogShell();

  void  createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const 
    {
    return __RWDIALOGSHELL;
    }
};

#endif  /* End of file DialogS.h - Do not add beyond this point! */
  
