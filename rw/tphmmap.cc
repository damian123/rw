/**********************************************************************
 *
 * Template definitions for RWTPtrHashMultiMap.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tphmmap.cc#1 $
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
RWTPtrHashMultiMap<K, T, H, EQ, A>::apply(void(*fn)(const K*, mapped_type&, void*), void* d)
{
    for (iterator i = begin(); i != end(); ++i) {
        (*fn)((*i).first, (*i).second, d);
    }
}

template <class K, class T, class H, class EQ, class A>
void
RWTPtrHashMultiMap<K, T, H, EQ, A>::apply(void(*fn)(const K*, const T*, void*), void* d) const
{
    for (const_iterator i = begin(); i != end(); ++i) {
        (*fn)((*i).first, (*i).second, d);
    }
}

template <class K, class T, class H, class EQ, class A>
void
RWTPtrHashMultiMap<K, T, H, EQ, A>::clearAndDestroy()
{
    // first delete items we point to
    for (iterator it = begin(); it != end(); ++it) {
        delete(*it).first;
        delete(*it).second;
    }
    clear(); // then empty self
}

template <class K, class T, class H, class EQ, class A>
typename RWTPtrHashMultiMap<K, T, H, EQ, A>::size_type
RWTPtrHashMultiMap<K, T, H, EQ, A>::removeAll(bool(*fn)(value_type, void*), void* d)
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

template <class K, class T, class H, class EQ, class A>
bool operator==(const RWTPtrHashMultiMap<K, T, H, EQ, A>& lhs,
                const typename RWTPtrHashMultiMap<K, T, H, EQ, A>::container_type& rhs)
{
    typedef RWTPtrHashMultiMap<K, T, H, EQ, A> container_type;
    typedef typename container_type::const_iterator const_iterator;
    typedef typename container_type::key_equal key_equal;
    typedef typename container_type::deref_equal deref_equal;
    typedef typename rw_iterator_traits<const_iterator>::difference_type size_type;
    typedef typename container_type::value_type value_type;
    typedef typename container_type::key_type key_type;
    typedef typename container_type::mapped_type mapped_type;
    typedef std::pair<key_type, mapped_type> vector_value_type;
    typedef std::vector<vector_value_type> vector_type;
    typedef typename vector_type::iterator vector_iterator;
    typedef rw_deref_pair_equal<vector_value_type, vector_value_type> vector_deref_equal;

    if (lhs.entries() != rhs.size()) {
        return false;
    }

    key_equal key_eq;
    vector_type lhs_tmp;

    for (const_iterator iter = lhs.begin(); iter != lhs.end();) {
        // for the first key in lhs, identify the number of entries for that key
        // and the iterator range for that key.
        const_iterator lbegin = iter++;
        size_type lcount = 1;
        while (iter != lhs.end() && key_eq((*lbegin).first, (*iter).first)) {
            ++lcount;
            ++iter;
        }
        const_iterator lend = iter;

        // for the first key in lhs, identify the number of entries for that
        // key in rhs and the iterator range for that key.
        std::pair<const_iterator, const_iterator> riter = rhs.equal_range((*lbegin).first);
        size_type rcount = rw_distance(riter.first, riter.second);

        if (lcount != rcount) {
            return false;
        }

        // Create a temporary collection of the key/values from lhs to allow
        // removing entries as they are matched in rhs.
#if !defined(_RWSTD_NO_MEMBER_TEMPLATES)
        lhs_tmp.assign(lbegin, lend);
#else
        lhs_tmp.clear();
        for (const_iterator copyIter = lbegin; copyIter != lend; ++copyIter) {
            lhs_tmp.push_back(vector_value_type((*copyIter).first, (*copyIter).second));
        }
#endif

        vector_iterator ltbegin = lhs_tmp.begin();
        vector_iterator ltend = lhs_tmp.end();

        // For each value in lhs, verify the number of instances of that
        // value in lhs and rhs are equal.
        while (ltbegin != ltend) {
            size_type rvcount =
                rw_count_if(riter.first, riter.second,
                            rw_bind2nd(deref_equal(), value_type((*ltbegin).first, (*ltbegin).second)));
            vector_iterator newltend =
                std::remove_if(ltbegin, ltend, rw_bind2nd(vector_deref_equal(), *ltbegin));
            size_type lvcount = rw_distance(newltend, ltend);
            if (lvcount != rvcount) {
                return false;
            }
            ltend = newltend;
        }
    }
    return true;
}

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_4(RWTPtrHashMultiMap)

template < class K, class T, class H, class EQ, class S, class A>
void rwSaveGuts(S& str, const RWTPtrHashMultiMap<K, T, H, EQ, A>& c)
{
    str.putSizeT(c.entries());
    typename RWTPtrHashMultiMap<K, T, H, EQ, A>::const_iterator it
        = c.begin();

    while (it != c.end()) {
        str << *(*it).first;
        str << *(*it).second;
        it++;
    }
}

template < class K, class T, class H, class EQ, class S, class A>
void rwRestoreGuts(S& str, RWTPtrHashMultiMap<K, T, H, EQ, A>& c)
{
    typedef typename RWTPtrHashMultiMap<K, T, H, EQ, A>::size_type size_type;
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

