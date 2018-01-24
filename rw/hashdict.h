#ifndef  RW_TOOLS_HASHDICT_H
#define  RW_TOOLS_HASHDICT_H

/**********************************************************************
 *
 * RWHashDictionary -- dictionary implemented using a hash table
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/hashdict.h#1 $
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
#include <rw/rwset.h>

class RW_TOOLS_GLOBAL RWCollectableAssociation;

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Represents a group of unordered values, accessible
 * by external keys
 *
 * An RWHashDictionary represents a group of unordered values, accessible
 * by external keys. Duplicate keys are not allowed. RWHashDictionary
 * is implemented as a hash table of associations of keys and values.
 * Both the key and the value must inherit from the abstract base
 * class RWCollectable, with a suitable definition of the virtual
 * function hash() and isEqual() for the key.
 *
 * This class corresponds to the Smalltalk class \b Dictionary.
 *
 * @section synopsis Synopsis
 *
 * @code
 * typedef RWHashDictionary Dictionary;  // Smalltalk typedef.
 * #include <rw/hashdict.h>
 * RWHashDictionary  a ;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWHashDictionary : public RWSet
{
    friend class RW_TOOLS_GLOBAL RWHashDictionaryIterator;

private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWHashDictionary);
    friend RWCollectable* rwCreateFN(RWHashDictionary)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns \c __RWHASHDICTIONARY.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    // Documented in base class.
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWHashDictionary*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWHashDictionary*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWHashDictionary& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWHashDictionary& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWHashDictionary& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWHashDictionary& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an empty hashed dictionary using \a n hashing buckets.
     */
    RWHashDictionary(size_t n = RWCollection::DEFAULT_CAPACITY);

    /**
     * Copy constructor. Makes a shallow copy of the collection \a hd.
     */
    RWHashDictionary(const RWHashDictionary& hd);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWHashDictionary takes ownership of the
     * data owned by \a hd.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWHashDictionary(RWHashDictionary && hd) : RWSet(rw_move(hd)) { }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Calls clear().
     */
    virtual ~RWHashDictionary();

    /**
     * Assignment operator. Makes a shallow copy of the collection \a hd.
     */
    RWHashDictionary& operator=(const RWHashDictionary& hd);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a hd.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWHashDictionary& operator=(RWHashDictionary && hd) {
        RWSet::operator=(rw_move(hd));
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns \c true if for every key-value pair in self, there is
     * a corresponding key in \a hd that isEqual(). Their corresponding
     * values must also be equal.
     */
    bool operator<=(const RWHashDictionary& hd) const;

    // Documented in base class.
    bool operator<=(const RWSet& hd) const;

    // Documented in base class.
    bool operator<=(const RWHashTable& hd) const;

    /**
     * Returns \c true if self and \a hd have the same number of entries
     * and if for every key-value pair in self, there is a corresponding
     * key in \a hd that isEqual(). Their corresponding values must
     * also be equal.
     */
    bool operator==(const RWHashDictionary& hd) const;

    // Documented in base class.
    bool operator==(const RWHashTable& hd) const;

    /**
     * Invokes the function pointer \a ap on each key-value pair in the
     * collection. Pairs are not visited in any particular order. An untyped
     * argument may be passed to the \a ap function through \a x.
     */
    void applyToKeyAndValue(RWapplyKeyAndValue ap, void* x);

    /**
     * Removes all key-value pairs in the collection.
     */
    virtual void clear();

    /**
     * Removes all key-value pairs
     * in the collection, and deletes the key \e and the value.
     */
    virtual void clearAndDestroy();

    /**
     * Returns the key which
     * isEqual() to the object pointed to by \a target, or #rwnil if
     * no key was found.
     */
    virtual RWCollectable* find(const RWCollectable* target) const;

    /**
     * Returns the key which isEqual() to the item pointed to by \a
     * target, or #rwnil if no key was found. The value is put in \a
     * v. You are responsible for defining \a v before calling this
     * function.
     */
    RWCollectable* findKeyAndValue(const RWCollectable* target, RWCollectable*& v) const;

    /**
     * Returns the value associated with the key which isEqual()
     * to the item pointed to by \a target, or #rwnil if no key was
     * found.
     */
    RWCollectable* findValue(const RWCollectable* target) const;

    /**
     * Returns the value associated with the key which isEqual()
     * to the item pointed to by \a target, or #rwnil if no key was
     * found. Replaces the value with \a newValue (if a key was found).
     */
    RWCollectable* findValue(const RWCollectable* target, RWCollectable* newValue) const;

    /**
     * Adds a key-value pair to the collection and returns the key if
     * successful, #rwnil if the key is already in the collection.
     */
    RWCollectable* insertKeyAndValue(RWCollectable* key, RWCollectable* value);

    // Documented in base class.
    virtual bool isEqual(const RWCollectable*) const;

    /**
     * Removes the key and value
     * pair where the key isEqual() to the item pointed to by \a target.
     * Returns the key, or #rwnil if no match was found.
     */
    virtual RWCollectable* remove(const RWCollectable* target);

    /**
     * Removes \e and deletes
     * the key and value pair where the key isEqual() to the item pointed
     * to by \a target. Note that both the key and the value are deleted.
     * Does nothing if the key is not found.
     */
    virtual void removeAndDestroy(const RWCollectable* target);

    /**
     * Removes the key and value pair where the key isEqual() to the
     * item pointed to by \a target. Returns the key, or #rwnil if
     * no match was found. The value part of the removed pair is put
     * in \a v. You are responsible for defining \a v before calling
     * this function.
     */
    RWCollectable* removeKeyAndValue(const RWCollectable* target, RWCollectable*& v);

    /**
     * Evaluates the function pointed to by \a tst for the key of each
     * item in the RWHashDictionary. It inserts keys and values for
     * which the function returns \c true into a new RWHashDictionary
     * allocated off the heap and returns a pointer to this new collection.
     * Because the new dictionary is allocated <i>off the heap</i>,
     * you are responsible for deleting it when done. This is a \c
     * virtual function which hides the non-virtual function inherited
     * from RWCollection.
     */
    virtual RWCollection* select(RWtestCollectable tst, void* x) const;

    /**
     * Evaluates the function pointed to by \a testfunc for both the
     * key and the value of each item in the RWHashDictionary. It inserts
     * keys and values for which the function returns \c true into a
     * new RWHashDictionary allocated off the heap and returns a pointer
     * to this new collection. Because the new dictionary is allocated
     * <i>off the heap</i>, you are responsible for deleting it when
     * done. This is a \c virtual function which hides the non-virtual
     * function inherited from RWCollection.
     */
    virtual RWCollection* select(RWtestCollectablePair testfunc, void* x) const;

    // Documented in base class.
    virtual RWIterator* newIterator();

    // Documented in base class.
    virtual RWConstIterator* newConstIterator() const;

protected:

    void copyAssociations();

    virtual RWCollectableAssociation*
    findAssociation(const RWCollectable* key) const;

    virtual RWCollectableAssociation*
    newAssociation(RWCollectable*, RWCollectable*) const;

    virtual RWCollectableAssociation*
    removeAssociation(const RWCollectable* key);

private:

    virtual RWCollectable* insert(RWCollectable* a) {
        return RWSet::insert(a);
    }

};

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Allows sequential access
 * to all the elements of RWHashDictionary
 *
 * Iterator for class RWHashDictionary, allowing sequential access
 * to all the elements of RWHashDictionary. Since RWHashDictionary
 * is unordered, elements are not accessed in any particular order.
 *
 * Like all Rogue Wave iterators, the "current item" is undefined
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
 * #include <rw/hashdict.h>
 * RWHashDictionary hd;
 * RWHashDictionaryIterator  iter(hd);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWHashDictionaryIterator
    : public RWSetIterator
{
    friend class RWHashDictionaryConstIterator;

public:

    /**
     * Constructs an iterator for an RWHashDictionary collection. Immediately
     * after construction, the position of the iterator is undefined
     * until positioned.
     */
    RWHashDictionaryIterator(RWHashDictionary& hd) : RWSetIterator(hd) { }

    /**
     * Empty destructor.
     */
    virtual ~RWHashDictionaryIterator() { }

    /**
     * Moves the iterator to the next
     * key-value pair where the key isEqual() to the object pointed
     * to by \a target. Returns the key or #rwnil if no key was found.
     */
    RWCollectable* findNext(const RWCollectable* target);

    // Documented in base class.
    RWCollectable* key() const;

    /**
     * Advances the iterator to the
     * next key-value pair and returns the key. Returns #rwnil if
     * the cursor is at the end of the collection. Use member function
     * value() to recover the value.
     */
    RWCollectable* operator()();

    // Documented in base class.
    void reset() {
        RWHashTableIterator::reset();
    }

    /**
     * Removes the key-value pair at the current iterator position.
     * Returns the key, or #rwnil if there was no key-value pair.
     */
    RWCollectable* remove();

    /**
     * Moves the iterator to the next key-value pair where the key
     * isEqual() to the object pointed to by \a target. Removes the key-value
     * pair, returning the key or #rwnil if there was no match.
     */
    RWCollectable* removeNext(const RWCollectable* target);

    /**
     * Returns the value at the current iterator position.
     */
    RWCollectable* value() const;

    /**
     * Replaces the value at the current iterator position and returns
     * the old value.
     */
    RWCollectable* value(RWCollectable* newValue) const;

};

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Allows sequential access to all the elements of RWHashDictionary
 * and, as a \c const iterator, does not change anything in the
 * underlying data structure
 *
 * One of two iterators for class RWHashDictionary. This iterator
 * allows sequential access to all the elements of RWHashDictionary,
 * and, as a \c const iterator, does not change anything in the
 * underlying data structure. Since RWHashDictionary is unordered,
 * elements are not accessed in any particular order.
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
 * #include <rw/hashdict.h>
 *
 * RWHashDictionary hd;
 * RWHashDictionaryConstIterator  iter(hd);
 *
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWHashDictionaryConstIterator
    : public RWSetConstIterator
{
public:

    /**
     * Constructs a \c const iterator for an RWHashDictionary collection.
     * Immediately after construction, the position of the iterator
     * is undefined.
     */
    RWHashDictionaryConstIterator(const RWHashDictionary& hd)
        : RWSetConstIterator(hd) { }

    /**
     * Constructs a \c const iterator for an RWHashDictionary collection
     * from an iterator. Immediately after construction, the position of the
     * \c const iterator is undefined.
     */
    RWHashDictionaryConstIterator(const RWHashDictionaryIterator& it)
        : RWSetConstIterator(it) { }

    virtual ~RWHashDictionaryConstIterator() { }

    /**
     * Moves the iterator to
     * the next key-value pair where the key isEqual() to the object
     * pointed to by \a target. Returns the key or #rwnil if no key
     * was found.
     */
    const RWCollectable* findNext(const RWCollectable* target);

    /**
     * Returns the key at the current iterator position.
     */
    const RWCollectable* key() const;

    /**
     * Advances the iterator to the next key-value pair and returns the key.
     * Returns #rwnil if the cursor is at the end of the collection.
     * Use member function value() to recover the value.
     */
    const RWCollectable* operator()();

    // Documented in base class.
    virtual void reset() {
        RWSetConstIterator::reset();
    }

    virtual void reset(RWHashTable& hd) {
        RWHashTableConstIterator::reset(hd);
    }

    /**
     * Returns the value at the current iterator position.
     */
    const RWCollectable* value() const;

};

#endif /* RW_TOOLS_HASHDICT_H */
