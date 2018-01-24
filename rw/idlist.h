#ifndef RW_TOOLS_IDLIST_H
#define RW_TOOLS_IDLIST_H



/**********************************************************************
 *
 * RWIsvDlist: An intrusive doubly-linked list.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/idlist.h#1 $
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

#ifndef RW_TOOLS_MEMPOOL_H
#  include "rw/mempool.h"
#endif

/****************************************************************
 *
 *   Declaration for RWIsvDlink
 *
 ****************************************************************/


class RW_TOOLS_SYMBOLIC RWIsvDlink RWMemoryPool_OPTION
{

public:

#ifdef RW_DEBUG
    RWIsvDlink() : next_(rwnil) {
        ;
    }
#else
    RWIsvDlink() {
        ;
    }
#endif

    RWIsvDlink*           next() const {
        return next_;
    }
    RWIsvDlink*           prev() const {
        return prev_;
    }
    RWIsvDlink*           clear() {
        RWIsvDlink* ret = next_;
        next_ = rwnil;
        return ret;
    }

    RWIsvDlink*           next_;          // Pointer to next link
    RWIsvDlink*           prev_;          // Pointer to previous link
};

/****************************************************************
 *                                                              *
 *              Declarations for RWIsvDlist                     *
 *                                                              *
 ****************************************************************/

class RW_TOOLS_SYMBOLIC RWIsvDlist
{

public:

    RWIsvDlist() {
        init();
    }
    RWIsvDlist(RWIsvDlink* a);
    ~RWIsvDlist() {
        clear();
    }

    /********************* Member functions **************************/
    void                  append(RWIsvDlink* a) {
        insertAfterLink(tail_.prev_, a);
    }
    RWIsvDlink*           at(size_t i) const;     // Index must be in range
    void                  clear();
    bool             containsReference(const RWIsvDlink*) const;
    size_t                entries() const {
        return nitems_;
    }
    RWIsvDlink*           first() const {
        return isEmpty() ? rwnil : head_.next_;
    }
    RWIsvDlink*           get() {
        return removeFirst();
    }
    void                  insert(RWIsvDlink* a) {
        append(a);
    }
    void                  insertAt(size_t, RWIsvDlink*);
    bool             isEmpty() const {
        return nitems_ == 0;
    }
    RWIsvDlink*           last() const {
        return isEmpty() ? rwnil : tail_.prev_;
    }
    size_t                occurrencesOfReference(const RWIsvDlink* a) const {
        return containsReference(a) ? 1u : 0;
    }
    void                  prepend(RWIsvDlink* a) {
        insertAfterLink(&head_, a);
    }
    RWIsvDlink*           removeAt(size_t);               // Relatively slow
    RWIsvDlink*           removeFirst() {
        return isEmpty() ? rwnil : removeReference(head_.next_);
    }
    RWIsvDlink*           removeLast() {
        return isEmpty() ? rwnil : removeReference(tail_.prev_);
    }
    RWIsvDlink*           removeReference(RWIsvDlink* a);//rem,ret link at addr a
    void                  swap(RWIsvDlist& lst);

protected:

    void                  init();
    void                  insertAfterLink(RWIsvDlink*, RWIsvDlink*);

    RWIsvDlink            head_;          // Sentinel
    RWIsvDlink            tail_;          // Sentinel
    size_t                nitems_;        // Number of entries in the list

private:

    // This class is not copyable or assignable.
    RWIsvDlist(const RWIsvDlist&);
    RWIsvDlist& operator=(const RWIsvDlist&);

    friend class RW_TOOLS_SYMBOLIC RWIsvDlistIterator;
    friend class RW_TOOLS_SYMBOLIC RWIsvDlistConstIterator;
};


/****************************************************************
 *                                                              *
 *              Declarations for RWIsvDlistIterator             *
 *                                                              *
 ****************************************************************/

// Iterator for doubly linked list:

class RW_TOOLS_SYMBOLIC RWIsvDlistIterator
{
    friend class RWIsvDlistConstIterator;
public:

    RWIsvDlistIterator(RWIsvDlist& s) : dlist_(&s), dhere_(&s.head_) {
        ;
    }

    // Operators:
    RWIsvDlink*           operator++();           // Advance and return.
    RWIsvDlink*           operator--();           // Backup and return.
    RWIsvDlink*           operator+=(size_t n);   // Advance n links and return.
    RWIsvDlink*           operator-=(size_t n);   // Backup n links and return.
    RWIsvDlink*           operator()();           // Advance and return.

    bool             atFirst() const;
    bool             atLast() const;
    void                  toFirst();
    void                  toLast();

    RWIsvDlist*           container() const {
        return dlist_;
    }
    void                  insertAfterPoint(RWIsvDlink*);// Insert after position.
    RWIsvDlink*           key() const;            // Return current item
    RWIsvDlink*           remove();               // Remove current item
    void                  reset();
    void                  reset(RWIsvDlist&);     //set container to be iterated

protected:

    void                  advance() {
        dhere_ = dhere_->next_;
    }
    void                  backup() {
        dhere_ = dhere_->prev_;
    }
    bool             isActive() const;

    RWIsvDlist*           dlist_;                 // The list we are iterating
    RWIsvDlink*           dhere_;                 // Iterator position

};


/****************************************************************
 *                                                              *
 *              Declarations for RWIsvDlistConstIterator        *
 *                                                              *
 ****************************************************************/

// const Iterator for doubly linked list:

class RW_TOOLS_SYMBOLIC RWIsvDlistConstIterator
{

public:

    RWIsvDlistConstIterator(RWIsvDlist& s) : dlist_(&s), dhere_(&s.head_) {
        ;
    }
    RWIsvDlistConstIterator(const RWIsvDlistIterator& it)
        : dlist_(it.dlist_), dhere_(it.dhere_) { }

    // Operators:
    const RWIsvDlink*     operator++();           // Advance and return.
    const RWIsvDlink*     operator--();           // Backup and return.
    const RWIsvDlink*     operator+=(size_t n);   // Advance n links and return.
    const RWIsvDlink*     operator-=(size_t n);   // Backup n links and return.
    const RWIsvDlink*     operator()();           // Advance and return.

    bool          atFirst() const;
    bool          atLast() const;
    void                  toFirst();
    void                  toLast();

    const RWIsvDlist*     container() const {
        return dlist_;
    }
    const RWIsvDlink*     key() const;            // Return current item
    void                  reset();
    void                  reset(const RWIsvDlist&);     //set container to be iterated

protected:

    void                  advance() {
        dhere_ = dhere_->next_;
    }
    void                  backup() {
        dhere_ = dhere_->prev_;
    }
    bool          isActive() const;

    RWIsvDlist*           dlist_;                 // The list we are iterating
    RWIsvDlink*           dhere_;                 // Iterator position

private:

    // Disallow postfix increment.  Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool             operator++(int);
    bool             operator--(int);

};

//////////////////////////////////////////////////////////
//                                                      //
//                      INLINES                         //
//                                                      //
//////////////////////////////////////////////////////////

/*
 * Prefix operator: advance iterator one link then return new link.
 * Returns nil if we were at the last link or if the list is empty.
 */
inline RWIsvDlink*
RWIsvDlistIterator::operator++()
{
    // Make sure that tail_ points to itself:
    RW_PRECONDITION(dlist_->tail_.next_ == &dlist_->tail_);

    advance();
    return dhere_ == &dlist_->tail_ ? rwnil : dhere_;
}

/*
 * Prefix operator: backup iterator one link then return new link.
 * Returns nil if we were at the first link or if the list is empty.
 */
inline RWIsvDlink*
RWIsvDlistIterator::operator--()
{
    // Make sure that head_ points to itself:
    RW_PRECONDITION(dlist_->head_.prev_ == &dlist_->head_);

    backup();
    return dhere_ == &dlist_->head_ ? rwnil : dhere_;
}

inline RWIsvDlink*
RWIsvDlistIterator::operator()()
{
    return ++(*this);
}

inline bool
RWIsvDlistIterator::isActive() const
{
    return dhere_ != &dlist_->head_ && dhere_ != &dlist_->tail_;
}

/*
 * Return the current item.  This will be nil if the iterator
 * is out of range: Either past the end, or before the beginning
 * of the list.
 */
inline RWIsvDlink*
RWIsvDlistIterator::key() const
{
    return isActive() ? dhere_ : rwnil;
}

/*
 * Reset iterator: position it to the phantom link at the head of the list
 */
inline void
RWIsvDlistIterator::reset()
{
    dhere_ = &dlist_->head_;
}

/*
 * Prefix operator: advance iterator one link then return new link.
 * Returns nil if we were at the last link or if the list is empty.
 */
inline const RWIsvDlink*
RWIsvDlistConstIterator::operator++()
{
    // Make sure that tail_ points to itself:
    RW_PRECONDITION(dlist_->tail_.next_ == &dlist_->tail_);

    advance();
    return dhere_ == &dlist_->tail_ ? rwnil : dhere_;
}

/*
 * Prefix operator: backup iterator one link then return new link.
 * Returns nil if we were at the first link or if the list is empty.
 */
inline const RWIsvDlink*
RWIsvDlistConstIterator::operator--()
{
    // Make sure that head_ points to itself:
    RW_PRECONDITION(dlist_->head_.prev_ == &dlist_->head_);

    backup();
    return dhere_ == &dlist_->head_ ? rwnil : dhere_;
}

inline const RWIsvDlink*
RWIsvDlistConstIterator::operator()()
{
    return ++(*this);
}

inline bool
RWIsvDlistConstIterator::isActive() const
{
    return dhere_ != &dlist_->head_ && dhere_ != &dlist_->tail_;
}

/*
 * Return the current item.  This will be nil if the iterator
 * is out of range: Either past the end, or before the beginning
 * of the list.
 */
inline const RWIsvDlink*
RWIsvDlistConstIterator::key() const
{
    return isActive() ? dhere_ : rwnil;
}

/*
 * Reset iterator: position it to the phantom link at the head of the list
 */
inline void
RWIsvDlistConstIterator::reset()
{
    dhere_ = &dlist_->head_;
}

#endif  /* RW_TOOLS_IDLIST_H */
