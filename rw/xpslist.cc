

/**********************************************************************
 *
 * Template definitions for RWClassicTPtrSlist<T> and RWClassicTPtrSlistIterator<T>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xpslist.cc#1 $
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


template <class TP> RWClassicTPtrSlist<TP>::RWClassicTPtrSlist(const RWClassicTPtrSlist<TP>& s) :
    RWTIsvSlist< RWTPtrSlink<TP> >()
{
    // Construct an iterator, casting away "constness"
    // (which we promise to honor anyway):
    RWClassicTPtrSlistIterator<TP> next((RWClassicTPtrSlist<TP>&)s);
    TP* p;

    while ((p = next()) != rwnil) {
        append(p);
    }

    RW_POSTCONDITION(s.entries() == entries());
}

template <class TP> RWClassicTPtrSlist<TP>&
RWClassicTPtrSlist<TP>::operator=(const RWClassicTPtrSlist<TP>& s)
{
    if (this != &s) {
        clear();
        // Construct an iterator, casting away "constness"
        // (which we promise to honor anyway):
        RWClassicTPtrSlistIterator<TP> next((RWClassicTPtrSlist<TP>&)s);
        TP* p;

        while ((p = next()) != rwnil) {
            append(p);
        }
    }

    RW_POSTCONDITION(s.entries() == entries());
    return *this;
}

template <class TP> void
RWClassicTPtrSlist<TP>::append(TP* p)
{
    RWTIsvSlist<RWTPtrSlink<TP> >::append(new RWTPtrSlink<TP>(p));
}

template <class TP> void
RWClassicTPtrSlist<TP>::apply(void (*applyFun)(TP*, void*), void* d)
{
    RWTPtrSlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        applyFun(link->info_, d);   // Apply the function
        link = link->next();    // Advance
    }
}

template <class TP> void
RWClassicTPtrSlist<TP>::clearAndDestroy()
{
    RWTPtrSlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        RWTPtrSlink<TP>* next = link->next();
        delete link->info_;
        delete link;
        link = next;
    }
    this->init();
}

template <class TP> bool
RWClassicTPtrSlist<TP>::contains(const TP* p) const
{
    return findLeftVal(p) != rwnil;
}

template <class TP> bool
RWClassicTPtrSlist<TP>::contains(bool (*testFun)(TP*, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    return findLeftFun(testFun, d) != rwnil;
}

template <class TP> TP*
RWClassicTPtrSlist<TP>::find(const TP* p) const
{
    RW_PRECONDITION(p != rwnil);

    RWTPtrSlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (*link->info_ == *p) {
            return link->info_;
        }
        link = link->next();    // Advance
    }
    return rwnil;
}

template <class TP> TP*
RWClassicTPtrSlist<TP>::find(bool (*testFun)(TP*, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    RWTPtrSlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_, d)) {
            return link->info_;
        }
        link = link->next();    // Advance
    }
    return rwnil;
}

template <class TP> size_t
RWClassicTPtrSlist<TP>::index(const TP* p) const
{
    RW_PRECONDITION(p != rwnil);

    size_t count = 0;
    RWTPtrSlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (*link->info_ == *p) {
            return count;
        }
        ++count;
        link = link->next();    // Advance
    }
    return RW_NPOS;
}

template <class TP> size_t
RWClassicTPtrSlist<TP>::index(bool (*testFun)(TP*, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    size_t count = 0;
    RWTPtrSlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_, d)) {
            return count;
        }
        ++count;
        link = link->next();    // Advance
    }
    return RW_NPOS;
}

template <class TP> void
RWClassicTPtrSlist<TP>::insert(TP* a)
{
    RWTIsvSlist<RWTPtrSlink<TP> >::insert(new RWTPtrSlink<TP>(a));
}

template <class TP> void
RWClassicTPtrSlist<TP>::insertAt(size_t i, TP* a)
{
    RWTIsvSlist<RWTPtrSlink<TP> >
    ::insertAt(i, new RWTPtrSlink<TP>(a));
}

template <class TP> size_t
RWClassicTPtrSlist<TP>::occurrencesOf(const TP* p) const
{
    RW_PRECONDITION(p != rwnil);

    size_t count = 0;
    RWTPtrSlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (*link->info_ == *p) {
            ++count;
        }
        link = link->next();    // Advance
    }
    return count;
}

template <class TP> size_t
RWClassicTPtrSlist<TP>::occurrencesOf(bool (*testFun)(TP*, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    size_t count = 0;
    RWTPtrSlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_, d)) {
            ++count;
        }
        link = link->next();    // Advance
    }
    return count;
}

template <class TP> void
RWClassicTPtrSlist<TP>::prepend(TP* a)
{
    RWTIsvSlist<RWTPtrSlink<TP> >
    ::prepend(new RWTPtrSlink<TP>(a));
}

template <class TP> TP*
RWClassicTPtrSlist<TP>::remove(const TP* p)
{
    RW_PRECONDITION(p != rwnil);
    RW_PRECONDITION(this->lastLink()->next() == this->tailLink());

    TP* ret = rwnil;

    // Set up a sentinel (casting away const: DO NOT MODIFY *newTail.info_):
    RWTPtrSlink<TP> newTail((TP*) p);
    this->lastLink()->next_ = &newTail;

    // Now search for it:
    RWTPtrSlink<TP>* link = this->headLink();
    while (!(*link->next()->info_ == *p)) {
        link = link->next();
    }

    // Reset the pointer in the last link:
    this->lastLink()->next_ = this->tailLink();

    if (link != this->lastLink()) {
        RWTPtrSlink<TP>* victim = RWTIsvSlist<RWTPtrSlink<TP> >::removeRight(link);
        ret = victim->info_;
        delete victim;
    }
    RW_POSTCONDITION(ret == rwnil || *ret == *p);
    return ret;
}

template <class TP> TP*
RWClassicTPtrSlist<TP>::remove(bool (*testFun)(TP*, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);
    TP* ret = rwnil;
    RWTPtrSlink<TP>* link = findLeftFun(testFun, d);
    if (link) {
        ret = peel(RWTIsvSlist< RWTPtrSlink<TP> >::removeRight(link));
    }

    RW_POSTCONDITION(ret == rwnil || testFun(ret, d));

    return ret;
}

template <class TP> size_t
RWClassicTPtrSlist<TP>::removeAll(const TP* p)
{
    RW_PRECONDITION(p != rwnil);

    size_t count = 0;
    RWTPtrSlink<TP>* prev = this->headLink();
    while (prev != this->lastLink()) {
        if (*prev->next()->info_ == *p) {
            // This will effectively advance the cursor--
            delete RWTIsvSlist<RWTPtrSlink<TP> >::removeRight(prev);
            count++;
        }
        else {
            prev = prev->next();
        }
    }
    return count;
}

template <class TP> size_t
RWClassicTPtrSlist<TP>::removeAll(bool (*testFun)(TP*, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    size_t count = 0;
    RWTPtrSlink<TP>* prev = this->headLink();
    while (prev != this->lastLink()) {
        if (testFun(prev->next()->info_, d)) {
            // This will effectively advance the cursor--
            delete RWTIsvSlist< RWTPtrSlink<TP> >::removeRight(prev);
            count++;
        }
        else {
            prev = prev->next();
        }
    }
    return count;
}

/***********************************************************
 *                             *
 *      RWClassicTPtrSlist<TP> protected functions         *
 *                             *
 ***********************************************************/

/*
 * Find the link *before* the link containing the indicated value.
 */
template <class TP> RWTPtrSlink<TP>*
RWClassicTPtrSlist<TP>::findLeftVal(const TP* p) const
{
    RW_PRECONDITION(p != rwnil);

    RWTPtrSlink<TP>* link = this->headLink();
    while (link != this->lastLink()) {
        if (*link->next()->info_ == *p) {
            return link;
        }
        link = link->next();
    }
    return rwnil;
}

/*
 * Find the link *before* the link that tests true.
 */
template <class TP> RWTPtrSlink<TP>*
RWClassicTPtrSlist<TP>::findLeftFun(bool (*testFun)(TP*, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    RWTPtrSlink<TP>* link = this->headLink();
    while (link != this->lastLink()) {
        if (testFun(link->next()->info_, d)) {
            return link;
        }
        link = link->next();
    }
    return rwnil;
}

/*
 * Extracts the value out of a link then throws the link away:
 */
template <class TP> TP*
RWClassicTPtrSlist<TP>::peel(RWTPtrSlink<TP>* link)
{
    if (!link) {
        return rwnil;
    }
    TP* ret = link->info_;
    delete link;
    return ret;
}

/****************************************************************
 *                              *
 *  Definitions for RWClassicTPtrSlistIterator<TP>          *
 *                              *
 ****************************************************************/

// Copy ctor for BUG#117
template <class TP>
RWClassicTPtrSlistIterator<TP>::RWClassicTPtrSlistIterator(const RWClassicTPtrSlistIterator<TP>& si)
    : RWTIsvSlistIterator<RWTPtrSlink<TP> >(si)
{
    ;
}


template <class TP> TP*
RWClassicTPtrSlistIterator<TP>::findNext(const TP* p)
{
    RW_PRECONDITION(p != rwnil);

    TP* a;
    while ((a = (*this)()) != rwnil) {
        if (*a == *p) {
            return a;
        }
    }
    return rwnil;
}

template <class TP> TP*
RWClassicTPtrSlistIterator<TP>::findNext(bool (*testFun)(TP*, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    TP* a;
    while ((a = (*this)()) != rwnil) {
        if (testFun(a, d)) {
            return a;
        }
    }
    return rwnil;
}

template <class TP> void
RWClassicTPtrSlistIterator<TP>::insertAfterPoint(TP* a)
{
    RWTIsvSlistIterator<RWTPtrSlink<TP> >
    ::insertAfterPoint(new RWTPtrSlink<TP>(a));
}

template <class TP> TP*
RWClassicTPtrSlistIterator<TP>::remove()
{
    return RWClassicTPtrSlist<TP>::peel(RWTIsvSlistIterator<RWTPtrSlink<TP> >::remove());
}

template <class TP> TP*
RWClassicTPtrSlistIterator<TP>::removeNext(const TP* p)
{
    RW_PRECONDITION(p != rwnil);

    while (this->cursor()->next() != container()->tailLink()) {
        if (*(this->cursor())->next()->info_ == *p) {
            return RWClassicTPtrSlist<TP>::peel(container()->removeRight(this->cursor()));
        }
        this->advance();
    }
    return rwnil;
}

template <class TP> TP*
RWClassicTPtrSlistIterator<TP>::removeNext(bool (*testFun)(TP*, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    while (this->cursor()->next() != container()->tailLink()) {
        if (testFun(this->cursor()->next()->info_, d)) {
            return RWClassicTPtrSlist<TP>::peel(container()->removeRight(this->cursor()));
        }
        this->advance();
    }
    return rwnil;
}

#include <rw/defs.h>
#define RW_USE_CLASSIC_TEMPLATES
#include <rw/epersist.h>

RW_DEFINE_PERSISTABLE_TEMPLATE(RWClassicTPtrSlist)

#ifdef RW_NO_TEMPLINST_ON_BASE
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTPtrSlist, RWpistream, RWpostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTPtrSlist, RWbistream, RWbostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTPtrSlist, RWeistream, RWeostream)
#endif


template <class TP, class S>
void rwSaveGuts(S& str, const RWClassicTPtrSlist<TP>& c)
{
    str.putSizeT(c.entries());
    RWClassicTPtrSlistIterator<TP> next((RWClassicTPtrSlist<TP>&)c);
    TP* p;
    while (rwnil != (p = next())) {
        str << *p;
    }
}

template <class TP, class S>
void rwRestoreGuts(S& str, RWClassicTPtrSlist<TP>& c)
{
    size_t count;
    str.getSizeT(count);
    c.clear();
    for (size_t i = 0; i < count; ++i) {
        TP* p;
        str >> p;
        c.insert(p);
    }
}
