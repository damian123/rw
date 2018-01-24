#ifndef  RW_TOOLS_HASHTAB_H
#define  RW_TOOLS_HASHTAB_H

/**********************************************************************
 *
 * Declarations for RWHashTable --- hash table lookup.
 * Hash table look up with chaining
 * Duplicates are kept as distinct entries.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/hashtab.h#1 $
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

#include <vector>
#include <rw/defs.h>
#include <rw/colclass.h>
#include <rw/iterator.h>
#include <rw/slistcol.h>

class RW_TOOLS_GLOBAL RWHashTableIterator;

// Declare a vector of pointers to RWSlistCollectables:
typedef RWSlistCollectables* RWSlistCollectablesP;

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

/**
 * @ingroup rwcollectable_classes
 *
 * @brief A simple hash table for objects inheriting from
 * RWCollectable. Uses chaining (as implemented by class RWSlistCollectables)
 * to resolve hash collisions
 *
 * This class is a simple hash table for objects inheriting from
 * RWCollectable. It uses chaining (as implemented by class
 * RWSlistCollectables) to resolve hash collisions. Duplicate objects
 * are allowed. An object stored by RWHashTable must inherit from the
 * abstract base class RWCollectable, with suitable definition for virtual
 * functions \link RWCollectable::hash() hash()\endlink and
 * \link RWCollectable::isEqual() isEqual()\endlink.
 *
 * To find an object that matches a key, the key's virtual function
 * \link RWCollectable::hash() hash()\endlink is first called to
 * determine in which bucket the object
 * occurs. The bucket is then searched linearly by calling the virtual
 * function \link RWCollectable::isEqual() isEqual()\endlink
 * for each candidate, with the key as the argument.
 *  The first object to return \c true is the returned object.
 *
 * The initial number of buckets in the table is set by the constructor.
 *  There is a default value. If the number of items in the collection
 * greatly exceeds the number of buckets then efficiency sags
 * because each bucket must be searched linearly. The number of
 * buckets can be changed by calling member function resize().
 * This requires that all objects be rehashed.
 *
 * The iterator for this class is RWHashTableIterator.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/hashtab.h>
 * RWHashTable h ;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 *
 * @section example Example
 *
 * @code
 * #include <rw/hashtab.h>
 * #include <rw/tools/ctdatetime.h>
 *
 * int main ()
 * {
 *     RWHashTable table;
 *
 *     RWCollectableDateTime *july = new RWCollectableDateTime ("July 7, 1990", RWDateTime::setDate);
 *     RWCollectableDateTime *may  = new RWCollectableDateTime ("May 1, 1977", RWDateTime::setDate);
 *     RWCollectableDateTime *feb  = new RWCollectableDateTime ("Feb 22, 1983", RWDateTime::setDate);
 *     RWCollectableDateTime *aug  = new RWCollectableDateTime ("Aug 2, 1966", RWDateTime::setDate);
 *
 *     table.insert(july);
 *     table.insert(may);
 *     table.insert(feb);
 *     table.insert(aug);
 *
 *     std::cout << "Table contains " << table.entries() << " entries ";
 *
 *     RWCollectableDateTime key("Feb 22, 1983", RWDateTime::setDate);
 *     std::cout << "and it does"
 *               << (table.contains(&key) ? " " : " not ")
 *               << "contain the key " << key << std::endl;
 *
 *     delete july;
 *     delete may;
 *     delete feb;
 *     delete aug;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * Table contains 4 entries and it does contain the key Tue Feb 22 00:00:00 1983
 * @endcode
 */
class RW_TOOLS_GLOBAL RWHashTable : public RWCollection
{
    friend class RW_TOOLS_GLOBAL RWHashTableIterator;
    friend class RW_TOOLS_GLOBAL RWHashTableConstIterator;

private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWHashTable);
    friend RWCollectable* rwCreateFN(RWHashTable)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns \c __RWHASHTABLE.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    // Documented in base class.
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWHashTable*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWHashTable*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWHashTable& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWHashTable& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWHashTable& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWHashTable& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an empty hash table with \a N buckets.
     */
    RWHashTable(size_t N = RWCollection::DEFAULT_CAPACITY);

    /**
     * Copy constructor. Creates a new hash table as a shallow copy
     * of the table \a t. The new table has the same number of
     * buckets as the old table. Hence, the members need not be and
     * will not be rehashed.
     */
    RWHashTable(const RWHashTable& t);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWHashTable takes ownership of the
     * data owned by \a t.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWHashTable(RWHashTable && t);
#endif // !RW_NO_RVALUE_REFERENCES

    virtual ~RWHashTable();

    /**
     * Assignment operator. Sets self as a shallow copy of \a t. Afterwards,
     * the two tables have the same number of buckets. Hence,
     * the members need not be and will not be rehashed.
     */
    RWHashTable& operator= (const RWHashTable& t);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a t.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWHashTable& operator=(RWHashTable && t);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns \c true if self is a subset of \a t, that is, every
     * element of self has a counterpart in \a t which
     * \link RWCollectable::isEqual() isEqual()\endlink.
     */
    bool    operator<= (const RWHashTable& t) const;

    /**
     * Returns \c true if self and \a t have the same number of elements
     * and if for every key in self there is a corresponding key in
     * \a t which \link RWCollectable::isEqual() isEqual()\endlink.
     */
    bool    operator== (const RWHashTable& t) const;

    /**
     * Returns the negation of operator==().
     */
    bool    operator!= (const RWHashTable&) const;

    /**
     * Invokes the function pointer \a ap on each item in the collection in no
     * particular order, due to the nature of hashing collections. The function
     * pointed to by \a ap should take no action that could change the hash value
     * or equality properties of the objects.
     */
    virtual void apply(RWapplyCollectable ap, void*);

    // Documented in base class.
    virtual void clear();

    // Documented in base class.
    virtual size_t entries() const {
        return nitems_;
    }

    // Documented in base class.
    virtual RWCollectable* find(const RWCollectable*) const;

    /**
     * Returns \a a if successful, otherwise #rwnil.
     */
    virtual RWCollectable* insert(RWCollectable* a);

    // Documented in base class.
    virtual bool isEmpty() const {
        return nitems_ == 0;
    }

    // Documented in base class.
    virtual bool isEqual(const RWCollectable*) const;

    // Documented in base class.
    virtual size_t occurrencesOf(const RWCollectable*) const;

    // Documented in base class.
    virtual RWCollectable* remove(const RWCollectable*);

    // Documented in base class.
    virtual RWIterator* newIterator();

    // Documented in base class.
    virtual RWConstIterator* newConstIterator() const;

    /**
     * Resizes the internal hash table to have \a n buckets. This causes
     * rehashing all the members of the collection. If \a n is zero,
     * resizes to <tt>3*</tt>entries()<tt>/2</tt>.
     */
    virtual void resize(size_t n = 0);

    size_t buckets() const {
        return table_.size();
    }

    /**
     * Swaps the data owned by self with the data owned by \a t.
     */
    void swap(RWHashTable& t);

protected:

    // Table of pointers to chains.
    // We can't use RWTPtrVector here as it will cause a circular dependency
    // because this class is used in the implementation of persistence for
    // RWCollectable, and template container classes.
    std::vector<RWSlistCollectablesP> table_;

    // Total number of stored objects.
    size_t nitems_;

protected:

    size_t hashIndex(const RWCollectable* p) const {
        return (p->hash() % buckets());
    }

    RWCollectable* insertIndex(size_t, RWCollectable*);
};

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Allows sequential access to all the elements of RWHashTable
 *
 * Iterator for class RWHashTable, which allows sequential access
 * to all the elements of RWHashTable.
 *
 * @note
 * An RWHashTable is unordered, elements are not accessed in any
 * particular order.
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
 * #include <rw/hashtab.h>
 * RWHashTable h;
 * RWHashTableIterator it(h);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWHashTableIterator : public RWIterator
{
    friend class RWHashTableConstIterator;

public:

    /**
     * Constructs an iterator for an RWHashTable. After construction,
     * the position of the iterator is undefined.
     */
    RWHashTableIterator(RWHashTable& h);

    RWHashTableIterator(const RWHashTableIterator&);

    virtual ~RWHashTableIterator();

    RWHashTableIterator& operator=(const RWHashTableIterator&);

    /**
     * Moves the iterator to the next
     * item which \link RWCollectable::isEqual() isEqual()\endlink to the
     * item pointed to by \a target and returns it.
     */
    virtual RWCollectable* findNext(const RWCollectable* target);

    // Documented in base class.
    virtual RWCollectable* key() const;

    // Documented in base class.
    virtual RWCollectable* operator()();

    // Documented in base class.
    virtual void reset();

    /**
     * Removes the item at the current iterator position from the collection.
     */
    RWCollectable* remove();

    /**
     * Moves the iterator to the next item which
     * \link RWCollectable::isEqual() isEqual()\endlink to
     * the item pointed to by \a target, removes it from the collection and
     * returns it. If no item is found, returns #rwnil and the position of
     * the iterator is undefined.
     */
    RWCollectable* removeNext(const RWCollectable* target);

protected:

    void nextIterator();

private:

    RWHashTable* myHash_;

    // Which Slist is current
    size_t idx_;

    // An iterator for that Slist
    RWTScopedPointer<RWSlistCollectablesIterator> iterator_;

};

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Allows sequential access to all the elements of RWHashTable and, as
 * a \c const iterator, it does not change anything in the underlying
 * data structure
 *
 * One of two iterators for class RWHashTable. As a \c const iterator,
 * it does not change anything in the underlying data structure.
 *
 * This iterator allows sequential access to all the elements of
 * RWHashTable.
 *
 * @note
 * An RWHashTable is unordered, elements are not accessed in any
 * particular order.
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
 * #include <rw/hashtab.h>
 * RWHashTable h;
 * RWHashTableConstIterator it(h);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWHashTableConstIterator : public RWConstIterator
{
public:

    /**
     * Constructs a \c const iterator for an RWHashTable. After construction,
     * the position of the iterator is undefined.
     */
    RWHashTableConstIterator(const RWHashTable& h);

    /**
     * Copy constructor. After construction, the position of the iterator is undefined.
     */
    RWHashTableConstIterator(const RWHashTableConstIterator&);

    /**
     * Constructs a \c const iterator from an iterator. After
     * construction, the position of the \c const iterator is undefined.
     */
    RWHashTableConstIterator(const RWHashTableIterator& it)
        : myHash_(it.myHash_), idx_(it.idx_), iterator_(it.iterator_.get()) { }

    virtual ~RWHashTableConstIterator();

    RWHashTableConstIterator& operator=(const RWHashTableConstIterator&);

    /**
     * Moves the iterator to the
     * next item which \link RWCollectable::isEqual() isEqual()\endlink
     * to the item pointed to by \a target and returns it.
     */
    virtual const RWCollectable* findNext(const RWCollectable* target);

    // Documented in base class.
    virtual const RWCollectable* key() const;

    // Documented in base class.
    virtual const RWCollectable* operator()();

    // Documented in base class.
    virtual void reset();

    virtual void reset(RWHashTable& h) {
        myHash_ = &h;
        idx_ = RW_NPOS;
        iterator_.reset(rwnil);
        reset();
    }

    virtual const RWHashTable* container() const {
        return const_cast<const RWHashTable*>(myHash_);
    }

    /******************* Special iterator functions ************************/

protected:

    void nextIterator();

private:

    RWHashTable* myHash_;

    // Which Slist is current
    size_t idx_;

    // An iterator for Slist
    RWTScopedPointer<RWSlistCollectablesIterator> iterator_;
};

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif /* RW_TOOLS_HASHTAB_H */
