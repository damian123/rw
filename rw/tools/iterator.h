#ifndef rw_tools_iterator_h_
#define rw_tools_iterator_h_

/**********************************************************************
 *
 * Wrappers around C++ Standard Library iterators
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/iterator.h#2 $
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

#if !defined(RW_STDLIB_VER) || !defined(_RWSTD_NO_CLASS_PARTIAL_SPEC)
#  define rw_iterator_traits std::iterator_traits
#else

template <typename I>
struct rw_iterator_traits {
    typedef typename I::value_type value_type;
    typedef typename I::difference_type difference_type;
    typedef typename I::pointer pointer;
    typedef typename I::reference reference;
    typedef typename I::iterator_category iterator_category;
};

#if defined(__SUNPRO_CC)
// work around native Standard Library bug with Oracle Solaris Studio
// reverse_iterator member types are inaccessible. SR 3-10373392271
template <typename T, typename U, typename V, typename W, typename X, typename Y>
struct rw_iterator_traits<std::__reverse_bi_iterator<T, U, V, W, X, Y> > {
    typedef typename V value_type;
    typedef typename Y difference_type;
    typedef typename X pointer;
    typedef typename W reference;
    typedef typename U iterator_category;
};
#endif

template <typename T>
struct rw_iterator_traits<T*> {
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::random_access_iterator_tag iterator_category;
};

template <typename T>
struct rw_iterator_traits<const T*> {
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef std::random_access_iterator_tag iterator_category;
};

#endif // !defined(RW_STDLIB_VER) || !defined(_RWSTD_NO_CLASS_PARTIAL_SPEC)

#endif // rw_tools_iterator_h_
