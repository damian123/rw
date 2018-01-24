

/**********************************************************************
 *
 * Template definitions for RWClassicTValSortedVector<T>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvsrtvec.cc#1 $
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



template <class T> size_t
RWClassicTValSortedVector<T>::index(const T& item) const
{
    size_t idx;
    if (! bsearch(item, idx)) { // Return RW_NPOS if item is not in collection:
        return RW_NPOS;
    }

    // The item is in the collection.
    // Search downwards looking for the first instance:

    while (idx && (*this)(idx - 1) == item) {
        --idx;
    }

    RW_POSTCONDITION((*this)(idx) == item);
    RW_POSTCONDITION(idx < this->nitems_);

    return idx;
}

template <class T> void
RWClassicTValSortedVector<T>::insert(const T& val)
{
#ifdef RW_DEBUG
    size_t count = occurrencesOf(val);
#endif

    size_t idx;
    if (bsearch(val, idx)) {

        // A matching item was found.  Insert after the
        // last equal item.
        while (idx < this->nitems_ && (*this)(idx) == val) {
            ++idx;
        }
    }
    else {
        // No matching item found.  Search upward
        // for the first item greater than the value
        // and insert before it.
        while (idx < this->nitems_ && (*this)(idx) < val) {
            ++idx;
        }
    }

    RWClassicTValOrderedVector<T>::insertAt(idx, val);
    RW_POSTCONDITION(isSorted());
    RW_POSTCONDITION(occurrencesOf(val) == count + 1);
}

template <class T> size_t
RWClassicTValSortedVector<T>::occurrencesOf(const T& val) const
{
    size_t iend;
    size_t istart = indexSpan(val, iend);
    return istart == RW_NPOS ? 0 : iend - istart + 1;
}

/*
 * Remove and return the first occurrence of an object == val
 */
template <class T> bool
RWClassicTValSortedVector<T>::remove(const T& val)
{
    size_t idx = index(val);

    if (idx == RW_NPOS) {
        return false;
    }

    this->removeAt(idx);
    return true;
}

template <class T> size_t
RWClassicTValSortedVector<T>::removeAll(const T& val)
{
    size_t iend;
    size_t istart = indexSpan(val, iend);

    if (istart == RW_NPOS) {
        return 0;
    }
    iend++;
    size_t nremoved = iend - istart;

    // Do a "solid body" slide left of the remaining items in the collection:
    while (iend < this->nitems_) {
        (*this)(istart++) = (*this)(iend++);
    }

    // reset now-unused cells to default value:
    T defaultValue = T();  // avoid "used before set" warning
    while (istart < this->nitems_) {
        (*this)(istart++) = defaultValue;
    }

    this->nitems_ -= nremoved;
    RW_POSTCONDITION(!this->contains(val));
    return nremoved;
}

/****************************************************************
 *                              *
 *  protected members of RWClassicTValSortedVector<T>       *
 *                              *
 ****************************************************************/

template <class T> bool
RWClassicTValSortedVector<T>::bsearch(const T& key, size_t& idx) const
{
    RW_PRECONDITION(isSorted());
    idx = 0;
    if (this->entries()) {
        size_t top = this->entries() - 1;
        size_t bottom = 0;

        while (top > bottom) {
            idx = (top + bottom) >> 1;
            // It is important that we use only the equality and less-than
            // operators:
            if (key == (*this)(idx)) {
                return true;
            }
            else if (key < (*this)(idx)) {
                top    = idx ? idx - 1 : 0u;
            }
            else {
                bottom = idx + 1;
            }
        }
        return key == (*this)(idx = bottom);
    }
    // Not found:
    return false;
}

template <class T> size_t
RWClassicTValSortedVector<T>::indexSpan(const T& key, size_t& end) const
{
    // Do a binary search to find the first match:
    size_t istart = index(key);

    if (istart != RW_NPOS) {
        // Found one.  Do a linear search, looking for the last match:
        end = istart;
        while (end + 1 < this->entries() && (*this)(end + 1) == key) {
            ++end;
        }
    }

    return istart;
}

#ifdef RW_DEBUG
template <class T> bool
RWClassicTValSortedVector<T>::isSorted() const
{
    // dispense with trivial cases:
    if (this->nitems_ < 2) {
        return true;
    }

    // otherwise:
    for (size_t idx = 0; idx < this->nitems_ - 1; idx++) {
        /*
         * Not sorted if the item at this index is not less
         * than and not equal to the item at the next index
         * (i.e., it is greater than).
         */
        if (!((*this)(idx) < (*this)(idx + 1))
                && !((*this)(idx) == (*this)(idx + 1))
           ) {
            return false;
        }
    }
    return true;
}
#endif


#include <rw/defs.h>
#define RW_USE_CLASSIC_TEMPLATES
#include <rw/epersist.h>

RW_DEFINE_PERSISTABLE_TEMPLATE(RWClassicTValSortedVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTValSortedVector, RWpistream, RWpostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTValSortedVector, RWbistream, RWbostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTValSortedVector, RWeistream, RWeostream)
#endif


template <class T, class S>
void rwSaveGuts(S& str, const RWClassicTValSortedVector<T>& c)
{
    str.putSizeT(c.entries());
    for (size_t i = 0; i < c.entries(); ++i) {
        str << c[i];
    }
}

template <class T, class S>
void rwRestoreGuts(S& str, RWClassicTValSortedVector<T>& c)
{
    size_t count;
    str.getSizeT(count);
    c.clear();
    for (size_t i = 0; i < count; ++i) {
        T t;
        str >> t;
        c.insert(t);
    }
}

template<class T>
int rwCompare(const RWClassicTValSortedVector<T>& lhs, const RWClassicTValSortedVector<T>& rhs)
{
    size_t l_len = lhs.length();
    size_t r_len = rhs.length();

    if (l_len == 0) {
        if (r_len == 0) {
            return 0;
        }
        else {
            return -1;
        }
    }
    if (r_len == 0) {
        return 1;
    }

    size_t count;
    if (r_len < l_len) {
        count = r_len;
    }
    else {
        count = l_len;
    }

    size_t i;
    for (i = 0; i < count; ++i) {
        if (!(lhs[i] == rhs[i])) {
            if (lhs[i] < rhs[i]) {
                return -1;
            }
            else {
                return 1;
            }
        }
    }

    if (l_len == r_len) {
        return 0;
    }
    else {
        if (l_len < r_len) {
            return -1;
        }
        else {
            return 1;
        }
    }
}
