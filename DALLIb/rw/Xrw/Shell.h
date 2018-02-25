#ifndef __RW_SHELL_H
#define __RW_SHELL_H
/*********************************************************************
*
*  $Id: Shell.h,v 1.8 1994/06/14 18:17:10 steves Exp $
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
*  File        : Shell.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Shell class.
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

#ifdef RW_UNREASONABLE_NESTING
#include <rw/hashtab.h>
#include <rw/ordcltn.h>
#include <sys/file.h>
#include <sys/time.h>
#include <stdio.h>
#endif

#include <rw/Xrw/Composite.h>


/*********************************************************************
* #define to choose caddr_t or XtPointer.  Some Protocols.h files
* use XtPointer instead of caddr_t.
*********************************************************************/

#ifdef __PROTOCOLS_NEED_XTPOINTER__
#define CLOSURE_TYPE XtPointer
#else
#define CLOSURE_TYPE caddr_t
#endif


/*********************************************************************
* RWShell : Class object which represents the Shell
* widget in the OSF/Motif widget set.  This is an internal class
* which is not meant to be instantiated directly.
*********************************************************************/

class RWShell : public RWComposite
{
 public:
  RWShell();         // Internal.  
  RWShell
    (
    Widget,
    Boolean managed = False
    );   // Internal.
  ~RWShell();

// RESOURCE MEMBER FUNCTIONS
  void        setAllowShellResize	(Boolean);
  Boolean     getAllowShellResize	(Boolean *val = 0);
  void        setCreatePopupChildProc	(XtOrderProc);
  XtOrderProc getCreatePopupChildProc	(XtOrderProc *val = 0);
  void        setGeometry		(String);
  String      getGeometry		(String *val = 0);
  void        setOverrideRedirect	(Boolean);
  Boolean     getOverrideRedirect	(Boolean *val = 0);
  void        setPopdownCallback 	(RWXtCallbackProc, RWXtPointer);
  void        setPopdownMethod 		(RWApplication *, RWMethodPtr, void *);
  void        setPopdownCallback 	(XtCallbackList);
  void        setPopupCallback 		(RWXtCallbackProc, RWXtPointer);
  void        setPopupMethod 		(RWApplication *, RWMethodPtr, void *);
  void        setPopupCallback 		(XtCallbackList);
  void        setSaveUnder		(Boolean);
  Boolean     getSaveUnder		(Boolean *val = 0);
  void        setVisual			(Visual *);
  Visual *    getVisual			(Visual **val = 0);

// MOTIF MEMBER FUNCTIONS
  void        activateProtocol		(Atom, Atom);
  void        activateWMProtocol	(Atom);
  void        addProtocolCallback	(Atom, Atom, XtCallbackProc, CLOSURE_TYPE);
  void        addProtocols		(Atom, Atom *, Cardinal);
  void        addWMProtocols		(Atom *, Cardinal);
  void        addWMProtocolCallback	(Atom, XtCallbackProc, CLOSURE_TYPE);
  void        deactivateProtocol	(Atom, Atom);
  void        deactivateWMProtocol	(Atom);
  void        removeProtocolCallback	(Atom, Atom, XtCallbackProc, CLOSURE_TYPE);
  void        removeProtocols		(Atom, Atom *, Cardinal);
  void        removeWMProtocolCallback	(Atom, XtCallbackProc, CLOSURE_TYPE);
  void        removeWMProtocols		(Atom *, Cardinal);
  void        setProtocolHooks		(Atom, Atom, XtCallbackProc, CLOSURE_TYPE,
					 XtCallbackProc, CLOSURE_TYPE);
  void        setWMProtocolHooks	(Atom, XtCallbackProc, CLOSURE_TYPE,
					 XtCallbackProc, CLOSURE_TYPE);
  Boolean     isMotifWMRunning          ();
  
// XT MEMBER FUNCTIONS
  virtual void manage();
  virtual void unmanage();
  
// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWSHELL;}
  
 private:
};

#endif  /* End of file Shell.h - Do not add beyond this point! */
