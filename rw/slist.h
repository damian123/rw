#ifndef  RW_TOOLS_SLIST_H
#define  RW_TOOLS_SLIST_H


/**********************************************************************
 *
 * RWSlist: A singly linked list of pointers to void
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/slist.h#1 $
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
#include <rw/islist.h>

/*
 * This is the actual link which includes a pointer
 */

class RW_TOOLS_SYMBOLIC RWPSlink : public RWIsvSlink
{

public:

    RWPSlink(void* a) : info_(a) {
        ;
    }
    RWPSlink*     next() const {
        return (RWPSlink*)next_;
    }

    void*         info_;
};

/****************************************************************
 *                                                              *
 *              Declarations for RWSlist                        *
 *                                                              *
 ****************************************************************/

class RW_TOOLS_GLOBAL RWSlist : private RWIsvSlist
{

public:

    RWSlist() {
        ;
    }
    RWSlist(void* a) : RWIsvSlist() {
        append(a);
    }
    RWSlist(const RWSlist&);

#  if !defined(RW_NO_RVALUE_REFERENCES)
    RWSlist(RWSlist && lst) {
        if (&lst != this) {
            swap(lst);
        }
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    ~RWSlist();

    // Operators:
    RWSlist&              operator=(const RWSlist& s);

#  if !defined(RW_NO_RVALUE_REFERENCES)
    RWSlist&              operator=(RWSlist && lst) {
        if (&lst != this) {
            swap(lst);
        }
        return *this;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    // Member functions:
    void*                 append(void* a);
    void                  apply(RWapplyGeneric, void*);
    void                  applyReference(void (*apRef)(void*&, void*), void*);
    void*&                at(size_t n) {
        return ((RWPSlink*)RWIsvSlist::at(n))->info_;
    }
    const void*           at(size_t n) const {
        return ((RWPSlink*)RWIsvSlist::at(n))->info_;
    }
    void                  clear();
    bool             contains(RWtestGeneric tfun, const void* c) const {
        return find(tfun, c) != rwnil;
    }
    bool             containsReference(const void* c) const {
        return findReference(c) != rwnil;
    }
    size_t                entries() const {
        return RWIsvSlist::entries();
    }
    void*                 find(RWtestGeneric, const void*) const;
    void*                 findReference(const void*) const; // First occurrence
    void*                 first() const;
    void*                 get() {
        return peel((RWPSlink*)RWIsvSlist::get());
    }
    size_t                index(RWtestGeneric, const void*) const;
    void*                 insert(void* a) {
        return append(a);
    }
    void*                 insertAfter(int i, void* a); // For backward compatibility
    void*                 insertAt(size_t i, void* a);
    bool             isEmpty() const {
        return RWIsvSlist::isEmpty();
    }
    void*                 last() const;
    size_t                occurrencesOf(RWtestGeneric, const void*) const;
    size_t                occurrencesOfReference(const void*) const;
    void*                 prepend(void* a);
    void*                 remove(RWtestGeneric, const void*);
    void*                 removeReference(const void*);
    void                  swap(RWSlist& lst) {
        RWIsvSlist::swap(lst);
    }
protected:

    RWPSlink*             headLink() const;
    RWPSlink*             tailLink() const;
    RWPSlink*             firstLink() const {
        return (RWPSlink*)head_.next_;
    }
    RWPSlink*             lastLink() const {
        return (RWPSlink*)last_;
    }

    // Type safe interfaces to base class:
    void                  insertAfterLink(RWPSlink* p, RWPSlink* v) {
        RWIsvSlist::insertAfterLink(p, v);
    }
    RWPSlink*             removeRight(RWPSlink* p) {
        return (RWPSlink*)RWIsvSlist::removeRight(p);
    }

    static void*          peel(RWPSlink*);

private:

    friend class RW_TOOLS_GLOBAL RWSlistIterator;
    friend class RW_TOOLS_GLOBAL RWSlistConstIterator;

};


/*
 * Declaration for iterator for singly linked list.
 */
class RW_TOOLS_GLOBAL RWSlistIterator :  private RWIsvSlistIterator
{
    friend class RWSlistConstIterator;
public:

    // Constructor: cast needed for Glock V2.0b:
    RWSlistIterator(RWSlist& s) : RWIsvSlistIterator((RWIsvSlist&)s) {
        ;
    }
    virtual ~RWSlistIterator() {}

    void*                 operator++();
    void*                 operator+=(size_t n);
    void*                 operator()();

    bool             atFirst() const {
        return RWIsvSlistIterator::atFirst();
    }
    bool             atLast() const {
        return RWIsvSlistIterator::atLast();
    }
    void                  toFirst() {
        RWIsvSlistIterator::toFirst();
    }
    void                  toLast() {
        RWIsvSlistIterator::toLast();
    }

    // Methods:
    RWSlist*              container() const {
        return (RWSlist*)slist_;
    }
    void*                 findNext(RWtestGeneric, const void*);
    void*                 findNextReference(const void*);
    void*                 insertAfterPoint(void*);
    void                  insertAfterPoint(RWIsvSlink* p) {
        RWIsvSlistIterator::insertAfterPoint(p);
    }
    void*                 key() const {
        return isActive() ? cursor()->info_ : rwnil;
    }
    void*                 remove() {
        return RWSlist::peel((RWPSlink*)RWIsvSlistIterator::remove());
    }
    void*                 removeNext(RWtestGeneric, const void*);
    void*                 removeNextReference(const void*);
    void                  reset() {
        RWIsvSlistIterator::reset();
    }
    void                  reset(RWIsvSlist& l) {
        RWIsvSlistIterator::reset(l);
    }
    void                  reset(RWSlist& sl) {
        RWIsvSlistIterator::reset((RWIsvSlist&)sl);
    }

protected:

    RWPSlink*             cursor() const {
        return (RWPSlink*)shere_;
    }

};


/*
 * Declaration for const iterator for singly linked list.
 */
class RW_TOOLS_GLOBAL RWSlistConstIterator : private RWIsvSlistConstIterator
{

public:

    // Constructor: cast needed for Glock V2.0b:
    RWSlistConstIterator(RWSlist& s) : RWIsvSlistConstIterator((RWIsvSlist&)s) {
        ;
    }
    RWSlistConstIterator(const RWSlistIterator& it)
        : RWIsvSlistConstIterator(it) { }
    virtual ~RWSlistConstIterator() {}

    const void*           operator++();
    const void*           operator+=(size_t n);
    const void*           operator()();

    bool             atFirst() const {
        return RWIsvSlistConstIterator::atFirst();
    }
    bool             atLast() const {
        return RWIsvSlistConstIterator::atLast();
    }
    void                  toFirst() {
        RWIsvSlistConstIterator::toFirst();
    }
    void                  toLast() {
        RWIsvSlistConstIterator::toLast();
    }

    // Methods:
    const RWSlist*              container() const {
        return static_cast<const RWSlist*>(const_cast<RWIsvSlist*>(slist_));
    }
    const void*                 findNext(RWtestGeneric, const void*);
    const void*                 findNextReference(const void*);
    const void*                 key() const {
        return isActive() ? cursor()->info_ : rwnil;
    }
    void                  reset() {
        RWIsvSlistConstIterator::reset();
    }
    void                  reset(RWSlist& sl) {
        RWIsvSlistConstIterator::reset(static_cast<RWIsvSlist&>(sl));
    }

protected:

    RWPSlink*             cursor() const {
        return static_cast<RWPSlink*>(const_cast<RWIsvSlink*>(shere_));
    }

};

//////////////////////////////////////////////////////////
//                                                      //
//                      INLINES                         //
//                                                      //
//////////////////////////////////////////////////////////

inline void*
RWSlist::first() const
{
    return isEmpty() ? rwnil : firstLink()->info_;
}

inline void*
RWSlist::last() const
{
    return isEmpty() ? rwnil : lastLink()->info_;
}

inline RWPSlink*
RWSlist::headLink() const
{
    return static_cast<RWPSlink*>(
               &const_cast<RWSlist*>(this)->head_);
}

inline RWPSlink*
RWSlist::tailLink() const
{
    return static_cast<RWPSlink*>(
               &const_cast<RWSlist*>(this)->tail_);
}

inline void*
RWSlistIterator::operator++()
{
    // Make sure tail link points to itself:
    RW_PRECONDITION(container()->tailLink()->next() == container()->tailLink());
    advance();
    return cursor() == container()->tailLink() ? rwnil : cursor()->info_;
}

inline void*
RWSlistIterator::operator()()
{
    return ++(*this);
}

inline const void*
RWSlistConstIterator::operator++()
{
    // Make sure tail link points to itself:
    RW_PRECONDITION(container()->tailLink()->next() == container()->tailLink());
    advance();
    return cursor() == container()->tailLink() ? rwnil : const_cast<const void*>(cursor()->info_);
}

inline const void*
RWSlistConstIterator::operator()()
{
    return ++(*this);
}

#endif /* RW_TOOLS_SLIST_H */
