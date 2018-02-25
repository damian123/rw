#ifndef __RW_TOP_LEVEL_SHELL_H
#define __RW_TOP_LEVEL_SHELL_H
/*********************************************************************
*
*  $Id: TopShell.h,v 1.8 1994/06/14 18:18:22 steves Exp $
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
*  File        : TopShell.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                TopLevel Shell class.
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

#include <rw/Xrw/VenShell.h>


/*********************************************************************
* RWTopLevelShell : Class object which represents the TopLevelShell
* widget class in X-Windows and the OSF/Motif widget set.
*********************************************************************/

class RWTopLevelShell : public RWVendorShell
{
 public:
  RWTopLevelShell();
  RWTopLevelShell
    (
    RWController *,
    const RWCString &,
    Boolean managed = False
    );
  RWTopLevelShell
    (
    RWController *,
    const RWCString &,
    const RWResourceObj &,
    Boolean managed = False
    );
  RWTopLevelShell
    (
    RWController *,
    const RWCString &,
    ArgList,
    Cardinal,
    Boolean managed = False
    );
  RWTopLevelShell
    (
    const RWCString &,
    const RWCString &,
    Display *,
    ArgList,
    Cardinal,
    Boolean managed = False
    );
  RWTopLevelShell
    (
    Widget,
    Boolean managed = False
    );  // Internal.
  RWTopLevelShell
    (
    RWController * myParent,
    RWResourceObj * resObj = NULL
    ) { beginWidget (myParent, resObj); }
  ~RWTopLevelShell();

// RESOURCE MEMBER FUNCTIONS
  void        setIconic           (Boolean);
  Boolean     getIconic           (Boolean *val = 0);
  void        setIconName         (const RWCString &);
  RWCString   getIconName         (RWCString *val = 0);
  void        setIconNameEncoding (Atom);
  Atom        getIconNameEncoding (Atom *val = 0);
  void        createWidget        (char *, RWBoolean managed = False);
  
// XT MEMBER FUNCTIONS
  virtual void manage();
  virtual void unmanage();

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWTOPLEVELSHELL;}
};

#endif  /* End of file TopShell.h - Do not add beyond this point! */
  
