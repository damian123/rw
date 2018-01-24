

/**********************************************************************
 *
 * Template definitions for RWClassicTValDlist<T> and RWClassicTValDlistIterator<T>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvdlist.cc#1 $
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


template <class T> RWClassicTValDlist<T>::RWClassicTValDlist(const RWClassicTValDlist<T>& s) :
    RWTIsvDlist< RWTValDlink<T> >()
{
    // Construct an iterator, casting away "constness"
    // (which we promise to honor anyway):
    RWClassicTValDlistIterator<T> next((RWClassicTValDlist<T>&)s);
    while (++next) {
        append(next.key());
    }
    RW_POSTCONDITION(s.entries() == entries());
}

template <class T> RWClassicTValDlist<T>&
RWClassicTValDlist<T>::operator=(const RWClassicTValDlist<T>& s)
{
    if (this != &s) {
        clear();
        // Construct an iterator, casting away "constness"
        // (which we promise to honor anyway):
        RWClassicTValDlistIterator<T> next((RWClassicTValDlist<T>&)s);
        while (++next) {
            append(next.key());
        }
    }
    RW_POSTCONDITION(s.entries() == entries());
    return *this;
}

template <class T> void
RWClassicTValDlist<T>::append(const T& a)
{
    RWTIsvDlist<RWTValDlink<T> >
    ::append(new RWTValDlink<T>(a));
}

template <class T> void
RWClassicTValDlist<T>::apply(void (*applyFun)(T&, void*), void* d)
{
    RWTValDlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        applyFun(link->info_, d);   // Apply the function
        link = link->next();    // Advance
    }
}

template <class T> bool
RWClassicTValDlist<T>::contains(const T& val) const
{
    return findVal(val) != rwnil;
}

template <class T> bool
RWClassicTValDlist<T>::contains(bool (*testFun)(const T&, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    return findFun(testFun, d) != rwnil;
}

template <class T> bool
RWClassicTValDlist<T>::find(const T& val, T& ret) const
{
    RWTValDlink<T>* link = findVal(val);
    return link ? (ret = link->info_, true) : false;
}

template <class T> bool
RWClassicTValDlist<T>::find(bool (*testFun)(const T&, void*), void* d, T& ret) const
{
    RW_PRECONDITION(testFun != rwnil);

    RWTValDlink<T>* link = findFun(testFun, d);

    return link ? (ret = link->info_, true) : false;
}

template <class T> size_t
RWClassicTValDlist<T>::index(const T& val) const
{
    size_t count = 0;
    RWTValDlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (link->info_ == val) {
            return count;
        }
        ++count;
        link = link->next();    // Advance
    }
    return RW_NPOS;
}

template <class T> size_t
RWClassicTValDlist<T>::index(bool (*testFun)(const T&, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    size_t count = 0;
    RWTValDlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_, d)) {
            return count;
        }
        ++count;
        link = link->next();    // Advance
    }
    return RW_NPOS;
}

template <class T> void
RWClassicTValDlist<T>::insert(const T& a)
{
    RWTIsvDlist<RWTValDlink<T> >::insert(new RWTValDlink<T>(a));
}

template <class T> void
RWClassicTValDlist<T>::insertAt(size_t i, const T& a)
{
    RWTIsvDlist<RWTValDlink<T> >
    ::insertAt(i, new RWTValDlink<T>(a));
}

template <class T> size_t
RWClassicTValDlist<T>::occurrencesOf(const T& val) const
{
    size_t count = 0;
    RWTValDlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (link->info_ == val) {
            ++count;
        }
        link = link->next();    // Advance
    }
    return count;
}

template <class T> size_t
RWClassicTValDlist<T>::occurrencesOf(bool (*testFun)(const T&, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    size_t count = 0;
    RWTValDlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_, d)) {
            ++count;
        }
        link = link->next();    // Advance
    }
    return count;
}

template <class T> void
RWClassicTValDlist<T>::prepend(const T& a)
{
    RWTIsvDlist<RWTValDlink<T> >
    ::prepend(new RWTValDlink<T>(a));
}

template <class T> bool
RWClassicTValDlist<T>::remove(const T& val)
{
    RW_PRECONDITION(this->lastLink()->next() == this->tailLink());

    // Set up a sentinel:
    RWTValDlink<T> newTail(val);
    this->lastLink()->next_ = &newTail;

    // Now search for it:
    RWTValDlink<T>* link = this->firstLink();
    while (!(link->info_ == val)) {
        link = link->next();
    }

    // Reset the pointer in the last link:
    this->lastLink()->next_ = this->tailLink();

    if (link != &newTail) {
        RW_POSTCONDITION(link->info_ == val);
        delete RWTIsvDlist< RWTValDlink<T> >::removeReference(link);
        return true;
    }
    return false;
}

template <class T> bool
RWClassicTValDlist<T>::remove(bool (*testFun)(const T&, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    RWTValDlink<T>* link = RWTIsvDlist< RWTValDlink<T> >::removeReference(findFun(testFun, d));
    if (link) {
        RW_POSTCONDITION(testFun(link->info_, d));
        delete link;
        return true;
    }
    return false;
}

template <class T> size_t
RWClassicTValDlist<T>::removeAll(const T& val)
{
    size_t count = 0;
    RWTValDlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (link->info_ == val) {
            RWTValDlink<T>* next = link->next();
            delete RWTIsvDlist< RWTValDlink<T> >::removeReference(link);
            link = next;
            count++;
        }
        else {
            link = link->next();
        }
    }
    return count;
}

template <class T> size_t
RWClassicTValDlist<T>::removeAll(bool (*testFun)(const T&, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    size_t count = 0;
    RWTValDlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_ , d)) {
            RWTValDlink<T>* next = link->next();
            delete RWTIsvDlist< RWTValDlink<T> >::removeReference(link);
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
 *                             *
 *      RWClassicTValDlist<T> protected functions          *
 *                             *
 ***********************************************************/

/*
 * Find the link containing the indicated value.
 */
template <class T> RWTValDlink<T>*
RWClassicTValDlist<T>::findVal(const T& val) const
{
    RWTValDlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (link->info_ == val) {
            return link;
        }
        link = link->next();
    }
    return rwnil;
}

/*
 * Find the link that contains info_ that tests true.
 */
template <class T> RWTValDlink<T>*
RWClassicTValDlist<T>::findFun(bool (*testFun)(const T&, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    RWTValDlink<T>* link = this->firstLink();
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
template <class T> T
RWClassicTValDlist<T>::peel(RWTValDlink<T>* link)
{
    RW_PRECONDITION2(link, "RWClassicTValDlist<T>::peel(RWTValDlink<T>*): nil link");

    T ret = link->info_;
    delete link;
    return ret;
}

/****************************************************************
 *                              *
 *  Definitions for RWClassicTValDlistIterator<T>           *
 *                              *
 ****************************************************************/

template <class T> bool
RWClassicTValDlistIterator<T>::findNext(const T& val)
{
    while (++(*this)) {
        if (key() == val) {
            return true;
        }
    }
    return false;
}

template <class T> bool
RWClassicTValDlistIterator<T>::findNext(bool (*testFun)(const T&, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    while (++(*this)) {
        if (testFun(key(), d)) {
            return true;
        }
    }
    return false;
}

template <class T> void
RWClassicTValDlistIterator<T>::insertAfterPoint(const T& a)
{
    RWTIsvDlistIterator<RWTValDlink<T> >
    ::insertAfterPoint(new RWTValDlink<T>(a));
}

template <class T> bool
RWClassicTValDlistIterator<T>::remove()
{
    RWTValDlink<T>* link = RWTIsvDlistIterator<RWTValDlink<T> >::remove();
    return link ? (delete link, true) : false;
}

template <class T> bool
RWClassicTValDlistIterator<T>::removeNext(const T& val)
{
    while (this->cursor()->next() != container()->tailLink()) {
        if (this->cursor()->next()->info_ == val) {
            delete container()->removeReference(this->cursor()->next());
            return true;
        }
        this->advance();
    }
    return false;
}

template <class T> bool
RWClassicTValDlistIterator<T>::removeNext(bool (*testFun)(const T&, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    while (this->cursor()->next() != container()->tailLink()) {
        if (testFun(this->cursor()->next()->info_ , d)) {
            delete container()->removeReference(this->cursor()->next());
            return true;
        }
        this->advance();
    }
    return false;
}

// Simple External Template Persistence.

#include <rw/defs.h>
#define RW_USE_CLASSIC_TEMPLATES
#include <rw/epersist.h>
// look for RW_DEFINE_PERSISTABLE_things and rwInsertInXxxx()

RW_DEFINE_PERSISTABLE_TEMPLATE(RWClassicTValDlist)

template <class T, class S>
void rwRestoreGuts(S& str, RWClassicTValDlist<T>& c)
{
    size_t count;
    str.getSizeT(count);
    c.clear();
    for (size_t i = 0; i < count; ++i) {
        T t;
        str >> t;
        c.insert(t);
    }
}

#ifndef RW_NO_TEMPLINST_ON_BASE
template <class T>
void rwSaveGuts(RWvostream& str, const RWClassicTValDlist<T>& c)
{
    str.putSizeT(c.entries());
    ((RWClassicTValDlist<T>&)c).apply(rwInsertInStream, &str);
}

template <class T>
void rwSaveGuts(RWFile& str, const RWClassicTValDlist<T>& c)
{
    str << c.entries();
    ((RWClassicTValDlist<T>&)c).apply(rwInsertInFile, &str);
}

#else

RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTValDlist, RWpistream, RWpostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTValDlist, RWbistream, RWbostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTValDlist, RWeistream, RWeostream)

template <class T, class S>
void rwSaveGuts(S& str, const RWClassicTValDlist<T>& c)
{
    str.putSizeT(c.entries());
    for (size_t i = 0; i < c.entries(); ++i) {
        str << c[i];
    }
}
#endif
