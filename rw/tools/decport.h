#ifndef __RWDECPORT_TOOLS_H__
#define __RWDECPORT_TOOLS_H__



/**********************************************************************
 *
 * tools/decport.h -- RWDecimalPortable: portable decimal representation,
 *                    used for I/O and as an intermediate in conversions
 * This class represents an arbitrary precision decimal fraction.  The
 * representation is not exposed via member functions for accessing the
 * strings representing the pieces before and after the decimal.  This
 * is done because it might make sense to one day replace this implementation
 * with an arbitrary precision integer based implementation.
 * The concept of trailing zeros after the decimal place is supported. If
 * you construct an RWDecimalPortable using the string constructor and have
 * trailing zeros (eg 1.200), then you are guaranteed that the string conversion
 * operator will return a string with these trailing zeros.  This is necessary
 * for correct conversions between fixed decimal types in Money.h++.  Also,
 * rounding supports the concept of trailing zeros, so round("1.2",2) yields
 * 1.20.  The math functions (+-*) truncate trailing zeros.  Use trimZeros()
 * to strip off the excess zeros.  Note that because trailing zeros are
 * allowed the relation x==y does not guarantee that RWCString(x)==RWCString(y)
 * since "1.200"=="1.2".
 * Input note: for details on allowable input syntax, see RWDecimalParser.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/decport.h#1 $
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

#include <stdlib.h>
#include <locale.h>
#include <errno.h>

#include <rw/defs.h>
#include <rw/edefs.h> // for rw_swap
#include <rw/locale.h>
#include <rw/cstring.h>
#include <rw/decbase.h>
#include <rw/tools/hash.h>

#include <math.h>
#include <limits>

class RW_TOOLS_SYMBOLIC RWDecimalPortable;

/**
 * @relates RWDecimalPortable
 * Returns an RWDecimalPortable which is the result of rounding
 * self according to the applied specifications.
 */
RW_TOOLS_SYMBOLIC RWDecimalPortable
round(const RWDecimalPortable& dp, RWssize_t digits,
      RWDecimalBase::RoundingMethod method = RWDecimalBase::PLAIN);

/**
 * @ingroup miscellaneous_classes decimal
 * @brief Represents an arbitrary precision decimal fraction, providing
 * maximum portability.
 *
 * RWDecimalPortable represents an arbitrary precision decimal fraction.
 *
 * The RWDecimalPortable class is designed to provide maximum portability.
 * However, its portability is gained at the expense of speed and power.
 * For example, RWDecimalPortable does not support all
 * of the functionality of the other decimal classes in the Currency Module.
 * In most cases we recommend that you use another of the decimal classes
 * from the Currency Module, but in situations in which the portability of
 * a number is paramount, RWDecimalPortable is useful.
 *
 * When constructing an RWDecimalPortable from a string, the constructor
 * parses the initial characters of the string until the first non-digit,
 * behaving similarly to Standard C Library's \c strtod() function.
 * For example, a string \c "12E3" creates an RWDecimalPortable value
 * of 12000.
 *
 * If an RWDecimalPortable is provided an invalid string, either
 * in the constructor or through assignment, it <i>does not</i>
 * throw an exception. Use isNumber() to determine if the object
 * is valid.
 *
 * @code
 *
 * RWDecimalPortable a("555AAA");
 * a.isNumber(); // returns true
 *
 * RWDecimalPortable b;
 * b = "A555AAA";
 * b.isNumber(); // returns false (NAN)
 * @endcode
 *
 * Note that:
 *
 * -  The validity or non-validity of the provided string is locale-dependent,
 *    as discussed below in the description for the constructor that
 *    takes a string, \link RWDecimalPortable(const  char *)
 *    RWDecimalPortable(const char *input) \endlink .
 * -  In the first example above, the likely value of the RWDecimalPortable
 *    would be 555. That is, the string parser parses until it encounters
 *    an invalid character, and constructs the number from the characters
 *    up to that point.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tools/decport.h>
 *
 * RWDecimalPortable dec;     // construct a decimal == 0
 * dec = "1.23";              // construct a decimal == 1.23
 * @endcode
 */
class RW_TOOLS_SYMBOLIC RWDecimalPortable : public RWDecimalBase
{
public:

    /**
     * The default constructor creates a value initialized to zero (0).
     */
    RWDecimalPortable() :  RWDecimalBase(normal), negative_(false) { }

    /**
     * Parses the initial portion of \a input formatted as a decimal
     * number up to the end of the string, or the first non-decimal
     * character, whichever comes first. The parse behavior is locale-dependent
     * (for decimal points, thousands separator, grouping, etc).  If
     * no locale is specified, the default locale is used, and parsing
     * behavior will be defined by that locale, creating possibly unexpected
     * results. To ensure predictable and expected behavior, the program
     * should explicitly specify the locale before using this constructor,
     * and input strings should be formatted according the numeric string
     * facets for that locale.
     *
     * For more information on conversions between strings and numbers,
     * see the section on numbers in the internationalization chapter of
     * the <i>Essential Tools Module User's Guide</i>.
     */
    RWDecimalPortable(const char* input);

    /**
     * Constructs a value with \a intPart as the part before the decimal
     * point.
     */
    RWDecimalPortable(long intPart);

    RWDecimalPortable(int);

    /**
     * Copy constructor. The constructed instance is a copy of \a dp.
     */
    RWDecimalPortable(const RWDecimalPortable& dp);

    /**
     * Assignment operator. Self is made a copy of \a dp.
     */
    RWDecimalPortable& operator= (const RWDecimalPortable& dp);

#if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a dp.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWDecimalPortable(RWDecimalPortable && dp);

    /**
     * Move assignment. Self takes ownership of the data owned by \a dp.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWDecimalPortable& operator= (RWDecimalPortable && dp);

#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWDecimalPortable& rhs);

    // format is, eg, -23.21 or 0.32 or 432
    /**
     * Returns self as an RWCString. Format is, for
     * example -23.21 or 0.32 or 432.
     */
    operator  RWCString() const;

    /**
     * Returns self as an RWCString using the global locale as determined
     * by the static member function RWLocale::global() for numeric formatting.
     * For example, an English locale could return strings such as -23.21,
     * 0.32, or 432.
     */
    RWCString asString() const;

    /**
     * Returns self as an RWCString using the provided RWLocale instance
     * for numeric formatting. For example, an English locale could
     * return strings such as -23.21, 0.32, or 432.
     */
    RWCString asString(const RWLocale& loc) const;

    bool isZero() const;

    // Number of digits. Does not count leading
    // 0's (for numbers < 1), minus signs or
    // decimal points.
    size_t numSignificantDigits() const;

    // precision is to total number of digits in the number
    size_t precision() const {
        return(before_.length() + after_.length());
    }
    // scale is the number of digits to the
    // right of the decimal point.
    size_t scale() const {
        return after_.length();
    }

    /**
     * Returns \c false if self represents a \c NaN, a missing value, infinity
     * or a \c NULL; otherwise returns \c true.
     * @note In this instance, the mathematical rather than the RDBMS meaning
     * of NULL is intended. Please see RWDBNullIndicator.
     */
    bool isNumber() const {
        return state_ == normal;
    }

    State state() const {
        return state_;
    }

    void setState(State);

    /**
     * Removes leading zeros before self's decimal point and trailing
     * zeros after.
     */
    void trimZeros();

    static RWCString decimalSeparator(const RWLocale&);
    static RWCString thousandsSeparator(const RWLocale&);
    static RWCString decimalSeparator();
    static RWCString thousandsSeparator();

    // RWCollectable compliant member functions
    /**
     * Returns the number of bytes required to store self using the
     * saveOn() methods.
     */
    size_t    binaryStoreSize() const;

    /**
     * Returns a value suitable for hashing.
     */
    unsigned  hash() const;

    /**
     * Reads \a vis, replacing the contents of self.
     */
    void      restoreFrom(RWvistream& vis);

    /**
     * Reads \a file, replacing the contents of self.
     */
    void      restoreFrom(RWFile& file);

    /**
     * Writes the contents of self to \a vos.
     */
    void      saveOn(RWvostream& vos) const;

    /**
     * Writes the contents of self to \a file.
     */
    void      saveOn(RWFile& file) const;

    // Additional Arithmetic operations
    const RWDecimalPortable& operator-=(const RWDecimalPortable& rhs);
    const RWDecimalPortable& operator+=(const RWDecimalPortable& rhs);
    const RWDecimalPortable& operator*=(const RWDecimalPortable& rhs);

private:

    bool
    preparse(const char*);

    // Private constructor
    RWDecimalPortable(State s): negative_(false) {
        state_ = s;
    }

    // implementation functions
    static RWDecimalPortable plus(const RWDecimalPortable&,
                                  const RWDecimalPortable&);
    static RWDecimalPortable minus(const RWDecimalPortable&,
                                   const RWDecimalPortable&);
    static bool asciiIsBiggerOrEqual(const RWCString&, const RWCString&);
    static RWCString unsignedLongToString(unsigned long);
    static RWCString asciiAdd(const RWCString&,
                              const RWCString&,
                              bool&);
    static RWCString asciiSubtract(const RWCString&, const RWCString&);
    static RWCString asciiMultiply(const RWCString&,
                                   const RWCString&,
                                   bool&);
    static RWCString asciiMultiply(const RWCString&, const char);


    void zeroNormalize();

    // helper classes
    friend class RW_TOOLS_GLOBAL RWDecimalParserBase;
    friend class RW_TOOLS_GLOBAL RWDecimalParser;
    friend class RW_TOOLS_GLOBAL RWDecimalStringParser;
    friend class RW_TOOLS_SYMBOLIC RWDecimalPortableInit;

    // helper functions
    friend RW_TOOLS_SYMBOLIC RWDecimalPortable
    round(const RWDecimalPortable&, RWssize_t, RoundingMethod);

    friend RW_TOOLS_SYMBOLIC double
    toDouble(const RWDecimalPortable&);

#if !defined(RW_NO_LONG_DOUBLE)
    friend RW_TOOLS_SYMBOLIC long double
    toLongDouble(const RWDecimalPortable&);
#endif // RW_NO_LONG_DOUBLE

    // Rudimentary math functions.  These are not very efficient.
    // For efficiency, use one of the decimal classes of Money.h++
    // These are very useful because they provide a
    // second implementation of arithmetic which can be used for testing
    // purposes with the RWDecimal classes.  They could also be used as a
    // poor man's arbitrary precision decimal class.

    friend RW_TOOLS_SYMBOLIC
    RWDecimalPortable operator-(const RWDecimalPortable&);

    friend RW_TOOLS_SYMBOLIC
    RWDecimalPortable operator+(const RWDecimalPortable&,
                                const RWDecimalPortable&);

    friend RW_TOOLS_SYMBOLIC
    RWDecimalPortable operator*(const RWDecimalPortable&,
                                const RWDecimalPortable&);

    // Relational operators.
    // The rest are inlines that are defined in terms of these
    friend RW_TOOLS_SYMBOLIC
    bool operator==(const RWDecimalPortable& x,
                    const RWDecimalPortable& y);

    friend RW_TOOLS_SYMBOLIC
    bool operator<(const RWDecimalPortable& x,
                   const RWDecimalPortable& y);

    bool negative_; // true indicates number less than zero
    RWCString before_;   // digits before the decimal
    RWCString after_;    // digits after the decimal
};


inline
RWDecimalPortable::RWDecimalPortable(long before)
    : RWDecimalBase(normal), negative_(false)
{
    if (before < 0) {
        negative_ = true;
        before = -before;
    }
    before_ = unsignedLongToString((unsigned long)before);
}

inline
RWDecimalPortable::RWDecimalPortable(int before) :
    RWDecimalBase(normal), negative_(false)
{
    if (before < 0) {
        negative_ = true;
        before = -before;
    }
    before_ = unsignedLongToString((unsigned long)before);
}

inline void
RWDecimalPortable::swap(RWDecimalPortable& rhs)
{
    // swap base
    RWDecimalBase::swap(rhs);

    // swap members
    rw_swap(rhs.negative_, negative_);
    before_.swap(rhs.before_);
    after_.swap(rhs.after_);
}

inline void
RWDecimalPortable::setState(State s)
{
    state_ = s;
    if (state_ != normal) {
        before_ = after_ = "";
        if (state_ != infinitystate) {
            negative_ = false;
        }
    }
}

/**************************************************************************/

inline RWCString
RWDecimalPortable::decimalSeparator(const RWLocale& loc)
{
    const RWCString& sep = loc.mon_decimal_point();
    if (sep.isNull() && loc.mon_thousands_sep() != ".") {
        return RWCString('.', 1);
    }
    else {
        return sep;
    }
}

inline RWCString
RWDecimalPortable::thousandsSeparator(const RWLocale& loc)
{
    const RWCString& sep = loc.mon_thousands_sep();
    if (sep.isNull() && loc.mon_decimal_point() != ",") {
        return RWCString(',', 1);
    }
    else {
        return sep;
    }
}

inline RWCString
RWDecimalPortable::decimalSeparator()
{
    return decimalSeparator(RWLocale::global());
}

inline RWCString
RWDecimalPortable::thousandsSeparator()
{
    return thousandsSeparator(RWLocale::global());
}

/**************************************************************************/

inline size_t
RWDecimalPortable::numSignificantDigits() const
{
    return before_.strip(RWCString::leading, '0').length() +
           after_.length();
}

inline void
RWDecimalPortable::trimZeros()
{
    if (after_.length() > 0 && after_(after_.length() - 1) == '0') {
        after_ = after_.strip(RWCString::trailing, '0');
    }
    if (before_.length() > 0 && before_(0) == '0') {
        before_ = before_.strip(RWCString::leading, '0');
    }
}

// RWCollectable compliant member functions
inline size_t
RWDecimalPortable::binaryStoreSize() const
{
    return sizeof(int)
           + before_.binaryStoreSize()
           + after_.binaryStoreSize()
           + sizeof(int);
}


inline unsigned
RWDecimalPortable::hash() const
{
    unsigned long xreturn = (unsigned long)before_.hash() + after_.hash();
    if (negative_) {
        xreturn = ~xreturn + 1;    // two's complement. Same as negating.
    }

    return unsigned(xreturn);
}

inline RWCString
RWDecimalPortable::asString() const
{
    return asString(RWLocale::global());
}

inline
RWDecimalPortable::operator RWCString() const
{
    return asString(RWLocale::global());
}

inline bool
RWDecimalPortable::isZero() const
{
    if (state_ == normal) {
        if ((before_.strip(RWCString::leading, '0').length() == 0) &&
                (after_.strip(RWCString::trailing, '0').length() == 0)) {
            return true;
        }
    }
    return false;
}

inline void
RWDecimalPortable::zeroNormalize()
{
    if (isZero() ||
            (state_ != normal && state_ != infinitystate)) {
        negative_ = false;
    }
}


// RWDecimal Global Functions

/**
 * @relates RWDecimalPortable
 * Returns a double which approximates the value of \a dp.
 */
RW_TOOLS_SYMBOLIC double
toDouble(const RWDecimalPortable& dp);

/**
 * @relates RWDecimalPortable
 * Returns a long double which approximates the value of \a dp.
 */
RW_TOOLS_SYMBOLIC long double
toLongDouble(const RWDecimalPortable& dp);


// RWDecimal Global Operator Functions

// I/O operator declarations

/**
 * @relates RWDecimalPortable
 * Writes an RWDecimalPortable to an output stream.
 */
RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& ostrm, const RWDecimalPortable& x);

//  defined  decin.cpp
/**
 * @relates RWDecimalPortable
 * Reads an RWDecimalPortable from an input stream.
 */
RW_TOOLS_SYMBOLIC std::istream&
operator>>(std::istream& strm, RWDecimalPortable& n);

inline RWvostream&
operator<<(RWvostream& strm, const RWDecimalPortable& n)
{
    n.saveOn(strm);
    return strm;
}

inline RWvistream&
operator>>(RWvistream& strm, RWDecimalPortable& n)
{
    n.restoreFrom(strm);
    return strm;
}


inline RWFile&
operator<<(RWFile& strm, const RWDecimalPortable& n)
{
    n.saveOn(strm);
    return strm;
}


inline RWFile&
operator>>(RWFile& strm, RWDecimalPortable& n)
{
    n.restoreFrom(strm);
    return strm;
}

// Arithmetic Operators

/**
 * @relates RWDecimalPortable
 * Rudimentary addition operator with minimal efficiency. For
 * efficiency, use one of the decimal classes of the
 * SourcePro Analysis product's Currency Module.
 */
RW_TOOLS_SYMBOLIC RWDecimalPortable
operator+(const RWDecimalPortable& x, const RWDecimalPortable& y);

/**
 * @relates RWDecimalPortable
 * Rudimentary multiplication operator with minimal efficiency. For
 * efficiency, use one of the decimal classes of the SourcePro
 * Analysis product's Currency Module.
 */
RW_TOOLS_SYMBOLIC RWDecimalPortable
operator*(const RWDecimalPortable& x, const RWDecimalPortable& y);

/**
 * @relates RWDecimalPortable
 * Rudimentary negation operator with minimal efficiency. For
 * efficiency, use one of the decimal classes of the SourcePro
 * Analysis product's Currency Module.
 */
inline RWDecimalPortable
operator-(const RWDecimalPortable& x)
{
    RWDecimalPortable y = x;
    y.negative_ = !x.negative_;
    return y;
}

/**
 * @relates RWDecimalPortable
 * Rudimentary subtraction operator with minimal efficiency. For
 * efficiency, use one of the decimal classes of the SourcePro
 * Analysis product's Currency Module.
 */
inline RWDecimalPortable
operator-(const RWDecimalPortable& x, const RWDecimalPortable& y)
{
    return x + (-y);
}

inline const RWDecimalPortable&
RWDecimalPortable::operator-=(const RWDecimalPortable& rhs)
{
    *this = operator-(*this, rhs);
    return *this;
}

inline const RWDecimalPortable&
RWDecimalPortable::operator+=(const RWDecimalPortable& rhs)
{
    *this = operator+(*this, rhs);
    return *this;
}

inline const RWDecimalPortable&
RWDecimalPortable::operator*=(const RWDecimalPortable& rhs)
{
    *this = operator*(*this, rhs);
    return *this;
}

// Relation operator declarations and inline definitions

/**
 * @relates RWDecimalPortable
 * Relational operator. Returns \c true if \a x is equal to \a y.
 */
RW_TOOLS_SYMBOLIC bool operator==(const RWDecimalPortable& x,
                                  const RWDecimalPortable& y);


/**
 * @relates RWDecimalPortable
 * Relational operator. Returns \c true if \a x is less than \a y.
 *
 * @note
 * If the \link RWDecimalBase::State state \endlink of either one of the operands
 * is anything other than \link RWDecimalBase::normal normal \endlink or
 * \link RWDecimalBase::infinitystate infinitystate \endlink, the result of
 * the operator is undefined.
 */
RW_TOOLS_SYMBOLIC bool operator<(const RWDecimalPortable& x,
                                 const RWDecimalPortable& y);

// >, >=, <=, !=, in terms of == and <

/**
 * @relates RWDecimalPortable
 * Relational operator. Returns \c true if \a x is not equal to \a y.
 */
inline bool
operator!=(const RWDecimalPortable& x,  const RWDecimalPortable& y)
{
    return !(x == y);
}

/**
 * @relates RWDecimalPortable
 * Relational operator. Returns \c true if \a x is greater than or equal
 * to \a y.
 *
 * @note
 * If the \link RWDecimalBase::State state \endlink of either one of the operands
 * is anything other than \link RWDecimalBase::normal normal \endlink or
 * \link RWDecimalBase::infinitystate infinitystate \endlink, the result of
 * the operator is undefined.
 */
inline bool
operator>=(const RWDecimalPortable& x, const RWDecimalPortable& y)
{
    if ((x.isNumber() || x.state() == RWDecimalBase::infinitystate) &&
            (y.isNumber() || y.state() == RWDecimalBase::infinitystate)) {
        return !(x < y);
    }
    return false;
}

/**
 * @relates RWDecimalPortable
 * Relational operator. Returns \c true if \a x is greater than \a y.
 *
 * @note
 * If the \link RWDecimalBase::State state \endlink of either one of the operands
 * is anything other than \link RWDecimalBase::normal normal \endlink or
 * \link RWDecimalBase::infinitystate infinitystate \endlink, the result of
 * the operator is undefined.
 */
inline bool
operator>(const RWDecimalPortable& x, const RWDecimalPortable& y)
{
    return (y < x);
}

/**
 * @relates RWDecimalPortable
 * Relational operator. Returns \c true if \a x is less than or equal
 * to \a y.
 *
 * @note
 * If the \link RWDecimalBase::State state \endlink of either one of the operands
 * is anything other than \link RWDecimalBase::normal normal \endlink or
 * \link RWDecimalBase::infinitystate infinitystate \endlink, the result of
 * the operator is undefined.
 */
inline bool
operator<=(const RWDecimalPortable& x, const RWDecimalPortable& y)
{
    if ((x.isNumber() || x.state() == RWDecimalBase::infinitystate) &&
            (y.isNumber() || y.state() == RWDecimalBase::infinitystate)) {
        return !(y < x);
    }
    return false;
}

// These exact match operators are provided to prevent
// ambiguities involving the the conversion operator
// to RWCString

// operator ==
inline bool operator==(const RWDecimalPortable& x,
                       const RWCString& y)
{
    return x == RWDecimalPortable(y.data());
}

inline bool operator==(const RWCString& x,
                       const RWDecimalPortable& y)
{
    return RWDecimalPortable(x.data()) == y;
}

inline bool operator==(const RWDecimalPortable& x,
                       const int& y)
{
    return x == RWDecimalPortable(y);
}

inline bool operator==(const int& x,
                       const RWDecimalPortable& y)
{
    return RWDecimalPortable(x) == y;
}

inline bool operator==(const RWDecimalPortable& x,
                       const long& y)
{
    return x == RWDecimalPortable(y);
}

inline bool operator==(const long& x,
                       const RWDecimalPortable& y)
{
    return RWDecimalPortable(x) == y;
}


// operator <
inline bool operator<(const RWDecimalPortable& x,
                      const RWCString& y)
{
    return x < RWDecimalPortable(y.data());
}

inline bool operator<(const RWCString& x,
                      const RWDecimalPortable& y)
{
    return RWDecimalPortable(x.data()) < y;
}

inline bool operator<(const RWDecimalPortable& x,
                      const int& y)
{
    return x < RWDecimalPortable(y);
}

inline bool operator<(const int& x,
                      const RWDecimalPortable& y)
{
    return RWDecimalPortable(x) < y;
}

inline bool operator<(const RWDecimalPortable& x,
                      const long& y)
{
    return x < RWDecimalPortable(y);
}

inline bool operator<(const long& x,
                      const RWDecimalPortable& y)
{
    return RWDecimalPortable(x) < y;
}


// operator >
inline bool operator>(const RWDecimalPortable& x,
                      const RWCString& y)
{
    return x > RWDecimalPortable(y.data());
}

inline bool operator>(const RWCString& x,
                      const RWDecimalPortable& y)
{
    return RWDecimalPortable(x.data()) > y;
}

inline bool operator>(const RWDecimalPortable& x,
                      const int& y)
{
    return x > RWDecimalPortable(y);
}

inline bool operator>(const int& x,
                      const RWDecimalPortable& y)
{
    return RWDecimalPortable(x) > y;
}

inline bool operator>(const RWDecimalPortable& x,
                      const long& y)
{
    return x > RWDecimalPortable(y);
}

inline bool operator>(const long& x,
                      const RWDecimalPortable& y)
{
    return RWDecimalPortable(x) > y;
}

// operator >=
inline bool operator>=(const RWDecimalPortable& x,
                       const RWCString& y)
{
    return x >= RWDecimalPortable(y.data());
}

inline bool operator>=(const RWCString& x,
                       const RWDecimalPortable& y)
{
    return RWDecimalPortable(x.data()) >= y;
}

inline bool operator>=(const RWDecimalPortable& x,
                       const int& y)
{
    return x >= RWDecimalPortable(y);
}

inline bool operator>=(const int& x,
                       const RWDecimalPortable& y)
{
    return RWDecimalPortable(x) >= y;
}

inline bool operator>=(const RWDecimalPortable& x,
                       const long& y)
{
    return x >= RWDecimalPortable(y);
}

inline bool operator>=(const long& x,
                       const RWDecimalPortable& y)
{
    return RWDecimalPortable(x) >= y;
}


// operator <=
inline bool operator<=(const RWDecimalPortable& x,
                       const RWCString& y)
{
    return x <= RWDecimalPortable(y.data());
}

inline bool operator<=(const RWCString& x,
                       const RWDecimalPortable& y)
{
    return RWDecimalPortable(x.data()) <= y;
}

inline bool operator<=(const RWDecimalPortable& x,
                       const int& y)
{
    return x <= RWDecimalPortable(y);
}

inline bool operator<=(const int& x,
                       const RWDecimalPortable& y)
{
    return RWDecimalPortable(x) <= y;
}

inline bool operator<=(const RWDecimalPortable& x,
                       const long& y)
{
    return x <= RWDecimalPortable(y);
}

inline bool operator<=(const long& x,
                       const RWDecimalPortable& y)
{
    return RWDecimalPortable(x) <= y;
}


// operator !=
inline bool operator!=(const RWDecimalPortable& x,
                       const RWCString& y)
{
    return x != RWDecimalPortable(y.data());
}

inline bool operator!=(const RWCString& x,
                       const RWDecimalPortable& y)
{
    return RWDecimalPortable(x.data()) != y;
}

inline bool operator!=(const RWDecimalPortable& x,
                       const int& y)
{
    return x != RWDecimalPortable(y);
}

inline bool operator!=(const int& x,
                       const RWDecimalPortable& y)
{
    return RWDecimalPortable(x) != y;
}

inline bool operator!=(const RWDecimalPortable& x,
                       const long& y)
{
    return x != RWDecimalPortable(y);
}

inline bool operator!=(const long& x,
                       const RWDecimalPortable& y)
{
    return RWDecimalPortable(x) != y;
}

template <typename T>
inline void stateConvert(const RWDecimalBase::State state, T& retVal)
{
    switch (state) {
        case RWDecimalBase::nullstate:
        case RWDecimalBase::ctorError:
        case RWDecimalBase::missingstate:
        case RWDecimalBase::NaNstate:
            retVal = std::numeric_limits<T>::quiet_NaN();
            break;
        case RWDecimalBase::SNaNstate:
            retVal = std::numeric_limits<T>::signaling_NaN();
            break;
        case RWDecimalBase::infinitystate:
            retVal = std::numeric_limits<T>::infinity();
            break;
        default:
            break;
    }
}

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWDecimalPortable.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWDecimalPortable> : public std::unary_function<RWDecimalPortable, size_t> {
    /**
     * Returns a hash of \a obj. This function is the equivalent of calling
     *
     * @code
     * obj.hash();
     * @endcode
     */
    size_t operator()(const RWDecimalPortable& obj) const {
        return obj.hash();
    }
};

#endif // __RWDECPORT_TOOLS_H__
