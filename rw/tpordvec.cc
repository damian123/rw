

/**********************************************************************
 *
 * Definitions for ./tpordvec.cc
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpordvec.cc#1 $
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

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE(RWTPtrOrderedVector)

template <class T, class S, class A>
void rwSaveGuts(S& str, const RWTPtrOrderedVector<T, A>& c)
{
    str.putSizeT(c.entries());
    for (size_t i = 0; i < c.entries(); ++i) {
        str << *c[i];
    }
}

template <class T, class S, class A>
void rwRestoreGuts(S& str, RWTPtrOrderedVector<T, A>& c)
{
    typedef typename RWTPtrOrderedVector<T, A>::size_type size_type;
    size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        T* p = 0;
        str >> p;
        c.insert(p);
    }
}