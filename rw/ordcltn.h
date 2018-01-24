#ifndef RW_TOOLS_ORDCLTN_H
#define RW_TOOLS_ORDCLTN_H

/**********************************************************************
 *
 * RWOrdered --- Ordered Collection
 * The copy constructor and assignment operator use memberwise
 * initialization and assignment, respectively.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/ordcltn.h#1 $
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
#include <rw/seqcltn.h>
#include <rw/iterator.h>
#include <rw/tpvector.h>

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Represents a group of ordered items, accessible
 * by an index number, but not accessible by an external key
 *
 * Class RWOrdered represents a group of ordered items, accessible
 * by an index number, but not accessible by an external key. Duplicates
 * are allowed. The ordering of elements is determined externally,
 * generally by the order of insertion and removal. An object stored
 * by RWOrdered must inherit from the abstract base class RWCollectable.
 *
 * Class RWOrdered is implemented as a vector of pointers, allowing
 * for more efficient traversing of the collection than the linked
 * list classes RWSlistCollectables and RWDlistCollectables, but
 * with slower insertion in the center of the collection.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/ordcltn.h>
 * RWOrdered a;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWOrdered : public RWSequenceable
{

    friend class RW_TOOLS_GLOBAL RWOrderedIterator;
    friend class RW_TOOLS_GLOBAL RWOrderedConstIterator;

private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWOrdered);
    friend RWCollectable* rwCreateFN(RWOrdered)();

public:
    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns \c __RWORDERED.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    // Documented in base class.
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWOrdered*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWOrdered*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWOrdered& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWOrdered& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWOrdered& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWOrdered& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an RWOrdered with an initial capacity of \a size.
     */
    RWOrdered(size_t size = RWCollection::DEFAULT_CAPACITY);

    /**
     * Copy constructor. Makes a shallow copy of \a o.
     */
    RWOrdered(const RWOrdered& o);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWOrdered takes ownership of the
     * data owned by \a o.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWOrdered(RWOrdered && o);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Assignment operator. Makes a shallow copy of \a o.
     */
    RWOrdered& operator=(const RWOrdered& o);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a o.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWOrdered& operator=(RWOrdered && o);
#endif // !RW_NO_RVALUE_REFERENCES

    virtual ~RWOrdered();

    /**
     * Returns \c true if for every item in self, the corresponding
     * item in \a od at the same index
     * \link RWCollectable::isEqual() isEqual() \endlink. The two collections
     * must also have the same number of members.
     */
    bool operator==(const RWOrdered& od) const;

    // Documented in base class.
    virtual RWCollectable* append(RWCollectable* a);

    /**
     * Invokes the function pointer \a ap on each item in the collection, in
     * order, from first to last.
     */
    virtual void apply(RWapplyCollectable ap, void* x);

    // Documented in base class.
    virtual RWCollectable*& at(size_t i);

    // Documented in base class.
    virtual const RWCollectable* at(size_t i) const;

    // Documented in base class.
    virtual void clear();

    // Documented in base class.
    virtual size_t entries() const {
        return nitems;
    }

    /**
     * Returns the first item that
     * \link RWCollectable::isEqual() isEqual() \endlink to the item pointed
     * to by \a target, or #rwnil if no item was found.
     */
    virtual RWCollectable* find(const RWCollectable* target) const;

    // Documented in base class.
    virtual RWCollectable* first() const;

    // Documented in base class.
    virtual size_t index(const RWCollectable* c) const;

    /**
     * Adds the item to the end
     * of the collection and returns it.  Returns #rwnil if the insertion
     * was unsuccessful.
     */
    virtual RWCollectable* insert(RWCollectable* c);

    // Documented in base class.
    virtual RWCollectable* insertAt(size_t indx, RWCollectable* e);

    // Documented in base class.
    virtual bool isEmpty() const {
        return nitems == 0;
    }

    // Documented in base class.
    virtual bool isEqual(const RWCollectable* a) const;

    // Documented in base class.
    virtual RWCollectable* last() const;

    /**
     * Returns the number of items that compare
     * \link RWCollectable::isEqual() isEqual() \endlink to
     * the item pointed to by \a target.
     */
    virtual size_t occurrencesOf(const RWCollectable* target) const;

    /**
     * Removes the first item that
     * \link RWCollectable::isEqual() isEqual() \endlink to the item pointed
     * to by \a target and returns it. Returns #rwnil if no item was found.
     */
    virtual RWCollectable* remove(const RWCollectable* target);

    // Documented in base class.
    virtual RWCollectable* prepend(RWCollectable* c);

    // Documented in base class.
    virtual RWIterator* newIterator();

    // Documented in base class.
    virtual RWConstIterator* newConstIterator() const;

    /**
     * Returns the \a i <sup>th</sup> element in the collection. If \a i is out
     * of range, the function throws an exception of type RWBoundsErr. The
     * results of this function can be used as an lvalue.
     */
    RWCollectable*& operator[](size_t i);

    /**
     * Returns the \a i <sup>th</sup> element in the collection. Bounds checking
     * is enabled by defining the preprocessor directive \c RWBOUNDS_CHECK
     * before including the header file \c ordcltn.h. In this case,
     * if \a i is out of range, the function throws an exception of type
     * RWBoundsErr. The results of this function can be used as an lvalue.
     */
    RWCollectable*& operator()(size_t i);

    RWCollectable* operator[](size_t i) const;

    RWCollectable* operator()(size_t i) const;

    const RWCollectableP* data() const;

    /**
     * This is an alternative implementation of a stack to class
     * RWSlistCollectablesStack. The item pointed to by \a c is
     * put at the end of the collection.
     */
    void push(RWCollectable* c);

    /**
     * This is an alternative implementation of a stack to class
     * RWSlistCollectablesStack. The last item in the collection
     * is removed and returned. If there are no items in the
     * collection, #rwnil is returned.
     */
    RWCollectable* pop();

    /**
     * Removes the item at the position \a index in the collection and
     * returns it.
     */
    RWCollectable* removeAt(size_t index);

    /**
     * Called by RWOrdered::resize() to determine the new container
     * capacity. The default implementation resizes the container in
     * fixed increments.
     * By overriding this method in derived classes, users may alter the
     * resizing policy of the RWOrdered based containers.
     */
    virtual size_t newCapacity(size_t) const;

    void resize(size_t);

    /**
     * This is an alternative implementation of a stack to class
     * RWSlistCollectablesStack. The last item in the collection
     * is returned.  If there are no items in the collection,
     * #rwnil is returned.
     */
    RWCollectable* top() const;

    // For backward compatibility.
    virtual RWCollectable* insertAfter(int, RWCollectable*);

    /**
     * Swaps the data owned by self with the data owned by \a o.
     */
    void swap(RWOrdered& o);

protected:

    size_t nitems;

    // An array of pointers to objects.
    RWTPtrVector<RWCollectable> vec;

    void boundsCheck(size_t) const;
};



/**
 * @ingroup rwcollectable_classes
 *
 * @brief Traverses the collection from the first to the last item
 *
 * Iterator for class RWOrdered. Traverses the collection from
 * the first to the last item. Like all Rogue Wave iterators, the
 * "current item" is undefined immediately after construction  --
 * you must define it by using operator()() or some other (valid)
 * operation.
 *
 * Once the iterator has advanced beyond the end of the collection
 * it is no longer valid  --  continuing to use it brings undefined
 * results.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/ordcltn.h>
 * RWOrdered a ;
 * RWOrderedIterator iter(a);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWOrderedIterator : public RWIterator
{
    friend class RW_TOOLS_GLOBAL RWOrderedConstIterator;

public:

    /**
     * Constructs an RWOrderedIterator from an RWOrdered collection.
     * Immediately after construction the position of the iterator is
     * undefined.
     */
    RWOrderedIterator(const RWOrdered& ord) {
        theCollection = &ord;
        here = RW_NPOS;
    }

    virtual ~RWOrderedIterator();

    /**
     * Moves the iterator to the next item which
     * \link RWCollectable::isEqual() isEqual() \endlink to the item
     * pointed to by \a target and returns it. If no item is found,
     * returns #rwnil and the position of the iterator is undefined.
     */
    virtual RWCollectable* findNext(const RWCollectable* target);

    // Documented in base class.
    virtual RWCollectable* key() const;

    // Documented in base class.
    virtual RWCollectable* operator()();

    // Documented in base class.
    virtual void reset() {
        here = RW_NPOS;
    }

private:

    const RWOrdered* theCollection;

    size_t here;

};



/**
 * @ingroup rwcollectable_classes
 *
 * @brief Traverses the collection from the first to the last item
 *
 * One of two iterators for class RWOrdered. Traverses the collection
 * from the first to the last item. As a \c const iterator, this
 * iterator does not change anything in the underlying data structure.
 *
 * Like all Rogue Wave iterators, the "current item" is undefined
 * immediately after construction -- you must define it by using
 * operator()() or some other (valid) operation.
 *
 * Once the iterator has advanced beyond the end of the collection
 * it is no longer valid -- continuing to use it brings undefined
 * results.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/ordcltn.h>
 * RWOrdered a;
 * RWOrderedConstIterator iter(a);
 *
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWOrderedConstIterator : public RWConstIterator
{

public:

    /**
     * Constructs an RWOrderedConstIterator from an RWOrdered collection.
     * Immediately after construction the position of the iterator is
     * undefined.
     */
    RWOrderedConstIterator(const RWOrdered& ord) {
        theCollection = &ord;
        here = RW_NPOS;
    }

    /**
     * Constructs an RWOrderedConstIterator from an RWOrderedIterator.
     * Immediately after construction the position of the iterator is
     * undefined.
     */
    RWOrderedConstIterator(const RWOrderedIterator& it)
        : theCollection(it.theCollection), here(it.here) { }

    virtual ~RWOrderedConstIterator();

    /**
     * Moves the iterator to the next item which
     * \link RWCollectable::isEqual() isEqual() \endlink to the item
     * pointed to by \a target and returns it. If no item is found,
     * returns #rwnil and the position of the iterator is undefined.
     */
    virtual const RWCollectable* findNext(const RWCollectable* target);

    // Documented in base class.
    virtual const RWCollectable* key() const;

    // Documented in base class.
    virtual const RWCollectable* operator()();

    // Documented in base class.
    virtual void reset() {
        here = RW_NPOS;
    }

    virtual void reset(RWOrdered& ord) {
        theCollection = &ord;
        reset();
    }

    virtual const RWOrdered* container() const {
        return theCollection;
    }

private:

    const RWOrdered* theCollection;
    size_t here;

};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                      INLINES                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

inline RWCollectable*&
RWOrdered::operator[](size_t i)
{
    boundsCheck(i);
    return vec(i);
}

inline RWCollectable*&
RWOrdered::operator()(size_t i)
{
#ifdef RWBOUNDS_CHECK
    boundsCheck(i);
#endif
    return vec(i);
}

inline RWCollectable*
RWOrdered::operator[](size_t i) const
{
    boundsCheck(i);
    return this->vec(i);
}

inline RWCollectable*
RWOrdered::operator()(size_t i) const
{
#  ifdef RWBOUNDS_CHECK
    boundsCheck(i);
#  endif
    return this->vec(i);
}

inline const RWCollectableP*
RWOrdered::data() const
{
    return vec.data();
}

inline void
RWOrdered::push(RWCollectable* c)
{
    insert(c);
}

inline RWCollectable*
RWOrdered::pop()
{
    return nitems > 0 ? removeAt(nitems - 1) : rwnil;
}

inline RWCollectable*
RWOrdered::top() const
{
    return nitems > 0 ? this->vec(nitems - 1) : rwnil;
}

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif /* RW_TOOLS_ORDCLTN_H */
