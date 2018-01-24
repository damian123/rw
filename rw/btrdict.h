#ifndef RW_TOOLS_BTRDICT_H
#define RW_TOOLS_BTRDICT_H



/**********************************************************************
 *
 * RWBTreeDictionary --- Dictionary implemented as an in memory B-Tree
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/btrdict.h#1 $
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
#include <rw/edefs.h>
#include <rw/btree.h>
#include <rw/colclass.h>

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Represents a dictionary for the storage and retrieval of
 * key-value pairs, and implemented as a B-tree.
 *
 * Dictionary class implemented as a B-tree, for the storage and
 * retrieval of key-value pairs. Both the keys and values must inherit
 * abstract base class RWCollectable. The elements are ordered
 * internally according to the value returned by virtual function
 * RWCollectable::compareTo() of the keys. Duplicate keys
 * are not allowed.
 *
 * The B-tree is \e balanced. That is, nodes are never allowed
 * to have less than a certain number of items (called the \e order).
 * The default order is 50, but may be changed by resetting the
 * value of the static constant \c order in the header file
 * \c <btree.h> and recompiling. Larger values result in shallower
 * trees, but less efficient use of memory.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/btrdict.h>
 * RWBTreeDictionary a;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWBTreeDictionary : public RWBTree
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWBTreeDictionary);
    friend RWCollectable* rwCreateFN(RWBTreeDictionary)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns \c __RWBTREEDICTIONARY.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWBTreeDictionary*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWBTreeDictionary*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWBTreeDictionary& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWBTreeDictionary& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWBTreeDictionary& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWBTreeDictionary& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an empty B-tree dictionary.
     */
    RWBTreeDictionary();

    /**
     * Constructs self as a shallow copy of \a btr.
     */
    RWBTreeDictionary(const RWBTreeDictionary& btr);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWBTreeDictionary takes ownership of the
     * data owned by \a btr.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWBTreeDictionary(RWBTreeDictionary && btr) : RWBTree(rw_move(btr)) { }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Sets self to a shallow copy of \a btr.
     */
    RWBTreeDictionary& operator=(const RWBTreeDictionary& btr);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a btr.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWBTreeDictionary& operator=(RWBTreeDictionary && btr) {
        RWBTree::operator=(rw_move(btr));
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Calls clear().
     */
    virtual ~RWBTreeDictionary();

    /**
     * Returns \c true if self and \a btr are equivalent. That is, they
     * must have the same number of items and for every item in self,
     * there must be an item in \a btr that compares equal.
     */
    bool operator==(const RWBTreeDictionary& btr) const;

    /**
     * Returns \c true if self is a subset of \a btr. That is, for every
     * item in self, there must be an item in \a btr that compares equal.
     * Normally this method is inherited from RWBTree.
     */
    bool operator<=(const RWBTreeDictionary& btr) const {
        return RWBTree::operator<=(btr);
    }

    /**
     * Invokes the function pointer \a ap on each key-value pair in the
     * collection, in order, from smallest to largest.
     */
    void applyToKeyAndValue(RWapplyKeyAndValue ap, void*);

    /**
     * Removes all key-value pairs from the collection.
     */
    virtual void clear();

    /**
     * Removes all key-value pairs from the collection, and deletes
     * \e both the key and the value.
     */
    virtual void clearAndDestroy();

    /**
     * Returns the key in the collection that compares equal to the
     * object pointed to by \a target, or #rwnil if no key is found.
     */
    virtual RWCollectable* find(const RWCollectable* target) const;

    /**
     * Returns the key in the collection that compares equal to the
     * object pointed to by \a target, or #rwnil if no key was found.
     * The value is put in \a v. You are responsible for defining
     * \a v before calling this function.
     */
    RWCollectable* findKeyAndValue(const RWCollectable* target, RWCollectable*& v) const;

    /**
     * Returns the \e value associated with the key that compares
     * equal to the object pointed to by \a target, or #rwnil if no
     * key was found.
     */
    RWCollectable* findValue(const RWCollectable* target) const;

    /**
     * Returns the \e value associated with the key that compares
     * equal to the object pointed to by \a target, or #rwnil if no
     * key was found. Replaces the value with \a newValue (if a key
     * was found).
     */
    RWCollectable* findValue(const RWCollectable* target, RWCollectable* newValue) const;

    /**
     * Adds a key-value pair to the collection and returns the key if
     * successful, or #rwnil if the key is already in the collection.
     */
    RWCollectable* insertKeyAndValue(RWCollectable* key, RWCollectable* value);

    // Documented in base class.
    virtual bool isEqual(const RWCollectable* a) const;

    /**
     * Removes the key-value pair for which the key compares equal
     * to the object pointed to by \a target. Returns the key, or
     * #rwnil if no match was found.
     */
    virtual RWCollectable* remove(const RWCollectable* target);

    /**
     * Removes \e and deletes the key-value pair for which the
     * key compares equal to the object pointed to by \a target. Note
     * that both the key and the value are deleted. Does nothing if
     * the key is not found.
     */
    virtual void removeAndDestroy(const RWCollectable* target);

    /**
     * Removes the key-value pair for which the key compares equal
     * to the object pointed to by target. Returns the key, or #rwnil
     * if no match was found. The value is put in \a v. You are responsible
     * for defining \a v before calling this function.
     */
    RWCollectable* removeKeyAndValue(const RWCollectable* target, RWCollectable*& v);

    /**
     * Evaluates the function pointed to by \a testfunc for the key
     * of each item in the RWBTreeDictionary. It inserts keys and values
     * for which the function returns \c true into a new RWBTreeDictionary
     * allocated off the heap and returns a pointer to this new collection.
     * Because the new dictionary is allocated <i>off the heap</i>,
     * you are responsible for deleting it when done. This is not a
     * virtual function.
     */
    RWCollection* select(RWtestCollectable testfunc, void* x) const;

    /**
     * Evaluates the function pointed to by \a testfunc for both the
     * key and the value of each item in the RWBTreeDictionary. It inserts
     * keys and values for which the function returns \c true into a
     * new RWBTreeDictionary allocated off the heap and returns a pointer
     * to this new collection. Because the new dictionary is allocated
     * <i>off the heap</i>, you are responsible for deleting it when
     * done. This is not a virtual function.
     */
    RWCollection* select(RWtestCollectablePair testfunc, void* x) const;

};


#endif /* RW_TOOLS_BTRDICT_H */
