/*
 * RWBitVec I/O
 *
 * $Id: bitvecio.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
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
 * $Log: bitvecio.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/05/16  16:45:09  jims
 * Port to Win32 DLL
 *
 * Revision 6.1  1994/04/15  19:05:22  vriezen
 * Move all files to 6.1
 *
 * Revision 2.10  1993/11/09  05:27:54  jims
 * Port to ObjectStore
 *
 * Revision 2.9  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.8  1993/06/06  00:45:00  keffer
 * Prettied up formatting on output.
 *
 * Revision 2.7  1993/05/18  00:45:13  keffer
 * Moved overloaded shift operators from bitvec.cpp to here.
 *
 * Revision 2.6  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.4  1993/02/23  04:29:24  myersn
 * fix scanFrom() to eat white space properly.
 *
 * Revision 2.3  1993/01/27  20:02:53  keffer
 * Changed variable names.
 *
 * Revision 2.2  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.1  1992/11/15  22:00:08  keffer
 * No longer uses function rwEatwhite()
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.5   29 May 1992 09:46:36   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.4   27 May 1992 18:08:54   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.3   24 Mar 1992 19:27:32   KEFFER
 * Ported to MetaWare High C.
 * 
 *    Rev 1.2   28 Oct 1991 09:23:50   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.1   08 Sep 1991 19:12:54   keffer
 * Changed name of eatwhite() to rwEatwhite()
 * 
 *    Rev 1.0   28 Jul 1991 08:38:46   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/bitvec.h"
#include "rw/rwfile.h"
#include "rw/vstream.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: bitvecio.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

const size_t default_resize = 32;	// Used for input

ostream&
RWBitVec::printOn(ostream& strm) const
{
  strm << "[\n  ";
  for (size_t i=0; i<length(); i++) {
    if(i>0 && (i%25 == 0)) strm << "\n  ";
    strm << ( (*this)(i) ? "1 " : "0 ");
  }
  return strm << "\n]";
}

void
RWBitVec::restoreFrom(RWvistream& strm)
{
  size_t oldnb = nbytes();		// Save old data space length

  strm >> npts_;		// Find new number of bits
				
  if( strm.good() ){
    size_t nb = nbytes();	// Get new data space length
    if( nb!=oldnb) { 		// Get new data space if they differ
      delete vec_;
      vec_ = new RWByte[nb];
    }
    strm.get((char*)vec_,nb);	// Get the bit pattern
  }
}

void
RWBitVec::restoreFrom(RWFile& file)
{
  size_t oldnb = nbytes();	// Save old data space length

  if( file.Read(npts_) ){
    size_t nb = nbytes();	// Get new data space length
    if( nb!=oldnb) { 		// Get new data space if they differ
      delete vec_;
      vec_ = new RWByte[nb];
    }
    file.Read((char*)vec_, nb);
  }
}

istream&
RWBitVec::scanFrom(istream& s)
{
  register size_t nextspace = 0;
  RWBoolean item;
  char c = 0;	// The first character read from the stream
  
  s >> ws >> c;	// Munch through any leading white space
  
  if (s && c != '[') {
    // Scan input stream, resizing as necessary.
    // Keep scanning till we can't scan no mo'
    s.putback(c);
    while( (s >> item).good() ) {
      if(nextspace >= length()) resize(length()+default_resize);
      (*this)(nextspace++) = item;
    }
  } else {  // Scan input stream, stop scanning at the matching ']' character
    
    s >> ws >> c;	// Munch through any leading white space

    while ( s && (c != ']')  ) {
      s.putback(c);
      if( (s >> item).good() ) {
        if( nextspace >= length() ) resize(length()+default_resize);
        (*this)(nextspace++) = item;
	s >> ws >> c;
      }
    }
  }

  if( s.good() ) {
    if( nextspace != length() ) resize(nextspace);
  }

  return s;
}

void
RWBitVec::saveOn(RWvostream& strm) const
{
  strm << npts_;
  size_t nb = nbytes();
  strm.put((char*)vec_,nb);
}
  
void
RWBitVec::saveOn(RWFile& file) const
{
  file.Write(npts_);
  file.Write((char*)vec_, nbytes());
}

// Typedefs are the easiest way to make more compilers happy
// about where the "rwexport" should be:
typedef ostream& ostreamRef;
typedef istream& istreamRef;

ostreamRef rwexport
operator<<(ostream& strm, const RWBitVec& u)
{
  return u.printOn(strm);
}

istreamRef rwexport
operator>>(istream& strm, RWBitVec& u)
{
  return u.scanFrom(strm);
}
