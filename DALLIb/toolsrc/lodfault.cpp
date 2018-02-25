/*
 * Definitions for class RWLocaleDefault
 *
 * $Id: lodfault.cpp,v 6.5 1994/07/18 20:51:00 jims Exp $
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
 * $Log: lodfault.cpp,v $
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.3  1994/06/17  09:44:05  myersn
 * Allow 1/1/00 date: means 1/1/1900.
 *
 * Revision 6.2  1994/06/15  17:10:30  jims
 * Remove theDefaultLocale to avoid static init, instead set
 * theGlobalLocale on first reference
 *
 * Revision 6.1  1994/04/15  19:07:01  vriezen
 * Move all files to 6.1
 *
 * Revision 1.28  1993/11/10  23:16:43  jims
 * Add missing declaration for strtod to ObjectStore version
 *
 * Revision 1.27  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.26  1993/08/20  04:10:22  myersn
 * appease borland.
 *
 * Revision 1.25  1993/07/29  07:33:26  myersn
 * make stringToDate less tolerant.
 *
 * Revision 1.24  1993/07/29  01:42:27  myersn
 * scope the enumeration CurrSym properly in declarations.
 *
 * Revision 1.23  1993/07/26  04:49:48  myersn
 * fix lots of little bugs.
 *
 * Revision 1.22  1993/07/13  05:16:04  myersn
 * add arg to stringToMoney(); move ~RWLocale() from locale.o; tidy up.
 *
 * Revision 1.21  1993/07/03  02:24:51  myersn
 * Implement moneyAsString() and stringToMoney(); tolerate spaces in
 * stringToNum(...).
 *
 * Revision 1.20  1993/06/23  18:54:29  myersn
 * add handling of 'C' format where supported.
 *
 * Revision 1.19  1993/05/27  18:58:39  keffer
 * Scope RWLocale::CurrSymbol
 *
 * Revision 1.18  1993/04/20  23:37:11  myersn
 * make struct tm* arguments const where appropriate.
 *
 * Revision 1.17  1993/04/09  05:20:06  myersn
 * fix asString(tm*, 'U'...) and ...'W'... problems.
 *
 * Revision 1.16  1993/04/07  03:14:36  myersn
 * eliminate obsolete RWDate functions, unnecessary use of RWDate::day().
 *
 * Revision 1.15  1993/04/06  17:08:07  myersn
 * add stubs for RWLocaleDefault::moneyAsString() and stringToMoney().
 *
 * Revision 1.14  1993/03/31  02:21:34  myersn
 * fix stringToDate/stringToTime internal bug.
 *
 * Revision 1.13  1993/03/26  23:26:45  myersn
 * handle time zone name properly.
 *
 * Revision 1.12  1993/03/25  05:48:47  myersn
 * fix isspace/isdigit bug with 8 bit chars & Solaris; eliminate
 * dayTy, MonthTy names (replace with unsigned).
 *
 * Revision 1.11  1993/02/19  04:12:10  myersn
 * make RWLocaleDefault::stringToDate() compile to less code.
 *
 * Revision 1.10  1993/02/19  03:29:43  myersn
 * add conversion from dates containing textual month names.
 *
 * Revision 1.9  1993/02/18  03:34:25  myersn
 * make LocaleDefault::asString(double...) obey the "show decimal point" flag.
 *
 * Revision 1.8  1993/02/17  03:12:30  keffer
 * Changed const notation to follow style guide
 *
 * Revision 1.7  1993/01/18  20:19:01  myersn
 * more
 *
 * Revision 1.6  1993/01/17  02:46:44  myersn
 * removed uses of strftime() for better portability.
 *
 * Revision 1.5  1992/12/04  05:02:03  myersn
 * make RWLocale::global() and RWZone::local() more flexible
 *
 * Revision 1.4  1992/12/02  00:24:14  myersn
 * changed to use "ignoreCase" rather than "insensitive" for string compares
 *
 * Revision 1.3  1992/12/01  04:50:00  myersn
 * *** empty log message ***
 *
 * Revision 1.2  1992/12/01  04:21:02  myersn
 * implement stringToDate() and stringToTime().
 *
 * Revision 1.1  1992/11/27  21:01:11  myersn
 * Initial revision
 * 
 */
#include "rw/compiler.h"
STARTWRAP
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
ENDWRAP
#include "rw/zone.h"
#include "rw/rwdate.h"
#include "rw/locale.h"


RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: lodfault.cpp,v $ $Revision: 6.5 $ $Date: 1994/07/18 20:51:00 $");

RWLocale::~RWLocale() {}

/****************************************************************
 *								*
 * 			static constants			*
 *								*
 ****************************************************************/

static const char* monthNames[12] = {
  "January","February","March","April","May","June","July",
  "August","September","October","November","December"
};
static const char* monthAbbrs[12] = {
  "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};
static const char*
weekDayNames[7] = {
  "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"
};
static const char*
weekDayAbbrs[7] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};

static const RWLocale* theGlobalLocale;

const RWLocale& RWLocale::global() 
{ 
  if (!theGlobalLocale) {
    theGlobalLocale = new RWLocaleDefault; 
  }
  return *theGlobalLocale; 
}

const RWLocale* RWLocale::global(const RWLocale* l)
{ 
  RWPRECONDITION(l);
  const RWLocale* tmp = theGlobalLocale; 
  theGlobalLocale = l; 
  return tmp; 
}

RWLocaleDefault::RWLocaleDefault() {}
RWLocaleDefault::~RWLocaleDefault() {} 

RWCString RWLocaleDefault::asString(long l) const
  { char buf[40]; sprintf(buf, "%ld", l); return RWCString(buf); }
RWCString RWLocaleDefault::asString(unsigned long ul) const
  { char buf[40]; sprintf(buf, "%lu", ul); return RWCString(buf); }

RWCString RWLocaleDefault::asString(double f, int prec, RWBoolean show) const
{
  char buf[80];
  sprintf(buf, (show?"%#.*f":"%.*f"), prec, f);
  return RWCString(buf);
}

RWCString
RWLocaleDefault::asString(
  const struct tm* tmbuf,
  char format,
  const RWZone& zone) const
{
  char buf[256]; buf[0] = 0;
  int tmp;
  switch (format) {
  case 'x': {
      int year = tmbuf->tm_year;
      if (year > 0 && year < 100)
	sprintf(buf, "%02d/%02d/%02d",
	        tmbuf->tm_mon + 1, tmbuf->tm_mday, year);
      else
	sprintf(buf, "%02d/%02d/%04d",
	        tmbuf->tm_mon + 1, tmbuf->tm_mday, year + 1900);
    } break;
  case 'X': sprintf(buf, "%02d:%02d:%02d",
                    tmbuf->tm_hour, tmbuf->tm_min, tmbuf->tm_sec); break;
  case 'c': {
      return asString(tmbuf, 'x', zone) + " " + asString(tmbuf, 'X', zone);
    }
#if !defined(RW_NO_STRFTIME_CAPC)
  case 'C': {
      RWCString zname = this->asString(tmbuf, 'Z', zone);
      sprintf(buf, "%3s %3s %2d %02d:%02d:%02d %s %4d",
        weekDayAbbrs[(tmbuf->tm_wday + 6)%7],
        monthAbbrs[tmbuf->tm_mon],  tmbuf->tm_mday,
	tmbuf->tm_hour, tmbuf->tm_min, tmbuf->tm_sec, zname.data(),
	tmbuf->tm_year + 1900);
    }
#endif
  case 'a': return weekDayAbbrs[(tmbuf->tm_wday + 6) % 7];
  case 'A': return weekDayNames[(tmbuf->tm_wday + 6) % 7];
  case 'b': return monthAbbrs[tmbuf->tm_mon];
  case 'B': return monthNames[tmbuf->tm_mon];
  case 'd': sprintf(buf, "%02d", tmbuf->tm_mday); break;
  case 'm': sprintf(buf, "%02d", tmbuf->tm_mon + 1); break;
  case 'M': sprintf(buf, "%02d", tmbuf->tm_min); break;
  case 'S': sprintf(buf, "%02d", tmbuf->tm_sec); break;
  case 'w': sprintf(buf, "%1d", tmbuf->tm_wday); break;
  case 'p': return (tmbuf->tm_hour < 12) ? "am" : "pm";
  case 'I':
      tmp = tmbuf->tm_hour % 12;
      if (tmp == 0) tmp = 12;

      sprintf(buf, "%02d", tmp);
      break;
  case 'H': sprintf(buf, "%02d", tmbuf->tm_hour); break;
  case 'y':
      tmp = tmbuf->tm_year + 1900;
      sprintf(buf, "%02d", tmp % 100);
      break;
  case 'Y': sprintf(buf, "%04d", tmbuf->tm_year + 1900); break;
  case 'j': sprintf(buf, "%03d", tmbuf->tm_yday + 1);    break;
  case 'Z': {
    if ((tmbuf->tm_isdst & ~1) == 0) {  // equal to 1 or 0
        if (tmbuf->tm_isdst) return zone.altZoneName();
	else                 return zone.timeZoneName();
      }
    }      break;
  case 'U':      // with provisional thanks to P.J. Plauger
      tmp = (tmbuf->tm_yday - tmbuf->tm_wday + 7) / 7;
      sprintf(buf, "%02d", tmp);
      break;
  case 'W':
      tmp = (tmbuf->tm_yday - ((tmbuf->tm_wday + 6) % 7) + 7) / 7;
      sprintf(buf, "%02d", tmp);
      break;
  default: { return RWCString(); }
  }
  return RWCString(buf);
}

RWCString
RWLocaleDefault::moneyAsString(
  double val,
#ifndef RW_GLOBAL_ENUMS
  RWLocale::
#endif
            CurrSymbol sym) const
{
  char buf[32];
  char* bp = buf;
  if (val < 0) { *bp++ = '('; val = -val; }
  switch (sym) {
    case INTL: memcpy(bp, "USD ", 4); bp += 4; break;
    case LOCAL: *bp++ = '$';
    case NONE: ;
  }
#ifndef RW_NO_ANSI_SPRINTF
  bp += sprintf(bp, ((val < 1e18) ? "%.2f" : "%.5e"), val/100);
  if (*buf == '(') { *bp++ = ')'; *bp = 0; }
#else
  // sprintf fails to return the number of bytes generated:
  sprintf(bp, ((val < 1e18) ? "%.2f" : "%.5e"), val/100);
  if (*buf == '(') {
    while (*bp) ++bp;
    *bp++ = ')'; *bp = 0;
  }
#endif
  return RWCString(buf);
}

static const char* skipSpaces(const char* sp)
  { while (isspace((unsigned char)*sp)) ++sp; return sp; }

static char eatSign(const char*& sp)
  { char c = *sp; sp = skipSpaces(sp+1); return c; }

RWBoolean
RWLocaleDefault::stringToNum( const RWCString& s, double* fp) const
{
  const char* sp = skipSpaces(s.data());
  char sign = (*sp == '-' || *sp == '+') ? eatSign(sp) : 0;
  if (!isdigit((unsigned char)*sp) && *sp != '.') return FALSE;
  double result = strtod(sp, (char**)&sp);
  sp = skipSpaces(sp);
  if (*sp) return FALSE;
  *fp = (sign == '-' ? -result : result);
  return TRUE;
}

RWBoolean 
RWLocaleDefault::stringToNum(const RWCString& s, long* lp) const
{ 
  const char* sp = skipSpaces(s.data());
  char sign = (*sp == '-' || *sp == '+') ? eatSign(sp) : 0;
  if (!isdigit((unsigned char)*sp)) return FALSE;
  long result = strtol(sp, (char**)&sp, 10); 
  sp = skipSpaces(sp);
  if (*sp) return FALSE;
  *lp = (sign == '-' ? -result : result);
  return TRUE;
}

static const char*
skipToDigit(const char* bp)
{ while (*bp && !isdigit((unsigned char)*bp)) ++bp; return bp; }

RWBoolean
RWLocaleDefault::stringToDate(const RWCString& s, struct tm* tmbuf) const
{
  const char* bp = s.data();
  char* ep = (char*) bp;  // a place for strtol to store a result
  long month = 11;
  for (; month >= 0; --month) {
    if (s.index(monthNames[month], 0, RWCString::ignoreCase) != RW_NPOS) break;
    if (s.index(monthAbbrs[month], 0, RWCString::ignoreCase) != RW_NPOS) break;
  }
  ++month;  // now month is 1..12, or 0 for not found.
  bp = skipToDigit(bp);
  if (month == 0) {  // have not yet got the month
    month = strtol(bp, &ep, 10); 
    if (ep == bp) return 0;
    else bp = ep;
  }
  bp = skipToDigit(bp);
  long mday =  strtol(bp, &ep, 10);
  if (ep == bp) return 0;
  else bp = ep;
  bp = skipToDigit(bp);
  long year =  strtol(bp, &ep, 10);
  if (ep == bp) return 0;
  else bp = ep;
  bp = skipToDigit(bp);
  if (*bp) return 0;  // too many numbers
  if (month < 1 || month > 12) return 0;
//  if (year < 1) return 0;
  if (year < 100) year += 1900;
  if (mday < 1 || mday > (long) RWDate::daysInMonth[month-1] &&
      !(month == 2 && mday == 29 && RWDate::leapYear((unsigned int)year)))
    return 0;
  tmbuf->tm_year = (int)(year - 1900);
  tmbuf->tm_mon = (int)(month - 1);
  tmbuf->tm_mday = (int)mday;
  return 1;
}

RWBoolean
RWLocaleDefault::stringToTime(const RWCString& s, struct tm* tmbuf) const
{
  char* ep = 0;
  const char* bp = skipSpaces(s.data());
  long hour = (int) strtol(bp, &ep, 10); if (ep == bp) return 0;
  bp = (*ep == ':' || *ep == '.') ? ep+1 : ep;
  long minute = (int) strtol(bp, &ep, 10); if (ep == bp) minute = 0;
  bp = (*ep == ':' || *ep == '.') ? ep+1 : ep;
  long second = (int) strtol(bp, &ep, 10); if (ep == bp) second = 0;
  bp = skipSpaces(ep);
  char p = toupper((unsigned char) *bp);
  if ((p == 'A' || p == 'P') && (toupper((unsigned char)bp[1]) == 'M')) {
    if (hour < 1 || hour > 12) return 0;
    else if (hour == 12) hour = (p == 'P') ? 12 : 0;
    else if (p == 'P') hour += 12;
  } else if (hour < 0 || hour > 23) return 0;
  if (minute < 0 || minute > 59) return 0;
  if (second < 0 || second > 59) return 0;
  tmbuf->tm_hour = (int)hour;
  tmbuf->tm_min = (int)minute;
  tmbuf->tm_sec = (int)second;
  return 1;
}

RWBoolean
RWLocaleDefault::stringToMoney(
  const RWCString& s,
  double* val,
#ifndef RW_GLOBAL_ENUMS
  RWLocale::
#endif
            CurrSymbol sym) const
{
  if (sym == NONE) return FALSE;
  const char* sp = skipSpaces(s.data());
  char sign = 0;
  if (sym == LOCAL) {
    if (*sp == '-' || *sp == '(') sign = eatSign(sp);
    if (*sp == '$') eatSign(sp);
  } else if (memcmp(sp, "USD", 3) == 0) {
    // INTL format is strict: USD [-]nnn.nn
    sp = skipSpaces(sp + 3);
  }
  if (!sign && (*sp == '-' || *sp == '(')) { sign = eatSign(sp); }
  const char* units_at = sp;
  double bux = 0.0;
  while (isdigit((unsigned char)*sp))
    bux = bux * 10 + (*sp++ - '0');
  int cents = 0;
  if (*sp == '.' &&
      isdigit((unsigned char)sp[1]) && isdigit((unsigned char)sp[2])) {
    cents = 10 * (sp[1] - '0') + (sp[2] - '0');
    sp += 3;
  }
  if (sp == units_at) return FALSE;  // empty string not a valid number.
  sp = skipSpaces(sp);
  if (sign == '(' && *sp == ')' || sym == LOCAL && !sign && *sp == '-')
    sign = eatSign(sp);
  if (*sp) return FALSE;
  bux = bux * 100 + cents;
  *val = sign ? -bux : bux;
  return TRUE;
}

int RWLocaleDefault::monthIndex(const RWCString& s) const
{
  int i = 11;
  do {
    if (s.compareTo(monthNames[i], RWCString::ignoreCase) == 0 ||
        s.compareTo(monthAbbrs[i], RWCString::ignoreCase) == 0)
      return (i + 1);
  } while (--i >= 0);
  return 0;
}

int RWLocaleDefault::weekdayIndex(const RWCString& s) const
{
  int i = 6;
  do {
    if (s.compareTo(weekDayNames[i], RWCString::ignoreCase) == 0 ||
        s.compareTo(weekDayAbbrs[i], RWCString::ignoreCase) == 0)
      return (i + 1);
  } while (--i >= 0);
  return 0;
}

///////////////////////////////////////////////////////////
