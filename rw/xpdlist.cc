

/**********************************************************************
 *
 * Template definitions for RWClassicTPtrDlist<T> and RWClassicTPtrDlistIterator<T>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xpdlist.cc#1 $
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


template <class TP>
RWClassicTPtrDlist<TP>::RWClassicTPtrDlist(const RWClassicTPtrDlist<TP>& s) :
    RWTIsvDlist< RWTPtrDlink<TP> > ()
{
    // Construct an iterator, casting away "constness"
    // (which we promise to honor anyway):
    RWClassicTPtrDlistIterator<TP> next((RWClassicTPtrDlist<TP>&)s);
    TP* p;

    while ((p = next()) != rwnil) {
        append(p);
    }

    RW_POSTCONDITION(s.entries() == entries());
}

template <class TP> RWClassicTPtrDlist<TP>&
RWClassicTPtrDlist<TP>::operator=(const RWClassicTPtrDlist<TP>& s)
{
    if (this != &s) {
        clear();
        // Construct an iterator, casting away "constness"
        // (which we promise to honor anyway):
        RWClassicTPtrDlistIterator<TP> next((RWClassicTPtrDlist<TP>&)s);
        TP* p;

        while ((p = next()) != rwnil) {
            append(p);
        }
    }

    RW_POSTCONDITION(s.entries() == entries());
    return *this;
}

template <class TP> void
RWClassicTPtrDlist<TP>::append(TP* a)
{
    RWTIsvDlist<RWTPtrDlink<TP> >
    ::append(new RWTPtrDlink<TP>(a));
}

template <class TP> void
RWClassicTPtrDlist<TP>::apply(void (*applyFun)(TP*, void*), void* d)
{
    RWTPtrDlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        applyFun(link->info_, d);   // Apply the function
        link = link->next();        // Advance
    }
}

template <class TP> void
RWClassicTPtrDlist<TP>::clearAndDestroy()
{
    RWTPtrDlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        RWTPtrDlink<TP>* next = link->next();
        delete link->info_;
        delete link;
        link = next;
    }
    this->init();
}

template <class TP> bool
RWClassicTPtrDlist<TP>::contains(const TP* p) const
{
    return findVal(p) != rwnil;
}

template <class TP> bool
RWClassicTPtrDlist<TP>::contains(bool (*testFun)(TP*, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    return findFun(testFun, d) != rwnil;
}

template <class TP> TP*
RWClassicTPtrDlist<TP>::find(const TP* p) const
{
    RW_PRECONDITION(p != rwnil);

    RWTPtrDlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (*link->info_ == *p) {
            return link->info_;
        }
        link = link->next();        // Advance
    }
    return rwnil;
}

template <class TP> TP*
RWClassicTPtrDlist<TP>::find(bool (*testFun)(TP*, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    RWTPtrDlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_, d)) {
            return link->info_;
        }
        link = link->next();        // Advance
    }
    return rwnil;
}

template <class TP> size_t
RWClassicTPtrDlist<TP>::index(const TP* p) const
{
    RW_PRECONDITION(p != rwnil);

    size_t count = 0;
    RWTPtrDlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (*link->info_ == *p) {
            return count;
        }
        ++count;
        link = link->next();        // Advance
    }
    return RW_NPOS;
}

template <class TP> size_t
RWClassicTPtrDlist<TP>::index(bool (*testFun)(TP*, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    size_t count = 0;
    RWTPtrDlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_, d)) {
            return count;
        }
        ++count;
        link = link->next();        // Advance
    }
    return RW_NPOS;
}


template <class TP> void
RWClassicTPtrDlist<TP>::insert(TP* a)
{
    RWTIsvDlist<RWTPtrDlink<TP> >
    ::insert(new RWTPtrDlink<TP>(a));
}

template <class TP> void
RWClassicTPtrDlist<TP>::insertAt(size_t i, TP* a)
{
    RWTIsvDlist<RWTPtrDlink<TP> >
    ::insertAt(i, new RWTPtrDlink<TP>(a));
}

template <class TP> size_t
RWClassicTPtrDlist<TP>::occurrencesOf(const TP* p) const
{
    RW_PRECONDITION(p != rwnil);

    size_t count = 0;
    RWTPtrDlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (*link->info_ == *p) {
            ++count;
        }
        link = link->next();        // Advance
    }
    return count;
}

template <class TP> size_t
RWClassicTPtrDlist<TP>::occurrencesOf(bool (*testFun)(TP*, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    size_t count = 0;
    RWTPtrDlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_, d)) {
            ++count;
        }
        link = link->next();        // Advance
    }
    return count;
}

template <class TP> void
RWClassicTPtrDlist<TP>::prepend(TP* a)
{
    RWTIsvDlist<RWTPtrDlink<TP> >
    ::prepend(new RWTPtrDlink<TP>(a));
}

template <class TP> TP*
RWClassicTPtrDlist<TP>::remove(const TP* p)
{
    RW_PRECONDITION(p != rwnil);
    RW_PRECONDITION(this->lastLink()->next() == this->tailLink());

    TP* ret = rwnil;

    // Set up a sentinel (casting away const: DO NOT MODIFY *newTail.info_):
    RWTPtrDlink<TP> newTail((TP*) p);
    this->lastLink()->next_ = &newTail;

    // Now search for it:
    RWTPtrDlink<TP>* link = this->firstLink();
    while (!(*link->info_ == *p)) {
        link = link->next();
    }

    // Reset the pointer in the last link:
    this->lastLink()->next_ = this->tailLink();

    if (link != &newTail) {
        RWTPtrDlink<TP>* victim = RWTIsvDlist< RWTPtrDlink<TP> >::removeReference(link);
        RW_ASSERT(victim == link);
        ret = victim->info_;
        delete victim;
    }
    RW_POSTCONDITION(ret == rwnil || *ret == *p);
    return ret;
}

template <class TP> TP*
RWClassicTPtrDlist<TP>::remove(bool (*testFun)(TP*, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    TP* ret = peel(RWTIsvDlist< RWTPtrDlink<TP> >::removeReference(findFun(testFun, d)));

    RW_POSTCONDITION(ret == rwnil || testFun(ret, d));

    return ret;
}

template <class TP> size_t
RWClassicTPtrDlist<TP>::removeAll(const TP* p)
{
    RW_PRECONDITION(p != rwnil);

    size_t count = 0;
    RWTPtrDlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (*link->info_ == *p) {
            RWTPtrDlink<TP>* next = link->next();
            delete RWTIsvDlist< RWTPtrDlink<TP> >::removeReference(link);
            link = next;
            count++;
        }
        else {
            link = link->next();
        }
    }
    return count;
}

template <class TP> size_t
RWClassicTPtrDlist<TP>::removeAll(bool (*testFun)(TP*, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    size_t count = 0;
    RWTPtrDlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_, d)) {
            RWTPtrDlink<TP>* next = link->next();
            delete RWTIsvDlist< RWTPtrDlink<TP> >::removeReference(link);
            link = next;
            count++;
        }
        else {
            link = link->next();
        }
    }
    return count;
}

/***********************************************************
 *                                                         *
 *          RWClassicTPtrDlist<TP> protected functions            *
 *                                                         *
 ***********************************************************/

/*
 * Find the link containing the indicated value.
 */
template <class TP> RWTPtrDlink<TP>*
RWClassicTPtrDlist<TP>::findVal(const TP* p) const
{
    RW_PRECONDITION(p != rwnil);

    RWTPtrDlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (*link->info_ == *p) {
            return link;
        }
        link = link->next();
    }
    return rwnil;
}

/*
 * Find the link that contains info_ that tests true.
 */
template <class TP> RWTPtrDlink<TP>*
RWClassicTPtrDlist<TP>::findFun(bool (*testFun)(TP*, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    RWTPtrDlink<TP>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_, d)) {
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
RWClassicTPtrDlist<TP>::peel(RWTPtrDlink<TP>* link)
{
    if (!link) {
        return rwnil;
    }

    TP* ret = link->info_;
    delete link;
    return ret;
}

/****************************************************************
 *                                                              *
 *      Definitions for RWClassicTPtrDlistIterator<TP>                 *
 *                                                              *
 ****************************************************************/

template <class TP> TP*
RWClassicTPtrDlistIterator<TP>::findNext(const TP* p)
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
RWClassicTPtrDlistIterator<TP>::findNext(bool (*testFun)(TP*, void*), void* d)
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
RWClassicTPtrDlistIterator<TP>::insertAfterPoint(TP* a)
{
    RWTIsvDlistIterator<RWTPtrDlink<TP> >
    ::insertAfterPoint(new RWTPtrDlink<TP>(a));
}

template <class TP> TP*
RWClassicTPtrDlistIterator<TP>::remove()
{
    return RWClassicTPtrDlist<TP>::peel(RWTIsvDlistIterator<RWTPtrDlink<TP> >::remove());
}

template <class TP> TP*
RWClassicTPtrDlistIterator<TP>::removeNext(const TP* p)
{
    RW_PRECONDITION(p != rwnil);

    while (this->cursor()->next() != container()->tailLink()) {
        if (*(this->cursor())->next()->info_ == *p) {
            return RWClassicTPtrDlist<TP>::peel(container()->removeReference(this->cursor()->next()));
        }
        this->advance();
    }
    return rwnil;
}

template <class TP> TP*
RWClassicTPtrDlistIterator<TP>::removeNext(bool (*testFun)(TP*, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    while (this->cursor()->next() != container()->tailLink()) {
        if (testFun(this->cursor()->next()->info_, d)) {
            return RWClassicTPtrDlist<TP>::peel(container()->removeReference(this->cursor()->next()));
        }
        this->advance();
    }
    return rwnil;
}

// Template Persistence

#include <rw/defs.h>
#define RW_USE_CLASSIC_TEMPLATES
#include <rw/epersist.h>


template <class TP, class S>
void rwSaveGuts(S& str, const RWClassicTPtrDlist<TP>& c)
{
    str.putSizeT(c.entries());
    RWClassicTPtrDlistIterator<TP> next((RWClassicTPtrDlist<TP>&)c);
    TP* p;
    while (rwnil != (p = next())) {
        str << *p;
    }
}

template <class TP, class S>
void rwRestoreGuts(S& str, RWClassicTPtrDlist<TP>& c)
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


RW_DEFINE_PERSISTABLE_TEMPLATE(RWClassicTPtrDlist)

#ifdef RW_NO_TEMPLINST_ON_BASE
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTPtrDlist, RWpistream, RWpostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTPtrDlist, RWbistream, RWbostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTPtrDlist, RWeistream, RWeostream)
#endif
