#ifndef __RWWCHAR_H
#define __RWWCHAR_H

/*
 * Standardizes wide character facilities of various vendors.
 *
 * $Id: rwwchar.h,v 6.3 1994/07/12 21:14:53 vriezen Exp $
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
 *
 ***************************************************************************
 *
 * $Log: rwwchar.h,v $
 * Revision 6.3  1994/07/12  21:14:53  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/06/02  18:57:49  foote
 * Port to Symantec 7.0 on Macintosh
 *
 * Revision 6.1  1994/04/15  19:07:47  vriezen
 * Move all files to 6.1
 *
 * Revision 1.8  1993/09/12  21:23:56  keffer
 * Added a typedef for wint_t.
 *
 * Revision 1.7  1993/09/12  21:10:21  keffer
 * All wide character utility functions are now declared in rwwchar.h
 *
 * Revision 1.6  1993/08/09  03:41:48  myersn
 * add #define for wcscspn
 *
 * Revision 1.5  1993/05/25  18:32:23  keffer
 * Ported to NT
 *
 * Revision 1.4  1993/05/24  19:12:22  keffer
 * Ported to xlC compiler.
 *
 * Revision 1.3  1993/05/01  18:22:59  keffer
 * Maps Sun names to X/Open names.
 *
 * 
 */


#ifndef __RWTOOLDEFS_H__
# include "rw/tooldefs.h"
#endif

STARTWRAP
#include <stdlib.h>		/* Looking for wcstombs() & MB_CUR_MAX */
#include <string.h>
ENDWRAP


// Do we have to supply definitions?
#ifdef RW_SUPPLY_WSTR


//
// This part used only if we need to supply definitions
//


// For these, the definitions are supplied elsewhere:
extern "C" wchar_t* rwwcschr  (const wchar_t *wstr, wchar_t wc);
extern "C" wchar_t* rwwcsrchr (const wchar_t *wstr, wchar_t wc);
extern "C" wchar_t* rwwcspbrk (const wchar_t *wstr, const wchar_t *wcset);
extern "C" size_t   rwwcsspn  (const wchar_t *wstr, const wchar_t *wcset);
extern "C" size_t   rwwcscspn (const wchar_t *wstr, const wchar_t *wcset);

#define wcschr  rwwcschr
#define wcsrchr rwwcsrchr
#define wcspbrk rwwcspbrk
#define wcsspn  rwwcsspn
#define wcscspn rwwcscspn


#ifdef __WIN32__

// The following implementations rely on the WIN32 API.

typedef wchar_t wint_t;		// Required for BCC4

#include <windows.h>

inline wchar_t rwtowupper(wchar_t wc)
{
  return LOWORD(CharUpperW((wchar_t*)MAKELONG(wc, 0)));
}

inline wchar_t rwtowlower(wchar_t wc)
{
  return LOWORD(CharLowerW((wchar_t*) MAKELONG(wc, 0)));
}

inline int rwisw_aux(wint_t wc, USHORT ctype)
{
  WORD result;
  BOOL retval;

  // set bits in result according to type of wc:
  retval = GetStringTypeW(CT_CTYPE1, &wc, 1, &result);

  RWASSERT(retval);
  return result & ctype;  // return true if relevant bit is set
}

inline int rwiswspace (wint_t wc) { return rwisw_aux(wc, C1_SPACE); }
inline int rwiswalpha (wint_t wc) { return rwisw_aux(wc, C1_ALPHA); }
inline int rwiswlower (wint_t wc) { return rwisw_aux(wc, C1_LOWER); }
inline int rwiswupper (wint_t wc) { return rwisw_aux(wc, C1_UPPER); }
inline int rwiswdigit (wint_t wc) { return rwisw_aux(wc, C1_DIGIT); }
inline int rwiswxdigit(wint_t wc) { return rwisw_aux(wc, C1_XDIGIT);}

inline size_t rwwcslen(const wchar_t *wstr)
{
  return lstrlenW(wstr);
}

inline wchar_t *rwwcscpy(wchar_t *dest, const wchar_t *src)
{
  return lstrcpyW(dest, src);
} 	

inline int rwwcscoll(const wchar_t *wstr1, const wchar_t *wstr2)
{
  return lstrcmpW(wstr1, wstr2);
}	



#define iswalpha  rwiswalpha
#define iswdigit  rwiswdigit
#define iswlower  rwiswlower
#define iswspace  rwiswspace
#define iswupper  rwiswupper
#define iswxdigit rwiswxdigit
#define towlower  rwtowlower
#define towupper  rwtowupper
#define wcscoll   rwwcscoll
#define wcscpy    rwwcscpy
#define wcslen    rwwcslen

#else

Error!  Unable to supply definitions for wide character functions!

#endif /* __WIN32__ */




#else  /* !RW_SUPPLY_WSTR */


//
// This section is used if compiler-supplied definitions are available.
// Now where are we going to get them?
//


#if defined(_XOPEN_SOURCE) || !defined(sun)

// X/OPEN include file names:
STARTWRAP
# include <string.h>
# include <ctype.h>
ENDWRAP

#else

// For Sun SVR4:
STARTWRAP
# include <widec.h>
# include <wctype.h>
ENDWRAP

// Map the X/OPEN names to the sun SVR4 names:
# define wcschr  wschr
# define wcscoll wscoll
# define wcscpy  wscpy
# define wcslen  wslen
# define wcsncpy wsncpy
# define wcspbrk wspbrk
# define wcspbrk wspbrk
# define wcsrchr wsrchr
# define wcsspn  wsspn
# define wcscspn wscspn
# define wcsxfrm wsxfrm

#endif


#endif /* RW_SUPPLY_WSTR */

#endif	/* __RWWCHAR_H */
