#ifndef RW_TOOLS_LOCALE_H
#define RW_TOOLS_LOCALE_H



/**********************************************************************
 *
 * Declarations for the RWLocale class
 * Classes RWLocale and RWLocaleSnapshot support encapsulation of local
 * formatting conventions.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/locale.h#1 $
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

#if !defined(RW_NO_LOCALE)
#  include <locale.h>
#endif

#include <time.h>               // for struct tm and time_t 
#include <rw/cstring.h>         // for RWCString 
#include <rw/zone.h>            // for RWZone::local() 

// RWLocale is an abstract base class defining an interface to
// locale-dependent programming facilities.

/**
 * @ingroup internationalization_classes
 *
 * @brief Defines an interface that
 * formats the conversion of strings to and from dates and times
 *
 * RWLocale is an abstract base class. It defines an interface that
 * formats the conversion of strings to and from dates and times.
 * It formats dates (including day and month names), times, currency,
 * and numbers (including digit grouping).
 *
 * Note that because it is an \e abstract base class, there
 * is no way to actually enforce these goals  --  the description
 * here is merely the model of how a class derived from RWLocale
 * should act.
 *
 * There are three ways to use an RWLocale object:
 *
 * -  By passing the object to functions which expect one, such as
 *    RWDate::asString().
 * -  By specifying a "global" locale using the static member function
 *    RWLocale::global(const RWLocale*). This locale is passed as the
 *    default argument to functions that use a locale.
 * -  By "imbuing" a stream with the object, so that when an RWDateTime
 *    or RWDate is written to a stream using operator<<(), the
 *    appropriate formatting is automatically used.
 *
 * The library provides two main implementations of RWLocale. These
 * subclasses each allow more than one locale to be active at a
 * time, and they also support conversion from strings to other
 * types. They include:
 *
 * -  Class RWAnsiLocale which encapsulates the Standard C++ Library locale,
 *    \b std::locale.
 * -  Class RWLocaleSnapshot which encapsulates the Standard C Library
 *    locale facility.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <locale.h>
 * #include <rw/locale.h>
 * (Abstract base class)
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWLocale
{
public:
    virtual ~RWLocale();

    // Onyx 13421
    // Name of the locale "snapped"
    /**
     * Returns the name of the encapsulated locale.
     */
    virtual const RWCString& locale_name() const = 0;

    virtual const RWCString& decimal_point() const = 0;
    virtual const RWCString& thousands_sep() const = 0;
    virtual const RWCString& grouping() const = 0;
    virtual const RWCString& int_curr_symbol() const = 0;
    virtual const RWCString& currency_symbol() const = 0;
    virtual const RWCString& mon_decimal_point() const = 0;
    virtual const RWCString& mon_thousands_sep() const = 0;
    virtual const RWCString& mon_grouping() const = 0;
    virtual const RWCString& positive_sign() const = 0;
    virtual const RWCString& negative_sign() const = 0;

    virtual char int_frac_digits() const = 0;
    virtual char frac_digits() const = 0;
    virtual char p_cs_precedes() const = 0;
    virtual char p_sep_by_space() const = 0;
    virtual char n_cs_precedes() const = 0;
    virtual char n_sep_by_space() const = 0;
    virtual char p_sign_posn() const = 0;
    virtual char n_sign_posn() const = 0;

    /**
     * Converts the number \a i to a string (for example, \c "3,456").
     */
    virtual RWCString asString(long i) const = 0;
    /**
     * @copydoc asString(long)const
     */
    virtual RWCString asString(unsigned long i) const = 0;

    /**
     * Converts the \c double \a f to a string. The variable \a precision
     * is the number of digits to place after the decimal separator.
     * If \a showpoint is \c true, the decimal separator appears regardless
     * of the precision.
     */
    virtual RWCString asString(double f, int precision = 6, bool showpoint = 0) const = 0;

#if !defined(RW_NO_LONG_DOUBLE)
    /**
     * Converts the value of \a val to a string. The variable precision
     * is the number of digits to place after the decimal separator.
     * If \a showpoint is \c true, the decimal separator appears regardless
     * of the precision.
     */
    virtual RWCString asString(long double val, int precision = 10, bool showpoint = 0) const = 0;
#endif // RW_NO_LONG_DOUBLE

    // asString(tm*, format): format is one of [aAbBcdHIjmMpSUwWxXyYZ],
    // as in ANSI C Library function \c strftime().
    /**
     * Converts components of \a tmbuf to a string, according to the format
     * character \a format. The meanings assigned to the format character
     * are identical to those used in the C99 function \c strftime(). The
     * members of \a tmbuf should be set consistently.
     *
     * Table 18: The following table contains examples of formatting
     * characters used by the C99 version of \c strftime(). For any format
     * that does not use all members of the <tt>struct tm</tt>, only those
     * members that are actually used are noted [in brackets]
     *
     * <table>
     * <tr>
     * <th><B>Format character </B></th>
     * <th><B>Meaning </B></th>
     * </tr>
     * <tr>
     * <td>\c \%a</td>
     * <td>
     * Replaced by the locale's abbreviated weekday name [from
     * \c tm::tm_wday]
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%A</td>
     * <td>
     * Replaced by the locale's full weekday name [from \c tm::tm_wday]
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%b</td>
     * <td>Replaced by the locale's abbreviated month name</td>
     * </tr>
     * <tr>
     * <td>\c \%B</td>
     * <td>Replaced by the locale's full month name.</td>
     * </tr>
     * <tr>
     * <td>\c \%c</td>
     * <td>
     * Replaced by the locale's appropriate date and time representation.
     * (See the Base Definitions volume of IEEE Std 1003 1-2001, \c <time.h>.)
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%C</td>
     * <td>
     * Replaced by the year divided by 100 and truncated to an
     * integer, as a decimal number [00,99]
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%d</td>
     * <td>Replaced by the day of the month as a decimal number [01,31]</td>
     * </tr>
     * <tr>
     * <td>\c \%D</td>
     * <td>Equivalent to <tt>\%m/\%d/\%y</tt></td>
     * </tr>
     * <tr>
     * <td>\c \%e</td>
     * <td>
     * Replaced by the day of the month as a decimal number [1,3];
     * a single digit is preceded by a space
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%F</td>
     * <td>
     * Equivalent to <tt>\%Y-\%m-\%d</tt> (the ISO 8601:2000 standard date
     * format)
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%g</td>
     * <td>
     * Replaced by the last 2 digits of the week-based year (see
     * below) as a decimal number [00,99]
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%G</td>
     * <td>
     * Replaced by the week-based year (see below) as a decimal
     * number (for example, 1977)
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%h</td>
     * <td>Equivalent to \c \%b</td>
     * </tr>
     * <tr>
     * <td>\c \%H</td>
     * <td>
     * Replaced by the hour (24-hour clock) as a decimal number
     * [00,23]
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%I</td>
     * <td>
     * Replaced by the hour (12-hour clock) as a decimal number
     * [01,12]
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%j</td>
     * <td>Replaced by the day of the year as a decimal number [001,366]</td>
     * </tr>
     * <tr>
     * <td>\c \%m</td>
     * <td>Replaced by the month as a decimal number [01,12]</td>
     * </tr>
     * <tr>
     * <td>\c \%M</td>
     * <td>Replaced by the minute as a decimal number [00,59]</td>
     * </tr>
     * <tr>
     * <td>\c \%n</td>
     * <td>Replaced by a newline character</td>
     * </tr>
     * <tr>
     * <td>\c \%p</td>
     * <td>
     * Replaced by the locale's equivalent of either a.m. or p.m.
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%r</td>
     * <td>
     * Replaced by the time in a. m. and p. m. notation; in the
     * POSIX locale this is equivalent to <tt>\%I:\%M:\%S \%p</tt>
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%R</td>
     * <td>Replaced by the time in 24-hour notation (<tt>\%H:\%M</tt>)</td>
     * </tr>
     * <tr>
     * <td>\c \%s</td>
     * <td>The number of seconds since the Epoch <tt>1970-01-01 00:00:00 UTC</tt>.</td>
     * </tr>
     * <tr>
     * <td>\c \%S</td>
     * <td>Replaced by the second as a decimal number [00,60]</td>
     * </tr>
     * <tr>
     * <td>\c \%t</td>
     * <td>Replaced by a tab character</td>
     * </tr>
     * <tr>
     * <td>\c \%T</td>
     * <td>Replaced by the time (<tt>\%H:\%M:\%S</tt>)</td>
     * </tr>
     * <tr>
     * <td>\c \%u</td>
     * <td>
     * Replaced by the weekday as a decimal number [1,7], with
     * 1 representing Monday
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%U</td>
     * <td>
     * Replaced by the week number of the year as a decimal number
     * [00,53] The first Sunday of January is the first day of week
     * 1; days in the new year before this are in week 0.
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%V</td>
     * <td>
     * Replaced by the week number of the year (Monday as the
     * first day of the week) as a decimal number [01,53]. If the week
     * containing 1 January has four or more days in the new year, then
     * it is considered week 1. Otherwise, it is the last week of the
     * previous year, and the next week is week 1. Both January 4th
     * and the first Thursday of January are always in week 1.
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%w</td>
     * <td>
     * Replaced by the weekday as a decimal number [0,6], with
     * 0 representing Sunday
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%W</td>
     * <td>
     * Replaced by the week number of the year as a decimal number
     * [00,53]. The first Monday of January is the first day of week
     * 1; days in the new year before this are in week 0.
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%x</td>
     * <td>
     * Replaced by the locale's appropriate date representation.
     * (See the Base Definitions volume of IEEE Std 1003 1-2001, \c <time.h>.)
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%X</td>
     * <td>
     * Replaced by the locale's appropriate time representation.
     * (See the Base Definitions volume of IEEE Std 1003 1-2001, \c <time.h>.)
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%y</td>
     * <td>
     * Replaced by the last two digits of the year as a decimal
     * number [00,99]
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%Y</td>
     * <td>Replaced by the year as a decimal number (for example, 1997)</td>
     * </tr>
     * <tr>
     * <td>\c \%z</td>
     * <td>
     * Replaced by the offset from UTC in the ISO 8601:2000 standard
     * format (\c +hhmm or \c -hhmm), or by no characters if no timezone
     * is determinable. For example, \c -0430 means 4 hours 30 minutes
     * behind UTC (west of Greenwich).
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%Z</td>
     * <td>
     * Replaced by the timezone name or abbreviation, or by no
     * bytes if no timezone information exists.
     * </td>
     * </tr>
     * <tr>
     * <td>\c \%\%</td>
     * <td>Replaced by \c \%</td>
     * </tr>
     * </table>
     */
    virtual RWCString asString(const struct tm* tmbuf, char format,
                               const RWZone& zone = RWZone::local()) const = 0;

    /**
     * Converts components of \a tmbuf to a string,
     * according to the format string \a format. Each format character in the
     * format string must be preceded by \c \%. Any characters not preceded
     * by \c \% are treated as ordinary characters which are returned
     * unchanged. You may represent the special character \c \% with
     * \c \%\%. Format specifiers are converted to a string representation
     * using RWLocale::asString(const struct tm*, char, const RWZone&) const.
     */
    RWCString asString(const struct tm* tmbuf, const char* format,
                       const RWZone& zone = RWZone::local()) const;

    /**
     * Controls how to format currency, specifying whether to use no
     * currency symbol, the local currency symbol (such as \c $), or
     * the international currency symbol (such as \c USD).
     */
    enum CurrSymbol {

        /**
         * No currency symbol
         */
        NONE,

        /**
         * Local currency symbol
         */
        LOCAL,

        /**
         * International currency symbol
         */
        INTL
    };

    /**
     * Returns a string containing the \a value argument formatted according
     * to monetary conventions for the locale. The \a value argument
     * should contain an integer representing the number of units of
     * currency (for example, <tt>moneyAsString(1000., RWLocale::LOCAL)</tt>
     * in a US locale would yield \c "$10.00"). The \a sym argument
     * determines which currency symbol to use.
     */
    virtual RWCString moneyAsString(double value,
                                    RWLocale::CurrSymbol sym = LOCAL) const = 0;

    /**
     * Interprets the RWCString argument as a floating point number.
     * Spaces are allowed before and after the (optional) sign, and
     * at the end. Digit group separators are allowed in the integer
     * portion. Returns \c true for a valid number, \c false for an
     * error. If it returns \c false, then the \c double* argument is
     * untouched. All valid numeric strings are accepted; all others are
     * rejected. The following are examples of valid numeric strings in an
     * English-speaking locale:
     *
     * @code
     * "1"          " -02. "     ".3"
     * "1234.56"    "1e10"       "+ 19,876.2E+20"
     * @endcode
     */
    virtual bool stringToNum(const RWCString& str, double* fp) const = 0;

    /**
     * Interprets the RWCString argument as an integer. Spaces are
     * allowed before and after the (optional) sign, and at the end.
     *  Digit group separators are allowed. Returns \c true for a valid
     * integer, \c false for an error. If it returns \c false, then the
     * \c long* argument is untouched. All valid numeric strings are
     * accepted; all others are rejected. The following are examples
     * of valid integral strings in an English-speaking locale:
     *
     * @code
     * "1"           " -02. "     "+ 1,234"
     * "1234545"     "1,234,567"
     *
     * @endcode
     */
    virtual bool stringToNum(const RWCString& str, long* ip) const = 0;

    /**
     * Interprets the RWCString argument as an integer. Allows spaces
     * before and after the (optional) sign, and at the end. Allows
     * digit group separators. Returns \c true for a valid integer,
     * \c false for an error. If it returns \c false, then the \c long*
     * argument is untouched. All valid numeric strings are accepted; all
     * others are rejected.
     *
     * @note
     * If you are using the C locale (likely the default for English-speaking
     * programmers), you must omit commas as thousands
     * separators because these numbers
     * may be rejected. This is true even in a locale where English is
     * the dominant language.
     */
    virtual bool stringToNum(const RWCString& str, unsigned long* ip) const = 0;

    /**
     * Interprets the RWCString as a date, and extracts the month, day,
     * and year components to \a tmbuf. It returns \c true
     * for a valid date, otherwise \c false. If it returns \c false, then
     * \a tmbuf is untouched; otherwise it sets
     * the \c tm_mday, \c tm_mon, and \c tm_year members. The date
     * entered is expected to be in the format indicated by the current
     * locale (\c \%x). If that is not the case, the algorithm attempts
     * to detect the different parts of the date string.
     *
     * @note
     * This function cannot reject all invalid date strings.
     *
     * The following are examples of parseable date strings in an English-speaking
     * locale:
     *
     * @code
     * "Jan 9, 62"     "1/9/62"     "January 9 1962"
     * "09Jan62"       "010962"
     *
     * @endcode
     */
    virtual bool stringToDate(const RWCString& str, struct tm* tmbuf) const = 0;

    /**
     * Interprets \a str as a time, with hour, minute,
     * and optional second. The time string is expected to be formatted
     * according to the locale's \c \%X format specifier. If that is
     * not the case, the algorithm attempts to detect the different
     * parts of the time string. If the hour is in the range [1..12],
     * the local equivalent of "AM" or "PM" is allowed.  Returns \c
     * true for a valid time string, \c false for an error.  If it returns
     * \c false, then \a tmbuf is untouched; otherwise it sets
     * the \c tm_hour, \c tm_min, and \c tm_sec members.
     *
     * @note
     * This function cannot reject all invalid time strings. The
     * following are examples of valid time strings in an English-speaking
     * locale:
     *
     * @code
     * "1:10 AM"     "13:45:30"     "12.30.45pm"
     * "PM 3:15"     "1430"
     * @endcode
     */
    virtual bool stringToTime(const RWCString& str, struct tm* tmbuf) const = 0;

    /**
     * Interprets the RWCString argument as a monetary value. The currency
     * symbol, if any, is ignored. Negative values may be specified
     * by the negation symbol or by enclosing parentheses. Digit group
     * separators are optional; if present they are checked. Returns
     * \c true for a valid monetary value, \c false for an error. If
     * it returns \c false, then the \c double* argument is untouched;
     * otherwise it is set to the integral number of monetary units entered
     * (for example, cents in a U.S. locale).
     */
    virtual bool stringToMoney(const RWCString& str, double* d,
                               RWLocale::CurrSymbol = LOCAL) const = 0;

    /**
     * Interprets its argument as a full or abbreviated month name,
     * returning values 1 through 12 to represent (respectively) January
     * through December, or 0 for an error. Ignores leading white space.
     */
    virtual int monthIndex(const RWCString& str) const = 0;

    /**
     * Interprets its argument as a full or abbreviated weekday name,
     * returning values 1 through 7 to represent (respectively) Monday
     * through Sunday, or 0 for an error.
     */
    virtual int weekdayIndex(const RWCString& str) const = 0;

    /**
     * Returns a reference to the present global "default" locale.
     */
    static const RWLocale& global();

    /**
     * Sets the global "default" locale object to \a loc, returning
     * the old object. This object is used by RWDateTime or RWDate
     * string conversion functions as a default locale. It is
     * initially set to refer to an instance of a class that provides
     * the functionality of RWLocaleSnapshot("C").
     */
    static const RWLocale* global(const RWLocale* loc);

    /**
     * Returns a pointer to a new instance of RWLocaleSnapshot("C").
     */
    static const RWLocale* defaultLocale();

    static void releaseCache();

#ifndef RW_IOS_XALLOC_BROKEN

    /**
     * Replaces the locale installed in \a stream with the
     * current locale and returns the old locale.
     */
    const RWLocale*   imbue(std::ios& stream) const;

    /**
     * Removes the locale that was installed in \a stream
     * by a previous call to RWLocale::imbue(). If no locale was previously
     * installed, returns the result of RWLocale::global().
     */
    static const RWLocale*  unimbue(std::ios& stream);

#endif /* RW_IOS_XALLOC_BROKEN */

    /**
     * Returns the locale installed in \a stream by a previous
     * call to RWLocale::imbue(); if no locale was installed, returns
     * the result from RWLocale::global().
     */
    static const RWLocale&  of(std::ios& stream);

};

#include <rw/tools/localesnapshot.h>

#if !defined(RW_NO_ANSILOCALE) //only include if in presence of a stdlib
#  include <rw/tools/ansilocale.h>
#endif

#endif /*RW_TOOLS_LOCALE_H*/
