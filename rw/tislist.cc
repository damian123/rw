

/**********************************************************************
 *
 * Template definitions for RWTIsvSlist<TL> and RWTIsvSlistIterator<TL>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tislist.cc#1 $
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


template <class TL> void
RWTIsvSlist<TL>::apply(void (*applyFun)(TL*, void*), void* d)
{
    TL* link = firstLink();
    while (link != tailLink()) {
        applyFun(link, d);          // Apply the function
        link = (TL*)link->next();   // Advance
    }
}

template <class TL> void
RWTIsvSlist<TL>::clearAndDestroy()
{
    TL* n;
    TL* link = firstLink();
    while (link != tailLink()) {
        n = (TL*)link->next();
        delete link;
        link = n;
    }
    init();
}

template <class TL> TL*
RWTIsvSlist<TL>::find(bool (*testFun)(const TL*, void*), void* d) const
{
    TL* link = firstLink();
    while (link != tailLink()) {
        if (testFun(link, d)) {
            return link;
        }
        link = (TL*)link->next();
    }
    return rwnil;
}

template <class TL> size_t
RWTIsvSlist<TL>::index(bool (*testFun)(const TL*, void*), void* d) const
{
    size_t count = 0;
    const TL* link = firstLink();
    while (link != tailLink()) {
        if (testFun(link, d)) {
            return count;
        }
        link = (const TL*)link->next();
        ++count;
    }
    return RW_NPOS;
}

template <class TL> size_t
RWTIsvSlist<TL>::occurrencesOf(bool (*testFun)(const TL*, void*), void* d) const
{
    size_t count = 0;
    const TL* link = firstLink();
    while (link != tailLink()) {
        if (testFun(link, d)) {
            ++count;
        }
        link = (const TL*)link->next();
    }
    return count;
}

/*
 * Remove and return the first link for which the tester function
 * returns true.
 */
template <class TL> TL*
RWTIsvSlist<TL>::remove(bool (*testFun)(const TL*, void*), void* d)
{
    RW_PRECONDITION(testFun != rwnil);

    TL* link = headLink();
    while (link != lastLink()) {
        if (testFun((const TL*)link->next(), d)) {
            return removeRight(link);
        }
        link = (TL*)link->next();
    }
    return rwnil;
}


/*
 * Return first occurrence where the tester returns true.
 */
template <class TL> TL*
RWTIsvSlistIterator<TL>::findNext(bool (*testFun)(const TL*, void*), void* d)
{
    TL* p;
    while ((p = (*this)()) != 0) {
        if (testFun(p, d)) {
            return p;
        }
    }
    return rwnil;
}

/*
 * Remove first occurrence where the tester returns true.
 * To get any decent efficiency out of this function with
 * a singly-linked list, we must remember the previous link.
 */
template <class TL> TL*
RWTIsvSlistIterator<TL>::removeNext(bool (*testFun)(const TL*, void*), void* d)
{
    while (cursor()->next() != container()->tailLink()) {
        if (testFun((const TL*)cursor()->next(), d)) {
            return container()->removeRight(cursor());
        }
        advance();
    }
    return rwnil;
}

/****************************************************************
 *
 *    Definitions for RWTIsvSlistConstIterator
 *
 ****************************************************************/

// Return first occurrence where the tester returns true.
template <class TL> const TL*
RWTIsvSlistConstIterator<TL>::findNext(bool (*testFun)(const TL*, void*), void* d)
{
    const TL* p;
    while ((p = (*this)()) != 0) {
        if (testFun(p, d)) {
            return p;
        }
    }
    return rwnil;
}
