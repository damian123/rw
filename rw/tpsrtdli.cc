

/**********************************************************************
 *
 * tpsrtdli.cc - template definitions for RWTPtrSortedDlistIterator<T,C>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpsrtdli.cc#1 $
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

// the following code is optimum if iterators are not random
template <class T, class C, class A>
typename RWTPtrSortedDlist<T, C, A>::size_type
RWTPtrSortedDlist<T, C, A>::index(const T* key) const
{
    // be sure there is a last() to compare.
    if (isEmpty()) {
        return RW_NPOS;
    }
    key_compare comp = key_compare();
    // Must stop before end(): last() "<" key: we would not stop
    if (comp(last(), key)) {
        return RW_NPOS;
    }
    size_type ret = 0;
    const_iterator it = begin();
    // loop as long as key is "<" than *iterator
    while (comp(*it, key)) {
        ++it;
        ++ret;
    }
    // if *iterator "less" than key, then we have no hit
    if (comp(*it, key)) {
        return RW_NPOS;
    }
    else if (comp(key, *it)) {
        return RW_NPOS;
    }
    else {
        return ret;
    }
}

template <class T, class C, class A>
typename RWTPtrSortedDlist<T, C, A>::size_type
RWTPtrSortedDlist<T, C, A>::removeAll(const T* key)
{
    const std::pair<iterator, iterator> range
        = std::equal_range(begin(), end(), key, key_compare());
    const size_type oldsize = std().size();
    std().erase(range.first, range.second);
    return oldsize - std().size();
}

template <class T, class C, class A>
typename RWTPtrSortedDlist<T, C, A>::size_type
RWTPtrSortedDlist<T, C, A>::insert(const container_type& coll)
{
    size_type ret = coll.size();
    if (ret) {
        key_compare cmp = key_compare();
        container_type copy(coll);
        rw_list_sort(copy, cmp);
#if !defined(RW_STDLIB_VER) || !defined(_RWSTD_NO_MEMBER_TEMPLATES)
        std().merge(copy, cmp);
#else
        const_iterator copyIter = copy.begin();
        for (iterator iter = begin(); iter != end(); ++iter) {
            while (copyIter != copy.end() && cmp(*copyIter, *iter)) {
                std().insert(iter, *(copyIter++));
            }
        }
        std().insert(end(), copyIter, (const_iterator)copy.end());
#endif
    }
    return ret;
}

template <class T, class C, class A>
typename RWTPtrSortedDlist<T, C, A>::size_type
RWTPtrSortedDlist<T, C, A>::merge(const RWTPtrSortedDlist<T, C, A>& coll)
{
    size_type ret = coll.entries();
    if (ret) {
        key_compare cmp = key_compare();
#if !defined(RW_STDLIB_VER) || !defined(_RWSTD_NO_MEMBER_TEMPLATES)
        container_type tmp(coll.std());
        std().merge(tmp, cmp);
#else
        const_iterator collIter = coll.begin();
        for (iterator iter = begin(); iter != end(); ++iter) {
            while (collIter != coll.end() && cmp(*collIter, *iter)) {
                std().insert(iter, *(collIter++));
            }
        }
        std().insert(end(), collIter, coll.end());
#endif
    }
    return ret;
}


// protected functions

template <class T, class C, class A>
typename RWTPtrSortedDlist<T, C, A>::iterator
RWTPtrSortedDlist<T, C, A>::insertSorted(iterator hint, value_type a)
{
    iterator hit;
    iterator last  = end();
    iterator first = begin();

    key_compare comp = key_compare();  // avoid warning "comp used before value set"
    if (hint == last) {
        hit = std::upper_bound(first, last, a, comp);
    }
    else {
        // test for "*hint is less than or equal to a"
        if (!comp(a, *hint)) {
            hit = std::upper_bound(hint, last, a, comp);
        }
        else { // the contained value must be "greater than" a
            hit = std::upper_bound(first, hint, a, comp);
        }
    }
    return std().insert(hit, a);
}

template <class T, class C, class A>
typename RWTPtrSortedDlist<T, C, A>::size_type
RWTPtrSortedDlist<T, C, A>::index(bool (*fn)(const T*, void*), void* x) const
{
    size_type      idx  = 0;
    const_iterator iter = begin();
    while (iter != end()) {
        if ((*fn)(*iter, x)) {
            return idx;
        }
        ++iter, ++idx;
    }

    return RW_NPOS;
}

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS

template <class T, class C, class A> bool
RWTPtrSortedDlistIterator<T, C, A>::operator++()
{
    RW_ASSERT(!(head_ == true  &&  tail_ == true));

    if (tail_) {
        /* do nothing */
    }
    else if (head_) {
        iter_ = list_->begin();
        head_ = false;
    }
    else {
        ++iter_;
    }

    if (!tail_ && iter_ == list_->end()) {
        tail_ = true;
    }

    RW_ASSERT(!(head_ == true  &&  tail_ == true));

    return !tail_;
}

template <class T, class C, class A> bool
RWTPtrSortedDlistIterator<T, C, A>::operator--()
{
    RW_ASSERT(!(head_ == true  &&  tail_ == true));

    if (head_) {
        /* do nothing */
    }
    else if (tail_) {
        if (list_->entries() == 0) {
            tail_ = false;
            head_ = true;
        }
        else {
            tail_ = false;
            --iter_;
        }
    }
    else if (iter_ == list_->begin()) {
        head_ = true;
    }
    else {
        --iter_;
    }

    RW_ASSERT(!(head_ == true  &&  tail_ == true));

    return !head_;
}

template <class T, class C, class A> bool
RWTPtrSortedDlistIterator<T, C, A>::operator+=(size_t n)
{
    while (n-- && ++(*this)) {
        /* do nothing */
    }
    return !tail_ && !list_->isEmpty();
}

template <class T, class C, class A> bool
RWTPtrSortedDlistIterator<T, C, A>::operator-=(size_t n)
{
    while (n-- && --(*this)) {
        /* do nothing */
    }
    return !head_;
}

template <class T, class C, class A> T*
RWTPtrSortedDlistIterator<T, C, A>::findNext(const T* p)
{
    T* a;
    while ((a = (*this)()) != rwnil) {
        if (*a == *p) {
            return a;
        }
    }
    return rwnil;
}

template <class T, class C, class A> T*
RWTPtrSortedDlistIterator<T, C, A>
::findNext(bool (*testFun)(T*, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    T* a;
    while ((a = (*this)()) != rwnil) {
        if (testFun(a, d)) {
            return a;
        }
    }
    return rwnil;
}

template <class T, class C, class A> T*
RWTPtrSortedDlistIterator<T, C, A>::remove()
{
    typename RWTPtrSortedDlist<T, C, A>::iterator i;

    if (head_ || tail_) {
        return rwnil;    // iterator is not active
    }

    T* ret = *iter_;
    i = iter_;
    if (iter_ == list_->begin()) {
        reset();
    }
    else {
        --i;
    }
    list_->std().erase(iter_);
    iter_ = i;
    return ret;
}

template <class T, class C, class A> T*
RWTPtrSortedDlistIterator<T, C, A>::removeNext(const T* p)
{
    while (++(*this)) {
        if (*(*iter_)  == *p) {
            return remove();
        }
    }
    return rwnil;
}

template <class T, class C, class A> T*
RWTPtrSortedDlistIterator<T, C, A>
::removeNext(bool (*testFun)(T*, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    while (++(*this)) {
        if (testFun(*iter_, d)) {
            return remove();
        }
    }
    return rwnil;
}

template <class T, class C, class A> bool
RWTPtrSortedDlistConstIterator<T, C, A>::operator++()
{
    RW_ASSERT(!(head_ == true  &&  tail_ == true));

    if (tail_) {
        /* do nothing */
    }
    else if (head_) {
        iter_ = list_->begin();
        head_ = false;
    }
    else {
        ++iter_;
    }

    if (!tail_ && iter_ == list_->end()) {
        tail_ = true;
    }

    RW_ASSERT(!(head_ == true  &&  tail_ == true));

    return !tail_;
}

template <class T, class C, class A> bool
RWTPtrSortedDlistConstIterator<T, C, A>::operator--()
{
    RW_ASSERT(!(head_ == true  &&  tail_ == true));

    if (head_) {
        /* do nothing */
    }
    else if (tail_) {
        if (list_->entries() == 0) {
            tail_ = false;
            head_ = true;
        }
        else {
            tail_ = false;
            --iter_;
        }
    }
    else if (iter_ == list_->begin()) {
        head_ = true;
    }
    else {
        --iter_;
    }

    RW_ASSERT(!(head_ == true  &&  tail_ == true));

    return !head_;
}

template <class T, class C, class A> bool
RWTPtrSortedDlistConstIterator<T, C, A>::operator+=(size_t n)
{
    while (n-- && ++(*this)) {
        /* do nothing */
    }
    return !tail_ && !list_->isEmpty();
}

template <class T, class C, class A> bool
RWTPtrSortedDlistConstIterator<T, C, A>::operator-=(size_t n)
{
    while (n-- && --(*this)) {
        /* do nothing */
    }
    return !head_;
}

template <class T, class C, class A> const T*
RWTPtrSortedDlistConstIterator<T, C, A>::findNext(const T* p)
{
    const T* a;
    while ((a = (*this)()) != rwnil) {
        if (*a == *p) {
            return a;
        }
    }
    return rwnil;
}

template <class T, class C, class A> const T*
RWTPtrSortedDlistConstIterator<T, C, A>::findNext(bool (*testFun)(T*, void*),
        void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    const T* a;
    while ((a = (*this)()) != rwnil) {
        if (testFun(a, d)) {
            return a;
        }
    }
    return rwnil;
}

RW_RESTORE_DEPRECATED_WARNINGS
#endif

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_2(RWTPtrSortedDlist)

template <class T, class C, class S, class A>
void rwSaveGuts(S& str, const RWTPtrSortedDlist<T, C, A>& c)
{
    str.putSizeT(c.entries());
    if (c.entries()) {
        // iterate backward so RestoreGuts is linear...
        typename RWTPtrSortedDlist<T, C, A>::const_iterator it
            = c.end();
        do {
            --it;
            str << *(*it);
        }
        while (it != c.begin());
    }
}

template <class T, class C, class S, class A>
void rwRestoreGuts(S& str, RWTPtrSortedDlist<T, C, A>& c)
{
    typedef typename RWTPtrSortedDlist<T, C, A>::size_type size_type;
    typename RWTPtrSortedDlist<T, C, A>::size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        T* tp = 0;
        str >> tp;
        c.insert(tp);
    }
}
