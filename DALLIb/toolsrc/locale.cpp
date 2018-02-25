
/*
 * Definitions for RWLocale and RWLocaleSnapshot class.
 *
 * $Id: locale.cpp,v 6.7 1994/07/27 18:57:20 jims Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 * (503) 754-3010   Fax: (503) 757-6650  email: support@roguewave.com
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
 * $Log: locale.cpp,v $
 * Revision 6.7  1994/07/27  18:57:20  jims
 * Fix spelling error
 *
 * Revision 6.6  1994/07/22  18:15:33  vriezen
 * Cover for xlC for localeconv()->grouping returning null pointer.
 *
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/18  06:42:27  jims
 * Simplify conditional expression in RWLocaleSnapshot::monthIndex
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/07/07  21:17:47  jims
 * When determining preferred order of date components, if year
 * is not present in string returned from strftime, assume it comes last.
 *
 * Revision 6.1  1994/04/15  19:06:59  vriezen
 * Move all files to 6.1
 *
 * Revision 1.43  1994/03/19  00:20:20  jims
 * RWLocaleSnapshot::monthIndex now ignores leading spaces when
 * comparing strings
 *
 * Revision 1.42  1993/11/09  09:01:23  jims
 * Port to ObjectStore
 *
 * Revision 1.41  1993/10/29  06:15:58  myersn
 * eliminate redundant/insufficient mutex locking.
 *
 * Revision 1.40  1993/09/14  16:34:58  keffer
 * Wrapped RWUNLOCK in a #ifdef RW_MULTI_THREAD to suppress bogus warnings
 * from Symantec and WATCOM.
 *
 * Revision 1.39  1993/09/11  22:44:58  keffer
 * Removed spurious assert().
 *
 * Revision 1.38  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.37  1993/09/09  23:43:51  myersn
 * accommodate MSC/Win32 long date format
 *
 * Revision 1.37  1993/09/09  21:02:22  myersn
 * add dummy definition for RW_NO_LOCALE case, for picky linkers.
 *
 * Revision 1.35  1993/08/06  17:42:44  jims
 * Remove declaration of unused variable "chars"
 *
 * Revision 1.34  1993/08/03  18:12:37  dealys
 * Ported to MPW C++ 3.3
 *
 * Revision 1.33  1993/08/02  23:51:57  myersn
 * minor cleanup.
 *
 * Revision 1.32  1993/07/29  01:42:27  myersn
 * scope the enumeration CurrSym properly in declarations.
 *
 * Revision 1.31  1993/07/28  22:40:59  keffer
 * Changed to reflect new RWCString interface.
 *
 * Revision 1.30  1993/07/28  21:47:35  myersn
 * more fixes to support japanese dates.
 * tolerate broken (i.e. Sun) locales, for money formatting.
 *
 * Revision 1.28  1993/07/20  21:29:23  myersn
 * make constructing RWLocaleSnapshot MT-safe.
 *
 * Revision 1.27  1993/07/13  05:16:04  myersn
 * implement stringToMoney(), remove ~RWLocale().
 *
 * Revision 1.27  1993/07/13  02:25:24  myersn
 * implement stringToMoney().
 *
 * Revision 1.26  1993/07/10  02:24:09  myersn
 * add support for money -> string conversion.
 *
 * Revision 1.25  1993/06/29  01:33:43  myersn
 * Detect space-filled month number, allow month name to contain a number.
 * (Necessary to support the Japanese locale.)
 *
 * Revision 1.24  1993/05/27  18:56:18  keffer
 * Casts away constness of a struct tm to satisfy SunOS 4.X strftime()
 *
 * Revision 1.23  1993/05/26  00:12:21  keffer
 * Ported to MS under NT
 *
 * Revision 1.22  1993/04/20  23:37:11  myersn
 * make struct tm* arguments const where appropriate.
 *
 * Revision 1.21  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 1.20  1993/04/09  05:20:06  myersn
 * fix asString(tm*, 'U'...) and ...'W'... problems.
 *
 * Revision 1.19  1993/04/08  04:30:42  myersn
 * fix leading in day-of-month bug.
 *
 * Revision 1.18  1993/04/06  17:08:07  myersn
 * add stubs for RWLocaleSnapshot::moneyAsString() and stringToMoney().
 *
 * Revision 1.17  1993/04/01  03:04:16  myersn
 * produce space-filled day-of-month field in date strings where needed.
 *
 * Revision 1.16  1993/03/31  02:21:34  myersn
 * add constructor RWLocaleSnapshot(const char* locName).
 *
 * Revision 1.15  1993/03/30  21:07:39  myersn
 * fix weekdayIndex()
 *
 * Revision 1.14  1993/03/26  23:26:45  myersn
 * handle time zone name properly.
 *
 * Revision 1.13  1993/03/25  05:48:07  myersn
 * fix isspace/isdigit bug with 8 bit chars & Solaris.
 *
 * Revision 1.12  1993/03/04  22:50:17  myersn
 * initialize RWLocaleSnapshot::dateOrder_ properly.
 *
 * Revision 1.11  1993/02/18  04:24:28  myersn
 * tolerate space as a digit group separator, without getting confused about
 * trailing spaces.
 *
 * Revision 1.10  1993/02/17  23:03:34  myersn
 * fix reversed test of result of RWCString::index() in stringToDate().
 *
 * Revision 1.9  1993/02/17  03:12:30  keffer
 * Changed const notation to follow style guide
 *
 * Revision 1.8  1993/02/14  05:18:57  myersn
 * change use of RWCString::index() to use a size_t result properly.
 *
 * Revision 1.7  1993/01/23  03:00:45  keffer
 * Simplified expressions to avoid cfront v2.1 loop.
 *
 * Revision 1.6  1992/12/02  00:24:14  myersn
 * changed to use "ignoreCase" rather than "insensitive" for string compares
 *
 * Revision 1.5  1992/12/01  05:44:03  myersn
 * *** empty log message ***
 *
 * Revision 1.4  1992/11/27  21:01:11  myersn
 * added RWZone interface
 *
 * Revision 1.4  1992/11/27  21:01:11  myersn
 * added RWZone interface
 *
 * Revision 1.3  1992/11/20  02:55:42  myersn
 * adjustments for support of locales, time zones, and struct tm
 *
 * Revision 1.2  1992/11/17  23:27:06  myersn
 * *** empty log message ***
 *
 * Revision 1.1  1992/11/14  00:16:15  myersn
 * Initial revision
 *
 * 
 */

#include "rw/compiler.h"
#ifndef RW_NO_LOCALE
STARTWRAP
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(RW_NO_GLOBAL_TZ) && !defined(RW_NO_GETTIMEOFDAY)
#include <sys/time.h>
#endif
ENDWRAP
#include "rw/rwdate.h"
#include "rw/locale.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: locale.cpp,v $ $Revision: 6.7 $ $Date: 1994/07/27 18:57:20 $");

#if defined(RW_NO_ANSI_SPRINTF) 
/* SunOS 4.x has C library problems: */

extern "C" time_t mktime(tm*);    // sun doesn\'t declare this in time.h
STARTWRAP
# include <stdarg.h>    /* for SPRINTF function below */
ENDWRAP
static int
SPRINTF(char* buf, const char* format ... )
{
  va_list ap;
  va_start(ap, format);
  vsprintf(buf, format, ap);	// ignore result, because that\'s wrong too!
  va_end(ap);
  return (int) strlen(buf);
}

#else /* has ANSI sprintf() */
# define SPRINTF sprintf
#endif /* ANSI sprintf */

static char digits[] = "0123456789";

enum RWTimeComp {
  // note: order matters; abbreviations and short strings come last.
  YEAR4, YEAR2, JULIANDAY, SECS, MINS, HOUR12, HOUR24,
  MONTHNUM, MONTH, MONABB, WEEKDAY, WEEKABB, AMPM,
  MONTHDAY, TIMEZONE, SUNWEEK, MONWEEK, WEEKDAYNUM,
  // note: these are space-filling versions:
  MONTHDAYSP, MONTHNUMSP,

  // warning: timeFmt tables are indexed by these values.  Changes
  //          here must be reflected below!
  NUM_TIMECOMP
};

// the following tables are indexed by RWTimeComp enum values.
static const char timeFmtSpecs[ NUM_TIMECOMP+1] =
  "YyjSMIHmBbAapdZUWw\004\015";
static const char timeFmtDigits[NUM_TIMECOMP] =
  { 4, 2, 3, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 2, 0, 2, 2, 1, 2, 2 };

struct RWTimeFmtList {
  RWTimeFmtList* next;
  RWTimeComp match;
  int offset;

  RWTimeFmtList(RWTimeComp c, int of) : next(0), match(c), offset(of) {}
  ~RWTimeFmtList();
};

RWTimeFmtList::~RWTimeFmtList() { if (next) delete next; }

#if 0
// mbstrstr() is an O(n^2) implementation of strstr() for multibyte strings
//   (not tested)

static const char*
mbstrstr(
  const char* str,
  const char* pattern)
{
  // translate pattern and format strings to wide format
  wchar_t wpat[64]; wpat[sizeof(wpat)/sizeof(wchar_t) - 1] = 0;
  assert( mbstowcs(wpat, pattern, sizeof(wpat)/sizeof(wchar_t) - 1) > 0 );
  wchar_t wstr[256]; wstr[sizeof(wstr)/sizeof(wchar_t) - 1] = 0;
  assert( mbstowcs(wstr, str, sizeof(wstr)/sizeof(wchar_t) - 1) > 0 );

  const wchar_t* ws = wstr;
  const wchar_t* wp = wpat;
  mblen(0, 0);	// initialize
  const char* match = str;
  while (*ws) {
    if (*wp == *ws) {
      const wchar_t* save = ws;
      while (*++wp && *++ws && *wp == *ws)
        ;
      if (*wp == 0) break;       // found a match!
      if (*ws == 0) return 0;    // no match possible
      ws = save;
      wp = wpat;
    }
    match += mblen(match, MB_CUR_MAX);	// mark spot in source string
    ws++;
  }
  if (*wp == 0)
    return match;
  return 0;
}
#endif

// extractFormat() finds substrings, passed in via comps[], in the
//  format string.  It returns with the format string stripped of the
//  substrings; its return value is a list of the positions in
//  the stripped string of all substrings found there.

static RWTimeFmtList*
extractFormat(
  char* format,
  const char* comps[NUM_TIMECOMP])
{
  RWTimeFmtList* out = 0;
  for (int i = 0; i < NUM_TIMECOMP; ++i) {
    if (comps[i] == 0 || *comps[i] == '\0')
      continue;

    // find the component in the format string, if present
    char* off = strstr(format, comps[i]);
    if (!off)
      continue;

    // remove the substring from the format string
    int len = (int) strlen(comps[i]);
    char* cp = off;
    while ((cp[0] = cp[len]) != '\0')
      ++cp;
    // make a record for the component.
    RWTimeFmtList *ele = new RWTimeFmtList((RWTimeComp) i, int(off - format));

    // insert it in the output list
    RWTimeFmtList* old = 0;
    RWTimeFmtList* e = out;
    while (e && e->offset <= ele->offset)
      old = e, e = e->next;
    ele->next = e;
    if (old)
      old->next = ele;
    else
      out = ele;

    // adjust other offsets
    for (; e; e = e->next)
      e->offset -= len;
  }
  return out;
}

static size_t
genFormat(
  char  spec,
  char* buf, 
  int buflen,
  const struct tm* tmb)
{
  char fmtbuf[6];
#if defined(_MSC_VER) && defined(__WIN32__)
  if (spec == 'C') strcpy(fmtbuf+1, "%#c");
  else
#endif
  { fmtbuf[0] = '%'; fmtbuf[1] = spec; fmtbuf[2] = 0; }
  // Cast away constness of tmb to satisfy SunOS 4.x:
  return (size_t) strftime(buf, buflen, fmtbuf, (struct tm*)tmb);
}

static void
patchFormat(
  char spec,
  RWTimeFmtList* fmtList,
  char* buf,
  int buflen,
  struct tm* tmb)
{
  char buf2[256];
  genFormat(spec, buf2, sizeof(buf2), tmb);
  for (; fmtList; fmtList = fmtList->next) {
    int* tmfield;
    RWTimeComp replace;
    if (fmtList->match == MONTHDAY) {
      tmfield = &tmb->tm_mday;
      replace =          MONTHDAYSP;
    } else if (fmtList->match == MONTHNUM) {
      tmfield = &tmb->tm_mon;
      replace =                 MONTHNUMSP;
    } else
      continue;

    int old = *tmfield;
    *tmfield = 2;
    genFormat(spec, buf, buflen, tmb);
    *tmfield = old;
    char* p = buf; char* q = buf2;
    while (*p && *p == *q) ++p, ++q;
    if (*p == ' ' && isdigit((unsigned char)*q))
      fmtList->match = replace;
  }
}

static RWBoolean hasDigits(const char* s)
{
  while (*s) if (isdigit((unsigned char)*s++)) return TRUE;
  return FALSE;
}

static void fix(char& val, int low, int high, int sub)
  { if (val < low || val > high) val = sub; }

RWLocaleSnapshot::RWLocaleSnapshot(const char* localeName)
:    locale_name_      (setlocale(LC_ALL, 0)) // stash the old name
,    decimal_point_    ((setlocale(LC_ALL, localeName), // set the new locale
                        localeconv()->decimal_point))
,    thousands_sep_    (localeconv()->thousands_sep)

// xlC might return a null pointer for grouping
#ifdef __xlC__
,    grouping_         (localeconv()->grouping ? localeconv()->grouping : "")
#else
,    grouping_         (localeconv()->grouping)
#endif

,    int_curr_symbol_  (localeconv()->int_curr_symbol)
,    currency_symbol_  (localeconv()->currency_symbol)
,    mon_decimal_point_(localeconv()->mon_decimal_point)
,    mon_thousands_sep_(localeconv()->mon_thousands_sep)
,    mon_grouping_     (localeconv()->mon_grouping)
,    positive_sign_    (localeconv()->positive_sign)
,    negative_sign_    (localeconv()->negative_sign)
,    int_frac_digits_  (localeconv()->int_frac_digits)
,    frac_digits_      (localeconv()->frac_digits)
,    p_cs_precedes_    (localeconv()->p_cs_precedes)
,    p_sep_by_space_   (localeconv()->p_sep_by_space)
,    n_cs_precedes_    (localeconv()->n_cs_precedes)
,    n_sep_by_space_   (localeconv()->n_sep_by_space)
,    p_sign_posn_      (localeconv()->p_sign_posn)
,    n_sign_posn_      (localeconv()->n_sign_posn)
,    am_('\0', 2)
,    pm_('\0', 2)
,    monthNameHasDigits_(0)  // aargh!
,    timeFmt_('\0', 7)
,    timeFmtList_(0)
,    dateFmt_('\0', 7)
,    dateFmtList_(0)
,    dateTimeFmt_('\0', 7)
,    dateTimeFmtList_(0)
#if !defined(RW_NO_STRFTIME_CAPC)
,    dateLongFmt_('\0', 7)
,    dateLongFmtList_(0)
#endif
,   localDivisor_(1.0)
,   intlDivisor_(1.0)
{
  // Assumes Gregorian calendar
  RWCString oldlocale = locale_name_;
  locale_name_ = RWCString(setlocale(LC_ALL, 0));
  
  tm tmb;
  // Friday November 30 1990  22:54:56; an unusually useful date/time.
  tmb.tm_sec = 56;	// NB: the order of members here can vary
  tmb.tm_min = 54;	//   from one machine to another, so a static
  tmb.tm_hour = 22;     //   init cannot be used.
  tmb.tm_mday = 30;
  tmb.tm_mon = 10;
  tmb.tm_year = 90;
  tmb.tm_wday = 5;
  tmb.tm_yday = -1;
  tmb.tm_isdst = -1;

  (void) mktime(&tmb);
  tmb.tm_hour = 22;    // set again in case mktime clobbered it.
  tmb.tm_min = 54;

  // extract names for days of the week
  for (int day = 0; day < 7; ++day) {
    char buf[256];
    int oldDay = tmb.tm_wday;
    tmb.tm_wday = day;
    strftime(buf, sizeof(buf), "%A", &tmb);
    weekDays_[day] = buf;
    strftime(buf, sizeof(buf), "%a", &tmb);
    weekAbbs_[day] = buf;
    tmb.tm_wday = oldDay;
  }

  // extract names for months
  for (int month = 0; month < 12; ++month) {
    char buf[256];
    int oldMonth = tmb.tm_mon;
    tmb.tm_mon = month;
    strftime(buf, sizeof(buf), "%B", &tmb);
    monthNames_[month] = buf;
    if (hasDigits(buf)) monthNameHasDigits_++;
    strftime(buf, sizeof(buf), "%b", &tmb);
    monthAbbrs_[month] = buf;
    if (hasDigits(buf)) monthNameHasDigits_++;
    tmb.tm_mon = oldMonth;
  }

  // extract am and pm strings, if any.
  {
    char buf[256];
    int oldHour = tmb.tm_hour;
    tmb.tm_hour = 5;
    strftime(buf, sizeof(buf), "%p", &tmb);
    am_ = buf;
    tmb.tm_hour = 15;
    strftime(buf, sizeof(buf), "%p", &tmb);
    pm_ = buf;
    tmb.tm_hour = oldHour;
  }

  // Extract time, date, and abbreviated date formats.
  // This code will fail if the strftime-generated strings
  // contain shift codes used in tricky ways.
  {
    char* comps[NUM_TIMECOMP];
    char buf[256];

    // generate strings for all the time/date components for sample date
    for (int i = 0; i < NUM_TIMECOMP; ++i) {
      size_t len = genFormat(timeFmtSpecs[i], buf, sizeof(buf), &tmb);
      comps[i] = (char*) memcpy((new char[len + 1]), buf, len + 1);
    }

    genFormat('X', buf, sizeof(buf), &tmb);
    timeFmtList_ = extractFormat(buf, (const char**) comps);
    timeFmt_ = buf;

    genFormat('x', buf, sizeof(buf), &tmb);
    dateFmtList_ = extractFormat(buf, (const char**) comps);
    dateFmt_ = buf;
    patchFormat('x', dateFmtList_, buf, sizeof(buf), &tmb);

    genFormat('c', buf, sizeof(buf), &tmb);
    dateTimeFmtList_ = extractFormat(buf, (const char**) comps);
    dateTimeFmt_ = buf;
    patchFormat('c', dateTimeFmtList_, buf, sizeof(buf), &tmb);

#if !defined(RW_NO_STRFTIME_CAPC)  /* not in ANSI C standard, but useful */
    genFormat('C', buf, sizeof(buf), &tmb);
    dateLongFmtList_ = extractFormat(buf, (const char**) comps);
    dateLongFmt_ = buf;
    patchFormat('C', dateLongFmtList_, buf, sizeof(buf), &tmb);
#endif

    for (i = 0; i < NUM_TIMECOMP; ++i)
      delete [] comps[i];
  }

  // discover preferred order of date components
  {
    int found_day = 0, found_month = 0, found_year = 0;
    int order = 0;
    for (RWTimeFmtList* e = dateFmtList_; e; e = e->next) {
      switch (e->match) {
       case MONTH:  case MONABB:  case MONTHNUM:
       case MONTHNUMSP: found_month = ++order; break;
       case YEAR4:
       case YEAR2:      found_year  = ++order; break;
       case MONTHDAY:
       case MONTHDAYSP: found_day   = ++order; break;
       default:         break;
      }
    }
    // If no year present, assume it comes last:
    if (!found_year) found_year = ++order;

    dateOrder_ = DMY;
    if (found_year < found_month)
      dateOrder_ = YDM;
    if (found_month < found_day)
      dateOrder_ = (RWDateOrder) ((int) dateOrder_ + 1);
    // other date formats are possible, e.g. (Julian day, year)
  }

  // construct the money formats (positive & negative)
  {
    fix(frac_digits_, 0, 18, 0);
    fix(int_frac_digits_, 0, 18, 0);
    fix(n_sign_posn_, 0, 4, 0);
    fix(p_sign_posn_, 0, 4, 1);
    // encoding: '(': enclose in parens; 'm': monetary value;
    //           '$': currency symbol; '-': sign; ' ': optional space.
    static const char moneyFormats[] = 
      "(m $" "-m $" "m $-" "m- $" "m $-" "($ m" "-$ m" "$ m-" "-$ m" "$ -m";
    positiveMoneyFormat_ = moneyFormats +
      20 * (p_cs_precedes_&1) + (p_sign_posn_ << 2);
    negativeMoneyFormat_ = moneyFormats + 
      20 * (n_cs_precedes_&1) + (n_sign_posn_ << 2);
    for (int j = frac_digits_; --j >= 0; localDivisor_ *= 10) {}
    for (int k = int_frac_digits_; --k >= 0; intlDivisor_ *= 10) {}
  }

  if (localeName) {    // locale was changed; restore previous locale
    setlocale(LC_ALL, oldlocale.data());
  }
}

RWLocaleSnapshot::~RWLocaleSnapshot()
{
  delete timeFmtList_;
  delete dateFmtList_;
  delete dateTimeFmtList_;
#if !defined(RW_NO_STRFTIME_CAPC)
  delete dateLongFmtList_;
#endif
}

static int             // returns the number of multibyte characters used
insert_separators(
  const char* inp,         // the input string
  int len,           // number of characters in input
  char** outpp,      // pointer to pointer to end of output buffer
  const char* grouping,    // the locale digit grouping spec
  const char* separator,   // the digit group separator multibyte character
  int sep_len)       // number of bytes in the separator
{
  int count = 0;
  const char* bp = inp + len;	// point at terminator
  const char* lastp = bp;    	// point at last separator
  char* op = *outpp;      // point at (assumed) terminator
  while (++count, (*--op = *--bp), bp > inp) {
    if (lastp - bp == (*grouping & 0xF) && bp[-1] != '-') {
      ++ count;
      op -= sep_len;
      memcpy(op, separator, sep_len);
      if (grouping[1])
        grouping++;
      lastp = bp;
    }
  }
  *outpp = op;
  return count;
}

int
RWLocaleSnapshot::fmt(
  char** bufpp,
  long i) const
{
  unsigned long ui = i;
  if (i < 0)
    ui = -ui;	// avoid overflow
  int chars = fmt(bufpp, ui);
  if (i < 0)
    ++chars, *--*bufpp = '-';
  return chars;
}

int
RWLocaleSnapshot::fmt(
  char** bufpp,
  unsigned long ui) const
{
  char mybuf[256];
  char* mp = mybuf + sizeof(mybuf) - 1;
  do {
    int digit = int(ui % 10);
    ui /= 10;
    *--mp = digits[digit];
  } while (ui != 0);
  **bufpp = '\0';
  int chars = insert_separators(mp, int(sizeof(mybuf) - (mp - mybuf) - 1),
    bufpp, grouping_.data(), thousands_sep_.data(),
    thousands_sep_.length());
  return chars;
}

int
RWLocaleSnapshot::fmt(
  char** bufpp,
  double f,
  int precision,
  int showdot,
  const RWCString& dec_point,
  const RWCString& thou_sep,
  const RWCString& grouping) const
{
  char mybuf[256];
  char* op = *bufpp;       // output pointer
  int chars = 0;           // count of output characters
  // format into mybuf; bp points to the terminating NUL.
  char* bp = mybuf + SPRINTF(mybuf, "%.*f", precision, f);
  // locate end of integral part
  char* ep = (*mybuf == '-') ? mybuf + 1 : mybuf;
  while (isdigit((unsigned char)*ep))
    ++ep;
  if (*ep) {                // there is a fraction part
    mblen(0, 0);            // init static state of mblen(), if any. Bleah.
    char* frac_p = ep;
    do {                    // skip over decimal point sequence
      frac_p += mblen(frac_p, MB_CUR_MAX);
    } while (!isdigit((unsigned char)*frac_p));
    while (bp > frac_p) {  // copy fraction part to output buffer
      *--op = *--bp;
      ++chars;
    }
  }
  // now install the radix separator, if called for
  if (*ep || showdot) {
    int dot_len = dec_point.length();
    op -= dot_len;
    memcpy(op, (const char*) dec_point, dot_len);
    ++chars;  // increment char count only once
  }
  *bufpp = op;
  // install the integral part, with digit group separators
  chars += insert_separators(mybuf, int(ep - mybuf), bufpp,
    grouping.data(), thou_sep.data(), thou_sep.length());
  return chars;
}

// numtostr() converts a number into buf, producing at least (digits) chars
//  (filling with fillChar), and returns the number of characters generated.
static int
numtostr(
  char* buf,
  unsigned ui,
  int digits,
  char fillChar)
{
  char mybuf[64];
  char* mp = mybuf + sizeof(mybuf) - 1;
  *mp = '\0';
  do {
    int digit = int(ui % 10);
    ui /= 10;
    *--mp = ("0123456789")[digit];
    --digits;
  } while (ui != 0);
  while (digits-- > 0)	// fill with zeros
    *--mp = fillChar;
  strcpy(buf, mp);
  return (sizeof(mybuf) - 1) - (mp - mybuf);
}

// formatComponent() generates the specified component into the buffer
// and returns the number of bytes generated.
int
RWLocaleSnapshot::formatComponent(
  char* buf,
  const tm* tmbuf,
  int match,
  const RWZone& zone) const
{
  const char* tfd = timeFmtDigits;
  switch(match) {
   case YEAR4: return numtostr(buf, tmbuf->tm_year+1900, tfd[match], '0');
   case YEAR2: return numtostr(buf, tmbuf->tm_year, tfd[match], '0');
   case JULIANDAY: return numtostr(buf, tmbuf->tm_yday+1, tfd[match], '0');
   case SECS:  return numtostr(buf, tmbuf->tm_sec, tfd[match], '0');
   case MINS:  return numtostr(buf, tmbuf->tm_min, tfd[match], '0');
   case HOUR12: {
      int hour = tmbuf->tm_hour;
      hour = (hour == 0 ? 12 : (hour > 12 ? hour - 12 : hour));
      return numtostr(buf, hour, 2, '0');
    }
   case HOUR24:  return numtostr(buf, tmbuf->tm_hour, tfd[match], '0');
   case MONTH:
    if (tmbuf->tm_mon < 0 || tmbuf->tm_mon > 11) return *buf = 0;
    strcpy(buf, (const char*) monthNames_[tmbuf->tm_mon]);
    return monthNames_[tmbuf->tm_mon].length();
   case MONABB:
    if (tmbuf->tm_mon < 0 || tmbuf->tm_mon > 11) return *buf = 0;
    strcpy(buf, (const char*) monthAbbrs_[tmbuf->tm_mon]);
    return monthAbbrs_[tmbuf->tm_mon].length();
   case WEEKDAY:
    if (tmbuf->tm_wday < 0 || tmbuf->tm_wday > 6) return *buf = 0;
    strcpy(buf, (const char*) weekDays_[tmbuf->tm_wday]);
    return weekDays_[tmbuf->tm_wday].length();
   case WEEKABB:
    if (tmbuf->tm_wday < 0 || tmbuf->tm_wday > 6) return *buf = 0;
    strcpy(buf, (const char*) weekAbbs_[tmbuf->tm_wday]);
    return weekAbbs_[tmbuf->tm_wday].length();
   case MONTHNUM: return numtostr(buf, tmbuf->tm_mon + 1, tfd[match], '0');
   case AMPM: {
      const RWCString * ampm = (tmbuf->tm_hour < 12) ? &am_ : &pm_;
      strcpy(buf, ampm->data());
      return ampm->length();
    }
   case MONTHDAY:   return numtostr(buf, tmbuf->tm_mday, tfd[match], '0');
   case MONTHDAYSP: return numtostr(buf, tmbuf->tm_mday, tfd[match], ' ');
   case TIMEZONE: {
      if (tmbuf->tm_isdst & ~1) { *buf = 0; return 0; }
      RWCString tz;
      if (tmbuf->tm_isdst) tz = zone.altZoneName();
      else                 tz = zone.timeZoneName();
      strcpy(buf, (const char*) tz);
      return tz.length();
    }
   case SUNWEEK: {
      // with provisional thanks to P.J. Plauger
      int week = (tmbuf->tm_yday - tmbuf->tm_wday + 7) / 7;
      return numtostr(buf, week, tfd[match], '0');
    }
   case MONWEEK: {
      int week = (tmbuf->tm_yday - ((tmbuf->tm_wday + 6) % 7) + 7) / 7;
      return numtostr(buf, week, tfd[match], '0');
    }
   case WEEKDAYNUM: return numtostr(buf, tmbuf->tm_wday, tfd[match], '0');

   default: break;
  }
  *buf = 0;
  return 0;
}

int
RWLocaleSnapshot::fillFormat(
  char* buf,
  const char* format,
  RWTimeFmtList* list,
  const struct tm* tmbuf,
  const RWZone& zone) const
{
  const char* cp = format;
  char* bp = buf;
  for (; list; list = list->next) {
    while (cp < format + list->offset)
      *bp++ = *cp++;
    int count = formatComponent(bp, tmbuf, (int)list->match, zone);
    bp += count;
  }
  while ((*bp++ = *cp++) != '\0')
    ;
  return bp - buf - 1;
}

// components that must be generated are translated here from the
// strftime() character selector to the RWTimeComp enum value.
int
RWLocaleSnapshot::fmt(
  char* buf,
  const struct tm* tmbuf,
  char format,
  const RWZone& zone) const
{
  RWTimeComp t;
  switch (format) {
   case 'c': return fillFormat(buf, (const char*) dateTimeFmt_,
                               dateTimeFmtList_, tmbuf, zone);
#if !defined(RW_NO_STRFTIME_CAPC)
   case 'C': return fillFormat(buf, (const char*) dateLongFmt_,
                               dateLongFmtList_, tmbuf, zone);
#endif
   case 'd': t = MONTHDAY; break;
   case 'H': t = HOUR24; break;
    
   case 'I': t = HOUR12; break;
   case 'j': t = JULIANDAY; break;
   case 'm': t = MONTHNUM; break;
   case 'M': t = MINS; break;
   case 'S': t = SECS; break;
   case 'U': t = SUNWEEK; break;
   case 'w': t = WEEKDAYNUM; break;
   case 'W': t = MONWEEK; break;
   case 'x': return fillFormat(buf, (const char*)dateFmt_,
    			       dateFmtList_, tmbuf, zone);
   case 'X': return fillFormat(buf, (const char*)timeFmt_,
			       timeFmtList_, tmbuf, zone);
   case 'y': t = YEAR2; break;
   case 'Y': t = YEAR4; break;
   case 'Z': t = TIMEZONE; break;

   default:  return (int)(*buf = 0);
  }
  return formatComponent(buf, tmbuf, (int) t, zone);
}

// public conversions:

RWCString
RWLocaleSnapshot::asString(
  long i) const
{
  char buf[256];
  char* fmtstr = buf + sizeof(buf) - 1;
  *fmtstr = '\0';
  fmt(&fmtstr, i);
  return RWCString(fmtstr, int(sizeof(buf) - (fmtstr - buf) - 1));
}

RWCString
RWLocaleSnapshot::asString(
  unsigned long ui) const
{
  char buf[256];
  char* fmtstr = buf + sizeof(buf) - 1;
  *fmtstr = '\0';
  fmt(&fmtstr, ui);
  return RWCString(fmtstr, int(sizeof(buf) - (fmtstr - buf) - 1));
}

RWCString
RWLocaleSnapshot::asString(
  double f,
  int precision,
  int showpoint) const
{
  char buf[256];
  char* fmtstr = buf + sizeof(buf) - 1;
  *fmtstr = '\0';
  fmt(&fmtstr, f, precision, showpoint,
    decimal_point_, thousands_sep_, grouping_);
  return RWCString(fmtstr, int(sizeof(buf) - (fmtstr - buf) - 1));
}

// values already in RWCString form are returned directly from here;
//  others are constructed as needed.
RWCString
RWLocaleSnapshot::asString(
  const tm* tmbuf,
  char format,
  const RWZone& zone) const
{
  // These cases are handled up-front to allow sharing by the RWCString
  // instances involved.
  switch (format) {
  case 'a': case 'A': {
      if (tmbuf->tm_wday < 0 || tmbuf->tm_wday > 6) break;
      if (format == 'a') return weekAbbs_[tmbuf->tm_wday];
      else               return weekDays_[tmbuf->tm_wday];
    }
  case 'B': case 'b': {
      if (tmbuf->tm_mon < 0 || tmbuf->tm_mon > 11) break;
      if (format == 'b') return monthAbbrs_[tmbuf->tm_mon];
      else               return monthNames_[tmbuf->tm_mon];
    }
  case 'p': {
      if (tmbuf->tm_hour < 12) return am_; // If statement to stop cfront loop
      else                     return pm_;
    }
  case 'Z': {
      if (tmbuf->tm_isdst & ~1) break;  // DST unknown
      if (tmbuf->tm_isdst) return zone.altZoneName();
      else                 return zone.timeZoneName();
    }
  default: {
      char buf[256];
      fmt(buf, tmbuf, format, zone);
      return RWCString(buf);
    }
  }
  return RWCString();
}

size_t
RWLocaleSnapshot::fmt(
  char** bufp,
  double cents,
  const RWCString& curr_sym,
  const char* format,
  const RWCString& sign,
  int sep_by_space,
  int frac_digits,
  double divisor) const
{
  char* buf = *bufp;
  size_t count = 0;
  if (*format == '(') {
    *--buf = ')'; ++count;
  }
  for (int i = 4; --i >= 0;) {
    switch (format[i]) {
    case ' ':  if (!sep_by_space) break; // else fall through
    case '(':
      *--buf = format[i]; ++count;
      break;
    case '-': {
      size_t length = sign.length();
      if (length) {
	buf -= length; count += sign.mbLength();
	memcpy(buf, sign.data(), length);
      }} break;
    case '$': {
      size_t length = curr_sym.length();
      if (length) {
	buf -= length; count += curr_sym.mbLength();
	memcpy(buf, curr_sym.data(), length);
      }} break;
    case 'm':
      count += fmt(&buf, cents/divisor, frac_digits, FALSE,
	mon_decimal_point_, mon_thousands_sep_, mon_grouping_);
      break;
    }
  }
  *bufp = buf;
  return count;
}


RWCString
RWLocaleSnapshot::moneyAsString(
  double cents, 
#ifndef RW_GLOBAL_ENUMS
  RWLocale::
#endif
            CurrSymbol sym) const
{
  char buf[128];
  char* bp = buf + sizeof(buf) - 1;
  *bp = 0;
  RWBoolean less = FALSE;
  if (cents < 0) { less = TRUE; cents = -cents; }
  if (sym == INTL) {
    fmt(&bp, cents, int_curr_symbol_, "$-m ", RWCString(less ? "-" : ""),
      FALSE, int_frac_digits_, intlDivisor_);
  } else {
    RWCString curr_sym;
    if (sym == LOCAL) curr_sym = currency_symbol_;
    if (less)
      fmt(&bp, cents, curr_sym,	negativeMoneyFormat_, negative_sign_,
	n_sep_by_space_, frac_digits_, localDivisor_);
    else
      fmt(&bp, cents, curr_sym, positiveMoneyFormat_, positive_sign_,
	p_sep_by_space_, frac_digits_, localDivisor_);
  }
  return RWCString(bp, (buf + sizeof(buf) - 1) - bp);
}

// the following code converts strings to dates or times.

static int
xnum(
  const char** spp,
  int* dest,
  int maxdigits,
  const char* monthptr,
  size_t monthlen)
{
  const char* sp = *spp;
  while (1) {
    if (sp == monthptr) sp += monthlen;
    if (*sp == '\0') return FALSE;
    if (isdigit((unsigned char)*sp)) break;
    ++sp;
  }
  for (*dest = 0; isdigit((unsigned char)*sp) && --maxdigits >= 0; ++sp)
    *dest = (*dest * 10) + (*sp - '0');
  *spp = sp;
  return TRUE;
}

RWBoolean
RWLocaleSnapshot::stringToDate(
  const RWCString& str,
  struct tm* tmbuf) const
{
#ifndef __GLOCK__
  enum DatePart { YR, MN, DY };  // indices into date
#else  /* glockenspiel enum, static initializer & typedef are broken */
#  define YR 0
#  define MN 1
#  define DY 2
#  define DatePart int
#endif
  int date[3];	// date parts: year, month, day

  // Note: someday this bit of code should use wide characters.
  size_t monthlen = 0;
  const char* monthptr = 0;
  if (monthNameHasDigits_)
    date[MN] = 0;
  else {
    for (date[MN] = 11; date[MN] >= 0; --date[MN]) {
      size_t monthpos;
      monthpos = str.index(monthNames_[date[MN]], 0, RWCString::ignoreCase);
      if (monthpos != RW_NPOS) {
	monthptr = str.data() + monthpos;
	monthlen = monthNames_[date[MN]].length();
	break;
      }
      monthpos = str.index(monthAbbrs_[date[MN]], 0, RWCString::ignoreCase);
      if (monthpos != RW_NPOS) {
	monthptr = str.data() + monthpos;
	monthlen = monthAbbrs_[date[MN]].length();
	break;
      }
    }
    ++date[MN];  // now month is in [1..12] if found, or 0 otherwise.
  }
    // extract components in the right order:
  static const DatePart orders[2][4][3] = {
    { { DY, MN, YR }, { MN, DY, YR },  { YR, DY, MN }, { YR, MN, DY } },
    { { DY, YR, YR }, { DY, YR, YR },  { YR, DY, DY }, { YR, DY, DY } }
  };
  static const int digits[3] = { 4, 2, 2 };  // YR, MN, DY
  const char* sp = str;
  int gotmonth = (date[MN] != 0);
  const DatePart* order = orders[gotmonth][dateOrder_];
  if (!xnum(&sp, &date[order[0]], digits[order[0]], monthptr, monthlen) ||
      !xnum(&sp, &date[order[1]], digits[order[1]], monthptr, monthlen) ||
      !(gotmonth ||
        xnum(&sp, &date[order[2]], digits[order[2]], monthptr, monthlen)))
    return FALSE;
  if (date[YR] < 100) date[YR] += 1900;
  
  // check for spurious numbers
  { int dummy; if (xnum(&sp, &dummy, 1, monthptr, monthlen)) return FALSE; }
  // check for bad date
  if (date[MN] > 12 || date[MN] == 0 || date[DY] == 0)
    return FALSE;
  if (date[DY] > (int)RWDate::daysInMonth[date[MN] - 1]) {
    if (!(date[MN] == 2 && date[DY] == 29 && RWDate::leapYear(date[YR])))
      return FALSE;
  }
  // install results
  tmbuf->tm_year = date[YR] - 1900;
  tmbuf->tm_mon = date[MN] - 1;
  tmbuf->tm_mday = date[DY];
  return TRUE;
}

RWBoolean
RWLocaleSnapshot::stringToTime(
  const RWCString& str,
  struct tm* tmbuf) const
{
  const char* sp = str;
  int hour, minute, second = 0, dummy;
  if (xnum(&sp, &hour, 2, 0, 0) && xnum(&sp, &minute, 2, 0, 0)) {
    xnum(&sp, &second, 2, 0, 0);	// seconds optional
      // check for spurious extra numbers:
    if (xnum(&sp, &dummy, 1, 0, 0)) return FALSE;
    if (second > 59 || minute > 59 || hour > 23) return FALSE;
    RWBoolean isam = (str.index(am_, 0, RWCString::ignoreCase) != RW_NPOS);
    RWBoolean ispm = (str.index(pm_, 0, RWCString::ignoreCase) != RW_NPOS);
    if (hour < 12 && hour > 0) {
      if (ispm && !isam) hour += 12;
    } else if (hour == 12) {
      if (isam && !ispm) hour = 0;
    } else {
      if ((am_.length() && isam) || (pm_.length() && ispm))
	return FALSE;  // AM/PM not allowed for obviously 24-hour clock times.
    }
    tmbuf->tm_hour = hour;
    tmbuf->tm_min = minute;
    tmbuf->tm_sec = second;
    return TRUE;
  }
  return FALSE;
}

// return a pointer after any spaces:
static const char*
skipSpaces(const char* sp)
 { while (isspace((unsigned char)*sp)) ++sp; return sp; }

// match a substring against a pattern, advancing over it if present:
static RWBoolean
matchSub(const char*& sp, const RWCString& pattern)
{
  size_t patlen = pattern.length();
  if (patlen && memcmp(sp, pattern.data(), patlen) == 0) {
    sp += patlen;
    return TRUE;
  }
  return FALSE;
}

// check digit group separator spacing:
static RWBoolean
checkGrouping(
  const char*& sp,
  int digitsSeen,
  int numSeps,
  const char* seps,
  const char* grouping,
  const char* separator)
{
  if (numSeps == 0) return TRUE;
  if (digitsSeen == seps[numSeps-1]) {
    if (*separator != ' ') return FALSE;
    --sp;
    if (--numSeps == 0) return TRUE;
  }
  if (*grouping == 0)  return FALSE;
  do {
    digitsSeen -= (*grouping & 0xF);  // "& 0xF": protect against Sun bug
    if (digitsSeen != seps[--numSeps]) return FALSE;
    if (grouping[1]) ++grouping;
  } while (numSeps);
  if (digitsSeen > (*grouping & 0xF)) return FALSE;
  return TRUE;
}

RWBoolean
RWLocaleSnapshot::stringToNum(
  const RWCString& str,
  double* fp) const
{
  char buf[256];
  if (str.length() >= sizeof(buf)) return FALSE;  // sanity check
  const char* sp = skipSpaces(str.data());  // allow whitespace in front
  char* bp = buf; *bp = '\0';
  if (*sp == '-' || *sp == '+') {
    *bp++ = *sp++;
    sp = skipSpaces(sp);  // allow whitespace between sign and number
  }
  char seps[sizeof(buf)/2];
  int numSeps = 0;
  int digitsSeen = 0;
  while (isdigit((unsigned char)*sp))  {
    do {
      *bp++ = *sp++; ++digitsSeen;
    } while (isdigit((unsigned char)*sp));
    if (matchSub(sp, thousands_sep_))
      seps[numSeps++] = digitsSeen;
  }
  if (!checkGrouping(sp, digitsSeen, numSeps, seps, grouping_, thousands_sep_))
    return FALSE;
  // do fractional part:
  if (matchSub(sp, decimal_point_)) {
    // require at least one digit before or after the decimal point
    if (!(isdigit((unsigned char)*sp) ||
	  (bp > buf && isdigit((unsigned char)bp[-1]))))
      return FALSE;
    // substitute the current locale\'s radix separator
    for (const char* dp = localeconv()->decimal_point; *dp; *bp++ = *dp++) {}
    while (isdigit((unsigned char)*sp))
      *bp++ = *sp++;
  } else if (bp == buf || !isdigit((unsigned char)bp[-1]))
    return FALSE;   // no int part, no frac part either.

  if (*sp == 'E' || *sp == 'e') {
    *bp++ = *sp++;
    if (*sp == '-' || *sp == '+')
      *bp++ = *sp++;
    if (!isdigit((unsigned char)*sp)) return FALSE;
    do { *bp++ = *sp++; } while (isdigit((unsigned char)*sp));
  }
  sp = skipSpaces(sp);  // allow whitespace at end
  if (*sp) return FALSE;
  // get the library to do the conversion, for exact compatibility
  *bp = '\0';
  double result = strtod(buf, &bp);
  if (*bp) return FALSE;  // strtod did not read the whole string
  *fp = result;
  return TRUE;
}

RWBoolean
RWLocaleSnapshot::stringToNum(
  const RWCString& str,
  long* lp) const
{
  const char* sp = skipSpaces(str.data());
  int negative = 0;
  if (*sp == '-' || *sp == '+') {
    if (*sp == '-') negative = 1;
    sp = skipSpaces(sp+1);   // allow whitespace between sign and number
  }
  unsigned long sum = 0;
  char seps[32];
  int numSeps = 0;
  char digitsSeen = 0;
  while (isdigit((unsigned char)*sp)) {
    do {
      unsigned long tmp = sum * 10 + (*sp++ - '0');
      if (tmp < sum) return FALSE; // overflow;
      sum = tmp;
      ++digitsSeen;
    } while (isdigit((unsigned char)*sp));
    if (matchSub(sp, thousands_sep_))
      seps[numSeps++] = digitsSeen;
  } 
  if (digitsSeen == 0) return FALSE;  // no digits, no number.
  if (!checkGrouping(sp, digitsSeen, numSeps, seps, grouping_, thousands_sep_))
      return FALSE;
  sp = skipSpaces(sp);  // allow whitespace at end
  if (*sp) return FALSE;
  if (negative) sum = -sum;
  if (((long)sum < 0) != negative) return FALSE;  // overflow
  *lp = sum;
  return TRUE;
}

static const char* checkSign(
  const char*& sp,
  const RWCString& pos,
  const RWCString& neg,
  char* sign)
{
  if (*sign || !*sp || isdigit((unsigned char)*sp))
    return sp;
  if (*sp == '-' || *sp == '+') {
    *sign = *sp++;
  } else if (matchSub(sp, neg)) {
    *sign = '-';
  } else if (matchSub(sp, pos)) {
    *sign = '+';
  } else
    return sp;
  return skipSpaces(sp);
}

static const char*
checkCurrsym(
  const char* sp,
  const RWCString cs,
  char* sym)
{
  if (!*sym && *sp && !isdigit((unsigned char)*sp) && matchSub(sp, cs)) {
    *sym = '$';
    return skipSpaces(sp);
  }
  return sp;
}

static const char*
checkBalance(
  const char* sp,
  const RWCString& dec_pt,
  const RWCString& thou_sep,
  const RWCString& grouping,
  int frac_digits,
  double* balance, char* num)
{
  if (*num || !(isdigit((unsigned char)*sp) || *sp == *dec_pt.data()))
    return sp;
  const char* oldsp = sp;
  double bal = 0.0;
  int digits_seen = 0, frac_seen = 0;
  char seps[10];
  int numSeps = 0;
  *num = '!';
  while (isdigit((unsigned char)*sp)) {
    do {
      bal = bal * 10.0 + (*sp - '0');
      ++sp; ++digits_seen;
    } while (isdigit((unsigned char)*sp));
    if (matchSub(sp, thou_sep)) {
      if (numSeps == sizeof(seps)) return oldsp;  // number too big
      seps[numSeps++] = digits_seen;
    }
  }
  if (!checkGrouping(sp, digits_seen, numSeps, seps, grouping, thou_sep))
    return oldsp;
  if (frac_digits && matchSub(sp, dec_pt)) {
    ++frac_seen;
    while (isdigit((unsigned char)*sp)) {
      bal = bal * 10.0 + (*sp - '0');
      ++sp; ++digits_seen; ++frac_seen;
    }
  }
  if (!frac_seen) {
    if (!digits_seen) {
      *num = 0; // no number here!
      return oldsp;
    } else while (frac_digits--)
      bal *= 10.0;
  } else if (frac_seen - 1 != frac_digits)
    return oldsp;
  *num = '.'; *balance = bal;
  return skipSpaces(sp);
}

RWBoolean
RWLocaleSnapshot::stringToMoney(
  const RWCString& s,
  double* cents,
#ifndef RW_GLOBAL_ENUMS
  RWLocale::
#endif
            CurrSymbol currency) const
{
  const char* sp = skipSpaces(s.data());  // eat leading space
  char sign = 0, sym = 0, num = 0;
  double balance = 0.0;
  if (*sp == '(')
    { sign = *sp; sp = skipSpaces(sp+1); }  // only allowed at beginnning
  
  // set up to assume INTL format
  int count = 2;
  int frac_digits = int_frac_digits_;
  RWCString pos_sign, neg_sign, curr_sym(int_curr_symbol_);
  
  switch (currency) {
  case LOCAL:  // oops, asked for local format
    pos_sign = positive_sign_;
    neg_sign = negative_sign_;
    curr_sym = currency_symbol_;
    frac_digits = frac_digits_;
    count = 0;
    // fall through:
  case INTL:
    while (*sp && count < 3 && !(sign && sym && num)) {
      sp = checkCurrsym(sp, curr_sym, &sym);
      sp = checkSign(sp, pos_sign, neg_sign, &sign);
      sp = checkBalance(sp, mon_decimal_point_, mon_thousands_sep_,
	mon_grouping_, frac_digits, &balance, &num);
      ++count;
    } break;
  case NONE:  // must specify so we know what frac_digit value to use.
    return FALSE;
  }
  if (sign == '(') {
    if (*sp != ')') return FALSE;
    sp = skipSpaces(sp+1);
    sign = '-';
  }
  if (*sp || !num || num == '!') return FALSE;
  *cents = (sign == '-') ? -balance : balance;
  return TRUE;
}

int
RWLocaleSnapshot::monthIndex(const RWCString& s) const
{
  RWCString ss = s.strip(RWCString::leading);
  int i = 11;

  do {
    if      (ss.compareTo(monthAbbrs_[i].strip(RWCString::leading), 
			  RWCString::ignoreCase) == 0)
      return (i + 1);

    else if (ss.compareTo(monthNames_[i].strip(RWCString::leading), 
			  RWCString::ignoreCase) == 0)
      return (i + 1); 
  } while (--i >= 0);

  return 0;
}

int
RWLocaleSnapshot::weekdayIndex(const RWCString& s) const
{
  int i = 6;
  do {
    if (s.compareTo(weekAbbs_[i], RWCString::ignoreCase) == 0 ||
        s.compareTo(weekDays_[i], RWCString::ignoreCase) == 0)
      return ((i + 6) % 7 + 1);
  } while (--i >= 0);
  return 0;
}

#else  /* RW_NO_LOCALE */
int rw_locale_linker_dummy;  // for picky linkers
#endif /* RW_NO_LOCALE */
