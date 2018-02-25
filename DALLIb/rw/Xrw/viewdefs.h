#ifndef __RWVIEWDEFS_H__
#define __RWVIEWDEFS_H__

/*********************************************************************
*
*  $Id: viewdefs.h,v 1.16 1994/01/11 00:26:32 cynthia Exp $
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
*  File        : viewdefs.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains general C++ definitions for View.h++
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#ifndef __RWDEFS_H__
#  include "rw/defs.h"
#endif
#include <X11/Xlib.h>	/* Looking for X11 release number */

#if defined(XlibSpecificationRelease)
#  define RWX11Version XlibSpecificationRelease
#else
#  define RWX11Version 4
#endif

#define RWVIEW 0x0121	/* Version number */

#ifdef __ATT3__
/* Define this if the template instantiation mechanism is unable
   to instantiate a template used entirely within the library
   and not in the user code. */
#  define RW_NO_LIBRARY_INSTANTIATE 1
#endif

#ifndef XTFUNCPROTO
#  define XTFUNCPROTO
#endif

#ifndef XK_MISCELLANY
#  define XK_MISCELLANY
#endif

typedef void * RWXtPointer;
#ifdef RW_NEEDS_STRICT_TYPES
class RWController;
class RWCallback;
typedef void (*RWXtCallbackProc)(RWController *, RWXtPointer, RWCallback *);
#else
typedef void * RWXtCallbackProc;
#endif

#if defined(__CLCC__) && defined(RW_SUNOS)
/******
 * CenterLine forgot to put this definition in the SunOS version
 * of their C++ headers
 ******/
#define MB_CUR_MAX 1
#endif

#endif
