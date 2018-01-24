

/**********************************************************************
 *
 * slist.cc - template definitions for rw_slist<T>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/stdex/slist.cc#4 $
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
#include <rw/stdex/destroy.h>
#include <rw/tools/iterator.h>

template<class T, class A>
typename rw_slist<T, A>::iterator
rw_slist<T, A>::insert(iterator it, const T& val)
{
    Node* nn = (Node*) RW_TLS_ALLOCATE(nodeAllocator, 1, Node);
    RW_ASSERT(nn != 0);

    // always use placement new
    new(nn) Node(val, *(it.prelink()));

    if (it.prelink() == tail_) {      // is iterator at end
        tail_ = nn->mynext();    // then update end() too.
    }

    *(it.prelink()) = nn;             // point list at node
    ++entries_;                       // update count of entries

    return it;
}

#if !defined(RW_NO_RVALUE_REFERENCES)
template<class T, class A>
typename rw_slist<T, A>::iterator
rw_slist<T, A>::insert(iterator it, T && val)
{
    Node* nn = (Node*) RW_TLS_ALLOCATE(nodeAllocator, 1, Node);
    RW_ASSERT(nn != 0);

    // always use placement new
    new(nn) Node(rw_move(val), *(it.prelink()));

    if (it.prelink() == tail_) {      // is iterator at end
        tail_ = nn->mynext();    // then update end() too.
    }

    *(it.prelink()) = nn;             // point list at node
    ++entries_;                       // update count of entries

    return it;
}
#endif // !RW_NO_RVALUE_REFERENCES


template<class T, class A> void
rw_slist<T, A>::  insert(iterator it, size_type n, const T& t)
{
    while (n--) {
        insert(it, t);
    }
}

template<class T, class A>
rw_slist<T, A>::rw_slist(size_type n, const T& value)
    : toHead_(0), head_(&toHead_), tail_(&toHead_), entries_(0)
{
    insert(begin(), n, value);
}


/*
 *  The remainder of this file is for member definitions that will never
 *  be seen from inside a class body.
 */

template <class T, class A>
rw_slist<T, A>::rw_slist()
    : toHead_(0), head_(&toHead_), tail_(&toHead_), entries_(0)
{
}


template <class T, class A>
rw_slist<T, A>::rw_slist(const rw_slist<T, A>& arg)
    : toHead_(0), head_(&toHead_), tail_(&toHead_), entries_(0)
{
    insert(begin(), arg.begin(), arg.end());
}

#if !defined(RW_NO_RVALUE_REFERENCES)

template <class T, class A>
rw_slist<T, A>::rw_slist(rw_slist<T, A> && arg)
    : toHead_(0), head_(&toHead_), tail_(&toHead_), entries_(0)
{
    arg.swap(*this);
}


template <class T, class A>
rw_slist<T, A>&
rw_slist<T, A>::operator=(rw_slist<T, A> && arg)
{
    if (&arg != this) {
        arg.swap(*this);
    }
    return *this;
}

#endif // !RW_NO_RVALUE_REFERENCES

template <class T, class A>
rw_slist<T, A>::~rw_slist()
{
    erase(begin(), end());
}

template <class T, class A>
rw_slist<T, A>&
rw_slist<T, A>::operator=(const rw_slist<T, A>& rhs)
{
    if (toHead_ != rhs.toHead_) {
        erase(begin(), end());
        insert(begin(), rhs.begin(), rhs.end());
    }
    return *this;
}

template <class T, class A>
template <class InputIterator>
void
rw_slist<T, A>::assign(InputIterator first, InputIterator last)
{
    rw_slist<T, A> tmp(first, last);
    swap(tmp);
}


template <class T, class A>
void rw_slist<T, A>::swap(rw_slist<T, A>& s)
{
    Node**    n = (s.entries_) ? s.tail_ : &toHead_;
    s.tail_     = (entries_)   ? tail_   : &(s.toHead_);
    tail_    = n;

    rw_swap(s.toHead_, toHead_);
    rw_swap(s.entries_, entries_);
}


template <class T, class A>
typename rw_slist<T, A>::Iterator
rw_slist<T, A>::erase(const_iterator it)
{
    /** precondition: it is dereferencable **/
    /** precondition: ++it exists **/
    /** bounds check for it != end()??? **/
    Node** pVictim = it.prelink();
    Node*  victim  = *pVictim;  // get ptr to victim

    if (victim->mynext() == tail_) { // is it the last link?
        tail_ = pVictim;
    }

    *pVictim = victim->next;    // link around victim

    entries_--;
    if (entries_ == 0) {      // keep tally
        tail_ = &toHead_;       // and update tail_
        /** not strictly needed, but makes all vslist.end() equal **/
        toHead_ = 0;        // ... and toHead
    }

    rw_destroy(&(victim->item()));
    RW_TLS_DEALLOCATE(nodeAllocator, victim, 1);

    /** postcondition loc is now what ++it was **/
    return iterator(it.prelink());
}

template <class T, class A>
typename rw_slist<T, A>::iterator
rw_slist<T, A>::erase(const_iterator first, const_iterator last)
{
    Node* temp = *last.prelink();

    iterator ret(first.prelink());
    while (*ret.prelink() != temp) {
        ret = erase(ret);
    }

    return ret;
}

template <class T, class A>
void rw_slist<T, A>::splice(iterator pos, rw_slist<T, A>& donor, iterator item)
{
    Node* temp = *(item.prelink());
    if (temp->mynext() == donor.tail_) { // is it the last link?
        donor.tail_ = item.prelink();    // then backup tail_
    }

    *(item.prelink()) = temp->next; // point item past link
    temp->next = *(pos.prelink());  // point link into new list
    if (pos == end()) {   // at our end?
        tail_ = temp->mynext();    // advance our tail_
    }

    *(pos.prelink()) = temp;    // point list at the link
    --(donor.entries_);     // and fix sizes
    ++entries_;       // ...
}

template <class T, class A>
void rw_slist<T, A>::splice(iterator pos, rw_slist<T, A>& donor,
                            iterator first, iterator last)
{
    if (first != last) {
        difference_type n = 0;
        if (&donor != this) {
#ifndef RW_3ARG_DISTANCE
            n = std::distance(first, last); // here is the O(n) part. Sigh.
#else
            std::distance(first, last, n); // here is the O(n) part. Sigh.
#endif
        }
        // Verify that a positive distance was returned and the cast to
        // size_t is safe.
        RW_PRECONDITION(n >= 0);
        cutPaste(pos, static_cast<size_type>(n), donor, first, last);
    }
}

template <class T, class A>
typename rw_slist<T, A>::iterator
rw_slist<T, A>::cutPaste(iterator pos, size_type n,
                         rw_slist<T, A>& ref, iterator first, iterator last)
{
    // protected method to do splicing with length knowledge already available
    Node* donorEnd = *last.prelink();
    Node* donorStart = *first.prelink();
    if (*last.prelink() == *ref.tail_) {
        ref.tail_ = first.prelink();
    }
    *(last.prelink()) = *(pos.prelink());   // merge donor end into list

    if (*pos.prelink() == *tail_) {
        tail_ = last.prelink();
    }
    *pos.prelink() = donorStart; // merge us onto begin
    *first.prelink() = donorEnd; // close up donor list

    if (n) {
        entries_ += n;
        ref.entries_ -= n; /** error/bounds check here? **/
    }
    pos.prelink() = donorEnd->mynext(); // build a ret value
    return last;
}

template <class T, class A> void
rw_slist<T, A>::remove(const T& val)
{
    // depends on erase(iterator) leaving iterator sane, on next item
    iterator hunt = begin();
    while (hunt != end())
        if (*hunt == val) {
            erase(hunt);
        }
        else {
            ++hunt;
        }
}

template <class T, class A> void
rw_slist<T, A>::unique()
{
    // depends on erase(iterator) leaving iterator sane, on 'next' item
    iterator hunt = begin();

    // use T* to avoid assuming T::operator=()
    T* first = &*(hunt++);

    // which may not exist (e.g. if T is pair<const ...>)
    while (hunt != end())
        if (*first == *hunt) {
            erase(hunt);
        }
        else {
            first = &*(hunt++);
        }

}

template <class T, class A> void
rw_slist<T, A>::merge(rw_slist<T, A>& input)
{
    // easy case is worth checking
    if (input.empty()) {
        return;
    }

    iterator it = begin();
    iterator in = input.begin();

    while (it != end() && in != input.end()) {
        if (*in < *it) {
            size_type n = 0;
            iterator tmp = in;
            while (in != input.end() && *in < *it) {
                ++in;
                ++n;
            }
            it = cutPaste(it, n, input, tmp, in);
            in = input.begin(); // always, after we merge something
        }
        else {
            ++it;
        }
    }

    if (in != input.end()) {
        splice(it, input, in, input.end());
    }
}

template <class T, class A> void
rw_slist<T, A>::reverse()
{
    if (! empty()) {
        iterator stop = end();
        while (++begin() != stop) {
            splice(stop, *this, begin());
        }
    }
}

template <class T, class A> void
rw_slist<T, A>::sortPairs()
{
    iterator it = begin();
    iterator nx = it;
    while (it != end() && nx != end()) {
        nx = it;
        ++nx;
        if (nx.isValid() && nx != end() && *nx < *it) {
            Node* b = *it.prelink();
            Node* a = *nx.prelink();
            Node* c = a->next;
            if (a->mynext() == tail_) {
                tail_ = b->mynext();
            }
            *it.prelink() = a;
            a->next = b;
            b->next = c;
            it = nx;      // swapping advanced nx;
        }
        else {
            it = nx;
            if (it.isValid()) {
                ++it;
            }
        }
    }
}

template <class T, class A>
typename rw_slist<T, A>::Iterator
rw_slist<T, A>::adjacentMerge(iterator i1, iterator i2, iterator i3,
                              size_type count1, size_type count2)
{
    // merge [i1,i2) and [i2,i3) assuming they are sorted
    // cope correctly with edge conditions; return iter 'just past
    // second interval' (where i3 would be without the merge)

    if (i1 == i2 || i2 == i3) {
        return i3;    // handle empty ranges
    }

    rw_slist<T, A> t1, t2;

    t1.cutPaste(t1.end(), count1 + count2, *this, i1, i3);
    t2.cutPaste(t2.end(), count2, t1, i2, t1.end());

    t1.merge(t2);

    return cutPaste(i1, t1.size(), t1, t1.begin(), t1.end());
}

template <class T, class A>
void rw_slist<T, A>::sort()
{
    // algorithm is:
    // for i in 2,4,...2^n... size()/2
    //   merge-sort i-length adjacent sub-lists
    // This will pass through the list about ln(size()) times for
    // total of about N*log(N) comparisons.

    if (size() < 2) {
        return;
    }
    sortPairs();
    for (size_type i = 2; i < size(); i *= 2) {
        size_type accumloc = 0;
        iterator i1 = begin();
        while (accumloc < size()) {
            size_type count1, count2;
            iterator i2(i1);
            if (accumloc < size() - i) {
                std::advance(i2, i);
                count1 = i;;
            }
            else {
                count1 = size() - accumloc;
                i2 = end();
            }
            accumloc += count1;
            iterator i3(i2);
            if (accumloc < size() - i) {
                std::advance(i3, i);
                count2 = i;
            }
            else {
                i3 = end();
                count2 = size() - accumloc;
            }
            accumloc += count2;
            i1 = adjacentMerge(i1, i2, i3, count1, count2);
        }
    }
}

#if 0 /* a hack to provide "almost" member template stuff */

template <class T, class A>
void rw_slist<T, A>::remove_if(UnaryPredicate<T, bool>& p)
{
    // depends on erase(iterator) leaving iterator sane, on next item
    iterator hunt = begin();
    while (hunt != end()) {
        if (p(*hunt)) {
            erase(hunt);
        }
        else {
            ++hunt;
        }
    }
}

template <class T, class A>
void rw_slist<T, A>::unique(BinaryPredicate<T, T, bool>& p)
{
    // depends on erase(iterator) leaving iterator sane, on 'next' item
    iterator hunt = begin();
    T first = *(hunt++);
    while (hunt != end()) {
        if (p(first, *hunt)) {
            erase(hunt);
        }
        else {
            first = *(hunt++);
        }
    }
}

template <class T, class A>
void rw_slist<T, A>::merge(rw_slist<T, A>& input, Comparison<T, A>& comp)
{
    // easy case is worth checking
    if (input.empty()) {
        return;
    }

    iterator it = begin();
    iterator in = input.begin();
    while (it != end() && in != input.end()) {
        if (comp(*in, *it)) {
            size_type n = 0;
            iterator front = in;
            while (in != input.end() && comp(*in, *it)) {
                ++in;
                ++n;
            }
            it = cutPaste(it, n, input, front, in);
            in = input.begin(); // always, after we merge something
        }
        else {
            ++it;
        }
    }
    if (in != input.end()) {
        splice(it, input, in, input.end());
    }
}

template <class T, class A>
void rw_slist<T, A>::sortPairs(Comparison<T, A>& comp)
{
    iterator it = begin();
    iterator nx = it;
    while (it != end() && nx != end()) {
        nx = it;
        ++nx;
        if (nx.isValid() && comp(*nx, *it)) {
            Node* b = *it.prelink();
            Node* a = *nx.prelink();
            Node* c = a->next;
            if (a->mynext() == tail_) {
                tail_ = b->mynext();
            }
            *it.prelink() = a;
            a->next = b;
            b->next = c;
            it = nx;      // swapping advanced nx;
        }
        else {
            it = nx;
            if (it.isValid()) {
                ++it;
            }
        }
    }
}

template <class T, class A>
rw_slist<T, A>::iterator
rw_slist<T, A>::adjacentMerge(iterator i1, iterator i2, iterator i3,
                              size_type count1, size_type count2,
                              Comparison<T, A>& comp)
{
    // merge [i1,i2) and [i2,i3) assuming they are sorted
    // cope correctly with edge conditions; return iter \'just past
    // second interval\' (where i3 would be without the merge)

    if (i1 == i2 || i2 == i3) {
        return i3;    // handle empty ranges
    }
    rw_slist<T, A> t1, t2;
    t1.cutPaste(t1.end(), count1 + count2, *this, i1, i3);
    t2.cutPaste(t2.end(), count2, t1, i2, t1.end());
    t1.merge(t2, comp);
    return cutPaste(i1, t1.size(), t1, t1.begin(), t1.end());
}

template <class T, class A>
void rw_slist<T, A>::sort(Comparison<T, A>& comp)
{
    // algorithm is:
    // for i in 2,4,...2^n... size()/2
    //   merge-sort i-length adjacent sub-lists
    // This will pass through the list about ln(size()) times for
    // total of about N*log(N) comparisons.

    if (size() < 2) {
        return;
    }
    sortPairs(comp);
    for (size_type i = 2; i < size(); i *= 2) {
        size_type accumloc = 0;
        iterator i1 = begin();
        while (accumloc < size()) {
            size_type count1, count2;
            iterator i2(i1);
            if (accumloc < size() - i) {
                std::advance(i2, i);
                count1 = i;;
            }
            else {
                count1 = size() - accumloc;
                i2 = end();
            }
            accumloc += count1;
            iterator i3(i2);
            if (accumloc < size() - i) {
                std::advance(i3, i);
                count2 = i;
            }
            else {
                i3 = end();
                count2 = size() - accumloc;
            }
            accumloc += count2;
            i1 = adjacentMerge(i1, i2, i3, count1, count2, comp);
        }
    }
}

template <class T, class A> void
rw_slist<T, A>::showOff(const char* s) const
{
    cout << s << ":" << flush;
    char sep = ' ';
    for (const_iterator i = begin(); i != end(); ++i) {
        cout << sep << *i << flush;
        sep = ',';
    }
    cout << "; len=" << size() << endl;
}

template <class T, class A> void
rw_slist<T, A>::iprint(const char* p,
                       rw_slist<T, A>::const_iterator i,
                       const char* t) const
{
    cout << p;
    if (i) {
        cout << *i << flush;
    }
    else {
        cout << "\"nil\"" << flush;
    }
    size_type dist = 0;
    std::distance(begin(), i, dist);
    cout << "<" << dist << ">";
    cout << t;
}
#endif
