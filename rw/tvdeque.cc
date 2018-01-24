/**********************************************************************
 *
 * tvdeque.cc - template definitions for RWTValDeque.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvdeque.cc#1 $
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

template <class T, class A>
typename RWTValDeque<T, A>::value_type
RWTValDeque<T, A>::removeAt(size_type n)
{
#ifdef RW_DEBUG
    size_type n_dbg = entries();
#endif

    boundsCheck(n);
    iterator i = begin();
    std::advance(i, n);
    value_type ret = *i;
    std().erase(i);
    RW_POSTCONDITION(entries() == n_dbg - 1);
    return ret;
}

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE(RWTValDeque)

template <class T, class S, class A>
void rwSaveGuts(S& str, const RWTValDeque<T, A>& c)
{
    str.putSizeT(c.entries());
    for (typename RWTValDeque<T, A>::size_type i = 0; i < c.entries(); ++i) {
        str << c[i];
    }
}

template <class T, class S, class A>
void rwRestoreGuts(S& str, RWTValDeque<T, A>& c)
{
    typedef typename RWTValDeque<T, A>::size_type size_type;
    size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        T t;
        str >> t;
        c.insert(t);
    }
}

