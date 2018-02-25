/*
 * Definitions for Binary formatted streams
 *
 * $Id: bstream.cpp,v 6.6 1994/07/18 20:51:00 jims Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave 
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
 ***************************************************************************
 *
 * $Log: bstream.cpp,v $
 * Revision 6.6  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.5  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.4  1994/06/29  23:51:24  vriezen
 * Fix flush() to use clear() instead of setstate().
 *
 * Revision 6.3  1994/06/18  01:34:52  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.2  1994/06/16  04:32:46  vriezen
 * Added flush() support
 *
 * Revision 6.1  1994/04/15  19:05:24  vriezen
 * Move all files to 6.1
 *
 * Revision 2.16  1994/03/22  23:15:32  vriezen
 * Use default ios() initialization with ios::init() rather
 * than ios(...) initialization only.
 *
 * Revision 2.15  1994/02/05  02:02:17  jims
 * Add huge pointer support for character reads and writes
 *
 * Revision 2.14  1993/11/17  04:12:27  keffer
 * Added function putString(), to allow support of embedded nulls
 *
 * Revision 2.13  1993/08/06  20:22:31  keffer
 * Guard against float to double promotions in cfronts with
 *  K&R backends by using a float temporary.
 *
 * Revision 2.12  1993/04/30  00:03:43  myersn
 * work around overloading bug in IBM xlC.
 *
 * Revision 2.11  1993/04/14  21:12:24  keffer
 * Constructor reflects new inheritance hierarchy.
 *
 * Revision 2.10  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.9  1993/04/06  19:24:34  keffer
 * Corrected problem with setting EOF for RWbistream::get().
 *
 * Revision 2.8  1993/03/23  01:56:23  keffer
 * Explicit int to unsigned conversions where necessary.
 *
 * Revision 2.7  1993/02/27  00:21:05  keffer
 * Added text messages to PRECONDITION clauses.
 *
 * Revision 2.6  1993/02/25  00:11:11  alv
 * added rwput and rwget; these call sgetn and sputn repeatedly
 * for implementations which don't refill the streambuf buffer
 * for you automaticall (eg Metaware High C++)
 *
 * Revision 2.3  1992/11/26  23:06:26  myersn
 * add definitions for wchar_t functions
 *
 * Revision 2.2  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.1  1992/11/15  21:51:11  keffer
 * Removed old V1.2 stream support
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.6   04 Mar 1992 10:27:18   KEFFER
 * Changed PRECONDITION to RWPRECONDITION
 * 
 *    Rev 1.5   13 Nov 1991 10:34:34   keffer
 * Now checks value of sgetn and sets failbit if necessary.
 * 
 *    Rev 1.4   17 Oct 1991 09:15:30   keffer
 * Changed include path to <rw/xxx.h>
 * 
 *    Rev 1.2   29 Jul 1991 11:33:12   keffer
 * Macro RW_NO_OVERLOAD_UCHAR checks for overloaded unsigned char
 * 
 */

#include "rw/bstream.h"
#include "rw/defs.h"
STARTWRAP
#include <string.h>
ENDWRAP

/*
 * Definitions of rwb[io]stream --- read and write on a stream
 * using a binary format.
 */

/*
 * rwget and rwput are used to read and write streambufs, allowing
 * for implementations of sgetn and sputn that do not bother calling
 * underflow() or overflow() for you.  Metaware's High C++ is an
 * example.
 */

static size_t rwget(streambuf *s, char *buf, int n)
{
  int totalRead = s->sgetn(buf,n);  // vanilla sgetn() is often all you need
  buf += totalRead;
  while(totalRead<n) {
    s->sgetc();                     // forces underflow(), if buffer empty
    int avail = s->in_avail();
    if (avail>n-totalRead) {
      avail = n-totalRead;
    }
    int got = s->sgetn(buf,avail);
    if (got<1) break;
    totalRead += got;
    buf += got;
  }
  return (size_t)totalRead;
}

static size_t rwput(streambuf *s, const char *buf, int n)
{
  int totalPut = s->sputn(buf,n);  // vanilla sputn() is often all you need
  buf += totalPut;
  while(totalPut<n) {
    s->sync();
    int stuffed = s->sputn(buf,n-totalPut);
    if (stuffed<1) break;
    totalPut += stuffed;
    buf += stuffed;
  }
  return (size_t)totalPut;
}
    

/* These macros are used to read & write binary on streams */
#define RWREAD(i)           if( rwget(rdbuf(),(char*)&i,       sizeof(i)       ) != sizeof(i)        ) clear(ios::eofbit  | ios::failbit | rdstate());
#define RWREADVEC(v,count)  if( rwget(rdbuf(),(char*)v,        sizeof(*v)*count) != sizeof(*v)*count ) clear(ios::eofbit  | ios::failbit | rdstate());
#define RWWRITE(i)          if( rwput(rdbuf(),(const char*)&i, sizeof(i)       ) != sizeof(i)        ) clear(ios::failbit | rdstate());
#define RWWRITEVEC(v,count) if( rwput(rdbuf(),(const char*)v,  sizeof(*v)*count) != sizeof(*v)*count ) clear(ios::failbit | rdstate());
#define FAILBIT             ios::failbit

/************************************************
 *						*
 *		RWbistream definitions		*
 *						*
 ************************************************/

/*
The following method of constructing the ios base via init() rather
than via the initialization list assures that a derived class will not
skip ios init, by omitting the ios in its ctor.  This situation occurs
due to the rules for virtual base class construction.
*/

RWbistream::RWbistream(istream& str) : ios() {ios::init(str.rdbuf());}
RWbistream::RWbistream(streambuf* s) : ios() {ios::init(s);}
RWbistream::~RWbistream() {}

int
RWbistream::get()
{
  int i = (int)rdbuf()->sbumpc();	// Widen to int
  return eof() ? EOF : i;		// Replace with EOF if necessary.
}

RWvistream& RWbistream::get(char& c)		{ c=rdbuf()->sbumpc(); return *this; }
#ifndef RW_NO_OVERLOAD_WCHAR
RWvistream& RWbistream::get(wchar_t& c)                   { RWREADVEC((char*)&c, sizeof(wchar_t)); return *this; }
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
RWvistream& RWbistream::get(unsigned char& c) { c=rdbuf()->sbumpc(); return *this; }
#endif


RWvistream& RWbistream::get(char* p, size_t N)          { RWREADVEC(p,N); return *this; }

#ifdef RW_SEGMENTED_ARCHITECTURE
RWvistream& RWbistream::get(char rwhuge* p, unsigned long N)
{
  unsigned long        remaining = N;      // chars still to be read 
  unsigned long        roomLeftInSegment;  // max bytes before segment overflow
  const int            maxCount = 32766;   // max chars safe to read in one go
  unsigned             offset;
  int                  count;
  const unsigned long  segmentLength = 65536;

  while (remaining) {
    offset = *((unsigned*)&p);	// offset of p from start of segment
    roomLeftInSegment = segmentLength - offset;

    // count should be the least of remaining, maxCount, and roomLeftInSegment
    count = (maxCount < remaining)      ? maxCount : (int) remaining;
    count = (count < roomLeftInSegment) ? count    : (int) roomLeftInSegment;

    RWREADVEC(p, count);
    if (fail()) break;		// don't continue if not all bytes read

    remaining -= count;
    p += count;
  }	
  return *this; 
}
#endif

#ifndef RW_NO_OVERLOAD_WCHAR
RWvistream& RWbistream::get(wchar_t* p, size_t N)       { RWREADVEC((char*)p,N*sizeof(wchar_t)); return *this; }
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
RWvistream& RWbistream::get(unsigned char* p,  size_t N){ RWREADVEC(p,N); return *this; }
#endif
RWvistream& RWbistream::get(double* p,         size_t N){ RWREADVEC(p,N); return *this; }
RWvistream& RWbistream::get(float* p,          size_t N){ RWREADVEC(p,N); return *this; }
RWvistream& RWbistream::get(int* p,            size_t N){ RWREADVEC(p,N); return *this; }
RWvistream& RWbistream::get(unsigned int* p,   size_t N){ RWREADVEC(p,N); return *this; }
RWvistream& RWbistream::get(long* p,           size_t N){ RWREADVEC(p,N); return *this; }
RWvistream& RWbistream::get(unsigned long* p,  size_t N){ RWREADVEC(p,N); return *this; }
RWvistream& RWbistream::get(short* p,          size_t N){ RWREADVEC(p,N); return *this; }
RWvistream& RWbistream::get(unsigned short* p, size_t N){ RWREADVEC(p,N); return *this; }

RWvistream& RWbistream::getString(char* s, size_t Nmax)
{
  RWPRECONDITION2( s!=0,"RWbistream::getString(char*, size_t): get into nil buffer" );
  size_t N, Nget;

  RWREAD(N);		// Get the string length
  Nget = rwmin(unsigned(N), unsigned(Nmax-1));  // cast works around xlC bug.

  RWREADVEC(s, Nget);

  if( Nget < N ){
    // We have read less than the string length; 
    // Read and throw away the rest of the string, then set the fail bit.
    size_t Nleft = N-Nget;		// This is the number of unread bytes

    while( Nleft-- && good() ) rdbuf()->stossc() ;

    clear(FAILBIT | rdstate() );		// Set the fail bit
  }
  s[Nget] = '\0';	// Add the null terminator
  return *this;
}

RWvistream& RWbistream::operator>>(char& c)          { c=rdbuf()->sbumpc(); return *this; }
#ifndef RW_NO_OVERLOAD_WCHAR
RWvistream& RWbistream::operator>>(wchar_t& c) { RWREADVEC((char*)&c, sizeof(wchar_t)); return *this; }
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
RWvistream& RWbistream::operator>>(unsigned char& c) { c=rdbuf()->sbumpc(); return *this; }
#endif

RWvistream& RWbistream::operator>>(double& t)        { RWREAD(t); return *this; }
RWvistream& RWbistream::operator>>(float& t)         { RWREAD(t); return *this; }
RWvistream& RWbistream::operator>>(int& t)           { RWREAD(t); return *this; }
RWvistream& RWbistream::operator>>(long& t)          { RWREAD(t); return *this; }
RWvistream& RWbistream::operator>>(short& t)         { RWREAD(t); return *this; }
RWvistream& RWbistream::operator>>(unsigned int& t)  { RWREAD(t); return *this; }
RWvistream& RWbistream::operator>>(unsigned long& t) { RWREAD(t); return *this; }
RWvistream& RWbistream::operator>>(unsigned short& t){ RWREAD(t); return *this; }

/************************************************
 *						*
 *		RWbostream definitions		*
 *						*
 ************************************************/

/*
The following method of constructing the ios base via init() rather
than via the initialization list assures that a derived class will not
skip ios init, by omitting the ios in its ctor.  This situation occurs
due to the rules for virtual base class construction.
*/

RWbostream::RWbostream(ostream& str) : ios() {ios::init(str.rdbuf());}
RWbostream::RWbostream(streambuf* s) : ios()           {ios::init(s);}
RWbostream::~RWbostream() {}

RWvostream&
RWbostream::putString(const char* t, size_t N)
{
  RWWRITE(N);		// Write the string length
  RWWRITEVEC(t,N);	// Then the string
  return *this;
}

RWvostream& RWbostream::operator<<(const char* t)   { return putString(t, (size_t)strlen(t)); }
RWvostream& RWbostream::operator<<(char c)          { rdbuf()->sputc(c); return *this; }
#ifndef RW_NO_OVERLOAD_WCHAR
  RWvostream& RWbostream::operator<<(wchar_t c)     { RWWRITEVEC((char*)&c, sizeof(wchar_t)); return *this; }
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
  RWvostream& RWbostream::operator<<(unsigned char c) { rdbuf()->sputc(c); return *this; }
#endif

RWvostream& RWbostream::operator<<(double v)        { RWWRITE(v); return *this; }
RWvostream& RWbostream::operator<<(int v)           { RWWRITE(v); return *this; }
RWvostream& RWbostream::operator<<(unsigned int v)  { RWWRITE(v); return *this; }
RWvostream& RWbostream::operator<<(long v)          { RWWRITE(v); return *this; }
RWvostream& RWbostream::operator<<(unsigned long v) { RWWRITE(v); return *this; }
RWvostream& RWbostream::operator<<(short v)         { RWWRITE(v); return *this; }
RWvostream& RWbostream::operator<<(unsigned short v){ RWWRITE(v); return *this; }

// Guard against float to double promotions in cfronts with K&R backends:
RWvostream& RWbostream::operator<<(float v){ float fv = v; RWWRITE(fv); return *this; }

RWvostream& RWbostream::put(char c)          { rdbuf()->sputc(c); return *this; }
#ifndef RW_NO_OVERLOAD_WCHAR
RWvostream& RWbostream::put(wchar_t c)       { RWWRITEVEC((char*)&c, sizeof(wchar_t)); return *this; }
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
RWvostream& RWbostream::put(unsigned char c) { rdbuf()->sputc(c); return *this; }
#endif

RWvostream& RWbostream::put(const char* p, size_t N)          { RWWRITEVEC(p,N); return *this; }

#ifdef RW_SEGMENTED_ARCHITECTURE
RWvostream& RWbostream::put(const char rwhuge* p, unsigned long N)
{
  unsigned long        remaining = N;      // chars still to be written
  unsigned long        roomLeftInSegment;  // max bytes before segment overflow
  const int            maxCount = 32766;   // max chars safe to write in one go
  unsigned             offset;
  int                  count;
  const unsigned long  segmentLength = 65536;

  while (remaining) {
    offset = *((unsigned*)&p);	// offset of p from start of segment
    roomLeftInSegment = segmentLength - offset;

    // count should be the least of remaining, maxCount, and roomLeftInSegment
    count = (maxCount < remaining)      ? maxCount : (int) remaining;
    count = (count < roomLeftInSegment) ? count    : (int) roomLeftInSegment;

    RWWRITEVEC(p, count);
    if (fail()) break;		// don't continue if not all bytes written
      
    remaining -= count; 
    p += count;
  }	
  return *this; 
}
#endif

#ifndef RW_NO_OVERLOAD_WCHAR
RWvostream& RWbostream::put(wchar_t const* p, size_t N)       { RWWRITEVEC((char*)p,N*sizeof(wchar_t)); return *this; }
#endif
#ifndef RW_NO_OVERLOAD_UCHAR
RWvostream& RWbostream::put(const unsigned char* p,  size_t N){ RWWRITEVEC(p,N); return *this; }
#endif
RWvostream& RWbostream::put(const short* p,          size_t N){ RWWRITEVEC(p,N); return *this; }
RWvostream& RWbostream::put(const unsigned short* p, size_t N){ RWWRITEVEC(p,N); return *this; }
RWvostream& RWbostream::put(const int* p,            size_t N){ RWWRITEVEC(p,N); return *this; }
RWvostream& RWbostream::put(const unsigned int* p,   size_t N){ RWWRITEVEC(p,N); return *this; }
RWvostream& RWbostream::put(const long* p,           size_t N){ RWWRITEVEC(p,N); return *this; }
RWvostream& RWbostream::put(const unsigned long* p,  size_t N){ RWWRITEVEC(p,N); return *this; }
RWvostream& RWbostream::put(const float* p,          size_t N){ RWWRITEVEC(p,N); return *this; }
RWvostream& RWbostream::put(const double* p,         size_t N){ RWWRITEVEC(p,N); return *this; }

RWvostream& RWbostream::flush() {
    if( rdbuf()->sync() == EOF )
	clear(ios::badbit | rdstate());
    return *this;
}
