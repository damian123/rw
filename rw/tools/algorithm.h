#ifndef RW_TOOLS_ALGORITHM_H
#define RW_TOOLS_ALGORITHM_H


/**********************************************************************
 *
 * Wrappers around C++ Standard Library algorithms
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/algorithm.h#1 $
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
#include <rw/tools/iterator.h>

#include <algorithm>

/**
 * @internal
 * Wrapper around std::count, abstracting away platform-specific API
 * differences.
 */
template <typename I, typename T>
typename rw_iterator_traits<I>::difference_type rw_count(I beg, I end, const T& val)
{
#if !defined(RW_STDLIB_VER) || !defined(_RWSTD_NO_CLASS_PARTIAL_SPEC)
    return std::count(beg, end, val);
#else
    typename rw_iterator_traits<I>::difference_type ret = 0;
    std::count(beg, end, val, ret);
    return ret;
#endif
}

/**
 * @internal
 * Wrapper around std::count_if, abstracting away platform-specific API
 * differences.
 */
template <typename I, typename F>
typename rw_iterator_traits<I>::difference_type rw_count_if(I beg, I end, F fn)
{
#if !defined(RW_STDLIB_VER) || !defined(_RWSTD_NO_CLASS_PARTIAL_SPEC)
    return std::count_if(beg, end, fn);
#else
    typename rw_iterator_traits<I>::difference_type ret = 0;
    std::count_if(beg, end, fn, ret);
    return ret;
#endif
}

/**
 * @internal
 * Wrapper around std::count and std::replace. Counts the number of
 * instances of \a val, and replaces each with \a newVal.
 */
template <typename I, typename T>
typename rw_iterator_traits<I>::difference_type rw_count_and_replace(I beg, I end, const T& val, const T& newVal)
{
    typename rw_iterator_traits<I>::difference_type count = 0;
    for (I iter = beg; iter != end; ++iter) {
        if (*iter == val) {
            ++count;
            *iter = newVal;
        }
    }
    return count;
}

/**
 * @internal
 * Wrapper around std::count_if and std::replace_if. Counts the number of
 * items that returns \c true for \a fn, and replaces those items with the
 * value \a newVal.
 */
template <typename I, typename T, typename F>
typename rw_iterator_traits<I>::difference_type rw_count_and_replace_if(I beg, I end, F fn, const T& newVal)
{
    typename rw_iterator_traits<I>::difference_type count = 0;
    for (I iter = beg; iter != end; ++iter) {
        if (fn(*iter)) {
            ++count;
            *iter = newVal;
        }
    }
    return count;
}

/**
 * @internal
 * Helper function that allows deleting all of the items in a container.
 * This algorithm assumes that I is an iterator over pointer types.
 */
template <typename I>
void rw_destroy(I beg, I end)
{
    for (I iter = beg; iter != end; ++iter) {
        delete *iter;
    }
}

/**
 * @internal
 * Wrapper around std::distance, abstracting away platform-specific API
 * differences.
 */
template <typename I>
typename rw_iterator_traits<I>::difference_type rw_distance(I beg, I end)
{
#ifndef RW_3ARG_DISTANCE
    return std::distance(beg, end);
#else
    rw_iterator_traits<I>::difference_type ret = 0;
    std::distance(beg, end, ret);
    return ret;
#endif
}

/**
 * @internal
 * Implement std::is_sorted for platforms that do not provide it.
 */
template <typename I, typename C>
bool rw_is_sorted(I beg, I end, C cmp)
{
    if (beg == end) {
        return true;
    }

    I iter = beg;
    I prev = iter++;

    while (iter != end) {
        if (cmp(*iter, *prev)) {
            return false;
        }
        prev = iter++;
    }
    return true;
}

/**
 * @internal
 * Returns \c true if the items in the range [\a beg1, \a end1) are a subset
 * of the items in the range [\a beg2, \a end2), otherwise returns \c false.
 * Items in both ranges are assumed to already be in sorted order according
 * to \a cmp.
 */
template <typename I1, typename I2, typename C>
bool rw_is_subset(I1 beg1, I1 end1, I2 beg2, I2 end2, C cmp)
{
    I1 iter1 = beg1;
    I2 iter2 = beg2;

    while (iter1 != end1 && iter2 != end2) {
        if (cmp(*iter1, *iter2)) {
            return false;
        }
        if (cmp(*iter2, *iter1)) {
            ++iter2;
        }
        else {
            ++iter1;
            ++iter2;
        }
    }
    return iter1 == end1;
}

/**
 * @internal
 * Implement a helper for sorting a std::list abstracting away platform-
 * specific API differences.
 */
template <typename T, typename C>
void rw_list_sort(T& lst, C cmp)
{
#if !defined(RW_STDLIB_VER) || !defined(_RWSTD_NO_MEMBER_TEMPLATES)
    lst.sort(cmp);
#else
    typedef typename T::iterator iterator;
    typedef typename T::size_type size_type;
    typedef typename T::value_type value_type;

    if (lst.size() < 2) {
        return;
    }

    // Sort leading pairs
    iterator it = lst.begin();
    iterator pv;
    while (it != lst.end()) {
        pv = it++; // pv holds 'first', it holds 'second'
        if (it == lst.end()) {
            break;
        }
        if (cmp(*it, *pv)) {
            rw_swap(*pv, *it);
        }
        ++it; // it past 'second' to new 'first'
    }

    for (size_type i = 2; i < lst.size(); i *= 2) {
        size_type accumloc = 0;
        iterator i1 = lst.begin();
        while (accumloc < lst.size()) {
            size_type count1, count2;
            iterator i2(i1);
            if (accumloc < lst.size() - i) {
                std::advance(i2, i);
                count1 = i;
            }
            else {
                count1 = lst.size() - accumloc;
                i2 = lst.end();
            }
            accumloc += count1;
            iterator i3(i2);
            if (accumloc < lst.size() - i) {
                std::advance(i3, i);
                count2 = i;
            }
            else {
                i3 = lst.end();
                count2 = lst.size() - accumloc;
            }
            accumloc += count2;

            iterator stop1 = lst.insert(i2, *i1); // put dummy in place
            while (i1 != stop1 && i2 != i3) {
                if (cmp(*i2, *i1)) {
                    iterator start2(i2);
                    while (i2 != i3 && cmp(*i2, *i1)) {
                        ++i2;
                    }

                    lst.splice(i1, lst, start2, i2);
                }
                else {
                    ++i1;
                }
            }
            lst.erase(stop1);           // remove the dummy
            i1 = i3;
        }
    }
#endif
}

/**
 * @internal
 */
template <typename T, typename C>
void rw_bitvec_sort(T& vec, C cmp)
{
    if (vec.size() < 2) {
        return;
    }

    // Find iterators for first and last elements in the
    // collection.
    typedef typename T::iterator iterator;
    iterator ibeg = vec.begin();
    iterator iend = ibeg;
    std::advance(iend, vec.size() - 1);

    // Determine which value is expected to be first according
    // to the cmp function.
    bool first = cmp(false, true) ? false : true;

    while (ibeg < iend) {
        // Find the first item that is out of order
        if (cmp(first, *ibeg)) {
            do {
                // Find the last item that is out of order
                if (cmp(*iend, !first)) {
                    // swap values.
                    (*ibeg++).flip();
                    (*iend--).flip();
                    break;
                }
                --iend;
            }
            while (ibeg < iend);
        }
        else {
            ++ibeg;
        }
    }
}

#endif
