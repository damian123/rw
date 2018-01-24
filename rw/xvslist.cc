

/**********************************************************************
 *
 * Template definitions for RWClassicTValSlist<T> and RWClassicTValSlistIterator<T>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvslist.cc#1 $
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



// Copy constructor:
template <class T> RWClassicTValSlist<T>::RWClassicTValSlist(const RWClassicTValSlist<T>& s) :
    RWTIsvSlist< RWTValSlink<T> >()
{
    // Construct an iterator, casting away "constness"
    // (which we promise to honor anyway):
    RWClassicTValSlistIterator<T> next((RWClassicTValSlist<T>&)s);

    while (++next) {
        append(next.key());
    }

    RW_POSTCONDITION(s.entries() == entries());
}

template <class T> RWClassicTValSlist<T>&
RWClassicTValSlist<T>::operator=(const RWClassicTValSlist<T>& s)
{
    if (this != &s) {
        clear();
        // Construct an iterator, casting away "constness"
        // (which we promise to honor anyway):
        RWClassicTValSlistIterator<T> next((RWClassicTValSlist<T>&)s);

        while (++next) {
            append(next.key());
        }

    }
    RW_POSTCONDITION(s.entries() == entries());
    return *this;
}

template <class T> void
RWClassicTValSlist<T>::append(const T& a)
{
    RWTIsvSlist<RWTValSlink<T> >
    ::append(new RWTValSlink<T>(a));
}

template <class T> void
RWClassicTValSlist<T>::apply(void (*applyFun)(T&, void*), void* d)
{
    RWTValSlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        applyFun(link->info_, d);   // Apply the function
        link = link->next();    // Advance
    }
}

template <class T> bool
RWClassicTValSlist<T>::contains(const T& val) const
{
    return findLeftVal(val) != rwnil;
}

template <class T> bool
RWClassicTValSlist<T>::contains(bool (*testFun)(const T&, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    return findLeftFun(testFun, d) != rwnil;
}

template <class T> bool
RWClassicTValSlist<T>::find(const T& val, T& k) const
{
    RWTValSlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (link->info_ == val) {
            k = link->info_;
            return true;
        }
        link = link->next();    // Advance
    }
    return false;
}

template <class T> bool
RWClassicTValSlist<T>::find(bool (*testFun)(const T&, void*), void* d, T& k) const
{
    RW_PRECONDITION(testFun != rwnil);
    RWTValSlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_, d)) {
            k = link->info_;
            return true;
        }
        link = link->next();    // Advance
    }
    return false;
}

template <class T> size_t
RWClassicTValSlist<T>::index(const T& val) const
{
    size_t count = 0;
    RWTValSlink<T>* link = this->firstLink();
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
RWClassicTValSlist<T>::index(bool (*testFun)(const T&, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    size_t count = 0;
    RWTValSlink<T>* link = this->firstLink();
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
RWClassicTValSlist<T>::insert(const T& a)
{
    RWTIsvSlist<RWTValSlink<T> >
    ::insert(new RWTValSlink<T>(a));
}

template <class T> void
RWClassicTValSlist<T>::insertAt(size_t i, const T& a)
{

    RWTIsvSlist<RWTValSlink<T> >
    ::insertAt(i, new RWTValSlink<T>(a));
}

template <class T> size_t
RWClassicTValSlist<T>::occurrencesOf(const T& val) const
{
    size_t count = 0;
    RWTValSlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (link->info_ == val) {
            ++count;
        }
        link = link->next();    // Advance
    }
    return count;
}

template <class T> size_t
RWClassicTValSlist<T>::occurrencesOf(bool (*testFun)(const T&, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    size_t count = 0;
    RWTValSlink<T>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (testFun(link->info_, d)) {
            ++count;
        }
        link = link->next();    // Advance
    }
    return count;
}

template <class T> void
RWClassicTValSlist<T>::prepend(const T& a)
{
    RWTIsvSlist<RWTValSlink<T> >
    ::prepend(new RWTValSlink<T>(a));
}


// Non-const function allows us to use a sentinel:
template <class T> bool
RWClassicTValSlist<T>::remove(const T& val)
{
    RW_PRECONDITION(this->lastLink()->next() == this->tailLink());

    // Set up a sentinel:
    RWTValSlink<T> newTail(val);
    this->lastLink()->next_ = &newTail;

    // Now search for it:
    RWTValSlink<T>* link = this->headLink();
    while (!(link->next()->info_ == val)) {
        link = link->next();
    }

    // Reset the pointer in the last link:
    this->lastLink()->next_ = this->tailLink();

    if (link != this->lastLink()) {
        RW_POSTCONDITION(link->next()->info_ == val);
        delete  RWTIsvSlist< RWTValSlink<T> >::removeRight(link);
        return true;
    }
    return false;
}

template <class T> bool
RWClassicTValSlist<T>::remove(bool (*testFun)(const T&, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    RWTValSlink<T>* link = findLeftFun(testFun, d);
    if (link) {
        link = RWTIsvSlist< RWTValSlink<T> >::removeRight(link);
    }
    if (link) {
        RW_POSTCONDITION(testFun(link->info_, d));
        delete link;
        return true;
    }
    return false;
}

template <class T> size_t
RWClassicTValSlist<T>::removeAll(const T& val)
{
    size_t count = 0;
    RWTValSlink<T>* prev = this->headLink();
    while (prev != this->lastLink()) {
        if (prev->next()->info_ == val) {
            // This will effectively advance the cursor--
            delete RWTIsvSlist< RWTValSlink<T> >::removeRight(prev);
            count++;
        }
        else {
            prev = prev->next();
        }
    }
    return count;
}

template <class T> size_t
RWClassicTValSlist<T>::removeAll(bool (*testFun)(const T&, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    size_t count = 0;
    RWTValSlink<T>* prev = this->headLink();
    while (prev != this->lastLink()) {
        if (testFun(prev->next()->info_, d)) {
            // This will effectively advance the cursor--
            delete RWTIsvSlist< RWTValSlink<T> >::removeRight(prev);
            count++;
        }
        else {
            prev = prev->next();
        }
    }
    return count;
}

/****************************************************************
 *                              *
 *      RWClassicTValSlist<T> protected functions       *
 *                              *
 ****************************************************************/


/*
 * Find the link *before* the link containing the indicated value.
 */
template <class T> RWTValSlink<T>*
RWClassicTValSlist<T>::findLeftVal(const T& val) const
{
    RWTValSlink<T>* link = this->headLink();
    while (link != this->lastLink()) {
        if (link->next()->info_ == val) {
            return link;
        }
        link = link->next();
    }
    return rwnil;
}

/*
 * Find the link *before* the link that tests true.
 */
template <class T> RWTValSlink<T>*
RWClassicTValSlist<T>::findLeftFun(bool (*testFun)(const T&, void*), void* d) const
{
    RW_PRECONDITION(testFun != rwnil);

    RWTValSlink<T>* link = this->headLink();
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
template <class T> T
RWClassicTValSlist<T>::peel(RWTValSlink<T>* link)
{
    RW_PRECONDITION2(link, "RWClassicTValSlist<T>::peel(RWTValSlink<T>*): nil link");

    T ret = link->info_;
    delete link;
    return ret;
}


/****************************************************************
 *                              *
 *  Definitions for RWClassicTValSlistIterator<T>           *
 *                              *
 ****************************************************************/

template <class T> bool
RWClassicTValSlistIterator<T>::findNext(const T& val)
{
    while (++(*this)) {
        if (key() == val) {
            return true;
        }
    }
    return false;
}

template <class T> bool
RWClassicTValSlistIterator<T>::findNext(bool (*testFun)(const T&, void*), void* d)
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
RWClassicTValSlistIterator<T>::insertAfterPoint(const T& a)
{
    RWTIsvSlistIterator<RWTValSlink<T> >
    ::insertAfterPoint(new RWTValSlink<T>(a));
}

template <class T> bool
RWClassicTValSlistIterator<T>::remove()
{
    RWTValSlink<T>* link = RWTIsvSlistIterator<RWTValSlink<T> >::remove();
    return link ? (delete link, true) : false;
}

template <class T> bool
RWClassicTValSlistIterator<T>::removeNext(const T& val)
{
    while (this->cursor()->next() != container()->tailLink()) {
        if (this->cursor()->next()->info_ == val) {
            delete container()->removeRight(this->cursor());
            return true;
        }
        this->advance();
    }
    return false;
}

template <class T> bool
RWClassicTValSlistIterator<T>::removeNext(bool (*testFun)(const T&, void*), void* d)
{
    while (this->cursor()->next() != container()->tailLink()) {
        if (testFun(this->cursor()->next()->info_, d)) {
            delete container()->removeRight(this->cursor());
            return true;
        }
        this->advance();
    }
    return false;
}

template <class T>
RWClassicTValSlistIterator<T>::RWClassicTValSlistIterator(const RWClassicTValSlistIterator<T>& si)
    : RWTIsvSlistIterator<RWTValSlink<T> >(si)
{
    ;
}


#include <rw/defs.h>
#define RW_USE_CLASSIC_TEMPLATES
#include <rw/epersist.h>
// look for RW_DEFINE_PERSISTABLE_things and rwInsertInXxxx()

RW_DEFINE_PERSISTABLE_TEMPLATE(RWClassicTValSlist)


template <class T, class S>
void rwRestoreGuts(S& str, RWClassicTValSlist<T>& c)
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
void rwSaveGuts(RWvostream& str, const RWClassicTValSlist<T>& c)
{
    str.putSizeT(c.entries());
    ((RWClassicTValSlist<T>&)c).apply(rwInsertInStream, &str);
}
template <class T>
void rwSaveGuts(RWFile& str, const RWClassicTValSlist<T>& c)
{
    str << c.entries();
    ((RWClassicTValSlist<T>&)c).apply(rwInsertInFile, &str);
}

#else

RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTValSlist, RWpistream, RWpostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTValSlist, RWbistream, RWbostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTValSlist, RWeistream, RWeostream)

template <class T, class S>
void rwSaveGuts(S& str, const RWClassicTValSlist<T>& c)
{
    str.putSizeT(c.entries());
    for (size_t i = 0; i < c.entries(); ++i) {
        str << c[i];
    }
}

#endif
