/**********************************************************************
 *
 * tvmap.cc - template definitions for RWTValMap.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvmap.cc#1 $
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

template <class K, class T, class C, class A>
void
RWTValMap<K, T, C, A>::apply(void (*fn)(const key_type&, mapped_type&, void*), void* x)
{
    for (iterator i = begin(); i != end(); ++i) {
        (*fn)((*i).first, (*i).second, x);
    }
}

template <class K, class T, class C, class A>
void
RWTValMap<K, T, C, A>::apply(void (*fn)(const key_type&, const mapped_type&, void*), void* x) const
{
    for (const_iterator i = begin(); i != end(); ++i) {
        (*fn)((*i).first, (*i).second, x);
    }
}

template <class K, class T, class C, class A>
typename RWTValMap<K, T, C, A>::size_type
RWTValMap<K, T, C, A>::removeAll(bool (*fn)(const_reference, void*), void* x)
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

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_3(RWTValMap)

template <class K, class T, class C, class S, class A>
void rwSaveGuts(S& str, const RWTValMap<K, T, C, A>& c)
{
    str.putSizeT(c.entries());
    typename RWTValMap<K, T, C, A>::const_iterator it = c.begin();

    while (it != c.end()) {
        str << (*it).first;   // key
        str << (*it).second;  // data
        it++;
    }
}

template <class K, class T, class C, class S, class A>
void rwRestoreGuts(S& str, RWTValMap<K, T, C, A>& c)
{
    typedef typename RWTValMap<K, T, C, A>::size_type size_type;
    typename RWTValMap<K, T, C, A>::size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        K key;
        str >> key;
        T data;
        str >> data;
        c.insert(key, data);
    }
}
