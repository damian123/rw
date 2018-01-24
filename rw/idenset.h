#ifndef  RW_TOOLS_IDENSET_H
#define  RW_TOOLS_IDENSET_H

/**********************************************************************
 *
 * Declarations for RWIdentitySet (i.e., an Identity Set).
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/idenset.h#1 $
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
 * For the storage and retrieval of items using the address of the item
 */

#include <rw/defs.h>
#include <rw/rwset.h>

/****************************************************************
 *                                                              *
 *                      RWIdentitySet                           *
 *                                                              *
 ****************************************************************/

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Finds items that have the same address as the key
 *
 * The class RWIdentitySet is similar to class RWSet except that
 * items are found by requiring that they be \e identical (i.e.,
 * have the same address) as the key, rather than being equal (i.e.,
 * test \c true for \link RWCollectable::isEqual() isEqual()\endlink).
 *
 * <i>The iterator for this class is</i> RWSetIterator.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/idenset.h>
 * typedef RWIdentitySet IdentitySet; // Smalltalk typedef
 * RWIdentitySet a;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWIdentitySet : public RWSet
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWIdentitySet);
    friend RWCollectable* rwCreateFN(RWIdentitySet)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns the unique ID for RWIdentitySet.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWIdentitySet*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWIdentitySet*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWIdentitySet& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWIdentitySet& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWIdentitySet& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWIdentitySet& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an empty identity set with \a N hashing buckets.
     */
    RWIdentitySet(size_t N = RWCollection::DEFAULT_CAPACITY);

    /**
     * Copy constructor.  Makes a shallow copy of the collection
     * \a s.
     */
    RWIdentitySet(const RWIdentitySet& s) : RWSet(s) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWIdentitySet takes ownership of the
     * data owned by \a s.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWIdentitySet(RWIdentitySet && s) : RWSet(rw_move(s)) { }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Assignment operator.  Makes a shallow copy of the collection
     * \a s.
     */
    RWIdentitySet& operator=(const RWIdentitySet& s) {
        RWSet::operator=(s);
        return *this;
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a s.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWIdentitySet& operator=(RWIdentitySet && s) {
        RWSet::operator=(rw_move(s));
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    virtual ~RWIdentitySet();

    // Redefined methods use identity semantics instead of value semantics:

    virtual RWCollectable* find(const RWCollectable*) const;

    virtual RWCollectable* insert(RWCollectable*);

    virtual RWCollectable* remove(const RWCollectable*);

    virtual bool isEqual(const RWCollectable*) const;

protected:

    size_t hashIndexReference(const RWCollectable* p) const;

};

/****************************************************************
 *                                                              *
 *                      INLINES                                 *
 *                                                              *
 ****************************************************************/

inline size_t
RWIdentitySet::hashIndexReference(const RWCollectable* a) const
{
    return RWhashAddress(a) % buckets();
}

#endif /* RW_TOOLS_IDENSET_H */
