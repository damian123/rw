
/*
 * Definitions for RWWString wide character string class.
 *
 * $Id: wstring.cpp,v 6.7 1994/07/18 20:51:00 jims Exp $
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
 * $Log: wstring.cpp,v $
 * Revision 6.7  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.6  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.5  1994/06/23  19:01:06  vriezen
 * Bug# 277. Changed (char) ctor to initChar, now called by inline ctor.
 *
 * Revision 6.4  1994/06/23  01:27:07  myersn
 * match changes in cstring, re: ctor from char, construction or
 * copy from empty string.
 *
 * Revision 6.3  1994/06/17  07:28:13  jims
 * Change pref_ to data_ for easier debugging
 *
 * Revision 6.2  1994/06/15  20:58:46  myersn
 * disable constructor from wchar_t, which causes ambiguities and
 * conversion problems.
 *
 * Revision 6.1  1994/04/15  19:07:27  vriezen
 * Move all files to 6.1
 *
 * Revision 1.63  1994/04/12  18:13:04  vriezen
 * Add caseCompare parameter to subString function.
 *
 * Revision 1.62  1993/11/22  11:44:17  jims
 * Rename unlink to "unLink" to avoid #defines in some of the DOS/WIN compilers
 *
 * Revision 1.61  1993/11/19  22:04:33  myersn
 * use new RWReference::setRefCount instead of plugging member directly.
 *
 * Revision 1.60  1993/11/17  01:59:11  myersn
 * make the static MT lock safe.
 *
 * Revision 1.59  1993/11/16  08:19:37  myersn
 * fix use of RW_NO_CONST_OVERLOAD[S]
 *
 * Revision 1.58  1993/11/15  00:42:27  keffer
 * Corrected return value from RWWString::operator=(const RWWString&) for
 * ObjectStore version.
 *
 * Revision 1.57  1993/11/15  00:36:21  keffer
 * Introduced m.f. clobber()
 *
 * Revision 1.56  1993/11/14  22:19:25  keffer
 * Introduced m.f. unlink()
 *
 * Revision 1.55  1993/11/13  22:54:25  keffer
 * Added const version of strip()
 *
 * Revision 1.54  1993/11/09  10:40:21  jims
 * Port to ObjectStore
 *
 * Revision 1.53  1993/11/02  02:37:15  keffer
 * Fixed clone() bug
 *
 * Revision 1.52  1993/09/14  16:34:22  keffer
 * Changed C++ '//' style commend from a preprocessor directive.
 *
 * Revision 1.51  1993/09/14  00:06:27  randall
 * include header file stdlib.h and removed mention of RWWiden and RWWidenAscii
 *
 * Revision 1.50  1993/09/12  21:10:21  keffer
 * All wide character utility functions are now declared in rwwchar.h
 *
 * Revision 1.49  1993/09/11  23:41:45  keffer
 * Corrected garbled comments.
 *
 * Revision 1.48  1993/09/10  20:03:37  keffer
 * Repositioned RW_RCSID macro.
 *
 * Revision 1.47  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.46  1993/09/08  20:20:47  randall
 * corrected retrieving of data for second argument in RWWSubString operator==
 *
 * Revision 1.45  1993/09/08  19:08:15  randall
 * removed RWPOSTCONDITION from constructor RWWString(wchar_t, size_t)
 *
 * Revision 1.44  1993/09/02  23:49:39  randall
 * changed ret->refs_ 's value from 1 to 0
 *
 * Revision 1.43  1993/09/01  03:40:34  myersn
 * use new static init semantics of RWReference for default RW?String ctor.
 *
 * Revision 1.42  1993/08/26  00:24:12  myersn
 * fix up hashing, optimize case-folding hash.
 *
 * Revision 1.41  1993/08/21  21:08:14  keffer
 * Added conversion constructors taking enum multiByte_ and ascii_.
 * Deprecated old "RWWiden" interface.
 *
 * Revision 1.40  1993/08/06  21:34:43  myersn
 * minor cleanup
 *
 * Revision 1.39  1993/08/06  20:41:45  keffer
 * Assignment or construction from a nil const char* is
 * now a violated precondition.
 *
 * Revision 1.38  1993/08/06  01:59:08  keffer
 * Changed how a null string is handled to something more DLL friendly.
 *
 * Revision 1.37  1993/08/05  11:47:26  jims
 * Distinguish between WIN16 DLLs and WIN32 DLLs by checking
 * for __WIN16__
 *
 * Revision 1.36  1993/08/04  19:56:29  keffer
 * Substrings now reference their cstring by a pointer rather than a reference
 * to work around a Symantec bug.
 *
 * Revision 1.35  1993/07/30  02:09:21  jims
 * Guard against tolower being a macro
 *
 * Revision 1.34  1993/07/29  04:07:02  keffer
 * Now uses new RWWString architecture.
 *
 * Revision 1.33  1993/06/13  21:31:51  jims
 * Surround strXform with #ifndef
 *
 * Revision 1.32  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 1.31  1993/05/29  18:48:24  keffer
 * Renamed or added a dummy variable in case nothing gets compiled
 *
 * Revision 1.30  1993/05/26  00:31:55  keffer
 * Ported to Microsoft C++ under NT
 *
 * Revision 1.29  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 1.28  1993/05/14  21:42:14  myersn
 * allow constructor to fill with nulls.
 *
 * Revision 1.27  1993/05/13  23:53:28  myersn
 * fix some 8-bit uncleanliness and null intolerance.
 *
 * Revision 1.26  1993/05/05  20:08:07  johnb
 * rwmin arguments cast to unsigned - required by xlC
 *
 * Revision 1.25  1993/05/01  18:21:55  keffer
 * Now uses X/Open wide string function names.
 * "rwwchar.h" used to map Sun names to these.
 *
 * Revision 1.24  1993/04/12  16:35:16  keffer
 * Indexing now uses size_t
 *
 * Revision 1.23  1993/04/12  12:17:20  jims
 * Now uses RWMutex class for locking
 *
 * Revision 1.22  1993/03/25  06:00:19  myersn
 * add embedNulls argument to ctor, append() and prepend().
 *
 * Revision 1.21  1993/03/20  22:47:39  keffer
 * All lengths are now unsigned instead of int.
 *
 * Revision 1.20  1993/02/22  22:36:21  myersn
 * clean up
 *
 * Revision 1.16  1993/02/17  03:12:30  keffer
 * Changed const notation to follow style guide
 *
 * Revision 1.15  1993/02/17  01:42:29  myersn
 * fix assertElement.
 *
 * Revision 1.14  1993/02/17  00:38:51  myersn
 * fix prepend(), not to terminate string at end of prepended chars.
 *
 * Revision 1.13  1993/02/16  19:11:39  myersn
 * install #ifndef RW_NO_WSTR protection.
 *
 * Revision 1.11  1993/02/10  21:43:49  myersn
 * use defmisc.h in place of _defs.h.
 *
 * Revision 1.10  1993/02/06  03:01:36  myersn
 * simplify ascii-widening constructors, remove mbtowc calls.
 *
 * Revision 1.9  1993/02/03  20:02:09  myersn
 * fix clone() for MT case: must always check result of removeReference.
 *
 * Revision 1.8  1993/01/29  20:07:31  myersn
 * make MT-safe via different interface to RWReference base class
 *
 * Revision 1.6  1992/12/02  00:24:14  myersn
 * fixed static initialization of the null string
 *
 * Revision 1.5  1992/12/01  22:14:50  myersn
 * changed "sensitive, insensitive" to "exact, ignoreCase" for clarity.
 *
 * Revision 1.4  1992/11/26  20:13:42  myersn
 * Used RWCS_T::value() instead of private member.
 *
 * Revision 1.3  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 1.2  1992/11/16  04:37:43  keffer
 * operator()(int) is now inline
 *
 * Revision 1.1  1992/11/16  03:22:21  keffer
 * Initial revision
 *
 * 
 */

#include "rw/compiler.h"
#ifndef RW_NO_WSTR

#include "rw/wstring.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"
#include "defmisc.h"
#include "rwwchar.h"
STARTWRAP
#include <stdlib.h>
#if defined(RW_NON_ANSI_HEADERS)
#  include <memory.h>		/* Looking for memcpy() */
#else
#  include <string.h>
#endif
ENDWRAP

#ifdef RW_MULTI_THREAD
# include "rw/mutex.h"
  static RWMutex rwwstringLock(RWMutex::staticCtor); 
# define MULTITHREAD_LOCK (rwwstringLock)
#else
# define MULTITHREAD_LOCK /* nothing */
#endif

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: wstring.cpp,v $ $Revision: 6.7 $ $Date: 1994/07/18 20:51:00 $");

// Defined below:
extern "C" int wscasecmp (const wchar_t*, const wchar_t*);
extern "C" int wsncasecmp(const wchar_t*, const wchar_t*, size_t N);

// Amount to shift hash values to avoid clustering

const unsigned RW_HASH_SHIFT = 5;

// This is the global null string representation, shared among all
// empty strings.  The space for it is in "nullref" which the
// loader will set to zero:
static long nullref[(sizeof(RWWStringRef)+sizeof(wchar_t))/sizeof(long) + 1];
static RWWStringRef* const nullStringRef = (RWWStringRef*)nullref;

/*
 ******************************************************************
 *
 * In what follows, npts_ is the length of the underlying representation
 * vector.  Hence, the capacity for a null terminated string held in this 
 * vector is npts_-1.  The variable nwchar_ts_ is the length of the held
 * string, excluding the terminating null.
 *
 * The algorithms make no assumptions about whether internal strings
 * hold embedded nulls.  However, they do assume that any string
 * passed in as an argument that does not have a length count is null
 * terminated and therefore has no embedded nulls.
 *
 * The internal string is always null terminated.
 *
 ******************************************************************
 *
 *  This class uses a number of protected and private member functions
 *  to do memory management.  Here are their semantics:
 *
 *  RWWString::cow();
 *    Insure that self is a distinct copy.  Preserve previous
 *    contents.
 *
 *  RWWString::cow(size_t nc);
 *    Insure that self is a distinct copy with capacity of at
 *    least nc.  Preserve previous contents.
 *
 *  RWWString::clobber(size_t nc);
 *    Insure that the StringRef is unshared and has a 
 *    capacity of at least nc.  No need to preserve contents.
 *
 *  RWWString::clone();
 *    Make self a distinct copy.  Preserve previous contents.
 *
 *  RWWString::clone(size_t);
 *    Make self a distinct copy with capacity of at least nc.
 *    Preserve previous contents.
 *
 ******************************************************************
 */



//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             RWWStringRef                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


/* static */ RWWStringRef*
RWWStringRef::getRep(size_t capacity, size_t nchar)
{
  if ((capacity | nchar) == 0) {
    nullStringRef->addReference(MULTITHREAD_LOCK);
    return nullStringRef;
  }
  RWWStringRef* ret = (RWWStringRef*) new char[(capacity+1)*sizeof(wchar_t)
					       + sizeof(RWWStringRef)];
  ret->capacity_ = capacity;
  ret->setRefCount(1);
  ret->data()[ret->nchars_ = nchar] = 0; // Terminating null
  return ret;
}

inline void RWWStringRef::unLink()
{
  if (removeReference(MULTITHREAD_LOCK) == 0)
    RWVECTOR_DELETE((capacity_+1)*sizeof(wchar_t)+sizeof(RWWStringRef)) (char*)this;
}

// String collation
int
RWWStringRef::collate(const wchar_t* wstr) const
{
  return wcscoll(data(), wstr);
}

// Find first occurrence of a character c
size_t
RWWStringRef::first(wchar_t c) const
{
  const wchar_t* f = wcschr(data(), c);
  return f ? f - data(): RW_NPOS;
}

// Find first occurrence of a character in wchr:
size_t
RWWStringRef::first(const wchar_t* cs) const
{
  RWPRECONDITION2(cs!=rwnil, "RWWStringRef::first(const wchar_t* cs)const: nil pointer");
  const wchar_t* f = wcspbrk(data(), cs);
  return f ? f - data() : RW_NPOS;
}

inline static void mash(unsigned& hash, unsigned chars)
{
  hash = (chars ^
       ((hash << RW_HASH_SHIFT) |
        (hash >> (RWBITSPERBYTE*sizeof(unsigned) - RW_HASH_SHIFT))));
}

/*
 * Return a case-sensitive hash value.
 */
unsigned
RWWStringRef::hash() const
{
  register unsigned hv       = (unsigned)length(); // Mix in the string length.
  register unsigned i        = length()*sizeof(wchar_t)/sizeof(unsigned);
  register const unsigned* p = (const unsigned*)data();

  while (i--)
    mash(hv, *p++);			// XOR in the characters.

  // XOR in any remaining characters:
  if ((i = length()*sizeof(wchar_t)%sizeof(unsigned)) != 0)
  {
    register const wchar_t* c = (const wchar_t*)p;
    while (i--)
      mash(hv, (unsigned) *c++);
  }

  return hv;
}

/*
 * Return a case-insensitive hash value.
 */
unsigned
RWWStringRef::hashFoldCase() const
{
  register unsigned hv = (unsigned)length();    // Mix in the string length.
  register unsigned i  = hv;
  register const wchar_t* p = (const wchar_t*) data();
  while (i--) {
    mash(hv, unsigned(towupper(*p)));
    ++p;
  }
  return hv;
}

// Find last occurrence of wide character c
size_t
RWWStringRef::last(wchar_t c) const
{
  const wchar_t* f = wcsrchr(data(), c);
  return f ? f - data() : RW_NPOS;
}



//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              RWWString                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


RWWString::RWWString()
{
  // Make sure the null string rep has not been clobbered.
  RWASSERT(nullStringRef->capacity()==0);
  RWASSERT(nullStringRef->length()==0);
  RWASSERT(*nullStringRef->data()==0);
  data_ = nullStringRef->data();
  pref()->addReference(MULTITHREAD_LOCK);
  RWPOSTCONDITION(length()==0);
}

RWWString::RWWString(RWSize_T ic)
{
  data_ = RWWStringRef::getRep(ic.value(), 0)->data();
  RWPOSTCONDITION(length()==0);
}

RWWString::RWWString(const wchar_t* cs)
{
  RWPRECONDITION(cs!=rwnil);
  size_t N = wcslen(cs);
  data_ = RWWStringRef::getRep(N, N)->data();
  memcpy(data_, cs, N*sizeof(wchar_t));
}

RWWString::RWWString(const wchar_t* cs, size_t N)
{
  RWPRECONDITION(cs!=rwnil);
  data_ = RWWStringRef::getRep(N, N)->data();
  memcpy(data_, cs, N*sizeof(wchar_t));
  RWPOSTCONDITION(length()==N);
}

RWWString::RWWString(wchar_t c)
{
  data_ = RWWStringRef::getRep(getInitialCapacity(), 1)->data();
  data_[0] = c;
  RWPOSTCONDITION(length()==1);
}

void RWWString::initChar(char c)
{
  data_ = RWWStringRef::getRep(getInitialCapacity(), 1)->data();
  data_[0] = (wchar_t)(unsigned char) c;
  RWPOSTCONDITION(length()==1);
}

RWWString::RWWString(wchar_t c, size_t N)
{
  data_ = RWWStringRef::getRep(N, N)->data();
  while (N--) data_[N] = c;
}

// Widen from a (known) ASCII character string
RWWString::RWWString(const char* cs, ascii_)
{
  RWPRECONDITION(cs!=rwnil);
  size_t N = strlen(cs);
  data_ = RWWStringRef::getRep(N, N)->data();
  for (size_t i = 0; i < N; ++i)
    data_[i] = (wchar_t)(unsigned char) cs[i];  // prevent sign-extend
}

// Widen from a (known) ASCII character string of length N
RWWString::RWWString(const char* cs, size_t N, ascii_)
{
  RWPRECONDITION(cs!=rwnil);
  data_ = RWWStringRef::getRep(N, N)->data();
  for (size_t i = 0; i < N; ++i)
    data_[i] = (wchar_t)(unsigned char) cs[i];  // prevent sign-extend
  RWPOSTCONDITION(length()==N);
}

RWWString::RWWString(const RWWSubString& substr)
{
  size_t len = substr.isNull() ? 0 : substr.length();
  data_ = RWWStringRef::getRep(adjustCapacity(len), len)->data();
  memcpy(data_, substr.data(), len*sizeof(wchar_t));
}

#ifdef RW_MULTI_THREAD
RWWString::RWWString(const RWWString& S)
{ data_ = S.data_; pref()->addReference(MULTITHREAD_LOCK); }
#endif

RWWString::~RWWString()
{
  pref()->unLink();
}

RWWString&
RWWString::operator=(const wchar_t* cs)
{
  RWPRECONDITION(cs!=rwnil);
  if (!*cs) {
    if (pref()->references() == 1) {
      pref()->nchars_ = 0;
    } else {
      pref()->unLink();
      nullStringRef->addReference(MULTITHREAD_LOCK);
      data_ = nullStringRef->data();
    }
    return *this;
  }
  return replace(0, length(), cs, wcslen(cs));
}

// Assignment:  
RWWString&
RWWString::operator=(const RWWString& str)
{
  str.pref()->addReference(MULTITHREAD_LOCK);
  pref()->unLink();
  data_ = str.data_;
  return *this;
}


/********************** Member Functions *************************/

RWWString&
RWWString::append(wchar_t c, size_t rep)
{
  size_t tot;
  cow(tot = length() + rep);
  wchar_t* p = data_ + length();
  while (rep--)
    *p++ = c;

  data_[pref()->nchars_ = tot] = (wchar_t)0;

  return *this;
}

RWWString&
RWWString::append(const wchar_t* cs)
{
  return replace(length(), 0, cs, wcslen(cs));
}

RWWString&
RWWString::operator+=(const wchar_t* cs)
{
  return append(cs, wcslen(cs));
}

// Change the string capacity, returning the new capacity
size_t
RWWString::capacity(size_t nc)
{
  if (nc > length() && nc != capacity())
    clone(nc);

  RWPOSTCONDITION(capacity() >= length());
  return capacity();
}

// String comparisons
int
RWWString::compareTo(const wchar_t* cs2, caseCompare cmp) const
{
  RWPRECONDITION2(cs2!=rwnil, "RWWString::compareTo(const wchar_t* cs2, caseCompare cmp) const: nil pointer");
  const wchar_t* cs1 = data();
  size_t len = length();
  size_t i = 0;
  if (cmp == exact) {
    for (; cs2[i]; ++i) {
      if (i == len) return -1;
      if (cs1[i] != cs2[i]) return ((cs1[i] > cs2[i]) ? 1 : -1);
    }
  } else {                  // ignore case
    for (; cs2[i]; ++i) {
      if (i == len) return -1;
      wchar_t c1 = towlower(cs1[i]);
      wchar_t c2 = towlower(cs2[i]);
      if (c1 != c2) return ((c1 > c2)? 1 : -1);
    }
  }
  return (i < len) ? 1 : 0;
}

int
RWWString::compareTo(const RWWString& str, caseCompare cmp) const
{
  const wchar_t* s1 = data();
  const wchar_t* s2 = str.data();
  size_t len = str.length();
  if (length() < len) len = length();
  if (cmp == exact) {
    int result = memcmp(s1, s2, len*sizeof(wchar_t));
    if (result != 0) return result;
  } else {
    size_t i = 0;
    for (; i < len; ++i) {
      wchar_t c1 = towlower(s1[i]);
      wchar_t c2 = towlower(s2[i]);
      if (c1 != c2) return ((c1 > c2)? 1 : -1);
    }
  }
  // strings are equal up to the length of the shorter one.
  if (length() == str.length()) return 0;
  return (length() > str.length())? 1 : -1;
}


RWBoolean
RWWString::contains(const wchar_t* s, caseCompare cmp) const
{
  return index(s, wcslen(s), (size_t)0, cmp) != RW_NPOS;
}


RWWString
RWWString::copy() const
{
  RWWString temp(*this);	// Has increased reference count
  temp.clone();			// Distinct copy
  return temp;
}

unsigned
RWWString::hash(caseCompare cmp) const
{
  return (cmp==exact) ? pref()->hash() : pref()->hashFoldCase();
}


static int
rwMemiEqual(const wchar_t* p, const wchar_t* q, size_t N)
{
  while (N--)
  {
    if (towlower(*p) != towlower(*q))
      return FALSE;
    p++; q++; 
  }
  return TRUE;
}

// Pattern Matching:
size_t
RWWString::index(const wchar_t* pattern, // Pattern to search for
		 size_t plen,		 // Length of pattern
		 size_t startIndex,	 // Starting index from which to start
		 caseCompare cmp) const	 // What type of case-comparison
{
  RWPRECONDITION2(pattern!=rwnil,"RWWString::index(const wchar_t*, size_t, caseCompare) const: nil pointer");
  
  size_t slen = length();
  if (slen < startIndex + plen) return RW_NPOS;
  if (plen == 0) return startIndex;
  slen -= startIndex + plen;
  const wchar_t* sp = data() + startIndex;
  if (cmp == exact) {
    wchar_t first = *pattern;
    for (size_t i = 0; i <= slen; ++i)
      if (sp[i] == first && memcmp(sp+i+1, pattern+1, (plen-1)*sizeof(wchar_t)) == 0)
        return i + startIndex;
  } else {
    wint_t first = towlower(*pattern);
    for (size_t i = 0; i <= slen; ++i)
      if (towlower(sp[i]) == first &&
	  rwMemiEqual(sp+i+1, pattern+1, plen-1))
	return i + startIndex;
  }
  return RW_NPOS;

}

size_t
RWWString::index(const wchar_t* s, size_t i, caseCompare cmp) const
{
  return index(s, wcslen(s), i, cmp);
}

RWWString&
RWWString::insert(size_t pos, const wchar_t* cs)
{
  return replace(pos, 0, cs, wcslen(cs));
}

// Prepend characters to self:
RWWString&
RWWString::prepend(wchar_t c, size_t rep)
{
  size_t tot = length() + rep;	// Final string length

  // Check for shared representation or insufficient capacity:
  if ( pref()->references() > 1 || capacity() < tot )
  {
    RWWStringRef* temp = RWWStringRef::getRep(adjustCapacity(tot), tot);
    memcpy(temp->data()+rep, data(), length()*sizeof(wchar_t));
    pref()->unLink();
    data_ = temp->data();
  }
  else
  {
    memmove(data_+rep, data(), length()*sizeof(wchar_t));
    data_[pref()->nchars_ = tot] = '\0';
  }

  wchar_t* p = data_;
  while (rep--)
    *p++ = c;

  return *this;
}

RWWString&
RWWString::prepend(const wchar_t* cs)
{
  return replace(0, 0, cs, wcslen(cs));
}


// Remove at most n1 characters from self beginning at pos,
// and replace them with the first n2 characters of cs.
RWWString&
RWWString::replace(size_t pos, size_t n1, const wchar_t* cs, size_t n2)
{
  n1 = rwmin(n1, length()-pos);
  if (!cs) n2 = 0;

  size_t tot = length()-n1+n2;	// Final string length
  size_t rem = length()-n1-pos;	// Length of remnant at end of string

  // Check for shared representation, insufficient capacity,
  // excess waste, or overlapping copy:
  if ( pref()->references() > 1 
       || capacity() < tot
       || capacity() - tot > getMaxWaste()
       || (cs && (cs >= data() && cs < data()+length())) )
  {
    RWWStringRef* temp = RWWStringRef::getRep(adjustCapacity(tot), tot);
    if (pos) memcpy(temp->data(), data(), pos*sizeof(wchar_t));
    if (n2 ) memcpy(temp->data()+pos, cs, n2*sizeof(wchar_t));
    if (rem) memcpy(temp->data()+pos+n2, data()+pos+n1, rem*sizeof(wchar_t));
    pref()->unLink();
    data_ = temp->data();
  }
  else
  {
    if (rem) memmove(data_+pos+n2, data()+pos+n1, rem*sizeof(wchar_t));
    if (n2 ) memmove(data_+pos   , cs, n2*sizeof(wchar_t));
    data_[pref()->nchars_ = tot] = 0;	// Add terminating null
  }

  return *this;
}


RWWString&
RWWString::replace(size_t pos, size_t n, const wchar_t* cs)
{
  return replace(pos, n, cs, wcslen(cs));
}


// Truncate or add blanks as necessary
void
RWWString::resize(size_t N)
{
  if (N < length())
    remove(N);			// Shrank; truncate the string
  else
    append((wchar_t)' ', N-length());	// Grew or stayed the same

}

// Return a substring of self stripped at beginning and/or end

RWWSubString RWWString::strip(

#ifndef RW_GLOBAL_ENUMS
  RWWString::
#endif
             stripType st,
  wchar_t c)
{
  size_t start = 0;		// Index of first character
  size_t end = length();	// One beyond last character
  const wchar_t* direct = data();	// Avoid a dereference w dumb compiler

  RWASSERT((int)st != 0);
  if (st & leading)
    while (start < end && direct[start] == c)
      ++start;
  if (st & trailing)
    while (start < end && direct[end-1] == c)
      --end;
  if (end == start) start = end = RW_NPOS;  // make the null substring
  return RWWSubString(*this, start, end-start);
}


const RWWSubString RWWString::strip(

#ifndef RW_GLOBAL_ENUMS
  RWWString::
#endif
             stripType st,
  wchar_t c) const
{
  // Just use the "non-const" version, adjusting the return type:
  return ((RWWString*)this)->strip(st,c);
}

// Change self to lower-case
void
RWWString::toLower()
{
  cow();
  register size_t N = length();
  register wchar_t* p = data_;
  while ( N-- ) { *p = towlower(*p); p++;}
}

// Change self to upper case
void
RWWString::toUpper()
{
  cow();
  register size_t N = length();
  register wchar_t* p = data_;
  while ( N-- ) { *p = towupper(*p); p++;}
}

wchar_t&
RWWString::operator[](size_t i)
{
   assertElement(i); 
   cow();
   return data_[i];
}

// Check to make a string index is in range
void
RWWString::assertElement(size_t i) const
{
  if ( i==RW_NPOS || i>=length() )
    RWTHROW( RWBoundsErr( RWMessage( RWTOOL_INDEX,
				     (unsigned)i,
				     (unsigned)length()-1) ) );
}

/********************** Protected functions ***********************/

// Special constructor to initialize with the concatenation of a1 and a2:
RWWString::RWWString(const wchar_t* a1, size_t N1, const wchar_t* a2, size_t N2)
{
  if (!a1) N1=0;
  if (!a2) N2=0;
  size_t tot = N1+N2;
  data_ = RWWStringRef::getRep(adjustCapacity(tot), tot)->data();
  memcpy(data_,    a1, N1*sizeof(wchar_t));
  memcpy(data_+N1, a2, N2*sizeof(wchar_t));
  RWPOSTCONDITION(length() == N1+N2);
}

// Calculate a nice capacity greater than or equal to nc
/* static */ size_t
RWWString::adjustCapacity(size_t nc)
{
  size_t ic = getInitialCapacity();
  if (nc<=ic) return ic;
  size_t rs = getResizeIncrement();
  return (nc - ic + rs - 1) / rs * rs + ic;
}

void RWWString::clobber(size_t nc)
{
  if (pref()->references() > 1 || capacity() < nc)
  {
    pref()->unLink();
    data_ = RWWStringRef::getRep(nc, 0)->data();
  }
  else
    data_[pref()->nchars_ = 0] = 0;
}

// Make self a distinct copy; preserve previous contents
void
RWWString::clone()
{
  RWWStringRef* temp = RWWStringRef::getRep(length(), length());
  memcpy(temp->data(), data(), length()*sizeof(wchar_t));
  pref()->unLink();
  data_ = temp->data();

  RWPOSTCONDITION((pref()->references()==1) || (isNull()));
}

// Make self a distinct copy with capacity of at least nc;
// preserve previous contents
void
RWWString::clone(size_t nc)
{
  size_t len = length();
  if (len > nc) len = nc;
  RWWStringRef* temp = RWWStringRef::getRep(nc, len);
  memcpy(temp->data(), data(), len*sizeof(wchar_t));
  pref()->unLink();
  data_ = temp->data();

  RWPOSTCONDITION(pref()->references() == 1);
}



/****************** Related global functions ***********************/

RWBoolean rwexport
operator==(const RWWString& s1, const wchar_t* s2)
{
  const wchar_t* data = s1.data();
  size_t len = s1.length();
  for (size_t i = 0; s2[i]; ++i)
    if (data[i] != s2[i] || i == len) return FALSE;
  return (i == len);
}

// Return a lower-case version of str:
RWWString rwexport
toLower(const RWWString& str)
{
  register size_t N = str.length();
  RWWString temp((wchar_t)0, N);
  register const wchar_t* uc = str.data();
  register       wchar_t* lc = (wchar_t*)temp.data();
  // Guard against towlower() being a macro:
  while( N-- ) { *lc++ = towlower(*uc); uc++; }
  return temp;
}

// Return an upper-case version of str:
RWWString rwexport
toUpper(const RWWString& str)
{
  register size_t N = str.length();
  RWWString temp((wchar_t)0, N);
  register const wchar_t* uc = str.data();
  register       wchar_t* lc = (wchar_t*)temp.data();
  // Guard against towupper() being a macro:
  while( N-- ) { *lc++ = towupper(*uc); uc++; }
  return temp;
}

RWWString rwexport
operator+(const RWWString& s, const wchar_t* cs)
{
  // Use the special concatenation constructor:
  return RWWString(s.data(), s.length(), cs, wcslen(cs));
}            

RWWString rwexport
operator+(const wchar_t* cs, const RWWString& s)
{
  // Use the special concatenation constructor:
  return RWWString(cs, wcslen(cs), s.data(), s.length());
}

RWWString rwexport
operator+(const RWWString& s1, const RWWString& s2)
{
  // Use the special concatenation constructor:
  return RWWString(s1.data(), s1.length(), s2.data(), s2.length());
}

/******************** Static Member Functions **********************/

#if !defined(__DLL__) || !defined(__WIN16__)

/*
 * Code for the statically linked library.  For a Win16-DLL version,
 * the instance data must be retrieved from the instance manager.
 */

// Static member variable initialization:
size_t		RWWString::initialCapac     = 15;
size_t		RWWString::resizeInc        = 16;
size_t		RWWString::freeboard        = 15;

size_t
RWWString::initialCapacity(size_t ic)
{
  size_t ret = initialCapac;
  initialCapac = ic;
  return ret;
}

size_t
RWWString::resizeIncrement(size_t ri)
{
  size_t ret = resizeInc;
  resizeInc = ri;
  return ret;
}

size_t
RWWString::maxWaste(size_t mw)
{  
  size_t ret = freeboard;
  freeboard = mw;
  return ret;
}

#endif


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             RWWSubString                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/*
 * A zero lengthed substring is legal.  It can start
 * at any character.  It is considered to be "pointing"
 * to just before the character.
 *
 * A "null" substring is a zero lengthed substring that
 * starts with the nonsense index RW_NPOS.  It can
 * be detected with the member function isNull().
 */

// Private constructor 
RWWSubString::RWWSubString(const RWWString & str, size_t start, size_t nextent)
: str_((RWWString*)&str),
  begin_(start),
  extent_(nextent)
{
#ifdef RWDEBUG
  size_t N = str.length();

  // Allow zero lengthed and null substrings:
  if ((start==RW_NPOS && nextent!=0) || (start!=RW_NPOS && start+nextent>N))
    subStringError(N, start, nextent);
#endif
}

// Sub-string operator
RWWSubString 
RWWString::operator()(size_t start, size_t len) 
{
  return RWWSubString(*this, start, len);
}

/*
 * Returns a substring matching "pattern", or the null substring 
 * if there is no such match.  It would be nice if this could be yet another 
 * overloaded version of operator(), but this would result in a type
 * conversion ambiguity with operator(size_t, size_t). 
 */
RWWSubString
RWWString::subString(const wchar_t* pattern, 
		     size_t startIndex, caseCompare cmp)
{
  RWPRECONDITION2(pattern!=rwnil,"RWWString::subString(const wchar_t*, size_t, caseCompare): null pointer");
  size_t len = wcslen(pattern);
  size_t i = index(pattern, len, startIndex,  cmp);
  return RWWSubString(*this, i, i == RW_NPOS ? 0 : len);
}

wchar_t&
RWWSubString::operator[](size_t i)
{
   assertElement(i); 
   str_->cow();
   return str_->data_[begin_+i];
}

wchar_t&
RWWSubString::operator()(size_t i)
{ 
#ifdef RWBOUNDS_CHECK    
   assertElement(i);
#endif
   str_->cow();
   return str_->data_[begin_+i];
}

#ifndef RW_NO_CONST_OVERLOAD
const RWWSubString
RWWString::operator()(size_t start, size_t len) const
{
  return RWWSubString(*this, start, len);
}

const RWWSubString
RWWString::subString(const wchar_t* pattern, size_t startIndex, caseCompare cmp) const
  {
  RWPRECONDITION2(pattern!=rwnil,"RWWString::subString(const wchar_t*, size_t, caseCompare) const: null pointer");
  size_t len = wcslen(pattern);
  size_t i = index(pattern, len, startIndex,  cmp);
  return RWWSubString(*this, i, i == RW_NPOS ? 0 : len);
}
#endif

RWWSubString&
RWWSubString::operator=(const RWWString& str) 
{
  if( !isNull() )
    str_->replace(begin_, extent_, str.data(), str.length());

  return *this;
}

RWWSubString&
RWWSubString::operator=(const wchar_t* cs)
{
  if (!isNull() )
    str_->replace(begin_, extent_, cs, wcslen(cs));

  return *this;
}

RWBoolean rwexport
operator==(const RWWSubString& ss, const wchar_t* cs)
{
  RWPRECONDITION2(cs!=rwnil,"::operator==(const RWWSubString& ss, const wchar_t* cs): nil pointer");

  if ( ss.isNull() ) return *cs =='\0'; // Two null strings compare equal

  RWASSERT(ss.begin_+ss.extent_<=ss.str_->length());

  const wchar_t* data = ss.str_->data() + ss.begin_;
  for (size_t i = 0; cs[i]; ++i)
    if (cs[i] != data[i] || i == ss.extent_) return FALSE;
  return (i == ss.extent_);
}

RWBoolean rwexport
operator==(const RWWSubString& ss, const RWWString& s)
{
  if (ss.isNull()) return s.isNull(); // Two null strings compare equal.
  if (ss.extent_ != s.length()) return FALSE;
  return !memcmp(ss.str_->data() + ss.begin_, s.data(), ss.extent_*sizeof(wchar_t));
}

RWBoolean rwexport
operator==(const RWWSubString& s1, const RWWSubString& s2)
{
  if (s1.isNull()) return s2.isNull();
  if (s1.extent_ != s2.extent_) return FALSE;
  return !memcmp(s1.str_->data()+s1.begin_, s2.str_->data()+s2.begin_, s1.extent_*sizeof(wchar_t));
}

// Convert self to lower-case
void
RWWSubString::toLower()
{
  if(!isNull())
  {				// Ignore null substrings
     str_->cow();
     register wchar_t* p = (wchar_t*)(str_->data() + begin_); // Cast away constness
     size_t N = extent_;
     while( N-- ) { *p = towlower(*p); p++;}
  }
}

// Convert self to upper-case
void
RWWSubString::toUpper()
{
  if(!isNull())
  {				// Ignore null substrings
     str_->cow();
     register wchar_t* p = (wchar_t*)(str_->data() + begin_); // Cast away constness
     size_t N = extent_;
     while( N-- ) { *p = towupper(*p); p++;}
  }
}

void
RWWSubString::subStringError(size_t sr, size_t start, size_t n) const
{
  RWTHROW(RWBoundsErr(RWMessage(RWTOOL_SUBSTRING,
				(unsigned)start,
				(unsigned)n,
				(unsigned)sr) ) );
}

void
RWWSubString::assertElement(size_t i) const
{
  if (i==RW_NPOS || i>=length())
    RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEX,
				  (unsigned)i,
				  (unsigned)length()) ) );
}


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                                MISC                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef RW_NO_WCSXFRM

RWWString rwexport
strXForm(const RWWString& wstr)
{
  // Get the size required to transform the string:
  size_t N = wcsxfrm(NULL, wstr.data(), 0);

  RWWString temp((wchar_t)0, N);

  // Return null string in case of failure:
  if ( wcsxfrm((wchar_t*)temp.data(), wstr.data(), N) >= N )
    return RWWString();

  return temp;
}

#endif /* RW_NO_WCSXFRM */


// Case-insensitive wide character comparison:
extern "C" int wscasecmp(const wchar_t* ws1, const wchar_t* ws2)
{
  RWPRECONDITION(ws1);
  RWPRECONDITION(ws2);

  for ( ; *ws1 && *ws2; ws1++, ws2++)
  {
    wchar_t ch1 = towupper(*ws1);
    wchar_t ch2 = towupper(*ws2);
    if (ch1 < ch2) return -1;
    if (ch1 > ch2) return  1;
  }
  return 0;
}

// Case-insensitive limited wide character comparison
extern "C" int wsncasecmp(const wchar_t* ws1, const wchar_t* ws2, size_t N)
{
  RWPRECONDITION(ws1);
  RWPRECONDITION(ws2);

  for ( ; *ws1 && *ws2 && N--; ws1++, ws2++)
  {
    wchar_t ch1 = towupper(*ws1);
    wchar_t ch2 = towupper(*ws2);
    if (ch1 < ch2) return -1;
    if (ch1 > ch2) return  1;
  }
  return 0;
}

#else	/* RW_NO_WSTR */

// This is to quiet fussy libraries if nothing gets compiled:
int rwDummy_wstring_cpp;

#endif /* RW_NO_WSTR */
