#ifndef  RW_TOOLS_DLIST_H
#define  RW_TOOLS_DLIST_H



/**********************************************************************
 *
 * Declarations for Doubly-linked lists.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/dlist.h#1 $
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
#include <rw/idlist.h>


/*
 * This is the actual link which includes a pointer
 */
class RW_TOOLS_SYMBOLIC RWPDlink : public RWIsvDlink
{

public:

    RWPDlink(void* a) : info_(a) {
        ;
    }
    RWPDlink*     next() const {
        return (RWPDlink*)next_;
    }

    void*         info_;

};

/****************************************************************
 *
 *    Declarations for RWDlist
 *
 ****************************************************************/

class RW_TOOLS_GLOBAL RWDlist : private RWIsvDlist
{
public:

    RWDlist() { }
    RWDlist(void* a) : RWIsvDlist() {
        append(a);
    }

    RWDlist(const RWDlist&);

#  if !defined(RW_NO_RVALUE_REFERENCES)
    RWDlist(RWDlist && lst) {
        if (&lst != this) {
            swap(lst);
        }
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    ~RWDlist() {
        clear();
    }

    // Operators:
    RWDlist&              operator=(const RWDlist&);

#  if !defined(RW_NO_RVALUE_REFERENCES)
    RWDlist&              operator=(RWDlist && lst) {
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
        return ((RWPDlink*)RWIsvDlist::at(n))->info_;
    }
    const void*           at(size_t n) const {
        return ((RWPDlink*)RWIsvDlist::at(n))->info_;
    }

    void                  clear();
    bool             contains(RWtestGeneric tfun, const void* c) const {
        return (find(tfun, c) != rwnil);
    }
    bool             containsReference(const void* c) const {
        return (findReference(c) != rwnil);
    }
    size_t                entries() const {
        return RWIsvDlist::entries();
    }
    void*                 find(RWtestGeneric, const void*) const;
    void*                 findReference(const void*) const; // First occurrence
    void*                 first() const;
    void*                 get() {
        return peel((RWPDlink*)RWIsvDlist::get());
    }
    size_t                index(RWtestGeneric, const void*) const;
    void*                 insert(void* a) {
        return append(a);
    }
    void*                 insertAfter(int, void*); // For backward compatibility
    void*                 insertAt(size_t, void*);
    bool             isEmpty() const {
        return RWIsvDlist::isEmpty();
    }
    void*                 last() const;
    size_t                occurrencesOf(RWtestGeneric, const void*) const;
    size_t                occurrencesOfReference(const void*) const;
    void*                 prepend(void* a);
    void*                 remove(RWtestGeneric, const void*);
    void*                 removeReference(const void*);
    void                  swap(RWDlist& lst) {
        RWIsvDlist::swap(lst);
    }

protected:

    RWPDlink*             headLink() const;
    RWPDlink*             tailLink() const;
    RWPDlink*             firstLink() const {
        return (RWPDlink*)head_.next_;
    }
    RWPDlink*             lastLink() const {
        return (RWPDlink*)tail_.prev_;
    }

    // Type safe interface to base class:
    void                  insertAfterLink(RWPDlink* p, RWPDlink* v) {
        RWIsvDlist::insertAfterLink(p, v);
    }

    static void*          peel(RWPDlink*);

private:

    friend class RW_TOOLS_GLOBAL RWDlistIterator;
    friend class RW_TOOLS_GLOBAL RWDlistConstIterator;

};


/*
 * Declaration for iterator for doubly-linked list.
 */
class RW_TOOLS_GLOBAL RWDlistIterator :  private RWIsvDlistIterator
{
    friend class RWDlistConstIterator;
public:

    RWDlistIterator(RWDlist& s) : RWIsvDlistIterator(s) {
    }

    virtual ~RWDlistIterator();

    void*                 operator++();
    void*                 operator--();
    void*                 operator+=(size_t n);
    void*                 operator-=(size_t n);
    void*                 operator()();

    bool             atFirst() const {
        return RWIsvDlistIterator::atFirst();
    }
    bool             atLast() const {
        return RWIsvDlistIterator::atLast();
    }
    void                  toFirst() {
        RWIsvDlistIterator::toFirst();
    }
    void                  toLast() {
        RWIsvDlistIterator::toLast();
    }

    // Methods:
    RWDlist*              container() const {
        return (RWDlist*)dlist_;
    }
    void*                 findNext(RWtestGeneric, const void*);
    void*                 findNextReference(const void*);
    void*                 insertAfterPoint(void*);
    void*                 key() const {
        return isActive() ? cursor()->info_ : rwnil;
    }
    void*                 remove() {
        return RWDlist::peel((RWPDlink*)RWIsvDlistIterator::remove());
    }
    void*                 removeNext(RWtestGeneric, const void*);
    void*                 removeNextReference(const void*);
    void                  reset() {
        RWIsvDlistIterator::reset();
    }
    void                  reset(RWDlist& sl) {
        RWIsvDlistIterator::reset((RWIsvDlist&)sl);
    }

protected:

    RWPDlink*             cursor() const {
        return (RWPDlink*)dhere_;
    }

};


/*
 * Declaration for const iterator for doubly-linked list.
 */
class RW_TOOLS_GLOBAL RWDlistConstIterator : private RWIsvDlistConstIterator
{
public:

    // Constructor: cast needed for Glock V2.0b:
    RWDlistConstIterator(RWDlist& s) : RWIsvDlistConstIterator((RWIsvDlist&)s) {
        ;
    }
    RWDlistConstIterator(const RWDlistIterator& it)
        : RWIsvDlistConstIterator(it) { }
    virtual ~RWDlistConstIterator();

    const void*                 operator++();
    const void*                 operator--();
    const void*                 operator+=(size_t n);
    const void*                 operator-=(size_t n);
    const void*                 operator()();

    bool             atFirst() const {
        return RWIsvDlistConstIterator::atFirst();
    }
    bool             atLast() const {
        return RWIsvDlistConstIterator::atLast();
    }
    void                  toFirst() {
        RWIsvDlistConstIterator::toFirst();
    }
    void                  toLast() {
        RWIsvDlistConstIterator::toLast();
    }

    // Methods:
    const RWDlist*        container() const {
        return static_cast<const RWDlist*>(const_cast<RWIsvDlist*>(dlist_));
    }
    const void*           findNext(RWtestGeneric, const void*);
    const void*           findNextReference(const void*);
    const void*           key() const {
        return isActive() ? const_cast<const void*>(cursor()->info_) : rwnil;
    }
    void                  reset() {
        RWIsvDlistConstIterator::reset();
    }
    void                  reset(RWDlist& sl) {
        RWIsvDlistConstIterator::reset((RWIsvDlist&)sl);
    }

protected:

    RWPDlink*             cursor() const {
        return static_cast<RWPDlink*>(const_cast<RWIsvDlink*>(dhere_));
    }

    void                  operator++(int);
    void                  operator--(int);
};

//////////////////////////////////////////////////////////
//                                                      //
//                      INLINES                         //
//                                                      //
//////////////////////////////////////////////////////////

inline void*
RWDlist::first() const
{
    return isEmpty() ? rwnil : firstLink()->info_;
}

inline void*
RWDlist::last() const
{
    return isEmpty() ? rwnil : lastLink()->info_;
}

inline RWPDlink*
RWDlist::headLink() const
{
    // Cast away constness to suppress unfounded cfront warning:
    return reinterpret_cast<RWPDlink*>(
               &const_cast<RWDlist*>(this)->head_);
}

inline RWPDlink*
RWDlist::tailLink() const
{
    // Cast away constness to suppress unfounded cfront warning:
    return reinterpret_cast<RWPDlink*>(
               &const_cast<RWDlist*>(this)->tail_);
}

inline void*
RWDlistIterator::operator++()
{
    // Make sure tail link points to itself:
    RW_PRECONDITION(container()->tailLink()->next() == container()->tailLink());

    advance();
    return cursor() == container()->tailLink() ? rwnil : cursor()->info_;
}

inline void*
RWDlistIterator::operator--()
{
    // Make sure head link points to itself:
    RW_PRECONDITION(container()->headLink()->prev() == container()->headLink());

    backup();
    return cursor() == container()->headLink() ? rwnil : cursor()->info_;
}

inline void*
RWDlistIterator::operator()()
{
    return ++(*this);
}

inline const void*
RWDlistConstIterator::operator++()
{
    // Make sure tail link points to itself:
    RW_PRECONDITION(container()->tailLink()->next() == container()->tailLink());

    advance();
    return cursor() == container()->tailLink() ? rwnil : const_cast<const void*>(cursor()->info_);
}

inline const void*
RWDlistConstIterator::operator--()
{
    // Make sure head link points to itself:
    RW_PRECONDITION(container()->headLink()->prev() == container()->headLink());

    backup();
    return cursor() == container()->headLink() ? rwnil : const_cast<const void*>(cursor()->info_);
}

inline const void*
RWDlistConstIterator::operator()()
{
    return ++(*this);
}

#endif  /* RW_TOOLS_DLIST_H */
