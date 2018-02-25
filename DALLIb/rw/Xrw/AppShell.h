#ifndef __RW_APPLICATION_SHELL_H
#define __RW_APPLICATION_SHELL_H
/*********************************************************************
*  $Id: AppShell.h,v 1.13 1993/10/22 22:05:14 keffer Exp $
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
*  File        : AppShell.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Application Shell class.  This class is responsible
*                making the connection to the X server.  It will be
*                the first class instantiated in all user interfaces.
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

#include <rw/Xrw/TopShell.h>

/*********************************************************************
* RWApplicationShell : The toplevel class in all View.h++ user interfaces.
* Its constructor makes the connection to the X server.  The
* Toplevel widget, returned by XtInitialize, is a data member of
* this class.
*********************************************************************/

class RWApplicationShell  : public RWTopLevelShell
{
 public:
  RWApplicationShell (int, char **, XtAppContext * app = 0);
  RWApplicationShell (const RWCString &, int, char **, XtAppContext * app = 0);
  RWApplicationShell (const RWCString &, const RWCString &, int, char **, XtAppContext * app = 0);
  RWApplicationShell (const RWCString &, const RWCString &, Display *, ArgList, int);
  RWApplicationShell (const RWCString &, int, char **, XtAppContext *,
		      XrmOptionDescList options, int num_options = 0,
		      String * fallbacks = 0);
  ~RWApplicationShell ();

// RESOURCE MEMBER FUNCTIONS
  void 	   setArgc (int);
  int 	   getArgc (int *val = 0);
  void 	   setArgv (String *);
  String * getArgv (String **val = 0);

#if RWX11Version>=5
// XmDisplay RESOURCE MEMBER FUNCTIONS
#if 0
  String         getDefaultVirtualBindings(String *val = 0);
#endif
  unsigned char  getDragInitiatorProtocolStyle(unsigned char *val = 0);
  unsigned char  getDragReceiverProtocolStyle(unsigned char *val = 0);
#endif
  
  void 	   manage ();

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const { return __RWAPPLICATIONSHELL; }

#if RWX11Version>=5
private:
  Widget xmdisplay;
#endif

};

#endif  /* End of file AppShell.h - Do not add beyond this point! */
