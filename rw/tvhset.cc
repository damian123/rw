/**********************************************************************
 *
 * Template definitions for RWTValHashSet
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvhset.cc#1 $
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

template < class T, class H, class EQ, class A>
RWTValHashSet<T, H, EQ, A>&
RWTValHashSet<T, H, EQ, A>::difference(const container_type& stdcoll)
{
    for (const_iterator cur = std().begin(); cur != std().end();) {
        if (stdcoll.find(*cur) != stdcoll.end()) {
            cur = std().erase(cur);
        }
        else {
            ++cur;
        }
    }
    return *this;
}

template < class T, class H, class EQ, class A>
RWTValHashSet<T, H, EQ, A>&
RWTValHashSet<T, H, EQ, A>::intersection(const container_type& stdcoll)
{
    for (const_iterator cur = std().begin(); cur != std().end();) {
        if (stdcoll.find(*cur) == stdcoll.end()) {
            cur = std().erase(cur);
        }
        else {
            ++cur;
        }
    }
    return *this;
}

template < class T, class H, class EQ, class A>
typename RWTValHashSet<T, H, EQ, A>::size_type
RWTValHashSet<T, H, EQ, A>::removeAll(bool (*fn)(const_reference, void*), void* x)
{
    size_type ret = entries();

    while (entries() && (*fn)(*(begin()), x)) {
        std().erase(begin());
    }

    iterator iter = begin();

    while (iter != end()) {
        iterator iter2 = iter;
        ++iter2;
        if (iter2 != end() && (*fn)(*iter2, x)) {
            std().erase(iter2);
        }
        else {
            ++iter;
        }
    }

    return ret - entries();
}

template < class T, class H, class EQ, class A>
RWTValHashSet<T, H, EQ, A>&
RWTValHashSet<T, H, EQ, A>::symmetricDifference(const container_type& stdcoll)
{
    for (const_iterator cur = stdcoll.begin();
            cur != stdcoll.end(); ++cur) {
        const_iterator found = std().find(*cur);
        if (found != std().end()) {
            std().erase(found);
        }
        else {
            std().insert(*cur);
        }
    }
    return *this;
}

template < class T, class H, class EQ, class A>
RWTValHashSet<T, H, EQ, A>&
RWTValHashSet<T, H, EQ, A>::Union(const container_type& stdcoll)
{
    for (const_iterator cur = stdcoll.begin();
            cur != stdcoll.end(); ++cur) {
        std().insert(*cur);
    }
    return *this;
}

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_3(RWTValHashSet)

template < class T, class H, class EQ, class S, class A>
void rwSaveGuts(S& str, const RWTValHashSet<T, H, EQ, A>& c)
{
    str.putSizeT(c.entries());
    typename RWTValHashSet<T, H, EQ, A>::const_iterator it
        = c.begin();

    while (it != c.end()) {
        str << *it;
        it++;
    }
}

template < class T, class H, class EQ, class S, class A>
void rwRestoreGuts(S& str, RWTValHashSet<T, H, EQ, A>& c)
{
    typedef typename RWTValHashSet<T, H, EQ, A>::size_type size_type;
    typename RWTValHashSet<T, H, EQ, A>::size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        T data;
        str >> data;
        c.insert(data);
    }
}
