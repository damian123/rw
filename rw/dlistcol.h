#ifndef  RW_TOOLS_DLISTCOL_H
#define  RW_TOOLS_DLISTCOL_H



/**********************************************************************
 *
 * Declarations for Doubly-linked list of RWCollectables.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/dlistcol.h#1 $
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
#include <rw/dlist.h>
#include <rw/seqcltn.h>
#include <rw/iterator.h>

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Represents a group of ordered items, not accessible by an
 * external key
 *
 * Class RWDlistCollectables represents a group of ordered items,
 * not accessible by an external key. Duplicates are allowed.
 * The ordering of elements is determined externally, generally
 * by the order of insertion and removal. An object stored by
 * RWDlistCollectables must inherit the abstract base class RWCollectable.
 *
 * Class RWDlistCollectables is implemented as a doubly-linked list,
 * which allows for efficient insertion and removal, as well as for
 * movement in either direction.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/dlistcol.h>
 * RWDlistCollectables a;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWDlistCollectables :
    public RWSequenceable, public RWDlist
{

    friend class RW_TOOLS_GLOBAL RWDlistCollectablesIterator;

private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWDlistCollectables);
    friend RWCollectable* rwCreateFN(RWDlistCollectables)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns the unique ID for RWDlistCollectables.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWDlistCollectables*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWDlistCollectables*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWDlistCollectables& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWDlistCollectables& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWDlistCollectables& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWDlistCollectables& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an empty doubly-linked list.
     */
    RWDlistCollectables()  :
        RWDlist() { }

    /**
     * Constructs a linked list with a single item \a a.
     */
    RWDlistCollectables(RWCollectable* a) :
        RWDlist(a) { }

    /**
     * Makes a shallow copy of \a lst.
     */
    RWDlistCollectables(const RWDlistCollectables& lst) :
        RWSequenceable(lst), RWDlist(lst) { }

    /**
     * Empty Destructor.
     */
    virtual ~RWDlistCollectables() {}


#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWDlistCollectables takes ownership of
     * the data owned by \a lst.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWDlistCollectables(RWDlistCollectables && lst) :
        RWDlist(rw_move(lst)) { }

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWDlistCollectables& operator=(RWDlistCollectables && rhs) {
        if (&rhs != this) {
            RWDlist::operator=(rw_move(rhs));
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Assignment operator.  Makes a shallow copy of \a rhs.
     */
    RWDlistCollectables& operator=(const RWDlistCollectables& rhs) {
        if (&rhs != this) {
            RWDlist::operator=(rhs);
        }
        return *this;
    }

    /**
     * Returns \c true if self and \a d have the same number of items
     * and if, for every item in self, the corresponding item in the
     * same position in \a d isEqual() to the item in self.
     */
    bool  operator==(const RWDlistCollectables& d) const;

    /**
     * Inserts the item at the end of
     * the collection and returns it.  Returns #rwnil if the insertion
     * was unsuccessful.
     */
    virtual RWCollectable*  append(RWCollectable* a) {
        return (RWCollectable*)RWDlist::append(a);
    }

    /**
     * Invokes the function pointer \a ap on each member in the collection, in
     * order, from first to last.
     */
    virtual void apply(RWapplyCollectable ap, void* x) {
        union {
            RWapplyCollectable pa;
            RWapplyGeneric     pv;
        } pf = { ap };
        RWDlist::apply(pf.pv, x);
    }

    /**
     * Allows access to the \a i <sup>th</sup> element of the collection.
     * The value \a i must be between zero and the number
     * of items in the collection less one, or the function throws an
     * exception of type RWBoundsErr. Note that for a linked
     * list, these functions must traverse all the links, making them
     * particularly inefficient.
     */
    virtual RWCollectable*& at(size_t i) {
        return (RWCollectable*&)RWDlist::at(i);
    }

    /**
     * @copydoc at(size_t)
     */
    virtual const RWCollectable*  at(size_t i) const {
        return (const RWCollectable*)RWDlist::at(i);
    }

    // Documented in base class.
    virtual void clear() {
        RWDlist::clear();
    }

    // Documented in base class.
    virtual bool contains(const RWCollectable* a) const {
        return RWCollection::contains(a);
    }

    // Documented in base class.
    virtual size_t entries() const {
        return RWDlist::entries();
    }

    /**
     * Returns the first item that isEqual()
     * to the item pointed to by \a target, or #rwnil if
     * no item is found.
     */
    virtual RWCollectable* find(const RWCollectable* target) const {
        return (RWCollectable*)RWDlist::find(rwIsEqualFun, target);
    }

    /**
     * Returns the item at the beginning of the list.
     */
    virtual RWCollectable* first() const {
        return (RWCollectable*) RWDlist::first();
    }

    /**
     * Returns the index of the
     * first item that isEqual() to the item pointed to by \a c, or
     * #RW_NPOS if there is no such index.
     */
    virtual size_t index(const RWCollectable* c) const {
        return RWDlist::index(rwIsEqualFun, c);
    }

    /**
     * Adds the item to the end
     * of the collection and returns it.  Returns #rwnil if the insertion
     * was unsuccessful.
     */
    virtual RWCollectable* insert(RWCollectable* c) {
        return (RWCollectable*)RWDlist::insert(c);
    }

    /**
     * Adds a new item to the collection at position
     * \a inx. The item previously at position \a i is moved
     * to \a i + 1, and so forth. The index \a inx must be between
     * zero and the number of items in the collection, or the function
     * throws an exception of type RWBoundsErr.
     */
    virtual RWCollectable* insertAt(size_t inx, RWCollectable* e) {
        return (RWCollectable*)RWDlist::insertAt(inx, e);
    }

    // Documented in base class.
    virtual bool isEmpty() const {
        return RWDlist::isEmpty();
    }

    // Documented in base class.
    virtual bool isEqual(const RWCollectable*) const;

    /**
     * Returns the item at the end of the list.
     */
    virtual RWCollectable* last() const {
        return (RWCollectable*) RWDlist::last();
    }

    /**
     * Returns the number of items that isEqual() to the item
     * pointed to by \a target.
     */
    virtual size_t occurrencesOf(const RWCollectable* target) const {
        return RWDlist::occurrencesOf(rwIsEqualFun, target);
    }

    /**
     * Adds the item to the beginning
     * of the collection and returns it.  Returns #rwnil if the insertion
     * was unsuccessful.
     */
    virtual RWCollectable* prepend(RWCollectable* a) {
        return (RWCollectable*)RWDlist::prepend(a);
    }

    /**
     * Removes and returns the first
     * item that isEqual() to the item pointed to by \a target. Returns
     * #rwnil if there is no such item.
     */
    virtual RWCollectable* remove(const RWCollectable* target) {
        return (RWCollectable*)RWDlist::remove(rwIsEqualFun, target);
    }

    /**
     * Returns true if the list contains an item that <i>is identical
     * to</i> the item pointed to by \a e (that is, that has the address
     * \a e).
     */
    bool containsReference(const RWCollectable* e) const {
        return RWDlist::containsReference(e);
    }

    /**
     * Returns the first item that <i>is identical to</i> the item pointed
     * to by \a e (that is, that has the address \a e), or #rwnil if
     * none is found.
     */
    RWCollectable* findReference(const RWCollectable* e) const {
        return (RWCollectable*)RWDlist::findReference(e);
    }

    /**
     * Returns and \e removes the item at the beginning of the list.
     */
    RWCollectable* get() {
        return (RWCollectable*) RWDlist::get();
    }

    /**
     * Returns the number of items that <i>are identical to</i> the
     * item pointed to by \a e (that is, that have the address \a e).
     */
    size_t occurrencesOfReference(const RWCollectable* e) const {
        return RWDlist::occurrencesOfReference(e);
    }

    /**
     * Removes and returns the first item that <i>is identical to</i>
     * the item pointed to by \a e (that is, that has the address \a
     * e).  Returns #rwnil if there is no such item.
     */
    RWCollectable* removeReference(const RWCollectable* e) {
        return (RWCollectable*)RWDlist::removeReference(e);
    }

    // For backward compatibility:
    virtual RWCollectable* insertAfter(int i, RWCollectable* a) {
        return (RWCollectable*)RWDlist::insertAfter(i, a);
    }

    // Documented in base class.
    virtual RWIterator* newIterator();

    // Documented in base class.
    virtual RWConstIterator* newConstIterator() const;

};

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Traverses the linked-list
 * from the first (head) to the last (tail) item
 *
 * Iterator for class RWDlistCollectables. Traverses the linked-list
 * from the first (head) to the last (tail) item.  Functions are
 * provided for moving in \e either direction.
 *
 * As with all Rogue Wave iterators, the "current item" is undefined
 * immediately after construction  --  you must define it by using
 * operator()() or some other (valid) operation.
 *
 * Once the iterator has advanced beyond the end of the collection
 * it is no longer valid  --  continuing to use it brings undefined
 * results.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/dlistcol.h>
 * RWDlistCollectables d;
 * RWDlistCollectablesIterator it(d);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWDlistCollectablesIterator :
    public RWIterator, public RWDlistIterator
{
    friend class RWDlistCollectablesConstIterator;

public:

    /**
     * Constructs an RWDlistCollectablesIterator from an RWDlistCollectables.
     * Immediately after construction, the position of the iterator is
     * undefined.
     */
    RWDlistCollectablesIterator(RWDlistCollectables& s)
#if defined(__HP_aCC) && __HP_aCC >= 53600 && __HP_aCC <= 55000
    ;
#else
        :
        RWDlistIterator(s) { }
#endif // aCC 5.xx

    virtual ~RWDlistCollectablesIterator() {}

    /**
     * Moves the iterator to the next
     * item which \link RWCollectable::isEqual() isEqual() \endlink to the
     * item pointed to by \a target and returns it.
     *  If no item is found, the function returns #rwnil and
     * the position of the iterator is undefined.
     */
    virtual RWCollectable* findNext(const RWCollectable* target) {
        return (RWCollectable*)RWDlistIterator::findNext(rwIsEqualFun, target);
    }

    // Documented in base class.
    virtual RWCollectable* key() const {
        return (RWCollectable*)RWDlistIterator::key();
    }

    // Documented in base class.
    virtual RWCollectable* operator()() {
        return (RWCollectable*)RWDlistIterator::operator()();
    }

    /**
     * Resets the iterator.  Afterwards, the position of the iterator
     * is undefined.
     */
    virtual void reset() {
        RWDlistIterator::reset();
    }

    /**
     * Moves the iterator to the next item which <i>is identical to</i>
     * the item pointed to by \a e (that is, that has address \a e)
     * and returns it.  If no item is found, the function returns #rwnil
     * and the position of the iterator is undefined.
     */
    RWCollectable* findNextReference(const RWCollectable* e) {
        return (RWCollectable*)RWDlistIterator::findNextReference(e);
    }

    /**
     * Inserts the item \a a after the current cursor position and returns
     * the item.  The cursor's position is unchanged.
     */
    RWCollectable* insertAfterPoint(RWCollectable* a) {
        return (RWCollectable*)RWDlistIterator::insertAfterPoint(a);
    }

    /**
     * Removes and returns the item at the current cursor position.
     *  Afterwards, the iterator is positioned at the previous
     * item in the list.
     */
    RWCollectable* remove() {
        return (RWCollectable*)RWDlistIterator::remove();
    }

    /**
     * Moves the iterator to the next item in the list which isEqual()
     * to the item pointed to by \a target, removes it from the list
     * and returns it.  Afterwards, the iterator is positioned
     * at the previous item in the list.  If no item is found, the function
     * returns #rwnil and the position of the iterator is undefined.
     */
    RWCollectable* removeNext(const RWCollectable* target) {
        return (RWCollectable*)RWDlistIterator::removeNext(rwIsEqualFun, target);
    }

    /**
     * Moves iterator to the next item in the list which <i>is identical
     * to</i> the item pointed to by \a e (that is, that has address
     * \a e), removes it from the list and returns it.  Afterwards,
     * the iterator is positioned at the previous item in the list.
     *  If no item is found, the function returns #rwnil and the position
     *  of the iterator is undefined.
     */
    RWCollectable* removeNextReference(const RWCollectable* e) {
        return (RWCollectable*)RWDlistIterator::removeNextReference(e);
    }
};

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Traverses the linked-list
 * from the first (head) to the last (tail) item
 *
 * One of two iterators for class RWDlistCollectables. As a \c const
 * iterator, this iterator does not contain any functions that change
 * the data structure it is iterating through.
 *
 * This iterator traverses the linked-list from the first (head)
 * to the last (tail) item.  Functions are provided for moving in
 * \e either direction.
 *
 * As with all Rogue Wave iterators, the "current item" is
 * undefined immediately after construction -- you must define it
 * by using operator()() or some other (valid) operation.
 *
 * Once the iterator has advanced beyond the end of the collection
 * it is no longer valid -- continuing to use it brings undefined
 * results.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/dlistcol.h>
 * RWDlistCollectables d;
 * RWDlistCollectablesConstIterator it(d);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWDlistCollectablesConstIterator
    : public RWConstIterator, public RWDlistConstIterator
{
public:
    /**
     * Constructs an RWDlistCollectablesConstIterator from an
     * RWDlistCollectables. Immediately after construction, the position
     * of the iterator is undefined.
     */
    RWDlistCollectablesConstIterator(const RWDlistCollectables& s)
#if defined(__HP_aCC) && __HP_aCC >= 53600 && __HP_aCC <= 55000
    ;
#else
        :
        RWDlistConstIterator(const_cast<RWDlistCollectables&>(s)) { }
#endif // aCC 5.xx

    /**
     * Constructs an RWDlistCollectablesConstIterator from an
     * RWDlistCollectablesIterator. Immediately after construction, the position
     * of the iterator is undefined.
     */
    RWDlistCollectablesConstIterator(const RWDlistCollectablesIterator& it)
#if !defined(__HP_aCC) || __HP_aCC < 53600 || __HP_aCC > 55000
        : RWDlistConstIterator(it) { }
#else
    ;
#endif // aCC !5.3x or 5.50

    virtual ~RWDlistCollectablesConstIterator() {}

    /**
     * Moves the iterator to the
     * next item which isEqual() to the item pointed to by \a target
     * and returns it.  If no item is found, the function returns #rwnil
     * and the position of the iterator is undefined.
     */
    virtual const RWCollectable* findNext(const RWCollectable* target) {
        return static_cast<const RWCollectable*>(
                   RWDlistConstIterator::findNext(rwIsEqualFun, target));
    }

    // Documented in base class.
    virtual const RWCollectable* key() const {
        return static_cast<const RWCollectable*>(RWDlistConstIterator::key());
    }

    // Documented in base class.
    virtual const RWCollectable* operator()() {
        return static_cast<const RWCollectable*>(
                   RWDlistConstIterator::operator()());
    }

    /**
     * Resets the iterator.  Afterwards, the position of the iterator
     * is undefined.
     */
    virtual void reset() {
        RWDlistConstIterator::reset();
    }

    virtual void reset(RWDlistCollectables& dl) {
        RWDlistConstIterator::reset(static_cast<RWDlist&>(dl));
    }

    virtual const RWDlistCollectables* container() const {
        return static_cast<const RWDlistCollectables*>(
                   RWDlistConstIterator::container());
    }

    /**
     * Moves the iterator to the next item that <i>is identical to</i> the
     * item pointed to by \a e (that is, that has address \a e) and
     * returns it.  If no item is found, the function returns #rwnil and
     * the position of the iterator is undefined.
     */
    const RWCollectable* findNextReference(const RWCollectable* e) {
        return static_cast<const RWCollectable*>(
                   RWDlistConstIterator::findNextReference(e));
    }
};

#endif  /* RW_TOOLS_DLISTCOL_H */
