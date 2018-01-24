#ifndef RW_TOOLS_XVORDVEC_H
#define RW_TOOLS_XVORDVEC_H



/**********************************************************************
 *
 * RWClassicTValOrderedVector<T>: Ordered collection of objects of type T.
 * Stores a *copy* of the inserted item into the collection.
 * Assumes that T has:
 *   - well-defined copy semantics (T::T(const T&) or equiv.);
 *   - well-defined assignment semantics (T::operator=(const T&) or equiv.);
 *   - well-defined equality semantics (T::operator==(const T&)).
 * Note that while these are automatically defined for builtin types
 * (such as "int", "double", or any pointer), you may need to provide
 * appropriate operators for your own classes, particularly those with
 * constructors and/or pointers to other objects.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvordvec.h#1 $
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
#include <rw/tvvector.h>
#include <rw/rwerr.h>
#include <rw/toolerr.h>

/****************************************************************
 *                              *
 *  Declarations for RWClassicTValOrderedVector<T>          *
 *                              *
 ****************************************************************/


template <class T> class  RWClassicTValOrderedVector : private RWTValVector<T>
{

public:

    //Constructors:
    RWClassicTValOrderedVector(size_t capac = RW_DEFAULT_CAPACITY)
        : RWTValVector<T>(capac),
          nitems_(0) {
        ;
    }

    // Destructor
    virtual ~RWClassicTValOrderedVector() {}

    //Operators:
    T&        operator[](size_t i) {
        boundsCheck(i);
        return RWTValVector<T>::operator()(i);
    }

    const T&  operator[](size_t i) const {
        boundsCheck(i);
        return RWTValVector<T>::operator()(i);
    }

    T&        operator()(size_t i) {
#ifdef RWBOUNDS_CHECK
        boundsCheck(i);
#endif
        return RWTValVector<T>::operator()(i);
    }

    const T&  operator()(size_t i) const {
#ifdef RWBOUNDS_CHECK
        boundsCheck(i);
#endif
        return RWTValVector<T>::operator()(i);
    }


    //Member functions:
    void          append(const T& val) {
        insertAt(nitems_, val);
    }
    T&            at(size_t i) {
        return (*this)[i];
    }
    const T&      at(size_t i) const {
        return (*this)[i];
    }
    void          clear();
    bool      contains(const T& a) const {
        return index(a) != RW_NPOS;
    }
    const T*      data() const {
        return RWTValVector<T>::data();
    }
    size_t        entries() const {
        return nitems_;
    }
    bool      find(const T& p, T& ret) const;
    T&                first() {
        return (*this)(0);
    }
    const T&      first() const {
        return (*this)(0);
    }
    virtual size_t    index(const T& p) const;
    virtual void      insert(const T& val) {
        insertAt(nitems_, val);
    }
    void          insertAt(size_t, const T&);
    bool      isEmpty() const {
        return nitems_ == 0;
    }
    T&            last() {
        return (*this)(nitems_ - 1);
    }
    const T&      last() const {
        return (*this)(nitems_ - 1);
    }
    size_t        length() const {
        return nitems_;
    }
    virtual size_t    occurrencesOf(const T&) const;
    void          prepend(const T& val) {
        insertAt(0, val);
    }
    virtual bool  remove(const T& p);
    virtual size_t    removeAll(const T&);
    T         removeAt(size_t);
    T         removeFirst() {
        return removeAt(0);
    }
    T         removeLast();
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
RW_DECLARE_PERSISTABLE_TEMPLATE(RWClassicTValOrderedVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
class RW_TOOLS_GLOBAL RWpistream;
class RW_TOOLS_GLOBAL RWpostream;
class RW_TOOLS_GLOBAL RWbistream;
class RW_TOOLS_GLOBAL RWbostream;
class RW_TOOLS_GLOBAL RWeistream;
class RW_TOOLS_GLOBAL RWeostream;
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTValOrderedVector, RWpistream, RWpostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTValOrderedVector, RWbistream, RWbostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTValOrderedVector, RWeistream, RWeostream)
#endif


template <class T, class S>
void rwSaveGuts(S& str, const RWClassicTValOrderedVector<T>& c);

template <class T, class S>
void rwRestoreGuts(S& str, RWClassicTValOrderedVector<T>& c);

template<class T>
bool rwCompareEqual(const RWClassicTValOrderedVector<T>& lhs, const RWClassicTValOrderedVector<T>& rhs);

//make sure Serialize.h++ doesn't define this function again.

#ifndef RW_TVECTOR_EQUALITY_DECLARED
#  define RW_TVECTOR_EQUALITY_DECLARED 1
#endif

template <class T>
inline bool operator==(const RWClassicTValOrderedVector<T>& lhs, const RWClassicTValOrderedVector<T>& rhs)
{
    return ((lhs.length() == rhs.length()) && rwCompareEqual(lhs, rhs));
}


#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xvordvec.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xvordvec.cc"
#endif


#endif  /* RW_TOOLS_XVORDVEC_H */
