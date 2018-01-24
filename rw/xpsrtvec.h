#ifndef RW_TOOLS_XPSRTVEC_H
#define RW_TOOLS_XPSRTVEC_H



/**********************************************************************
 *
 * Sorted pointer-based vector.  Inserts pointers to values using an insertion sort.
 * Stores a pointer to the inserted item into the collection according
 * to an ordering determined by the less-than (<) operator.
 * Assumes that TP has:
 *   - well-defined equality semantics (TP::operator==(const TP&));
 *   - well-defined less-than semantics (TP::operator<(const TP&)).
 * This class uses binary searches for efficient value-based retrievals.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xpsrtvec.h#1 $
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
#include <rw/xpordvec.h>


template <class TP> class  RWClassicTPtrSortedVector : public RWClassicTPtrOrderedVector<TP>
{

public:

    //Constructors
    RWClassicTPtrSortedVector
    (
        size_t capac = RW_DEFAULT_CAPACITY
    ) :  RWClassicTPtrOrderedVector<TP>(capac) {
        ;
    }

    RWClassicTPtrSortedVector
    (
        const RWClassicTPtrSortedVector<TP>& c
    ) : RWClassicTPtrOrderedVector<TP>(c) { }

    // Overridden virtual functions
    virtual size_t    index(const TP* p) const;
    virtual void      insert(TP* p);
    virtual size_t    occurrencesOf(const TP* p) const;
    virtual TP*       remove(const TP* p);
    virtual size_t    removeAll(const TP* p);

protected:

    bool      bsearch(const TP*, size_t&) const; // binary search routine
    size_t        indexSpan(const TP*, size_t&) const;
#ifdef RW_DEBUG
    bool      isSorted() const;
#endif

};

#include <rw/edefs.h>
RW_DECLARE_PERSISTABLE_TEMPLATE(RWClassicTPtrSortedVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
class RW_TOOLS_GLOBAL RWpistream;
class RW_TOOLS_GLOBAL RWpostream;
class RW_TOOLS_GLOBAL RWbistream;
class RW_TOOLS_GLOBAL RWbostream;
class RW_TOOLS_GLOBAL RWeistream;
class RW_TOOLS_GLOBAL RWeostream;
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTPtrSortedVector, RWpistream, RWpostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTPtrSortedVector, RWbistream, RWbostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTPtrSortedVector, RWeistream, RWeostream)
#endif

template <class T, class S>
void rwSaveGuts(S& str, const RWClassicTPtrSortedVector<T>& c);

template <class T, class S>
void rwRestoreGuts(S& str, RWClassicTPtrSortedVector<T>& c);

template<class T>
int rwDerefCompare(const RWClassicTPtrSortedVector<T>& lhs, const RWClassicTPtrSortedVector<T>& rhs);

template <class T>
inline bool operator<(const RWClassicTPtrSortedVector<T>& lhs, const RWClassicTPtrSortedVector<T>& rhs)
{
    return (rwDerefCompare(lhs, rhs) < 0);
}

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xpsrtvec.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xpsrtvec.cc"
#endif


#endif  /* RW_TOOLS_XPSRTVEC_H */
