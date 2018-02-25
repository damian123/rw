#ifndef __RWDEFCOL_H__
#define __RWDEFCOL_H__

/*
 * This header file is used internally by Tools.h++.  When compiling
 * Tools.h++ as a DLL or shared lib, it does not insert a creator 
 * function into the Factory.  Otherwise it does.  
 *
 * $Id: defcol.h,v 6.3 1994/07/12 21:14:53 vriezen Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 *
 * (c) Copyright 1989, 1990, 1991, 1992, 1993, 1994 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 52.227-7013.
 * 
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 ***************************************************************************
 *
 *
 ***************************************************************************
 *
 * $Log: defcol.h,v $
 * Revision 6.3  1994/07/12  21:14:53  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/06/15  17:14:08  jims
 * Use DLL version of macro for shared libs as well
 *
 * Revision 6.1  1994/04/15  19:06:23  vriezen
 * Move all files to 6.1
 *
 * Revision 2.2  1993/08/05  11:47:26  jims
 * Distinguish between WIN16 DLLs and WIN32 DLLs by checking
 * for __WIN16__
 *
 * Revision 2.1  1993/03/13  02:26:58  keffer
 * ClassID->RWClassID
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.3   27 May 1992 18:28:36   KEFFER
 * Uses new V5.1 RWDEFINE_COLLECTABLE.
 * 
 *    Rev 1.2   13 Nov 1991 11:54:02   keffer
 * Added destructor for Init(className)
 * 
 *    Rev 1.1   28 Oct 1991 09:24:04   keffer
 * Include file path now <rw/xxx.h>
 * 
 */

#if defined(_RWBUILDDLL) || defined(_RWBUILDSHARED) 

#define RWDEFINE_COLLECTABLE2(className, id)		\
RWCollectable* className::copy() const			\
{ return new className(*this); }			\
RWClassID className::isA() const			\
{ return id; }						\
RWCollectable* className::newSpecies() const		\
{ return new className; }

#else	/* neither DLL nor shared */

/*
 * If not compiling for a DLL or shared lib, use the regular version:
 */
#define RWDEFINE_COLLECTABLE2(className, id)		\
RWDEFINE_COLLECTABLE(className, id)

#endif	/* __DLL__ */

#endif
