#ifndef RW_TOOLS_COLLTIME_H
#define RW_TOOLS_COLLTIME_H


/**********************************************************************
 *
 * RWCollectableTime --- collectable times.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/colltime.h#1 $
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
#include <rw/rwtime.h>
#include <rw/collect.h>
#include <rw/zone.h>

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS

/**
 * @ingroup date_and_time_classes
 *
 * @deprecated As of SourcePro 11.1, use RWCollectableDateTime instead.
 *
 * RWCollectableTime and RWTime are deprecated and are no longer
 * supported. They may be removed from future releases. Please use
 * RWCollectableDateTime and RWDateTime.
 *
 * @brief Deprecated. Inherits classes RWTime and RWCollectable
 *
 * Inherits classes RWTime and RWCollectable.  This class is useful
 * when times are used as keys in the "dictionary" collection classes,
 * or if times are stored and retrieved as RWCollectable instances. \par
 *
 * The virtual functions of the base class RWCollectable have
 * been redefined.
 *
 * @section synopsis Synopsis
 *
 * @code
 * typedef RWCollectableTime Time;  // Smalltalk typedef
 * #include <rw/colltime.h>
 * RWCollectableTime  t;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_DEPRECATE_TYPE("Use RWCollectableDateTime instead") RW_TOOLS_GLOBAL RWCollectableTime
    : public RWCollectable,  public RWTime
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWCollectableTime);
    friend RWCollectable* rwCreateFN(RWCollectableTime)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns the unique ID for RWCollectableTime.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWCollectableTime*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWCollectableTime*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWCollectableTime& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWCollectableTime& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWCollectableTime& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWCollectableTime& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Calls the corresponding constructor of RWTime.
     */
    RWCollectableTime();

    /**
     * Calls the corresponding constructor of RWTime.
     */
    RWCollectableTime(unsigned long s) : RWTime(s) { }

    /**
     * Calls the corresponding constructor of RWTime.
     */
    RWCollectableTime(const RWTime& t) : RWTime(t) { }

    /**
     * Calls the corresponding constructor of RWTime.
     */
    RWCollectableTime(unsigned h, unsigned m, unsigned s = 0,
                      const RWZone& zone = RWZone::local())
        : RWTime(h, m, s, zone) { }

    /**
     * Calls the corresponding constructor of RWTime.
     */
    RWCollectableTime(const RWDate& d, unsigned h = 0, unsigned m = 0, unsigned s = 0,
                      const RWZone& zone = RWZone::local())
        : RWTime(d, h, m, s, zone) { }

    /**
     * Calls the corresponding constructor of RWTime.
     */
    RWCollectableTime(const RWDate& d, const RWCString& str,
                      const RWZone& z = RWZone::local(),
                      const RWLocale& l = RWLocale::global())
        : RWTime(d, str, z, l) { }

    /**
     * Calls the corresponding constructor of RWTime.
     */
    RWCollectableTime(const struct tm* tmb, const RWZone& zone = RWZone::local())
        : RWTime(tmb, zone) { }

    /**
     * Empty destructor.
     */
    virtual ~RWCollectableTime();

    // Documented in base class.
    virtual RWspace binaryStoreSize() const {
        return RWTime::binaryStoreSize();
    }

    /**
     * Returns the results of calling RWTime::compareTo().
     */
    virtual int compareTo(const RWCollectable* c) const;

    /**
     * Returns the results of calling RWTime::hash().
     */
    virtual unsigned hash() const;

    /**
     * Returns the results of calling operator==()
     * for the base class RWTime by using appropriate casts.
     */
    virtual bool isEqual(const RWCollectable*) const;

    // Documented in base class.
    virtual void restoreGuts(RWvistream&);

    // Documented in base class.
    virtual void restoreGuts(RWFile&);

    // Documented in base class.
    virtual void saveGuts(RWvostream&) const;

    // Documented in base class.
    virtual void saveGuts(RWFile&) const;
};

RW_RESTORE_DEPRECATED_WARNINGS
#endif

#endif /* RW_TOOLS_COLLTIME_H */
