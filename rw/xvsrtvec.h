#ifndef RW_TOOLS_XVSRTVEC_H
#define RW_TOOLS_XVSRTVEC_H



/**********************************************************************
 *
 * Sorted value-based vector.  Inserts values using an insertion sort.
 * Stores a *copy* of the inserted item into the collection according
 * to an ordering determined by the less-than (<) operator.
 * Assumes that T has:
 *   - well-defined copy semantics (T::T(const T&) or equiv.);
 *   - well-defined assignment semantics (T::operator=(const T&) or equiv.);
 *   - well-defined equality semantics (T::operator==(const T&));
 *   - well-defined less-than semantics (T::operator<(const T&)).
 * Note that while these are automatically defined for builtin types
 * (such as "int", "double", or any pointer), you may need to provide
 * appropriate operators for your own classes, particularly those with
 * constructors and/or pointers to other objects.
 * This class uses binary searches for efficient value-based retrievals.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvsrtvec.h#1 $
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
#include <rw/xvordvec.h>


template <class T> class  RWClassicTValSortedVector : public RWClassicTValOrderedVector<T>
{

public:

    //Constructors
    RWClassicTValSortedVector
    (
        size_t capac = RW_DEFAULT_CAPACITY
    ) :  RWClassicTValOrderedVector<T>(capac) {
        ;
    }

    RWClassicTValSortedVector
    (
        const RWClassicTValSortedVector<T>& c
    ) : RWClassicTValOrderedVector<T>(c) {
        ;
    }

    // Overridden virtual functions
    virtual size_t    index(const T& p) const;
    virtual void      insert(const T& p);
    virtual size_t    occurrencesOf(const T& p) const;
    virtual bool  remove(const T& p);
    virtual size_t    removeAll(const T& p);

protected:

    bool      bsearch(const T&, size_t&) const;   // binary search routine
    size_t        indexSpan(const T&, size_t&) const;
#ifdef RW_DEBUG
    bool      isSorted() const;
#endif

};

#include <rw/edefs.h>
RW_DECLARE_PERSISTABLE_TEMPLATE(RWClassicTValSortedVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
class RW_TOOLS_GLOBAL RWpistream;
class RW_TOOLS_GLOBAL RWpostream;
class RW_TOOLS_GLOBAL RWbistream;
class RW_TOOLS_GLOBAL RWbostream;
class RW_TOOLS_GLOBAL RWeistream;
class RW_TOOLS_GLOBAL RWeostream;
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTValSortedVector, RWpistream, RWpostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTValSortedVector, RWbistream, RWbostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTValSortedVector, RWeistream, RWeostream)
#endif


template <class T, class S>
void rwSaveGuts(S& str, const RWClassicTValSortedVector<T>& c);

template <class T, class S>
void rwRestoreGuts(S& str, RWClassicTValSortedVector<T>& c);

template<class T>
int rwCompare(const RWClassicTValSortedVector<T>& lhs, const RWClassicTValSortedVector<T>& rhs);

template <class T>
inline bool operator<(const RWClassicTValSortedVector<T>& lhs, const RWClassicTValSortedVector<T>& rhs)
{
    return (rwCompare(lhs, rhs) < 0);
}

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xvsrtvec.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xvsrtvec.cc"
#endif


#endif  /* RW_TOOLS_XVSRTVEC_H */
