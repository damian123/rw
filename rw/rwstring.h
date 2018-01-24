#ifndef RW_RWSTRING_H
#define RW_RWSTRING_H



/**********************************************************************
 *
 * RWString: Backwards compatibility declaration
 * This file is provided for backward compatibility.  Note, however, that
 * the copy constructor and assignment operator of RWString used reference
 * semantics while the new RWCString uses copy semantics.
 * $Log: rwstring.h,v $
 * Revision 7.3  1996/02/18 01:46:26  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 * Revision 7.2  1995/09/05 19:41:47  jims
 * Use new copyright macro
 * Revision 7.1  1994/10/16  02:57:03  josh
 * Merged 6.1 and 7.0 development trees
 * Revision 6.4  1994/07/18  06:17:36  jims
 * Back out previous change; update test suite instead.
 * Revision 6.3  1994/07/13  04:21:08  nevis
 * #define RWString to RWCString for MetaWare to help
 * solve problems compiling older test suite code
 * Revision 6.2  1994/07/12  19:58:19  vriezen
 * Update Copyright notice
 * Revision 6.1  1994/04/15  19:10:51  vriezen
 * Move all files to 6.1
 * Revision 2.2  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 *    Rev 1.8   01 Mar 1992 15:43:34   KEFFER
 * Now just a typedef to RWCString.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rwstring.h#3 $
 *
 **********************************************************************
 *
 * Copyright (c) 1989-2015 Rogue Wave Software, Inc.  All Rights Reserved.
 * 
 * This computer software is owned by Rogue Wave Software, Inc. and is
 * protected by U.S. copyright laws and other laws and by international
 * treaties.  This computer software is furnished by Rogue Wave Software, Inc.
 * pursuant to a written license agreement and may be used, copied, transmitted,
 * and stored only in accordance with the terms of such license agreement and 
 * with the inclusion of the above copyright notice.  This computer software or
 * any other copies thereof may not be provided or otherwise made available to
 * any other person.
 * 
 * U.S. Government Restricted Rights.  This computer software: (a) was
 * developed at private expense and is in all respects the proprietary
 * information of Rogue Wave Software, Inc.; (b) was not developed with 
 * government funds; (c) is a trade secret of Rogue Wave Software, Inc. for all
 * purposes of the Freedom of Information Act; and (d) is a commercial item and
 * thus, pursuant to Section 12.212 of the Federal Acquisition Regulations (FAR)
 * and DFAR Supplement Section 227.7202, Government's use, duplication or
 * disclosure of the computer software is subject to the restrictions set forth
 * by Rogue Wave Software, Inc.
 *
 **********************************************************************/


#include <rw/defs.h>
#include <rw/cstring.h>

typedef RWCString RWString;

#endif
