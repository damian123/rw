/*
 * Definitions for class RWTime
 *
 * $Id: rwtime.cpp,v 6.8 1994/07/18 20:51:00 jims Exp $
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
 * Implementation of class RWTime
 * Provides an object that represents a RWTime, stored as the number of
 * seconds since January 1, 1901.
 *
 * Parts of this code have been taken from "The NIH Class Library",
 * a public domain C++ class library written by Keith Gorlen, of the
 * National Institute of Health.
 *
 ***************************************************************************
 *
 * $Log: rwtime.cpp,v $
 * Revision 6.8  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.7  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.6  1994/07/01  00:49:14  vriezen
 * Fixes endDST similarly to begineDST.  Log# 1581.
 *
 * Revision 6.5  1994/06/21  06:44:57  myersn
 * fix RWTime::beginDST(), broken by previous fixes.
 *
 * Revision 6.4  1994/06/17  09:59:55  myersn
 * check lower bound on date, mark time invalid if exceeded.
 *
 * Revision 6.3  1994/06/17  09:13:03  myersn
 * improve behavior around DST transition
 *
 * Revision 6.2  1994/06/14  23:49:28  jims
 * Replace refDate and maxDate with their julian day counterparts
 * to avoid static init constructor calls
 *
 * Revision 6.1  1994/04/15  19:07:42  vriezen
 * Move all files to 6.1
 *
 * Revision 2.24  1994/04/13  19:02:00  jims
 * Fix RWTime::isDST to adjust for passed-in RWZone parameter
 *
 * Revision 2.23  1994/01/04  01:19:23  myersn
 * Allow IBM AIX to use efficient conversion from time_t to RWTime.
 *
 * Revision 2.22  1993/10/29  06:15:58  myersn
 * check for a bad date when constructing a time from one.
 *
 * Revision 2.21  1993/09/15  19:12:15  randall
 * changed BSD guard to RW_STRUCT_TM_TZ for extra struct tm members
 *
 * Revision 2.20  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.19  1993/08/06  04:33:45  randall
 * removed use of RW_CAFE_ALPHA
 *
 * Revision 2.18  1993/07/26  05:07:24  myersn
 * under BSD, set struct tm members tm_zone and tm_gmtoff.
 *
 * Revision 2.17  1993/07/09  09:40:29  jims
 * Use localtime_r for Sun only
 *
 * Revision 2.16  1993/06/02  22:24:29  randall
 * added use of mt-safe function localtime_r(...)
 *
 * Revision 2.15  1993/05/01  01:16:43  keffer
 * refDate and maxDate are no longer static data in RWTime.
 *
 * Revision 2.14  1993/04/12  19:25:54  keffer
 * Added static member function RWTime::now().
 *
 * Revision 2.12  1993/03/31  02:52:39  myersn
 * add constructor from RWCString and RWLocale.
 *
 * Revision 2.11  1993/03/26  23:26:45  myersn
 * fix RWTime::extract() setting tm_isdst member.
 *
 * Revision 2.10  1993/03/25  05:55:15  myersn
 * change minuteTy etc. to unsigned.  secFrom_Jan_1_(etc) becomes rwEpoch.
 *
 * Revision 2.9  1993/03/17  21:21:15  keffer
 * far->rwfar; added SYSV and BSD to unix
 *
 * Revision 2.8  1993/02/17  03:12:30  keffer
 * Changed const notation to follow style guide
 *
 * Revision 2.7  1992/12/04  05:02:03  myersn
 * make RWLocale::global() and RWZone::local() more flexible
 *
 * Revision 2.6  1992/12/02  02:38:00  myersn
 * removed mutual recursion between RWTime::extract() and
 * RWDate::RWDate(const RWTime&, const RWZone&)
 *
 * Revision 2.5  1992/12/01  05:44:03  myersn
 * *** empty log message ***
 *
 * Revision 2.4  1992/11/27  21:01:11  myersn
 * added RWZone interface
 *
 * Revision 2.4  1992/11/27  21:01:11  myersn
 * added RWZone interface
 *
 * Revision 2.3  1992/11/20  02:55:42  myersn
 * adjustments for support of locales, time zones, and struct tm
 *
 * Revision 2.2  1992/11/14  00:15:39  myersn
 * fold in interface to struct tm and RWLocale
 *
 * Revision 2.1  1992/11/02  00:38:03  myersn
 * fix spelling of "Bering" time zone name
 * 
 *    Rev 1.11   07 Jun 1992 14:27:22   KEFFER
 * Ported to Liant: added prototype for gettimeofday().
 * 
 *    Rev 1.10   24 Apr 1992 10:39:46   KEFFER
 * Ported to DEC's C++.
 * M.F. localSecs() now adjusts for DST using UTC, not local time.
 * Simplified buildFrom() algorithm.
 * 
 *    Rev 1.9   04 Mar 1992 10:27:20   KEFFER
 * Changed PRECONDITION to RWPRECONDITION
 * 
 *    Rev 1.8   25 Nov 1991 12:40:00   keffer
 * TIME_ZONE is now consistently declared "long"
 * 
 *    Rev 1.6   13 Nov 1991 11:49:18   keffer
 * TIME_ZONE and DST_OBSERVED obtained from functions for the DLL version.
 * 
 *    Rev 1.5   28 Oct 1991 09:24:28   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.4   20 Sep 1991 19:07:34   keffer
 * Added additional preconditions.
 * Corrected error in compareTo().
 * 
 *    Rev 1.3   08 Sep 1991 19:25:40   keffer
 * Corrected type conversion error for times between midnight and 1AM.
 * 
 *    Rev 1.2   22 Aug 1991 10:19:40   keffer
 * Simplified construction of times by defining m.f. buildFrom()
 * 
 *    Rev 1.1   29 Jul 1991 14:42:12   keffer
 * VMS must also hardwire in timezone.
 * 
 */

#include "rw/rwdate.h"
#include "rw/rwtime.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: rwtime.cpp,v $ $Revision: 6.8 $ $Date: 1994/07/18 20:51:00 $");

static const unsigned long SECONDS_IN_DAY  = 86400L;
static const unsigned long SECONDS_IN_HOUR = 3600L;
static const unsigned      SECONDS_IN_MIN  = 60;

/*
 * RWTime constants
 * use julian day numbers (instead of RWDates) so as not to depend 
 * on static constructor initialization:
 */
const unsigned long refDate_jday = rwJul1901; // jday for RWDate(0, 0)
const unsigned long maxDate_jday = 2465095L;  // jday for RWDate(49709, 0); 
                                              // ((2**32)-1)/SECONDS_IN_DAY -1

/****************************************************************
 *								*
 *			CONSTRUCTORS				*
 *								*
 ****************************************************************/

// Specified time and today's date:
RWTime::RWTime(unsigned h,
	       unsigned m,
	       unsigned s,
	       const RWZone& zone)
{
  RWDate today;
  sec = RWTime::buildFrom(today, h, m, s, zone);
}

RWTime::RWTime(const struct tm* tmbuf,
	       const RWZone& zone)
{
  RWDate date(tmbuf);
  sec = RWTime::buildFrom(date, tmbuf->tm_hour,
			  tmbuf->tm_min, tmbuf->tm_sec, zone);
}

RWTime::RWTime(const RWDate& date,
               const RWCString& str,
               const RWZone& zone,
               const RWLocale& locale)
{
  struct tm tmbuf;
  sec = date.isValid() && locale.stringToTime(str, &tmbuf) ?
    buildFrom(date,
	      tmbuf.tm_hour,
	      tmbuf.tm_min,
	      tmbuf.tm_sec,
	      zone) : 0;
}

/*************** conversion from RWTime to RWDate *******************/

RWDate::RWDate(const RWTime& time, const RWZone& zone)
{
  struct tm tmbuf;
  time.extract(&tmbuf, zone);
  *this = RWDate(&tmbuf);
}

/****************************************************************
 *								*
 *			PUBLIC FUNCTIONS			*
 *								*
 ****************************************************************/

int
RWTime::compareTo(const RWTime* t) const
{
  RWPRECONDITION( t!=0 );
  RWPRECONDITION( RWTime::isValid() );
  RWPRECONDITION( t->RWTime::isValid() );
  return sec == t->sec ? 0 : (sec > t->sec ? 1 : -1);
}

// Hash function:
unsigned
RWTime::hash() const
{
  RWPRECONDITION( RWTime::isValid() );
  return (unsigned)sec;
}

void
RWTime::extractGMT(struct tm* tmbuf) const
{
  RWDate date((unsigned)(sec / SECONDS_IN_DAY), 0);
  date.extract(tmbuf);
  tmbuf->tm_hour = hourGMT();
  tmbuf->tm_min = minuteGMT();
  tmbuf->tm_sec = second();
  tmbuf->tm_isdst = 0;
#ifdef RW_STRUCT_TM_TZ
  tmbuf->tm_zone = "GMT";
  tmbuf->tm_gmtoff = 0;
#endif  
}

static RWBoolean 
checkDST(const RWTime& t, struct tm* tmb, const RWZone& zone)
{
  (t - zone.timeZoneOffset()).extractGMT(tmb);
  if (!zone.isDaylight(tmb)) return 0;
  struct tm alt;
  (t - zone.altZoneOffset()).extractGMT(&alt);
  if (!zone.isDaylight(&alt)) return 0;
  *tmb = alt;
  return 1;
}

void
RWTime::extract(struct tm* tmbuf, const RWZone& zone) const
{
  if (!isValid()) { RWDate::clobber(tmbuf); return; }
  if (!zone.daylightObserved())
    (*this - zone.timeZoneOffset()).extractGMT(tmbuf);
  else if (checkDST(*this, tmbuf, zone)) {
    tmbuf->tm_isdst = 1;
# ifdef RW_STRUCT_TM_TZ
    tmbuf->tm_zone = (char*) zone.altZoneName().data();
    tmbuf->tm_gmtoff = zone.altZoneOffset();
# endif
    return;
  }
# ifdef RW_STRUCT_TM_TZ
  tmbuf->tm_zone = (char*) zone.timeZoneName().data();
  tmbuf->tm_gmtoff = zone.timeZoneOffset();
# endif
}

unsigned
RWTime::hour(const RWZone& zone) const
{
  RWPRECONDITION( RWTime::isValid() );
  struct tm tmbuf;
  extract(&tmbuf, zone);
  return tmbuf.tm_hour;
}

unsigned   
RWTime::hourGMT() const 
{
  RWPRECONDITION( RWTime::isValid() );
  return (sec % SECONDS_IN_DAY) / SECONDS_IN_HOUR;
} 

RWBoolean 
RWTime::isDST(const RWZone& zone) const
{
  RWPRECONDITION( RWTime::isValid() );
  if (!zone.daylightObserved()) return FALSE;
  struct tm tmbuf;
  return checkDST(*this, &tmbuf, zone);
}

RWTime
RWTime::max(const RWTime& t) const 
{
  RWPRECONDITION( RWTime::isValid() );
  RWPRECONDITION( t.RWTime::isValid() );
  if ( *this > t ) return *this;
  return t;
}

RWTime
RWTime::min(const RWTime& t) const 
{
  RWPRECONDITION( RWTime::isValid() );
  RWPRECONDITION( t.RWTime::isValid() );
  if ( *this < t ) return *this;
  return t;
}

unsigned
RWTime::minute(const RWZone& zone) const
{
  RWPRECONDITION( RWTime::isValid() );
  struct tm tmbuf;
  extract(&tmbuf, zone);
  return tmbuf.tm_min;
}

unsigned 
RWTime::minuteGMT() const 
{
  RWPRECONDITION( RWTime::isValid() );
  return ((sec % SECONDS_IN_DAY) % SECONDS_IN_HOUR) / SECONDS_IN_MIN;
} 

// second(); local time or UTC

unsigned 
RWTime::second() const 
{
  RWPRECONDITION( RWTime::isValid() );
  return ((sec % SECONDS_IN_DAY) % SECONDS_IN_HOUR) % SECONDS_IN_MIN;
}

/*************** public static member functions *******************/

/* static */ RWTime 
RWTime::beginDST(unsigned year, const RWZone& zone)
{
  if (zone.daylightObserved()) {
    struct tm tmbuf;
    tmbuf.tm_year = year - 1900;
    zone.getBeginDaylight(&tmbuf);
    return RWTime(&tmbuf, RWZone::utc()) + zone.timeZoneOffset();
  } else
    return RWTime((unsigned long) 0);
}

/* static */ RWTime
RWTime::endDST(unsigned year, const RWZone& zone)
{
  if (zone.daylightObserved()) {
    struct tm tmbuf;
    tmbuf.tm_year = year - 1900;
    zone.getEndDaylight(&tmbuf);
    return RWTime(&tmbuf, RWZone::utc()) + zone.altZoneOffset();
  } else
    return RWTime((unsigned long)0);
}

/* static */ RWTime
RWTime::now()
{
  return RWTime(RWTime::currentTime());
}

/****************************************************************
 *								*
 *			PROTECTED FUNCTIONS			*
 *								*
 ****************************************************************/

RWBoolean 
RWTime::assertDate(const RWDate& date)
{
  return date.between(RWDate(refDate_jday), RWDate(maxDate_jday));
}   

/*
 * Set self from the specified (local) Date, hour, minute, and second.
 * Note: this algorithm will fail if DST correction is something other
 * than an hour.
 * It is complicated by the DST boundary problem: 
 * 1) Times in the phantom zone between 2AM and 3AM when DST is invoked
 *    are invalid.
 * 2) Times in the hour after 1AM when DST ends, are redundant.
 *    In this case, standard time should be chosen.
 */


/****************************************************************
 *								*
 *			PRIVATE FUNCTIONS			*
 *								*
 ****************************************************************/

/* static */ unsigned long
RWTime::buildFrom(
  const RWDate& date,
  unsigned h, unsigned m, unsigned s,
  const RWZone& zone)
{
  if (!date.isValid()) return 0;
  if (date < RWDate(refDate_jday)) return 0;
  unsigned long secs = SECONDS_IN_DAY  * (date - RWDate(refDate_jday))
                     + SECONDS_IN_HOUR * h
                     + SECONDS_IN_MIN  * m
                     +                   s;
  if (zone.daylightObserved())
  {
    struct tm tmbuf;
    date.extract(&tmbuf);
    tmbuf.tm_hour = h; tmbuf.tm_min = m; tmbuf.tm_sec = s;
    if (zone.isDaylight(&tmbuf))
    { 
      secs += zone.altZoneOffset();
      if (!RWTime(secs).isDST()) return 0;  // that nonexistent hour!
      return secs;
    }
  }
  secs += zone.timeZoneOffset();  
  return secs;
}

unsigned long
RWTime::currentTime()
{
#if defined (SYSV) || defined (BSD) || defined(unix) || defined(_AIX)
  unsigned long secs = (unsigned long)(time(0));
  secs += rwEpoch;
  return secs;
#else  /* MS-DOS: use time struct. This is awkward, but guarantees correct local time */
  time_t ltime;
  time(&ltime);
  struct tm t;
#if defined (sun) && defined(RW_MULTI_THREAD)
  localtime_r(&ltime, &t);
#else
  t = *localtime(&ltime);
#endif    /*  end RW_MULTI_THREAD  */
  // Construct the date.  The time struct returns int, so casts are used.
  RWDate today((unsigned)t.tm_mday, (unsigned)(t.tm_mon + 1), (unsigned)t.tm_year);
  return buildFrom(today, (unsigned)t.tm_hour, (unsigned)t.tm_min, (unsigned)t.tm_sec);
#endif
}
