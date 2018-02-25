/*
 * RWCString / RWWString conversions
 *
 * $Id: strngcv.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
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
 * $Log: strngcv.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/06/17  07:30:07  jims
 * Change pref_ to data_ for easier debugging
 *
 * Revision 6.1  1994/04/15  19:07:57  vriezen
 * Move all files to 6.1
 *
 * Revision 1.28  1993/11/09  09:37:36  jims
 * Port to ObjectStore
 *
 * Revision 1.27  1993/09/14  00:06:27  randall
 * include header file stdlib.h and removed mention of RWWiden and RWWidenAscii
 *
 * Revision 1.26  1993/09/12  21:10:21  keffer
 * All wide character utility functions are now declared in rwwchar.h
 *
 * Revision 1.25  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.24  1993/09/10  01:54:32  keffer
 * Corrected length error in initMB() for non-null terminated strings.
 *
 * Revision 1.23  1993/09/09  02:49:57  keffer
 * Added constructors allowing MB to wide character conversion
 * of RWCStrings
 *
 * Revision 1.22  1993/08/21  21:08:14  keffer
 * Added conversion constructors taking enum multiByte_ and ascii_.
 * Deprecated old "RWWiden" interface.
 *
 * Revision 1.21  1993/07/29  04:07:02  keffer
 * Now uses new RWWString architecture.
 *
 * Revision 1.20  1993/07/28  22:43:21  keffer
 * Changed to reflect new RWCString interface.
 *
 * Revision 1.19  1993/07/20  00:14:42  myersn
 * move functions to cstring.o so wide string support not pulled in.
 *
 * Revision 1.18  1993/06/13  21:26:46  jims
 * Remove "::" from wcstombs to avoid problem when implemented as macro
 *
 * Revision 1.17  1993/05/14  21:43:03  myersn
 * make RWCString::mbLength() report the number of wide characters
 * found, up to the first (embedded) null.
 *
 * Revision 1.16  1993/05/14  00:14:27  myersn
 * define RWWString(const RWWidener&) constructor, replacing the
 * RWWString(..., widenFrom) constructors.
 *
 * Revision 1.15  1993/05/01  18:21:18  keffer
 * Ported to IBM xlC compiler.
 *
 * Revision 1.14  1993/04/20  19:43:36  myersn
 * fix SunOS 4 incompatibility (RW_NO_WSTR)
 *
 * Revision 1.13  1993/04/13  20:39:18  myersn
 * remove use of euclen(), add RWCString::isAscii().
 *
 * Revision 1.12  1993/03/24  03:46:44  myersn
 * change decl of euclen (again!).
 *
 * Revision 1.11  1993/02/17  04:41:39  myersn
 * eliminate bogus RWWidenAscii class, replace with enum arg to constructor.
 *
 * Revision 1.10  1993/02/17  03:12:30  keffer
 * Changed const notation to follow style guide
 *
 * Revision 1.9  1993/02/15  23:32:59  myersn
 * add extern "C" to declaration of euclen().
 *
 * Revision 1.8  1993/02/15  23:17:26  myersn
 * declare euclen() to be extern "C".
 *
 * Revision 1.7  1993/02/15  02:46:08  myersn
 * replaced RWWString(RWMBString) with RWWString(const RWWidenAscii&).
 *
 * Revision 1.6  1993/02/06  03:01:36  myersn
 * simplify ascii-widening constructors.
 *
 * Revision 1.5  1993/02/05  23:19:06  myersn
 * delete widen() function -- RWWString constructor sufficient.
 *
 * Revision 1.4  1993/02/04  01:13:19  myersn
 * add new RWWString(RWMBString) constructor.
 *
 * Revision 1.3  1993/01/29  01:12:40  myersn
 * add RWWString fromMultiByte(const RWCString&) global function.
 *
 * Revision 1.2  1993/01/27  21:13:27  myersn
 * add conversions from ascii->wide, wide->ascii, and wide->multibyte
 *
 * Revision 1.1  1992/11/17  21:31:11  keffer
 * Initial revision
 * 
 */

#include "rw/defs.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: strngcv.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                  Wide string to multibyte string conversions             //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#ifndef RW_NO_WSTR		/* WS supporting compilers only */

#include "rw/cstring.h"
#include "rw/wstring.h"
#include "rwwchar.h"		/* wide character facilities */
#include <stdlib.h>		/* MB_CUR_MAX, mblen(), wcstombs(), etc.. */

RWWString::RWWString(const char* cs, multiByte_)
{
  initMB(cs, strlen(cs));
}

RWWString::RWWString(const char* cs, size_t N, multiByte_)
{
  initMB(cs, N);
}

RWWString::RWWString(const RWCString& cstr, multiByte_)
{
  initMB(cstr.data(), cstr.length());
}

RWWString::RWWString(const RWCString& cstr, ascii_)
{
  size_t N = cstr.length();
  const char* cs = cstr.data();
  data_ = RWWStringRef::getRep(N, N)->data();
  for (size_t i = 0; i < N; ++i)
    data_[i] = (wchar_t)(unsigned char) cs[i];  // prevent sign-extend
  RWPOSTCONDITION(cstr.length()==length());
}

// Protected function:
void
RWWString::initMB(const char* cs, size_t N)
{
  RWPRECONDITION(cs!=rwnil);
  wchar_t buffer[64];  // avoid new()ing a buffer in most cases.
  wchar_t* buf = (N>=64) ? new wchar_t[N + 1] : buffer;
  size_t len = mbstowcs(buf, cs, N);
  if (len == size_t(-1)) len = 0;
  data_ = RWWStringRef::getRep(len, len)->data();
  memcpy(data_, buf, len*sizeof(wchar_t));
  if (buf != buffer)
    RWVECTOR_DELETE(N+1) buf;
}

RWCString
RWWString::toMultiByte() const
{
  size_t N = length()*MB_CUR_MAX + 1;
  char* tmp = new char[N];
  size_t L = wcstombs(tmp, data(), N);
  RWASSERT(L == (size_t) -1 || L <= N-1);
  if (L == (size_t) -1)
    tmp[0] = '\0';	// Null string
  RWCString str(tmp);
  delete tmp;
  return str;
}

RWBoolean
RWWString::isAscii() const
{
  size_t len = length();
  const wchar_t* wp = data();
  for (size_t i = 0; i < len; ++i)
    if (wp[i] & ~0x7f)
      return 0;
  return 1;
}

RWCString
RWWString::toAscii() const
{
  size_t len = length();
  RWCString result(' ', len);
  const wchar_t* wp = data();
  for (size_t i = 0; i < len; ++i)
    result(i) = (char)wp[i];
  return result;
}

#endif /* RW_NO_WSTR */
