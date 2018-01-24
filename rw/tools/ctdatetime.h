#ifndef RW_TOOLS_CTDATETIME_H
#define RW_TOOLS_CTDATETIME_H



/**********************************************************************
 *
 * Declarations for the RWCollectableDateTime class
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/ctdatetime.h#1 $
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


#include <rw/tools/datetime.h>
#include <rw/tools/hash.h>
#include <rw/collect.h>

class  RWCString;
class  RWZone;

#if !defined(RW_DISABLE_DEPRECATED)
class  RWTime;
#endif

class  RWDate;
struct tm;

/**
 * @ingroup date_and_time_classes
 *
 * @brief Provides a polymorphically persistable RWDateTime
 *
 * Collectable RWDateTime instances. Inherits from classes RWDateTime and
 * RWCollectable. This class is useful when date/time objects are
 * used as keys in the "dictionary" collection classes, or if date/time
 * objects are stored and retrieved as RWCollectable instances.
 *
 * Redefines the virtual functions of the base class RWCollectable.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tools/ctdatetime.h>
 * RWCollectableDateTime d;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWCollectableDateTime
    : public RWCollectable, public RWDateTime
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWCollectableDateTime);
    friend RWCollectable* rwCreateFN(RWCollectableDateTime)();

public:
    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns the unique ID for RWCollectableDateTime.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWCollectableDateTime*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWCollectableDateTime*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWCollectableDateTime& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWCollectableDateTime& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWCollectableDateTime& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWCollectableDateTime& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Calls the corresponding constructor of the base class RWDateTime.
     */
    RWCollectableDateTime(InitialState init_state = invalid)
        : RWDateTime(init_state) { }

    /**
     * Calls the corresponding constructor of the base class RWDateTime.
     */
    RWCollectableDateTime(const RWDateTime& dt)
        : RWDateTime(dt) { }

    /**
     * Calls the corresponding constructor of the base class RWDateTime.
     */
    explicit RWCollectableDateTime(rwint64 msec)
        : RWDateTime(msec) { }

    /**
     * Calls the corresponding constructor of the base class RWDateTime.
     */
    RWCollectableDateTime(const struct tm* t, unsigned ms = 0,
                          const RWZone& zone = RWZone::local()) :
        RWDateTime(t, ms, zone) {}

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    /**
     * @deprecated As of SourcePro 10, provided for compatibility with
     * code that relies on the deprecated RWTime type.
     */
    RW_DEPRECATE_FUNC("")
    RWCollectableDateTime(const RWTime& t, unsigned ms = 0) :
        RWDateTime(t, ms) {}

    RW_RESTORE_DEPRECATED_WARNINGS
#endif

    /**
     * Calls the corresponding constructor of the base class RWDateTime.
     */
    RWCollectableDateTime(const RWDate& d, unsigned h = 0, unsigned m = 0, unsigned s = 0,
                          unsigned ms = 0, const RWZone& zone = RWZone::local()) :
        RWDateTime(d, h, m, s, ms, zone) { }

    /**
     * Calls the corresponding constructor of the base class RWDateTime.
     */
    RWCollectableDateTime(unsigned d, const char* mo, unsigned y,
                          unsigned h, unsigned m, unsigned s,
                          unsigned ms, const RWLocale& loc = RWLocale::global(),
                          const RWZone& zone = RWZone::local()) :
        RWDateTime(d, mo, y, h, m, s, ms, loc, zone) { }

    /**
     * Calls the corresponding constructor of the base class RWDateTime.
     */
    RWCollectableDateTime(unsigned d, unsigned mo, unsigned y,
                          unsigned h = 0, unsigned m = 0, unsigned s = 0,
                          unsigned ms = 0, const RWZone& zone = RWZone::local()) :
        RWDateTime(d, mo, y, h, m, s, ms, zone) {}

    /**
     * Calls the corresponding constructor of the base class RWDateTime.
     */
    explicit RWCollectableDateTime(const RWCString& str, SetType set_type = setBoth,
                                   const RWLocale& loc = RWLocale::global(),
                                   const RWZone& zone = RWZone::local()) :
        RWDateTime(str, set_type, loc, zone) {}

    /**
     * Calls the corresponding constructor of the base class RWDateTime.
     */
    RWCollectableDateTime(std::istream& s, SetType set_type,
                          const RWLocale& loc = RWLocale::global(),
                          const RWZone& zone = RWZone::local()) :
        RWDateTime(s, set_type, loc, zone) {}

    /**
     * Empty destructor.
     */
    ~RWCollectableDateTime() {}

    // Documented in base class.
    virtual RWspace binaryStoreSize() const {
        return RWDateTime::binaryStoreSize();
    }

    /**
     * Returns the results of calling RWDateTime::compareTo().
     */
    virtual int compareTo(const RWCollectable* c) const {
        RW_PRECONDITION(0 != c);

        const RWCollectableDateTime& ct =
            dynamic_cast<const RWCollectableDateTime&>(*c);

        return RWDateTime::compareTo(ct);
    }

    /**
     * Returns the results of calling RWDateTime::hash().
     */
    virtual unsigned hash() const {
        return RWDateTime::hash();
    }

    /**
     * Returns the results of calling compareTo()
     * for the base class RWDateTime by using appropriate
     * casts.
     */
    virtual bool isEqual(const RWCollectable* c) const {
        RW_PRECONDITION(0 != c);

        if (c->isA() != RWCollectableDateTime::isA()) {
            return false;
        }

        return 0 == compareTo(c);
    }

    // Documented in base class.
    virtual void restoreGuts(RWvistream& stream) {
        RWDateTime::restoreFrom(stream);
    }

    // Documented in base class.
    virtual void restoreGuts(RWFile& file) {
        RWDateTime::restoreFrom(file);
    }

    // Documented in base class.
    virtual void saveGuts(RWvostream& stream) const {
        RWDateTime::saveOn(stream);
    }

    // Documented in base class.
    virtual void saveGuts(RWFile& file) const {
        RWDateTime::saveOn(file);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWCollectableDateTime.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWCollectableDateTime> : std::unary_function<RWCollectableDateTime, size_t> {
    /**
     * Returns a hash of \a obj. This function is the equivalent of calling
     *
     * @code
     * obj.hash();
     * @endcode
     */
    size_t operator()(const RWCollectableDateTime& obj) const {
        return obj.hash();
    }
};


#endif
