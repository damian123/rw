
/**********************************************************************
 *
 * Declarations for class RWDateTime.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/datetime.h#1 $
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

#ifndef RW_TOOLS_DATETIME_H
#define RW_TOOLS_DATETIME_H

#include <rw/defs.h>

#include <math.h>
#include <time.h>
#include <limits.h>

#include <rw/zone.h>
#include <rw/locale.h>

#if !defined(RW_DISABLE_DEPRECATED)
#  include <rw/rwtime.h>
#endif

#include <rw/rwdate.h>
#include <rw/ctoken.h>
#include <rw/rwerr.h>
#include <rw/toolerr.h>

#include <rw/tools/hash.h>
#include <rw/tools/ristream.h>
#include <rw/tools/rostream.h>

/***************************************************************************/

// The Julian Day number starting Jan 1st 1901 12:00:00,
// counting since January 1, 4713 BCE, 12:00:00.
#define RW_JUL1901 (2415386) // starting at noon

// The Julian Day number in effect on Jan 1st 1901 00:00:00,
// counting since January 1, 4713 BCE, 12:00:00.
#define RW_MIDNIGHT_JUL1901     (2415385)

#define RW_MILLISECONDS_IN_SEC  (1000)
#define RW_MILLISECONDS_IN_MIN  (60000)
#define RW_MILLISECONDS_IN_HOUR (3600000)
#define RW_MILLISECONDS_IN_DAY  (86400000)

#define RW_JDAY0_NOON \
        (rwint64(-2415386) * rwint64(86400000) + \
         rwint64(12) * rwint64(3600000))

#define RW_JDAY0_MIDNIGHT \
        (rwint64(-2415386) * rwint64(86400000))

#define RW_USER_SENTINEL_START  RWINT64_MIN
#define RW_SENTINEL_START       RWINT64_MIN + 128
#define RW_INVALID_SENTINEL     RWINT64_MIN + 128
#define RW_NULL_SENTINEL        RWINT64_MIN + 129
#define RW_PAST_SENTINEL        RWINT64_MIN + 256
#define RW_MIN_DATETIME         RWINT64_MIN + 257
#define RW_MAX_DATETIME         RWINT64_MAX - 1
#define RW_FUTURE_SENTINEL      RWINT64_MAX

/**
 * @ingroup date_and_time_classes
 * @brief Represents a date and time stored in milliseconds.
 *
 * Class RWDateTime represents a date and time, stored in milliseconds,
 * from January 1, 1901 00:00:00:000 UTC. Milliseconds are stored
 * in a 64-bit integral type, which is represented by the global
 * typedef \c rwint64.
 *
 * RWDateTime supports the ISO 8601 international standard for
 * the formatting of dates and times. For information and formatting
 * syntax, see the chapter on internationalization in the
 * <i>Essential Tools Module User's Guide</i>.
 *
 * RWDateTime is based on the proleptic Gregorian calendar. It applies
 * Gregorian calendar rules for dates in the future as well as those
 * prior to its introduction in 1582.
 *
 * Four public member functions, all starting with \c julian, let
 * you manipulate the date via Modified Julian days (MJD). Note
 * that the Modified Julian day number is not the same as a date
 * according to the Julian calendar. The <i>Essential Tools Module
 * User's Guide</i> provides more information.
 *
 * @note
 * The modern Modified Julian Day (MJD) was introduced in the 1950s and
 * starts at midnight, as opposed to the historic Julian Day (JD), which
 * began at 12 noon. MJD is defined as <tt>MJD = JD - 2400000.5</tt>.
 * The half day is subtracted so that the day starts at midnight in
 * conformance with civil time reckoning.
 *
 * @note
 * Two-digit year specifiers assume the century 1900, so you should create
 * programs that use four-digit year specifiers. You can enforce the use
 * of a four-digit year by defining the build macro #RW_CENTURY_REQD in RCB
 * when building the libraries. Be aware, however, that defining this macro
 * \e requires a four-digit year to be provided when constructing an
 * RWDateTime. For more information, see the section on RWDateTime in the
 * <i>Essential Tools Module User's Guide</i>.
 *
 * An RWDateTime can be converted to and from an RWDate instance. RWDate
 * represents a date with no time component and does not change according to
 * the time zone. In contrast, RWDateTime represents a date and time and
 * varies depending on the time zone in which the date and time data is
 * retrieved.
 *
 * @note
 * RWTime is deprecated and so are all methods that either take or return
 * instances of this class. Deprecation means that these classes and
 * related methods are no longer supported, and may be removed in a future
 * release.
 *
 * RWDateTime instances can also be converted to and from the C Standard
 * Library type <tt>struct tm</tt> defined in \c \<time.h>, and listed
 * below.
 *
 * <b>The Standard C Library Type <tt>struct tm</tt></b>
 * <table>
 * <tr>
 * <td><tt>int tm_sec;</tt> </td>
 * <td><tt>seconds after the minute - [0,59]</tt> </td>
 * </tr>
 * <tr>
 * <td><tt>int tm_min;</tt> </td>
 * <td><tt>minutes after the hour - [0,59]</tt> </td>
 * </tr>
 * <tr>
 * <td><tt>int tm_hour;</tt> </td>
 * <td><tt>hours since midnight - [0,23]</tt> </td>
 * </tr>
 * <tr>
 * <td><tt>int tm_mday;</tt> </td>
 * <td><tt>day of the month - [1,31]</tt> </td>
 * </tr>
 * <tr>
 * <td><tt>int tm_mon;</tt> </td>
 * <td><tt>months since January - [0,11]</tt> </td>
 * </tr>
 * <tr>
 * <td><tt>int tm_year;</tt> </td>
 * <td><tt>years since 1900</tt> </td>
 * </tr>
 * <tr>
 * <td><tt>int tm_wday;</tt> </td>
 * <td><tt>days since Sunday - [0,6]</tt> </td>
 * </tr>
 * <tr>
 * <td><tt>int tm_yday;</tt> </td>
 * <td><tt>days since January 1 - [0,365]</tt> </td>
 * </tr>
 * <tr>
 * <td><tt>int tm_isdst;</tt> </td>
 * <td><tt>daylight savings time flag</tt></td>
 * </tr>
 * </table>
 *
 * The default constructor for this class creates an instance with
 * an invalid date and time, unlike RWDate, which holds the current
 * date by default. Consequently, it is much quicker to create a
 * large array using RWDateTime than it is using RWDate. The example
 * below illustrates the difference:
 *
 * @code
 * // Figures out the current date 5000 times
 * RWDate v[5000];
 *
 * // Creates an array with 5000 undefined dates and times
 * RWDateTime v[5000];
 * @endcode
 *
 * RWDateTime objects can hold a variety of sentinel values that
 * do not refer to actual dates. Consequently it is important to
 * know whether an object holds an actual date before performing
 * many operations. The isSentinel() function returns \c true when
 * an object holds a sentinel value.
 *
 * Note that the use of the isValid() function does not determine
 * whether a member function can be successfully applied to an object,
 * since the null sentinel value represents a valid RWDateTime whose
 * value is not yet determined. For more information about RWDateTime
 * sentinels, see the section on RWDateTime Sentinels in the
 * internationalization chapter of the <i>Essential Tools Module
 * User's Guide</i>.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tools/datetime.h>
 * RWDateTime dt(RWDateTime::setCurrentTime); // construct
 *                                            // today's date
 * @endcode
 *
 * @section persistence Persistence
 *
 * Simple
 *
 * @section example Example
 *
 * @code
 * #include <rw/tools/datetime.h>
 * #include <iostream>
 *
 * int main() {
 *       // Today's date
 *      RWDateTime dt(RWDateTime::setCurrentTime);
 *
 *      // Last Sunday's date
 *      RWDateTime lastSunday = dt.previous("Sunday");
 *
 *      std::cout << dt << std::endl << lastSunday << std::endl;
 * }
 * @endcode
 */
class RW_TOOLS_GLOBAL RWDateTime
{
public:

    /**
     * Specifies whether the constructor should construct an \a invalid,
     * \a null, or current RWDateTime. Default is to set an \a invalid
     * RWDateTime, with an undefined date and time.
     */
    enum InitialState {

        /**
         * Constructs Invalid RWDateTime
         */
        invalid,

        /**
         * Constructs null RWDateTime
         */
        null,

        /**
         * Constructs RWDateTime consisting of current time
         */
        setCurrentTime
    };

    /**
     * Specifies whether the constructor should set just the date, just
     * the time, or both. Default is to set both.
     */
    enum SetType {

        /**
         * Sets the date
         */
        setDate,

        /**
         * Sets the time
         */
        setTime,

        /**
         * Sets both the date and the time
         */
        setBoth
    };

    enum ImplType { implDouble, implLong };

    /**
     * Specifies the format to be used for converting between RWCString
     * and RWDateTime using the ISO 8601 standard. This standard converts
     * any date and time to a locale-nonspecific numeric format.
     */
    enum Format {

        /**
         * Parses dates, times, and date-times in the ISO 8601 standard
         * formats as described in the
         * \ref iso8601_reference "ISO 8601 Compatibility Parser Reference".
         */
        iso8601_compat,

        /**
         * Parses dates, times, and date-times in the ISO 8601:2000 standard
         * formats as described in the
         * \ref iso8601_2000_reference "ISO 8601:2000 Parser Reference".
         */
        iso8601_2000,

        /**
         * Parses dates, times, and date-times in the ISO 8601:2000 standard
         * format. When generating a string representation, midnight appears
         * as hour 24 of the previous day rather than hour 0 of the given day.
         */
        iso8601_midnight24,

        /**
         * An alias for #iso8601_2000.
         */
        iso8601 = iso8601_2000

    };

    // Julian Day Number for noon 1/1/1901
    RW_STATIC_CONST(rwint64 jul1901, = RW_JUL1901);

    // Julian Day Number for midnight 1/1/1901
    RW_STATIC_CONST(rwint64 midnightJul1901, = RW_MIDNIGHT_JUL1901);

    /**
     * The number of milliseconds in a second.
     */
    RW_STATIC_CONST(rwint64 millisecsInSec, = RW_MILLISECONDS_IN_SEC);

    /**
     * The number of milliseconds in a minute.
     */
    RW_STATIC_CONST(rwint64 millisecsInMin, = RW_MILLISECONDS_IN_MIN);

    /**
     * The number of milliseconds in a hour.
     */
    RW_STATIC_CONST(rwint64 millisecsInHour, = RW_MILLISECONDS_IN_HOUR);

    /**
     * The number of milliseconds in a day.
     */
    RW_STATIC_CONST(rwint64 millisecsInDay, = RW_MILLISECONDS_IN_DAY);

    // Millisecond count at noon on julian day zero
    RW_STATIC_CONST(rwint64 jday0, = RW_JDAY0_NOON);
    RW_STATIC_CONST(rwint64 midnightJday0, = RW_JDAY0_MIDNIGHT);

    // 128 Sentinels available for users
    /**
     * The starting value for user-defined sentinels.
     */
    RW_STATIC_CONST(rwint64 userSentinelStart, = RW_USER_SENTINEL_START);

    // 120 Sentinels available for Rogue Wave internal use
    RW_STATIC_CONST(rwint64 sentinelStart, = RW_SENTINEL_START);

    // Sentinels for sorting and searching
    /**
     * Sentinel values representing past RWDateTime sentinels.
     */
    RW_STATIC_CONST(rwint64 pastSentinel, = RW_PAST_SENTINEL);

    /**
     * Sentinel values representing future RWDateTime sentinels.
     */
    RW_STATIC_CONST(rwint64 futureSentinel, = RW_FUTURE_SENTINEL);

    // Rogue Wave internal sentinels begin here
    /**
     * Sentinel values for invalid RWDateTime sentinels.
     */
    RW_STATIC_CONST(rwint64 invalidSentinel, = RW_INVALID_SENTINEL);

    /**
     * Sentinel values for null RWDateTime sentinels.
     */
    RW_STATIC_CONST(rwint64 nullSentinel, = RW_NULL_SENTINEL);

    /**
     * The minimum number of milliseconds that an RWDateTime may hold.
     */
    RW_STATIC_CONST(rwint64 minDateTime, = RW_MIN_DATETIME);

    /**
     * The maximum number of milliseconds that an RWDateTime may hold.
     */
    RW_STATIC_CONST(rwint64 maxDateTime, = RW_MAX_DATETIME);

    /******************************************************************/

    /**
     * Default constructor. Constructs an RWDateTime with undefined
     * date and time. If \a init_state is set to RWDateTime::setCurrentTime,
     * an instance with the current date and time is created. If
     * \a init_state is set to RWDateTime::null, an instance set to
     * null is created.
     */
    RWDateTime(InitialState init_state = invalid);

    /**
     * Copy constructor.
     */
    RWDateTime(const RWDateTime& dt) : msec_(dt.msec_) { }

    /**
     * Constructs an RWDateTime from the number of milliseconds since
     * 1 January 1901 00:00:00:000 UTC.  \c rwint64 is at least a 64-bit
     * integral type.
     */
    explicit RWDateTime(rwint64 msec) : msec_(msec) { }

    /**
     * Constructs an RWDateTime from the \c tm_year, \c tm_mon, \c tm_mday,
     * \c tm_hour, \c tm_min, \c tm_sec components of the <tt>struct tm</tt>
     * argument, with the milliseconds portion of the RWDateTime
     * as specified by \a msec. The components are understood to be
     * relative to the time zone passed in, which defaults to local
     * time. Note that the numbering of the months and years in a
     * <tt>struct tm</tt> differs from that used in RWDateTime arguments,
     * which start at the number 1. The arguments included in <tt>struct tm</tt>
     * are shown in the class description.
     *
     * The constructor tests as a precondition the <tt>struct tm</tt>
     * pointer which cannot be null. If null, it asserts in debug builds
     * and throws an object of type RWInternalErr in optimized builds.
     *
     * This constructor produces a valid RWDateTime instance for date-time
     * values between -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999
     * in the specified time zone. Attempts to create an RWDateTime with values
     * outside this range will result in undefined behavior.
     */
    RWDateTime(const struct tm* tmbuf, unsigned msec = 0,
               const RWZone& loc = RWZone::local());

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    /**
     * @deprecated As of SourcePro 10. Provided for compatibility with
     * code that relies on the deprecated RWTime type.
     *
     * Constructs an RWDateTime from the given RWTime using the \a msec
     * argument for the number of milliseconds.
     */
    RW_DEPRECATE_FUNC("")
    RWDateTime(const RWTime& t, unsigned msec = 0);

    RW_RESTORE_DEPRECATED_WARNINGS
#endif

    /**
     * Constructs an RWDateTime using the RWDate \a d and the time
     * specified by \a hours, \a minutes, \a seconds, and \a msec.
     * The time specified defaults to local time.
     */
    RWDateTime(const RWDate& d,
               unsigned hours = 0, unsigned minutes = 0, unsigned seconds = 0,
               unsigned msec = 0,
               const RWZone& zone = RWZone::local());

    /**
     * Constructs an RWDateTime using the specified day, month, year,
     * hour, minute, second, and millisecond, all relative to the time
     * zone specified, which defaults to local time. The optional locale
     * argument \a loc is used to convert the month name.
     *
     * This constructor produces a valid RWDateTime instance for date-time
     * values between -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999
     * in the specified time zone. Attempts to create an RWDateTime with values
     * outside this range will result in undefined behavior.
     *
     * As a precondition, \a month is tested to determine if it is a
     * null pointer. If null, the method asserts in debug mode, and
     * throws RWInternalErr in optimized builds.
     */
    RWDateTime(unsigned month_day, const char* month,
               unsigned year, unsigned hours, unsigned minutes,
               unsigned seconds, unsigned msec,
               const RWLocale& loc = RWLocale::global(),
               const RWZone& zone = RWZone::local());

    /**
     * Constructs an RWDateTime using the specified day, month, year,
     * hour, minute, second, and millisecond, all relative to the time
     * zone specified, which defaults to local time.
     *
     * This constructor produces a valid RWDateTime instance for date-time
     * values between -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999
     * in the specified time zone. Attempts to create an RWDateTime with values
     * outside this range will result in undefined behavior.
     */
    RWDateTime(unsigned day, unsigned month, unsigned year,
               unsigned hours = 0, unsigned minutes = 0, unsigned seconds = 0,
               unsigned milliseconds = 0,
               const RWZone& zone = RWZone::local());

    /**
     * Converts the string \a str to a date and/or time as indicated
     * by \a set_type, all relative to the time zone specified, which
     * defaults to local time. By default, both the date and time are
     * set using RWLocale \a loc. The constructor expects the date and
     * time portions of the RWCString to be separated by a semicolon
     * when both date and time are being set. The locale argument is
     * used to convert the month name.
     *
     * Because RWLocale cannot rigorously check date input, the member
     * function isValid() must be used to test that the results are a
     * valid date.
     *
     * This constructor handles years 0 to 9999. Passing a string with a
     * year outside this range will result in undefined behavior. Year
     * values 0 to 99 are treated differently based on how the library
     * was built. In all builds that do not define the macro #RW_CENTURY_REQD,
     * years 0 to 99 are treated as 1900 to 1999. Debug builds that define
     * the macro will produce a debug assertion. Release builds that define
     * the macro will produce an invalid RWDateTime instance.
     *
     * <i>Two Examples: </i>
     *
     * @code
     * RWDateTime("April 4, 1998; 10:00 am",
     *            RWDateTime::setBoth,
     *            RWLocale::global());  // sets date and time
     * RWDateTime("April 4, 1998",
     *            RWDateTime::setDate,
     *            RWLocale::global());  // sets date and initializes
     *                                  // time to 00:00:00
     * @endcode
     *
     * @code
     * RWDateTime("10:00 am",
     *            RWDateTime::setTime,
     *            RWLocale::global());  // initializes date to
     *                                  // today at time 10:00
     * @endcode
     */
    explicit RWDateTime(const RWCString& str, SetType set_type = setBoth,
                        const RWLocale& loc = RWLocale::global(),
                        const RWZone& zone = RWZone::local());

    /**
     * Constructs an RWDateTime from the string \a str using the format
     * specified by \a f.  RWDateTime currently supports the ISO 8601
     * format which is numeric and locale-neutral; as such the RWLocale
     * argument is ignored for ISO 8601, but is reserved for future
     * use. The constructor uses the RWZone argument unless the string
     * specifies the UTC timezone or an offset from UTC, in which case
     * the RWZone argument is ignored in favor of UTC.  For more information,
     * see the section on applying the ISO standard to timezone offsets, in
     * the internationalization chapter of the <i>Essential Tools Module
     * User's Guide</i>.
     *
     * This constructor handles four-digit year values 0000 to 9999. Passing
     * a string with a year value that is not exactly four digits or is outside
     * of this range will result in undefined behavior.
     *
     * @note
     * Please note that confusion can occur when using a string that
     * contains only a date or only a time indication, since it is not
     * possible to differentiate between some representations of the
     * two formats. For example. the string \c 190415 could represent
     * either or both of the following:
     * -  \e date: the 15th day of the 4th month of the 19th year in
     *    the default century
     * -  \e time: 19:04:15
     * <br><br>You can avoid such confusion by using more explicit formats.
     * For example, you can completely and safely avoid all confusion
     * if you are prepend "T" (allowed by the standard in
     * time-only indications) for all times or if you always use extended
     * formats.
     * <br><br>For more information, see the section on applying the ISO
     * standard to timezone offsets, in the internationalization chapter
     * of the <i>Essential Tools Module User's Guide</i>.
     *
     * <i>Two Examples: </i>
     *
     * @code
     * RWDateTime("1998-04-04T00:00:00-08:00",
     *            RWDateTime::iso8601,
     *            RWLocale::global());
     * RWDateTime("1998-04-04T00:00:00Z",
     *            RWDateTime::iso8601_midnight24,
     *            RWLocale::global());
     * @endcode
     *
     * @code
     * RWDateTime("2001-12-24",
     *            RWDateTime::iso8601,
     *            RWLocale::global());  // initializes time to
     *                                  // 00:00:00
     * RWDateTime("10:24:55",
     *            RWDateTime::iso8601,
     *            RWLocale::global());  // initializes date to
     *                                  // current system date
     * @endcode
     *
     * For a date or time supplied singly (for example, "2001-12-24"
     * or "23:59:59"), RWDateTime calls the system to retrieve the current
     * date or time.
     */
    RWDateTime(const RWCString& str, Format f,
               const RWLocale& loc = RWLocale::global(),
               const RWZone& zone = RWZone::local());

    /**
     * Reads a full line and converts it to a date and/or time as indicated
     * by the \a set_type, all relative to the specified time zone, which
     * defaults to local time. By default, both the date and time are set
     * using the RWLocale \a loc. The constructor expects the date and time
     * portions of the RWCString to be separated by a semi-colon when both
     * day and time are being set. The locale argument is used to convert
     * the month name.
     *
     * Because RWLocale cannot rigorously check date input, the member
     * function isValid() must be used to test that the results are a
     * valid date.
     *
     * This constructor handles years 0 to 9999. Passing a string with
     * a year outside this range will result in undefined behavior.
     *
     * Year values 0 to 99 are treated differently based on how the library
     * was built. In all builds that do not define the macro #RW_CENTURY_REQD,
     * years 0 to 99 are treated as 1900 to 1999. Debug builds that define
     * the macro will produce a debug assertion. Release builds that define
     * the macro will produce an invalid RWDateTime instance.
     */
    RWDateTime(std::istream& s, SetType set_type,
               const RWLocale& loc = RWLocale::global(),
               const RWZone& zone = RWZone::local());


    /******************************************************************/

    /**
     * Assignment operator.
     */
    RWDateTime&
    operator= (const RWDateTime&);

    /**
     * Returns the date and time as a string, formatted by the optional
     * locale argument. The function defaults to using the RWLocale
     * global locale.
     *
     * Formats are as defined in the description for RWLocale::asString().
     * RWDateTime extends these formats with the addition of the \c 'L'
     * format specifier, which prints the milliseconds.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    RWCString
    asString(char format,
             const RWLocale& loc = RWLocale::global(),
             const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the date and time as a string, formatted by the optional
     * locale argument. The function defaults to using the RWLocale
     * global locale.
     *
     * Formats are as defined in the description for RWLocale::asString().
     * RWDateTime extends these formats with the addition of the \c '%L'
     * format specifier, which prints the milliseconds.
     *
     * As a precondition, \a format is tested to determine if it is a
     * null pointer. If null, the method asserts in debug mode, and
     * throws RWInternalErr in optimized builds.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    RWCString
    asString(const char* format,
             const RWLocale& loc = RWLocale::global(),
             const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the date and time as a string, using the specified format.
     *
     * Currently supports the ISO 8601 format. This format is numeric
     * and locale-neutral, and as such does not use the RWLocale argument.
     *
     * For more information, see the section on international standards
     * for dates and times in the internationalization chapter of the
     * <i>Essential Tools Module User's Guide</i>.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    RWCString
    asString(Format format,
             const RWLocale& loc = RWLocale::global(),
             const RWZone& zone = RWZone::local()) const;

    /**
     * Returns \c true if self is between \a a and \a b, inclusive.
     */
    bool
    between(const RWDateTime& a, const RWDateTime& b) const;

    /**
     * Returns the number of bytes necessary to store the object using
     * the global function:
     *
     * @code
     * RWFile& operator<<(RWFile&, const RWDateTime&);
     * @endcode
     */
    RWspace
    binaryStoreSize() const;

    /**
     * Compares self to the RWDateTime referenced by \a d and returns:
     *
     * @code
     * 0 if self == *d
     * 1 if self > *d
     * -1 if self < *d
     * @endcode
     *
     * As a precondition, \a d is tested to determine if it is a
     * null pointer. If null, the method asserts in debug mode, and
     * throws RWInternalErr in optimized builds.
     */
    int
    compareTo(const RWDateTime* d) const;

    /**
     * Compares self to the RWDateTime pointed to by \a d and returns:
     *
     * @code
     * 0 if self == d
     * 1 if self > d
     * -1 if self < d
     * @endcode
     */
    int
    compareTo(const RWDateTime& d) const {
        return compareTo(&d);
    }

    /**
     * Returns the value of the Julian day number. This value is one
     * day less than the value of RWDateTime::julian() if the time is
     * before noon UTC (GMT).
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     */
    unsigned
    correctedJulian() const;

    /**
     * Returns the day of the year (1-366) for this date. The default for
     * the optional timezone argument is local.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     *
     * @note
     * This method throws an exception if called on a sentinel date. Otherwise,
     * it returns a valid day of year.
     */
    unsigned
    day(const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the day of the year (1-366) for this date relative to
     * UTC (GMT).
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     *
     * @note
     * This method throws an exception if called on a sentinel date. Otherwise,
     * it returns a valid day of year.
     */
    unsigned
    dayGMT() const;

    /**
     * Returns the day of the month (1-31) for this date. The default for
     * the optional timezone argument is local.
     */
    unsigned
    dayOfMonth(const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the day of the month (1-31) for this date relative to
     * UTC (GMT).
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     */
    unsigned dayOfMonthGMT() const;

    /**
     * Decrements the class's milliseconds data member by
     * \a d * millisecsInDay(), then returns self.
     *
     * @note if this operation results in crossing a DST boundary in the
     * instance's timezone, outputting the date will present with a DST
     * offset as well.
     */
    RWDateTime&
    decrementDay(const rwint64 d);

    /**
     * Decrements the class's milliseconds data member by
     * \a h * millisecsInHour(), then returns self.
     *
     * @note If this operation results in crossing a DST boundary in the
     * instance's timezone, outputting the date will present with a DST
     * offset as well.
     */
    RWDateTime&
    decrementHour(const rwint64 h);

    /**
     * Decrements the class's milliseconds data member by
     * \a m * millisecsInMin(), then returns self.
     *
     * @note If this operation results in crossing a DST boundary in the
     * instance's timezone, outputting the date will present with a DST
     * offset as well.
     */
    RWDateTime&
    decrementMinute(const rwint64 m);

    /**
     * Decrements the class's milliseconds data member by
     * \a s * millisecsInSec(), then returns self.
     *
     * @note If this operation results in crossing a DST boundary in the
     * instance's timezone, outputting the date will present with a DST
     * offset as well.
     */
    RWDateTime&
    decrementSecond(const rwint64 s);

    /**
     * Decrements the class's milliseconds data member by \a msec,
     * then returns self.
     *
     * @note If this operation results in crossing a DST boundary in the
     * instance's timezone, outputting the date will present with a DST
     * offset as well.
     */
    RWDateTime&
    decrementMillisecond(const rwint64 msec);

    /**
     * Returns with the <tt>struct tm</tt> argument filled out completely.
     * Note that the encoding for months and days of the week used in
     * <tt>struct tm</tt> differs from that used elsewhere in RWDateTime.
     * If the date is a sentinel, the function throws an exception of
     * type RWInternalErr.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     *
     * As a precondition, \a tmbuf is tested to determine if it is a
     * null pointer. If null, the method asserts in debug mode, and
     * throws RWInternalErr in optimized builds.
     */
    void
    extract(struct tm* tmbuf, const RWZone& zone = RWZone::local()) const;

    /**
     * Returns with the <tt>struct tm</tt> argument filled out completely,
     * relative to UTC (GMT). Note that the encoding for months and
     * days of the week used in <tt>struct tm</tt> differs from that used
     * elsewhere in RWDateTime. If the date is a sentinel, the function
     * throws an exception of type RWInternalErr.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     *
     * As a precondition, \a tmbuf is tested to determine if it is a
     * null pointer. If null, the method asserts in debug mode, and
     * throws RWInternalErr in optimized builds.
     */
    void
    extractGMT(struct tm* tmbuf) const;

    /**
     * Returns the day of the year (1-366) corresponding to the first
     * day of \a mon of the year for this RWDateTime. The default for the
     * optional timezone argument is local.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    unsigned
    firstDayOfMonth(unsigned mon, const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the day of the year (1-366) corresponding to the first
     * day of the month and year for this RWDateTime.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    unsigned
    firstDayOfMonth(const RWZone& zone = RWZone::local()) const {
        return firstDayOfMonth(month(zone), zone);
    }

    /**
     * Returns a suitable hashing value.
     */
    unsigned hash() const;

    /**
     * Returns the hour relative to the timezone specified. The default
     * for the optional timezone argument is local.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    unsigned hour(const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the hour relative to UTC (GMT).
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     */
    unsigned hourGMT() const;

    /**
     * Increments the class's milliseconds data member by
     * \a d * millisecsInDay(), then returns self.
     *
     * @note If this operation results in crossing a DST boundary in the
     * instance's timezone, outputting the date will present with a DST
     * offset as well.
     */
    RWDateTime&
    incrementDay(const rwint64 d);

    /**
     * Increments the class's milliseconds data member by
     * \a h * millisecsInHour(), then returns self.
     *
     * @note If this operation results in crossing a DST boundary in the
     * instance's timezone, outputting the date will present with a DST
     * offset as well.
     */
    RWDateTime&
    incrementHour(const rwint64 h);

    /**
     * Increments the class's milliseconds data member by
     * \a m * millisecsInMin(), then returns self.
     *
     * @note If this operation results in crossing a DST boundary in the
     * instance's timezone, outputting the date will present with a DST
     * offset as well.
     */
    RWDateTime&
    incrementMinute(const rwint64 m);

    /**
     * Increments the class's milliseconds data member by
     * \a s * millisecsInSec(), then returns self.
     *
     * @note If this operation results in crossing a DST boundary in the
     * instance's timezone, outputting the date will present with a DST
     * offset as well.
     */
    RWDateTime&
    incrementSecond(const rwint64 s);

    /**
     * Increments the class's milliseconds data member by \a msec,
     * then returns self.
     *
     * @note If this operation results in crossing a DST boundary in the
     * instance's timezone, outputting the date will present with a DST
     * offset as well.
     */
    RWDateTime&
    incrementMillisecond(const rwint64 msec);

    /**
     * Returns \c true if self is during daylight saving time in the
     * timezone given by \a zone, \c false otherwise.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     */
    bool
    isDST(const RWZone& zone = RWZone::local()) const;

    /**
     * Returns \c true if self is a future sentinel.
     */
    bool isFuture() const {
        return msec_ == futureSentinel;
    }

    /**
     * Returns \c true if self is a past sentinel.
     */
    bool isPast() const {
        return msec_ == pastSentinel;
    }

    /**
     * Returns \c true if self is a null sentinel.
     */
    bool isNull() const {
        return msec_ == nullSentinel;
    }

    /**
     * Returns \c true if self is an invalid sentinel.
     */
    bool isInvalid() const {
        return msec_ == invalidSentinel;
    }

    /**
     * Returns \c true if self is a sentinel of any kind.
     */
    bool isSentinel() const {
        return (msec_ == futureSentinel || msec_ <= pastSentinel);
    }

    /**
     * Returns \c true if self represents an actual time or is the null
     * sentinel, \c false otherwise.
     */
    bool isValid() const {

#if defined(__INTEL_COMPILER) && __INTEL_COMPILER <= 1000

        // Workaround for Intel C++ bug #443810
        return
            (lessThanFutureSentinel() && moreThanPastSentinel()) ||
            msec_ == nullSentinel;

#else

        return
            (msec_ > pastSentinel && msec_ < futureSentinel) ||
            msec_ == nullSentinel;

#endif // __INTEL_COMPILER && __INTEL_COMPILER <= 1000

    }

    /**
     * Returns the value of the current date and time as a Modified
     * Julian day number. The integer/whole number part of the double
     * indicates the day, and the fractional part indicates the time.
     * For example, the Modified Julian day number for 1/1/1901 is 2415386.
     * Since Modified Julian days start at midnight GMT, 12 a.m. is
     * 2415386.5 and 6 p.m. is 2415386.75 (these values are computed
     * for the Greenwich meridian - a different timezone modifies the result
     * of \c julianDay).
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     */
    double
    julianDay() const;

    /**
     * Sets the value of the date and time to the Modified Julian day
     * number \a j. The integer/whole number part of the double indicates
     * the day, and the fractional part indicates the time. For example,
     * the Julian day number for 1/1/1901 is 2415386. Since Julian
     * days start at midnight GMT, 12 a.m. is 2415386.5 and 6 p.m. is
     * 2415386.75 (these values are interpreted differently in RWDateTime
     * objects that have different timezones).
     *
     * This method produces a valid output for values of \a j from 0 to the
     * maximum value that can be stored in an <tt>unsigned</tt>. Attempts
     * to invoke this method with values outside that range will result in
     * undefined behavior.
     */
    void
    julianDay(double j);

    /**
     * Returns the value of the Modified Julian day number.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     */
    unsigned julian() const;

    /**
     * Sets the value of the Modified Julian day number to \a j.
     */
    void julian(unsigned j);

    /**
     * Returns \c true if self is a leap year relative to the RWZone
     * argument.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    bool
    leap(const RWZone& zone = RWZone::local()) const {
        return leapYear(year(zone));
    }

    /**
     * Returns \c true if self is a leap year relative to UTC (GMT).
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     */
    bool leapGMT() const {
        return leapYear(yearGMT());
    }

    /**
     * Returns the later RWDateTime of self or \a t.
     */
    RWDateTime
    RW_MACRO_GUARD(max)(const RWDateTime& t) const {
        RW_PRECONDITION(msec_ >= pastSentinel);
        RW_PRECONDITION(t.msec_ >= pastSentinel);

        if (msec_ < pastSentinel || t.msec_ < pastSentinel) {
            RWTHROW(RWInternalErr(RWMessage(RWTOOL_DTINVALIDOP)));
        }

        return milliSeconds() > t.milliSeconds() ? *this : t;
    }

    /**
     * Returns the earlier RWDateTime of self or \a t.
     */
    RWDateTime
    RW_MACRO_GUARD(min)(const RWDateTime& t) const {
        RW_PRECONDITION(msec_ >= pastSentinel);
        RW_PRECONDITION(t.msec_ >= pastSentinel);

        if (msec_ < pastSentinel || t.msec_ < pastSentinel) {
            RWTHROW(RWInternalErr(RWMessage(RWTOOL_DTINVALIDOP)));
        }

        return milliSeconds() < t.milliSeconds() ? *this : t;
    }

    /**
     * Returns the number of milliseconds since 00:00:00:000 January
     * 1, 1901 UTC.
     */
    rwint64
    milliSeconds() const {
        return msec_;
    }

    /**
     * Returns the millisecond part of self.
     */
    unsigned
    milliSecond() const;

    /**
     * Returns the minute, adjusted to the timezone specified.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    unsigned
    minute(const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the minute relative to UTC (GMT).
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     */
    unsigned
    minuteGMT() const;

    /**
     * Returns the month (1-12) for this date. The default for the
     * optional timezone argument is local.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    unsigned
    month(const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the month (1-12) for this date relative to UTC (GMT).
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     */
    unsigned
    monthGMT() const;

    /**
     * Returns the name of the month for this date, according to the
     * optional locale argument. The function defaults to using the
     * RWLocale global locale.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    RWCString
    monthName(const RWLocale& loc = RWLocale::global(),
              const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the date of the next day of the week, where \e Monday
     * = 1, ..., \e Sunday = 7. The variable \a dayNum must be between
     * 1 and 7, inclusive. The default for the optional timezone argument
     * is local.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    RWDateTime
    next(unsigned dayNum, const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the date of the next \a dayName (for example, the date
     * of the next Monday). The weekday name is interpreted according
     * to the optional locale argument.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     *
     * As a precondition, \a dayName is tested to determine if it is a
     * null pointer. If null, the method asserts in debug mode, and
     * throws RWInternalErr in optimized builds.
     */
    RWDateTime
    next(const char* dayName,
         const RWLocale& loc = RWLocale::global(),
         const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the date of the previous numbered day of the week, where
     * \e Monday = 1, ..., \e Sunday = 7. The variable \a dayNum
     * must be between 1 and 7, inclusive. The default for the optional
     * timezone argument is local.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    RWDateTime
    previous(unsigned dayNum, const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the day of the previous \a dayName; for example, the
     * date of the previous Monday. The weekday name is interpreted
     * according to the optional locale argument. The function defaults
     * to using the RWLocale global locale.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     *
     * As a precondition, \a dayName is tested to determine if it is a
     * null pointer. If null, the method asserts in debug mode, and
     * throws RWInternalErr in optimized builds.
     */
    RWDateTime
    previous(const char* dayName,
             const RWLocale& loc = RWLocale::global(),
             const RWZone& zone = RWZone::local()) const;

    /**
     * Reads a full line and converts it to a time according to the locale
     * imbued on the stream. The member function isValid() must be used
     * to test whether the results are a valid time.
     */
    std::istream&
    readTime(std::istream& is);

    /**
     * Reads a full line and converts to a date according to the locale
     * imbued on the stream. The member function isValid() must be used
     * to test whether the results are a valid date.
     *
     * This method handles date values with years in the range 0 to 9999.
     * Passing a stream that holds a year outside this range will
     * result in undefined behavior. Year values 0 to 99 are treated
     * differently based on how the library was built. In all builds
     * that do not define the macro #RW_CENTURY_REQD, years 0 to 99 are
     * treated as 1900 to 1999. Debug builds that define the macro will
     * produce a debug assertion. Release builds that define the macro
     * will produce an invalid RWDateTime instance.
     */
    std::istream&
    readDate(std::istream& is);

    /**
     * Outputs the date on the \b std::ostream \a os, according to the
     * locale imbued on the stream.
     */
    std::ostream&
    writeDate(std::ostream& os) const;

    /**
     * Outputs the time on the \b std::ostream \a os, according to the
     * locale imbued on the stream.
     */
    std::ostream&
    writeTime(std::ostream& os) const;

    /**
     * Converts self to an RWDate.
     */
    RWDate
    toRWDate(const RWZone& zone = RWZone::local()) const;

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    /**
     * Converts self to an RWTime.
     */
    RWTime
    toRWTime() const;

    RW_RESTORE_DEPRECATED_WARNINGS
#endif

    /**
     * Restores an instance of RWDateTime from a reference to an RWFile
     * into which an instance has been persisted using the method
     * saveOn(RWFile&)const.
     */
    void restoreFrom(RWFile&);

    /**
     * Restores an instance of RWDateTime from a reference to a virtual
     * stream into which an instance has been persisted using the method
     * saveOn(RWvostream&)const.
     */
    void restoreFrom(RWvistream&);

    /**
     * Saves an instance of RWDateTime to the indicated RWFile object
     * reference. The RWDateTime can be restored using the method
     * restoreFrom(RWFile&).
     */
    void saveOn(RWFile&) const;

    /**
     * Saves an instance of RWDateTime to the indicated virtual stream.
     * The RWDateTime can be restored using the method
     * restoreFrom(RWvistream&).
     */
    void saveOn(RWvostream&) const;

    /**
     * Returns the second, adjusted to the timezone specified.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    unsigned
    second(const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the second relative to UTC (GMT).
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     */
    unsigned
    secondGMT() const;

    /**
     * Returns the number of the day of the week for this date, where
     * \e Monday = 1, ..., \e Sunday = 7.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    unsigned
    weekDay(const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the name of the day of the week for this date, according
     * to the optional locale argument. The function defaults
     * to using the RWLocale global locale.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    RWCString
    weekDayName(const RWLocale& loc = RWLocale::global(),
                const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the number of the day of the week for this date relative
     * to UTC (GMT).
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     */
    unsigned
    weekDayGMT() const;

    /**
     * Returns the year of self. The default for the optional
     * timezone argument is local.
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000 and 11754508-12-13T23:59:59.999 in the
     * specified time zone. Attempts to invoke this method with values
     * outside that range will result in undefined behavior.
     */
    unsigned
    year(const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the year of self relative to UTC (GMT).
     *
     * This method produces a valid output for date-time values between
     * -4713-11-24T00:00:00.000Z and 11754508-12-13T23:59:59.999Z.
     * Attempts to invoke this method with values outside that range will
     * result in undefined behavior.
     */
    unsigned
    yearGMT() const;

    /**
     * Returns the start of daylight saving time (DST) for the given
     * year, in the given timezone. Returns an "invalid" time if DST
     * is not observed in that year and zone.
     */
    static RWDateTime
    beginDST(unsigned year, const RWZone& zone = RWZone::local());

    /**
     * Returns the number of days in a given month and year. Returns
     * 0 if \a month is not between 1 and 12 inclusive.
     */
    static unsigned
    daysInMonthYear(unsigned month, unsigned year);

    /**
     * Returns the number of days in a given year.
     */
    static unsigned
    daysInYear(unsigned year);

    /**
     * Returns the number of the day of the week corresponding to the
     * given \e day, where \e Monday = 1, ..., \e Sunday = 7.
     * Names are interpreted according to the optional locale argument.
     * The function defaults to using the RWLocale global locale.
     *
     * As a precondition, \a day is tested to determine if it is a
     * null pointer. If null, the method asserts in debug mode, and
     * throws RWInternalErr in optimized builds.
     */
    static unsigned
    dayOfWeek(const char* day, const RWLocale& loc = RWLocale::global());

    /**
     * Returns \c true if \a day (1-31) is within a given month in
     * a given year.
     */
    static bool
    dayWithinMonth(unsigned month, unsigned day, unsigned year);

    /**
     * Returns the end of daylight saving time for the given year, in
     * the given timezone. Returns an "invalid time" if DST is not
     * observed in that year and zone.
     */
    static RWDateTime
    endDST(unsigned year, const RWZone& zone = RWZone::local());

    /**
     * Returns the hash value of \a dt as returned by
     * \link hash()const dt.hash() \endlink .
     */
    static unsigned
    hash(const RWDateTime& dt) {
        return dt.hash();
    }

    /**
     * Returns a number between 1 and 12 corresponding to the given
     * month name \a monthName. Returns 0 for no match. Months are interpreted
     * by the optional locale argument. The function defaults to using
     * the RWLocale global locale.
     *
     * As a precondition, \a monthName is tested to determine if it is a
     * null pointer. If null, the method asserts in debug mode, and
     * throws RWInternalErr in optimized builds.
     */
    static unsigned
    indexOfMonth(const char* monthName, const RWLocale& loc = RWLocale::global());

    /**
     * Returns \c true if a given \a year is a leap year.
     */
    static bool
    leapYear(unsigned year);

    /**
     * Returns current date and time.
     */
    static RWDateTime
    now() {
        return RWDateTime(RWDateTime::currentTime());
    }

    /**
     * Returns the name of month, where \e January = 1, ..., \e December
     * = 12. Months are interpreted by the optional locale argument.
     * The function defaults to using the RWLocale global locale.
     */
    static RWCString
    nameOfMonth(unsigned monthNum, const RWLocale& loc = RWLocale::global());

    /**
     * Returns the name of the day of the week \a dayNum where
     * \e Monday = 1, ..., \e Sunday = 7. Days are interpreted by the optional
     * locale argument. The function defaults to using the RWLocale
     * global locale.
     */
    static RWCString
    weekDayName(unsigned dayNum, const RWLocale& loc = RWLocale::global());

    /**
     * Returns user-defined sentinel number \a n.
     *
     * @throw RWBoundsErr Thrown if the argument is outside the range
     * 0 to 127.
     */
    static RWDateTime
    userSentinel(int n);

    // Static const data
    static const unsigned char daysInMonth[12];         // [1..31]
    static const unsigned int  firstDayOfEachMonth[12]; // [0..334]

protected:

    static bool
    assertWeekDayNumber(unsigned int d) {
        return d >= 1 && d <= 7;
    }

    static bool
    assertIndexOfMonth(unsigned int m) {
        return m >= 1 && m <= 12;
    }

private:

#if defined(__INTEL_COMPILER) && __INTEL_COMPILER <= 1000

    // Workaround for Intel C++ bug #443810
    bool
    lessThanFutureSentinel() const {
        return msec_ < futureSentinel;
    }

    bool
    moreThanPastSentinel() const {
        return msec_ > pastSentinel;
    }

#endif // __INTEL_COMPILER && __INTEL_COMPILER <= 1000

    void doExtract(struct tm*, const RWZone&) const;

    void doExtractGMT(struct tm*) const;

    unsigned doCorrectedJulian() const;

    unsigned doJulian() const;

    double doJulianDay() const;

    unsigned doHourGMT() const;

    unsigned doMinuteGMT() const;

    unsigned doSecondGMT() const;

    unsigned doWeekDayGMT() const;

    static rwint64
    buildFrom(int,
              unsigned, unsigned, unsigned, unsigned, unsigned, unsigned,
              const RWZone& zone = RWZone::local());

    static rwint64
    buildFrom(unsigned long,
              unsigned, unsigned, unsigned, unsigned,
              const RWZone& zone = RWZone::local());

    static rwint64
    currentTime();

    static unsigned long
    jday(unsigned, unsigned, unsigned);

    static void
    mdy_from_julian(unsigned, unsigned&, unsigned&, unsigned&);

    static RWCString
    asString(char, const struct tm*, const RWLocale& loc = RWLocale::global());

    void
    initFromString(const RWCString&, SetType,
                   const RWLocale&, const RWZone&);

    void mdy(unsigned& month, unsigned& day, unsigned& year) const;

    // methods that are here to prevent users from using them...
    RWDateTime& operator++ ();
    RWDateTime  operator++ (int);

    RWDateTime& operator-- ();
    RWDateTime  operator-- (int);

    RWDateTime& operator+= (rwint64 ms);
    RWDateTime& operator-= (rwint64 ms);

    /**
     * Outputs the date and time stored in \a dt on \b std::ostream
     * \a s, according to the locale imbued on the stream.
     */
    friend RW_TOOLS_SYMBOLIC std::ostream&
    operator<< (std::ostream& s, const RWDateTime& dt);

    /**
     * Reads \a dt from \b std::istream \a s. The function reads one
     * full line, and the string it contains is converted according
     * to the locale imbued on the stream. The function isValid()
     * must be used to test whether the results are a valid date.
     */
    friend RW_TOOLS_SYMBOLIC std::istream&
    operator>> (std::istream& s, RWDateTime& dt);

    /**
     * Returns the date \a s milliseconds in the future from the date
     * \a d.
     */
    friend RW_TOOLS_SYMBOLIC RWDateTime
    operator+ (const RWDateTime& d, rwint64 s);

    /**
     * @copydoc operator+(const RWDateTime&, rwint64)
     */
    friend RW_TOOLS_SYMBOLIC RWDateTime
    operator+ (rwint64 s, const RWDateTime& d);

    /**
     * Returns the date \a s milliseconds in the past from \a d.
     */
    friend RW_TOOLS_SYMBOLIC RWDateTime
    operator- (const RWDateTime& d, rwint64 s);

    /**
     * Returns the number of milliseconds between \a dt1 and \a dt2.
     *
     * @throw RWInternalErr Thrown if either \a dt1 or \a dt2 is a sentinel.
     */
    friend RW_TOOLS_SYMBOLIC rwint64
    operator- (const RWDateTime& dt1, const RWDateTime& dt2);

private:

    rwint64 msec_;

};


inline RWDateTime&
RWDateTime::incrementDay(const rwint64 n)
{
    *this += n * millisecsInDay;
    return *this;
}

inline RWDateTime&
RWDateTime::incrementHour(const rwint64 n)
{
    *this += n * millisecsInHour;
    return *this;
}

inline RWDateTime&
RWDateTime::incrementMinute(const rwint64 n)
{
    *this += n * millisecsInMin;
    return *this;
}

inline RWDateTime&
RWDateTime::incrementSecond(const rwint64 n)
{
    *this += n * millisecsInSec;
    return *this;
}

inline RWDateTime&
RWDateTime::incrementMillisecond(const rwint64 milliseconds)
{
    *this += milliseconds;
    return *this;
}

inline RWDateTime&
RWDateTime::decrementDay(const rwint64 n)
{
    *this -= n * millisecsInDay;
    return *this;
}

inline RWDateTime&
RWDateTime::decrementHour(const rwint64 n)
{
    *this -= n * millisecsInHour;
    return *this;
}

inline RWDateTime&
RWDateTime::decrementMinute(const rwint64 n)
{
    *this -= n * millisecsInMin;
    return *this;
}

inline RWDateTime&
RWDateTime::decrementSecond(const rwint64 n)
{
    *this -= n * millisecsInSec;
    return *this;
}

inline RWDateTime&
RWDateTime::decrementMillisecond(const rwint64 milliseconds)
{
    *this -= milliseconds;
    return *this;
}


inline void
RWDateTime::extract(struct tm* tmbuf, const RWZone& zone) const
{
    if (RWDateTime::isSentinel()) {
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_DTINVALIDOP)));
    }

    doExtract(tmbuf, zone);
}


inline void
RWDateTime::extractGMT(struct tm* tmbuf) const
{
    if (RWDateTime::isSentinel()) {
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_DTINVALIDOP)));
    }

    doExtractGMT(tmbuf);
}

inline unsigned
RWDateTime::correctedJulian() const
{
    RW_PRECONDITION(!RWDateTime::isSentinel());
    if (RWDateTime::isSentinel()) {
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_DTINVALIDOP)));
    }

    return doCorrectedJulian();
}

// Returns the hour represented in *this relative to GMT.
inline unsigned
RWDateTime::hourGMT() const
{
    RW_PRECONDITION(!RWDateTime::isSentinel());
    if (RWDateTime::isSentinel()) {
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_DTINVALIDOP)));
    }
    return doHourGMT();
}


inline unsigned
RWDateTime::julian() const
{
    RW_PRECONDITION(!RWDateTime::isSentinel());
    if (RWDateTime::isSentinel()) {
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_DTINVALIDOP)));
    }
    return doJulian();
}


inline unsigned
RWDateTime::doWeekDayGMT() const
{
    return unsigned(julian() % 7 + 1);
}


inline unsigned
RWDateTime::weekDayGMT() const
{
    RW_PRECONDITION(!RWDateTime::isSentinel());
    if (RWDateTime::isSentinel()) {
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_DTINVALIDOP)));
    }
    return doWeekDayGMT();
}

inline unsigned
RWDateTime::secondGMT() const
{
    RW_PRECONDITION(!RWDateTime::isSentinel());
    if (RWDateTime::isSentinel()) {
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_DTINVALIDOP)));
    }
    return doSecondGMT();
}

inline unsigned
RWDateTime::minuteGMT() const
{
    RW_PRECONDITION(!RWDateTime::isSentinel());
    if (RWDateTime::isSentinel()) {
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_DTINVALIDOP)));
    }
    return doMinuteGMT();
}

/**
 * @relates RWDateTime
 * Restores  \a t from a virtual stream, replacing the previous contents
 * of \a t.
 */
inline RWvistream&
operator>> (RWvistream& str, RWDateTime& t)
{
    t.restoreFrom(str);
    return str;
}

/**
 * @relates RWDateTime
 * Restores  \a t from an RWFile, replacing the previous contents
 * of \a t.
 */
inline RWFile&
operator>> (RWFile& file, RWDateTime& t)
{
    t.restoreFrom(file);
    return file;
}


/**
 * @relates RWDateTime
 * Restores an RWDateTime from a virtual stream by allocating an RWDateTime
 * on the heap and restoring its state.
 */
inline RWvistream&
operator>> (RWvistream& str, RWDateTime*& t)
{
    t = new RWDateTime;
    t->restoreFrom(str);
    return str;
}

/**
 * @relates RWDateTime
 * Restores an RWDateTime from an RWFile by allocating an RWDateTime on the
 * heap and restoring its state.
 */
inline RWFile&
operator>> (RWFile& file, RWDateTime*& t)
{
    t = new RWDateTime;
    t->restoreFrom(file);
    return file;
}

/**
 * @relates RWDateTime
 * Saves \a t to a virtual stream.
 */
inline RWvostream&
operator<< (RWvostream& str, const RWDateTime& t)
{
    t.saveOn(str);
    return str;
}

/**
 * @relates RWDateTime
 * Saves \a t to an RWFile.
 */
inline RWFile&
operator<< (RWFile& file, const RWDateTime& t)
{
    t.saveOn(file);
    return file;
}

// Relational operators
/**
 * @relates RWDateTime
 * Returns \c true if \a d1 is the same as \a d2.
 */
RW_TOOLS_SYMBOLIC bool
operator== (const RWDateTime& d1, const RWDateTime& d2);

/**
 * @relates RWDateTime
 * Returns \c true if \a d1 is not the same as \a d2.
 */
RW_TOOLS_SYMBOLIC bool
operator!= (const RWDateTime& d1, const RWDateTime& d2);

/**
 * @relates RWDateTime
 * Returns \c true if \a d1 is before \a d2.
 */
RW_TOOLS_SYMBOLIC bool
operator< (const RWDateTime& d1, const RWDateTime& d2);

/**
 * @relates RWDateTime
 * Returns \c true if \a d1 is after \a d2.
 */
RW_TOOLS_SYMBOLIC bool
operator> (const RWDateTime& d1, const RWDateTime& d2);

/**
 * @relates RWDateTime
 * Returns \c true if \a d1 is before or the same as \a d2.
 */
RW_TOOLS_SYMBOLIC bool
operator<= (const RWDateTime& d1, const RWDateTime& d2);

/**
 * @relates RWDateTime
 * Return \c true if \a d1 is after or the same as \a d2.
 */
RW_TOOLS_SYMBOLIC bool
operator>= (const RWDateTime& d1, const RWDateTime& d2);

// Source compatibility
inline rwint64
UL2rwint64(const unsigned long x)
{
    return rwint64(x);
}

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWDateTime.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWDateTime> : std::unary_function<RWDateTime, size_t> {
    /**
     * Returns a hash of \a obj. This function is the equivalent of calling
     *
     * @code
     * obj.hash();
     * @endcode
     */
    size_t operator()(const RWDateTime& obj) const {
        return obj.hash();
    }
};

#endif /* RW_TOOLS_DATETIME_H */

