#ifndef RW_TOOLS_COLLASS_H
#define RW_TOOLS_COLLASS_H



/**********************************************************************
 *
 * Declarations for RWCollectable Associations, used in dictionaries.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/collass.h#1 $
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
 * Binds a "key" and a "value" together.  The key and value must be RWCollectable.
 */

#include <rw/defs.h>
#include <rw/collect.h>

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Associates a key with a value in the Essential Tools Module
 * "dictionary" collection classes.
 *
 * RWCollectableAssociation inherits class RWCollectable.  Used
 * internally to associate a key with a value in the Essential Tools
 * Module "dictionary" collection classes.  Comparison and equality
 * testing are forwarded to the key part of the association.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/collass.h>
 * @endcode
 *
 * @section related Related Classes
 *
 * The dictionary containers RWBTreeDictionary, RWHashDictionary,
 * and RWIdentityDictionary make use of RWCollectableAssociation.
 * When any of their content is dealt with as an RWCollectable,
 * such as when operator+=() or asBag() is used, the
 * RWCollectableAssociation is exposed.
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWCollectableAssociation : public RWCollectable
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWCollectableAssociation);
    friend RWCollectable* rwCreateFN(RWCollectableAssociation)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns the unique ID for RWCollectableAssociation.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWCollectableAssociation*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWCollectableAssociation*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWCollectableAssociation& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWCollectableAssociation& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWCollectableAssociation& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWCollectableAssociation& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Default constructor
     */
    RWCollectableAssociation();

    /**
     * Constructs an RWCollectableAssociation with the given key and
     * value.
     */
    RWCollectableAssociation(RWCollectable* k, RWCollectable* v) {
        ky = k;
        val = v;
    }

    /**
     * Empty destructor.
     */
    virtual ~RWCollectableAssociation();

    /**
     * Returns the key part of the association.
     */
    RWCollectable* key() const {
        return ky;
    }

    /**
     * Returns the value part of the association.
     */
    RWCollectable* value() const {
        return val;
    }

    /**
     * Sets the value to \a ct and returns the old value.
     */
    RWCollectable* value(RWCollectable* ct);

    // Documented in base class.
    virtual RWspace binaryStoreSize() const;

    /**
     * Returns the results of calling RWCollectable::compareTo().
     */
    virtual int compareTo(const RWCollectable* c) const;

    /**
     * Returns the results of calling key()->hash().
     */
    virtual unsigned hash() const;

    /**
     * Returns the results of calling RWCollectable::isEqual().
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

protected:

    RWCollectable* ky;
    RWCollectable* val;

private: /* and deprecated */

    void                          setValue(RWCollectable* v) {
        val = v;
    }
};


/****************************************************************
 *                                                              *
 *                      RWCollectableIDAssociation              *
 *                                                              *
 ****************************************************************/

class RW_TOOLS_GLOBAL RWCollectableIDAssociation
    : public RWCollectableAssociation
{

    RW_DECLARE_COLLECTABLE_CLASS(RW_TOOLS_SYMBOLIC, RWCollectableIDAssociation);

public:

    RWCollectableIDAssociation();
    RWCollectableIDAssociation(RWCollectable* k, RWCollectable* v)
        : RWCollectableAssociation(k, v) { }

    virtual ~RWCollectableIDAssociation();

    /*** Override these methods in order to use addresses, not values ***/

    virtual unsigned hash() const;
    virtual int compareTo(const RWCollectable*) const;
    virtual int compareTo(const RWCollectable& c) const {
        return compareTo(&c);
    }
    virtual bool isEqual(const RWCollectable* c) const;
};


#endif /* RW_TOOLS_COLLASS_H */
