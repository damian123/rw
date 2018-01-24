#ifndef RW_TOOLS_XPSLIST_H
#define RW_TOOLS_XPSLIST_H


/**********************************************************************
 *
 * RWClassicTPtrSlist<TP>: A singly-linked list of pointers to objects of type TP.
 * Stores a *pointer* to the inserted item into the collection.
 * Assumes that TP has:
 *   - well-defined equality semantics (TP::operator==(const TP&)).
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xpslist.h#1 $
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

template <class TP> class  RWClassicTPtrSlistIterator;

/****************************************************************
 *                              *
 *      Declarations for RWTPtrSlink<TP>        *
 *                              *
 ****************************************************************/


/*
 * This is the actual link that is stored in the linked list.
 * It includes data of type "TP".
 */

template <class TP> class  RWTPtrSlink : public RWIsvSlink
{

public:

    RWTPtrSlink(TP* a) : info_(a) {
        ;
    }
    RWTPtrSlink<TP>*  next() const {
        return (RWTPtrSlink<TP>*)next_;
    }

    TP*       info_;

};

/****************************************************************
 *                              *
 *  Declarations for RWClassicTPtrSlist<TP>         *
 *                              *
 ****************************************************************/

template <class TP> class  RWClassicTPtrSlist
    : private RWTIsvSlist< RWTPtrSlink<TP> >
{
public:

    RWClassicTPtrSlist() {
        ;
    }
    RWClassicTPtrSlist(const RWClassicTPtrSlist<TP>&);
    ~RWClassicTPtrSlist() {
        clear();
    }

    // Operators:
    RWClassicTPtrSlist<TP>&   operator=(const RWClassicTPtrSlist<TP>&);

    TP*&      operator[](size_t i) {
        return RWTIsvSlist<RWTPtrSlink<TP> >::at(i)->info_;
    }
    TP* const& operator[](size_t i) const {
        return RWTIsvSlist<RWTPtrSlink<TP> >::at(i)->info_;
    }

    // Member functions:
    void      append(TP* a);

    void      apply(void (*applyFun)(TP*, void*), void*);

    TP*&      at(size_t i) {
        return RWTIsvSlist<RWTPtrSlink<TP> >::at(i)->info_;
    }
    TP* const& at(size_t i) const {
        return RWTIsvSlist<RWTPtrSlink<TP> >::at(i)->info_;
    }

    void      clear() {
        RWTIsvSlist<RWTPtrSlink<TP> >::clearAndDestroy();
    }

    void      clearAndDestroy();

    bool  contains(const TP* a) const;
    bool  contains(bool (*testFun)(TP*, void*), void*) const;

    size_t    entries() const {
        return RWTIsvSlist<RWTPtrSlink<TP> >::entries();
    }

    TP*       find(const TP* p) const;
    TP*       find(bool (*testFun)(TP*, void*), void* d) const;

    TP*&      first() {
        return this->firstLink()->info_;
    }
    TP* const& first() const {
        return this->firstLink()->info_;
    }

    TP*       get() {
        return peel(RWTIsvSlist<RWTPtrSlink<TP> >::get());
    }

    size_t    index(const TP* a) const;
    size_t    index(bool(*testFun)(TP*, void*), void*) const;

    void      insert(TP* a);

    void      insertAt(size_t i, TP* a);

    bool  isEmpty() const {
        return RWTIsvSlist<RWTPtrSlink<TP> >::isEmpty();
    }

    TP*&      last() {
        return this->lastLink()->info_;
    }
    TP* const& last() const {
        return this->lastLink()->info_;
    }

    size_t    occurrencesOf(const TP* a) const;
    size_t    occurrencesOf(bool (*testFun)(TP*, void*), void*) const;

    void      prepend(TP* a);

    TP*       remove(const TP* p);
    TP*       remove(bool (*testFun)(TP*, void*), void* d);

    TP*       removeAt(size_t i) {
        return peel(RWTIsvSlist<RWTPtrSlink<TP> >::removeAt(i));
    }

    TP*       removeFirst() {
        return peel(RWTIsvSlist<RWTPtrSlink<TP> >::removeFirst());
    }

    TP*       removeLast() {
        return peel(RWTIsvSlist<RWTPtrSlink<TP> >::removeLast());
    }

    size_t    removeAll(const TP* a);
    size_t    removeAll(bool (*testFun)(TP*, void*), void*);

protected:

    RWTPtrSlink<TP>*  findLeftVal(const TP*) const;
    RWTPtrSlink<TP>*  findLeftFun(bool (*testFun)(TP*, void*), void*) const;

    static TP*        peel(RWTPtrSlink<TP>* link);    // return "info_"; delete link

private:

    friend class  RWClassicTPtrSlistIterator<TP>;

};

#include <rw/edefs.h>
RW_DECLARE_PERSISTABLE_TEMPLATE(RWClassicTPtrSlist)

#ifdef RW_NO_TEMPLINST_ON_BASE
class RW_TOOLS_GLOBAL RWpistream;
class RW_TOOLS_GLOBAL RWpostream;
class RW_TOOLS_GLOBAL RWbistream;
class RW_TOOLS_GLOBAL RWbostream;
class RW_TOOLS_GLOBAL RWeistream;
class RW_TOOLS_GLOBAL RWeostream;
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTPtrSlist, RWpistream, RWpostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTPtrSlist, RWbistream, RWbostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTPtrSlist, RWeistream, RWeostream)
#endif


/****************************************************************
 *                              *
 *  Declarations for RWClassicTPtrSlistIterator<TP>     *
 *                              *
 ****************************************************************/

template <class TP> class  RWClassicTPtrSlistIterator :
    private RWTIsvSlistIterator<RWTPtrSlink<TP> >
{

public:

    RWClassicTPtrSlistIterator(RWClassicTPtrSlist<TP>& s) :
        RWTIsvSlistIterator<RWTPtrSlink<TP> >(s) {
        ;
    }

    RWClassicTPtrSlistIterator(const RWClassicTPtrSlistIterator<TP>&);
    // Copy ctor added for BUG#117

    // Operators:
    bool  operator++() {
        this->advance();
        return this->cursor() != container()->tailLink();
    }
    bool  operator+=(size_t n) {
        return RWTIsvSlistIterator<RWTPtrSlink<TP> >::operator+=(n) != rwnil;
    }
    TP*       operator()() {
        this->advance();
        return this->cursor() == container()->tailLink() ? rwnil : this->cursor()->info_;
    }

    // Methods
    RWClassicTPtrSlist<TP>*   container() const {
        return (RWClassicTPtrSlist<TP>*)RWTIsvSlistIterator<RWTPtrSlink<TP> >::container();
    }

    TP*       findNext(const TP* p);
    TP*       findNext(bool (*testFun)(TP*, void*), void* d);

    void      insertAfterPoint(TP* a);

    TP*       key() const {
        return this->isActive() ? this->cursor()->info_ : rwnil;
    }

    TP*       remove();

    TP*       removeNext(const TP*);
    TP*       removeNext(bool (*testFun)(TP*, void*), void*);

    void      reset() {
        RWTIsvSlistIterator<RWTPtrSlink<TP> >::reset();
    }
    void      reset(RWClassicTPtrSlist<TP>& s) {
        RWTIsvSlistIterator<RWTPtrSlink<TP> >::reset(s);
    }

private:

    // Disallow postfix increment.  Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool      operator++(int);

};

template <class TP, class S>
void rwSaveGuts(S& str, const RWClassicTPtrSlist<TP>& c);

template <class TP, class S>
void rwRestoreGuts(S& str, RWClassicTPtrSlist<TP>& c);


#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xpslist.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xpslist.cc"
#endif


#endif  /* RW_TOOLS_XPSLIST_H */
