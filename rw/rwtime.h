#ifndef RW_TOOLS_TIME_H
#define RW_TOOLS_TIME_H

/**********************************************************************
 *
 * Declarations for class RWTime.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rwtime.h#1 $
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

#include <time.h>

#include <rw/defs.h>
#include <rw/locale.h>

class RW_TOOLS_GLOBAL RWDate;

/**************************************************************************/

extern RW_TOOLS_GLOBAL const unsigned long rwEpoch;
extern RW_TOOLS_GLOBAL const unsigned long rwEpoch_68;

/**************************************************************************/

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS

/**
 * @ingroup date_and_time_classes
 * @brief Deprecated. Represents a time, stored as the number of seconds
 * since 00:00:00 January 1, 1901 GMT.
 *
 * @deprecated As of SourcePro 10, use RWDateTime instead.
 *
 * Class RWTime represents a time, stored as the number of seconds
 * since 00:00:00 January 1, 1901 GMT.  The <i>Essential Tools User's Guide</i>
 * chapter on date and time classes describes how to set the time
 * zone for your compiler.  Failure to do so may result in GMT
 * times being wrong.
 *
 * @note
 * RWTime is unable to store times prior to 00:00:00 1 Jan 1901.
 *
 * Output formatting is done using an RWLocale object.  The default
 * locale formats according to U.S. conventions.
 *
 * @note
 * Because the default constructor for this class creates
 * an instance holding the current date and time, constructing a
 * large array of RWTime may be slow.
 *
 * @code
 *
 * RWTime v[5000];     // Figures out the current time 5000 times
 * @endcode
 *
 * Those with access to the C++ Standard Library-based versions
 * of the Essential Tools Module template collections should
 * consider the following:
 *
 * @code
 *
 * // Figures out the current time just once:
 * RWTValOrderedVector<RWTime> v(5000, RWTime());
 * @endcode
 *
 * Thanks to the smart allocation scheme of the standard collections,
 * the above declaration will result in only one call to the default
 * constructor followed by 5000 invocations of the copy constructor.
 * In the case of RWTime, the copy constructor amounts to an assignment
 * of one \c long to another, resulting in faster creation than
 * the simple array.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rwtime.h>
 * RWTime a;   // Construct with current time
 * @endcode
 *
 * @section persistence Persistence
 *
 * Simple
 *
 * @section example Example
 *
 * @code
 *
 * #include <rw/rwtime.h>
 * #include <rw/rwdate.h>
 *
 * int main()
 * {
 *     // Current time
 *     RWTime t;
 *     RWTime d (RWTime::beginDST (1990, RWZone::local()));
 *
 *     std::cout << "Current time: "       << t << std::endl;
 *     std::cout << "Start of DST, 1990: " << d << std::endl;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * Current time: 06/18/09 13:43:39
 * Start of DST, 1990: 04/01/90 03:00:00
 * @endcode
 */
class RW_DEPRECATE_TYPE("Use RWDateTime instead") RW_TOOLS_SYMBOLIC RWTime
{
public:

    /**
     * Default constructor.  Constructs a time with the present time.
     */
    RWTime()
        : value_(RWTime::currentTime()) {
    }

    /**
     * Constructs a time with \a s seconds since 00:00:00 January 1,
     * 1901 GMT.  If \a s <tt>== 0</tt>, an invalid time is constructed.
     * @note
     * For small \a s, this may be prior to January 1, 1901 in your
     * time zone.
     *
     * The compiler can parse 0 as either an integer or a pointer. Since
     * there is also a constructor that takes a pointer (to <tt>struct tm</tt>,
     * if you want to construct a time from the unsigned \c long value 0,
     * you must be explicit:
     *
     * @code
     * RWTime earlyTime((unsigned long)0);
     * @endcode
     */
    RWTime(unsigned long s)
        : value_(s) {
    }

    /**
     * Constructs a time with today's date and the specified \a hour,
     * \a minute, and \a second, relative to the time zone \a zone, which
     * defaults to local time.
     */
    RWTime(unsigned hour,
           unsigned minute,
           unsigned second = 0,
           const RWZone& zone = RWZone::local());

    /**
     * Constructs a time for a given \a date, \a hour, \a minute, and \a
     * second, relative to the time zone \a zone, which defaults to local
     * time.
     * @note
     * The maximum RWTime is much sooner than maximum RWDate.
     * (In fact, it is on Feb. 5, 2037 for platforms with 4-byte
     * \c longs.)  This is a consequence of the fact that RWTime counts
     * seconds, while RWDate deals only with full days.
     */
    RWTime(const RWDate& date,
           unsigned hour = 0,
           unsigned minute = 0,
           unsigned second = 0,
           const RWZone& zone = RWZone::local());

    /**
     * Constructs a time for the given \a date, extracting the time from
     * the string \a str.  The string \a str should contain only the
     * time.  The time is understood to be relative to the time zone
     * \a zone, which defaults to local time.  The specified \a locale
     * is used for formatting information.  Use function isValid() to
     * check the results.
     * @note
     * Not all time string errors can be detected
     * by this function.
     */
    RWTime(const RWDate& date,
           const RWCString& str,
           const RWZone& zone = RWZone::local(),
           const RWLocale& locale = RWLocale::global());

    /**
     * Constructs a time from the \c tm_year, \c tm_mon, \c tm_mday,
     * \c tm_hour, \c tm_min, and \c tm_sec components of the <tt>struct tm</tt>
     * argument.  These components are understood to be relative
     * to the time zone \a zone, which defaults to local time.
     * @note
     * The numbering of months and years in a <tt>struct tm</tt> differs
     * from that used in RWTime arguments.
     */
    RWTime(const struct tm* ptm, const RWZone& zone = RWZone::local());

    /**
     * Fills all members of the <tt>struct tm</tt> argument, adjusted to
     * the time zone specified by the RWZone argument.  If the time
     * is invalid, the <tt>struct tm</tt> members are all set to -1.
     * @note
     * The encoding of <tt>struct tm</tt> members is different from
     * that used in RWTime and RWDate functions.
     */
    void
    extract(struct tm* ptm, const RWZone& zone = RWZone::local()) const;

    void
    extractGMT(struct tm* ptm) const;

    void
    extractGMT(struct tm* ptm, bool) const;

    /**
     * Returns self as a string, formatted by the RWLocale argument,
     * with the time zone adjusted according to the RWZone argument.
     * Formats are as defined by the C99 function \c strftime().  The
     * default format is the date followed by the time: <tt>"%x %X"</tt>.
     * The exact format of the date and time returned is dependent
     * upon the Locale-dependent \c \%x  and \c \%X  formatting. For more
     * information, see RWLocale.
     */
    RWCString
    asString(char format = '\0',
             const RWZone& zone = RWZone::local(),
             const RWLocale& locale = RWLocale::global()) const;

    /**
     * Returns self as a string, formatted by the RWLocale argument,
     * with the time zone adjusted according to the RWZone argument.
     * Formats are as defined by the C99 function \c strftime(). For more
     * information, see RWLocale.
     */
    RWCString
    asString(const char* format,
             const RWZone& zone = RWZone::local(),
             const RWLocale& locale = RWLocale::global()) const;

    /**
     * Returns \c true if RWTime is between \a a and \a b, inclusive.
     */
    bool
    between(const RWTime& a, const RWTime& b) const;

    /**
     * Returns the number of bytes necessary to store the object using
     * the global function
     *
     * @code
     * RWFile& operator<<(RWFile&, const RWTime&);
     * @endcode
     */
    RWspace
    binaryStoreSize() const {
        return sizeof(value_);
    }

    /**
     * Comparison function, useful for sorting times.  Compares self
     * to the RWTime pointed to by \a t.
     *
     * @retval 0  if self == *\a t;
     *
     * @retval 1  if self > *\a t;
     *
     * @retval -1  if self < *\a t;
     */
    int
    compareTo(const RWTime* t) const;

    int
    compareTo(const RWTime& t) const {
        return compareTo(&t);
    }

    /**
     * Returns a suitable hashing value.
     */
    unsigned
    hash() const;

    /**
     * Returns the hour, adjusted to the time zone specified.
     */
    unsigned
    hour(const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the hour in GMT.
     */
    unsigned
    hourGMT() const;

    /**
     * Returns \c true if self is during Daylight Saving Time in the
     * time zone given by \a zone, \c false otherwise.
     */
    bool
    isDST(const RWZone& zone = RWZone::local()) const;

    /**
     * Returns \c true if this is a valid time, \c false otherwise.
     */
    bool
    isValid() const {
        return value_ != 0;
    }

    /**
     * Returns the later time of self or \a t .
     */
    RWTime
    RW_MACRO_GUARD(max)(const RWTime& t) const {
        RW_PRECONDITION(RWTime::isValid());
        RW_PRECONDITION(t.isValid());

        return (*this > t) ? *this : t;
    }

    /**
     * Returns the earlier time of self or \a t .
     */
    RWTime
    RW_MACRO_GUARD(min)(const RWTime& t) const {
        RW_PRECONDITION(RWTime::isValid());
        RW_PRECONDITION(t.isValid());

        return (*this < t) ? *this : t;
    }

    /**
     * Returns the minute, adjusted to the time zone specified.
     */
    unsigned
    minute(const RWZone& zone = RWZone::local()) const;

    /**
     * Returns the minute in GMT.
     */
    unsigned
    minuteGMT() const;

    void
    restoreFrom(RWFile&);

    void
    restoreFrom(RWvistream&);

    void
    saveOn(RWFile&) const;

    void
    saveOn(RWvostream&) const;

    /**
     * Returns the second in local time or GMT.
     */
    unsigned
    second() const;

    /**
     * Returns the number of seconds since 00:00:00 January 1, 1901
     * GMT.
     */
    unsigned long
    seconds() const {
        return value_;
    }

    /**
     * Outputs the time \a t on \c ostream \a s, according to the locale
     * imbued in the stream (see class RWLocale), or by RWLocale::global()
     * if none.
     */
    friend RW_TOOLS_SYMBOLIC
    std::ostream&
    operator<< (std::ostream& s, const RWTime& t);

    /**
     * Prefix increment operator.  Adds one second to self, then returns
     * the results.
     */
    RWTime
    operator++ () {
        // bounds checking is not necessary. if value_ is max, then the
        // increment would make it 0, which is the invalid sentinel.
        if (isValid()) {
            ++value_;
        }
        return *this;
    }

    /**
     * Prefix decrement operator.  Subtracts one second from self, then
     * returns the results.
     */
    RWTime
    operator-- () {
        // bounds checking is not necessary. if value_ is 1, then the
        // decrement would make it 0, which is the invalid sentinel.
        if (isValid()) {
            --value_;
        }
        return *this;
    }

    /**
     * Postfix increment operator.  Adds one second to self, returning
     * the initial value.
     */
    RWTime
    operator++ (int) {
        // bounds checking is not necessary.
        if (isValid()) {
            return RWTime(value_++);
        }
        return *this;
    }

    /**
     * Postfix decrement operator.  Subtracts one second from self, returning
     * the initial value.
     */
    RWTime
    operator-- (int) {
        // bounds checking is not necessary.
        if (isValid()) {
            return RWTime(value_--);
        }
        return *this;
    }

    /**
     * Adds \a s seconds to self, returning self.
     */
    RWTime&
    operator+= (unsigned long s) {
        // this is not properly bounds checked and can overflow.
        if (isValid()) {
            value_ += s;
        }
        return *this;
    }

    /**
     * Subtracts \a s seconds from self, returning self.
     */
    RWTime&
    operator-= (unsigned long s) {
        // this is not properly bounds checked and can underflow.
        if (isValid()) {
            value_ -= s;
        }
        return *this;
    }

    /**
     * Returns the start of Daylight Saving Time (DST) for the given
     * year, in the given time zone.  Returns an "invalid time" if DST
     * is not observed in that year and zone.
     */
    static RWTime
    beginDST(unsigned year, const RWZone& zone = RWZone::local());

    /**
     * Returns the end of Daylight Saving Time for the given year, in
     * the given time zone.  Returns an "invalid time" if DST is not
     * observed in that year and zone.
     */
    static RWTime
    endDST(unsigned year, const RWZone& zone = RWZone::local());

    /**
     * Returns the present time.
     */
    static RWTime
    now();

    /**
     * Returns the hash value of \a t as returned by t.hash().
     */
    static unsigned
    hash(const RWTime& t);

    /**
     * Returns \c true if \a t1 is less than \a t2.
     */
    friend bool
    operator< (const RWTime& t1, const RWTime& t2);

    /**
     * Returns \c true if \a t1 is equal to \a t2.
     */
    friend bool
    operator== (const RWTime& t1, const RWTime& t2);

    /**
     * Returns \c true if \a t1 is less than or equal to \a t2.
     */
    friend bool
    operator<= (const RWTime& t1, const RWTime& t2);

    /**
     * Returns \c true if \a t1 is greater than \a t2.
     */
    friend bool
    operator> (const RWTime& t1, const RWTime& t2);

    /**
     * Returns \c true if \a t1 is greater than or equal to \a t2.
     */
    friend bool
    operator>= (const RWTime& t1, const RWTime& t2);

    /**
     * Returns \c true if \a t1 is not equal to \a t2.
     */
    friend bool
    operator!= (const RWTime& t1, const RWTime& t2);

    /**
     * Returns an RWTime \a s seconds greater than \a t.
     */
    friend RWTime
    operator+ (const RWTime& t, unsigned long s);

    /**
     * @copydoc operator+(const RWTime&, unsigned long)
     */
    friend RWTime
    operator+ (unsigned long s, const RWTime& t);

    /**
     * Returns an RWTime \a s seconds less than \a t.
     */
    friend RWTime
    operator- (const RWTime& t, unsigned long s);

protected:

    static bool
    assertDate(const RWDate& date);

private:

    static unsigned long
    buildFrom(const RWDate& date,
              unsigned h, unsigned m, unsigned s,
              const RWZone& zone = RWZone::local());

    static bool
    buildMore(unsigned long& secs,
              int zoneOffset,
              const RWDate& date);

    static unsigned long
    currentTime();

private:

    unsigned long value_;

};

inline bool
operator== (const RWTime& lhs, const RWTime& rhs)
{
    return lhs.value_ == rhs.value_;
}

inline bool
operator< (const RWTime& lhs, const RWTime& rhs)
{
    return lhs.value_ < rhs.value_;
}

inline bool
operator<= (const RWTime& lhs, const RWTime& rhs)
{
    return lhs.value_ <= rhs.value_;
}

inline bool
operator> (const RWTime& lhs, const RWTime& rhs)
{
    return lhs.value_ > rhs.value_;
}

inline bool
operator>= (const RWTime& lhs, const RWTime& rhs)
{
    return lhs.value_ >= rhs.value_;
}

inline bool
operator!= (const RWTime& lhs, const RWTime& rhs)
{
    return lhs.value_ != rhs.value_;
}

inline bool
RWTime::between(const RWTime& a, const RWTime& b) const
{
    return *this >= a && *this <= b;
}

inline RWTime
operator+ (const RWTime& t, unsigned long s)
{
    // this is not properly bounds checked and can overflow.
    return RWTime(t.isValid() ? t.value_ + s : 0);
}

inline RWTime
operator- (const RWTime& t, unsigned long s)
{
    // this is not properly bounds checked and can underflow.
    return RWTime(t.isValid() ? t.value_ - s : 0);
}

inline RWTime
operator+ (unsigned long s, const RWTime& t)
{
    // this is not properly bounds checked and can overflow.
    return RWTime(t.isValid() ? t.value_ + s : 0);
}

/**
 * @relates RWTime
 *
 * Restores an RWTime into \a t from the virtual stream \a str, replacing
 * the previous contents of \a t.
 */
inline RWvistream&
operator>> (RWvistream& str, RWTime& t)
{
    t.restoreFrom(str);
    return str;
}

/**
 * @relates RWTime
 *
 * Restores an RWTime into \a t from the RWFile \a file, replacing
 * the previous contents of \a t.
 */
inline RWFile&
operator>> (RWFile& file, RWTime& t)
{
    t.restoreFrom(file);
    return file;
}

inline RWvistream&
operator>> (RWvistream& str, RWTime*& t)
{
    t = new RWTime;
    t->restoreFrom(str);
    return str;
}

inline RWFile&
operator>> (RWFile& file, RWTime*& t)
{
    t = new RWTime;
    t->restoreFrom(file);
    return file;
}

/**
 * @relates RWTime
 *
 * Saves an RWTime \a t onto the virtual stream \a str.
 */
inline RWvostream&
operator<< (RWvostream& str, const RWTime& t)
{
    t.saveOn(str);
    return str;
}

/**
 * @relates RWTime
 *
 * Saves an RWTime \a t onto the RWFile \a file.
 */
inline RWFile&
operator<< (RWFile& file, const RWTime& t)
{
    t.saveOn(file);
    return file;
}

inline
RWTime::RWTime(const RWDate& date,
               unsigned h, unsigned m, unsigned s,
               const RWZone& zone)
{
    value_ = buildFrom(date, h, m, s, zone);
}

RW_RESTORE_DEPRECATED_WARNINGS
#endif

#endif // RW_TOOLS_TIME_H
