#ifndef  RW_TOOLS_SLISTCOL_H
#define  RW_TOOLS_SLISTCOL_H


/**********************************************************************
 *
 * Declarations for Singly linked list of RWCollectables.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/slistcol.h#1 $
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
#include <rw/slist.h>
#include <rw/seqcltn.h>
#include <rw/iterator.h>

/**
 * @ingroup rwcollectable_classes
 * @brief Represents a group of ordered elements without keyed access, where
 * duplicates are allowed.
 *
 * Class RWSlistCollectables represents a group of ordered elements,
 * without keyed access.  Duplicates are allowed.  The ordering
 * of elements is determined externally, by the order of insertion
 * and removal.  An object stored by RWSlistCollectables must inherit
 * from abstract base class RWCollectable.
 *
 * You must use the virtual function \link RWCollectable::isEqual()
 * isEqual() \endlink (see class RWCollectable) to find a match
 * between a target and an item in the collection.
 *
 * Class RWSlistCollectables is implemented as a singly-linked list,
 * which allows for efficient insertion and removal, but efficient
 * movement in only one direction.  This class corresponds to the
 * Smalltalk class \b \e LinkedList.
 *
 * @section synopsis Synopsis
 *
 * @code
 * // Smalltalk typedef:
 * typedef RWSlistCollectables LinkedList ;
 * #include <rw/slistcol.h>
 * RWSlistCollectables a;
 * @endcode
 *
 * @section persistence Persistence
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWSlistCollectables
    : public RWSequenceable, public RWSlist
{
    friend class RW_TOOLS_GLOBAL RWSlistCollectablesIterator;
    friend class RW_TOOLS_GLOBAL RWSlistCollectablesConstIslterator;

private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWSlistCollectables);
    friend RWCollectable* rwCreateFN(RWSlistCollectables)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns \c __RWSLISTCOLLECTABLES, i.e. a <i>class identifier</i>,
     * that identifies this object's class.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    // Documented in base class.
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWSlistCollectables*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWSlistCollectables*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWSlistCollectables& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWSlistCollectables& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWSlistCollectables& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWSlistCollectables& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an empty linked list.
     */
    RWSlistCollectables();

    /**
     * Constructs a linked list with single item \a a.
     */
    RWSlistCollectables(RWCollectable* a) :
        RWSlist(a) { }

    /**
     * Makes a shallow copy of \a lst.
     */
    RWSlistCollectables(const RWSlistCollectables& lst) :
        RWSequenceable(lst), RWSlist(lst) { }

    virtual ~RWSlistCollectables();

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWSlistCollectables takes ownership of
     * the data owned by \a lst.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWSlistCollectables(RWSlistCollectables && lst) :
        RWSlist(rw_move(lst)) { }

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWSlistCollectables& operator=(RWSlistCollectables && rhs) {
        if (&rhs != this) {
            RWSlist::operator=(rw_move(rhs));
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Assignment operator.  Makes a shallow copy of \a rhs.
     */
    RWSlistCollectables& operator=(const RWSlistCollectables& rhs) {
        if (&rhs != this) {
            RWSlist::operator=(rhs);
        }
        return *this;
    }

    /**
     * Returns \c true if self and \a s have the same number of members
     * and if, for every item in self, the corresponding item at the
     * same index in \a s isEqual() to it.
     */
    bool  operator==(const RWSlistCollectables& s) const;

    // Documented in base class.
    virtual RWCollectable* append(RWCollectable* a) {
        return (RWCollectable*)RWSlist::append(a);
    }

    /**
     * Invokes the function pointer \a ap on each item in the collection, in
     * order from first to last.
     */
    virtual void apply(RWapplyCollectable ap, void* x) {
        union {
            RWapplyCollectable pa;
            RWapplyGeneric     pv;
        } pf = { ap };
        RWSlist::apply(pf.pv, x);
    }

    /**
     * The index \a i must be between \c 0 and the number of items
     * in the collection less one, or an exception of type RWBoundsErr
     * is thrown.
     *
     * @note
     * For a linked list, these functions must traverse all the links,
     * decreasing their efficiency.
     */
    virtual RWCollectable*& at(size_t i) {
        return (RWCollectable*&)RWSlist::at(i);
    }

    /**
     * @copydoc at(size_t)
     */
    virtual const RWCollectable*  at(size_t i) const {
        return (const RWCollectable*)RWSlist::at(i);
    }

    // Documented in base class.
    virtual void  clear() {
        RWSlist::clear();
    }

    // Documented in base class.
    virtual bool contains(const RWCollectable* target) const {
        return RWCollection::contains(target);
    }

    // Documented in base class.
    virtual size_t entries() const {
        return RWSlist::entries();
    }

    /**
     * Returns the first item that matches \a target, or #rwnil
     * if no item is found.
     */
    virtual RWCollectable*  find(const RWCollectable* target) const {
        return (RWCollectable*)RWSlist::find(rwIsEqualFun, target);
    }

    // Documented in base class.
    virtual RWCollectable* first() const {
        return (RWCollectable*) RWSlist::first();
    }

    /**
     * Returns the index of the first item that isEqual() to the
     * item pointed to by \a c.  If there is no such item, returns
    *      #RW_NPOS.
     */
    virtual size_t index(const RWCollectable* c) const {
        return RWSlist::index(rwIsEqualFun, c);
    }

    /**
     * Adds the item \a a to the end of the collection and returns it.
     * Returns #rwnil if the insertion was unsuccessful.
     */
    virtual RWCollectable* insert(RWCollectable* a) {
        return (RWCollectable*)RWSlist::append(a);
    }

    // Documented in base class.
    virtual RWCollectable* insertAt(size_t indx, RWCollectable* e) {
        return (RWCollectable*)RWSlist::insertAt(indx, e);
    }

    // Documented in base class.
    virtual bool isEmpty() const {
        return RWSlist::isEmpty();
    }

    // Documented in base class.
    virtual bool isEqual(const RWCollectable*) const;

    // Documented in base class.
    virtual RWCollectable* last() const {
        return (RWCollectable*) RWSlist::last();
    }

    /**
     * Returns the number of items that compare isEqual() to
     * the item pointed to by \a target.
     */
    virtual size_t occurrencesOf(const RWCollectable* target) const {
        return RWSlist::occurrencesOf(rwIsEqualFun, target);
    }

    // Documented in base class.
    virtual RWCollectable* prepend(RWCollectable* a) {
        return (RWCollectable*)RWSlist::prepend(a);
    }

    /**
     * Removes and returns the first item that isEqual() to the
     * item pointed to by \a target.  Returns #rwnil if there is
     * no such item.
     */
    virtual RWCollectable* remove(const RWCollectable* target) {
        return (RWCollectable*)RWSlist::remove(rwIsEqualFun, target);
    }

    // Documented in base class.
    virtual RWIterator* newIterator();

    // Documented in base class.
    virtual RWConstIterator* newConstIterator() const;

    /**
     * Returns \c true if the list contains an item that <i>is identical
     * to</i> the item pointed to by \a e (that is, that has the address
     * \a e).
     */
    bool containsReference(const RWCollectable* e) const {
        return RWSlist::containsReference(e);
    }

    /**
     * Returns the first item that <i>is identical to</i> the item pointed
     * to by \a e (that is, that has the address \a e), or #rwnil if
     * none is found.
     */
    RWCollectable* findReference(const RWCollectable* e) const {
        return (RWCollectable*)RWSlist::findReference(e);
    }

    /**
     * Returns and \e removes the item at the beginning of the list.
     */
    RWCollectable* get() {
        return (RWCollectable*) RWSlist::get();
    }

    /**
     * Returns the number of items that <i>are identical to</i> the
     * item pointed to by \a e (that is, that have the address \a e).
     */
    size_t occurrencesOfReference(const RWCollectable* e) const {
        return RWSlist::occurrencesOfReference(e);
    }

    /**
     * Removes and returns the first item that <i>is identical to</i>
     * the item pointed to by \a e (that is, that has the address
     * \a e).  Returns #rwnil if there is no such item.
     */
    RWCollectable* removeReference(const RWCollectable* e) {
        return (RWCollectable*)RWSlist::removeReference(e);
    }

    // For backward compatibility:
    virtual RWCollectable*  insertAfter(int i, RWCollectable* a) {
        return (RWCollectable*)RWSlist::insertAfter(i, a);
    }
};


/**
 * @ingroup rwcollectable_classes
 * @brief Iterator for class RWSlistCollectables. Traverses the linked-list
 * from the first to last item.
 *
 * Iterator for class RWSlistCollectables. Traverses the linked-list
 * from the first to last item.
 *
 * The "current item" is undefined immediately after construction.
 * You must define it by using operator()() or some other (valid)
 * operation.
 *
 * Once the iterator has advanced beyond the end of the collection,
 * it is no longer valid; continuing to use it will result in undefined
 * behavior.
 *
 * @section synopsis Synopsis
 *
 * @code
 * // Smalltalk typedef.
 * typedef RWSlistCollectablesIterator LinkedListIterator;
 * #include <rw/slistcol.h>
 * RWSlistCollectables sc;
 * RWSlistCollectablesIterator sci(sc) ;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWSlistCollectablesIterator
    : public RWIterator, public RWSlistIterator
{
public:

    /**
     * Constructs an iterator from a singly-linked list.  Immediately
     * after construction, the position of the iterator is undefined.
     */
    RWSlistCollectablesIterator(RWSlistCollectables& s)
#if defined(__HP_aCC) && __HP_aCC >= 53600 && __HP_aCC <= 55000
    ;
#else
        :
        RWSlistIterator(s) { }
#endif

    virtual ~RWSlistCollectablesIterator() {}

    /**
     * Returns \c true if the iterator is at the beginning of the list,
     * otherwise returns \c false.
     */
    bool atFirst() const {
        return RWSlistIterator::atFirst();
    }

    /**
     * Returns \c true if the iterator is at the end of the list,
     * otherwise returns \c false;
     */
    bool atLast() const {
        return RWSlistIterator::atLast();
    }

    /**
     * Moves the iterator to the beginning of the list.
     */
    void toFirst() {
        RWSlistIterator::toFirst();
    }

    /**
     * Moves the iterator to the end of the list.
     */
    void toLast() {
        RWSlistIterator::toLast();
    }

    /**
     * Advances the iterator one item.
     */
    void* operator++() {
        return RWSlistIterator::operator++();
    }

    /**
     * Advances the iterator \a n items.
     */
    void* operator+=(size_t n) {
        return RWSlistIterator::operator+=(n);
    }

    /**
     * Moves iterator to the next item that isEqual() to the item
     * pointed to by \a target and
     * returns it.  If no item is found, returns #rwnil and the position
     * of the iterator will be undefined.
     */
    virtual RWCollectable* findNext(const RWCollectable* target) {
        return (RWCollectable*)RWSlistIterator::findNext(rwIsEqualFun, target);
    }

    // Documented in base class.
    virtual RWCollectable* key() const {
        return (RWCollectable*)RWSlistIterator::key();
    }

    // Documented in base class.
    virtual RWCollectable* operator()() {
        return (RWCollectable*)RWSlistIterator::operator()();
    }

    /**
     * Resets the iterator.  Afterwards,
     * the position of the iterator will be undefined.
     */
    virtual void reset() {
        RWSlistIterator::reset();
    }

    /**
     * Moves iterator to the next item which <i>is identical to</i>
     * the item pointed to by \a e (that is, that has address \a e)
     * and returns it.  If no item is found, returns #rwnil and the
     * position of the iterator will be undefined.
     */
    RWCollectable* findNextReference(const RWCollectable* e) {
        return (RWCollectable*)RWSlistIterator::findNextReference(e);
    }

    /**
     * Insert item \a a after the current cursor position and return
     * the item.  The cursor's position will be unchanged.
     */
    RWCollectable* insertAfterPoint(RWCollectable* a) {
        return (RWCollectable*)RWSlistIterator::insertAfterPoint(a);
    }

    /**
     * Removes and returns the item at the current cursor position.
     *  Afterwards, the iterator will be positioned at the previous
     * item in the list.  This function is not very efficient in a singly-linked
     * list.
     */
    RWCollectable* remove() {
        return (RWCollectable*)RWSlistIterator::remove();
    }

    /**
     * Moves iterator to the next item in the list that isEqual()
     * to the item pointed to by \a target, removes it from the list
     * and returns it.  Afterwards, the iterator will be positioned
     * at the previous item in the list.  If no item is found, returns
     * #rwnil and the position of the iterator will be undefined.
     */
    RWCollectable* removeNext(const RWCollectable* target) {
        return (RWCollectable*)RWSlistIterator::removeNext(rwIsEqualFun, target);
    }

    /**
     * Moves iterator to the next item in the list which <i>is identical
     * to</i> the item pointed to by \a e (that is, that has address
     * \a e), removes it from the list and returns it.  Afterwards,
     * the iterator will be positioned at the previous item in the list.
     *  If no item is found, returns #rwnil and the position of the
     * iterator will be undefined.
     */
    RWCollectable* removeNextReference(const RWCollectable* e) {
        return (RWCollectable*)RWSlistIterator::removeNextReference(e);
    }
};

/**
 * @ingroup rwcollectable_classes
 * @brief Const iterator for class RWSlistCollectables that traverses
 * the linked-list from the first to last item.
 *
 * One of two iterators for class RWSlistCollectables. This version
 * is a \c const iterator, that cannot effect any changes to the
 * data structure it is iterating through.  This iterator traverses
 * the linked-list from the first to last item.
 *
 * The "current item" is undefined immediately after construction
 *  --  you must define it by using operator()() or some other (valid)
 * operation.
 *
 * Once the iterator has advanced beyond the end of the collection
 * it is no longer valid -- continuing to use it will bring undefined
 * results.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/slistcol.h>
 * RWSlistCollectables sc;
 * RWSlistCollectablesConstIterator sci(sc);
 *
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWSlistCollectablesConstIterator :
    public RWConstIterator, public RWSlistConstIterator
{
public:

    /**
     * Constructs a \c const iterator from a singly-linked list.  Immediately
     * after construction, the position of the iterator is undefined.
     */
    RWSlistCollectablesConstIterator(const RWSlistCollectables& s)
#if defined(__HP_aCC) && __HP_aCC >= 53600 && __HP_aCC <= 55000
    ;
#else
        :
        RWSlistConstIterator(const_cast<RWSlistCollectables&>(s)) { }
#endif

    /**
     * Constructs a \c const iterator from an iterator.  Immediately
     * after construction, the position of the \c const iterator is undefined.
     */
    RWSlistCollectablesConstIterator(const RWSlistCollectablesIterator& it)
#if defined(__HP_aCC) && __HP_aCC >= 53600 && __HP_aCC <= 55000
    ;
#else
        :
        RWSlistConstIterator(it) { }
#endif

    virtual ~RWSlistCollectablesConstIterator() {}

    /**
     * Returns \c true if the iterator is at the beginning of the list,
     * otherwise returns \c false.
     */
    bool atFirst() const {
        return RWSlistConstIterator::atFirst();
    }

    /**
     * Returns \c true if the iterator is at the end of the list,
     * otherwise returns \c false.
     */
    bool atLast() const {
        return RWSlistConstIterator::atLast();
    }

    /**
     * Moves the iterator to the beginning of the list.
     */
    void toFirst() {
        RWSlistConstIterator::toFirst();
    }

    /**
     * Moves the iterator to the end of the list.
     */
    void toLast() {
        RWSlistConstIterator::toLast();
    }

    /**
     * Advances the iterator one item.
     */
    const void* operator++() {
        return RWSlistConstIterator::operator++();
    }

    /**
     * Advances the iterator \a n items.
     */
    const void* operator+=(size_t n) {
        return RWSlistConstIterator::operator+=(n);
    }

    /**
     * Moves iterator to the next item that isEqual() to
     * the item pointed to by \a target and returns it.  If no item
     * is found, returns #rwnil and the position of the iterator
     * is undefined.
     */
    virtual const RWCollectable*  findNext(const RWCollectable* target) {
        return static_cast<const RWCollectable*>(
                   RWSlistConstIterator::findNext(rwIsEqualFun, target));
    }

    // Documented in base class.
    virtual const RWCollectable*  key() const {
        return static_cast<const RWCollectable*>(RWSlistConstIterator::key());
    }

    // Documented in base class.
    virtual const RWCollectable*  operator()() {
        return static_cast<const RWCollectable*>(
                   RWSlistConstIterator::operator()());
    }

    /**
     * Resets the iterator, after which the position of the
     * iterator is undefined.
     */
    virtual void reset() {
        RWSlistConstIterator::reset();
    }

    virtual void reset(RWSlistCollectables& sl) {
        RWSlistConstIterator::reset(static_cast<RWSlist&>(sl));
    }

    virtual const RWSlistCollectables*  container() const {
        return static_cast<const RWSlistCollectables*>(
                   RWSlistConstIterator::container());
    }

    /**
     * Moves iterator to the next item which <i>is identical to</i>
     * the item pointed to by \a e (that is, that has address \a e)
     * and returns it.  If no item is found, returns #rwnil and the
     * position of the iterator is undefined.
     */
    const RWCollectable* findNextReference(const RWCollectable* e) {
        return static_cast<const RWCollectable*>(
                   RWSlistConstIterator::findNextReference(e));
    }
};

#endif  // RW_TOOLS_SLISTCOL_H 
