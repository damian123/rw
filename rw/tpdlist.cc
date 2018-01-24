/**********************************************************************
 *
 * tpdlist.cc - template definitions for RWTPtrDlist<T>
 *              and RWTPtrDlistIterator
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpdlist.cc#1 $
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

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS

template <class T, class A> bool
RWTPtrDlistIterator<T, A>::operator++()
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
RWTPtrDlistIterator<T, A>::operator--()
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

template <class T, class A> bool
RWTPtrDlistIterator<T, A>::operator+=(size_t n)
{
    while (n-- && ++(*this)) {
        /* do nothing */
    }
    return !tail_ && !list_->isEmpty();
}

template <class T, class A> bool
RWTPtrDlistIterator<T, A>::operator-=(size_t n)
{
    while (n-- && --(*this)) {
        /* do nothing */
    }
    return !head_;
}

template <class T, class A> T*
RWTPtrDlistIterator<T, A>::findNext(const T* p)
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
RWTPtrDlistIterator<T, A>::findNext(bool (*testFun)(T*, void*),
                                    void* d)
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
RWTPtrDlistIterator<T, A>::insertAfterPoint(T* a)
{
    RW_PRECONDITION2(!tail_, "RWTPtrDlistIterator::insertAfterPoint(T* a): Attempt to insertAfterPoint while iterator is past the end");

    typename RWTPtrDlist<T, A>::iterator i;

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
RWTPtrDlistIterator<T, A>::remove()
{
    typename RWTPtrDlist<T, A>::iterator i;

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

template <class T, class A> T*
RWTPtrDlistIterator<T, A>::removeNext(const T* p)
{
    while (++(*this)) {
        if (*(*iter_)  == *p) {
            return remove();
        }
    }
    return rwnil;
}

template <class T, class A> T*
RWTPtrDlistIterator<T, A>::removeNext(bool (*testFun)(T*, void*),
                                      void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    while (++(*this)) {
        if (testFun(*iter_, d)) {
            return remove();
        }
    }
    return rwnil;
}


/* RWTPtrDlistConstIterator */

template <class T, class A> bool
RWTPtrDlistConstIterator<T, A>::operator++()
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
RWTPtrDlistConstIterator<T, A>::operator--()
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

template <class T, class A> bool
RWTPtrDlistConstIterator<T, A>::operator+=(size_t n)
{
    while (n-- && ++(*this)) {
        /* do nothing */
    }
    return !tail_ && !list_->isEmpty();
}

template <class T, class A> bool
RWTPtrDlistConstIterator<T, A>::operator-=(size_t n)
{
    while (n-- && --(*this)) {
        /* do nothing */
    }
    return !head_;
}

template <class T, class A> const T*
RWTPtrDlistConstIterator<T, A>::findNext(const T* p)
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
RWTPtrDlistConstIterator<T, A>::findNext(bool (*testFun)(T*, void*),
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

template <class T, class A>
typename RWTPtrDlist<T, A>::size_type
RWTPtrDlist<T, A>::index(bool (*fn)(const T*, void*), void* x) const
{
    size_type idx = 0;
    for (const_iterator iter = begin(); iter != end(); ++iter, ++idx) {
        if ((*fn)(*iter, x)) {
            return idx;
        }
    }
    return RW_NPOS;
}

template <class T, class A>
typename RWTPtrDlist<T, A>::size_type
RWTPtrDlist<T, A>::index(bool (*fn)(value_type, void*),
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
typename RWTPtrDlist<T, A>::size_type
RWTPtrDlist<T, A>::index(const T* t) const
{
    size_type idx  = 0;
    for (const_iterator iter = begin(); iter != end(); ++iter, ++idx) {
        if (deref_equal()(*iter, t)) {
            return idx;
        }
    }
    return RW_NPOS;
}

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE(RWTPtrDlist)

template <class T, class S, class A>
void rwSaveGuts(S& str, const RWTPtrDlist<T, A>& c)
{
    str.putSizeT(c.entries());
    typename RWTPtrDlist<T, A>::const_iterator it = c.begin();
    while (it != c.end()) {
        str << *(*it);
        ++it;
    }
}

template <class T, class S, class A>
void rwRestoreGuts(S& str, RWTPtrDlist<T, A>& c)
{
    typedef typename RWTPtrDlist<T, A>::size_type size_type;
    size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        T* p = 0;
        str >> p;
        c.insert(p);
    }
}
