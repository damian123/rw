/*
 * Definitions for RWWString Input/Output functions
 *
 * $Id: wstrngio.cpp,v 6.5 1994/07/18 20:51:00 jims Exp $
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
 * Because the C++ streaming library does not yet define
 * any standard I/O for wide characters, we can't supply 
 * character based insertion and extraction operations.
 * Also, for persistence, we must store and restore wide
 * character strings as vectors of literal numbers.
 *
 * The functions readToDelim, readToken, operator<<, etc.
 * below should be rewritten for environments where wide string i/o
 * works, when such become available.
 *
 ***************************************************************************
 *
 * $Log: wstrngio.cpp,v $
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.3  1994/06/17  07:29:16  jims
 * change pref_ to data_ for easier debugging
 *
 * Revision 6.2  1994/05/16  17:46:12  jims
 * Port to Win32 DLL
 *
 * Revision 6.1  1994/04/15  19:08:09  vriezen
 * Move all files to 6.1
 *
 * Revision 1.19  1993/11/15  00:35:49  keffer
 * Introduced m.f. clobber()
 *
 * Revision 1.18  1993/11/02  01:42:01  keffer
 * Now honors setw()
 *
 * Revision 1.17  1993/09/10  20:05:33  keffer
 * Repositioned RW_RCSID macro.
 *
 * Revision 1.16  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.15  1993/07/29  04:07:02  keffer
 * Now uses new RWWString architecture.
 *
 * Revision 1.14  1993/06/30  00:51:23  myersn
 * fix readToDelimiter for case when the first char is the delimiter.
 *
 * Revision 1.13  1993/05/29  18:48:24  keffer
 * Renamed or added a dummy variable in case nothing gets compiled
 *
 * Revision 1.12  1993/05/25  18:35:04  keffer
 * I/O of wide character strings done in a more nearly bitwise
 * sense.  This will parallel the emerging X3J16 standard.
 *
 * Revision 1.11  1993/05/19  01:57:56  myersn
 * handle embedded nulls.
 *
 * Revision 1.10  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 1.9  1993/03/25  05:58:03  myersn
 * fix isspace bug with 8 bit chars and Solaris.
 *
 * Revision 1.6  1993/02/16  19:11:39  myersn
 * install #ifndef RW_NO_WSTR protection.
 *
 * Revision 1.4  1993/01/28  21:55:32  myersn
 * add RWWStringRef::readFile() to support new RWWString::readFile() semantics:
 * treat nulls as valid characters.
 *
 * Revision 1.2  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 1.1  1992/11/17  21:34:02  keffer
 * Initial revision
 *
 */

#include "rw/compiler.h"
#ifndef RW_NO_WSTR

#include "rw/rstream.h"
#include "rw/wstring.h"
#include "rw/vstream.h"
#include "rw/rwfile.h"
#include "rwwchar.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: wstrngio.cpp,v $ $Revision: 6.5 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

/********************************************************
 *							*
 *			UTILITY				*
 *							*
 ********************************************************/

static void rwnear fail(ios& strm) { strm.clear(strm.rdstate()|ios::failbit); }

// Gets a single wide character from an input stream.
static wchar_t rwnear rwgetwc(istream& strm)
{
  wchar_t wc = 0;
  if (strm.rdbuf()->sgetn((char*)&wc, sizeof(wc)) != sizeof(wc))
    fail(strm);
  return wc;
}
  
// Puts a single wide character to an output stream.
static void rwnear rwputwc(ostream& strm, wchar_t wc)
{
  if (strm.rdbuf()->sputn((const char*)&wc, sizeof(wc)) != sizeof(wc))
    fail(strm);
}

// Skips wide character whitespace.
// Returns the first non-whitespace character encountered.
static wchar_t rwnear rwwsSkipWhite(istream& strm)
{
  wchar_t wc = 0;

  if (!strm.good()) return wc;

  while ( (wc = rwgetwc(strm), strm.good() && iswspace(wc) ) )
  {;}
    
  return wc;
}  

/********************************************************
 *							*
 *			RWWString I/O			*
 *							*
 ********************************************************/

istream&
RWWString::readFile(istream& strm)
{
  clobber(getInitialCapacity());
  
  wchar_t wc;
  
  if (strm.good())
  {
    while ( (wc = rwgetwc(strm), strm.good()) )
    {
      if (length()==capacity())	// Check for overflow
	capacity( length() + getResizeIncrement() );
      data_[pref()->nchars_++] = wc;
    }
  
    data_[length()] = (wchar_t)0; // Add null terminator

    if (capacity()-length() > getMaxWaste())
      capacity(adjustCapacity(capacity()));

  }
  return strm;
}

istream&
RWWString::readLine(istream& strm, RWBoolean skipWhite)
{
  return readToDelim(strm, (wchar_t)'\n', skipWhite);
}

istream&
RWWString::readString(istream& strm)
{
  return readToDelim(strm, 0, FALSE);
}

istream&
RWWString::readToDelim(istream& strm, wchar_t delim)
{
  return readToDelim(strm, delim, FALSE);
}

/* Read up to an EOF, or a delimiting character, whichever comes
 * first.  The delimiter is not stored in the string,
 * but is removed from the input stream.  This parallels the
 * semantics of RWCString.
 *
 * At the time of this writing, X3J16 has not defined wide character
 * string I/O yet.  The following merely shifts in characters
 * in a bitwise sense.  Most likely this will be what they require.
 * The only likely difference is that shift in is not "tied" to
 * a possible output stream (istream::ipfx() and friends).
 */

istream&
RWWString::readToDelim(istream& strm, wchar_t delim, RWBoolean skipWhite)
{
  clobber(getInitialCapacity());
  
  wchar_t wc = (skipWhite) ? rwwsSkipWhite(strm) : rwgetwc(strm);
  while ( strm.good() && wc != delim )
  {
    if (length()==capacity())	// Check for overflow
      capacity( length() + getResizeIncrement() );
     data_[pref()->nchars_++] = wc;
    wc = rwgetwc(strm);
  }

  data_[length()] = (wchar_t)0; // Add null terminator

  if (capacity()-length() > getMaxWaste())
    capacity(adjustCapacity(capacity()));

  return strm;
}

/*
 * Read a token, delimited by whitespace, from the input stream.
 */

istream&
RWWString::readToken(istream& strm)
{
  clobber(getInitialCapacity());

  // Skip any leading wide character whitespace:
  wchar_t wc = rwwsSkipWhite(strm);
  
  size_t wid = strm.width(0);

  if (strm.good())
  {
    do
    {
      if (length()==capacity())	// Check for overflow
	capacity( length() + getResizeIncrement() );
      data_[pref()->nchars_++] = wc;
      if (wid && pref()->nchars_ >= wid) break;
      wc = rwgetwc(strm);
    } while ( strm.good() && !iswspace(wc) ) ;
  }
  
  data_[length()] = (wchar_t)0; // Add null terminator

  if (capacity()-length() > getMaxWaste())
    capacity(adjustCapacity(capacity()));

  return strm;
}

void
RWWString::restoreFrom(RWvistream& strm)
{
  size_t len;

  strm >> len;			// Get the string length & check for errors.
  if (strm.good())
  {
    clobber(len);

    RWASSERT(capacity() >= len);
    strm.get(data_, len);	// Restore the string.
    data_[pref()->nchars_ = len] = 0;
  }
}

void
RWWString::restoreFrom(RWFile& file)
{
  size_t len;

  if(file.Read(len))		// Get the string length & check for errors.
  {
    clobber(len);

    RWASSERT(capacity() >= len);
    file.Read(data_, len); // Read the string.
    data_[pref()->nchars_ = len] = (wchar_t)0;
  }
}

void
RWWString::saveOn(RWvostream& strm) const
{
  strm << length();
  strm.put(data(), length());	// No terminating null
}

void
RWWString::saveOn(RWFile& file) const
{
  size_t len = length();

  // Store the number of characters, then the string itself,
  // without the terminating null:
  if( file.Write(len) )  file.Write(data(), len);
}

/************************ Global functions ****************************/

// Typedefs are the easiest way to make more compilers happy
// about where the "rwexport" should be:
typedef ostream& ostreamRef;
typedef istream& istreamRef;

istreamRef rwexport
operator>>(istream& strm, RWWString& s)
{
  return s.readToken(strm);
}

ostreamRef rwexport
operator<<(ostream& os, const RWWString& s)
{
  if (os.opfx()) {
    size_t len = s.length();
    size_t wid = os.width(0);
    wid = (len < wid) ? wid - len : 0;
    long flags = os.flags();
    if (wid && !(flags & ios::left))
      while (wid--) rwputwc(os, (wchar_t)os.fill());
    size_t i = len;
    const wchar_t* wp = s.data();
    while (i--)
    {
      rwputwc(os, *wp++);
      if (!os.good()) break;
    }
    if (wid && (flags & ios::left))
      while (wid--) rwputwc(os, (wchar_t)os.fill());
  }
  os.osfx();
  return os;
}

#else	/* RW_NO_WSTR */
// This is to quiet fussy libraries if nothing gets compiled:
int rwDummy_wstringio_cpp;
#endif
