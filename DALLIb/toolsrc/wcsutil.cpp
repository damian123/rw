/*
 * Definitions of wide-character string functions that were not supplied 
 *   by the compiler vendor.
 *
 * $Id: wcsutil.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 * Voice: (503) 754-3010	FAX: (503) 757-6650
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
 * $Log: wcsutil.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:08:07  vriezen
 * Move all files to 6.1
 *
 * Revision 1.6  1993/09/13  06:21:26  myersn
 * add wcscspn definition
 *
 * Revision 1.5  1993/09/12  21:10:21  keffer
 * All wide character utility functions are now declared in rwwchar.h
 *
 * Revision 1.4  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.3  1993/08/21  22:14:06  keffer
 * Fixed RCS ident.
 *
 *
 */


#include "rw/defs.h"
#include "rwwchar.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: wcsutil.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");


#if defined(RW_SUPPLY_WSTR)

extern "C" wchar_t *wcschr(const wchar_t *wstr, wchar_t wc)
{
  while (*wstr != wc)
    if (!*wstr++) return NULL;  // not found

  return (wchar_t*) wstr;
}


extern "C" wchar_t *wcsrchr(const wchar_t *wstr, wchar_t wc)
{
  const wchar_t* p = NULL;

  do {
    if (*wstr == wc)
      p = wstr;
  } while (*wstr++);
  
  return (wchar_t*) p;
}

  
extern "C" wchar_t *wcspbrk(const wchar_t *wstr, const wchar_t *wcset)
{
  while (*wstr && !wcschr(wcset, *wstr)) ++wstr;
  
  return *wstr ? (wchar_t*) wstr : NULL;
}


extern "C" size_t wcsspn(const wchar_t *wstr, const wchar_t *wcset)
{
  for (size_t i=0; *wstr; ++i, ++wstr) 
    if (!wcschr(wcset, *wstr)) break;

  return i;
}

extern "C" size_t wcscspn(const wchar_t *wstr, const wchar_t *wcset)
{
  for (size_t i=0; *wstr; ++i, ++wstr) 
    if (wcschr(wcset, *wstr)) break;

  return i;
}


#else  /* RW_SUPPLY_WSTR */

// This is to quiet fussy libraries if nothing gets compiled:
int rwDummy_wcsutil_cpp;

#endif /* RW_SUPPLY_WSTR */
