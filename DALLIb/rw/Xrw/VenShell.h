#ifndef __RW_VENDOR_SHELL_H
#define __RW_VENDOR_SHELL_H
/*********************************************************************
*
*  $Id: VenShell.h,v 1.9 1993/10/22 22:05:14 keffer Exp $
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
*  File        : VenShell.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                VendorShell class.  This C++ class represents both
*                OSF/Motif widget classes WMShell and VendorShell.
*                From a C++ perspective, there was no need to
*                implement both of these because they are not directly
*                instantiated.  Subclasses merely need access to their
*                resources; the proper inheritance of which are handled
*                by Motif.  Member functions to access the combined
*                WMShell and VendorShell resources are provided on
*                this class object.
*                
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

#include <rw/Xrw/Shell.h>

/*********************************************************************
* The following definitions are borrowed from MwmUtil.h so that the 
* whole file does not have to be included.  (Really want to avoid
* the Xmd.h warning!)
*********************************************************************/

/* bit definitions for MwmHints.flags */
#define MWM_HINTS_FUNCTIONS	(1L << 0)
#define MWM_HINTS_DECORATIONS	(1L << 1)
#define MWM_HINTS_INPUT_MODE	(1L << 2)
#define MWM_HINTS_STATUS	(1L << 3)

/* bit definitions for MwmHints.functions */
#define MWM_FUNC_ALL		(1L << 0)
#define MWM_FUNC_RESIZE		(1L << 1)
#define MWM_FUNC_MOVE		(1L << 2)
#define MWM_FUNC_MINIMIZE	(1L << 3)
#define MWM_FUNC_MAXIMIZE	(1L << 4)
#define MWM_FUNC_CLOSE		(1L << 5)

/* bit definitions for MwmHints.decorations */
#define MWM_DECOR_ALL		(1L << 0)
#define MWM_DECOR_BORDER	(1L << 1)
#define MWM_DECOR_RESIZEH	(1L << 2)
#define MWM_DECOR_TITLE		(1L << 3)
#define MWM_DECOR_MENU		(1L << 4)
#define MWM_DECOR_MINIMIZE	(1L << 5)
#define MWM_DECOR_MAXIMIZE	(1L << 6)

/* values for MwmHints.input_mode */
#define MWM_INPUT_MODELESS			0
#define MWM_INPUT_PRIMARY_APPLICATION_MODAL	1
#define MWM_INPUT_SYSTEM_MODAL			2
#define MWM_INPUT_FULL_APPLICATION_MODAL	3

/* bit definitions for MwmHints.status */
#define MWM_TEAROFF_WINDOW	(1L << 0)

/*********************************************************************
* RWVendorShell : Class object which represents the VendorShell
* widget in the OSF/Motif widget set.  This is an internal class
* which is not meant to be instantiated directly.
*********************************************************************/

class RWVendorShell : public RWShell
{
 public:
  RWVendorShell();       // Internal.
  RWVendorShell
    (
    Widget,
    Boolean managed = False
    ); // Internal.
  ~RWVendorShell();

// RESOURCE MEMBER FUNCTIONS
//   From WMShell
  void            setBaseHeight		(int);
  int             getBaseHeight		(int *val = 0);
  void            setBaseWidth		(int);
  int             getBaseWidth		(int *val = 0);
  void            setHeightInc		(int);
  int             getHeightInc		(int *val = 0);
  void            setIconMask		(Pixmap);
  Pixmap          getIconMask		(Pixmap *val = 0);
  void            setIconPixmap		(Pixmap);
  Pixmap          getIconPixmap		(Pixmap *val = 0);
  void            setIconWindow		(Window);
  Window          getIconWindow		(Window *val = 0);
  void            setIconWindowC	(RWController *);
  RWController *  getIconWindowC	(RWController **val = 0);
  void            setIconX		(int);
  int             getIconX		(int *val = 0);
  void            setIconY		(int);
  int             getIconY		(int *val = 0);
  void            setInitialState	(int);
  int             getInitialState	(int *val = 0);
  void            setInput		(Boolean);
  Boolean         getInput		(Boolean *val = 0);
  void            setMaxAspectX		(int);
  int             getMaxAspectX		(int *val = 0);
  void            setMaxAspectY		(int);
  int             getMaxAspectY		(int *val = 0);
  void            setMaxHeight		(int);
  int             getMaxHeight		(int *val = 0);
  void            setMaxWidth		(int);
  int             getMaxWidth		(int *val = 0);
  void            setMinAspectX		(int);
  int             getMinAspectX		(int *val = 0);
  void            setMinAspectY		(int);
  int             getMinAspectY		(int *val = 0);
  void            setMinHeight		(int);
  int             getMinHeight		(int *val = 0);
  void            setMinWidth		(int);
  int             getMinWidth		(int *val = 0);
  void            setTitle		(char *);
  char *          getTitle		(char **val = 0);
  void            setTitleEncoding	(Atom);
  Atom            getTitleEncoding	(Atom *val = 0);
  void            setTransient		(Boolean);
  Boolean         getTransient		(Boolean *val = 0);
  void            setWaitForWm		(Boolean);
  Boolean         getWaitForWm		(Boolean *val = 0);
  void            setWidthInc		(int);
  int             getWidthInc		(int *val = 0);
  void            setWindowGroup	(Window);
  Window          getWindowGroup	(Window *val = 0);
  void            setWinGravity		(int);
  int             getWinGravity		(int *val = 0);
  void            setWmTimeout		(int);
  int             getWmTimeout		(int *val = 0);

//   From VendorShell
#if RWX11Version>=5
  void            setAudibleWarning     (unsigned char);
  unsigned char   getAudibleWarning     (unsigned char *val = 0);
#endif
  void            setDeleteResponse	(unsigned char);
  unsigned char   getDeleteResponse	(unsigned char *val = 0);
  void            setKeyboardFocusPolicy(unsigned char);
  unsigned char   getKeyboardFocusPolicy(unsigned char *val = 0);
  void            setMwmDecorations	(int);
  int             getMwmDecorations	(int *val = 0);
  void            setMwmFunctions	(int);
  int             getMwmFunctions	(int *val = 0);
  void            setMwmInputMode	(int);
  int             getMwmInputMode	(int *val = 0);
  void            setMwmMenu		(String);
  String          getMwmMenu		(String *val = 0);
  void            setShellUnitType	(unsigned char);
  unsigned char   getShellUnitType	(unsigned char *val = 0);
  void            setUseAsyncGeometry	(Boolean);
  Boolean         getUseAsyncGeometry	(Boolean *val = 0);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWVENDORSHELL;}
  
 private:
};

#endif  /* End of file VenShell.h - Do not add beyond this point! */
