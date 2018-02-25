/*
 * Definitions for RWDate class
 *
 * $Id: rwdate.cpp,v 6.7 1994/07/18 20:51:00 jims Exp $
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
 * Parts of this code have been taken from "The NIH Class Library",
 * a public domain C++ class library written by Keith Gorlen, of the
 * National Institute of Health.
 *
 ***************************************************************************
 *
 * $Log: rwdate.cpp,v $
 * Revision 6.7  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.6  1994/07/12  22:03:24  nevis
 * Changed location of sys/types.h for MetaWare under OS/2
 *
 * Revision 6.5  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.4  1994/06/18  01:11:11  myersn
 * Allow RWDate(1,1,0) == Jan 1, 1900.
 *
 * Revision 6.3  1994/06/17  10:10:44  myersn
 * check range when constructing a date from m/d/y components.
 *
 * Revision 6.2  1994/06/02  18:42:30  foote
 * Port to Symantec 7.0 on Macintosh
 *
 * Revision 6.1  1994/04/15  19:07:33  vriezen
 * Move all files to 6.1
 *
 * Revision 2.25  1993/09/15  19:12:15  randall
 * changed BSD guard to RW_STRUCT_TM_TZ for extra struct tm members
 *
 * Revision 2.24  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.23  1993/08/06  04:32:48  randall
 * removed use of RW_CAFE_ALPHA.
 *
 * Revision 2.22  1993/08/03  18:17:00  dealys
 * Ported to MPW C++ 3.3
 *
 * Revision 2.21  1993/07/29  20:56:18  keffer
 * Corrected typo for BSD code.
 *
 * Revision 2.20  1993/07/26  05:07:24  myersn
 * under BSD, set struct tm members tm_zone and tm_gmtoff.
 *
 * Revision 2.19  1993/07/09  09:40:29  jims
 * Use localtime_r for Sun only
 *
 * Revision 2.18  1993/06/11  21:45:31  myersn
 * RWDate::dayWithinMonth was accepting dates one beyond the end of the month.
 *
 * Revision 2.17  1993/06/02  22:23:25  randall
 * added use of mt-safe function, localtime_r(...)
 *
 * Revision 2.16  1993/04/12  19:25:54  keffer
 * Added static member function RWDate::now()
 *
 * Revision 2.15  1993/04/07  00:46:59  myersn
 * add static functions to get names of months and days of the week.
 *
 * Revision 2.15  1993/04/07  00:12:22  myersn
 * add static member functions monthName(unsigned, RWLocale&) and weekDayName()
 *
 * Revision 2.13  1993/03/31  02:52:39  myersn
 * add constructor from RWCString and RWLocale.
 *
 * Revision 2.12  1993/03/30  21:07:39  myersn
 * remove RWDate::dayOfWeek and indexOfMonth to rwdate.h, as inlines.
 *
 * Revision 2.11  1993/03/25  05:53:02  myersn
 * replace monthTy, etc. with unsigned; jul1901 is now rwJul1901.
 *
 * Revision 2.10  1993/03/13  01:25:47  keffer
 * near->rwnear; far->rwfar; PARANOID_CONST_BUG->RW_PARANOID_CONST_BUG
 *
 * Revision 2.9  1993/02/17  03:12:30  keffer
 * Changed const notation to follow style guide
 *
 * Revision 2.8  1993/02/07  18:17:17  keffer
 * Simplified algorithm for weekDay();
 * previous() now returns 7 days earlier for same day of week.
 *
 * Revision 2.7  1993/01/26  21:26:31  myersn
 * remove unused monthNames and weekDayNames (now in lodfault.cpp)
 *
 * Revision 2.6  1993/01/17  03:06:24  myersn
 * removed obsoleted indexOf () functions to lodfault.cpp
 *
 * Revision 2.5  1992/12/01  05:44:03  myersn
 * *** empty log message ***
 *
 * Revision 2.4  1992/11/27  21:01:11  myersn
 * added RWZone interface
 *
 * Revision 2.3  1992/11/20  02:55:42  myersn
 * adjustments for support of locales, time zones, and struct tm
 *
 * Revision 2.1  1992/11/14  00:15:39  myersn
 * fold in interface to struct tm and RWLocale
 *
 *    Rev 1.7   29 May 1992 09:46:54   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.6   27 May 1992 18:09:08   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.5   04 Mar 1992 09:17:26   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.4   18 Feb 1992 19:29:16   KEFFER
 * No longer takes difference of two unsigned numbers.
 * 
 *    Rev 1.3   14 Nov 1991 10:07:36   keffer
 * 
 *    Rev 1.2   12 Nov 1991 09:33:58   keffer
 * Constructor now uses type "time_t" instead of "long".
 * 
 *    Rev 1.1   28 Oct 1991 09:24:24   keffer
 * Include file path now <rw/xxx.h>
 * 
 */

#include "rw/locale.h"
#include "rw/rwdate.h"
#include "rw/rwtime.h"
STARTWRAP
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#ifndef macintosh
# if defined(__HIGHC__) && defined(__OS2__)
#  include <types.h>
# else
#  include <sys/types.h>
# endif
#endif
ENDWRAP

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: rwdate.cpp,v $ $Revision: 6.7 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

/***************************************************************************/

// 	constructors

/***************************************************************************/

/*
 * Construct a RWDate with a given day of the year and a given year.  The
 * base date for this computation is Dec. 31 of the previous year.  If
 * year == 0, Construct a RWDate with Jan. 1, 1901 as the "day zero".
 * i.e., RWDate(-1) = Dec. 31, 1900 and RWDate(1) = Jan. 2, 1901.
 */

RWDate::RWDate(unsigned day, unsigned year) {
  if (year) julnum = jday(12, 31, year-1) + (unsigned long)day;
  else      julnum = rwJul1901              + (unsigned long)day;
}

//   Construct a RWDate for the given day, monName, and year.
RWDate::RWDate(
  unsigned day, const char* monName, unsigned year,
  const RWLocale& locale)
{
  julnum = jday(indexOfMonth(monName, locale), day, year);
}

//   Construct a RWDate for the given day, month, and year.
RWDate::RWDate(unsigned day, unsigned month, unsigned year)
{
  julnum = jday(month, day, year);
}

//   Construct a RWDate for the given day, month, and year.
RWDate::RWDate(const struct tm* tmbuf) 
{
  julnum = jday(tmbuf->tm_mon + 1, tmbuf->tm_mday, 1900 + tmbuf->tm_year);
}

RWDate::RWDate(const RWCString& str, const RWLocale& locale)
{
  struct tm tmbuf;
  if (locale.stringToDate(str, &tmbuf))
    julnum = jday(tmbuf.tm_mon + 1, tmbuf.tm_mday, 1900 + tmbuf.tm_year);
  else
    julnum = 0;
}

/****************************************************************
 *								*
 *			Member functions 			*
 *								*
 ****************************************************************/

// extract to struct tm components
void
RWDate::extract(
  struct tm* tmbuf) const
{
  if (!isValid()) { clobber(tmbuf); return; }
  unsigned m; unsigned d; unsigned y;
  mdy(m, d, y);
  tmbuf->tm_year = y - 1900;
  tmbuf->tm_mon = m - 1;
  tmbuf->tm_mday = d;
  tmbuf->tm_wday = weekDay() % 7;
  tmbuf->tm_yday = firstDayOfEachMonth[m - 1] + (d - 1);
  if (leapYear(y) && m > 2)
    ++tmbuf->tm_yday;
  tmbuf->tm_hour = tmbuf->tm_min = tmbuf->tm_sec = 0;
  tmbuf->tm_isdst = -1;
#ifdef RW_STRUCT_TM_TZ
  tmbuf->tm_zone = "";
  tmbuf->tm_gmtoff = 0;
#endif
}

// Compare function:
int
RWDate::compareTo(
  const RWDate* d) const
{
  RWPRECONDITION( d!=0 );
  return julnum == d->julnum ? 0 : (julnum > d->julnum ? 1 : -1);
}

unsigned
RWDate::day() const
{
  return julnum - jday(12, 31, year()-1 );
}

// Returns the day of the month of this RWDate.
unsigned 
RWDate::dayOfMonth() const
{
  unsigned m; unsigned d; unsigned y;
  mdy(m, d, y);
  return d;
}

// Return the number of the first day of a given month
// Return 0 if "month" is outside of the range 1 through 12, inclusive.
unsigned 
RWDate::firstDayOfMonth(unsigned month) const
{
  if ( !assertIndexOfMonth(month) ) return 0;
  unsigned firstDay = 1 + firstDayOfEachMonth[month-1];
  if (month > 2 && leap()) firstDay++;
  return firstDay;
}

unsigned
RWDate::hash() const
{
  return (unsigned)julnum;
}

RWDate
RWDate::max(const RWDate& dt) const
{
#ifdef RW_PARANOID_CONST_BUG
  if( dt.julnum > julnum ) return dt;
  return *this;
#else
  return dt.julnum > julnum ? dt : *this;
#endif
}

RWDate 
RWDate::min(const RWDate& dt) const 
{
#ifdef RW_PARANOID_CONST_BUG
  if( dt.julnum < julnum ) return dt;
  return *this;
#else
  return dt.julnum < julnum ? dt : *this;
#endif
}

// Returns the month of this RWDate.
unsigned 
RWDate::month() const
{
  unsigned m; unsigned d; unsigned y;
  mdy(m, d, y);
  return m;
}

RWDate
RWDate::previous(const char* dayName, const RWLocale& locale) const
{
  return previous( dayOfWeek(dayName, locale) );
}

RWDate 
RWDate::previous(unsigned desiredDayOfWeek) const
{
  RWPRECONDITION( desiredDayOfWeek >= 1);
  RWPRECONDITION( desiredDayOfWeek <= 7);

  unsigned delta = (weekDay() + 6 - desiredDayOfWeek) % 7 + 1;

  return RWDate(julnum - delta);
}

unsigned
RWDate::weekDay() const
{
  return julnum % 7 + 1;
}

// Returns the year of this RWDate.
unsigned 
RWDate::year() const
{
  unsigned m; unsigned d; unsigned y;
  mdy(m, d, y);
  return y;
}

/***************************************************************************/

// 			static member functions

/***************************************************************************/

// Is a day (1-31) within a given month?
/* static */RWBoolean 
RWDate::dayWithinMonth(unsigned month, unsigned day, unsigned year)
{
  if (day <= 0 || !assertIndexOfMonth(month) ) return FALSE;
  unsigned d = daysInMonth[month-1];
  if (leapYear(year) && month == 2) d++;
  return day <= d;
}

// How many days are in the given year?
/* static */ unsigned
RWDate::daysInYear(unsigned year)
{
  return leapYear(year) ? 366 : 365;
}

/*
 * Convert Gregorian calendar date to the corresponding Julian day
 * number j.  Algorithm 199 from Communications of the ACM, Volume 6, No.
 * 8, (Aug. 1963), p. 444.  Gregorian calendar started on Sep. 14, 1752.
 * This function not valid before that.
 * Returns 0 if the date is invalid.
 */

/* static */ unsigned long 
RWDate::jday(unsigned m, unsigned d, unsigned y)
{
  if( y <= 99 ) y += 1900;
  unsigned yh = y;
  unsigned long c, ya, j;
  if( !dayWithinMonth(m, d, y) ) return (unsigned long)0;

  if (m > 2) { m -= 3; }  // wash out the leap day
  else       { m += 9;	y--; } 
  c = y / 100;
  ya = y - 100*c;
  j = ((146097*c)>>2) + ((1461*ya)>>2) + (153*m + 2)/5 + d + 1721119;
  if (RWDate(j).year() != yh) return 0;
  return j;
} 

/* static */ RWCString
RWDate::weekDayName(unsigned day, const RWLocale& locale)
{
  struct tm tmbuf;
  tmbuf.tm_wday = day % 7;
  return locale.asString(&tmbuf, 'A');
}

/* static */ RWCString
RWDate::nameOfMonth(unsigned mon, const RWLocale& locale)
{
  struct tm tmbuf;
  tmbuf.tm_mon = mon - 1;
  return locale.asString(&tmbuf, 'B');
}

// Return today's date:
/* static */ RWDate
RWDate::now()
{
  return RWDate(RWDate::currentDate());
}

/***********************************************************************
 *                                                                     *
 *                      Private functions                              *
 *                                                                     *
 ***********************************************************************/

void
RWDate::clobber(struct tm* tmbuf)
{
  tmbuf->tm_year = tmbuf->tm_mon = tmbuf->tm_mday = tmbuf->tm_wday =
    tmbuf->tm_yday = tmbuf->tm_hour = tmbuf->tm_min = tmbuf->tm_sec =
    tmbuf->tm_isdst = ~(~0u>>1);
#ifdef RW_STRUCT_TM_TZ
  tmbuf->tm_gmtoff = ~(~0u>>1);
  tmbuf->tm_zone = (char*)~0;
#endif
}

/*
 * Convert a Julian day number to its corresponding Gregorian calendar
 * date.  Algorithm 199 from Communications of the ACM, Volume 6, No. 8,
 * (Aug. 1963), p. 444.  Gregorian calendar started on Sep. 14, 1752.
 * This function not valid before that.  
 */

void rwnear
RWDate::mdy(unsigned& m, unsigned& D, unsigned& y) const
{
  unsigned long d;
  unsigned long j = julnum - 1721119;
  y = (unsigned) (((j<<2) - 1) / 146097);
  j = (j<<2) - 1 - 146097*y;
  d = (j>>2);
  j = ((d<<2) + 3) / 1461;
  d = (d<<2) + 3 - 1461*j;
  d = (d + 4)>>2;
  m = (unsigned)(5*d - 3)/153;
  d = 5*d - 3 - 153*m;
  D = (unsigned)((d + 5)/5);
  y = (unsigned)(100*y + j);
  if (m < 10)
  	m += 3;
  else {
  	m -= 9;
  	y++;
  } 
} 

/* static */ unsigned long
RWDate::currentDate()
{
  time_t clk = time(0);
  struct tm t;
#if defined (sun) && defined(RW_MULTI_THREAD)
  localtime_r(&clk, &t);
#else
  t = *localtime(&clk);
#endif
  return jday(t.tm_mon+1, t.tm_mday, t.tm_year+1900);
}

