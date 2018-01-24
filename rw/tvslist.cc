/**********************************************************************
 *
 * Definitions for ./tvslist.cc
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvslist.cc#2 $
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

/****************************************************************
 *
 *  Definitions for RWTValSlistIterator
 *
 ****************************************************************/

#include <rw/epersist.h>

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS

template <class T, class A> bool
RWTValSlistIterator<T, A>::operator++()
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
RWTValSlistIterator<T, A>::operator+=(size_t n)
{
    while (n-- && ++(*this)) {
        /* do nothing */
    }
    return !tail_ && !list_->isEmpty();
}

template <class T, class A> bool
RWTValSlistIterator<T, A>::findNext(const T& val)
{
    while (++(*this)) {
        if (key() == val) {
            return true;
        }
    }
    return false;
}

template <class T, class A> bool
RWTValSlistIterator<T, A>
::findNext(bool (*testFun)(const T&, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    while (++(*this)) {
        if (testFun(key(), d)) {
            return true;
        }
    }
    return false;
}

template <class T, class A> void
RWTValSlistIterator<T, A>::insertAfterPoint(const T& a)
{
    RW_PRECONDITION2(!tail_,
                     "RWTValSlistIterator::insertAfterPoint(const T& a): Attempt to"
                     " insertAfterPoint while iterator is past the end");

    typename RWTValSlist<T, A>::iterator i;

    if (head_) {
        list_->prepend(a);
    }
    else {
        i = iter_;
        ++i;
        list_->std().insert(i, a);
    }
}

template <class T, class A> bool
RWTValSlistIterator<T, A>::remove()
{
    typename RWTValSlist<T, A>::iterator i, j;

    if (head_ || tail_) {
        return false;    // iterator is not active
    }

    if (iter_ == list_->begin()) {
        list_->removeFirst();
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

        list_->std().erase(iter_);
        iter_ = j;
    }
    return true;
}

template <class T, class A> bool
RWTValSlistIterator<T, A>::removeNext(const T& val)
{
    while (++(*this)) {
        if (*iter_  == val) {
            return remove();
        }
    }
    return false;
}

template <class T, class A> bool
RWTValSlistIterator<T, A>
::removeNext(bool (*testFun)(const T&, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    while (++(*this)) {
        if (testFun(*iter_, d)) {
            return remove();
        }
    }
    return false;
}


/****************************************************************
 *
 *      Definitions for RWTValSlistConstIterator
 *
 ****************************************************************/

template <class T, class A> bool
RWTValSlistConstIterator<T, A>::operator++()
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
RWTValSlistConstIterator<T, A>::operator+=(size_t n)
{
    while (n-- && ++(*this)) {
        /* do nothing */
    }
    return !tail_ && !list_->isEmpty();
}

template <class T, class A> bool
RWTValSlistConstIterator<T, A>::findNext(const T& val)
{
    while (++(*this)) {
        if (key() == val) {
            return true;
        }
    }
    return false;
}

template <class T, class A> bool
RWTValSlistConstIterator<T, A>::findNext(bool (*testFun)(const T&, void*),
        void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    while (++(*this)) {
        if (testFun(key(), d)) {
            return true;
        }
    }
    return false;
}

RW_RESTORE_DEPRECATED_WARNINGS
#endif

template <class T, class A>
typename RWTValSlist<T, A>::size_type
RWTValSlist<T, A>::index(bool (*fn)(const_reference, void*), void* x) const
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

template <class T, class A>
typename RWTValSlist<T, A>::size_type
RWTValSlist<T, A>::index(const_reference key) const
{
    size_type      idx  = 0;
    const_iterator iter = begin();
    while (iter != end()) {
        if (*iter == key) {
            return idx;
        }
        ++iter, ++idx;
    }

    return RW_NPOS;
}

template <class T, class A>
typename RWTValSlist<T, A>::value_type
RWTValSlist<T, A>::removeAt(size_type n)
{
#ifdef RW_DEBUG
    size_type n_dbg = entries();
#endif

    boundsCheck(n);
    iterator i = begin();
    std::advance(i, n);
    value_type ret = *i;
    erase(i);
    RW_POSTCONDITION(entries() == n_dbg - 1);
    return ret;
}

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE(RWTValSlist)

template <class T, class S, class A>
void rwSaveGuts(S& str, const RWTValSlist<T, A>& c)
{
    str.putSizeT(c.entries());
    typename RWTValSlist<T, A>::const_iterator it
        = c.begin();
    while (it != c.end()) {
        str << *it;
        ++it;
    }
}

template <class T, class S, class A>
void rwRestoreGuts(S& str, RWTValSlist<T, A>& c)
{
    typedef typename RWTValSlist<T, A>::size_type size_type;
    typename RWTValSlist<T, A>::size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        T t;
        str >> t;
        c.insert(t);
    }
}
