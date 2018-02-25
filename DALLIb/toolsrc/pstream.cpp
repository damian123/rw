/*
 * Definitions for portable ASCII formatted streams
 *
 * $Id: pstream.cpp,v 6.10 1994/07/18 20:51:00 jims Exp $
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
 * $Log: pstream.cpp,v $
 * Revision 6.10  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.9  1994/07/14  13:21:38  jims
 * Move declaration of nextChar and don't initialize
 *
 * Revision 6.8  1994/07/14  05:46:15  jims
 * Break statement *s++ = getCChar() into two statements to avoid bad
 * code generation by certain compilers.
 *
 * Revision 6.7  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.6  1994/06/22  22:09:03  nevis
 * Elimination of spurious diffs.
 *
 * Revision 6.5  1994/06/18  01:29:27  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.4  1994/06/16  04:32:46  vriezen
 * Added flush() support
 *
 * Revision 6.3  1994/06/16  00:52:00  vriezen
 * Enhance precision support for RWpostream
 *
 * Revision 6.2  1994/05/09  20:04:04  vriezen
 * Increased floating precision for underlying ostream
 *
 * Revision 6.1  1994/04/15  19:07:22  vriezen
 * Move all files to 6.1
 *
 * Revision 2.23  1994/02/05  02:02:17  jims
 * Add huge pointer support for character reads and writes
 *
 * Revision 2.22  1993/11/17  21:41:24  keffer
 * [io]stream is now member data instead of multiply inheriting, thus
 * avoiding ambiguous shift operations.
 *
 * Revision 2.21  1993/11/17  04:12:27  keffer
 * Added function putString(), to allow support of embedded nulls
 *
 * Revision 2.20  1993/09/15  01:54:18  keffer
 * Initialized 'column' to zero.
 *
 * Revision 2.19  1993/09/14  17:06:05  keffer
 * Late Symantec does not have ios::init() either.
 *
 * Revision 2.18  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.17  1993/08/04  22:46:47  randall
 * removed null characters from last line in file
 *
 * Revision 2.16  1993/08/04  20:04:56  keffer
 * Ported to Symantec
 *
 * Revision 2.15  1993/07/28  18:45:53  jims
 * Implement wchar_t functions
 *
 * Revision 2.14  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.13  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.12  1993/04/15  02:03:58  keffer
 * Changed ctor's to use ios::init() to initialize streambufs.
 *
 * Revision 2.11  1993/04/14  21:12:24  keffer
 * Constructor reflects new inheritance hierarchy.
 *
 * Revision 2.10  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.9  1993/03/25  05:52:15  myersn
 * fix isprint bug with 8 bit chars & Solaris.
 *
 * Revision 2.8  1993/02/17  03:12:30  keffer
 * Changed const notation to follow style guide
 *
 * Revision 2.7  1992/11/26  23:12:10  myersn
 * *** empty log message ***
 *
 * Revision 2.6  1992/11/26  23:06:26  myersn
 * add definitions for wchar_t functions
 *
 * Revision 2.5  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.4  1992/11/15  23:03:58  keffer
 * *** empty log message ***
 *
 * Revision 2.3  1992/11/15  22:00:08  keffer
 * No longer uses function rwEatwhite()
 *
 * Revision 2.2  1992/11/15  21:51:11  keffer
 * Removed old V1.2 stream support
 *
 * Revision 2.1  1992/11/05  18:35:18  keffer
 * Removed ^Z from end of file.
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.12   10 Sep 1992 15:53:30   KEFFER
 * Fixed problem with saving a single blank character.
 * 
 *    Rev 1.11   29 May 1992 09:46:52   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.10   27 May 1992 18:09:06   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.9   04 Mar 1992 10:27:20   KEFFER
 * Changed PRECONDITION to RWPRECONDITION
 * 
 *    Rev 1.8   13 Nov 1991 11:50:44   keffer
 * Removed "near" qualifiers.
 * 
 *    Rev 1.7   05 Nov 1991 14:06:26   keffer
 * Removed RWXXXXErr macros --- now done by error package
 * 
 *    Rev 1.6   29 Oct 1991 13:58:06   keffer
 * Changed bufr from static to auto.
 * 
 *    Rev 1.5   17 Oct 1991 09:15:22   keffer
 * Changed include path to <rw/xxx.h>
 * 
 *    Rev 1.3   08 Sep 1991 19:12:56   keffer
 * Changed name of eatwhite() to rwEatwhite()
 * 
 *    Rev 1.2   29 Jul 1991 11:33:14   keffer
 * Macro RW_NO_OVERLOAD_UCHAR checks for overloaded unsigned char
 * 
 */

#include "rw/pstream.h"
#include "rw/rwerr.h"
#include "rw/coreerr.h"
STARTWRAP
#include <ctype.h>
#include <stdio.h>
#include <string.h>
ENDWRAP

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: pstream.cpp,v $ $Revision: 6.10 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

/************************************************
 *						*
 *		RWpistream definitions		*
 *						*
 ************************************************/

RWpistream::RWpistream(istream& str) : istr_(str.rdbuf()) {;}
RWpistream::RWpistream(streambuf* s) : istr_(s)           {;}
RWpistream::~RWpistream() {;}

int
RWpistream::get()
{
  int i;
  istr_ >> i;
  return eof() ? EOF : i;
}

RWvistream&
RWpistream::get(char& c)
{
  int i;
  istr_ >> i;
  c = i;
  return *this;
}

#ifndef RW_NO_OVERLOAD_WCHAR
// read a single wchar as such, not as a number.
RWvistream&
RWpistream::get(wchar_t& c)
{
  unsigned long ul;
  istr_ >> ul;
  c = ul;
  return *this;	
}
#endif

#ifndef RW_NO_OVERLOAD_UCHAR
RWvistream&
RWpistream::get(unsigned char& c)
{
  unsigned i;
  istr_ >> i;
  c = i;
  return *this;
}
#endif

RWvistream&
RWpistream::get(char* p, size_t N)
{
  int i;
  while (N--)
  {
    istr_ >> i;
    *p++ = i;
  }
  return *this;
}

#ifdef RW_SEGMENTED_ARCHITECTURE
RWvistream&
RWpistream::get(char rwhuge* p, unsigned long N)
{
  int i;
  while (N--)
  {
    istr_ >> i;
    *p++ = i;
  }
  return *this;
}
#endif

RWvistream&
RWpistream::get(double* p,size_t N)
{
  while (N--) istr_ >> *p++;
  return *this;
}

RWvistream&
RWpistream::get(float* p, size_t N)
{
  while (N--) istr_ >> *p++;
  return *this;
}

RWvistream&
RWpistream::get(int* p, size_t N)
{
  while (N--) istr_ >> *p++;
  return *this;
}

RWvistream&
RWpistream::get(long* p, size_t N)
{
  while (N--) istr_ >> *p++;
  return *this;
}

RWvistream&
RWpistream::get(short* p, size_t N)
{
  while (N--) istr_ >> *p++;
  return *this;
}

#ifndef RW_NO_OVERLOAD_WCHAR
RWvistream&
RWpistream::get(wchar_t* p, size_t N)
{
  unsigned long ul;
  while (N--) {
    istr_ >> ul;
    *p++ = ul;
  }
  return *this;
}
#endif

#ifndef RW_NO_OVERLOAD_UCHAR
RWvistream&
RWpistream::get(unsigned char* p, size_t N)
{
  unsigned i;
  while (N--)
  {
    istr_ >> i;
    *p++ = i;
  }
  return *this;
}
#endif

RWvistream&
RWpistream::get(unsigned short* p, size_t N)
{
  while (N--) istr_ >> *p++;
  return *this;
}

RWvistream&
RWpistream::get(unsigned int* p, size_t N)
{
  while (N--) istr_ >> *p++;
  return *this;
}

RWvistream&
RWpistream::get(unsigned long* p, size_t N)
{
  while (N--) istr_ >> *p++;
  return *this;
}

/*
 * Read characters in C string format from the input stream into buffer area s.
 * maxlen is the length of s, so at most maxlen-1 characters can be
 * read due to the byte required for the null terminator.
 */

RWvistream&
RWpistream::getString(char* s, size_t N)
{
  RWPRECONDITION( s!=0 );
  RWPRECONDITION( N >= 1);

  char c, nextChar;

  istr_ >> ws;			// Eat through leading whitespace

  istr_.get(c);			// Get a character

  if (!good())			// Check the stream state
    return *this;

  if (c != '\"')		// First character must be a quote
    syntaxErr("\"",c);

  while(1)
  {

    // Skip any embedded newlines.  Real newlines will be escaped with a backslash n.
    do {
      istr_.get(c);
    } while (c=='\n') ;

    // Look for the trailing quote.  This marks the end of the string.
    if (c == '\"') break;

    istr_.putback(c);

    if (--N == 0)		// Check for buffer overflow
    {
      while (c != '\"' && good())
	istr_.get(c);	// Search for trailing quote
      clear(ios::failbit | rdstate());	// Set the fail bit
      break;
     }

    nextChar = getCChar();      // Get the next character
    *s++ = nextChar;
  }

  *s = '\0';			// Add the terminating null
  return *this;

}

// Skip whitespace, then read in a single (possibly escaped) character:
RWvistream&
RWpistream::operator>>(char& c)
{
  istr_ >> ws;			// Eat through leading whitespace
  c = getCChar();
  return *this;
}

#ifndef RW_NO_OVERLOAD_WCHAR
RWvistream& RWpistream::operator>>(wchar_t& t)
{
  unsigned long ul;
  char c, *p;
  char buf[2*sizeof(unsigned long)+1];
    
  istr_ >> ws;			// Eat through leading whitespace
   
  // Make sure wchar_t is of the form "L'\x...'   
  istr_.get(buf, 5);
  if (!good()) return *this;
  if (buf[0] != 'L' ) syntaxErr("L" , buf[0]);
  if (buf[1] != '\'') syntaxErr("\'", buf[1]);
  if (buf[2] != '\\') syntaxErr("\\", buf[2]);
  if (buf[3] != 'x' ) syntaxErr("x" , buf[3]);
  
  // Now fill in buf with verified hex digits until quote or error:
  p = buf;
  istr_ >> c;
  while (c != '\'' && (p-buf < sizeof(buf)-1) && good()) {
    if (!isxdigit(c)) syntaxErr("Hex digit", c);
    *p++ = c;
    istr_ >> c;
  }
 
  if (c != '\'') syntaxErr("\'", c);
   
  *p = '\0';
  sscanf(buf, "%x", &ul);
  t = ul;
  return *this;
}
#endif

RWvistream& RWpistream::operator>>(double& t)        { istr_ >> t; return *this; }
RWvistream& RWpistream::operator>>(float& t)         { istr_ >> t; return *this; }
RWvistream& RWpistream::operator>>(int& t)           { istr_ >> t; return *this; }
RWvistream& RWpistream::operator>>(long& t)          { istr_ >> t; return *this; }
RWvistream& RWpistream::operator>>(short& t)         { istr_ >> t; return *this; }
#ifndef RW_NO_OVERLOAD_UCHAR
RWvistream& RWpistream::operator>>(unsigned char& t) { istr_ >> t; return *this; }
#endif
RWvistream& RWpistream::operator>>(unsigned int& t)  { istr_ >> t; return *this; }
RWvistream& RWpistream::operator>>(unsigned long& t) { istr_ >> t; return *this; }
RWvistream& RWpistream::operator>>(unsigned short& t){ istr_ >> t; return *this; }

/* Get a single (possibly escaped) character */
char
RWpistream::getCChar()
{
  char c;
  do {
    istr_.get(c);
  } while (c == '\n') ;		// Skip newlines

  if (c != '\\') return c;	// Check for simple, printable character

  // It's an escaped character.  Get the second character & decode it
  switch ((istr_.get(c), c)) {
  case 'b' :
    c = '\b'; break;
  case 'f' :
    c = '\f'; break;
  case 'n' :
    c = '\n'; break;
  case 'r' :
    c = '\r'; break;
  case 't' :
    c = '\t'; break;
#ifdef __STDC__
  case 'v' :
    c = '\v'; break;
  case 'a' :
    c = '\a'; break;
#endif
  case '\\':
    c = '\\'; break;
  case '\"' :
    c = '\"'; break;
  case '[' :
    c = '{'; break;
  case ']' :
    c = '}'; break;
  case 'x' : {		// Hex character code
    char buf[4];
    int i;
    istr_.get(buf,3);
    sscanf(buf,"%2x",&i);
    c = i;
  }
  } /* end switch */

  return c;
}

void
RWpistream::syntaxErr(const char* expect, char was)
{
  char context[2];
  context[0] = was;
  context[1] = '\0';
  istr_.putback(was);
  RWTHROW( RWExternalErr(RWMessage(RWCORE_SYNSTREAM, expect, context) ));
}

/************************************************
 *						*
 *		RWpostream definitions		*
 *						*
 ************************************************/

const unsigned RWpostream::MAXCOL = 80;

RWpostream::RWpostream(ostream& str)
 : ostr_(str.rdbuf()),
   column_(0)
{
  ostr_.setf(ios::scientific, ios::floatfield);
  ostr_.precision(RW_DEFAULT_PRECISION);
}

RWpostream::RWpostream(streambuf* s)
: ostr_(s),
  column_(0)
{
  ostr_.setf(ios::scientific, ios::floatfield);
  ostr_.precision(RW_DEFAULT_PRECISION);
}

RWpostream::~RWpostream() {}

/*
 * Output a string in an operating system independent
 * manner.  The string will be delimited by quotes.
 */
RWvostream&
RWpostream::putString(const char* s, size_t N)
{
  putwrap('\"');		// Leading quote
  putCChars(s, N);
  putwrap('\"');		// Trailing quote
  return *this;
}

RWvostream&
RWpostream::operator<<(const char* s)
{
  return putString(s, (size_t)strlen(s));
}

// Output a single character in an operating system independent manner
RWvostream&
RWpostream::operator<<(char c)
{
  char bufr[2];
  bufr[0] = c;
  bufr[1] = '\0';
  putCChars(bufr, 1);
  return *this;
}

#ifndef RW_NO_OVERLOAD_WCHAR
RWvostream&
RWpostream::operator<<(wchar_t c)
{
  char buf[2*sizeof(unsigned long)+7];
  sprintf(buf,"L'\\x%x'",(unsigned long)c);
  ostr_ << buf;
  return *this;
}
#endif

#ifndef RW_NO_OVERLOAD_UCHAR
RWvostream&
RWpostream::operator<<(unsigned char c)
{
  char bufr[2];
  bufr[0] = c;
  bufr[1] = '\0';
  putCChars(bufr, 1);
  return *this;
}
#endif

RWvostream& RWpostream::operator<<(double v)        { ostr_ << v << '\n'; return *this; }
RWvostream& RWpostream::operator<<(float v)         { ostr_ << v << '\n'; return *this; }
RWvostream& RWpostream::operator<<(int v)           { ostr_ << v << '\n'; return *this; }
RWvostream& RWpostream::operator<<(unsigned int v)  { ostr_ << v << '\n'; return *this; }
RWvostream& RWpostream::operator<<(long v)          { ostr_ << v << '\n'; return *this; }
RWvostream& RWpostream::operator<<(unsigned long v) { ostr_ << v << '\n'; return *this; }
RWvostream& RWpostream::operator<<(short v)         { ostr_ << v << '\n'; return *this; }
RWvostream& RWpostream::operator<<(unsigned short v){ ostr_ << v << '\n'; return *this; }

RWvostream&
RWpostream::put(char c)
{
  ostr_ << (int)c << '\n';
  return *this;
}

/*
 * Output a vector of characters as numeric values.
 */
RWvostream&
RWpostream::put(const char* p, size_t N)
{
  while (N--) ostr_ << (int)*p++ << '\n';
  return *this;
}

#ifdef RW_SEGMENTED_ARCHITECTURE
RWvostream&
RWpostream::put(const char rwhuge* p, unsigned long N)
{
  while (N--) ostr_ << (int)*p++ << '\n';
  return *this;
}
#endif

#ifndef RW_NO_OVERLOAD_WCHAR
RWvostream&
RWpostream::put(wchar_t c)
{
  ostr_ << (unsigned long)c << '\n';
  return *this;
}

RWvostream&
RWpostream::put(const wchar_t* p, size_t N)
{
  while (N--) ostr_ << (unsigned long) *p++ << '\n';	  
  return *this;
}
#endif /* RW_NO_OVERLOAD_WCHAR */

#ifndef RW_NO_OVERLOAD_UCHAR
RWvostream&
RWpostream::put(unsigned char c)
{
  ostr_ << (unsigned)c << '\n';
  return *this;
}

RWvostream&
RWpostream::put(const unsigned char* p, size_t N)
{
  while (N--) ostr_ << (unsigned)*p++ << '\n';
  return *this;
}

#endif /* RW_NO_OVERLOAD_UCHAR */

RWvostream&
RWpostream::put(const short* p, size_t N)
{
  while (N--) ostr_ << *p++ << '\n';
  return *this;
}

RWvostream&
RWpostream::put(const unsigned short* p, size_t N)
{
  while (N--) ostr_ << *p++ << '\n';
  return *this;
}

RWvostream&
RWpostream::put(const int* p, size_t N)
{
  while (N--) ostr_ << *p++ << '\n';
  return *this;
}

RWvostream&
RWpostream::put(const unsigned int* p, size_t N)
{
  while (N--) ostr_ << *p++ << '\n';
  return *this;
}

RWvostream&
RWpostream::put(const long* p, size_t N)
{
  while (N--) ostr_ << *p++ << '\n';
  return *this;
}

RWvostream&
RWpostream::put(const unsigned long* p, size_t N)
{
  while (N--) ostr_ << *p++ << '\n';
  return *this;
}

RWvostream&
RWpostream::put(const float* p, size_t N)
{
  while (N--) ostr_ << *p++ << '\n';
  return *this;
}

RWvostream&
RWpostream::put(const double* p, size_t N)
{
  while (N--) ostr_ << *p++ << '\n';
  return *this;
}

/*
 * Output a string in an operating system independent
 * manner.
 */
void
RWpostream::putCChars(const char* s, size_t N)
{
  while( N-- )
  {
    unsigned char c = *s++;
    const char* p = 0;
    switch (c) {		// Check for escape sequence
    case '\n' : 
      { p = "\\n"; break; }	// Line feed
    case '\t' : 
      { p = "\\t"; break; }	// Horizontal tab
    case '\b' : 
      { p = "\\b"; break; }	// Backspace
    case '\r' : 
      { p = "\\r"; break; }	// Carriage return
    case '\f' : 
      { p = "\\f"; break; }	// Form feed
#ifdef __STDC__
    case '\v' : 
      { p = "\\v"; break; }	// Vertical tab
    case '\a' : 
      { p = "\\a"; break; }	// Audible alert
#endif
    case '\\' : 
      { p = "\\\\"; break; }
    case '\"'  : 
      { p = "\\\""; break; }
    case '{'  : 
      { p = "\\["; break; }
    case '}'  : 
      { p = "\\]"; break; }
    }
    if (p)
      putwrap(p,2);	// Put 2-character escape sequence
    else {
	if (isprint((unsigned char)c) && c != ' ' )
	  putwrap(c);		// Printable character
	else {			// Hex character code
	  char buf[8];
	  sprintf(buf,"\\x%02x",c);
	  putwrap(buf,4);
	}
    }
  }
}

void
RWpostream::putwrap(char c)
{
  if (++column_ > MAXCOL) {
    column_ = 1;
    ostr_ << '\n';
  }
  ostr_ << c;
}

#ifndef RW_NO_OVERLOAD_UCHAR
void
RWpostream::putwrap(unsigned char c)
{
  putwrap((char)c);
}
#endif

void
RWpostream::putwrap(const char* s, size_t len)
{
  column_ += len;
  if (column_ > MAXCOL) {
    column_ = len;
    ostr_ << '\n';
  }
  ostr_ << s;
}

RWvostream&  RWpostream::flush() {
  ostr_.flush();
  return *this;
}
