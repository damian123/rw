#ifndef RW_TOOLS_INT_H
#define RW_TOOLS_INT_H

/**********************************************************************
 *
 * Declarations for an integer class.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rwint.h#1 $
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

#include <rw/tools/ristream.h>
#include <rw/tools/rostream.h>

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Useful as a base class for classes
 * that use integers as keys in dictionaries, and so forth
 *
 * Integer class.  This class is useful as a base class for classes
 * that use integers as keys in dictionaries, and so forth.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rwint.h>
 * RWInteger i;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Isomorphic
 */
class RW_TOOLS_SYMBOLIC RWInteger
{
    int intBase;

public:

    /**
     * Constructs an RWInteger with value \a i.  Serves as a type conversion
     * from \c int.
     * @note
     * If no argument is supplied, the value defaults to zero (0).
     */
    RWInteger(int i = 0) {
        intBase = i;
    }

    // The following two should not be necessary, but are required to
    // support feeble compilers:
    RWInteger(const RWInteger& i) : intBase(i.intBase) { }

    RWInteger& operator=(const RWInteger& i) {
        intBase = i.intBase;
        return *this;
    }

    bool operator==(const RWInteger& i) const {
        return intBase == i.intBase;
    }

    /**
     * Type conversion to \c int.
     */
    operator int() const {
        return intBase;
    }

    /**
     * Returns the number of bytes necessary to store the object using
     * the global function:
     *
     * RWFile& operator<<(RWFile&, const RWInteger&);
     */
    RWspace binaryStoreSize() const {
        return sizeof(int);
    }

    void restoreFrom(RWvistream& s);
    void restoreFrom(RWFile& f);

    void saveOn(RWvostream& s) const;
    void saveOn(RWFile& f) const;

    /**
     * Returns the value of the RWInteger.
     */
    int value() const {
        return intBase;
    }

    /**
     * Changes the value of the RWInteger to \a newval and returns the
     * old value.
     */
    int value(int newval) {
        int temp = intBase;
        intBase = newval;
        return temp;
    }

    /**
     * Inputs \a x from \b std::istream \a i.
     */
    friend std::istream&
    operator>>(std::istream& i, RWInteger& x) {
        return i >> x.intBase;
    }

};

/**
 * @relates RWInteger
 * Outputs \a x to \b std::ostream \a o.
 */
inline std::ostream&
operator<<(std::ostream& o, const RWInteger& x)
{
    return o << x.value();
}

/**
 * @relates RWInteger
 * Restores an RWInteger into \a x from a virtual stream,
 * replacing the previous contents of \a x.
 */
inline RWvistream&
operator>>(RWvistream& str, RWInteger& x)
{
    x.restoreFrom(str);
    return str;
}

inline RWvistream&
operator>>(RWvistream& str, RWInteger*& x)
{
    x = new RWInteger;
    x->restoreFrom(str);
    return str;
}

/**
 * @relates RWInteger
 * Restores an RWInteger into \a x from an RWFile,
 * replacing the previous contents of \a x.
 */
inline RWFile&
operator>>(RWFile& file, RWInteger& x)
{
    x.restoreFrom(file);
    return file;
}

inline RWFile&
operator>>(RWFile& file, RWInteger*& x)
{
    x = new RWInteger;
    x->restoreFrom(file);
    return file;
}

/**
 * @relates RWInteger
 * Saves the RWInteger \a x to a virtual stream.
 */
inline RWvostream&
operator<<(RWvostream& str, const RWInteger& x)
{
    x.saveOn(str);
    return str;
}

/**
 * @relates RWInteger
 * Saves the RWInteger \a x to an RWFile.
 */
inline RWFile&
operator<<(RWFile& file, const RWInteger& x)
{
    x.saveOn(file);
    return file;
}

#endif /* RW_TOOLS_INT_H */
