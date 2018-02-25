/*
 * Definitions for classes RWDaylight and RWZone 
 *
 * $Id: zone.cpp,v 6.7 1994/07/18 20:51:00 jims Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 *
 * (c) Copyright 1989, 1990, 1991, 1992, 1993, 1994 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 52.227-7013.
 * 
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 ***************************************************************************
 *
 *
 ***************************************************************************
 *
 * $Log: zone.cpp,v $
 * Revision 6.7  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.6  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.5  1994/06/24  22:48:38  nevis
 * Fixed bug 526, GMT observing DST!
 *
 * Revision 6.4  1994/06/15  17:12:17  jims
 * Set daylight rules on first reference to avoid static init
 *
 * Revision 6.3  1994/06/02  19:12:51  foote
 * Port to Symantec 7.0 on Macintosh
 *
 * Revision 6.2  1994/05/16  17:43:24  jims
 * Port to Win32 DLL
 *
 * Revision 6.1  1994/04/15  19:08:15  vriezen
 * Move all files to 6.1
 *
 * Revision 1.35  1993/12/30  00:18:40  myersn
 * Fake up TZ names when RW_STRUCT_TM_TZ indicates real names are not available.
 *
 * Revision 1.34  1993/09/13  08:59:53  myersn
 * change the default usage from "daylight" to "_daylight", and use
 * the flag RW_NO_LEADING_UNDERSCORE to choose the old usage.
 *
 * Revision 1.33  1993/09/10  20:18:49  jims
 * Add "RW" prefix to ZoneManager; rename Zones to RWStaticZones
 *
 * Revision 1.32  1993/09/10  17:30:49  jims
 * Add Win16 DLL instance support for local, std, and utc zones,
 * Add RW_RCSID string, Change $Header to $Id
 *
 * Revision 1.31  1993/09/08  00:54:38  myersn
 * call tzset() in makelocal().
 *
 * Revision 1.30  1993/09/07  03:56:25  jims
 * Port to release version of Windows NT SDK
 *
 * Revision 1.29  1993/09/05  03:53:26  myersn
 * defer initialization of default zones until asked for, to avoid problems
 * with static initialization, and unnecessary initialization.
 *
 * Revision 1.28  1993/08/25  20:03:38  myersn
 * cast enum to int before applying arithmetic (breaks Zortech, otherwise)
 *
 * Revision 1.27  1993/08/13  15:38:30  dealys
 * fixed typo introduced in 1.24
 *
 * Revision 1.26  1993/08/06  17:45:46  keffer
 * Changed name ZoneDefault to ZoneSimple for clarity.
 *
 * Revision 1.25  1993/08/06  04:33:45  randall
 * removed use of RW_CAFE_ALPHA
 *
 * Revision 1.24  1993/08/03  18:29:49  dealys
 * Ported to MPW C++ 3.3
 *
 * Revision 1.22  1993/07/09  09:40:29  jims
 * Use localtime_r for Sun only
 *
 * Revision 1.21  1993/07/03  02:24:51  myersn
 * fix DST calculation for case of "last sunday in month", and also
 * add support for Nth day of month, to permit mainland Chinese DST.
 *
 * Revision 1.20  1993/06/02  22:24:56  randall
 * added use of mt-safe function, localtime_r(...)
 *
 * Revision 1.19  1993/05/26  00:56:54  keffer
 * Ported to Microsoft C++ under NT
 *
 * Revision 1.18  1993/05/18  00:17:40  myersn
 * fix previous fix.
 *
 * Revision 1.17  1993/05/17  21:34:29  myersn
 * remove dependency of BSD code on mktime(), which is not generally available.
 *
 * Revision 1.16  1993/04/20  19:43:36  myersn
 * fix some SunOS 4 incompatibilities (?: exprs, RW_NO_WSTR, RW_NO_GLOBAL_TZ)
 *
 * Revision 1.15  1993/04/13  23:15:41  myersn
 * correct names of standard time zones.
 *
 * Revision 1.14  1993/04/13  20:39:18  myersn
 * fix construction of UTC time zone.
 *
 * Revision 1.13  1993/04/13  08:37:54  myersn
 * add RWZoneSimple(StdZone, DstRule) constructor.
 *
 * Revision 1.12  1993/04/12  21:54:31  myersn
 * add labels for time zones.
 *
 * Revision 1.11  1993/04/12  11:50:00  jims
 * Added work-around for Sun Cafe Alpha problem with static const
 *
 * Revision 1.10  1993/04/09  23:17:26  myersn
 * generalize and simplify making time zones.
 *
 * Revision 1.9  1993/03/25  05:58:48  myersn
 * replace monthTy with unsigned; eliminate RWDaylightRule::mustFree_ member.
 *
 * Revision 1.8  1993/03/12  02:21:06  myersn
 * make usRule D.S.T. rules available to users defining zones.
 *
 * Revision 1.7  1993/02/17  03:12:30  keffer
 * Changed const notation to follow style guide
 *
 * Revision 1.6  1993/01/26  20:55:39  myersn
 * add observed_ flag to RWDaylightRule, fix firstDay list and use of next_
 * field in daylight rules.
 *
 * Revision 1.5  1992/12/16  00:53:10  myersn
 * improvements to daylight rule interface
 *
 * Revision 1.4  1992/12/04  05:02:03  myersn
 * make RWLocale::global() and RWZone::local() more flexible
 *
 * Revision 1.2  1992/11/27  21:01:11  myersn
 * Generalize into complete time zone encapsulation, independent of RWLocale
 *
 * Revision 1.1  1992/11/20  02:55:42  myersn
 * Initial revision
 */

#include "rw/zone.h"
#include "rw/rwdate.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: zone.cpp,v $ $Revision: 6.7 $ $Date: 1994/07/18 20:51:00 $");

#if defined(__WIN16__) && defined(_RWBUILDDLL)
#  include "rw/instmgr.h"
#endif

/****************************************************************
 *								*
 * 	    RWDate members used for DST calculations  	*
 *								*
 ****************************************************************/

// Global static constants also used by RWTime and RWLocale:
const unsigned char
RWDate::daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

const unsigned
RWDate::firstDayOfEachMonth[12] = {0,31,59,90,120,151,181,212,243,273,304,334};

// Algorithm from K & R, "The C Programming Language", 1st ed.
RWBoolean
RWDate::leapYear(unsigned year)
{
  return (year&3) == 0 && (year%100 != 0 || year % 400 == 0);
}

/*
 * The following is designed to define timeZone(), altZone() and
 *      daylightObserved().
 * Synopsis:
 *	If you are using Unix or Borland, you will need to set 
 *	your environment variable TZ.  For example, for Pacific
 *	coast time, set TZ=PDT8PST.  For other time zones, see
 *	your respective manuals.  For other compilers, you
 *	will probably have to set the static variables
 *	below to appropriate local values.
 */

STARTWRAP
/* #include <stdio.h> */  /* what for? */
#ifndef macintosh
# include <sys/types.h>
#endif
ENDWRAP


#ifndef RW_NO_GLOBAL_TZ  /* Use System V-like environment timezone features */

  STARTWRAP
# include <time.h>
  ENDWRAP
# if !defined(__FLAT__) && defined(__DLL__) && defined(__TURBOC__) && (__TURBOC__ >= 0x400)
    extern "C" int  far * far __getDaylight();
    extern "C" long far * far __getTimezone();

    static RWZone*
    makelocal() {
      tzset();
      return new RWZoneSimple(*__getDaylight() ? RWZone::dstRule() : 0,
        *__getTimezone(), "", *__getTimezone() - 3600, "");
    }

# else
#if defined(RW_NO_LEADING_UNDERSCORE)

    static RWZone*
    makelocal() {
      tzset();
      return new RWZoneSimple(daylight ? RWZone::dstRule() : 0,
		      timezone, tzname[0], timezone - 3600, tzname[1]);
    }

#   else  /* The modern approach */

    static RWZone*
    makelocal() {
      _tzset();
      return new RWZoneSimple(_daylight ? RWZone::dstRule() : 0,
		      _timezone, _tzname[0], _timezone - 3600, _tzname[1]);
    }

#   endif

# endif

#else  /* no global tz and daylight */
#if !defined(RW_NO_GETTIMEOFDAY)

    // Use Berkeley-like gettimeofday() features for time zones.
  STARTWRAP
# include <sys/time.h>
  ENDWRAP
# if defined(__DECCXX) || defined(__LPI__)
      // This was left out by DEC's and Liant's C++:
    extern "C" int gettimeofday(struct timeval *tp, struct timezone *tzp);
# endif

      // Use Berkeley-like gettimeofday() features for time zones.

    static RWZone*
    makelocal() {
      struct timeval tval;            /* see <sys/time.h> */
      struct timezone tz;             /* see <sys/time.h> */
      gettimeofday(&tval, &tz);
      
      struct tm tmbuf;
      time_t t = time(0);
#     if defined (sun) && defined(RW_MULTI_THREAD)
        localtime_r(&t, &tmbuf);
#     else
        tmbuf = *localtime(&t);
#     endif


      RWCString zname, altname;

#  if defined(RW_STRUCT_TM_TZ)

      int mon = tmbuf.tm_mon;
      time_t t2 = t + (5 - mon) * (3600*24*30L);
#     if defined (sun) && defined(RW_MULTI_THREAD)
        localtime_r(&t2, &tmbuf);
#     else 
        tmbuf = *localtime(&t2);
#     endif
      if (tmbuf.tm_isdst) altname = tmbuf.tm_zone;
      else                zname = tmbuf.tm_zone;
      t2 = t + (11 - mon) * (3600*24*30L);
#     if defined (sun) && defined(RW_MULTI_THREAD)
        localtime_r(&t2, &tmbuf);
#     else 
        tmbuf = *localtime(&t2);
#     endif
      if (tmbuf.tm_isdst) altname = tmbuf.tm_zone;
      else                zname = tmbuf.tm_zone;

#  else   /* RW_STRUCT_TM_TZ */

      // Convex doesn't provide time zone names in struct tm,
      // so we don't know the time zone name.
      zname = "?ST";
      altname = "?DT"; 

#  endif  /* RW_STRUCT_TM_TZ */

      return new RWZoneSimple(
        tz.tz_dsttime ? RWZone::dstRule() : 0,
	60*(tz.tz_minuteswest), zname,
	60*(tz.tz_minuteswest-60), altname);

    }

# else  /* no gettimeofday, and no global tz or daylight, either */

    /*
     * If you end up here, your compiler does not support setting
     * timezones from the environment.  Zortech and VMS, in particular,
     * do not.  You will have to hardwire in an appropriate initial value
     * below.  These can be adjusted at runtime.
     */

    static RWZone*
    makelocal() {
      return new RWZoneSimple(RWZone::dstRule(),
		      3600 * 8, "PST", 3600 * 7, "PDT");
    }

#   endif  /* no gettimeofday, no tz */
#   endif  /* no tz */

/****************************************************************
 *								*
 * 	    Daylight members			  		*
 *								*
 ****************************************************************/

// this is defined to prevent the compiler from generating it inline.
// RWDaylightRule has no constructor, so that it may be initialized
// statically.
RWDaylightRule&
RWDaylightRule::operator=(const RWDaylightRule& arg)
{
  begin_ = arg.begin_;
  end_ = arg.end_;
  return *this;
}

// No constructors, so we can have static instances.
// RWDaylightRule::RWDaylightRule(const RWDaylightRule& arg)
//  : begin_(arg.begin_), end_(arg.end_) { }

// Find the mday of the wday in nth week, in the month & year in tmbuf.
static int
dateOfNthWday(int nthweek, int wday, const struct tm* tmbuf)
{
  if (nthweek < 0) {
    return wday;  // just the Nth day of the month, no foolishness.
  } else if (nthweek < 4) {
    return (nthweek * 7 + ((wday - tmbuf->tm_wday) +
                           (tmbuf->tm_mday - 1) + 7) % 7) + 1;
  } else {
    // find last target weekday in month
    int daysInMonth = RWDate::daysInMonth[tmbuf->tm_mon];
    if (tmbuf->tm_mon == 1  && RWDate::leapYear(tmbuf->tm_year + 1900))
      ++daysInMonth;
    return (daysInMonth - ((tmbuf->tm_wday - wday) - 
                           (tmbuf->tm_mday - daysInMonth) + 7) % 7);
  }
}

RWBoolean
RWDaylightBoundary::after(
  const struct tm* tmbuf) const
{
  // given tm_year, tm_mon, tm_mday and tm_wday, report whether
  // that time is after the boundary for that year.
  if (tmbuf->tm_mon != month_)
    return (tmbuf->tm_mon < month_);
  int boundary = dateOfNthWday(week_, weekday_, tmbuf);
  if (boundary != tmbuf->tm_mday)
      return (tmbuf->tm_mday < boundary);
  return (tmbuf->tm_hour * 60 + tmbuf->tm_min < minute_);
}

void
RWDaylightBoundary::date(
  struct tm* tmbuf) const
{
  // given tm_year, tm_mon, and any tm_mday and tm_wday in the month
  // of the boundary, set tm_mday, tm_wday, and the time
  tmbuf->tm_mday = dateOfNthWday(week_, weekday_, tmbuf);
  tmbuf->tm_wday = weekday_;
  tmbuf->tm_hour = minute_ / 60;
  tmbuf->tm_min = minute_ % 60;
  tmbuf->tm_sec = 0;
  tmbuf->tm_isdst = -1;
}

RWBoolean
RWDaylightRule::isDaylight(
  const struct tm* tmbuf) const
{
  if (!observed_) return 0;
  int b = begin_.after(tmbuf);
  int e = end_.after(tmbuf);
  return ((b == e) != (begin_.month_ < end_.month_));
}

///////////////////////////////////////////////////////////////////
//
//  RWZone
//
///////////////////////////////////////////////////////////////////

#if defined(__WIN16__) && defined(_RWBUILDDLL)
/*
 * Maintain unique zones for each task using the DLL:
 */
struct RWStaticZones { 
  const RWZone* localzone; 
  const RWZone* stdzone;   
  const RWZone* utczone;   
  RWStaticZones() { localzone = stdzone = utczone = NULL; }
};

class RWZoneManager : public RWInstanceManager
{
    // Redefined from RWInstanceManager; Supplies initial value:	
    virtual void rwfar*	newValue()
      { return (void rwfar*) new RWStaticZones; }

    // Redefined from RWInstanceManager; deletes value:
    virtual void deleteValue(void rwfar* p) { delete (RWStaticZones*) p; }
public:
    /*
     * Get current value.  If no currentValue, then this is a new task, so
     * initialize with default value and return that.
     */	
    RWStaticZones& getCurrentZones()
      { RWStaticZones* zones = (RWStaticZones*) currentValue();
        if (zones)  return *zones;
	else        return *(RWStaticZones*) addValue();
      }
};

static RWZoneManager zoneManager;

#define localzone (zoneManager.getCurrentZones().localzone)
#define stdzone (zoneManager.getCurrentZones().stdzone)
#define utczone (zoneManager.getCurrentZones().utczone)

#else /* Not building a Win16 DLL */

static const RWZone* localzone;  // static init to 0
static const RWZone* stdzone;    // static init to 0
static const RWZone* utczone;    // static init to 0

#endif  /* defined(__WIN16__) && defined(_RWBUILDDLL) */

const RWZone& RWZone::local()
{
  if (!localzone) localzone = makelocal();
  return *localzone;
}

const RWZone* RWZone::local(const RWZone* zone)
  { const RWZone* tmp = localzone; localzone = zone; return tmp; }

const RWZone& RWZone::standard()
{
  if (!stdzone) stdzone = new RWZoneSimple(local().timeZoneOffset(),
                                           local().timeZoneName());
  return *stdzone;
}

const RWZone* RWZone::standard(const RWZone* zone)
  { const RWZone* tmp = stdzone; stdzone = zone; return tmp; }

const RWZone& RWZone::utc()
{
  if (!utczone) utczone = new RWZoneSimple(RWZone::Greenwich, RWZone::NoDST);
  return *utczone;
}

RWZone::~RWZone() {}

/*
 * The first field of each struct would normally be set at link time,
 * but not all platforms support this.  Therefore they are set explicitly
 * at first reference at run-time below.  Same for the "rules" array.
 * See TAN 39.
 */
static RWDaylightRule usRuleAuld =
  { 0,                   0000, 1, { 3, 4, 0, 120 }, { 9, 4, 0, 120 } };
static RWDaylightRule usRule67 =
  { 0 /* &usRuleAuld */,         1967, 1, { 3, 0, 0, 120 }, { 9, 4, 0, 120 } };
static RWDaylightRule usRule74 =
  { 0 /* &usRule67 */,           1974, 1, { 0, 0, 0, 120 }, { 9, 4, 0, 120 } };
static RWDaylightRule usRule75 =
  { 0 /* &usRule74 */,           1975, 1, { 1, 4, 0, 120 }, { 9, 4, 0, 120 } };
static RWDaylightRule usRule76 =
  { 0 /* &usRule75 */,           1976, 1, { 3, 4, 0, 120 }, { 9, 4, 0, 120 } };
static RWDaylightRule usRuleLate =
  { 0 /* &usRule76 */,           1987, 1, { 3, 0, 0, 120 }, { 9, 4, 0, 120 } };

static RWDaylightRule euRuleAuld =
  { 0, /* &euRuleAuld */         0000, 1, { 2, 4, 0, 120 }, { 8, 4, 0, 120 } };
static RWDaylightRule euRuleLate =
  { 0, /* &euRule96 */           1996, 1, { 2, 4, 0, 120 }, { 9, 4, 0, 120 } };

static const RWDaylightRule* rules[3]; // = { 0, &usRuleLate, &euRuleLate };
static RWBoolean rulesSet = FALSE;

const RWDaylightRule* RWZone::dstRule(DstRule rule) 
{ 
  if (!rulesSet) {
    usRule67.next_   = &usRuleAuld;
    usRule74.next_   = &usRule67;
    usRule75.next_   = &usRule74;
    usRule76.next_   = &usRule75;
    usRuleLate.next_ = &usRule76;
    euRuleLate.next_ = &euRuleAuld;
    rules[0] = 0;
    rules[1] = &usRuleLate;
    rules[2] = &euRuleLate;  //BUG 526; used to read rules[0] (ouch!)
    rulesSet = TRUE;
  }
  return rules[rule]; 
}

struct _RWZoneNames { char zname[8]; char altname[8]; };
static _RWZoneNames zones[24] = {
  { "GMT+12", "GMT+11" }, // NewZealand
  { "GMT+11", "GMT+10" }, // CarolineIslands
  { "GMT+10", "GMT+9" }, // MarianaIslands
  { "GMT+9", "GMT+8" }, // Japan
  { "GMT+8", "GMT+7" }, // China
  { "GMT+7", "GMT+6" }, // Java
  { "GMT+6", "GMT+5" }, // Kazakh
  { "GMT+5", "GMT+4" }, // Pakistan
  { "GMT+4", "GMT+3" }, // CaspianSea
  { "GMT+3", "GMT+2" }, // Ukraine
  { "EET",   "EET DST" }, // Nile
  { "MET",   "MET DST" }, // Europe
  { "GMT",   "WET DST" }, // Greenwich
  { "GMT-1", "GMT-2" }, // Azores
  { "GMT-2", "GMT-3" }, // Oscar
  { "GMT-3", "GMT-4" }, // Greenland
  { "GMT-4", "GMT-5" }, // Atlantic
  { "EST", "EDT" }, // USEastern
  { "CST", "CDT" }, // USCentral
  { "MST", "MDT" }, // USMountain
  { "PST", "PDT" }, // USPacific
  { "YST", "YDT" }, // Yukon
  { "GMT-10", "GMT-11" }, // Hawaii
  { "GMT-11", "GMT-12" }, // Bering
};

RWZoneSimple::RWZoneSimple(long zoff, const RWCString& zname)
: rule_(0), daylightObserved_(0)
, timeZoneOffset_(zoff), timeZoneName_(zname)
, altZoneOffset_(zoff), altZoneName_(timeZoneName_)
{ }

RWZoneSimple::RWZoneSimple(
  RWZone::StdZone zone,
  const RWDaylightRule* rule)
: rule_(rule), daylightObserved_(rule != 0)
, timeZoneOffset_(zone * 3600)
, timeZoneName_(zones[zone+12].zname)
, altZoneOffset_((int(zone)-daylightObserved_) * 3600)
, altZoneName_(timeZoneName_) // "?:" expr here would break some compilers
{
  if (daylightObserved_) altZoneName_ = RWCString(zones[zone+12].altname);
}

RWZoneSimple::RWZoneSimple(
  RWZone::StdZone zone,
  RWZone::DstRule rulename)
: rule_(RWZone::dstRule(rulename))
, daylightObserved_(rulename != RWZone::NoDST)
, timeZoneOffset_(zone * 3600)
, timeZoneName_(zones[zone+12].zname)
, altZoneOffset_((int(zone)-daylightObserved_) * 3600)
, altZoneName_(timeZoneName_)  // "?:" expr here would break some compilers
{ 
  if (daylightObserved_) altZoneName_ = RWCString(zones[zone+12].altname);
}

RWZoneSimple::~RWZoneSimple() { }

int RWZoneSimple::timeZoneOffset()         const { return timeZoneOffset_; }
int RWZoneSimple::altZoneOffset()          const { return altZoneOffset_; }
RWBoolean RWZoneSimple::daylightObserved() const { return daylightObserved_; }
RWCString RWZoneSimple::timeZoneName()     const { return timeZoneName_; }
RWCString RWZoneSimple::altZoneName()      const { return altZoneName_; }

static RWDaylightRule const*
findRule(
  const RWDaylightRule* rule,
  const struct tm* tmbuf)
{
  short year = (short) tmbuf->tm_year + 1900;
  while (rule && rule->firstYear_ > year) rule = rule->next_;
  return rule;
}

// isDaylight() uses on all members of tmbuf except tm_isdst and tm_yday.
RWBoolean
RWZoneSimple::isDaylight(
  const struct tm* tmbuf) const
{
  if (!daylightObserved_) return 0;
  const RWDaylightRule* rule = findRule(rule_, tmbuf);
  if (rule) return rule->isDaylight(tmbuf);
  return 0;
}

// The following functions use the tm_year member of struct tm, and
//   set all the remaining members.
void
RWZoneSimple::getBeginDaylight(
  struct tm* tmbuf) const
{
  if (!daylightObserved_) { RWDate::clobber(tmbuf); return; }
  const RWDaylightRule* rule = findRule(rule_, tmbuf);
  if (!rule || !rule->observed_) { RWDate::clobber(tmbuf); return; }
  tmbuf->tm_mon = rule->begin_.month_;
  tmbuf->tm_mday = 1;
  RWDate day(tmbuf);
  tmbuf->tm_wday = day.weekDay() % 7;
  rule->begin_.date(tmbuf);
}

void
RWZoneSimple::getEndDaylight(
  struct tm* tmbuf) const
{
  if (!daylightObserved_) { RWDate::clobber(tmbuf); return; }
  const RWDaylightRule* rule = findRule(rule_, tmbuf);
  if (!rule || !rule->observed_) { RWDate::clobber(tmbuf); return; }
  tmbuf->tm_mon = rule->end_.month_;
  tmbuf->tm_mday = 1;
  RWDate day(tmbuf);
  tmbuf->tm_wday = day.weekDay() % 7;
  rule->end_.date(tmbuf);
}
