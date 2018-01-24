#ifndef RW_TOOLS_XVDLIST_H
#define RW_TOOLS_XVDLIST_H


/**********************************************************************
 *
 * RWClassicTValDlist<T>: Doubly-linked list of values of type T.
 * Stores a *copy* of the inserted item into the collection.
 * Assumes that T has:
 *   - well-defined copy constructor (T::T(const T&) or equiv.);
 *   - well-defined assignment operator (T::operator=(const T&) or equiv.);
 *   - well-defined equality semantics (T::operator==(const T&)).
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvdlist.h#1 $
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
#include <rw/tidlist.h>

template <class T> class  RWClassicTValDlistIterator;

/****************************************************************
 *                              *
 *       Declarations for RWTValDlink<T>        *
 *                              *
 ****************************************************************/

/*
 * This is the actual link that is stored in the linked list.
 * It includes data of type "T".
 */

template <class T> class  RWTValDlink : public RWIsvDlink
{

public:

    RWTValDlink(const T& a) : info_(a) {
        ;
    }
    RWTValDlink<T>* next() const {
        return (RWTValDlink<T>*)next_;
    }
    RWTValDlink<T>* prev() const {
        return (RWTValDlink<T>*)prev_;
    }

    T     info_;

};

/****************************************************************
 *                              *
 *      Declarations for RWClassicTValDlist<T>          *
 *                              *
 ****************************************************************/

template <class T> class  RWClassicTValDlist
    : private RWTIsvDlist< RWTValDlink<T> >
{

public:

    RWClassicTValDlist() {
        ;
    }
    RWClassicTValDlist(const RWClassicTValDlist<T>&);
    ~RWClassicTValDlist() {
        clear();
    }

    // Operators:
    RWClassicTValDlist<T>&    operator=(const RWClassicTValDlist<T>&);

    T&        operator[](size_t i) {
        return RWTIsvDlist<RWTValDlink<T> >::at(i)->info_;
    }
    const T&  operator[](size_t i) const {
        return RWTIsvDlist<RWTValDlink<T> >::at(i)->info_;
    }

    // Member functions:
    void      append(const T& a);

    void      apply(void (*applyFun)(T&, void*), void*);

    T&        at(size_t i) {
        return RWTIsvDlist<RWTValDlink<T> >::at(i)->info_;
    }
    const T&  at(size_t i) const {
        return RWTIsvDlist<RWTValDlink<T> >::at(i)->info_;
    }

    void      clear() {
        RWTIsvDlist<RWTValDlink<T> >::clearAndDestroy();
    }

    bool  contains(const T& a) const;
    bool  contains(bool (*testFun)(const T&, void*), void*) const;

    size_t    entries() const {
        return RWTIsvDlist<RWTValDlink<T> >::entries();
    }

    bool  find(const T& p, T& ret) const; // Find first; return in "ret"
    bool  find(bool (*testFun)(const T&, void*), void* d, T& ret) const;

    T&            first() {
        return this->firstLink()->info_;
    }
    const T&  first() const {
        return this->firstLink()->info_;
    }

    T     get() {
        return peel(RWTIsvDlist<RWTValDlink<T> >::get());
    }

    size_t    index(const T& a) const;
    size_t    index(bool(*testFun)(const T&, void*), void*) const;

    void      insert(const T& a);
    void      insertAt(size_t i, const T& a);

    bool  isEmpty() const {
        return RWTIsvDlist<RWTValDlink<T> >::isEmpty();
    }

    T&            last() {
        return this->lastLink()->info_;
    }
    const T&  last() const {
        return this->lastLink()->info_;
    }

    size_t    occurrencesOf(const T& a) const;
    size_t    occurrencesOf(bool (*testFun)(const T&, void*), void*) const;

    void      prepend(const T& a);

    bool  remove(const T& a);
    bool  remove(bool (*testFun)(const T&, void*), void* d);

    size_t    removeAll(const T& a);
    size_t    removeAll(bool (*testFun)(const T&, void*), void*);

    T     removeAt(size_t i) {
        return peel(RWTIsvDlist<RWTValDlink<T> >::removeAt(i));
    }

    T     removeFirst() {
        return peel(RWTIsvDlist<RWTValDlink<T> >::removeFirst());
    }

    T     removeLast() {
        return peel(RWTIsvDlist<RWTValDlink<T> >::removeLast());
    }

protected:

    RWTValDlink<T>*   findVal(const T&) const;
    RWTValDlink<T>*   findFun(bool (*testFun)(const T&, void*), void*) const;

    static T      peel(RWTValDlink<T>* link);

private:

    friend class  RWClassicTValDlistIterator<T>;

};

#include <rw/edefs.h>
RW_DECLARE_PERSISTABLE_TEMPLATE(RWClassicTValDlist)

#ifdef RW_NO_TEMPLINST_ON_BASE
class RW_TOOLS_GLOBAL RWpistream;
class RW_TOOLS_GLOBAL RWpostream;
class RW_TOOLS_GLOBAL RWbistream;
class RW_TOOLS_GLOBAL RWbostream;
class RW_TOOLS_GLOBAL RWeistream;
class RW_TOOLS_GLOBAL RWeostream;
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTValDlist, RWpistream, RWpostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTValDlist, RWbistream, RWbostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTValDlist, RWeistream, RWeostream)
#endif



template <class T> class  RWClassicTValDlistIterator :
    private RWTIsvDlistIterator<RWTValDlink<T> >
{

public:

    RWClassicTValDlistIterator(RWClassicTValDlist<T>& s) :
        RWTIsvDlistIterator<RWTValDlink<T> > (s) {
        ;
    }

    // Operators:
    bool  operator++() {
        this->advance();
        return this->cursor() != container()->tailLink();
    }
    bool  operator--() {
        this->backup();
        return this->cursor() != container()->headLink();
    }
    bool  operator+=(size_t n) {
        return RWTIsvDlistIterator<RWTValDlink<T> >::operator+=(n) != rwnil;
    }
    bool  operator-=(size_t n) {
        return RWTIsvDlistIterator<RWTValDlink<T> >::operator-=(n) != rwnil;
    }
    bool  operator()() {
        this->advance();
        return this->cursor() != container()->tailLink();
    }

    // Methods:
    RWClassicTValDlist<T>*    container() const {
        return (RWClassicTValDlist<T>*)RWTIsvDlistIterator<RWTValDlink<T> >::container();
    }

    bool      findNext(const T& p);
    bool      findNext(bool (*testFun)(const T&, void*), void* d);

    void          insertAfterPoint(const T& a);

    T&            key() const {
        return this->cursor()->info_;
    }

    bool      remove();   // Remove item at cursor

    bool      removeNext(const T& p);
    bool      removeNext(bool (*testFun)(const T&, void*), void* d);

    void          reset() {
        RWTIsvDlistIterator<RWTValDlink<T> >::reset();
    }

    void          reset(RWClassicTValDlist<T>& s) {
        RWTIsvDlistIterator<RWTValDlink<T> >::reset(s);
    }

private:

    // Disallow postfix increment & decrement.  Unless we hide it, some
    // compilers will substitute the prefix operator in its place.
    bool      operator++(int) {
        return false;
    }
    bool      operator--(int) {
        return false;
    }

};


template <class T, class S>
void rwRestoreGuts(S& str, RWClassicTValDlist<T>& c);

#ifndef RW_NO_TEMPLINST_ON_BASE

template <class T>
void rwSaveGuts(RWvostream& str, const RWClassicTValDlist<T>& c);

template <class T>
void rwSaveGuts(RWFile& str, const RWClassicTValDlist<T>& c);

#else

template <class T, class S>
void rwSaveGuts(S& str, const RWClassicTValDlist<T>& c);

#endif

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xvdlist.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xvdlist.cc"
#endif


#endif  /* RW_TOOLS_XVDLIST_H */
