#ifndef RW_TOOLS_BAG_H
#define RW_TOOLS_BAG_H

/**********************************************************************
 *
 * Declarations for RWBag --- an unordered collection.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rwbag.h#1 $
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
#include <rw/hashdict.h>

class RW_TOOLS_GLOBAL RWBagIterator;
class RW_TOOLS_GLOBAL RWBagConstIterator;

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Corresponds to the Smalltalk class Bag, representing a group
 * of unordered elements not accessible by an external key.
 *
 * Class RWBag corresponds to the Smalltalk class Bag. It represents
 * a group of unordered elements, not accessible by an external
 * key. Duplicates are allowed.
 *
 * An object stored by RWBag must inherit abstract base class RWCollectable,
 * with suitable definition for virtual functions RWCollectable::hash()
 * and RWCollectable::isEqual(). The function hash() is used to find
 * objects with the same hash value, then isEqual() is used to confirm
 * the match.
 *
 * Class RWBag is implemented by using an internal hashed dictionary
 * (RWHashDictionary) which keeps track of the number of occurrences
 * of an item. If an item is added to the collection that compares
 * equal (isEqual()) to an existing item in the collection, then
 * the count is incremented. Note that this means that only the
 * first instance of a value is actually inserted: subsequent instances
 * cause the occurrence count to be incremented. This behavior parallels
 * the Smalltalk implementation of Bag.
 *
 * Member function apply() and the iterator are called repeatedly
 * according to the count for an item.
 *
 * See class RWHashTable if you want duplicates to be stored, rather
 * than merely counted.
 *
 * @section synopsis Synopsis
 *
 * @code
 * typedef RWBag Bag;     // Smalltalk typedef .
 * #include <rw/rwbag.h>
 * RWBag h;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWBag : public RWCollection
{
    friend class RW_TOOLS_GLOBAL RWBagIterator;
    friend class RW_TOOLS_GLOBAL RWBagConstIterator;

private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWBag);
    friend RWCollectable* rwCreateFN(RWBag)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns the unique id for RWBag.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWBag*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWBag*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWBag& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWBag& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWBag& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWBag& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Construct an empty bag with \a n buckets.
     */
    RWBag(size_t n = RWCollection::DEFAULT_CAPACITY);

    /**
     * Copy constructor. Makes a shallow copy of \a b.
     */
    RWBag(const RWBag& b);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWBag takes ownership of the
     * data owned by \a b.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWBag(RWBag && b);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Calls the clear() method.
     */
    virtual ~RWBag();

    /**
     * Assignment operator. Makes a shallow copy of \a b.
     */
    RWBag& operator=(const RWBag& b);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a b.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWBag& operator=(RWBag && b);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns \c true if self and bag \a b have the same number of
     * total entries and if for every key in self there is a corresponding
     * key in \a b which isEqual() and which has the same number of
     * entries.
     */
    bool operator== (const RWBag& b) const;

    /**
     * Invokes the function pointer \a ap on each item in the collection in a
     * generally unpredictable order. If an item has been inserted more than
     * once (i.e., more than one item isEqual()), then apply() will be called
     * that many times. The function pointed to by \a ap should take no action
     * that could change the hash value or the meaning of isEqual() for the
     * items.
     */
    virtual void apply(RWapplyCollectable ap, void*);

    // Documented in base class.
    virtual void clear();

    // Documented in base class.
    virtual void clearAndDestroy();

    // Documented in base class.
    virtual size_t entries() const {
        return totalEntries;
    }

    /**
     * The first item that was inserted
     * into the \c Bag and which equals \a target is returned, or
     * #rwnil if no item is found. Hashing is used to narrow the search.
     */
    virtual RWCollectable* find(const RWCollectable* target) const;

    /**
     * Inserts the item \a c into
     * the collection and returns it, or if an item was already in the
     * collection that isEqual() to \a c, returns the old item
     * and increments its count.
     */
    virtual RWCollectable* insert(RWCollectable* c);

    // Documented in base class.
    virtual bool isEmpty() const {
        return totalEntries == 0;
    }

    // Documented in base class.
    virtual bool isEqual(const RWCollectable* a) const;

    /**
     * Returns the number of items
     * that <i>are equal to</i> the item pointed to by \a target.
     */
    virtual size_t occurrencesOf(const RWCollectable* target) const;

    /**
     * Removes and returns the item
     * that isEqual() to the item pointed to by \a target. Returns
     * #rwnil if no item was found.
     */
    virtual RWCollectable* remove(const RWCollectable* target);

    /**
     * Removes the item that isEqual()
     * to the item pointed to by \a target. Destroys the item as well
     * if it is the last occurrence in the collection.
     */
    virtual void removeAndDestroy(const RWCollectable* target);

    // Documented in base class.
    virtual RWIterator* newIterator();

    // Documented in base class.
    virtual RWConstIterator* newConstIterator() const;

    /**
     * Inserts the item \a c into the collection with count \a n and
     * returns it, or if an item was already in the collection that
     * isEqual() to \a c, then returns the old item and increments
     * its count by \a n.
     */
    RWCollectable* insertWithOccurrences(RWCollectable* c, int n);

    /**
     * Resizes the internal hash table to have \a n buckets. The overhead
     * for this function is the hashing of every element in the collection.
     * If \a n is zero, then an appropriate size is picked automatically.
     */
    void resize(size_t n = 0);

    /**
     * Returns the current number of buckets allocated to the internal
     * hash table.
     */
    size_t buckets() const {
        return contents.buckets();
    }

    /**
     * Swaps the data owned by self with the data owned by \a b.
     */
    void swap(RWBag& b);

private:

    size_t totalEntries;

    // Dictionary of number of occurrences.
    RWHashDictionary contents;

    void deepenTally();
};

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Iterator for class RWBag, which allows sequential access to all
 * the elements of RWBag.
 *
 * Iterator for class RWBag, which allows sequential access to all
 * the elements of RWBag. Note that because an RWBag is unordered,
 * elements are not accessed in any particular order. If an item
 * was inserted \c N times into the collection, then it will be
 * visited \c N consecutive times.
 *
 * Like all Rogue Wave iterators, the "current item" is undefined
 * immediately after construction -- you must define it by using
 * #operator()() or some other (valid) operation.
 *
 * Once the iterator has advanced beyond the end of the collection
 * it is no longer valid, so continuing to use it has undefined results.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rwbag.h>
 * RWBag b;
 * RWBagIterator it(b);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWBagIterator : public RWIterator
{
    friend class RWBagConstIterator;
public:

    /**
     * Construct an iterator for an RWBag. After construction, the position
     * of the iterator is undefined.
     */
    RWBagIterator(const RWBag&);

    virtual ~RWBagIterator() {}

    /**
     * Moves the iterator to the next item
     * that isEqual() to the object pointed to by \a target, and returns
     * it. Hashing is used to find the target. If no item is found,
     * returns #rwnil and the position of the iterator is undefined.
     */
    virtual RWCollectable* findNext(const RWCollectable* target);

    // Documented in base class.
    virtual RWCollectable* key() const;

    // Documented in base class.
    virtual RWCollectable* operator()();

    // Documented in base class.
    virtual void reset();

private:

    RWHashDictionaryIterator contentsIterator;

    int count;

    RWCollectable* currentItem;
};

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Iterator for class RWBag, which allows sequential access to all
 * the elements of RWBag but allows no changes.
 *
 * Provides an iterator for class RWBag that allows sequential access
 * to all the elements of RWBag and that does not change any member
 * of RWBag. Note that because an RWBag is unordered, elements
 * are not accessed in any particular order. If an item was inserted
 * \c N times into the collection, then it is visited \c N consecutive
 * times.
 *
 * Like all Rogue Wave iterators, the "current item" is undefined
 * immediately after construction -- you must define it by using
 * #operator()() or some other (valid) operation.
 *
 * Once the iterator has advanced beyond the end of the collection
 * it is no longer valid, so continuing to use it has undefined results.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rwbag.h>
 * RWBag b;
 * RWBagConstIterator it(b);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWBagConstIterator : public RWConstIterator
{
public:

    /**
     * Constructs a \c const iterator for an RWBag. After construction, the
     * position of the iterator is undefined.
     */
    RWBagConstIterator(const RWBag&);

    /**
     * Constructs a \c const iterator from an iterator. After construction,
     * the position of the \c const iterator is undefined.
     */
    RWBagConstIterator(const RWBagIterator& it)
        : contentsIterator(it.contentsIterator),
          count(it.count),
          currentItem(it.currentItem) { }

    virtual ~RWBagConstIterator() { }

    /**
     * Moves the iterator to the
     * next item that isEqual() to the object pointed to by \a target,
     * and returns it. Uses hashing to find the target. If no item
     * is found, returns #rwnil and the position of the iterator is
     * undefined.
     */
    virtual const RWCollectable* findNext(const RWCollectable* target);

    // Documented in base class.
    virtual const RWCollectable* key() const;

    // Documented in base class.
    virtual const RWCollectable* operator()();

    // Documented in base class.
    virtual void reset();

    virtual void reset(RWBag& b) {
        bag_ = &b;
        contentsIterator = b.contents;
        reset();
    }

    virtual const RWBag* container() const {
        return bag_;
    }

private:

    RWHashDictionaryIterator contentsIterator;

    int count;

    RWCollectable* currentItem;

    const RWBag* bag_;
};

#endif /* RW_TOOLS_BAG_H */
