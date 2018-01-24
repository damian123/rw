#ifndef RW_TOOLS_COLCLASS_H
#define RW_TOOLS_COLCLASS_H


/**********************************************************************
 *
 * Base class for the Smalltalk(TM)-like collection classes.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/colclass.h#1 $
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
#include <rw/collect.h>
#include <rw/iterator.h>

class RW_TOOLS_GLOBAL RWBag;
class RW_TOOLS_GLOBAL RWOrdered;
class RW_TOOLS_GLOBAL RWSet;
class RW_TOOLS_GLOBAL RWBinaryTree;

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Contains virtual functions for inserting and retrieving pointers
 * to RWCollectable objects into the collection classes
 *
 * Class RWCollection is an abstract base class for the Smalltalk-like
 * collection classes. The class contains virtual functions for
 * inserting and retrieving pointers to RWCollectable objects into
 * the collection classes. Virtual functions are also provided for
 * storing and reading the collections to files and streams. Collections
 * that inherit this base class typically redefine one or more
 * of these functions.
 *
 * Several of the methods of this class are abstract. These functions
 * must be defined in derived classes. For these functions the description
 * is intended to be generic  --  all inheriting collection classes
 * generally follow the described pattern. Exceptions are noted in the
 * documentation for the particular class.
 *
 * For many other functions, a suitable definition is provided by
 * RWCollection and a deriving class may not need to redefine the
 * function. Examples are contains() or restoreGuts().
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/colclass.h>
 * typedef RWCollection Collection;   // Smalltalk typedef
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWCollection : public RWCollectable
{
public:
    virtual void* self();
    virtual const void* self() const;

    friend RW_TOOLS_GLOBAL RWvistream&
    operator>>(RWvistream&, RWCollection*&);

    friend RW_TOOLS_GLOBAL RWFile&
    operator>>(RWFile&, RWCollection*&);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWCollection& Cl) {
        Cl.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWCollection& Cl) {
        Cl.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWCollection& Cl) {
        RWCollectable::recursiveRestoreFrom(v, &Cl);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWCollection& Cl) {
        RWCollectable::recursiveRestoreFrom(f, &Cl);
        return f;
    }

public:

    /**
     * The default number of buckets in a RWCollection class.
     */
    static size_t  DEFAULT_CAPACITY;
    static size_t  DEFAULT_RESIZE;

public:

    /**
     * Empty destructor.
     */
    virtual ~RWCollection();

    // Documented in base class.
    virtual RWspace binaryStoreSize() const;

    /**
     * Returns \c __RWCOLLECTION
     */
    virtual RWClassID isA() const {
        return __RWCOLLECTION;
    }

    /**
     * Repeatedly calls the global operator:
     *
     * @code
     * RWvistream& operator>>(RWvistream&, RWCollectable*&);
     * @endcode
     *
     * followed by insert(RWCollectable*) for each item in the collection.
     */
    virtual void restoreGuts(RWvistream&);

    /**
     * Repeatedly calls the global operator:
     *
     * @code
     * RWFile&  operator>>(RWFile&, RWCollectable*&);
     * @endcode
     *
     * followed by insert(RWCollectable*) for each item in the collection.
     */
    virtual void restoreGuts(RWFile&);

    /**
     * Calls the global operator.
     *
     * @code
     * RWvostream& operator<<(RWvostream&, const RWCollectable&);
     * @endcode
     *
     * for each object in the collection.
     */
    virtual void saveGuts(RWvostream&) const;

    /**
     * Calls the global operator:
     *
     * @code
     * RWFile& operator<<(RWFile&, const RWCollectable&);
     * @endcode
     *
     * for each object in the collection.
     */
    virtual void saveGuts(RWFile&) const;

    /**
     * This function applies the user-supplied function pointed to by
     * \a ap to each member of the collection. This function should
     * have the following prototype:
     *
     * @code
     * void yourApplyFunction(RWCollectable* ctp, void* vp);
     * @endcode
     *
     * The function \c yourApplyFunction() can perform any operation on
     * the item at address \c ctp that <i>does not change</i> the hash
     * value or sorting order of the item. Client data may be passed
     * to this function through the second argument \a vp.
     */
    virtual void apply(RWapplyCollectable ap, void* vp) = 0;

    /**
     * Removes all objects from the collection. Does not delete the
     * objects themselves.
     */
    virtual void clear() = 0;

    /**
     * Removes all objects from the collection <i>and deletes</i> them.
     * Takes into account duplicate objects within a collection and
     * only deletes them once. However, it does <i>not</i> take into
     * account objects shared between different collections. Either
     * do not use this function if you are sharing objects between
     * separate collections, or put all collections that could be sharing
     * objects into one single "super-collection" and call clearAndDestroy()
     * on that.
     */
    virtual void clearAndDestroy();

    /**
     * Returns \c true if the collection contains an item where the
     * virtual function find() returns non-nil.
     */
    virtual bool contains(const RWCollectable* target) const;

    /**
     * Returns the total number of items in the collection.
     */
    virtual size_t entries() const = 0;

    /**
     * Returns a pointer to the first item in the collection which "matches"
     * the object pointed to by \a target or #rwnil if no item was found.
     * For most collections, an item "matches" the target if either
     * \link RWCollectable::isEqual() isEqual()\endlink or compareTo() find
     * equivalence, whichever is appropriate
     * for the actual collection type. However, the "identity collections"
     * (i.e., RWIdentitySet and RWIdentityDictionary) look for an item
     * with the same address (i.e., "is identical to").
     */
    virtual RWCollectable* find(const RWCollectable* target) const = 0;

    /**
     * Adds an item to the collection and returns a pointer to it. If
     * the item is already in the collection, some collections derived
     * from RWCollection return the old instance, others return #rwnil.
     */
    virtual RWCollectable* insert(RWCollectable* e) = 0;

    /**
     * Returns \c true if the collection is empty, otherwise returns
     * \c false.
     */
    virtual bool isEmpty() const = 0;

    /**
     * Returns the number of items in the collection which are "matches"
     * for \a t. See function find() for a definition of matches.
     */
    virtual size_t occurrencesOf(const RWCollectable* t) const = 0;

    /**
     * Removes and returns a pointer to the first item in the collection
     * which "matches" the object pointed to by \a target. Returns \c
     * nil if no object was found. Does not delete the object.
     * See function find() for a definition of matches.
     */
    virtual RWCollectable* remove(const RWCollectable* target) = 0;

    /**
     * Removes <i>and deletes</i> the first item in the collection which
     * "matches" the object pointed to by \a target.
     */
    virtual void removeAndDestroy(const RWCollectable* target);

    /**
     * Returns a dynamically allocated iterator for the collection.
     */
    virtual RWIterator* newIterator() = 0;

    /**
     * Returns a const pointer to a dynamically allocated iterator for
     * the collection.
     */
    virtual RWConstIterator* newConstIterator() const = 0;

    /**
     * Adds each item in \a c to self. Calling operator+=() on a
     * RWBinaryTree where \a c is a sorted collection can cause that
     * tree to become unbalanced, possibly to the point of stack overflow.
     */
    void operator+=(const RWCollection& c);

    /**
     * Removes each item in \a c from self.
     */
    void operator-=(const RWCollection& c);

    void addContentsTo(RWCollection*) const;
    void copyContentsTo(RWCollection*) const;

    /**
     * Allows any collection to be converted to an RWBag. The return
     * value is a \e copy of the data. This can be very expensive
     * for large collections. You should consider using operator+=()
     * to insert each item from this collection into a collection of
     * your choice.
     *
     * @note
     * If self contains pairs of keys and values, the result of
     * this call is a container holding objects of type
     * RWCollectableAssociation.
     */
    RWBag         asBag() const;

    /**
     * Allows any collection to be converted to an RWOrdered. The return
     * value is a \e copy of the data. This can be very expensive
     * for large collections. You should consider using operator+=()
     * to insert each item from this collection into a collection of
     * your choice.
     *
     * @note
     * If self contains pairs of keys and values, the result of
     * this call is a container holding objects of type
     * RWCollectableAssociation.
     */
    RWOrdered     asOrderedCollection() const;

    /**
     * Allows any collection to be converted to an RWSet. The return
     * value is a \e copy of the data. This can be very expensive
     * for large collections. You should consider using operator+=()
     * to insert each item from this collection into a collection of
     * your choice.
     *
     * @note
     * If self contains pairs of keys and values, the result of
     * this call is a container holding objects of type
     * RWCollectableAssociation.
     */
    RWSet         asSet() const;

    /**
     * Allows any collection to be converted to an RWBinaryTree. The
     * return value is a \e copy of the data. This can be very
     * expensive for large collections. You should consider using
     * operator+=() to insert each item from this collection into a
     * collection of your choice.
     *
     * @note
     * Converting a collection containing data which is already
     * sorted to an RWBinaryTree with this method may build a very
     * unbalanced tree.
     *
     * @note
     * If self contains pairs of keys and values, the result of
     * this call is a container holding objects of type
     * RWCollectableAssociation.
     */
    RWBinaryTree  asSortedCollection() const;

    /**
     * @copydoc RWCollection::asSortedCollection() const
     */
    RWBinaryTree  asBinaryTree() const;

    /**
     * Evaluates the function pointed to by \a tst for each item in
     * the collection. It inserts those items for which the function
     * returns \c true into a new collection allocated off the heap
     * of the same type as self and returns a pointer to this new collection.
     * Because the new collection is allocated <i>off the heap</i>,
     * you are responsible for deleting it when done. This is not a
     * virtual function.
     */
    RWCollection* select(RWtestCollectable tst, void* vp) const;

#ifndef RW_BAD_STATIC_ACCESS
protected:
#else
    // certain compilers cannot gain access to the protected static functions
    // so we must provide public methods
public:
#endif /* RW_NO_ACCESS_TO_PROTECTED */
    static void saveObjToStream(RWCollectable*, void*);
    static void saveObjToRWFile(RWCollectable*, void*);
};

typedef void (*RWapplyKeyAndValue)(RWCollectable*, RWCollectable*, void*);

#endif /* RW_TOOLS_COLCLASS_H */
