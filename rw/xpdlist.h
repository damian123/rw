#ifndef RW_TOOLS_XPDLIST_H
#define RW_TOOLS_XPDLIST_H


/**********************************************************************
 *
 * RWClassicTPtrDlist<TP>: A doubly-linked list of pointers to objects of type TP.
 * Stores a *pointer* to the inserted item into the collection.
 * Assumes that TP has:
 *   - well-defined equality semantics (TP::operator==(const TP&)).
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xpdlist.h#1 $
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

template <class TP> class  RWClassicTPtrDlistIterator;

/****************************************************************
 *                                                              *
 *              Declarations for RWTPtrDlink<TP>                *
 *                                                              *
 ****************************************************************/


/*
 * This is the actual link that is stored in the linked list.
 * It includes data of type "TP".
 */

template <class TP> class  RWTPtrDlink : public RWIsvDlink
{

public:

    RWTPtrDlink(TP* a) : info_(a) {
        ;
    }
    RWTPtrDlink<TP>*      next() const {
        return (RWTPtrDlink<TP>*)RWIsvDlink::next();
    }
    RWTPtrDlink<TP>*      prev() const {
        return (RWTPtrDlink<TP>*)RWIsvDlink::prev();
    }

    TP*           info_;

};

/****************************************************************
 *                                                              *
 *      Declarations for RWClassicTPtrDlist<TP>                        *
 *                                                              *
 ****************************************************************/

template <class TP> class  RWClassicTPtrDlist
    : private RWTIsvDlist< RWTPtrDlink<TP> >
{

public:

    RWClassicTPtrDlist() {
        ;
    }
    RWClassicTPtrDlist(const RWClassicTPtrDlist<TP>&);
    ~RWClassicTPtrDlist() {
        clear();
    }

    // Operators:
    RWClassicTPtrDlist<TP>&      operator=(const RWClassicTPtrDlist<TP>&);

    TP*&          operator[](size_t i) {
        return RWTIsvDlist<RWTPtrDlink<TP> >::at(i)->info_;
    }
    TP* const&     operator[](size_t i) const {
        return RWTIsvDlist<RWTPtrDlink<TP> >::at(i)->info_;
    }

    // Members
    void          append(TP* a);

    void          apply(void (*applyFun)(TP*, void*), void*);

    TP*&          at(size_t i) {
        return RWTIsvDlist<RWTPtrDlink<TP> >::at(i)->info_;
    }
    TP* const&     at(size_t i) const {
        return RWTIsvDlist<RWTPtrDlink<TP> >::at(i)->info_;
    }

    void          clear() {
        RWTIsvDlist<RWTPtrDlink<TP> >::clearAndDestroy();
    }

    void          clearAndDestroy();

    bool     contains(const TP* a) const;
    bool     contains(bool (*testFun)(TP*, void*), void*) const;

    size_t        entries() const {
        return RWTIsvDlist<RWTPtrDlink<TP> >::entries();
    }

    TP*           find(const TP* p) const;
    TP*           find(bool (*testFun)(TP*, void*), void* d) const;

    TP*&          first() {
        return this->firstLink()->info_;
    }
    TP* const&     first() const {
        return this->firstLink()->info_;
    }

    TP*           get() {
        return peel(RWTIsvDlist<RWTPtrDlink<TP> >::get());
    }

    size_t        index(const TP* a) const;
    size_t        index(bool(*testFun)(TP*, void*), void*) const;

    void          insert(TP* a);

    void          insertAt(size_t i, TP* a);

    bool     isEmpty() const {
        return RWTIsvDlist<RWTPtrDlink<TP> >::isEmpty();
    }

    TP*&          last() {
        return this->lastLink()->info_;
    }
    TP* const&     last() const {
        return this->lastLink()->info_;
    }

    size_t        occurrencesOf(const TP* a) const;
    size_t        occurrencesOf(bool (*testFun)(TP*, void*), void*) const;

    void          prepend(TP* a);

    TP*           remove(const TP* p);
    TP*           remove(bool (*testFun)(TP*, void*), void* d);

    TP*           removeAt(size_t i) {
        return peel(RWTIsvDlist<RWTPtrDlink<TP> >::removeAt(i));
    }

    TP*           removeFirst() {
        return peel(RWTIsvDlist<RWTPtrDlink<TP> >::removeFirst());
    }

    TP*           removeLast() {
        return peel(RWTIsvDlist<RWTPtrDlink<TP> >::removeLast());
    }

    size_t        removeAll(const TP* a);
    size_t        removeAll(bool (*testFun)(TP*, void*), void*);

protected:

    RWTPtrDlink<TP>*      findVal(const TP*) const;
    RWTPtrDlink<TP>*      findFun(bool (*testFun)(TP*, void*), void*) const;
    static TP*            peel(RWTPtrDlink<TP>* link);

private:

    friend class  RWClassicTPtrDlistIterator<TP>;

};

#include <rw/edefs.h>
RW_DECLARE_PERSISTABLE_TEMPLATE(RWClassicTPtrDlist)

#ifdef RW_NO_TEMPLINST_ON_BASE
class RW_TOOLS_GLOBAL RWpistream;
class RW_TOOLS_GLOBAL RWpostream;
class RW_TOOLS_GLOBAL RWbistream;
class RW_TOOLS_GLOBAL RWbostream;
class RW_TOOLS_GLOBAL RWeistream;
class RW_TOOLS_GLOBAL RWeostream;
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTPtrDlist, RWpistream, RWpostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTPtrDlist, RWbistream, RWbostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWClassicTPtrDlist, RWeistream, RWeostream)
#endif

/****************************************************************
 *                                                              *
 *      Declarations for RWClassicTPtrDlistIterator<TP>                *
 *                                                              *
 ****************************************************************/

template <class TP> class  RWClassicTPtrDlistIterator :
    public RWTIsvDlistIterator<RWTPtrDlink<TP> >
{

public:

    RWClassicTPtrDlistIterator(RWClassicTPtrDlist<TP>& s) :
        RWTIsvDlistIterator<RWTPtrDlink<TP> >(s) { }

    // Operators:
    bool     operator++() {
        this->advance();
        return this->cursor() != container()->tailLink();
    }
    bool     operator--() {
        this->backup();
        return this->cursor() != container()->headLink();
    }
    bool     operator+=(size_t n) {
        return RWTIsvDlistIterator<RWTPtrDlink<TP> >::operator+=(n) != rwnil;
    }
    bool     operator-=(size_t n) {
        return RWTIsvDlistIterator<RWTPtrDlink<TP> >::operator-=(n) != rwnil;
    }
    TP*           operator()() {
        this->advance();
        return this->cursor() == container()->tailLink() ? rwnil : this->cursor()->info_;
    }

    // Methods:
    RWClassicTPtrDlist<TP>* container() const {
        return (RWClassicTPtrDlist<TP>*)RWTIsvDlistIterator<RWTPtrDlink<TP> >::container();
    }

    TP*           findNext(const TP* p);
    TP*           findNext(bool (*testFun)(TP*, void*), void* d);

    void          insertAfterPoint(TP* a);

    TP*           key() const {
        return this->isActive() ? this->cursor()->info_ : rwnil;
    }

    TP*           remove();

    TP*           removeNext(const TP* p);
    TP*           removeNext(bool (*testFun)(TP*, void*), void* d);

    void          reset() {
        RWTIsvDlistIterator<RWTPtrDlink<TP> >::reset();
    }
    void          reset(RWClassicTPtrDlist<TP>& s) {
        RWTIsvDlistIterator<RWTPtrDlink<TP> >::reset(s);
    }

private:

    // Disallow postfix operators.  Unless we hide it, some compilers will
    // substitute the prefix operator in its place.
    bool             operator++(int);
    bool             operator--(int);

};


template <class TP, class S>
void rwSaveGuts(S& str, const RWClassicTPtrDlist<TP>& c);

template <class TP, class S>
void rwRestoreGuts(S& str, RWClassicTPtrDlist<TP>& c);


#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xpdlist.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xpdlist.cc"
#endif


#endif  /* RW_TOOLS_XPDLIST_H */
