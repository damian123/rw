

/**********************************************************************
 *
 * Template definitions for RWClassicTPtrSortedVector<TP>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xpsrtvec.cc#1 $
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



template <class TP> size_t
RWClassicTPtrSortedVector<TP>::index(const TP* item) const
{
    size_t idx;
    if (! bsearch(item, idx)) { // Return RW_NPOS if item is not in collection:
        return RW_NPOS;
    }

    // The item is in the collection.
    // Search downwards looking for the first instance:

    while (idx && *(*this)(idx - 1) == *item) {
        --idx;
    }

    RW_POSTCONDITION(*(*this)(idx) == *item);
    RW_POSTCONDITION(idx < this->nitems_);

    return idx;
}

template <class TP> void
RWClassicTPtrSortedVector<TP>::insert(TP* p)
{
    // This algorithm uses only equality and less-than operators

#ifdef RW_DEBUG
    size_t count = occurrencesOf(p);
#endif

    size_t idx;
    if (bsearch(p, idx)) {

        // A matching item was found.  Insert after the
        // last equal item.
        while (idx < this->nitems_ && *(*this)(idx) == *p) {
            ++idx;
        }
    }
    else {
        // No matching item found.  Search upward
        // for the first item greater than the value
        // and insert before it.
        while (idx < this->nitems_ && *(*this)(idx) < *p) {
            ++idx;
        }
    }

    RWClassicTPtrOrderedVector<TP>::insertAt(idx, p);

    RW_POSTCONDITION(isSorted());
    RW_POSTCONDITION(occurrencesOf(p) == count + 1);
}

template <class TP> size_t
RWClassicTPtrSortedVector<TP>::occurrencesOf(const TP* p) const
{
    size_t iend;
    size_t istart = indexSpan(p, iend);
    return istart == RW_NPOS ? 0 : iend - istart + 1;
}

/*
 * Remove and return the first occurrence of an object with the
 * same value as the object pointed to by "p".
 */
template <class TP> TP*
RWClassicTPtrSortedVector<TP>::remove(const TP* p)
{
    size_t idx = index(p);
    return idx == RW_NPOS ? rwnil : this->removeAt(idx);
}

template <class TP> size_t
RWClassicTPtrSortedVector<TP>::removeAll(const TP* p)
{
    size_t iend;
    size_t istart = indexSpan(p, iend);

    if (istart == RW_NPOS) {
        return 0;
    }
    iend++;
    size_t nremoved = iend - istart;

    // Do a "solid body" slide left of the remaining items in the collection:
    while (iend < this->nitems_) {
        (*this)(istart++) = (*this)(iend++);
    }

    this->nitems_ -= nremoved;
    RW_POSTCONDITION(!this->contains(p));
    return nremoved;
}

/****************************************************************
 *                              *
 *  protected members of RWClassicTPtrSortedVector<TP>      *
 *                              *
 ****************************************************************/

template <class TP> bool
RWClassicTPtrSortedVector<TP>::bsearch(const TP* key, size_t& idx) const
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
            if (*key == *(*this)(idx)) {
                return true;
            }
            else if (*key < * (*this)(idx)) {
                top    = idx ? idx - 1 : 0u;
            }
            else {
                bottom = idx + 1;
            }
        }
        return *key == *(*this)(idx = bottom);
    }
    // Not found:
    return false;
}

template <class TP> size_t
RWClassicTPtrSortedVector<TP>::indexSpan(const TP* key, size_t& end) const
{
    // Do a binary search to find the first match:
    size_t istart = index(key);

    if (istart != RW_NPOS) {
        // Found one.  Do a linear search, looking for the last match:
        end = istart;
        while (end + 1 < this->entries() && *((*this)(end + 1)) == *key) {
            ++end;
        }
    }

    return istart;
}

#ifdef RW_DEBUG
template <class TP> bool
RWClassicTPtrSortedVector<TP>::isSorted() const
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
        if (!(*(*this)(idx) <  * (*this)(idx + 1))
                && !(*(*this)(idx) == *(*this)(idx + 1))
           ) {
            return false;
        }
    }
    return true;
}
#endif


#include <rw/defs.h>
#include <rw/epersist.h>

RW_DEFINE_PERSISTABLE_TEMPLATE(RWClassicTPtrSortedVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTPtrSortedVector, RWpistream, RWpostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTPtrSortedVector, RWbistream, RWbostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTPtrSortedVector, RWeistream, RWeostream)
#endif


template <class T, class S>
void rwSaveGuts(S& str, const RWClassicTPtrSortedVector<T>& c)
{
    str.putSizeT(c.entries());
    for (size_t i = 0; i < c.entries(); ++i) {
        str << *c[i];
    }
}

template <class T, class S>
void rwRestoreGuts(S& str, RWClassicTPtrSortedVector<T>& c)
{
    size_t count;
    str.getSizeT(count);
    c.clear();
    for (size_t i = 0; i < count; ++i) {
        T* p;
        str >> p;
        c.insert(p);
    }
}

template<class T>
int rwDerefCompare(const RWClassicTPtrSortedVector<T>& lhs, const RWClassicTPtrSortedVector<T>& rhs)
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
        if (!(*lhs[i] == *rhs[i])) {
            if (*lhs[i] < *rhs[i]) {
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
