

/**********************************************************************
 *
 * Template definitions for RWTValAssocLink<K,V>
 * $Log: tasslnk.cc,v $
 * Revision 7.3  1996/02/18 01:46:50  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 * Revision 7.2  1995/09/05 19:46:23  jims
 * Use new copyright macro
 * Revision 7.1  1994/10/16  03:13:51  josh
 * Merged 6.1 and 7.0 development trees
 * Revision 6.2  1994/07/12  20:04:35  vriezen
 * Updated Copyright.
 * Revision 6.1  1994/04/15  19:11:03  vriezen
 * Move all files to 6.1
 * Revision 1.3  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 * Revision 1.2  1993/02/17  18:32:03  keffer
 * Now passes T's by const reference, rather than by value
 * Revision 1.1  1993/01/28  21:11:49  keffer
 * Ported to cfront V3.0
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tasslnk.cc#1 $
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


/*
 * Template-generated single-value constructor.  This can be overridden
 * by the user.
 */
template <class K, class V>
RWTValAssocLink<K, V>::RWTValAssocLink(const K& key) : key_(key) {}
