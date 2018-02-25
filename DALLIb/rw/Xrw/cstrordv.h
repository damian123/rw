#ifndef __RW_CSTRORDV_H
#define __RW_CSTRORDV_H

/*********************************************************************
*
*  $Id: cstrordv.h,v 1.7 1994/06/07 05:05:22 alv Exp $
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
*  File        : cstrordv.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains general declarations for View.h++
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#include <rw/Xrw/viewdefs.h>
#include <rw/cstring.h>

/*
 * The following evaluates true every time.  It is possible to use 
 * templates with compilers (like the IBM xlC compiler) that can
 * instantiate from a library, but this makes life much more difficult
 * when porting because now we need to supply a version of Tools.h++ which
 * includes the template classes.  So why bother?
 */
#if 1 || defined(RW_NO_TEMPLATES) || defined(RW_NO_LIBRARY_INSTANTIATE)
# include <rw/gordvec.h>
  declare(RWGVector,RWCString)
  declare(RWGOrderedVector,RWCString)
  typedef RWGOrderedVector(RWCString) RWCStringOrderedVec;
#else
# include <rw/tvordvec.h>
  typedef RWTValOrderedVector<RWCString> RWCStringOrderedVec;
#endif /* RW_NO_TEMPLATES */

#endif /* cstrordv.h */
