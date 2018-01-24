#ifndef RW_TOOLS_TOOLS_ZONE_H
#define RW_TOOLS_TOOLS_ZONE_H

/**********************************************************************
 *
 * Declarations for RWZone
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/zone.h#2 $
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
#include <rw/cstring.h>

#include <time.h>

/**
 * @ingroup internationalization_classes
 * @brief Struct used by RWDaylightRule to define the beginning
 * and ending of daylight saving time.
 *
 * RWDaylightBoundary is a struct with no constructors that is used
 * by RWDaylightRule to define the beginning and ending of daylight
 * saving time.
 */
struct RW_TOOLS_SYMBOLIC RWDaylightBoundary {
    /**
     * The month from <tt>(0 - 11)</tt>, where \c 0 = January.
     */
    int month_;

    /**
     * A week of the month from <tt>(0 - 4)</tt>, or \c -1 if the following
     * field is to represent a day within the month.
     */
    int week_;

    /**
     * A day of the week from <tt>(0 - 6)</tt>, where \c 0 = Sunday,
     * or, if the \c week_ field is \c -1, a day of the month from
     * <tt>(1 - 31)</tt>.
     */
    int weekday_;

    /**
     * Minutes after 12:00 AM, from <tt>(0 - 1439)</tt>.  For example,
     *  \c 120 = 2 AM.
     */
    int minute_;

    void
    date(struct tm*) const;

    // Returns true if
    bool
    after(const struct tm*) const;
};

/**
 * @ingroup internationalization_classes
 * @brief Struct that defines daylight saving time rules.
 *
 * The RWDaylightRule struct defines daylight saving time rules and
 * is passed to RWZoneSimple's constructor. It can define a
 * single rule for all years or the head of a chain of rules going
 * backwards in time.
 *
 * RWDaylightRule has no constructors. The data members of this
 * structure are as follows:
 */
struct RW_TOOLS_SYMBOLIC RWDaylightRule {
    /**
     * Points to the next rule in a chain that continues backwards
     * in time.
     */
    const RWDaylightRule* next_;

    /**
     * Four-digit representation of the year in which this rule first
     * goes into effect.
     */
    short firstYear_;

    /**
     * A boolean value that can be used to specify a period of years
     * for which Daylight Saving Time is not observed.
     *
     * \c 1 = Daylight Saving Time is in effect during this period
     *
     * \c 0 = Daylight Saving Time is not in effect during this period
     *
     * @note
     * These are numeric values, as distinguished from "1" and "0".
     */
    char  observed_;

    /**
     * Indicates the time of year, to the minute, when
     * DST begins during this period.
     */
    RWDaylightBoundary begin_;

    /**
     * Indicates the time of year, to the minute, when
     * standard time resumes during this period.
     */
    RWDaylightBoundary end_;

    RWDaylightRule&
    operator= (const RWDaylightRule&);

    bool
    isDaylight(const struct tm*) const;
};

// Uncomment the following define for a link-incompatible fix
// which replaces the return type for timeZoneOffset () and
// altZoneOffset () with long int.

// #define RW_USE_LONG_ZONE_OFFSET

#if defined(RW_USE_LONG_ZONE_OFFSET)
#  define RW_ZONE_OFFSET_T   long int
#else
#  define RW_ZONE_OFFSET_T   int
#endif // RW_USE_LONG_ZONE_OFFSET

/**
 * @ingroup internationalization_classes
 * @brief Abstract base class defining an interface to describe
 * time zones.
 *
 * RWZone is an abstract base class that defines an interface for
 * time zone issues, such as whether or not Daylight Saving Time (DST)
 * is in use, the names and offsets from Greenwich Mean Time (GMT)
 * (also known as Coordinated Universal Time, or UTC), for both
 * standard and Daylight Saving Time, and the start and
 * stop dates for DST, if used. Note that because
 * it is an abstract base class, there is no way to actually enforce
 * these goals -- the description here is merely the model of how
 * a class derived from RWZone should act.
 *
 * Most programs interact with RWZone only by passing an RWZone
 * reference to SourcePro member functions that expect one.
 *
 * DST rules are volatile, often reflecting geographical
 * and political changes. In some cases, the hard-coded, table-driven
 * struct RWDaylightRule does not accurately reflect the locale
 * installed on your machine. RWZone::os() creates a new RWZone
 * containing the daylight rule discovered from the underlying operating
 * system. This way, the operating system itself defines the DST rules.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <time.h>
 * #include <rw/zone.h>
 * (abstract base class)
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/zone.h>
 *
 * #include <rw/zone.h>
 * #include <rw/tools/datetime.h>
 *
 * int main ()
 * {
 *     RWDateTime now(RWDateTime::setCurrentTime);
 *
 *     std::cout << now.asString("%x %X", RWLocale::global(), RWZone::local()) << std::endl;
 *     std::cout << now.asString("%x %X", RWLocale::global(), RWZone::utc()) << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 */
class RW_TOOLS_GLOBAL RWZone
{

public:

    virtual ~RWZone();

    /**
     * Returns the number of seconds west of GMT for standard time in
     * this zone.  The number is negative for zones east of Greenwich,
     * England.
     */
    virtual RW_ZONE_OFFSET_T timeZoneOffset() const = 0;

    /**
     * Returns the number of seconds west of GMT for DST in this zone.
     */
    virtual RW_ZONE_OFFSET_T altZoneOffset() const = 0;

    /**
     * Returns \c true if DST is observed for this zone.
     */
    virtual bool daylightObserved() const = 0;

    /**
     * Returns the name of the standard time zone represented, such as \c "PST".
     * Note that the current date and time have no effect on the return value.
     */
    virtual RWCString timeZoneName() const = 0;

    /**
     * Returns the name of the DST zone represented, such as \c "PDT".
     * Note that the current date and time have no effect on the return value.
     */
    virtual RWCString altZoneName() const = 0;

    /**
     * Populates \a tmbuf for this zone based on \a msec milliseconds
     * since January 1, 1901 00:00:00:000 UTC. If not \a NULL, \a tzOffset
     * and \a tzName are populated with the time zone offset and time
     * zone name at the time specified.
     */
    virtual void toTM(rwint64 msec, struct tm* tmbuf, int* tzOffset,
                      RWCString* tzName) const;

    /**
     * Returns the number of milliseconds that have elapsed since
     * January 1, 1901 00:00:00:000 UTC in this zone given the date and
     * time in \a tmbuf.
     */
    virtual rwint64 fromTM(const struct tm* tmbuf) const;

    /**
     * Returns the number of seconds west of GMT in this zone for the time
     * given in \a tmbuf.
     *
     * The default implementation of this function returns timeZoneOffset()
     * if isDaylight() returns \c false for \a tmbuf, otherwise returns
     * altZoneOffset().
     */
    virtual int offset(const struct tm* tmbuf) const;

    /**
     * Returns the current name of the zone for the time given in \a tmbuf.
     *
     * The default implementation of this function returns timeZoneName() if
     * isDaylight() returns \c false for \a tmbuf, otherwise returns
     * altZoneName().
     */
    virtual RWCString name(const struct tm* tmbuf) const;

    /**
     * Returns \c true if the time and date represented in the <tt>struct tm</tt>
     * argument is in the range of DST for this zone. The elements of \a tmbuf
     * must all be self-consistent; in particular, the \c tm_wday member must
     * agree with the \c tm_year, \c tm_mon, and \c tm_day members.
     */
    virtual bool isDaylight(const struct tm* tmbuf) const = 0;

    /**
     * Returns with the <tt>struct tm</tt> argument set to the local time
     * that DST begins for the year specified by the \c tm_year field of
     * \a tmbuf. If DST is not observed, the members of \a tmbuf are all
     * set to a negative value. Note that in the southern hemisphere, DST
     * ends at an earlier date than it begins.
     */
    virtual void getBeginDaylight(struct tm* tmbuf) const = 0;

    /**
     * Returns with the <tt>struct tm</tt> argument set to the local time
     * that DST ends for the year specified by the \c tm_year field of
     * \a tmbuf. If DST is not observed, the members of \a tmbuf are all
     * set to a negative value. Note that in the southern hemisphere, DST
     * ends at an earlier date than it begins.
     */
    virtual void getEndDaylight(struct tm* tmbuf) const = 0;

    /**
     * Returns a reference to an RWZone representing local time.  By
     * default, this is an instance of RWZoneSimple created with
     * offsets and zone names from the operating system, with U.S. rules
     * for DST, if observed. This is used as a default argument value
     * to member functions of other classes where an RWZone argument
     * is required.
     */
    static const RWZone& local();

    /**
     * Returns a reference to an RWZone representing standard local
     * time, with no DST corrections.  By default, this is an instance
     * of RWZoneSimple with offset and zone name from the operating
     * system.
     */
    static const RWZone& standard();

    /**
     * Returns a reference to an RWZone representing GMT (UTC) universal
     * time.
     */
    static const RWZone& utc();

    /**
     * On the first call, determines DST rules for the years 1970 - 2038,
     * from the underlying operating system. On subsequent calls, a cached
     * copy is used. You can set the DST rules for these years by
     * instantiating RWZone::local(&RWZone::os()).
     */
    static const RWZone& os();

    /**
     * @internal
     * Create an RWZone instance based on time zone information from the
     * underlying operating system. DST rules for the years 1970 - 2038
     * are captured in the resulting RWZone instance.
     */
    static const RWZone* createOSZone();

    /**
     * Sets the RWZone to be returned by RWZone::local() to \a zone and
     * returns the previous value.
     */
    static const RWZone* local(const RWZone* zone);

    /**
     * Sets the RWZone to be returned by RWZone::standard() to \a zone and
     * returns the previous value.
     */
    static const RWZone* standard(const RWZone* zone);

    // system () is used internally  for platforms that do not provide an
    // ANSI time ()  function that correctly returns the  current time in
    // UTC. (At present,  none of the certified platforms  fall into this
    // category). By  default it  is set just  as is the  initial default
    // value of local (). The system () zone, if needed, should correctly
    // reflect  the time  zone represented  by  the system  clock in  the
    // machine.  If  you need to set system  (), you should do  so at the
    // beginning of your program.  It should not vary during execution.

    static const RWZone& system();
    static const RWZone* system(const RWZone*);   // set system ()

    static void  clearUtc();

    /**
     * Names the standard time zones.  Its values are intended
     * to be passed to constructors of classes derived from RWZone.
     */
    enum StdZone {

        /**
         * = -12
         */
        NewZealand = -12,

        /**
         * &nbsp;
         */
        CarolineIslands,

        /**
         * &nbsp;
         */
        MarianaIslands,

        /**
         * &nbsp;
         */
        Japan,

        /**
         * &nbsp;
         */
        China,

        /**
         * &nbsp;
         */
        Java,

        /**
         * &nbsp;
         */
        Kazakh,

        /**
         * &nbsp;
         */
        Pakistan,

        /**
         * &nbsp;
         */
        CaspianSea,

        /**
         * &nbsp;
         */
        Ukraine,

        /**
         * &nbsp;
         */
        Nile,

        /**
         * &nbsp;
         */
        Europe,

        /**
         * &nbsp;
         */
        Greenwich,

        /**
         * &nbsp;
         */
        Azores,

        /**
         * &nbsp;
         */
        Oscar,

        /**
         * &nbsp;
         */
        Greenland,

        /**
         * &nbsp;
         */
        Atlantic,

        /**
         * &nbsp;
         */
        USEastern,

        /**
         * &nbsp;
         */
        USCentral,

        /**
         * &nbsp;
         */
        USMountain,

        /**
         * &nbsp;
         */
        USPacific,

        /**
         * &nbsp;
         */
        Yukon,

        /**
         * &nbsp;
         */
        Hawaii,

        /**
         * &nbsp;
         */
        Bering
    };

    /**
     * Used by the static member function dstRule() and by constructors
     * for classes derived from RWZone, including RWZoneSimple.
     */
    enum DstRule {

        /**
         * Daylight Savings Time never observed
         */
        NoDST,

        /**
         * North America (US, Canada)
         */
        NoAm,

        /**
         * Western Europe, except UK
         */
        WeEu,

        /**
         * Official European Union DST rules.
         */
        OfficialEU
    };

    /**
     * Returns one of the built-in DST rules according
     * to \a rule.  This function is provided for convenience in
     * constructing RWZoneSimple instances for time zones in which common
     * DST rules are obeyed.  RWZone has predefined
     * names for these rules, defined under the enumerations section for this
     * class.  If <tt>DstRule NoDST</tt> is provided, then \c 0 is returned.
     * The result of calling dstRule() is normally passed to the RWZoneSimple
     * constructor.
     */
    static const RWDaylightRule* dstRule(DstRule rule = NoAm);

    // RWZoneSimple implements DST rules simple enough to represent
    // as static instances of RWDaylightRule. DST Jurisdictions too
    // complex to represent in table form require deriving from RWZone
    // to implement necessary semantics.

};

/**
 * @ingroup internationalization_classes
 * @brief  A basic, concrete timezone implementation of the abstract
 * class RWZone, based on the Daylight Saving Time (DST) rules defined
 * in \c struct RWDaylightRule.
 *
 * RWZoneSimple is an implementation of the abstract interface defined
 * by class RWZone. It implements a simple DST
 * rule sufficient to represent all historical U.S. conventions
 * and many European and Asian conventions. It is table-driven and
 * depends on parameters given by the \c struct RWDaylightRule.
 *
 * In many cases, you may want more explicit control of the DST
 * rule for the intended RWZoneSimple. If so, you can build a DST
 * rule with arbitrary begin and end times (see the RWDaylightRule
 * below), and provide it as a parameter to RWZoneSimple.
 *
 * Direct use of RWDaylightRule affords the most general interface
 * to RWZoneSimple. However, a much simpler programmatic interface
 * is offered, as illustrated by the examples below.
 *
 * Instances of RWZoneSimple may be constructed to represent other time
 * zones, and may be installed globally using the RWZone static member
 * function RWZone::local(const RWZone*).
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <time.h>
 * #include <rw/zone.h>
 * RWZoneSimple myZone(RWZone::USCentral);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 * To install US Central time as your global "local" time use:
 *
 * @code
 * RWZone::local(new RWZoneSimple(RWZone::USCentral));
 * @endcode
 *
 * To install Hawaiian time (where Daylight Saving Time is not observed), use:
 *
 * @code
 * RWZone::local(new RWZoneSimple(RWZone::Hawaii,
 *                                RWZone::NoDST));
 * @endcode
 *
 * Likewise for Japan, use:
 *
 * @code
 * RWZone::local(new RWZoneSimple(RWZone::Japan, RWZone::NoDST));
 * @endcode
 *
 * For France:
 *
 * @code
 * RWZone::local(new RWZoneSimple(RWZone::Europe, RWZone::WeEu));
 * @endcode
 *
 * RWZone has predefined values for the RWZone::DstRule rules
 *
 * Here are the rules used internally for the RWZone::NoAm, RWZone::WeEu,
 * and RWZone::OfficialEU values of RWZone::DstRule. First, here are the
 * rules for the RWZone::NoAm value:
 *
 * @code
 * // last Sun in Apr to last in Oct:
 * const RWDaylightRule usRuleAuld =
 *     { 0, 0000, 1, { 3, 4, 0, 120 }, { 9, 4, 0, 120 } };
 * // first Sun in Apr to last in Oct
 * const RWDaylightRule usRule67 =
 *     { &usRuleAuld, 1967, 1, { 3, 0, 0, 120 }, { 9, 4, 0, 120 } };
 * // first Sun in Jan to last in Oct:
 * const RWDaylightRule usRule74 =
 *     { &usRule67, 1974, 1, { 0, 0, 0, 120 }, { 9, 4, 0, 120 } };
 * // last Sun in Feb to last in Oct
 * const RWDaylightRule usRule75 =
 *     { &usRule74, 1975, 1, { 1, 4, 0, 120 }, { 9, 4, 0, 120 } };
 * // last Sun in Apr to last in Oct
 * const RWDaylightRule usRule76 =
 *     { &usRule75, 1976, 1, { 3, 4, 0, 120 }, { 9, 4, 0, 120 } };
 * // first Sun in Apr to last in Oct
 * const RWDaylightRule usRuleLate =
 *     { &usRule76, 1987, 1, { 3, 0, 0, 120 }, { 9, 4, 0, 120 } };
 * @endcode
 *
 * And here are the rules for the RWZone::WeEu value:
 *
 * @code
 * // last Sun in March (2am) to last in September
 * static RWDaylightRule euRuleAuld =
 *     { 0, 0000, 1, { 2, 4, 0, 120 }, { 8, 4, 0, 120 } };
 * // last Sun in March (1am) to last in Oct
 * static RWDaylightRule euRuleLate =
 *     { &euRuleAuld, 1998, 1, { 2, 4, 0, 60 }, { 9, 4, 0, 60 } };
 * @endcode
 *
 * And here are the rules for the RWZone::OfficialEU value:
 *
 * @code
 * // Last Sun in March (2am) to last in Sept
 * static RWDaylightRule euOfficialRuleAuld =
 *     { 0, 0000, 1, { 2, 4, 0, 120 }, { 8, 4, 0, 120 } };
 * // Last Sun in March (2am) to last in Oct
 * static RWDaylightRule euOfficialRuleLate1996 =
 *     { &euOfficialRuleAuld, 1996, 1, { 2, 4, 0, 120 }, { 9, 4, 0, 180 } };
 * @endcode
 *
 * Given these definitions,
 *
 * @code
 * RWZone::local(new RWZoneSimple(RWZone::USCentral,
 *                                RWZone::NoAm));
 * @endcode
 *
 * is equivalent to the first example given above and repeated here:
 *
 * @code
 * RWZone::local(new RWZoneSimple(RWZone::USCentral));
 * @endcode
 *
 * Daylight Saving Time systems that cannot be represented with RWDaylightRule
 * and RWZoneSimple must be modeled by deriving from RWZone and implementing
 * its virtual functions.
 *
 * For example, under Britain's Summer Time rules, alternate timekeeping begins
 * the morning after the third Saturday in April, unless that is Easter (in
 * which case it begins the week before) or unless the Council decides on some
 * other time for that year. In some years Summer Time has been two hours ahead,
 * or has extended through winter without a break. British Summer Time clearly
 * deserves an RWZone class all its own.
 */
class RW_TOOLS_GLOBAL RWZoneSimple : public RWZone
{

public:

    const RWDaylightRule* rule_;

    bool daylightObserved_;

    RW_ZONE_OFFSET_T timeZoneOffset_;
    RW_ZONE_OFFSET_T altZoneOffset_;

    RWCString timeZoneName_;
    RWCString altZoneName_;

    /**
     * Constructs an RWZoneSimple instance in which DST
     * is computed according to the provided \a rule. If
     * \a rule is zero, DST is not observed.
     *
     * @param rule    The DST rule
     * @param stdoff   The offset from GMT (in seconds, positive
     *                if west of 0 degrees longitude)
     * @param stdname  The name of Standard time
     * @param altoff  The offset (typically equal to
     *                \a tzoff <tt>- 600</tt>) to define
     *                when DST is in effect
     * @param altname Alternative name for the timezone when DST
     *                is in effect, for example, MDT for Mountain
     *                Daylight Time.
     */
    RWZoneSimple(const RWDaylightRule* rule,
                 long int stdoff, const RWCString& stdname,
                 long int altoff, const RWCString& altname)
        : rule_(rule),
          daylightObserved_(rule != 0),
          timeZoneOffset_((RW_ZONE_OFFSET_T)stdoff),
          altZoneOffset_((RW_ZONE_OFFSET_T)altoff),
          timeZoneName_(stdname),
          altZoneName_(altname) {
    }

    /**
     * Constructs an RWZoneSimple instance in which DST
     * is not observed. Argument \a tzoff is the offset from GMT
     * (in seconds, positive if west of 0 degrees longitude) and
     * \a tzname is the name of the zone.
     */
    RWZoneSimple(long int tzoff,
                 const RWCString& tzname);

    /**
     * Constructs an RWZoneSimple instance in which offsets and names
     * are specified by the \c StdZone argument. DST
     * is computed according to the \a rule argument, if non-zero;
     * otherwise, DST is not observed.
     */
    RWZoneSimple(RWZone::StdZone zone,
                 const RWDaylightRule* rule);

    /**
     * Constructs an RWZoneSimple instance using internal RWDaylightRule
     * instances. This is the simplest interface to RWZoneSimple. The argument
     * \a zone is the time zone for which an RWZoneSimple is to be constructed.
     * The second argument defines the DST rule to follow. North America's DST
     * rules are the default.
     */
    RWZoneSimple(RWZone::StdZone zone, RWZone::DstRule = RWZone::NoAm);

    /**
     * Copy constructor.
     */
    RWZoneSimple(const RWZoneSimple& zone);

    /**
     * Assignment operator.
     */
    RWZoneSimple& operator= (const RWZoneSimple&);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a zone.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWZoneSimple(RWZoneSimple && zone);

    /**
     * Move assignment. Self takes ownership of the data owned by \a zone.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWZoneSimple& operator= (RWZoneSimple && zone);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWZoneSimple& rhs);

    virtual ~RWZoneSimple();

    virtual RW_ZONE_OFFSET_T timeZoneOffset() const;
    virtual RW_ZONE_OFFSET_T altZoneOffset() const;

    virtual bool daylightObserved() const;
    virtual RWCString timeZoneName() const;
    virtual RWCString altZoneName() const;

    // isDaylight () may use all members of tmbuf except tm_isdst
    virtual bool isDaylight(const struct tm* /* tmbuf */) const;

    // The following functions use the tm_year member of struct tm,
    // and set all the remaining members.
    virtual void getBeginDaylight(struct tm*) const;
    virtual void getEndDaylight(struct tm*) const;

};

#endif // RW_TOOLS_TOOLS_ZONE_H
