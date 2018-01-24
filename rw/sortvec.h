#ifndef RW_TOOLS_SORTVEC_H
#define RW_TOOLS_SORTVEC_H

/**********************************************************************
 *
 * RWSortedVector -- sorted vector; uses insertion sort.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/sortvec.h#1 $
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

/*
 * This collection inherits from RWOrdered.  A few member functions
 * must be disallowed because the insertion order is determined internally
 * rather than by function calls.  Examples are "insertAfter()", or
 * "prepend()".  A few others can be done more efficiently because we can
 * take advantage of the internal sorting of objects.  Examples are "index()",
 * "occurrencesOf()", etc.
 */

#include <rw/defs.h>
#include <rw/ordcltn.h>


/**
 * @relates RWSortedVector
 *
 * RWSortedVectorIterator is a typedef for RWOrderedIterator.
 * For information on how to use it, please see RWOrderedIterator.
 */
typedef RWOrderedIterator RWSortedVectorIterator;

// Needs documentation.
typedef RWOrderedConstIterator RWSortedVectorConstIterator;

/**
 * @ingroup rwcollectable_classes
 * @brief Represents a group of ordered items.
 *
 * Class RWSortedVector represents a group of ordered items, internally
 * sorted by the compareTo() function and accessible by an index
 * number.  Duplicates are allowed.  An object stored by RWSortedVector
 * must inherit from the abstract base class RWCollectable.  An
 * insertion sort is used to maintain the vector in sorted order.
 *
 * Because class RWSortedVector is implemented as a vector of pointers,
 * traversing the collection is more efficient than using RWBinaryTree.
 * However, insertions are slower in the center of the collection.
 *
 * @note
 * The vector is sorted; do not modify elements contained in the vector
 * in such a way as to invalidate the ordering.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/sortvec.h>
 * RWSortedVector a;
 * @endcode
 *
 * @section persistence Persistence
 * Polymorphic
 *
 * @section example Example
 *
 * @code
 * #include <rw/sortvec.h>
 * #include <rw/collstr.h>
 *
 * int main ()
 * {
 *     RWSortedVector sv;
 *
 *     sv.insert(new RWCollectableString("monster"));
 *     sv.insert(new RWCollectableString("rabbit"));
 *     sv.insert(new RWCollectableString("hog"));
 *     sv.insert(new RWCollectableString("dog"));
 *     sv.insert(new RWCollectableString("cat"));
 *     sv.insert(new RWCollectableString("fish"));
 *
 *     RWSortedVectorIterator next(sv);
 *     RWCollectableString* item;
 *
 *     while( (item = (RWCollectableString*)next() ) != 0)
 *         std::cout << *item << std::endl;
 *
 *     sv.clearAndDestroy();
 *
 *     return 0;
 * }
 * @endcode
 *
 * <i>Program output:</i>
 * @code
 * cat
 * dog
 * fish
 * hog
 * monster
 * rabbit
 * @endcode
 */
class RW_TOOLS_GLOBAL RWSortedVector : public RWOrdered
{

private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWSortedVector);
    friend RWCollectable* rwCreateFN(RWSortedVector)();

public:
    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns \c __RWSORTEDVECTOR, i.e. a <i>class identifier</i>,
     * that identifies this object's class.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    // Documented in base class.
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWSortedVector*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWSortedVector*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWSortedVector& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWSortedVector& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWSortedVector& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWSortedVector& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an empty RWSortedVector that has an initial capacity
     * of \a size items.  The capacity will be increased automatically
     * as needed.
     */
    RWSortedVector(size_t size = RWCollection::DEFAULT_CAPACITY);

    virtual ~RWSortedVector();

    /**
     * Copy constructor. Makes a shallow copy of \a sv.
     */
    RWSortedVector(const RWSortedVector& sv) : RWOrdered(sv) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWSortedVector takes ownership of the
     * data owned by \a sv.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWSortedVector(RWSortedVector && sv) : RWOrdered(rw_move(sv)) { }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Assignment operator. Makes a shallow copy of \a o.
     */
    RWSortedVector& operator=(const RWSortedVector& sv) {
        RWOrdered::operator=(sv);
        return *this;
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a sv.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWSortedVector& operator=(RWSortedVector && sv) {
        RWOrdered::operator=(rw_move(sv));
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns \c true if for every item in self, the corresponding
     * item in \a sv at the same index isEqual().  The two collections
     * must also have the same number of members.
     */
    bool operator==(const RWSortedVector& sv) const {
        return RWOrdered::operator==(sv);
    }

    // Documented in base class.
    virtual const RWCollectable* at(size_t) const;

    /**
     * Performs a binary search to return the index of the first item
     * that compares equal to the \a target item, or #RW_NPOS if no
     * such item can be found.
     *
     * @note
     * Derived methods RWOrdered::find() and RWOrdered::remove()
     * use this virtual function to perform their search.
     * Hence, these methods called on an instance of RWSortedVector
     * results in a binary search.
     */
    virtual size_t index(const RWCollectable* target) const;

    /**
     * Performs a binary search to insert the item pointed to by \a c
     * after all items that compare less than or equal to it, but before
     * all items that compare greater to it.  Returns \a c if the
     * insertion was successful, otherwise returns #rwnil.
     */
    virtual RWCollectable* insert(RWCollectable* c);

    // Documented in base class.
    virtual size_t occurrencesOf(const RWCollectable* target) const;

    /**
     * Returns \c true or \c false based on operator==().
     */
    virtual bool isEqual(const RWCollectable* a) const;

    /**
     * Returns the \a i <sup>th</sup> element in the collection.
     * If \a i is out of range, an exception of type RWBoundsErr
     * is thrown.  The return value cannot be used as an lvalue.
     */
    const RWCollectable* operator[](size_t i) const {
        boundsCheck(i);
        return this->vec(i);
    }

    /**
     * Returns the \a i <sup>th</sup> element in the collection.
     * Bounds checking is enabled by defining the preprocessor directive
     * \c RWBOUNDS_CHECK before including the header file \c <rw/sortvec.h>.
     * In this case, if \a i is out of range, an exception of type
     * RWBoundsErr is thrown.  The return value cannot be used as an lvalue.
     */
    const RWCollectable* operator()(size_t i) const {
#ifdef RWBOUNDS_CHECK
        boundsCheck(i);
#endif
        return this->vec(i);
    }

private:

    virtual RWCollectable*& at(size_t);

    virtual RWCollectable* append(RWCollectable* a);

    virtual RWCollectable* insertAt(size_t, RWCollectable*);

    virtual RWCollectable* prepend(RWCollectable*);

    void push(RWCollectable*);

    RWCollectable* pop();

    RWCollectable* top() const;

    virtual RWCollectable* insertAfter(int, RWCollectable*);

#if defined(RW_DEBUG)
    bool isSorted() const;
#endif
};

#endif /* RW_TOOLS_SORTVEC_H */
