/**********************************************************************
 *
 * Template definitions for RWTPtrSortedVector<TP>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpsrtvec.cc#1 $
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

template <class T, class C, class A>
typename RWTPtrSortedVector<T, C, A>::size_type
RWTPtrSortedVector<T, C, A>::insert(const container_type& a)
{
    size_type ret = a.size();
    if (ret) {
        size_type e = entries();
        std().reserve(e + ret);
        std().insert(std().end(), a.begin(), a.end());
        std::sort(std().begin() + e, std().end(), key_compare());
        std::inplace_merge(std().begin(), std().begin() + e, std().end(), key_compare());
    }
    return ret;
}


template <class T, class C, class A>
typename RWTPtrSortedVector<T, C, A>::size_type
RWTPtrSortedVector<T, C, A>::merge(const RWTPtrSortedVector<T, C, A>& vec)
{
    size_type ret = vec.entries();
    if (ret) {
        size_type e = entries();
        std().reserve(e + ret);
        std().insert(std().end(), vec.begin(), vec.end());
        std::inplace_merge(std().begin(), std().begin() + e, std().end(), key_compare());
    }
    return ret;
}


template <class T, class C, class A>
typename RWTPtrSortedVector<T, C, A>::size_type
RWTPtrSortedVector<T, C, A>::removeAll(const T* key)
{
    const std::pair<iterator, iterator> range
        = std::equal_range(begin(), end(), key, key_compare());
    const size_type oldsize = std().size();
    std().erase(range.first, range.second);
    return oldsize - std().size();
}

// protected functions

template <class T, class C, class A>
typename RWTPtrSortedVector<T, C, A>::iterator
RWTPtrSortedVector<T, C, A>::insertSorted(iterator hint, value_type a)
{
    iterator hit;
    iterator last  = end();
    iterator first = begin();

    key_compare comp = key_compare();  // avoid warning "comp used before value set"
    if (hint == last) {
        hit = std::upper_bound(first, last, a, comp);
    }
    else {
        // test for "*hint less than or equal to a"
        if (!comp(a, *hint)) {
            hit = std::upper_bound(hint, last, a, comp);
        }
        else { // the contained value must be "greater than" a
            hit = std::upper_bound(first, hint, a, comp);
        }
    }
    return std().insert(hit, a);
}

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_2(RWTPtrSortedVector)

template <class T, class C, class S, class A>
void rwSaveGuts(S& str, const RWTPtrSortedVector<T, C, A>& c)
{
    str.putSizeT(c.entries());
    for (size_t i = 0; i < c.entries(); ++i) {
        str << *c[i];
    }
}

template <class T, class C, class S, class A>
void rwRestoreGuts(S& str, RWTPtrSortedVector<T, C, A>& c)
{
    typedef typename RWTPtrSortedVector<T, C, A>::size_type size_type;
    size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        T* p = 0;
        str >> p;
        c.insert(p);
    }
}
