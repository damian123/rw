#ifndef RW_TOOLS_COLLINT_H
#define RW_TOOLS_COLLINT_H

/**********************************************************************
 *
 * RWCollectableInt --- RWCollectable Integers.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/collint.h#1 $
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
#include <rw/rwint.h>
#include <rw/collect.h>
#include <rw/tools/hash.h>

#include <iosfwd>

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Provides a polymorphically persistable RWInteger
 *
 * Collectable integers. Inherits from classes RWInteger and RWCollectable.
 * This class is useful when integers are used as keys in the "dictionary"
 * collection classes, or if integers are stored and retrieved as
 * RWCollectable instances.
 *
 * The virtual functions of the base class RWCollectable have
 * been redefined.
 *
 * @section synopsis Synopsis
 *
 * @code
 * typedef RWCollectableInt Integer;  // Smalltalk typedef
 * #include <rw/collint.h>
 * RWCollectableInt  i;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWCollectableInt
    : public RWCollectable, public RWInteger
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWCollectableInt);
    friend RWCollectable* rwCreateFN(RWCollectableInt)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns the unique ID for RWCollectableInt.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

public:

    /**
     * Calls the appropriate base class constructor. See
     * RWInteger::RWInteger().
     */
    RWCollectableInt();

    /**
     * Calls the appropriate base class constructor. See
     * RWInteger::RWInteger(int).
     */
    RWCollectableInt(int j) : RWInteger(j) { }

    RWCollectableInt(const RWCollectableInt& ci)
        : RWCollectable(ci), RWInteger(ci.value()) { }

    /**
     * Empty destructor.
     */
    virtual ~RWCollectableInt();

    bool operator==(const RWCollectableInt& c) const {
        return value() == c.value();
    }
    bool operator==(const RWInteger& rwi) const {
        return value() == rwi.value();
    }

    // Documented in base class.
    virtual RWspace binaryStoreSize() const {
        return RWInteger::binaryStoreSize();
    }

    /**
     * Returns the difference between
     * self and the RWCollectableInt pointed to by \a c.
     */
    virtual int compareTo(const RWCollectable* c) const;

    /**
     * Returns the value of RWCollectableInt
     * as an \c unsigned, to be used as a hash value.
     */
    virtual unsigned hash() const;

    /**
     * Returns \c true if self has
     * the same value as the RWCollectableInt at address \a c.
     */
    virtual bool isEqual(const RWCollectable* c) const;

    // Documented in base class.
    virtual void restoreGuts(RWvistream&);

    // Documented in base class.
    virtual void restoreGuts(RWFile&);

    // Documented in base class.
    virtual void saveGuts(RWvostream&) const;

    // Documented in base class.
    virtual void saveGuts(RWFile&) const;
};

inline std::ostream&
operator<<(std::ostream& os, const RWCollectableInt& cti)
{
    return os << (const RWInteger&)cti;
}

inline std::istream&
operator>>(std::istream& is, RWCollectableInt& cti)
{
    return is >> (RWInteger&)cti;
}

inline RWvostream&
operator<<(RWvostream& v, const RWCollectableInt& obj)
{
    obj.recursiveSaveOn(v);
    return v;
}

inline RWFile&
operator<<(RWFile& f, const RWCollectableInt& obj)
{
    obj.recursiveSaveOn(f);
    return f;
}

inline RWvistream&
operator>>(RWvistream& v, RWCollectableInt& obj)
{
    RWCollectable::recursiveRestoreFrom(v, &obj);
    return v;
}

inline RWFile&
operator>>(RWFile& f, RWCollectableInt& obj)
{
    RWCollectable::recursiveRestoreFrom(f, &obj);
    return f;
}

RW_TOOLS_SYMBOLIC RWvistream&
operator>>(RWvistream& s, RWCollectableInt*& pCl);

RW_TOOLS_SYMBOLIC RWFile&
operator>>(RWFile& f, RWCollectableInt*& pCl);

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWCollectableInt.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWCollectableInt> : public std::unary_function<RWCollectableInt, size_t> {
    /**
     * Returns a hash of \a obj. This function is the equivalent of calling
     *
     * @code
     * obj.hash();
     * @endcode
     */
    size_t operator()(const RWCollectableInt& obj) const {
        return obj.hash();
    }
};

#endif /* RW_TOOLS_COLLINT_H */
