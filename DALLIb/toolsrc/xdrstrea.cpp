/*
 * Definitions for XDR streams
 *
 * $Id: xdrstrea.cpp,v 6.5 1994/07/18 20:51:00 jims Exp $
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
 * $Log: xdrstrea.cpp,v $
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.3  1994/06/21  19:16:06  vriezen
 * Added updates received from C. Perdue fixing various bugs.
 *
 * Revision 6.2  1994/06/18  03:16:36  myersn
 * add out-of-line virtual dtor.
 *
 * Revision 6.1  1994/04/15  19:08:12  vriezen
 * Move all files to 6.1
 *
 * Revision 1.14  1993/11/17  04:12:27  keffer
 * Added function putString(), to allow support of embedded nulls
 *
 * Revision 1.13  1993/09/16  00:39:45  keffer
 * Added RW_NO_XDR.
 *
 * Revision 1.12  1993/09/16  00:37:24  keffer
 * Rearranged header file includes to avoid multiple definition
 * error with IBM xlC compiler.
 *
 * Revision 1.11  1993/09/15  23:08:25  keffer
 * Fix for cfronts with K&R backends which promote floats
 * to doubles.
 *
 * Revision 1.10  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.9  1993/09/01  02:51:16  randall
 * corrected setting of variable status
 *
 * Revision 1.8  1993/08/13  20:44:48  randall
 * port to cafe: added casts from wchar_t* to long* and added definitions for
 * RWXDRistream::get(wchar_t*,size_t), and RWXDRistream::operator>>(wchar_t&)
 *
 * Revision 1.7  1993/08/12  19:17:05  randall
 * modified macro XDRISGetN(...) to increment vector argument.
 *
 * Revision 1.6  1993/05/31  21:32:54  keffer
 * Ported to IBM xlC compiler; required including <stdio.h>
 *  and <stddef.h>
 *
 * Revision 1.5  1993/05/29  18:20:25  keffer
 * Ported to Sun cfront V2.X.  NB: user may have to patch header file
 * <rpc/xdr.h>.  See instructions in docs/toolread.doc.
 *
 * Revision 1.4  1993/05/25  16:22:46  keffer
 * Now uses name2() to paste things together instead of '##'
 *
 * Revision 1.3  1993/05/05  20:12:14  johnb
 * changed unsigned in get and put calls to size_t for xlC
 *
 * Revision 1.2  1993/04/24  01:45:39  keffer
 * Added SCCS keyword
 *
 * Revision 1.1  1993/04/14  21:12:24  keffer
 * Initial revision
 *
 *
 */

// Note: NO SPACES are allowed in the parameter list of the macros
// below because of the rather brain dead SunOS 4.x preprocessor.
// For example use:
//   XDRISGetN(double,double)
//   XDRISGetN(float,float)
// NOT:
//   XDRISGetN(double, double)
//   XDRISGetN(float, float)


#include "rw/compiler.h"

#ifndef RW_NO_XDR

#include "rw/xdrstrea.h"
#include "rw/generic.h"
#include "rw/collect.h"
STARTWRAP
#include <ctype.h>
#include <string.h>
ENDWRAP

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: xdrstrea.cpp,v $ $Revision: 6.5 $ $Date: 1994/07/18 20:51:00 $");

#ifdef CPP_ANSI_RECURSION
# define new rwnew
#endif

int xdr(XDR* xdrp, RWCollectable*& rwp) {
    switch (xdrp->x_op) {
      case XDR_ENCODE:
	{
	    RWXDRostream os(xdrp);
	    rwp->recursiveSaveOn(os);
	    return os.good();
	}
      case XDR_DECODE:
	{
	    RWPRECONDITION2(rwp==0, "Free it before store into it");
	    RWXDRistream is(xdrp);
	    rwp = RWCollectable::recursiveRestoreFrom(is);
	    return is.good();
	}
      case XDR_FREE:
	break;
    }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//
// RWXDRistream definitions
//
////////////////////////////////////////////////////////////////////////////

RWXDRistream::RWXDRistream(XDR *xp) : xdrp(xp) {;}
RWXDRistream::~RWXDRistream() {;}


int
RWXDRistream::get()
{
    char c;
    if (!xdr_char(xdrp, &c)) {
	clear( rdstate() | ios::failbit );
	return EOF;
    } else {
	return c;
    }
}

RWvistream&
RWXDRistream::get(char& c)
{
    if (!xdr_char(xdrp, &c)) clear( rdstate() | ios::failbit );
    return *this;
}

#ifndef RW_NO_OVERLOAD_WCHAR
RWvistream&
RWXDRistream::get(wchar_t& c)
{
    long l = c;
    if (!xdr_long(xdrp, &l)) status |= ios::failbit;
    else c = (wchar_t)l;
    return *this;
}
#endif

#ifndef RW_NO_OVERLOAD_UCHAR
RWvistream&
RWXDRistream::get(unsigned char& c)
{
    if (!xdr_u_char(xdrp, &c)) clear( rdstate() | ios::failbit );
    return *this;
}
#endif

RWvistream&
RWXDRistream::get(char* s, size_t n) {
    u_int l=n;
    if (!xdr_bytes(xdrp, (char**)&s, &l, n))
      clear( rdstate() | ios::failbit );
    return *this;
}

#define XDRISGetN(type, xdrtype) \
RWvistream&								      \
RWXDRistream::get(type* p, size_t N)					      \
{									      \
    int i;								      \
    while (N-- && (i=name2(xdr_,xdrtype)(xdrp, p++))) {}		      \
    if (!i) clear( rdstate() | ios::failbit );                                \
    return *this;							      \
}									      \

XDRISGetN(double,double)
XDRISGetN(float,float)
XDRISGetN(int,int)
XDRISGetN(long,long)
XDRISGetN(short,short)

#ifndef RW_NO_OVERLOAD_WCHAR
RWvistream&
RWXDRistream::get(wchar_t* p, size_t n) {
    long l;
    while (n--) {
	l=*p;
	if (!xdr_long(xdrp, &l)) {
	    clear( rdstate() | ios::failbit );                                \
	    break;
	}
	*p++ = (wchar_t)l;
    }
    return *this;
}
#endif

#ifndef RW_NO_OVERLOAD_UCHAR
RWvistream&
RWXDRistream::get(unsigned char* s, size_t n) {
    u_int l=n;
    if (!xdr_bytes(xdrp, (char**)&s, &l, n))
      clear( rdstate() | ios::failbit );
    return *this;
}
#endif

XDRISGetN(unsigned short,u_short)
XDRISGetN(unsigned int,u_int)
XDRISGetN(unsigned long,u_long)

RWvistream&
RWXDRistream::getString(char* s, size_t ssize)
{
    u_int l;
    if (!xdr_bytes(xdrp, (char**)&s, &l, ssize-1))
      clear( rdstate() | ios::failbit );
    s[l]='\0';
    return *this;
}

#define XDROpGet(type, xdrtype) \
RWvistream&								      \
RWXDRistream::operator>>(type& c)					      \
{									      \
    if (!name2(xdr_,xdrtype(xdrp, &c))) clear( rdstate() | ios::failbit );    \
    return *this;							      \
}									      \

#ifndef RW_NO_OVERLOAD_UCHAR
XDROpGet(unsigned char,u_char)
#endif

XDROpGet(char,char)
XDROpGet(double,double)
XDROpGet(float,float)
XDROpGet(int,int)
XDROpGet(long,long)
XDROpGet(short,short)

XDROpGet(unsigned int,u_int)
XDROpGet(unsigned long,u_long)
XDROpGet(unsigned short,u_short)

#ifndef RW_NO_OVERLOAD_WCHAR
RWvistream&
RWXDRistream::operator>>(wchar_t& wc)
{
    long l=wc;
    if (!xdr_long(xdrp, &l)) clear( rdstate() | ios::failbit );
    else wc=(wchar_t)l;
    return *this;
}
#endif

////////////////////////////////////////////////////////////////////////////
//
// RWXDRostream definitions
//
////////////////////////////////////////////////////////////////////////////


RWXDRostream::RWXDRostream(XDR* xdrs) : xdrp(xdrs) {;}
RWXDRostream::~RWXDRostream() {}

RWvostream&
RWXDRostream::putString(const char* s, size_t N)
{
    u_int u = N;
    if (!xdr_bytes(xdrp, (char**)&s, &u,u))
       clear( rdstate() | ios::failbit );
    return *this;
}


RWvostream&
RWXDRostream::operator<<(const char* s)
{
    return putString(s, (size_t) strlen(s));
}

#define XDROpPut(type, xdrtype) \
RWvostream&								      \
RWXDRostream::operator<<(type c)					      \
{									      \
    if (!name2(xdr_,xdrtype(xdrp, &c))) clear( rdstate() | ios::failbit );    \
    return *this;							      \
}									      \

XDROpPut(char,char)
XDROpPut(double,double)
XDROpPut(int,int)
XDROpPut(unsigned int,u_int)
XDROpPut(long,long)
XDROpPut(unsigned long,u_long)
XDROpPut(short,short)
XDROpPut(unsigned short,u_short)

#ifndef RW_NO_OVERLOAD_UCHAR
XDROpPut(unsigned char,u_char)
#endif

// Special version for wchar_t:
#ifndef RW_NO_OVERLOAD_WCHAR
RWvostream&
RWXDRostream::operator<<(wchar_t c)
{
    long l = c;
    if (! xdr_long(xdrp, &l)) clear( rdstate() | ios::failbit );
    return *this;
}
#endif


// Special version for floats, to avoid problems with cfronts with
// K&R backends that promote floats to doubles:

RWvostream&
RWXDRostream::operator<<(float c)
{
  float temp = c;		// Make a temporary
  if (! xdr_float(xdrp, &temp)) clear( rdstate() | ios::failbit );
  return *this;
}


////////////////////////////////////////////////////////////////////////////
// put
////////////////////////////////////////////////////////////////////////////

RWvostream&
RWXDRostream::put(char c)
{
  return (*this << c);
}

RWvostream&
RWXDRostream::put(const char* s, size_t n)
{
    u_int l=n;
    if (!xdr_bytes(xdrp, (char**)&s, &l, n)) clear( rdstate() | ios::failbit );
    return *this;
}

// Input avoids xdr_vector, thus is more type-safe.
// Perhaps that approach is better.
#define XDROSPutN(type, xdrtype) \
RWvostream&								      \
RWXDRostream::put(const type* p, size_t N)				      \
{									      \
    if (!xdr_vector(xdrp, (char *)p, N, sizeof(type), (xdrproc_t)name2(xdr_,xdrtype))) \
      clear( rdstate() | ios::failbit );                                      \
    return *this;							      \
}									      \

#ifndef RW_NO_OVERLOAD_WCHAR
RWvostream&
RWXDRostream::put(wchar_t c)
{
  return (*this << c);
}

RWvostream&
RWXDRostream::put(const wchar_t* p, size_t N)
{
    long l;
    while (N--) {
	l=*p++;
	if (!xdr_long(xdrp, &l)) {
	    clear( rdstate() | ios::failbit );
	    break;
	}
    }
    return *this;
}
#endif

#ifndef RW_NO_OVERLOAD_UCHAR
RWvostream&
RWXDRostream::put(unsigned char c)
{
  return (*this << c);
}

RWvostream&
RWXDRostream::put(const unsigned char* s, size_t n)
{
    u_int l=n;
    if (!xdr_bytes(xdrp, (char**)&s, &l, n)) clear( rdstate() | ios::failbit );
    return *this;
}
#endif /* RW_NO_OVERLOAD_UCHAR */

XDROSPutN(short,short)
XDROSPutN(unsigned short,u_short)
XDROSPutN(int,int)
XDROSPutN(unsigned int,u_int)
XDROSPutN(long,long)
XDROSPutN(unsigned long,u_long)
XDROSPutN(float,float)
XDROSPutN(double,double)

#else	/* !RW_NO_XDR */

// This definition is always compiled in case nothing else is,
// in order to quiet down some fussy librarians:
int rwXDRstreaDummy;

#endif	/* RW_NO_XDR */
