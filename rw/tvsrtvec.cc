/**********************************************************************
 *
 * Template definitions for RWTValSortedVector<T>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvsrtvec.cc#1 $
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
#include <rw/epfunc.h>

#include <algorithm>

template <class T, class C, class A>
typename RWTValSortedVector<T, C, A>::size_type
RWTValSortedVector<T, C, A>::insert(const container_type& a)
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
typename RWTValSortedVector<T, C, A>::size_type
RWTValSortedVector<T, C, A>::merge(const RWTValSortedVector<T, C, A>& coll)
{
    size_type ret = coll.entries();
    if (ret) {
        size_type e = entries();
        std().reserve(e + ret);
        std().insert(std().end(), coll.begin(), coll.end());
        std::inplace_merge(std().begin(), std().begin() + e, std().end(), key_compare());
    }
    return ret;
}

template <class T, class C, class A>
typename RWTValSortedVector<T, C, A>::size_type
RWTValSortedVector<T, C, A>::removeAll(const_reference key)
{
    const std::pair<iterator, iterator> range
        = std::equal_range(begin(), end(), key, key_compare());
    const size_type oldsize = std().size();
    std().erase(range.first, range.second);
    return oldsize - std().size();
}

template <class T, class C, class A>
typename RWTValSortedVector<T, C, A>::value_type
RWTValSortedVector<T, C, A>::removeAt(size_type n)
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

template <class C, class A>
typename RWTValSortedVector<bool, C, A>::size_type
RWTValSortedVector<bool, C, A>::insert(const container_type& a)
{
    size_type ret = a.size();
    if (ret) {
        size_type e = entries();
        std().reserve(e + ret);
#if defined(_MSC_VER) && (_MSC_VER >= 1500 && _MSC_VER <= 1800)
        if (&std() != &a) {
            std().insert(std().end(), a.begin(), a.end());
        }
        else {
            std::vector<bool, A> tmp(a);
            std().insert(std().end(), tmp.begin(), tmp.end());
        }
#else
        std().insert(std().end(), a.begin(), a.end());
#endif
        rw_bitvec_sort(std(), key_compare());
    }
    return ret;
}

template <class C, class A>
typename RWTValSortedVector<bool, C, A>::size_type
RWTValSortedVector<bool, C, A>::merge(const RWTValSortedVector<bool, C, A>& coll)
{
    size_type ret = coll.entries();
    if (ret) {
        size_type e = entries();
        std().reserve(e + ret);
#if defined(_MSC_VER) && (_MSC_VER >= 1500 && _MSC_VER <= 1800)
        if (&std() != &coll.std()) {
            std().insert(std().end(), coll.begin(), coll.end());
        }
        else {
            std::vector<bool, A> tmp(coll.std());
            std().insert(std().end(), tmp.begin(), tmp.end());
        }
#else
        std().insert(std().end(), coll.begin(), coll.end());

#endif
        rw_bitvec_sort(std(), key_compare());
    }
    return ret;
}

template <class C, class A>
typename RWTValSortedVector<bool, C, A>::size_type
RWTValSortedVector<bool, C, A>::removeAll(const_reference key)
{
    const std::pair<iterator, iterator> range
        = std::equal_range(begin(), end(), key, key_compare());
    const size_type oldsize = std().size();
    std().erase(range.first, range.second);
    return oldsize - std().size();
}

template <class C, class A>
typename RWTValSortedVector<bool, C, A>::value_type
RWTValSortedVector<bool, C, A>::removeAt(size_type n)
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

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_2(RWTValSortedVector)

template <class T, class C, class S, class A>
void rwSaveGuts(S& str, const RWTValSortedVector<T, C, A>& c)
{
    str.putSizeT(c.entries());
    for (typename RWTValSortedVector<T, C, A>::size_type i = 0; i < c.entries(); ++i) {
        str << c[i];
    }
}

template <class T, class C, class S, class A>
void rwRestoreGuts(S& str, RWTValSortedVector<T, C, A>& c)
{
    typedef typename RWTValSortedVector<T, C, A>::size_type size_type;
    typename RWTValSortedVector<T, C, A>::size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        T t;
        str >> t;
        c.insert(t);
    }
}
