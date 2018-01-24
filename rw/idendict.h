#ifndef  RW_TOOLS_IDENDICT_H
#define  RW_TOOLS_IDENDICT_H

/**********************************************************************
 *
 * Declarations for RWIdentityDictionary (Hashed Identity Dictionary)
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/idendict.h#1 $
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
#include <rw/edefs.h>

/**
 * @ingroup rwcollectable_classes
 *
 * @brief A hash table for key-value pairs. Items are found by requiring
 * them to be identical
 *
 * The class RWIdentityDictionary is implemented as a hash table,
 * for the storage and retrieval of key-value pairs.  Class RWIdentityDictionary
 * is similar to class RWHashDictionary except that items are found
 * by requiring that they be \e identical (i.e., have the same
 * address) as the key, rather than being equal (i.e., test \c true
 * for \link RWCollectable::isEqual() isEqual()\endlink).
 *
 * <i>Both keys and values must inherit from the abstract base class</i>
 * RWCollectable.
 *
 * The iterator for this class is RWHashDictionaryIterator.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/idendict.h>
 * // Smalltalk typedef:
 * typedef RWIdentityDictionary IdentityDictionary;
 * RWIdentityDictionary a;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWIdentityDictionary : public RWHashDictionary
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWIdentityDictionary);
    friend RWCollectable* rwCreateFN(RWIdentityDictionary)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns \c __RWIDENTITYDICTIONARY.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    // Documented in base class.
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWIdentityDictionary*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWIdentityDictionary*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWIdentityDictionary& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWIdentityDictionary& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWIdentityDictionary& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWIdentityDictionary& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an empty identity dictionary with \a n hashing buckets.
     */
    RWIdentityDictionary(size_t n = RWCollection::DEFAULT_CAPACITY);

    /**
     * Copy constructor. Makes a shallow copy of \a h.
     */
    RWIdentityDictionary(const RWIdentityDictionary& h) : RWHashDictionary(h) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWIdentityDictionary takes ownership of the
     * data owned by \a h.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWIdentityDictionary(RWIdentityDictionary && h) : RWHashDictionary(rw_move(h)) { }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Assignment operator. Makes a shallow copy of the collection \a h.
     */
    RWIdentityDictionary& operator=(const RWIdentityDictionary& h) {
        RWHashDictionary::operator=(h);
        return *this;
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a h.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWIdentityDictionary& operator=(RWIdentityDictionary && h) {
        RWHashDictionary::operator=(rw_move(h));
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    bool operator<=(const RWIdentityDictionary& h) {
        return RWHashDictionary::operator<= (h);
    }

    /**
     * Returns \c true if self is a subset of \a h, that is, every
     * element of self has a counterpart in \a h which
     * \link RWCollectable::isEqual() isEqual()\endlink.
     * This operator is not explicitly present unless you are compiling
     * with an implementation of the C++ Standard Library. It is normally
     * inherited from RWHashDictionary.
     */
    bool operator<=(const RWIdentityDictionary& h) const {
        return RWHashDictionary::operator<= (h);
    }

    // Documented in base class.
    bool operator<=(const RWHashDictionary&) const;

    // Documented in base class.
    bool operator<=(const RWSet&) const;

    // Documented in base class.
    bool operator<=(const RWHashTable&) const;

    virtual ~RWIdentityDictionary();

    // Documented in base class.
    virtual bool isEqual(const RWCollectable*) const;

protected:

    virtual RWCollectableAssociation*
    findAssociation(const RWCollectable* key) const;

    virtual RWCollectableAssociation*
    newAssociation(RWCollectable*, RWCollectable*) const;

    virtual RWCollectableAssociation*
    removeAssociation(const RWCollectable* key);
};

#endif /* RW_TOOLS_IDENDICT_H */
