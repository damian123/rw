#ifndef RW_TOOLS_XVSLIST_H
#define RW_TOOLS_XVSLIST_H


/**********************************************************************
 *
 * RWClassicTValSlist<T>: Singly-linked list of values of type T.
 * Stores a *copy* of the inserted item into the collection.
 * Assumes that T has:
 *   - well-defined copy constructor (T::T(const T&) or equiv.);
 *   - well-defined assignment operator (T::operator=(const T&) or equiv.);
 *   - well-defined equality operator (T::operator==(const T&) or equiv.)
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvslist.h#1 $
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
#include <rw/tislist.h>

template <class T> class  RWClassicTValSlistIterator;

/****************************************************************
 *                              *
 *      Declarations for RWTValSlink<T>         *
 *                              *
 ****************************************************************/

/*
 * This is the actual link that is stored in the linked list.
 * It includes data of type "T".
 */


template <class T> class  RWTValSlink : public RWIsvSlink
{

public:

    RWTValSlink(const T& a) : info_(a) {
        ;
    }
    RWTValSlink<T>* next() const {
        return (RWTValSlink<T>*)next_;
    }

    T     info_;

};

/****************************************************************
 *                              *
 *      Declarations for RWClassicTValSlist<T>          *
 *                              *
 ****************************************************************/

template <class T> class  RWClassicTValSlist
    : private RWTIsvSlist< RWTValSlink<T> >
{

public:

    RWClassicTValSlist() {
        ;
    }
    RWClassicTValSlist(const RWClassicTValSlist<T>&);
    ~RWClassicTValSlist() {
        clear();
    }

    // Operators:
    RWClassicTValSlist<T>&    operator=(const RWClassicTValSlist<T>&);

    T&        operator[](size_t i) {
        return RWTIsvSlist<RWTValSlink<T> >::at(i)->info_;
    }
    const T&  operator[](size_t i) const {
        return RWTIsvSlist<RWTValSlink<T> >::at(i)->info_;
    }

    // Member functions:
    void      append(const T& a);

    void      apply(void (*applyFun)(T&, void*), void*);

    T&        at(size_t i) {
        return RWTIsvSlist<RWTValSlink<T> >::at(i)->info_;
    }
    const T&  at(size_t i) const {
        return RWTIsvSlist<RWTValSlink<T> >::at(i)->info_;
    }

    void      clear() {
        RWTIsvSlist<RWTValSlink<T> >::clearAndDestroy();
    }

    bool  contains(const T& a) const;
    bool  contains(bool (*testFun)(const T&, void*), void*) const;

    size_t    entries() const {
        return RWTIsvSlist<RWTValSlink<T> >::entries();
    }

    bool  find(const T& a, T& ret) const; // Find first; return in "ret"
    bool  find(bool (*testFun)(const T&, void*), void*, T& ret) const;

    T&            first() {
        return this->firstLink()->info_;
    }
    const T&  first() const {
        return this->firstLink()->info_;
    }


    T     get() {
        return peel(RWTIsvSlist<RWTValSlink<T> >::get());
    }

    size_t    index(const T& a) const;
    size_t    index(bool (*testFun)(const T&, void*), void*) const;

    void      insert(const T& a);

    void      insertAt(size_t i, const T& a);

    bool  isEmpty() const {
        return RWTIsvSlist<RWTValSlink<T> >::isEmpty();
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

    bool  remove(const T& val);
    bool  remove(bool (*testFun)(const T&, void*), void*);

    size_t    removeAll(const T& a);
    size_t    removeAll(bool (*testFun)(const T&, void*), void*);

    T     removeAt(size_t i) {
        return peel(RWTIsvSlist<RWTValSlink<T> >::removeAt(i));
    }

    T     removeFirst() {
        return peel(RWTIsvSlist<RWTValSlink<T> >::removeFirst());
    }

    T     removeLast() {
        return peel(RWTIsvSlist<RWTValSlink<T> >::removeLast());
    }

protected:

    RWTValSlink<T>*   findLeftVal(const T&) const;
    RWTValSlink<T>*   findLeftFun(bool (*testFun)(const T&, void*), void*) const;

    static T      peel(RWTValSlink<T>* link);

private:

    friend class  RWClassicTValSlistIterator<T> ;

};

#include <rw/edefs.h>
RW_DECLARE_PERSISTABLE_TEMPLATE(RWClassicTValSlist)

#ifdef RW_NO_TEMPLINST_ON_BASE
class RW_TOOLS_GLOBAL RWpistream;
class RW_TOOLS_GLOBAL RWpostream;
class RW_TOOLS_GLOBAL RWbistream;
class RW_TOOLS_GLOBAL RWbostream;
class RW_TOOLS_GLOBAL RWeistream;
class RW_TOOLS_GLOBAL RWeostream;
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTValSlist, RWpistream, RWpostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTValSlist, RWbistream, RWbostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTValSlist, RWeistream, RWeostream)
#endif



/****************************************************************
 *                              *
 *      Declarations for RWClassicTValSlistIterator<T>      *
 *                              *
 ****************************************************************/

template <class T> class  RWClassicTValSlistIterator :
    private RWTIsvSlistIterator< RWTValSlink<T> >
{

public:

    RWClassicTValSlistIterator(RWClassicTValSlist<T>& s) :
        RWTIsvSlistIterator<RWTValSlink<T> >(s) {
        ;
    }

    RWClassicTValSlistIterator(const RWClassicTValSlistIterator<T>&);
    // Copy ctor added to address bug #117

    // Operators:
    bool  operator++() {
        this->advance();
        return this->cursor() != container()->tailLink();
    }
    bool  operator+=(size_t n) {
        return RWTIsvSlistIterator<RWTValSlink<T> >::operator+=(n) != rwnil;
    }
    bool  operator()() {
        this->advance();
        return this->cursor() != container()->tailLink();
    }

    // Methods
    RWClassicTValSlist<T>*    container() const {
        return (RWClassicTValSlist<T>*)RWTIsvSlistIterator<RWTValSlink<T> >::container();
    }

    bool      findNext(const T& a);
    bool      findNext(bool (*testFun)(const T&, void*), void*);

    void          insertAfterPoint(const T& a);

    T&            key() const {
        return this->cursor()->info_;
    }

    bool      remove();   // Remove item at cursor

    bool      removeNext(const T&);
    bool      removeNext(bool (*testFun)(const T&, void*), void*);

    void          reset() {
        RWTIsvSlistIterator<RWTValSlink<T> >::reset();
    }
    void          reset(RWClassicTValSlist<T>& s) {
        RWTIsvSlistIterator<RWTValSlink<T> >::reset(s);
    }

private:

    // Disallow postfix increment.  Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool      operator++(int) {
        return false;
    }

};


template <class T, class S>
void rwRestoreGuts(S& str, RWClassicTValSlist<T>& c);

#ifndef RW_NO_TEMPLINST_ON_BASE

template <class T>
void rwSaveGuts(RWvostream& str, const RWClassicTValSlist<T>& c);

template <class T>
void rwSaveGuts(RWFile& str, const RWClassicTValSlist<T>& c);

#else

template <class T, class S>
void rwSaveGuts(S& str, const RWClassicTValSlist<T>& c);

#endif


#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xvslist.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xvslist.cc"
#endif


#endif  /* RW_TOOLS_XVSLIST_H */
