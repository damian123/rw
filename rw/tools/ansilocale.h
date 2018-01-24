
/**********************************************************************
 *
 * Declarations for the RWAnsiLocale class
 *
 * RWAnsiLocale is a wrapper around a standard library locale object.
 * This class provides the same behavior as RWLocaleSnapshot (which uses
 * the C locale).
 *
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/ansilocale.h#1 $
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

#ifndef RW_TOOLS_ANSI_LOCALE_H
#define RW_TOOLS_ANSI_LOCALE_H

#include <rw/defs.h>

#include <locale>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#ifdef RW_NO_NEW_HEADER
#  include <time.h>              // System time management.
#else
#  include <ctime>               // System time management.
#endif

#include <rw/zone.h>            // for RWZone::local()
#include <rw/ref.h>             // for RWReference
#include <rw/cstring.h>         // for RWCString
#include <rw/locale.h>          // for RWLocale
#include <rw/mutex.h>           // for RWMutex

// Helper macros
#define _Facet(_Name,_ARG1,_ARG2)   _Name < _ARG1, _ARG2 >

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

class RWAnsiLocaleRef : public RWReference
{
public:

    typedef std::money_base::pattern money_pattern;

public:

    RWAnsiLocaleRef();

    ~RWAnsiLocaleRef();

    // deletes cache if no references exist
    inline void   unLink();

    bool     numericsCached_;

    // Individual Formatting Attributes
    RWCString     locale_name_;
    RWCString     decimal_point_;
    RWCString     thousands_sep_;
    RWCString     grouping_;
    RWCString     int_curr_symbol_;
    RWCString     currency_symbol_;
    RWCString     mon_decimal_point_;
    RWCString     mon_thousands_sep_;
    RWCString     mon_grouping_;
    RWCString     positive_sign_;
    RWCString     negative_sign_;

    char int_frac_digits_;
    char frac_digits_;

    money_pattern pos_fmt_;
    money_pattern neg_fmt_;
    money_pattern int_pos_fmt_;
    money_pattern int_neg_fmt_;

    char          p_cs_precedes_;
    char          p_sep_by_space_;
    char          n_cs_precedes_;
    char          n_sep_by_space_;
    char          p_sign_posn_;
    char          n_sign_posn_;

    // synchronization object for refcounting purposes
    RWMutex       lock_;
};

/**
 * @ingroup internationalization_classes
 *
 * @brief Implements the RWLocale interface, and encapsulates the
 * C++ Standard Library locale.
 *
 * RWAnsiLocale implements the RWLocale interface. It encapsulates
 * the C++ Standard Library locale, \b std::locale and adds
 * functionality for date and time conversions. The C++ Standard
 * Library \b std::locale provides localized representations of dates,
 * numbers, and currencies. RWAnsiLocale offers a simple interface
 * to \b std::locale so you can avoid working directly with \b std::locale
 * facets. Additionally, RWAnsiLocale provides flexible conversions
 * of strings to dates and times. While the C++ Standard Library
 * \b std::locale accepts dates and times only in formats defined by the
 * Standard C Library function \c std::strftime(), RWAnsiLocale accepts
 * multiple date and time formats.
 *
 * There are three ways to use an RWAnsiLocale object:
 *
 * -  By passing the object to functions that expect one, such as
 *    RWDateTime::asString().
 * -  By using the object to specify a global locale using the static
 *    member function RWLocale::global(). This locale is then used
 *    as the default argument to functions that use a locale.
 * -  By imbuing a stream with the object, so that when an RWDateTime
 *    is written to a stream using operator<<(), the appropriate formatting
 *    is used automatically.
 *
 * For a description of the static members of the RWLocale interface
 * that operate on objects of type RWAnsiLocale, see the RWLocale
 * class description.
 *
 * @note
 * For both RWAnsiLocale and \b std::locale, the default global
 * locale is the locale defined in the Standard C Library file
 * \c <locale.h>.
 *
 * @conditional
 * This class is not supported with STLport.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/locale.h>          // for RWLocale
 * RWAnsiLocale loc;     // sets loc to the current global locale
 * RWAnsiLocale loc("")  // sets loc to the default system locale
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/cstring.h>
 * #include <rw/locale.h>
 * #include <rw/tools/datetime.h>
 *
 * #include <iostream>
 *
 * #if defined(_WIN32)
 *    // french should be a valid French locale name on Windows
 * #  define FRENCH_LOCALE_NAME "french"
 * #elif defined(__HP_aCC)
 * #  define FRENCH_LOCALE_NAME "fr_FR.utf8"
 * #else
 *    // fr_FR should be a valid French locale name on most Unices
 * #  define FRENCH_LOCALE_NAME "fr_FR"
 * #endif
 *
 * int main() {
 *
 *   try {
 *     // Create an ansi locale object
 *     RWAnsiLocale frenchLoc(FRENCH_LOCALE_NAME);
 *
 *     // Change global locale
 *     const RWLocale* previousLoc = RWLocale::global(&frenchLoc);
 *
 *     // Print global locale name and the current time in a
 *     // locale specific format
 *     std::cout << RWLocale::global().locale_name() << ' '
 *               << RWDateTime::now().asString('c') << std::endl;
 *
 *     // Restore the global locale
 *     RWLocale::global(previousLoc);
 *   }
 *   catch (...) {
 *     std::cout << "failed to create locale." << std::endl;
 *   }
 *
 *   return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * Actual output will depend on the time the example is run,
 * and on the platform.
 *
 * @code
 * fr_FR 10 juillet 2009 21:24:29 GMT
 * @endcode
 */
class RW_TOOLS_GLOBAL RWAnsiLocale : public RWLocale
{
public:
    // individual formatting attributes
    /**
     * Specifies syntactic elements used to indicate monetary formats,
     * defined as follows:
     */
    enum part {

        /**
         * No grouping separator
         */
        none   = std::money_base::none,

        /**
         * Use space for grouping separator
         */
        space  = std::money_base::space,

        /**
         * Currency symbol
         */
        symbol = std::money_base::symbol,

        /**
         * Sign of monetary value
         */
        sign   = std::money_base::sign,

        /**
         * The monetary value itself
         */
        value  = std::money_base::value
    };

    /**
     * Typedef for a locale-specific rule for formatting monetary outputs.
     */
    typedef std::money_base::pattern money_pattern;

public:

    /**
     * Copy constructor.
     */
    RWAnsiLocale(const RWAnsiLocale&);

    /**
     * Constructs an RWAnsiLocale object using the locale name \a localeName.
     *
     * Valid values for \a localeName are \c NULL, \c "", \c "C", and any
     * value that is accepted by \b std::locale.
     *
     * If the value for \a localeName is not provided or is \c NULL, the
     * required locale information is extracted from the current C++
     * Standard Library global locale. Otherwise, \a localeName is used to
     * construct a C++ Standard Library \b std::locale object to be queried
     * for the necessary locale information.
     *
     * @throw
     * std::runtime_error if the locale name \a localeName is not recognized
     * by \b std::locale.
     */
    RWAnsiLocale(const char* localeName = 0);

    /**
     * Constructs an RWAnsiLocale object from a C++ Standard Library
     * \b std::locale.
     */
    RWAnsiLocale(const std::locale&);

    /**
     * Virtual destructor.
     */
    virtual ~RWAnsiLocale();

    /**
     * Assignment operator.
     */
    RWAnsiLocale& operator= (const RWAnsiLocale&);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a locale.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWAnsiLocale(RWAnsiLocale && locale);

    /**
     * Move assignment. Self takes ownership of the data owned by \a locale.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWAnsiLocale& operator= (RWAnsiLocale && locale);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWAnsiLocale& rhs);

    /**
     * Converts the number \a val to a string (for example, \c "3,456").
     */
    virtual RWCString asString(long val) const;

    /**
     * @copydoc asString(long) const
     */
    virtual RWCString asString(unsigned long val) const;

    /**
     * Converts the \c double \a val to a string. The parameter \a precision
     * is the number of digits to place after the decimal separator.
     * If \a showpoint is \c true, the decimal separator appears regardless
     * of the precision.
     */
    virtual RWCString asString(double val, int precision = 6, bool showpoint = 0) const;

#if !defined(RW_NO_LONG_DOUBLE)
    /**
     * Converts the value of \a val to a string. The parameter \a precision
     * is the number of digits to place after the decimal separator.
     * If showpoint is \c true, the decimal separator appears regardless
     * of the precision.
     */
    virtual RWCString asString(long double val, int precision = 10, bool showpoint = 0) const;
#endif // RW_NO_LONG_DOUBLE

    /**
     * Converts components of the \a ptm object to a string,
     * according to the \a format character. The meanings assigned to the
     * \a format character are identical to those used in the Standard
     * C Library function \c strftime(). The members of \a ptm
     * are assumed to be set consistently.
     *
     * See http://www.opengroup.org/onlinepubs/007908799/xsh/strftime.html
     * for a summary of \c strftime() formatting characters.
     */
    virtual RWCString asString(const struct tm* ptm, char format,
                               const RWZone& zone = RWZone::local()) const;

    /**
     * Converts components of the <tt>tm struct</tt> object \a ptm to a string,
     * according to the format string. Each format character in the
     * \a format string must be preceded by \c %. Any characters not preceded
     * by \c % are treated as ordinary characters which are returned
     * unchanged. You may represent the special character \c % with
     * \c %%. The meanings assigned to the \a format character are
     * identical to those used in the Standard C Library function \c strftime().
     * The members of \a ptm are assumed to be set consistently.
     *
     * See http://www.opengroup.org/onlinepubs/007908799/xsh/strftime.html
     * for a summary of \c strftime() formatting characters.
     */
    RWCString asString(const struct tm* ptm, const char* format,
                       const RWZone& zone = RWZone::local()) const;

    /**
     * Returns a string containing the \a value parameter formatted according
     * to monetary conventions for the locale. The \a value parameter
     * is assumed to contain an integer representing the number of units
     * of currency (for example, <tt>moneyAsString(1000., RWAnsiLocale::cs_local)</tt>
     * in a US locale would yield \c "$10.00"). The \c CurrSymbol parameter
     * determines which currency symbol should be applied if any --
     * the local (for example, \c "$") or international (for example,
     * \c "USD"), or none.
     */
    virtual RWCString moneyAsString(double value,
                                    RWAnsiLocale::CurrSymbol = LOCAL) const;

    virtual RWCString moneyAsString(long double,
                                    RWAnsiLocale::CurrSymbol = LOCAL) const;

    /**
     * Interprets the RWCString parameter value as a monetary value. The
     * currency symbol, if any, is ignored. Negative values may be specified
     * by the negation symbol or by enclosing parentheses. Digit group
     * separators are optional; if present they are checked. Returns
     * \c true for a valid monetary value, \c false for an error. If
     * it returns \c false, the \c double* parameter value is untouched;
     * otherwise it is set to the integral number of monetary units entered
     * (for example, cents in a U.S. locale).
     */
    virtual bool stringToMoney(const RWCString&, double*,
                               RWAnsiLocale::CurrSymbol = LOCAL) const;

    /**
     * Interprets \a date as a date, and extracts the month, day,
     * and year components to the \c tm parameter. Returns \c true for
     * a valid date, \c false otherwise. If it returns \c false, the
     * \c struct \c tm object is untouched; otherwise it sets the
     * \c tm_mday, \c tm_mon, and \c tm_year members. The expected format
     * for the date string is the one specified by the strftime() character
     * \c x in the current locale.
     *
     * An example of a valid string in \c en_US locale is \c 09/12/1990.
     */
    virtual bool stringToDate(const RWCString& date, struct tm*) const;

    /**
     * Interprets \a time as a time, with hours, minutes,
     * and optional seconds. If the hour is in the range [1...12], the
     * local equivalent of "AM" or "PM" is allowed. Returns \c true
     * for a valid time string, \c false for an error. If it returns
     * \c false, the \c tm object is untouched; otherwise it sets
     * the \c tm_hour, \c tm_min, and \c tm_sec members. The expected
     * format for the date string is the one specified by strftime()
     * character \c X (capital X) in the current locale.
     *
     * An example of a valid string in \c en_US locale is \c 10:30:05.
     */
    virtual bool stringToTime(const RWCString& time, struct tm*) const;

    /**
     * Interprets \a value as a floating point number,
     * by reading to the first non-numeric or non-punctuation character,
     * then converting. Spaces are allowed before and after the (optional)
     * sign, and at the end. Digit group separators are allowed in the
     * integer portion. Returns \c true for a valid number, \c false
     * for an error. If it returns \c false, the \c double* parameter
     * is untouched. All valid numeric strings are accepted; all others
     * are rejected. The following are examples of valid numeric strings
     * in an English-speaking locale:
     *
     * @code
     *  "1"              "  -02."          ".3"
     *  "1"              "  -02."          ".3"
     *  "1234.56"        "1e10"            "+  19,876.2E+20"
     *  "1234.56"        "1e10"            "+  19,876.2E+20"
     * @endcode
     */
    virtual bool stringToNum(const RWCString& value, double*) const;

    /**
     * Interprets \a value as an integer, by reading to
     * the first non-numeric or non-punctuation character, then converting.
     * Spaces are allowed before and after the (optional) sign, and
     * at the end. Digit group separators are allowed. Returns \c true
     * for a valid integer, \c false for an error. If it returns
     * \c false, the \c long* parameter is untouched. All valid numeric
     * strings are accepted; all others are rejected. The following
     * are examples of valid integral strings in an English-speaking
     * locale:
     *
     * @code
     *  "1"                "  -02."          "+  1,234"
     *  "1"                "  -02."          "+  1,234"
     *  "1234545"          "1,234,567"
     *  "1234545"          "1,234,567"
     * @endcode
     */
    virtual bool stringToNum(const RWCString& value, long*) const;

    /**
     * Interprets \a value as an integer, by reading to
     * the first non-numeric or non-punctuation character, then converting.
     * Spaces are allowed before and after the (optional) sign, and
     * at the end. Digit group separators are allowed. Returns \c true
     * for a valid integer, \c false for an error. If it returns
     * \c false, the \c long* parameter is untouched. All valid numeric
     * strings are accepted; all others are rejected.
     */
    virtual bool stringToNum(const RWCString& value, unsigned long*) const;

    /**
     * Interprets \a month as a full or abbreviated month name,
     * returning values 1 through 12 to represent (respectively) January
     * through December, or 0 for an error. Ignores leading white space.
     */
    virtual int monthIndex(const RWCString& month) const;

    /**
     * Interprets \a day as a full or abbreviated weekday name,
     * returning values 1 through 7 to represent (respectively) Monday
     * through Sunday, or 0 for an error.
     */
    virtual int weekdayIndex(const RWCString& day) const;

    /**
     * Returns a pointer to the encapsulated \b std::locale.
     */
    virtual std::locale* std();

    /**
     * Returns the name of the encapsulated \b std::locale.
     */
    virtual const RWCString&  locale_name() const;

    /**
     * Returns a string to use as the numerical decimal separator.
     */
    virtual const RWCString&  decimal_point() const;

    /**
     * Returns a \c char to use as the numerical thousands separator.
     */
    virtual const RWCString&  thousands_sep() const;

    /**
     * Returns a string identifying the number of digits to be included
     * in a numerical group. A group is simply the digits between adjacent
     * thousand's separators.
     *
     * Each character of the string is an integer that specifies the
     * number of digits in a group, starting with the right most group.
     *
     * -  If the value is equal to \c 0, the previous element is used repeatedly.
     * -  If the value is equal to \c CHAR_MAX, no further grouping is
     *    performed.
     *
     * <i>Example</i>
     *
     * @code
     * "\3"       // Specifies the United States grouping system.
     *            // For example, "1,000,000". The "\00" acts as an
     *            // escape
     * "\3\2"     // Specifies the Nepalese grouping system.
     *            // For example, "1,00,00,000"
     * @endcode
     */
    virtual const RWCString&  grouping() const;

    /**
     * Returns an RWCString to use as the international currency symbol.
     */
    virtual const RWCString&  int_curr_symbol() const;

    /**
     * Returns a string to use as the local currency symbol.
     */
    virtual const RWCString&  currency_symbol() const;

    /**
     * Returns a string to use as the monetary decimal separator.
     */
    virtual const RWCString&  mon_decimal_point() const;

    /**
     * Returns a string to use as the monetary thousands separator.
     */
    virtual const RWCString&  mon_thousands_sep() const;

    /**
     * Returns a string identifying the number of digits to be included
     * in a numerical group. A group is simply the digits between adjacent
     * thousand's separators.
     *
     * Each character of the string is an integer that specifies the
     * number of digits in a group, starting with the right most group.
     *
     * -  If the value is equal to \c 0, the previous element is used repeatedly.
     * -  If the value is equal to \c CHAR_MAX, no further grouping is
     *    performed.
     *
     * See the example in the grouping() description.
     */
    virtual const RWCString&  mon_grouping() const;

    /**
     * Returns a string to use as the positive sign for a monetary quantity.
     */
    virtual const RWCString&  positive_sign() const;

    /**
     * Returns a string to use as the negative sign for a monetary quantity.
     */
    virtual const RWCString&  negative_sign() const;

    /**
     * Returns the number of digits in the fractional part of a particular
     * national monetary representation.
     */
    virtual char int_frac_digits() const;

    /**
     * Returns the number of digits in the fractional part of the monetary
     * representation.
     */
    virtual char frac_digits() const;

    /**
     * Returns a #money_pattern object specifying the location of the various
     * syntactic elements in a monetary representation. The enumeration
     * values #symbol, #sign, and #value appear exactly once in
     * this pattern, with the remaining location taken by either
     * #none or #space. #none never occupies the first position
     * in the #money_pattern and #space never occupies the first or last
     * position. Beyond these restrictions, elements may appear in any
     * order.
     */
    virtual money_pattern pos_format() const;

    /**
     * @copydoc pos_format() const
     */
    virtual money_pattern neg_format() const;

    /**
     * Returns a #money_pattern object specifying the location of the various
     * syntactic elements in an international monetary representation.
     * The restrictions listed for pos_format() and neg_format() apply.
     */
    virtual money_pattern int_pos_format() const;

    /**
     * @copydoc int_pos_format() const
     */
    virtual money_pattern int_neg_format() const;

    /**
     * Gets the positive currency symbol location.
     */
    virtual char p_cs_precedes() const;

    /**
     * Gets the space separating the positive sign from its number,
     * if any.
     */
    virtual char p_sep_by_space() const;

    /**
     * Gets the negative currency symbol location.
     */
    virtual char n_cs_precedes() const;

    /**
     * Gets the space separating the negative sign from its number,
     * if any.
     */
    virtual char n_sep_by_space() const;

    /**
     * Gets the positive sign position.
     */
    virtual char p_sign_posn() const;

    /**
     * Gets the negative sign position.
     */
    virtual char n_sign_posn() const;

    RWAnsiLocaleRef* ref() const {
        return ref_;
    }

protected:
    virtual void initNumerics() const;
    void getMoneyPunctData() const;

    RWCString do_getMonthName(const int&, const char) const;
    RWCString do_getWeekdayName(const int&, const char) const;

    RWCString getMonthName(const int& n) const {
        return do_getMonthName(n, 'B');
    }

    RWCString getMonthAbbr(const int& n) const {
        return do_getMonthName(n, 'b');
    }

    RWCString getWeekdayName(const int& n) const {
        return do_getWeekdayName(n, 'A');
    }

    RWCString getWeekdayAbbr(const int& n) const {
        return do_getWeekdayName(n, 'a');
    }

    // encapsulated std::locale
    std::locale loc_;

    // data cache (reference counted)
    RWAnsiLocaleRef* ref_;
};

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif /*RW_TOOLS_ANSI_LOCALE_H*/
