#ifndef  RW_TOOLS_SET_H
#define  RW_TOOLS_SET_H

/**********************************************************************
 *
 * Declarations for RWSet --- hash table lookup.
 * Duplicates are not allowed.
 * Hash table look up with chaining: derived from RWHashTable
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rwset.h#1 $
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
#include <rw/hashtab.h>
#include <rw/iterator.h>
#include <rw/edefs.h>

class RW_TOOLS_GLOBAL RWSetIterator;

/**
 * @ingroup rwcollectable_classes
 * @brief  A group of unordered elements, not accessible
 * by an external key, where duplicates are not allowed.
 *
 * Class RWSet represents a group of unordered elements, not accessible
 * by an external key, where duplicates are not allowed.  It corresponds
 * to the Smalltalk class \b \e Set.
 *
 * An object stored by RWSet must inherit from abstract base class
 * RWCollectable, with suitable definition for virtual functions
 * RWCollectable::hash() and RWCollectable::isEqual().
 * Use the function \link RWCollectable::hash() hash() \endlink
 * to find objects with the same hash value, and then use
 * \link RWCollectable::isEqual() isEqual() \endlink
 * to confirm the match.
 *
 * An item \c c is considered to be "already in the collection"
 * if there is a member of the collection with the same hash value
 * as \c c for which \link RWCollectable::isEqual() isEqual(c) \endlink
 * returns \c true.
 * In this case, method \link insert() insert(c) \endlink will not add it,
 * thus insuring that there are no duplicates.
 *
 * The iterator for this class is RWSetIterator.
 *
 * @section synopsis Synopsis
 *
 * @code
 * typedef RWSet Set;   // Smalltalk typedef.
 * #include <rw/rwset.h>
 * RWSet h ;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWSet : public RWHashTable
{

    friend class RW_TOOLS_GLOBAL RWSetIterator;
    friend class RW_TOOLS_GLOBAL RWSetConstIterator;

private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWSet);
    friend RWCollectable* rwCreateFN(RWSet)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns \c __RWSET, i.e. a <i>class identifier</i>,
     * that identifies this object's class.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    // Documented in base class.
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWSet*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWSet*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWSet& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWSet& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWSet& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWSet& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an empty set with \a n hashing buckets.
     */
    RWSet(size_t n = RWCollection::DEFAULT_CAPACITY);

    /**
     * Copy constructor.  Makes a shallow copy of the collection
     * \a h.
     */
    RWSet(const RWSet& h) :  RWHashTable(h) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWSet takes ownership of the
     * data owned by \a h.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWSet(RWSet && h) : RWHashTable(rw_move(h)) { }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Calls clear(), inherited from RWHashTable.
     */
    virtual ~RWSet();

    /**
     * Assignment operator.  Makes a shallow copy of the collection
     * \a h.
     */
    RWSet& operator=(const RWSet& h) {
        RWHashTable::operator=(h);
        return *this;
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a h.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWSet& operator=(RWSet && h) {
        RWHashTable::operator=(rw_move(h));
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns \c true  if self is a subset of \a h, that is, every
     * element of self has a counterpart in \a h which isEqual().
     */
    bool operator<=(const RWSet& h) const;

    // Documented in base class.
    bool operator<=(const RWHashTable& t) const;

    /**
     * Returns \c true  if self is a proper subset of \a h, that is,
     * every element of self has a counterpart in \a h which isEqual(),
     * but where the two sets are not identical.
     */
    bool operator< (const RWSet& h) const;

    /**
     * Sets self to be the intersection of self and \a h.  Returns self.
     */
    RWSet&    operator*=(const RWSet& h);

    /**
     * Computes the intersection of self and \a h, and inserts the result
     * into \a ret (which may be either empty or not, depending on the
     * effect desired). This function may be slightly more efficient
     * than operator*=().
     */
    void  intersectWith(const RWSet& h, RWSet& ret) const;

    // Documented in base class.
    virtual void clearAndDestroy();

    /**
     * Adds \a c to the collection and returns it.  If an item is already
     * in the collection which isEqual() to \a c, then the existing item
     * is returned and the new item is not inserted.
     */
    virtual RWCollectable* insert(RWCollectable* c);

    // Documented in base class.
    virtual bool isEqual(const RWCollectable*) const;

    /**
     * Returns the count of entries that compare isEqual() to
     * the item pointed to by \a target. Because duplicates
     * are not allowed for this collection, only 0 or 1 can be returned.
     *
     * @note If self is a collection of key-value pairs
     * (e.g. RWHashDictionary), this method compares \a target with the keys
     * in the collection.
     */
    virtual size_t occurrencesOf(const RWCollectable* target) const;

    // Documented in base class.
    virtual RWIterator* newIterator();

    // Documented in base class.
    virtual RWConstIterator* newConstIterator() const;

};

/**
 * @ingroup rwcollectable_classes
 * @brief Iterator for class RWSet, allowing sequential access to all
 * the elements of RWSet.
 *
 * Iterator for class RWSet, which allows sequential access to all
 * the elements of RWSet.
 *
 * @note An RWSet is unordered; elements are not accessed in any
 * particular order.
 *
 * The "current item" is undefined immediately after construction.
 * You must define it by using operator()() or some other (valid)
 * operation.
 *
 * Once the iterator has advanced beyond the end of the collection,
 * it is no longer valid.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rwset.h>
 * RWSet h;
 * RWSetIterator it(h) ;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWSetIterator : public RWHashTableIterator
{
    RW_DECLARE_HOME(RWSetIterator)

public:

    /**
     * Constructs an iterator for an RWSet.  After construction, the
     * position of the iterator is undefined.
     */
    RWSetIterator(RWSet& h) : RWHashTableIterator(h) { }

    // Documented in base class.
    void reset() {
        RWHashTableIterator::reset();
    }
};


/**
 * @ingroup rwcollectable_classes
 * @brief Const iterator for class RWSet, to allow sequential
 * access to all elements of RWSet.
 *
 * One of two iterators for class RWSet.  This \c const iterator
 * allows sequential access to all the elements of RWSet and prohibits
 * any changes to the contents of the data structure.
 *
 * @note An RWSet is unordered; elements are not accessed in any
 * particular order.
 *
 * The "current item" is undefined immediately after construction.
 * You must define it by using operator()() or some other (valid)
 * operation.
 *
 * Once the iterator has advanced beyond the end of the collection,
 * it is no longer valid.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rwset.h>
 * RWSet h;
 * RWSetConstIterator it(h);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWSetConstIterator
    : public RWHashTableConstIterator
{
    RW_DECLARE_HOME(RWSetConstIterator)

public:

    /**
     * Constructs a \c const iterator for an RWSet. After construction, the
     * position of the iterator is undefined.
     */
    RWSetConstIterator(const RWSet& h) : RWHashTableConstIterator(h) { }

    /**
     * Constructs a \c const iterator from an iterator. After construction, the
     * position of the \c const iterator is undefined.
     */
    RWSetConstIterator(const RWSetIterator& it)
        : RWHashTableConstIterator(it) { }

    // Documented in base class.
    void reset() {
        RWHashTableConstIterator::reset();
    }

    void reset(RWHashTable& h) {
        RWHashTableConstIterator::reset(h);
    }
};

#endif /* RW_TOOLS_SET_H */
