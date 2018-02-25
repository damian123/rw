/*
 * Definitions for RWCString Input/Output functions
 *
 * $Id: cstrngio.cpp,v 6.8 1994/07/18 20:51:00 jims Exp $
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
 * $Log: cstrngio.cpp,v $
 * Revision 6.8  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.7  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.6  1994/07/12  18:00:09  foote
 * Reimplement 6.3 changes
 *
 * Revision 6.5  1994/07/07  21:34:31  myersn
 * leave room for istream::get(cp, n, delim) to put the terminator.
 *
 * Revision 6.4  1994/06/17  07:26:55  jims
 * Change RWCString::pref_ to data_ for easier debugging
 *
 * Revision 6.3  1994/06/02  18:28:32  foote
 * Port to Symantec 7.0 on Macintosh
 *
 * Revision 6.2  1994/05/16  17:44:17  jims
 * Port to Win32 DLL
 *
 * Revision 6.1  1994/04/15  19:05:59  vriezen
 * Move all files to 6.1
 *
 * Revision 2.11  1994/01/05  16:15:12  griswolf
 * fix readToken to never eat trailing white character
 *
 * Revision 2.10  1993/11/23  22:57:03  alv
 * includes iostream, not rw/rstream so rstream doesn't have to
 * be in Money.h++
 *
 * Revision 2.9  1993/11/17  04:11:57  keffer
 * Split RWCString I/O functions
 *
 * Revision 2.8  1993/11/17  04:06:11  keffer
 * Now stores embedded nulls in a portable manner.
 *
 * Revision 2.7  1993/11/14  22:01:04  keffer
 * Introduced clobber()
 *
 * Revision 2.6  1993/11/02  01:42:01  keffer
 * Now honors setw()
 *
 * Revision 2.5  1993/09/14  17:06:37  keffer
 * Symantec now has the more standard name "iomanip.h".
 *
 * Revision 2.4  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.3  1993/08/10  05:52:33  myersn
 * reserve space to read chars into, in readToDelim() etc.
 *
 * Revision 2.2  1993/07/29  16:28:48  keffer
 * Changed logic in RWCString::readFile().
 *
 * Revision 2.1  1993/07/28  22:35:49  keffer
 * New architecture using variable lengthed RWCStringRef
 *
 * Revision 1.13  1993/07/28  03:21:13  myersn
 * fix fixes to readToken and readToDelim.
 *
 * Revision 1.12  1993/07/27  22:01:03  myersn
 * support embedded nulls in saveOn() and read*().
 *
 * Revision 1.11  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 1.10  1993/03/25  05:47:17  myersn
 * fix isspace bug with 8 bit chars and Solaris.
 *
 * Revision 1.9  1993/03/20  22:51:26  keffer
 * All lengths are now unsigned.
 *
 * Revision 1.8  1993/02/23  03:35:19  myersn
 * fix readFile() on CRLF machines (e.g. dos, os/2).
 *
 * Revision 1.6  1993/01/28  21:55:32  myersn
 * readFile() treats nulls as valid characters.
 *
 * Revision 1.5  1993/01/28  20:00:53  myersn
 * make operator<< handle embedded nulls correctly.
 *
 * Revision 1.4  1993/01/26  21:34:07  myersn
 * add skipWhite default argument for RWCString::readLine
 *
 * Revision 1.3  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 1.1  1992/11/16  03:23:25  keffer
 * Initial revision
 * 
 */

#include "rw/cstring.h"
#include "iostream.h"
STARTWRAP
#include <ctype.h>		/* Looking for isspace() */
ENDWRAP

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: cstrngio.cpp,v $ $Revision: 6.8 $ $Date: 1994/07/18 20:51:00 $");

// Looking for 'ws' iomanip:
#ifdef __GLOCK__
#  include <iomanip.hxx>
#else
#  include <iomanip.h>
#endif

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

/********************************************************
 *							*
 *		RWCString I/O				*
 *							*
 ********************************************************/

// Replace self with the contents of strm, stopping at an EOF.
istream&
RWCString::readFile(istream& strm)
{
  clobber(getInitialCapacity());

  while(1)
  {
    strm.read(data_+length(), capacity()-length());
    pref()->nchars_ += strm.gcount();

    if (!strm.good())
      break;			// EOF encountered

    // If we got here, the read must have stopped because
    // the buffer was going to overflow.  Resize and keep
    // going.
    capacity(length() + getResizeIncrement());
  }

  data_[length()] = '\0';			// Add null terminator

  if (capacity()-length() > getMaxWaste())
    capacity(adjustCapacity(capacity()));

  return strm;
}

istream&
RWCString::readLine(istream& strm, RWBoolean skipWhite)
{
  if (skipWhite)
    strm >> ws;

  return readToDelim(strm, '\n');
}

istream&
RWCString::readString(istream& strm)
{
  return readToDelim(strm, '\0');
}


/*
 * Read up to an EOF, or a delimiting character, whichever comes
 * first.  The delimiter is not stored in the string,
 * but is removed from the input stream.  
 *
 * Because we don't know how big a string to expect, we first read
 * as much as we can and then, if the EOF or null hasn't been
 * encountered, do a resize and keep reading.
 */

istream&
RWCString::readToDelim(istream& strm, char delim)
{
  clobber(getInitialCapacity());

  while (1)
  {
    strm.get(data_+length(),	        // Address of next byte
	     capacity()-length()+1,	// Space available (+1 for terminator)
	     delim);			// Delimiter
    pref()->nchars_ += strm.gcount();
    if (!strm.good()) break;		// Check for EOF or stream failure
    int p = strm.peek();
    if (p == delim)			// Check for delimiter
    {
      strm.get();			// eat the delimiter.
      break;
    }
    // Delimiter not seen.  Resize and keep going:
    capacity(length() + getResizeIncrement());
  }

  data_[length()] = '\0';		// Add null terminator

  if (capacity()-length() > getMaxWaste())
    capacity(adjustCapacity(capacity()));

  return strm;
}

/*
 * Read a token, delimited by whitespace, from the input stream.
 */

istream&
RWCString::readToken(istream& strm)
{
  clobber(getInitialCapacity());

  strm >> ws;					// Eat whitespace

  size_t wid = strm.width(0);
  char c;
  RWBoolean hitSpace = FALSE;
  while ((wid == 0 || pref()->nchars_ < wid) &&
	 strm.get(c).good() && (0==(hitSpace = isspace((unsigned char)c))))
  {
    // Check for overflow:
    if (length() == capacity())
      capacity(length() + getResizeIncrement());

    RWASSERT(capacity() > length());
    data_[pref()->nchars_++] = c;
  }
  if(hitSpace)
    strm.putback(c);

  data_[length()] = '\0';			// Add null terminator

  if (capacity()-length() > getMaxWaste())
    capacity(adjustCapacity(capacity()));

  return strm;
}

/************************ Global functions ****************************/

// Typedefs are the easiest way to make more compilers happy
// about where the "rwexport" should be:
typedef ostream& ostreamRef;
typedef istream& istreamRef;

istreamRef rwexport
operator>>(istream& strm, RWCString& s)
{
  return s.readToken(strm);
}

ostreamRef rwexport
operator<<(ostream& os, const RWCString& s)
{
  if (os.opfx()) {
    size_t len = s.length();
    size_t wid = os.width();
    wid = (len < wid) ? wid - len : 0;
    os.width(wid);
    long flags = os.flags();
    if (wid && !(flags & ios::left))
      os << "";  // let the ostream fill
    os.write((char*)s.data(), s.length());
    if (wid && (flags & ios::left))
      os << "";  // let the ostream fill
  }
  os.osfx();
  return os;
}
