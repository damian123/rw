
/*
 * Definitions for class RWCRegexp --- Regular Expressions.
 *
 * $Id: regexp.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: regexp.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:07:29  vriezen
 * Move all files to 6.1
 *
 * Revision 2.20  1994/01/04  23:47:07  jims
 * In getPattern: force return value of rwmakepat into legal RWCRegexp::statVal
 *
 * Revision 2.19  1993/11/09  09:01:23  jims
 * Port to ObjectStore
 *
 * Revision 2.18  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.17  1993/07/29  06:07:39  myersn
 * tolerate nulls & 8-bit chars better, also escape sequences.
 *
 * Revision 2.16  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.15  1993/05/19  00:00:05  keffer
 * Constructor takes const char*
 *
 * Revision 2.14  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.13  1993/05/14  00:13:55  myersn
 * add RWCRegexp(const RWCString&) constructor.
 *
 * Revision 2.12  1993/05/06  02:38:56  myersn
 * change matchs() and makepat() to rwmatchs(), rwmakepat(), and install
 * new semantics for rwmatchs().
 *
 * Revision 2.11  1993/03/17  20:37:19  keffer
 * patternType -> RWPatternType
 *
 * Revision 2.10  1993/02/14  05:18:57  myersn
 * make indices size_t in place of int.
 *
 * Revision 2.9  1993/02/11  03:17:33  keffer
 * Consistent 'constness' done in pointinter arithmetic to satisfy GLOCK.
 *
 * Revision 2.8  1993/02/08  19:42:50  keffer
 * Workaround for MS C7 const bug.
 *
 * Revision 2.7  1993/02/06  03:49:51  keffer
 * Changed name _defs.h to defmisc.h to avoid collision with Borland header file
 *
 * Revision 2.6  1993/01/29  20:12:46  myersn
 * extend regexp to be 8-bit clean.
 *
 * Revision 2.5  1993/01/25  18:17:44  keffer
 * RW_NO_CONST_OVERLOADS->RW_NO_CONST_OVERLOAD
 *
 * Revision 2.4  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.3  1992/11/17  21:35:21  keffer
 * Now includes _defs.h to get extern declarations.
 * Now called RWCRegexp.
 *
 * Revision 2.2  1992/11/05  23:35:31  myersn
 * remove extern "C" decl for match.o functions
 *
 * Revision 2.1  1992/10/31  23:46:03  keffer
 * Check for BSD changed to RW_NON_ANSI_HEADERS
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.7   29 May 1992 09:46:52   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.6   27 May 1992 18:09:06   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.5   09 Mar 1992 11:54:52   KEFFER
 * Debugging now happens with RWDEBUG instead of DEBUG
 * 
 *    Rev 1.4   04 Mar 1992 10:26:54   KEFFER
 * Changed RWString to RWCString
 * 
 *    Rev 1.3   05 Nov 1991 14:06:30   keffer
 * Removed RWXXXXErr macros --- now done by error package
 * 
 *    Rev 1.2   29 Oct 1991 13:58:24   keffer
 * Changed len from static to auto.
 * Added definitions for RWCString::operator()(const RWRegexp&);
 * 
 *    Rev 1.1   28 Oct 1991 09:24:22   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 08:39:34   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/regexp.h"
#include "rw/cstring.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"
#include "defmisc.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: regexp.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

STARTWRAP
#if defined(RW_NON_ANSI_HEADERS)
#  include <memory.h>		/* Looking for memcpy() */
#else
#  include <string.h>
#endif
ENDWRAP

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

const unsigned RWCRegexp::maxpat_=128;

RWCRegexp::RWCRegexp(const char* re)
{
  genPattern(re);
}

RWCRegexp::RWCRegexp(const RWCString& re)
{
  genPattern(re.data());
}

RWCRegexp::RWCRegexp(const RWCRegexp& r)
{
  copyPattern(r);
}
  
RWCRegexp::~RWCRegexp()
{
  RWVECTOR_DELETE(maxpat_) thePattern_;
}

RWCRegexp&
RWCRegexp::operator=(const RWCRegexp& r)
{
  if(this != &r){
    RWVECTOR_DELETE(maxpat_) thePattern_;
    copyPattern(r);
  }
  return *this;
}

RWCRegexp&
RWCRegexp::operator=(const char* str)
{
  RWVECTOR_DELETE(maxpat_) thePattern_;
  genPattern(str);
  return *this;
}

RWCRegexp&
RWCRegexp::operator=(const RWCString& str)
{
  RWVECTOR_DELETE(maxpat_) thePattern_;
  genPattern(str.data());
  return *this;
}

void
RWCRegexp::genPattern(const char* str)
{
  thePattern_ = new RWPatternType[maxpat_];
  int error = rwmakepat(str, thePattern_, maxpat_);
  stat_ = (error < 3) ? (statVal) error : TOOLONG;
}

void
RWCRegexp::copyPattern(const RWCRegexp& r)
{
  thePattern_ = new RWPatternType[maxpat_];
  memcpy(thePattern_, r.thePattern_, maxpat_ * sizeof(RWPatternType));
  stat_ = r.stat_;
}

size_t
RWCRegexp::index(const RWCString& string, size_t* len, size_t i) const
{
  if(stat_!=OK)
    RWTHROW( RWInternalErr(RWMessage(RWTOOL_BADRE) ));

  const char* startp;
  const char* s = string.data();
  size_t slen = string.length();
  if (slen < i) return RW_NPOS;
  const char* endp = ::rwmatchs(s+i, slen-i, thePattern_, &startp);
  if (endp) {
    *len = endp - startp;
    return startp - s;
  } else {
    *len = 0;
    return RW_NPOS;
  }
}

#ifdef RW_GLOBAL_ENUMS
statVal
#else
RWCRegexp::statVal
#endif
RWCRegexp::status()
{
  statVal temp = stat_;
  stat_ = OK;
  return temp;
}

/********************************************************
 *							*
 *	RWCString member functions,			*
 *	put here so the linker will include		*
 *	them only if regular expressions are used.	*
 *							*
 ********************************************************/

size_t
RWCString::index(const RWCRegexp& r, size_t start) const
{
  size_t len;
  return r.index(*this, &len, start); // len not used
}

size_t
RWCString::index(const RWCRegexp& r, size_t* extent, size_t start) const
{
  return r.index(*this, extent, start);
}

RWCSubString
RWCString::operator()(const RWCRegexp& r, size_t start)
{
  size_t len;
  size_t begin = index(r, &len, start);
  return RWCSubString(*this, begin, len);
}

RWCSubString
RWCString::operator()(const RWCRegexp& r)
{
  return (*this)(r,0);
}

#ifndef RW_NO_CONST_OVERLOAD
const RWCSubString
RWCString::operator()(const RWCRegexp& r, size_t start) const
{
  size_t len;
  size_t begin = index(r, &len, start);
  return RWCSubString(*this, begin, len);
}

const RWCSubString
RWCString::operator()(const RWCRegexp& r) const
{
  return (*this)(r,0);
}
#endif
