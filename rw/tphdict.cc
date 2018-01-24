/**********************************************************************
 *
 * Template definitions for RWTPtrHashDictionary<KP,VP>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tphdict.cc#1 $
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

template <class K, class T, class H, class EQ, class A>
void
RWTPtrHashMap<K, T, H, EQ, A>::apply(void(*fn)(const K*, mapped_type&, void*), void* d)
{
    for (iterator i = begin(); i != end(); ++i) {
        (*fn)((*i).first, (*i).second, d);
    }
}

template <class K, class T, class H, class EQ, class A>
void
RWTPtrHashMap<K, T, H, EQ, A>::apply(void(*fn)(const K*, const T*, void*), void* d) const
{
    for (const_iterator i = begin(); i != end(); ++i) {
        (*fn)((*i).first, (*i).second, d);
    }
}

template <class K, class T, class H, class EQ, class A>
typename RWTPtrHashMap<K, T, H, EQ, A>::size_type
RWTPtrHashMap<K, T, H, EQ, A>::removeAll(bool(*fn)(value_type, void*), void* d)
{
    size_type ret = entries();

    while (entries() && (*fn)(*(value_type*)&*begin(), d)) {
        std().erase(begin());
    }

    iterator iter = begin();
    iterator iter2;

    while (iter != end()) {
        iter2 = iter;
        ++iter2;
        if (iter2 != end() && (*fn)(*(value_type*)&*iter2, d)) {
            std().erase(iter2);
        }
        else {
            ++iter;
        }
    }

    return ret - entries();
}

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_4(RWTPtrHashMap)

template < class K, class T, class H, class EQ, class S, class A>
void rwSaveGuts(S& str, const RWTPtrHashMap<K, T, H, EQ, A>& c)
{
    str.putSizeT(c.entries());
    typename RWTPtrHashMap<K, T, H, EQ, A>::const_iterator it
        = c.begin();

    while (it != c.end()) {
        str << *(*it).first;
        str << *(*it).second;
        it++;
    }
}

template < class K, class T, class H, class EQ, class S, class A>
void rwRestoreGuts(S& str, RWTPtrHashMap<K, T, H, EQ, A>& c)
{
    typedef typename RWTPtrHashMap<K, T, H, EQ, A>::size_type size_type;
    size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        K* keyP = 0;
        str >> keyP;
        T* dataP = 0;
        str >> dataP;
        c.insert(keyP, dataP);
    }
}
