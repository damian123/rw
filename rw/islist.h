#ifndef  RW_TOOLS_ISLIST_H
#define  RW_TOOLS_ISLIST_H



/**********************************************************************
 *
 * RWIsvSlist: An intrusive singly-linked list.
 * See Stroustrup II, Section 8.3.1 for a guide to intrusive lists.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/islist.h#1 $
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
 *                                                              *
 *              Declaration for RWIsvSlink                      *
 *                                                              *
 ****************************************************************/


class RW_TOOLS_SYMBOLIC RWIsvSlink RWMemoryPool_OPTION
{

public:

#ifdef RW_DEBUG
    RWIsvSlink() : next_(rwnil) {
        ;
    }
#else
    RWIsvSlink() {
        ;
    }
#endif

    RWIsvSlink*           next() const {
        return next_;
    }
    RWIsvSlink*           clear() {
        RWIsvSlink* ret = next_;
        next_ = rwnil;
        return ret;
    }

    RWIsvSlink*           next_;          // Pointer to next link.
};


/****************************************************************
 *                                                              *
 *              Declarations for RWIsvSlist                     *
 *                                                              *
 ****************************************************************/

class RW_TOOLS_SYMBOLIC RWIsvSlist
{

public:

    RWIsvSlist() {
        init();
    }
    RWIsvSlist(RWIsvSlink* a);
    ~RWIsvSlist() {
        clear();
    }

    /********************* Member functions **************************/
    void                  append(RWIsvSlink* a) {
        insertAfterLink(last_, a);
    }
    RWIsvSlink*           at(size_t i) const;     // Index must be in range
    void                  clear();
    bool             containsReference(const RWIsvSlink*) const;
    size_t                entries() const {
        return nitems_;
    }
    RWIsvSlink*           first() const {
        return isEmpty() ? rwnil : head_.next_;
    }
    RWIsvSlink*           get() {
        return removeFirst();
    }
    void                  insert(RWIsvSlink* a) {
        append(a);
    }
    void                  insertAt(size_t, RWIsvSlink*);
    bool             isEmpty() const {
        return nitems_ == 0;
    }
    RWIsvSlink*           last() const {
        return isEmpty() ? rwnil : last_;
    }
    size_t                occurrencesOfReference(const RWIsvSlink* a) const {
        return containsReference(a) ? 1u : 0;
    }
    void                  prepend(RWIsvSlink* a) {
        insertAfterLink(&head_, a);
    }
    RWIsvSlink*           removeAt(size_t);               // Relatively slow
    RWIsvSlink*           removeFirst() {
        return isEmpty() ? rwnil : removeRight(&head_);
    }
    RWIsvSlink*           removeLast() {
        return isEmpty() ? rwnil : removeReference(last_);
    }
    RWIsvSlink*           removeReference(RWIsvSlink* a);//rem,ret link at addr a
    void                  swap(RWIsvSlist& lst);

protected:

    RWIsvSlink*           findLeftIsv(const RWIsvSlink*) const;
    void                  init();
    void                  insertAfterLink(RWIsvSlink*, RWIsvSlink*);
    RWIsvSlink*           removeRight(RWIsvSlink*);  //Rem,ret link after arg

    RWIsvSlink            head_;          // Sentinel
    RWIsvSlink            tail_;          // Sentinel
    RWIsvSlink*           last_;          // Points to last link
    size_t                nitems_;        // Number of entries in the list

private:

    // This class is not copyable or assignable.
    RWIsvSlist(const RWIsvSlist&);
    RWIsvSlist& operator=(const RWIsvSlist&);

    friend class RW_TOOLS_SYMBOLIC RWIsvSlistIterator;
    friend class RW_TOOLS_GLOBAL RWSlistIterator;
    friend class RWIsvSlistConstIterator;

};


/****************************************************************
 *                                                              *
 *              Declarations for RWIsvSlistIterator             *
 *                                                              *
 ****************************************************************/

// Iterator for singly linked list:

class RW_TOOLS_SYMBOLIC RWIsvSlistIterator
{
    friend class RWIsvSlistConstIterator;
public:

    RWIsvSlistIterator(RWIsvSlist& s) : slist_(&s), shere_(&s.head_) {
        ;
    }

    // Operators:
    RWIsvSlink*           operator++();           // Advance and return.
    RWIsvSlink*           operator+=(size_t n);   // Advance n links and return.
    RWIsvSlink*           operator()();           // Advance and return.  For lateral compatibility

    bool             atFirst() const;
    bool             atLast() const;
    void                  toFirst();
    void                  toLast();

    RWIsvSlist*           container() const {
        return slist_;
    }
    void                  insertAfterPoint(RWIsvSlink*);// Insert item after position.
    RWIsvSlink*           key() const;            // Return current item
    RWIsvSlink*           remove();               // Remove current item (slow)
    void                  reset();
    void                  reset(RWIsvSlist&);     // Reset container to be iterated over.

protected:

    void                  advance() {
        shere_ = shere_->next_;
    }
    bool             isActive() const;

    RWIsvSlist*           slist_;                 // The list over which we are iterating
    RWIsvSlink*           shere_;                 // Iterator position

};


/****************************************************************
 *                                                              *
 *              Declarations for RWIsvSlistConstIterator        *
 *                                                              *
 ****************************************************************/

// const iterator for singly linked list:

class RW_TOOLS_SYMBOLIC RWIsvSlistConstIterator
{

public:

    RWIsvSlistConstIterator(RWIsvSlist& s) : slist_(&s), shere_(&s.head_) {
        ;
    }
    RWIsvSlistConstIterator(const RWIsvSlistIterator& it)
        : slist_(it.slist_), shere_(it.shere_) { }

    // Operators:
    const RWIsvSlink*     operator++();           // Advance and return.
    const RWIsvSlink*     operator+=(size_t n);   // Advance n links and return.
    const RWIsvSlink*     operator()();           // Advance and return.  For lateral compatibility

    bool          atFirst() const;
    bool          atLast() const;
    void                  toFirst();
    void                  toLast();

    const RWIsvSlist*     container() const {
        return slist_;
    }

    const RWIsvSlink*     key() const;            // Return current item

    void                  reset();
    void                  reset(const RWIsvSlist&);     // Reset container to be iterated over.

protected:

    void                  advance() {
        shere_ = shere_->next_;
    }
    bool          isActive() const;

    RWIsvSlist*           slist_;                 // The list over which we are iterating
    RWIsvSlink*           shere_;                 // Iterator position

private:

    // Disallow postfix increment.  Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool          operator++(int);

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
inline RWIsvSlink*
RWIsvSlistIterator::operator++()
{
    RW_PRECONDITION(slist_->tail_.next_ == &slist_->tail_);
    advance();
    return shere_ == &slist_->tail_ ? rwnil : shere_;
}

inline RWIsvSlink*
RWIsvSlistIterator::operator()()
{
    return ++(*this);
}

inline bool
RWIsvSlistIterator::isActive() const
{
    return shere_ != &slist_->head_ && shere_ != &slist_->tail_;
}

/*
 * Return the current item.  This will be nil if the iterator
 * is out of range: Either past the end, or before the beginning
 * of the list.
 */
inline RWIsvSlink*
RWIsvSlistIterator::key() const
{
    return isActive() ? shere_ : rwnil;
}

/*
 * Reset iterator: position it to the phantom link at the head of the list
 */
inline void
RWIsvSlistIterator::reset()
{
    shere_ = &slist_->head_;
}

/*
 * Prefix operator: advance iterator one link then return new link.
 * Returns nil if we were at the last link or if the list is empty.
 */
inline const RWIsvSlink*
RWIsvSlistConstIterator::operator++()
{
    RW_PRECONDITION(slist_->tail_.next_ == &slist_->tail_);
    advance();
    return shere_ == &slist_->tail_ ? rwnil : shere_;
}

inline const RWIsvSlink*
RWIsvSlistConstIterator::operator()()
{
    return ++(*this);
}

inline bool
RWIsvSlistConstIterator::isActive() const
{
    return shere_ != &slist_->head_ && shere_ != &slist_->tail_;
}

/*
 * Return the current item.  This will be nil if the iterator
 * is out of range: Either past the end, or before the beginning
 * of the list.
 */
inline const RWIsvSlink*
RWIsvSlistConstIterator::key() const
{
    return isActive() ? shere_ : rwnil;
}

/*
 * Reset iterator: position it to the phantom link at the head of the list
 */
inline void
RWIsvSlistConstIterator::reset()
{
    shere_ = &slist_->head_;
}

#endif  /* RW_TOOLS_ISLIST_H */
