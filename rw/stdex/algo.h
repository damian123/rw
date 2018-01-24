#ifndef __RWSTL_ALGORITHM_H__
#define __RWSTL_ALGORITHM_H__


/**********************************************************************
 *
 * STL-(almost)-compliant algorithms
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/stdex/algo.h#1 $
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


template <class IIter1, class IIter2, class OIter, class Compare>
void
// depends on the "inplace" container
//   -- being sorted
//   -- remaining sorted after an insert at any iterator
set_union_inplace(
    IIter1 first1, IIter1 bound1,
    IIter2 first2, IIter2 bound2,
    OIter self_inserter,
    Compare comp
)
{
    while (first1 != bound1 && first2 != bound2) {
        if (comp(*first1, *first2)) {
            ++first1;
        }
        else if (comp(*first2, *first1)) {
            self_inserter = *first2++;
        }
        else {
            ++first1;
            ++first2;
        }
    }
    while (first2 != bound2) {
        self_inserter = *first2++;
    }
}

template <class Container, class IIter1, class IIter2, class Compare>
void
// depends on the "inplace" container
//   -- being sorted
//   -- remaining sorted after a deletion at any iterator
set_intersection_inplace(
    Container& con,
    IIter1 first1, IIter1 bound1,
    IIter2 first2, IIter2 bound2,
    Compare comp
)
{
    while (first1 != bound1 && first2 != bound2) {
        if (comp(*first1, *first2)) {
            con.erase(first1++);
        }
        else if (comp(*first2, *first1)) {
            ++first2;
        }
        else {
            ++first1;
            ++first2;
        }
    }
    con.erase(first1, bound1);
}

template <class Container, class IIter1, class IIter2, class Compare>
void
// depends on the "inplace" container
//   -- being sorted
//   -- remaining sorted after a deletion at any iterator
set_difference_inplace(
    Container& con,
    IIter1 first1, IIter1 bound1,
    IIter2 first2, IIter2 bound2,
    Compare comp
)
{
    while (first1 != bound1 && first2 != bound2) {
        if (comp(*first1, *first2)) {
            ++first1;
        }
        else if (comp(*first2, *first1)) {
            ++first2;
        }
        else {
            con.erase(first1++);
            ++first2;
        }
    }
}

template <class Container, class IIter1, class IIter2, class Compare>
void
// depends on the "inplace" container
//   -- being sorted
//   -- remaining sorted after a deletion or insertion at any iterator
set_symmetric_difference_inplace(
    Container& con,
    IIter1 first1, IIter1 bound1,
    IIter2 first2, IIter2 bound2,
    Compare comp
)
{
    while (first1 != bound1 && first2 != bound2) {
        if (comp(*first1, *first2)) {
            first1++;
        }
        else if (comp(*first2, *first1)) {
            con.insert(*first2++);
        }
        else {
            con.erase(first1++);
            ++first2;
        }
    }
    while (first2 != bound2) {
        con.insert(*first2++);
    }
}

#endif /*__RWSTL_ALGORITHM_H__*/
