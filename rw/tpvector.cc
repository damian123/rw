/**********************************************************************
 *
 * File: tpvector.cc
 * Template definitions for RWTPtrVector<T>
 * $Log: tpvector.cc,v $
 * Revision 7.7  1996/12/03 22:22:10  griswolf
 * Fix bad eol in log 7.1 comment.
 * Revision 7.6  1996/09/05 15:20:49  griswolf
 * Scopus #4418: Fix problem with diff type and size of size_t.
 * Revision 7.5  1996/06/27 19:52:50  griswolf
 * *** empty log message ***
 * Revision 7.4  1996/03/16 15:15:37  jims
 * Port non-stdlib template persistence to DEC and Watcom
 * Revision 7.3  1996/02/12 21:03:06  kevinj
 * Switched stream and class args in rw*Guts.
 * Revision 7.2  1996/01/29 18:03:02  kevinj
 * Simple External Template Persistence (ETP).
 * Revision 7.1  1996/01/29 17:36:50  jims
 * Move to revision 7.1
 * Revision 1.2  1993/02/17 18:28:08  keffer
 * Now obsolete.
 * Revision 1.1  1993/01/28  02:39:38  keffer
 * Initial revision
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpvector.cc#1 $
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
#include <rw/epersist.h>

RW_DEFINE_PERSISTABLE_TEMPLATE(RWTPtrVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
RW_DEFINE_PERSIST_TEMPLATE_IO(RWTPtrVector, RWpistream, RWpostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWTPtrVector, RWbistream, RWbostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWTPtrVector, RWeistream, RWeostream)
#endif

template <class T, class S>
void rwSaveGuts(S& str, const RWTPtrVector<T>& c)
{
    str.putSizeT(c.entries());
    for (size_t i = 0; i < c.entries(); ++i) {
        str << *c(i);
    }
}

template <class T, class S>
void rwRestoreGuts(S& str, RWTPtrVector<T>& c)
{
    size_t count;
    str.getSizeT(count);

    RWTPtrVector<T> buf(count);
    for (size_t i = 0; i < count; ++i) {
        str >> buf(i);
    }

    buf.swap(c);
}
