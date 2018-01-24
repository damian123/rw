#ifndef RW_TOOLS_COLLDATE_H
#define RW_TOOLS_COLLDATE_H



/**********************************************************************
 *
 * RWCollectableDate --- RWCollectable Dates.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/colldate.h#1 $
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


#include <rw/tools/ristream.h>
#include <rw/tools/rostream.h>
#include <rw/rwdate.h>
#include <rw/collect.h>
#include <rw/locale.h>

/**
 * @ingroup date_and_time_classes
 *
 * @brief Inherits from classes RWDate and RWCollectable
 *
 * Collectable Dates. Inherits from classes RWDate and RWCollectable.
 * This class is useful when dates are used as keys in the "dictionary"
 * collection classes, or if dates are stored and retrieved as RWCollectable
 * instances.
 *
 * The virtual functions of the base class RWCollectable have been
 * redefined.
 *
 * @section synopsis Synopsis
 *
 * @code
 * typedef RWCollectableDate Date;  // Smalltalk typedef
 * #include <rw/colldate.h>
 * RWCollectableDate  d;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWCollectableDate
    : public RWCollectable, public RWDate
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWCollectableDate);
    friend RWCollectable* rwCreateFN(RWCollectableDate)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns the unique ID for RWCollectableDate.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWCollectableDate*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWCollectableDate*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWCollectableDate& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWCollectableDate& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWCollectableDate& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWCollectableDate& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Calls the corresponding constructor of the base class RWDate.
     */
    RWCollectableDate();

    /**
     * Calls the corresponding constructor of the base class RWDate.
     */
    RWCollectableDate(unsigned long j) : RWDate(j) { }

    /**
     * Calls the corresponding constructor of the base class RWDate.
     */
    RWCollectableDate(unsigned d, unsigned y) : RWDate(d, y) { }

    /**
     * Calls the corresponding constructor of the base class RWDate.
     */
    RWCollectableDate(unsigned d, const char* s, unsigned y,
                      const RWLocale& loc = RWLocale::global())
        : RWDate(d, s, y, loc) { }

    /**
     * Calls the corresponding constructor of the base class RWDate.
     */
    RWCollectableDate(unsigned d, unsigned m, unsigned y)
        : RWDate(d, m, y) { }

    /**
     * Calls the corresponding constructor of the base class RWDate.
     */
    RWCollectableDate(std::istream& s,
                      const RWLocale& locale = RWLocale::global())
        : RWDate(s, locale) { }

    /**
     * Calls the corresponding constructor of the base class RWDate.
     */
    RWCollectableDate(const RWCString& str,
                      const RWLocale& locale = RWLocale::global())
        : RWDate(str, locale) { }

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS
    /**
     * @deprecated As of SourcePro 13, Use RWDateTime::toRWDate(const* RWZone&)
     * instead.
     *
     * Calls the corresponding constructor of the base class RWDate.
     */
    RW_DEPRECATE_FUNC("Use RWDateTime::toRWDate(const RWZone&) instead")
    RWCollectableDate(const RWTime& t, const RWZone& zone = RWZone::local())
        : RWDate(t, zone) { }

    RW_RESTORE_DEPRECATED_WARNINGS
#endif

    /**
     * Calls the corresponding constructor of the base class RWDate.
     */
    RWCollectableDate(const RWDate& d) : RWDate(d) { }

    /**
     * Calls the corresponding constructor of the base class RWDate.
     */
    RWCollectableDate(const struct tm* tmb) : RWDate(tmb) { }

    /**
     * Empty destructor.
     */
    virtual ~RWCollectableDate();

    // Documented in base class.
    virtual RWspace binaryStoreSize() const {
        return RWDate::binaryStoreSize();
    }

    /**
     * Returns the results of calling RWDate::compareTo().
     */
    virtual int compareTo(const RWCollectable*) const;

    /**
     * Returns the results of calling RWDate::hash().
     */
    virtual unsigned hash() const;

    /**
     * Returns the results of calling operator==()
     * for the base class RWDate by using appropriate casts.
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

#endif /* __RWCOLLECTABLE_DATE__ */
