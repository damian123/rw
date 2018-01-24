#ifndef RW_TOOLS_EPAUX_H
#define RW_TOOLS_EPAUX_H

/**********************************************************************
 *
 * epaux.h - helper components for pointer-based STL wrapper classes
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/epaux.h#1 $
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
#include <iterator>
#include <functional>
#include <utility>

template <class T, class U>
struct rw_deref_pair_equal : std::binary_function <T, U, bool> {
    bool operator()(const T& x, const U& y) const {
        return *x.first == *y.first && *x.second == *y.second;
    }
};

template <class T, class U>
struct rw_deref_pair_less : std::binary_function <T, U, bool> {
    bool operator()(const T& x, const U& y) const {
        return (*x.first < *y.first) || (!(*y.first < *x.first) &&
                                         (*x.second < *y.second)) ;
    }
};

template <class Compare, class T>
struct rw_deref_compare : public std::binary_function<T*, T*, bool> {
    Compare cmp_;
    // Work around GCC optimizer bug that causes an uninitialized warning
    // to occur if a class instance does not have any non-empty class
    // members. (http://gcc.gnu.org/bugzilla/show_bug.cgi?id=40146)
#if defined(RW_GCC_VERSION) && (RW_GCC_VERSION >= 40400 && RW_GCC_VERSION < 40500)
    char dummy_;
#endif

    rw_deref_compare(const Compare& c = Compare())
        : cmp_(c)
#if defined(RW_GCC_VERSION) && (RW_GCC_VERSION >= 40400 && RW_GCC_VERSION < 40500)
        , dummy_(0)
#endif
    {
    }

    bool operator()(const T* const x, const T* const y) const {
        return cmp_(*x, *y);
    }
};

template <class Compare, class P>
struct rw_deref_pair_compare : public std::binary_function<P, P, bool> {
    Compare cmp_;

    rw_deref_pair_compare(const Compare& c = Compare())
        : cmp_(c) {
    }

    bool operator()(const P& x, const P& y) const {
        return cmp_(*x.first, *y.first);
    }
};

template <class Hash, class T>
struct rw_deref_hash : public std::unary_function<T*, unsigned> {
    Hash hash_;

    rw_deref_hash(const Hash& h = Hash())
        : hash_(h) {
    }

    unsigned operator()(const T* x) const {
        return unsigned(hash_(*x));
    }
};

template <class Compare, class P>
struct rw_pair_compare : public std::binary_function<P, P, bool> {
    Compare cmp_;

    rw_pair_compare(const Compare& c = Compare())
        : cmp_(c) {
    }

    bool operator()(const P& x, const P& y) const {
        return cmp_(x.first, y.first);
    }
};

// moved struct deref_match declaration from the template
// definition file ep_seq.cc to ep_seq.h for link time template instantiation
template <class PtrToConst>
struct rw_deref_match : public std::unary_function<PtrToConst, bool> {
    PtrToConst val;

    rw_deref_match(PtrToConst t)
        : val(t) {
    }

    bool operator()(PtrToConst t) const {
        return *t == *val;
    }
};

#endif /* RW_TOOLS_EPAUX_H */
