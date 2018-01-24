/**********************************************************************
 *
 * tvset.cc - template definitions for RWTValSet.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvset.cc#1 $
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

#include <rw/epersist.h>

template <class T, class C, class A>
typename RWTValSet<T, C, A>::size_type
RWTValSet<T, C, A>::removeAll(bool (*fn)(const_reference, void*), void* x)
{
    size_type ret = entries();

    iterator iter = begin();

    while (iter != end()) {
        iterator prev = iter++;
        if ((*fn)(*prev, x)) {
            std().erase(prev);
        }
    }

    return ret - entries();
}

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_2(RWTValSet)

template <class T, class C, class S, class A>
void rwSaveGuts(S& str, const RWTValSet<T, C, A>& c)
{
    str.putSizeT(c.entries());
    typename RWTValSet<T, C, A>::const_iterator it = c.begin();

    while (it != c.end()) {
        str << *it;
        it++;
    }
}

template <class T, class C, class S, class A>
void rwRestoreGuts(S& str, RWTValSet<T, C, A>& c)
{
    typedef typename RWTValSet<T, C, A>::size_type size_type;
    typename RWTValSet<T, C, A>::size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        T t;
        str >> t;
        c.insert(t);
    }
}
