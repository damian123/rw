#ifndef RW_TOOLS_XPORDVEC_H
#define RW_TOOLS_XPORDVEC_H



/**********************************************************************
 *
 * RWClassicTPtrOrderedVector<TP>: Ordered collection of pointers to objects of type TP.
 * Stores a pointer to the inserted item into the collection.
 * Assumes that TP has:
 *   - well-defined equality semantics (TP::operator==(const TP&)).
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xpordvec.h#1 $
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
#include <rw/tpvector.h>
#include <rw/rwerr.h>
#include <rw/toolerr.h>

/****************************************************************
 *                              *
 *  Declarations for RWClassicTPtrOrderedVector<TP>     *
 *                              *
 ****************************************************************/


template <class TP> class  RWClassicTPtrOrderedVector : private RWTPtrVector<TP>
{

public:

    //Constructors:
    RWClassicTPtrOrderedVector(size_t capac = RW_DEFAULT_CAPACITY)
        : RWTPtrVector<TP>(capac),
          nitems_(0) {
        ;
    }

    //Destructor
    virtual ~RWClassicTPtrOrderedVector() {};

    TP*&      operator()(size_t i) {
#ifdef RWBOUNDS_CHECK
        boundsCheck(i);
#endif
        return RWTPtrVector<TP>::operator()(i);
    }
    TP* const& operator()(size_t i) const {
#ifdef RWBOUNDS_CHECK
        boundsCheck(i);
#endif
        return RWTPtrVector<TP>::operator()(i);
    }

    TP*&      operator[](size_t i) {
        boundsCheck(i);
        return RWTPtrVector<TP>::operator()(i);
    }
    TP* const& operator[](size_t i) const {
        boundsCheck(i);
        return RWTPtrVector<TP>::operator()(i);
    }

    //Member functions:
    void          append(TP* val) {
        insertAt(nitems_, val);
    }
    TP*&          at(size_t i) {
        return (*this)[i];
    }
    TP* const&     at(size_t i) const {
        return (*this)[i];
    }
    void          clear() {
        nitems_ = 0;
        if (RWTPtrVector<TP>::entries() > RW_DEFAULT_CAPACITY) {
            this->reshape(RW_DEFAULT_CAPACITY);
        }
    }
    void          clearAndDestroy();
    bool      contains(const TP* a) const {
        return index(a) != RW_NPOS;
    }
    TP* const*     data() const {
        return RWTPtrVector<TP>::data();
    }
    size_t        entries() const {
        return nitems_;
    }
    TP*           find(const TP* p) const;
    TP*&          first() {
        return (*this)(0);
    }
    TP* const&     first() const {
        return (*this)(0);
    }
    virtual size_t    index(const TP* p) const;
    virtual void      insert(TP* val) {
        insertAt(nitems_, val);
    }
    void          insertAt(size_t, TP*);
    bool      isEmpty() const {
        return nitems_ == 0;
    }
    TP*&              last() {
        return (*this)(nitems_ - 1);
    }
    TP* const&     last() const {
        return (*this)(nitems_ - 1);
    }
    size_t        length() const {
        return nitems_;
    }
    virtual size_t    occurrencesOf(const TP*) const;
    void          prepend(TP* val) {
        insertAt(0, val);
    }
    virtual TP*       remove(const TP* p);
    virtual size_t    removeAll(const TP*);
    TP*           removeAt(size_t);
    TP*           removeFirst() {
        return removeAt(0);
    }
    TP*           removeLast() {
        TP* ret = last();
        --nitems_;
        return ret;
    }

    void          resize(size_t N) {
        if (N >= nitems_) {
            this->reshape(N);
        }
    }

protected:
    void          boundsCheck(size_t i) const;

    size_t        nitems_;    // Number of items in the collection

};

#include <rw/edefs.h>
RW_DECLARE_PERSISTABLE_TEMPLATE(RWClassicTPtrOrderedVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
class RW_TOOLS_GLOBAL RWpistream;
class RW_TOOLS_GLOBAL RWpostream;
class RW_TOOLS_GLOBAL RWbistream;
class RW_TOOLS_GLOBAL RWbostream;
class RW_TOOLS_GLOBAL RWeistream;
class RW_TOOLS_GLOBAL RWeostream;
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTPtrOrderedVector, RWpistream, RWpostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTPtrOrderedVector, RWbistream, RWbostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTPtrOrderedVector, RWeistream, RWeostream)
#endif

template <class T, class S>
void rwSaveGuts(S& str, const RWClassicTPtrOrderedVector<T>& c);

template <class T, class S>
void rwRestoreGuts(S& str, RWClassicTPtrOrderedVector<T>& c);

template<class T>
bool rwDerefCompareEqual(const RWClassicTPtrOrderedVector<T>& lhs, const RWClassicTPtrOrderedVector<T>& rhs);

//make sure Serialize.h++ doesn't define this function again.
#ifndef RW_TVECTOR_EQUALITY_DECLARED
#  define RW_TVECTOR_EQUALITY_DECLARED 1
#endif

template <class T>
inline bool operator==(const RWClassicTPtrOrderedVector<T>& lhs, const RWClassicTPtrOrderedVector<T>& rhs)
{
    return ((lhs.length() == rhs.length()) && rwDerefCompareEqual(lhs, rhs));
}

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xpordvec.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xpordvec.cc"
#endif


#endif  /* RW_TOOLS_XPORDVEC_H */
