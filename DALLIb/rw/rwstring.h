#ifndef __RWSTRING_H__
#define __RWSTRING_H__

/*
 * RWString: Backwards compatibility declaration
 *
 * $Id: rwstring.h,v 6.4 1994/07/18 06:17:36 jims Exp $
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
 * This file is provided for backwards compatibility.  Note, however, that
 * the copy constructor and assignment operator of RWString used reference
 * semantics while the new RWCString uses copy semantics.
 *
 ***************************************************************************
 *
 * $Log: rwstring.h,v $
 * Revision 6.4  1994/07/18  06:17:36  jims
 * Back out previous change; update test suite instead.
 *
 * Revision 6.3  1994/07/13  04:21:08  nevis
 * #define RWString to RWCString for MetaWare to help
 * solve problems compiling older test suite code
 *
 * Revision 6.2  1994/07/12  19:58:19  vriezen
 * Update Copyright notice
 *
 * Revision 6.1  1994/04/15  19:10:51  vriezen
 * Move all files to 6.1
 *
 * Revision 2.2  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.8   01 Mar 1992 15:43:34   KEFFER
 * Now just a typedef to RWCString.
 */

#include "rw/cstring.h"

typedef RWCString RWString;

#endif	/* __RWSTRING_H__ */