#ifndef __RW_CLIPBOARD_H
#define __RW_CLIPBOARD_H
/*********************************************************************
*
*  $Id: ClipBD.h,v 1.10 1993/10/22 22:05:14 keffer Exp $
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
*  File        : ClipBD.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                RWClipboard class.
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

#include <rw/Xrw/Control.h>
  
/*********************************************************************
* RWClipBoard : Class object that encapsulates the XmClipboard
* functions of OSF/Motif.
*********************************************************************/

class RWClipboard
{
 public:
  RWClipboard (Display *, Window);
  ~RWClipboard();

// MOTIF MEMBER FUNCTIONS
#if RWX11Version>=5
  int    copy                (long, const RWCString &, XtPointer, unsigned long, long, long *);
  int    copyByName          (long, XtPointer, unsigned long, long);
  int    inquireCount        (int *, unsigned long *);
  int    inquireFormat       (int, XtPointer, unsigned long, unsigned long *);
  int    registerFormat      (const RWCString &, int);
  int    retrieve            (const RWCString &,RWXtPointer,unsigned long,unsigned long *,long *);
  int    startCopy           (RWXmString *, Time, RWController *, XmCutPasteProc, long *);
  int    startCopy           (const RWCString &, Time, RWController *, XmCutPasteProc, long *);
  int    withdrawFormat      (long);
#else
  int    copy                (long, const RWCString &, RWXtPointer, unsigned long, int, int *);
  int    copyByName          (int, char *, unsigned long, int);
  int    inquireCount        (int *, int *);
  int    inquireFormat       (int, char *, unsigned long, unsigned long *);
  int    registerFormat      (const RWCString &, unsigned long);
  int    retrieve            (const RWCString &, char *, unsigned long, unsigned long *, int *);
#ifdef hpux
  int    startCopy           (RWXmString *, Time, RWController *, XmCutPasteProc, long *);
  int    startCopy           (const RWCString &, Time, RWController *, XmCutPasteProc, long *);
#else
  int    startCopy           (RWXmString *, Time, RWController *, VoidProc, long *);
  int    startCopy           (const RWCString &, Time, RWController *, VoidProc, long *);
#endif
  int    withdrawFormat      (int);
#endif
  void   cancelCopy          (long);
  int    endCopy             (long);
  int    endRetrieve         ();
  int    inquireLength       (const RWCString &, unsigned long *);
  int    inquirePendingItems (const RWCString &, XmClipboardPendingList *, unsigned long *);
  int    lock                ();
  int    startRetrieve       (Time);
  int    undoCopy            ();
  int    unlock              (Boolean);

// RW TOOLS MEMBER FUNCTIONS
//  virtual RWClassID isA () const {return __RWCLIPBOARD;}
 
 private:
  Display * display;
  Window    win;
};

#endif  /* End of file ClipBD.h - Do not add beyond this point! */
