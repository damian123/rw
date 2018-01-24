#ifndef RW_TOOLS_EPFUNC_H
#define RW_TOOLS_EPFUNC_H

/**********************************************************************
 *
 * epfunc.h - work-around classes for problems with some STL functors
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/epfunc.h#3 $
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
#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4181)
#endif

#include <functional>

template <class Arg1, class Arg2, class Result>
class rw_pointer_to_binary_function
    : public std::binary_function<Arg1, Arg2, Result>
{
protected:
    Result(*ptr)(Arg1, Arg2);

public:
    rw_pointer_to_binary_function(Result(*x)(Arg1, Arg2))
        : ptr(x) {
    }

    Result operator()(Arg1 x, Arg2 y) const {
        return ptr(x, y);
    }
};

template <class Arg1, class Arg2, class Result>
rw_pointer_to_binary_function<Arg1, Arg2, Result> rw_ptr_fun(Result(*fn)(Arg1, Arg2))
{
    return rw_pointer_to_binary_function<Arg1, Arg2, Result>(fn);
}


template <class T>
class rw_binder2nd
    : public std::unary_function < typename T::first_argument_type,
      typename T::result_type >
{
    typedef typename T::first_argument_type  first_argument_type;
    typedef typename T::second_argument_type second_argument_type;
    typedef typename T::result_type          result_type;

protected:

    T op;
    second_argument_type value;

public:
    rw_binder2nd(const T& x, const second_argument_type y)
        : op(x), value(y) {
    }

    result_type operator()(first_argument_type x) const {
        return op(x, value);
    }
};

template <class T, class U>
rw_binder2nd<T> rw_bind2nd(const T& op, const U& x)
{
    return rw_binder2nd<T>(op, typename T::second_argument_type(x));
}


template <class T>
class rw_binder2nd_ref
    : public std::unary_function < typename T::first_argument_type,
      typename T::result_type >
{
    typedef typename T::first_argument_type  first_argument_type;
    typedef typename T::second_argument_type second_argument_type;
    typedef typename T::result_type          result_type;

protected:

    T op;
    const second_argument_type& value;

public:
    rw_binder2nd_ref(const T& x, const second_argument_type& y)
        : op(x), value(y) {
    }

    result_type operator()(const first_argument_type& x) const {
        return op(x, value);
    }
};

template <class T>
rw_binder2nd_ref<T> rw_bind2nd_ref(const T& op, const typename T::second_argument_type& x)
{
    return rw_binder2nd_ref<T>(op, x);
}


#ifdef _WIN32
#  pragma warning(pop)
#endif
#endif /* RW_TOOLS_EPFUNC_H */
