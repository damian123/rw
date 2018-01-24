/**********************************************************************
 *
 * Template definitions for RWTValHashMultiSet
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvhasht.cc#1 $
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

#include <rw/epersist.h>

template < class T, class H, class EQ, class A>
void
RWTValHashMultiSet<T, H, EQ, A>::difference(const container_type& stdcoll)
{
    // depends on all eq items being adjacent in each collection
    typename container_type::key_equal_type eq = std().key_equal();
    typename container_type::const_iterator look = begin();
    while (look != end()) {
        typename container_type::const_iterator probe = stdcoll.find(*look);
        const typename RWTValHashMultiSet<T, H, EQ, A>::value_type match = *look;
        while (probe != stdcoll.end() && look != end()
                && eq(match, *look) && eq(*look, *probe)) {
            // if there is a match, then erase (them) in sequence
            look = std().erase(look); // depends on "next iter" semantics
            ++probe;
        }
        // no match, or no more match: skip (trailing) part in self
        while (look != end() && eq(match, *look)) {
            ++look;
        }
    }
}

template < class T, class H, class EQ, class A>
void
RWTValHashMultiSet<T, H, EQ, A>::intersection(const container_type& stdcoll)
{
    // depends on all eq items being adjacent in each collection
    typename container_type::key_equal_type eq = std().key_equal();
    typename container_type::const_iterator look = std().begin();
    while (look != std().end()) {
        typename RWTValHashMultiSet<T, H, EQ, A>::const_iterator probe = stdcoll.find(*look);
        const typename container_type::value_type match = *look;
        while (probe != stdcoll.end() && look != std().end()
                && eq(match, *look) && eq(*look, *probe)) {
            // if we have a match, then march in sequence 1 or more times
            ++probe;
            ++look;
        }
        // no match, or no _more_ match: remove (trailing) part in self
        while (look != std().end() && eq(match, *look)) {
            look = std().erase(look);    // depends on "next iter" semantics
        }
    }
}

template < class T, class H, class EQ, class A>
typename RWTValHashMultiSet<T, H, EQ, A>::size_type
RWTValHashMultiSet<T, H, EQ, A>::removeAll(bool (*fn)(const_reference, void*), void* x)
{
    size_type    ret = entries();

    while (entries() && (*fn)(*(begin()), x)) {
        std().erase(begin());
    }

    iterator iter = begin();

    while (iter != end()) {
        iterator iter2 = iter;
        ++iter2;
        if (iter2 != end() && (*fn)(*iter2, x)) {
            std().erase(iter2);
        }
        else {
            ++iter;
        }
    }

    return ret - entries();
}

template < class T, class H, class EQ, class A>
void
RWTValHashMultiSet<T, H, EQ, A>::symmetricDifference(const container_type& stdcoll)
{
    // depends on all eq items being adjacent in each collection
    typename container_type::key_equal_type eq = std().key_equal();
    typename container_type::const_iterator probe = stdcoll.begin();
    while (probe != stdcoll.end()) {
        typename container_type::iterator found = std().find(*probe);
        if (found != end()) {       // cope with (multiple) match(es)
            const typename RWTValHashMultiSet<T, H, EQ, A>::value_type match = *found;
            while (found != end() && probe != stdcoll.end()
                    && eq(match, *found) && eq(*found, *probe)) {
                // if we have a match, then remove in sequence 1 or more times
                found = std().erase(found);
                ++probe;
            }
            // if no match, or no _more_ match; insert (remainder) into self
            while (probe != stdcoll.end() && eq(match, *probe)) {
                std().insert(*probe++);
            }
        }
        else {                      // self has no match to *probe
            const typename RWTValHashMultiSet<T, H, EQ, A>::value_type& match = *probe;
            while (probe != stdcoll.end() && eq(match, *probe)) {
                std().insert(*probe++);
            }
        }
    }
}

template < class T, class H, class EQ, class A>
void
RWTValHashMultiSet<T, H, EQ, A>::Union(const container_type& stdcoll)
{
    // depends on all eq items being adjacent in each collection
    typename container_type::key_equal_type eq = std().key_equal();
    typename container_type::const_iterator probe = stdcoll.begin();
    while (probe != stdcoll.end()) {
        typename container_type::iterator found = std().find(*probe);
        if (found != end()) {       // cope with (multiple) match(es)
            const typename RWTValHashMultiSet<T, H, EQ, A>::value_type& match = *found;
            while (found != end() && probe != stdcoll.end()
                    && eq(match, *found) && eq(*found, *probe)) {
                // if we have a match, then march in sequence 1 or more times
                ++found;
                ++probe;
            }
            // if no match, or no _more_ match; insert (remainder) into self
            while (probe != stdcoll.end() && eq(match, *probe)) {
                std().insert(*probe++);
            }
        }
        else {                      // self has no match to *probe
            const typename RWTValHashMultiSet<T, H, EQ, A>::value_type& match = *probe;
            while (probe != stdcoll.end() && eq(match, *probe)) {
                std().insert(*probe++);
            }
        }
    }
}

RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_3(RWTValHashMultiSet)

template < class T, class H, class EQ, class S, class A>
void rwSaveGuts(S& str, const RWTValHashMultiSet<T, H, EQ, A>& c)
{
    str.putSizeT(c.entries());
    typename RWTValHashMultiSet<T, H, EQ, A>::const_iterator it
        = c.begin();

    while (it != c.end()) {
        str << *it;
        it++;
    }
}

template < class T, class H, class EQ, class S, class A>
void rwRestoreGuts(S& str, RWTValHashMultiSet<T, H, EQ, A>& c)
{
    typedef typename RWTValHashMultiSet<T, H, EQ, A>::size_type size_type;
    typename RWTValHashMultiSet<T, H, EQ, A>::size_type count;
    str.getSizeT(count);
    c.clear();
    for (size_type i = 0; i < count; ++i) {
        T data;
        str >> data;
        c.insert(data);
    }
}
