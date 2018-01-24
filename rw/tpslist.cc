/**********************************************************************
 *
 * Definitions for ./tpslist.cc
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpslist.cc#1 $
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
typename RWTPtrSlist<T, A>::size_type
RWTPtrSlist<T, A>::index(bool (*fn)(const T*, void*), void* x) const
{
    size_type idx  = 0;
    for (const_iterator iter = begin(); iter != end(); ++iter, ++idx) {
        if ((*fn)(*iter, x)) {
            return idx;
        }
    }
    return RW_NPOS;
}

template <class T, class A>
typename RWTPtrSlist<T, A>::size_type
RWTPtrSlist<T, A>::index(bool (*fn)(value_type, void*),
                         void* x) const
{
    size_type idx  = 0;
    for (const_iterator iter = begin(); iter != end(); ++iter, ++idx) {
        if ((*fn)(*iter, x)) {
            return idx;
        }
    }
    return RW_NPOS;
}

template <class T, class A>
typename RWTPtrSlist<T, A>::size_type
RWTPtrSlist<T, A>::index(const T* t) const
{
    size_type idx  = 0;
    for (const_iterator iter = begin(); iter != end(); ++iter, ++idx) {
        if (deref_equal()(*iter, t)) {
            return idx;
        }
    }
    return RW_NPOS;
}

/*
 * This is a "for now" sorting algorithm.  Ultimately the underlying
 * implementation will have a template sort member function that
 * takes a comparitor, needed for dereferencing the elements
 * in the rw_slist<T*> impl_.
 *
 * So, for now, here is an insertion sort just like the one in "Algorithms"
 * by Robert Sedgwick, 2nd Edition, p.100:
 */

template <class T, class A>
void RWTPtrSlist<T, A>::sort()
{
    if (entries() < 2) {
        return;
    }

    const deref_less less_than = deref_less();

    container_type  dst_col;
    container_type& src_col = impl_;

    // just take the first element out of ourselves and
    // splice it into dst_col
    dst_col.splice(dst_col.end(), src_col, src_col.begin());

    for (iterator dst = dst_col.begin(); entries(); /**/) {
        iterator src = src_col.begin();

        // use `dst' as a partition point. if `*src' comes before `*dst' then
        // search from the front looking for an insertion point. otherwise we
        // start from `dst' and look for an insertion point. this should give
        // us a performance boost over a straight insertion sort, because we
        // would, on average, search through `n / 2' elements looking for the
        // insertion point.

        // the worst case scenario for this algorithm would be for a list to
        // already be sorted. in this case the complexity to insert a single
        // element would always be O(n), making the total complexity O(n^2),
        // which is the same worst case as the insertion sort that we used
        // previously.

        if (dst == dst_col.end()) {
            dst = dst_col.begin();
        }
        else if (less_than(*src, *dst)) {
            dst = dst_col.begin();
        }

        for (/**/; dst != dst_col.end(); ++dst) {
            if (!less_than(*dst, *src)) {
                break;
            }
        }

        dst_col.splice(dst, src_col, src);
    }

    // dst_col is now sorted, swap data back to ourselves
    dst_col.swap(src_col);
}


//===========================================

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS

template <class T, class A> bool
RWTPtrSlistIterator<T, A>::operator++()
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


template <class T, class A> bool
RWTPtrSlistIterator<T, A>::operator+=(size_t n)
{
    while (n-- && ++(*this)) {
        /* do nothing */
    }
    return !tail_ && !list_->isEmpty();
}

template <class T, class A> T*
RWTPtrSlistIterator<T, A>::findNext(const T* p)
{
    T* a;
    while ((a = (*this)()) != rwnil) {
        if (*a == *p) {
            return a;
        }
    }
    return rwnil;
}

template <class T, class A> T*
RWTPtrSlistIterator<T, A>
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

template <class T, class A> void
RWTPtrSlistIterator<T, A>::insertAfterPoint(T* a)
{
    RW_PRECONDITION2(!tail_,
                     "RWTPtrSlistIterator::insertAfterPoint(T* a): Attempt to "
                     "insertAfterPoint while iterator is past the end");

    typename RWTPtrSlist<T, A>::iterator i;

    if (head_) {
        list_->prepend(a);
    }
    else {
        i = iter_;
        ++i;
        list_->std().insert(i, a);
    }
}

template <class T, class A> T*
RWTPtrSlistIterator<T, A>::remove()
{
    typename RWTPtrSlist<T, A>::iterator i, j;
    T* ret;

    if (head_ || tail_) {
        return rwnil;    // iterator is not active
    }

    if (iter_ == list_->begin()) {
        ret = list_->removeFirst();
        head_ = true;
    }
    else {
        // As the manual warns, for slists this is "relatively inefficient:"
        i = list_->begin();
        do {
            j = i;
            ++i;
        }
        while (i != iter_);

        ret = *iter_;
        list_->std().erase(iter_);
        iter_ = j;
    }
    return ret;
}

template <class T, class A> T*
RWTPtrSlistIterator<T, A>::removeNext(const T* p)
{
    while (++(*this)) {
        if (*(*iter_)  == *p) {
            return remove();
        }
    }
    return rwnil;
}

template <class T, class A> T*
RWTPtrSlistIterator<T, A>
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

template <class T, class A> bool
RWTPtrSlistConstIterator<T, A>::operator++()
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


template <class T, class A> bool
RWTPtrSlistConstIterator<T, A>::operator+=(size_t n)
{
    while (n-- && ++(*this)) {
        /* do nothing */
    }
    return !tail_ && !list_->isEmpty();
}

template <class T, class A> const T*
RWTPtrSlistConstIterator<T, A>::findNext(const T* p)
{
    T* a;
    while ((a = const_cast<T*>((*this)())) != rwnil) {
        if (*a == *p) {
            return a;
        }
    }
    return rwnil;
}

template <class T, class A> const T*
RWTPtrSlistConstIterator<T, A>::findNext(bool (*testFun)(T*, void*),
        void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    T* a;
    while ((a = const_cast<T*>((*this)())) != rwnil) {
        if (testFun(a, d)) {
            return a;
        }
    }
    return rwnil;
}

RW_RESTORE_DEPRECATED_WARNINGS
#endif

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE(RWTPtrSlist)

template <class T, class S, class A>
void rwSaveGuts(S& str, const RWTPtrSlist<T, A>& c)
{
    str.putSizeT(c.entries());
    typename RWTPtrSlist<T, A>::const_iterator it
        = c.begin();
    while (it != c.end()) {
        str << *(*it);
        ++it;
    }
}

template <class T, class S, class A>
void rwRestoreGuts(S& str, RWTPtrSlist<T, A>& c)
{
    typedef typename RWTPtrSlist<T, A>::size_type size_type;
    size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        T* p = 0;
        str >> p;
        c.insert(p);
    }
}
