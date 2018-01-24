#ifndef RW_TOOLS_DATE_H
#define RW_TOOLS_DATE_H



/**********************************************************************
 *
 * Declarations for RWDate Class
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rwdate.h#1 $
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
#include <rw/zone.h>
#include <rw/locale.h>

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS

class RW_TOOLS_SYMBOLIC  RWTime;

RW_RESTORE_DEPRECATED_WARNINGS
#endif

class RW_TOOLS_GLOBAL RWCString;
class RW_TOOLS_GLOBAL  RWLocale;

// The Julian day number at 12:00:00 on Jan 1st 1901
const unsigned long rwJul1901 = 2415386L;

/**
 * @ingroup date_and_time_classes
 *
 * @brief Represents a date stored as a Julian day number.
 *
 * Class RWDate represents a date, stored as a Julian day number.
 * The member function isValid() can be used to determine whether
 * an RWDate is a valid date.  For example, isValid() would return
 * \c false for the date 29 February 1991 because 1991 is not a
 * leap year. See the chapter on date and time classes in the <i>Essential
 * Tools Module User's Guide</i>.
 *
 * @note
 * RWDate instances can be converted to and from the C Standard
 * Library type <tt>struct tm</tt> defined in \c <time.h>.
 *
 * We recommend creating programs that use four-digit year specifiers.
 * For more information, see the section on RWDate in the <i>Essential
 * Tools Module User's Guide</i>.
 *
 * Note that because the default constructor for this class creates
 * an instance holding the current date, constructing a large array
 * of RWDate may be slow.
 *
 * @code
 *
 * RWDate v[5000]; // Figures out the current date 5000 times
 * @endcode
 *
 * When using the Essential Tools Module template collections,
 * consider the following:
 *
 * @code
 *
 * // Figures out the current date just once:
 * RWTValOrderedVector<RWDate> v(5000, RWDate());
 * @endcode
 *
 * Thanks to the smart allocation scheme of the standard collections,
 * the above declaration results in only one call to the default
 * constructor, followed by 5000 invocations of the copy constructor.
 *  In the case of RWDate, the copy constructor amounts to an assignment
 * of one \c long to another, resulting in faster creation than
 * the simple array.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rwdate.h>
 * RWDate a;   // Construct today's date
 * @endcode
 *
 * @section persistence Persistence
 * Simple
 *
 * @section example Example
 *
 * @code
 * #include <rw/rwdate.h>
 *
 * int main ()
 * {
 *     // ENIAC start date
 *     RWDate d (14, "February", 1945);
 *
 *     // Today
 *     RWDate today;
 *
 *     std::cout << d.asString("%A, %B %d 19%y")
 *               << " was the day the ENIAC computer was\nfirst turned on. "
 *               << today - d << " days have gone by since then. " << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 */
class RW_TOOLS_GLOBAL RWDate
{
public:

    /**
     * Default constructor.  Constructs an RWDate with the present date.
     */
    RWDate() : julnum(RWDate::currentDate()) {
        ;
    }

    /**
     * Constructs a date from the Julian Day number \a jd.  Note that
     * it is possible to construct a valid RWDate that represents a
     * day previous to the beginning of the Gregorian calendar for some
     * locality.  Rogue Wave doesn't know the specifics for your locality,
     * so will not enforce an arbitrary cutoff for "validity."
     */
    RWDate(unsigned long jd) : julnum(jd) {
        ;   // Julian day; beware type conversion
    }

    /* Construct a RWDate with a given day of the year and a given year.
       The base date for this computation is Dec. 31 of the previous year.
       If year == 0, Construct a RWDate with Jan. 1, 1901 as the "day zero".
          i.e., RWDate(-1) = Dec. 31, 1900 and RWDate(1) = Jan. 2, 1901. */
    /**
     * Constructs an RWDate with a given day of the year and a given
     * year.  The member function isValid() can be used to test whether
     * the results are a valid date.
     */
    RWDate(unsigned day, unsigned year);

    /**
     * Constructs an RWDate with the given day of the month, month, and
     * year.  The locale argument is used to convert the month name.
     *  Days should be 1-31, months may be specified as (for example):
     * January, JAN, or Jan, and the year may be specified as (for example)
     * 1990, or 90.  The member function isValid() can be used to test
     * whether the results are a valid date.
     */
    RWDate(unsigned day, const char* month, unsigned year,
           const RWLocale& loc = RWLocale::global()); // for month names

    /**
     * Constructs an RWDate with the given day of the month, month of
     * the year, and year.  Days should be 1-31, months should be 1-12,
     * and the year may be specified as (for example) 1990, or 90.
     * The member function isValid() can be used to test whether the
     * results are a valid date.
     */
    RWDate(unsigned day, unsigned month, unsigned year);

    /**
     * Constructs a date whereby the locale argument reads and converts
     * a full line to a date. Because RWLocale cannot rigorously check
     * date input, the member function isValid() must be used
     * to test whether the results are a valid date.
     */
    RWDate(std::istream& s, const RWLocale& loc = RWLocale::global()) {
        parseFrom(s, loc);   // Read date from stream.
    }

    /**
     * Converts the string \a str to a date.  Because RWLocale cannot
     * rigorously check date input, the member function isValid()
     * must be used to test whether the results are a valid date.
     */
    RWDate(const RWCString& str, const RWLocale& loc = RWLocale::global());

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS
    /**
     * Constructs an RWDate from an RWTime. The time zone used defaults
     * to local.  The member function isValid() must be used to test
     * whether the results are a valid date.
     */
    RW_DEPRECATE_FUNC("Use RWDateTime::toRWDate(const RWZone&) instead")
    RWDate(const RWTime& t, const RWZone& zone = RWZone::local());

    RW_RESTORE_DEPRECATED_WARNINGS
#endif

    /**
     * Constructs an RWDate from the contents of the <tt>struct tm </tt>
     * argument members \c tm_year, \c tm_mon, and \c tm_mday.  Note
     * that the numbering of months and years used in <tt>struct tm </tt>
     * differs from that used for RWDate operations. <tt>struct tm </tt>
     * is declared in the C Standard Library include file \c \<time.h\>.
     */
    RWDate(const struct tm* tmbuf);  // from members: tm_year, tm_mon, tm_mday

    // The following two should not be necessary, but are required to
    // support feeble compilers:
    /**
     * Copy constructor.
     */
    RWDate(const RWDate& d) : julnum(d.julnum) {
        ;
    }

    /**
     * Assignment operator.
     */
    RWDate& operator=(const RWDate& d) {
        julnum = d.julnum;
        return *this;
    }

    /**
     * Returns the date as a string, formatted by the optional RWLocale
     * argument.  Formats are as defined in the C99 Standard C Library
     * function \c strftime(), explained in the description for RWLocale.
     */
    RWCString             asString(char format = 'x',
                                   const RWLocale& loc = RWLocale::global()) const;

    /**
     * Returns the date as a string, formatted by the optional RWLocale
     * argument.  Formats are as defined in the C99 Standard C Library
     * function \c strftime(), explained in the description for RWLocale.
     */
    RWCString             asString(const char* format,
                                   const RWLocale& loc = RWLocale::global()) const;

    /**
     * Returns \c true  if this RWDate is between \a a and \a b, inclusive.
     */
    bool             between(const RWDate& a, const RWDate& b) const {
        if (a <= b) {
            return julnum >= a.julnum && julnum <= b.julnum;
        }
        else {
            return julnum >= b.julnum && julnum <= a.julnum;
        }
    }

    /**
     * Compares self to the RWDate pointed to by \a d and returns:
     *
     *     0   if self == *\a d;
     *
     *     1   if self > *\a d;
     *
     *    -1   if self < *\a d.
     */
    int                   compareTo(const RWDate* d) const;

    int                   compareTo(const RWDate& d) const {
        return compareTo(&d);
    }
    /**
     * Returns the day of the year (1-366) for this date.
     */
    unsigned              day() const;

    /**
     * Returns the day of the month (1-31) for this date.
     */
    unsigned              dayOfMonth() const;

    /**
     * Returns with the <tt>struct tm</tt> argument filled out completely,
     * with the time members set to 0 and \c tm_isdst set to -1.  Note
     * that the encoding for months and days of the week used in
     * <tt>struct tm</tt> differs from that used elsewhere in RWDate.  If
     * the date is invalid, all fields are set to  -1.
     */
    void                  extract(struct tm* tmbuf) const;

    /**
     * Returns the day of the year (1-366) corresponding to the first
     * day of the month \a month (1-12) of the year for this RWDate.
     */
    unsigned              firstDayOfMonth(unsigned month) const;

    /**
     * Returns the day of the year (1-366) corresponding to the first
     * day of the month and year for this RWDate.
     */
    unsigned              firstDayOfMonth() const {
        return firstDayOfMonth(month());
    }

    /**
     * Returns a suitable hashing value.
     */
    unsigned              hash() const;

    /**
     * Returns \c true if \c this is a valid date, \c false otherwise.
     */
    bool             isValid() const {
        return julnum > 0;
    }

    /**
     * Returns the value of the Julian day number.
     */
    unsigned long         julian() const {
        return julnum;
    }

    /**
     * Changes the value of the Julian day number to \a j.
     */
    void                  julian(const unsigned long j) {
        julnum = j;
    }

    /**
     * Returns \c true if the year of this RWDate is a leap year,
     * otherwise \c false.
     */
    bool             leap() const {
        return leapYear(year());   // leap year?
    }

    /**
     * Returns the later date of self or \a dt.
     */
    RWDate
    RW_MACRO_GUARD(max)(const RWDate& dt) const {
        return dt.julnum > julnum ? dt : *this;
    }

    /**
     * Returns the earlier date of self or \a dt.
     */
    RWDate
    RW_MACRO_GUARD(min)(const RWDate& dt) const {
        return dt.julnum < julnum ? dt : *this;
    }

    /**
     * Returns the month (1-12) for this date.
     */
    unsigned              month() const;

    /**
     * Returns the name of the month for this date, according to the
     * optional RWLocale argument.
     */
    RWCString             monthName(const RWLocale& loc = RWLocale::global()) const;

    /**
     * Returns the date of the next \a dayName (for example, the date
     * of the previous Monday)  The weekday name is interpreted according
     * to the optional RWLocale argument.
     */
    RWDate                next(const char* dayName,
                               const RWLocale& loc = RWLocale::global()) const;

    /**
     * Returns the date of the next numbered day of the week, where
     * \e Monday = 1, ..., \e Sunday = 7.  The variable \a dayNum
     * must be between 1 and 7, inclusive.
     */
    RWDate                next(unsigned dayNum) const;

    /**
     * Returns the date of the previous \a dayName (for example, the
     * date of the previous Monday)  The weekday name is interpreted
     * according to the optional RWLocale argument.
     */
    RWDate                previous(const char* dayName,
                                   const RWLocale& loc = RWLocale::global()) const;

    /**
     * Returns the date of the previous numbered day of the week, where
     * \e Monday = 1, ..., \e Sunday = 7.  The variable \a dayNum
     * must be between 1 and 7, inclusive.
     */
    RWDate                previous(unsigned dayNum) const;

    /**
     * Returns the number of the day of the week for this date, where
     * \e Monday = 1, ..., \e Sunday = 7.
     */
    unsigned              weekDay() const;

    /**
     * Returns the name of the day of the week for this date, according
     * to the optional RWLocale argument. The function defaults to using
     * the RWLocale global locale.
     */
    RWCString             weekDayName(const RWLocale& loc = RWLocale::global()) const;

    /**
     * Returns the year of this date.
     */
    unsigned              year() const;

    // Arithmetic operators:
    /**
     * Prefix increment operator.  Adds one day to self, returning
     * the result.
     */
    RWDate&               operator++() {
        ++julnum;
        return *this;
    }

    /**
     * Prefix decrement operator.  Subtracts one day from self, returning
     * the result.
     */
    RWDate&               operator--() {
        --julnum;
        return *this;
    }

    /**
     * Postfix increment operator.  Adds one day to self, returning
     * the initial value.
     */
    RWDate                operator++(int) {
        return RWDate(julnum++);
    }

    /**
     * Postfix decrement operator.  Subtracts one day from self, returning
     * the initial value.
     */
    RWDate                operator--(int) {
        return RWDate(julnum--);
    }

    /**
     * Adds \a s days to self, returning self.
     */
    RWDate&               operator+=(unsigned long s) {
        julnum += s;
        return *this;
    }

    /**
     * Subtracts \a s days from self, returning self.
     */
    RWDate&               operator-=(unsigned long s) {
        julnum -= s;
        return *this;
    }

    // Read or write dates:
    /**
     * Outputs the date \a d on \c ostream \a s, according to the locale
     * imbued in the stream (see class RWLocale), or by RWLocale::global()
     * if none.
     */
    friend RW_TOOLS_SYMBOLIC std::ostream& operator<<(std::ostream& s, const RWDate& d);

    /**
     * Reads \a t from \b std::istream \a s.  One full line is read, and
     * the string contained is converted according to the locale imbued
     * in the stream (see class RWLocale), or by RWLocale::global()
     * if none.  The function RWDate::isValid() must be used to test
     * whether the results are a valid date.
     */
    friend RW_TOOLS_SYMBOLIC std::istream& operator>>(std::istream& s, RWDate& t);

    /**
     * Returns the number of bytes necessary to store the object using
     * the global function
     *
     * @code
     * RWFile& operator<<(RWFile&, const RWDate&);
     * @endcode
     */
    RWspace               binaryStoreSize() const {
        return sizeof(julnum);
    }

    void                  restoreFrom(RWvistream& s);
    void                  restoreFrom(RWFile&);
    void                  saveOn(RWvostream& s) const;
    void                  saveOn(RWFile&) const;

    // Global friends:
    /**
     * Returns the date \a dd days in the future from the date \a dt.
     */
    friend RWDate           operator+(const RWDate& dt, unsigned long dd) {
        return RWDate(dt.julnum + dd);
    }

    /**
     * @copydoc operator+(const RWDate&, unsigned long)
     */
    friend RWDate           operator+(unsigned long dd, const RWDate& dt) {
        return RWDate(dt.julnum + dd);
    }

    /**
     * Returns the date \a dd days in the past from \a dt.
     */
    friend RWDate           operator-(const RWDate& dt, unsigned long dd) {
        return RWDate(dt.julnum - dd);
    }

    /**
     * If \a d1 > \a d2, returns the number of days between \a d1 and
     * \a d2. Otherwise, the result is implementation-defined.
     */
    friend unsigned long          operator-(const RWDate& d1, const RWDate& d2) {
        return d1.julnum - d2.julnum;
    }

    /**
     * Returns \c true if the date \a d1 is before \a d2, otherwise \c false.
     */
    friend bool        operator<(const RWDate& d1, const RWDate& d2) {
        return d1.julnum < d2.julnum;
    }

    /**
     * Returns \c true if the date  \a d1 is the same as \a d2, otherwise \c false.
     */
    friend bool        operator==(const RWDate& d1, const RWDate& d2) {
        return d1.julnum == d2.julnum;
    }

    /**
     * Returns \c true if the date \a d1 is before or the same as \a
     * d2, otherwise \c false.
     */
    friend bool        operator<=(const RWDate& d1, const RWDate& d2) {
        return d1.julnum <= d2.julnum;
    }

    /**
     * Returns \c true if the date \a d1 is after \a d2, otherwise \c false.
     */
    friend bool        operator>(const RWDate& d1, const RWDate& d2) {
        return d1.julnum > d2.julnum;
    }

    /**
     * Returns \c true if the date  \a d1 is after or the same as \a
     * d2, otherwise \c false.
     */
    friend bool        operator>=(const RWDate& d1, const RWDate& d2) {
        return d1.julnum >= d2.julnum;
    }

    /**
     * Returns \c true if the date \a d1 is not the same as \a d2,
     * otherwise \c false.
     */
    friend bool        operator!=(const RWDate& d1, const RWDate& d2) {
        return d1.julnum != d2.julnum;
    }


    // Static member functions:
    static  void        clobber(struct tm*);

    /**
     * Returns the number of the day of the week corresponding to the
     * given \a dayName ( \e Monday = 1, ..., \e Sunday = 7).  Names
     * are interpreted by the optional RWLocale argument.  Returns 0
     * if no match is found.
     */
    static  unsigned    dayOfWeek(const char* dayName,
                                  const RWLocale& loc = RWLocale::global());

    /**
     * Returns the number of days in a given year.
     */
    static  unsigned    daysInYear(unsigned year);

    /**
     * Returns the number of days in a given month and year. Returns
     * 0 if \a month is not between 1 and 12, inclusive.
     */
    static  unsigned    daysInMonthYear(unsigned month, unsigned year);

    /**
     * Returns \c true if a day (1-31) is within a given month in a
     * given year, otherwise \c false.
     */
    static  bool   dayWithinMonth(unsigned month, unsigned day, unsigned year);

    /**
     * Returns the hash value of \a d as returned by \c d.hash().
     */
    static  unsigned      hash(const RWDate& d);

    /**
     * Returns the number of the month (1-12) corresponding to the given
     * \a monthName.  Returns 0 for no match.
     */
    static  unsigned      indexOfMonth(const char* monthName,
                                       const RWLocale& loc = RWLocale::global());

    /**
     * Returns the Julian day corresponding to the given month (1-12),
     * day (1-31) and year.  Returns zero (0) if the date is invalid.
     */
    static  unsigned long jday(unsigned month, unsigned day, unsigned year);

    /**
     * Returns \c true if a given year is a leap year, otherwise \c false.
     */
    static  bool     leapYear(unsigned year);

    /**
     * Returns the name of month \a monNum ( \e January = 1, ...,
     * \e December = 12), formatted for the given locale.
     */
    static  RWCString     nameOfMonth(unsigned monNum,
                                      const RWLocale& loc = RWLocale::global());

    /**
     * Returns today's date.
     */
    static  RWDate        now();

    /**
     * Returns the name of the day of the week \a day ( \e Monday
     * = 1, ..., \e Sunday = 7), formatted for the given locale.
     * The function defaults to the RWLocale global locale.
     */
    static  RWCString     weekDayName(unsigned day,
                                      const RWLocale& loc = RWLocale::global());


    // static const data
    static const unsigned char daysInMonth[12];          // [1..31]
    static const unsigned      firstDayOfEachMonth[12];  // [0..334]

    // For functions that returned char*, use RWLocale facilities for new code:
    // for member nameOfDay()
    //    use member weekDayName(const RWLocale&)
    // for member nameOfMonth()
    //    use member monthName(const RLocale&)
    // for static member dayName(unsigned)
    //    use static member weekDayName(unsigned, const RWLocale&)
    // for static member monthName(unsigned)
    //    use static member nameOfMonth(unsigned, const RWLocale&)

protected:

    static bool      assertWeekDayNumber(unsigned d) {
        return d >= 1 && d <= 7;
    }
    static bool      assertIndexOfMonth(unsigned m) {
        return m >= 1 && m <= 12;
    }
private:
#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    friend class RW_TOOLS_SYMBOLIC RWTime;

    RW_RESTORE_DEPRECATED_WARNINGS
#endif

    void                  parseFrom(std::istream&, const RWLocale&);
    void                  mdy(unsigned& m, unsigned& d, unsigned& y) const;
    static  unsigned long currentDate();
    unsigned long         julnum; // Julian Day Number (not same as Julian date).

};


// inline members
inline RWCString RWDate::monthName(const RWLocale& locale) const
{
    return nameOfMonth(month(), locale);
}
inline RWCString RWDate::weekDayName(const RWLocale& locale) const
{
    return weekDayName(weekDay(), locale);
}

// inline statics

// Return the number, [1..7], of the day of the week named, or 0 for failure
inline unsigned RWDate::dayOfWeek(const char* dayName, const RWLocale& locale)
{
    return (unsigned) locale.weekdayIndex(RWCString(dayName));
}

// Return the number, [1..12], of the month named, or 0 for failure
inline unsigned RWDate::indexOfMonth(const char* mon, const RWLocale& locale)
{
    return (unsigned) locale.monthIndex(RWCString(mon));
}


// inline globals

/**
 * @relates RWDate
 *
 * Restores the date into \a date from the virtual stream \a str,
 * replacing the previous contents of \a date.
 */
inline RWvistream& operator>>(RWvistream& str, RWDate& date)
{
    date.restoreFrom(str);
    return str;
}

/**
 * @relates RWDate
 *
 * Restores the date into \a date from the RWFile \a file,
 * replacing the previous contents of \a date.
 */
inline RWFile& operator>>(RWFile& file, RWDate& date)
{
    date.restoreFrom(file);
    return file;
}


inline RWvistream& operator>>(RWvistream& str, RWDate*& date)
{
    date = new RWDate;
    date->restoreFrom(str);
    return str;
}

inline RWFile& operator>>(RWFile& file, RWDate*& date)
{
    date = new RWDate;
    date->restoreFrom(file);
    return file;
}

/**
 * @relates RWDate
 *
 * Saves the date into \a date from the virtual stream \a str.
 */
inline RWvostream& operator<<(RWvostream& str, const RWDate& date)
{
    date.saveOn(str);
    return str;
}

/**
 * @relates RWDate
 *
 * Saves the date into \a date from the RWFile \a file.
 */
inline RWFile& operator<<(RWFile& file, const RWDate& date)
{
    date.saveOn(file);
    return file;
}

#endif   /* RW_TOOLS_DATE_H */
