
/*
 * Arbitrary lengthed bit vectors --- definitions.
 *
 * $Id: bitvec.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: bitvec.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:05:18  vriezen
 * Move all files to 6.1
 *
 * Revision 2.16  1993/11/09  05:27:54  jims
 * Port to ObjectStore
 *
 * Revision 2.15  1993/09/15  20:32:22  keffer
 * Changed hash algorithm to avoid clustering.
 *
 * Revision 2.14  1993/09/15  20:25:11  keffer
 * Changed hashing algorithm to avoid potential overindexing.
 *
 * Revision 2.13  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.12  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.11  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.10  1993/04/30  00:01:06  myersn
 * work around IBM xlC overloading bug.
 *
 * Revision 2.9  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.8  1993/03/24  23:49:27  keffer
 * Indexing operations now used unsigned
 *
 * Revision 2.7  1993/02/27  00:24:30  keffer
 * Added text messages to PRECONDITION clauses.
 *
 * Revision 2.6  1993/02/06  03:48:12  keffer
 * Changed name _defs.h to defmisc.h to avoid collision with Borland header file
 *
 * Revision 2.5  1993/01/27  19:16:54  keffer
 * Now uses RWor, RWxor, and RWand to implement respective operators.
 *
 * Revision 2.4  1993/01/25  21:21:34  keffer
 * Corrected error in sum(const RWBitVec&) in last byte.
 *
 * Revision 2.3  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.2  1992/11/05  23:32:16  keffer
 * Now includes header file defmisc.h instead of mask.h
 *
 * Revision 2.1  1992/10/31  23:46:03  keffer
 * Check for BSD changed to RW_NON_ANSI_HEADERS
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.8   29 May 1992 09:46:36   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.7   27 May 1992 18:08:54   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.6   04 Mar 1992 09:17:16   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.5   18 Feb 1992 19:26:38   KEFFER
 * Improved equality check.
 * 
 *    Rev 1.4   05 Nov 1991 14:05:38   keffer
 * Removed RWXXXXErr macros --- now done by error package
 * 
 *    Rev 1.2   10 Oct 1991 09:37:14   keffer
 * Exported global operators and ::sum().
 * 
 *    Rev 1.1   15 Aug 1991 17:09:38   keffer
 * Corrected some errors having to do with assumptions about the
 * values in the last, partially full byte.
 * 
 *    Rev 1.0   28 Jul 1991 08:38:44   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

/*
 * This code assumes that a byte is 8 bits.  The value of any unused
 * bytes is left unspecified.  
 */

#include "rw/bitvec.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"
#include "defmisc.h"
STARTWRAP
#if defined(RW_NON_ANSI_HEADERS)
#  include <memory.h>		/* Looking for memcpy() */
#else
#  include <string.h>
#endif
ENDWRAP

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: bitvec.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

// Amount to shift hash values to avoid clustering

const unsigned RW_HASH_SHIFT = 5;

inline static unsigned mash(unsigned hash, unsigned v)
{
  // Rotates hash values:
  return (v ^ ((hash << RW_HASH_SHIFT) |
	       (hash >> (RWBITSPERBYTE*sizeof(unsigned) - RW_HASH_SHIFT))));
}

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

RWBitVec::RWBitVec(size_t N)
  : vec_(rwnil),
    npts_(N)
{
  if( npts_ )
  {
    size_t nb;
    vec_ = new RWByte[nb = nbytes()];
    memset(vec_, 0, nb);
  }
}

RWBitVec::RWBitVec(size_t N, RWBoolean init)
  : vec_(rwnil),
    npts_(N)
{
  if(npts_)
  {
    vec_ = new RWByte[nbytes()];
    *this = init;
  }
}

// Construct a RWBitVec and initialize it from the specified byte vector.
RWBitVec::RWBitVec(const RWByte* src, size_t N)
  : vec_(rwnil),
    npts_(N)
{
  RWPRECONDITION2(src!=rwnil,"RWBitVec::RWBitVec(const RWByte*,size_t): Initialization from nil buffer");

  if(npts_)
  {
    size_t nb = nbytes();
    vec_ = new RWByte[nb];
    memcpy(vec_, src, nb);
  }
}

// Copy constructor
RWBitVec::RWBitVec(const RWBitVec& U)
  : vec_(rwnil),
    npts_(U.npts_)
{
  if(npts_)
  {
    size_t nb = nbytes();
    vec_ = new RWByte[nb];
    memcpy(vec_, U.vec_, nb);
  }
}

RWBitVec::~RWBitVec()
{
  RWVECTOR_DELETE(nbytes()) vec_;
}

RWBitVec&
RWBitVec::operator=(const RWBitVec& U)
{
  if(vec_ != U.vec_)
  {
    delete vec_;
    vec_ = rwnil;
    npts_ = U.npts_;
    if(npts_){
      size_t nb = nbytes();
      vec_ = new RWByte[nb];
      memcpy(vec_, U.data(), nb);
    }
  }
  return *this;
}

// Set all elements to a RWBoolean value
RWBitVec&
RWBitVec::operator=(RWBoolean b)
{
  register size_t nb = nbytes();
  register RWByte* dp = vec_;
  register unsigned char val = b ? ~0 : 0;
  while (nb--) *dp++ = val;
  return *this;
}

RWBoolean
RWBitVec::operator==(RWBoolean b) const
{
  register RWByte val  = b ? ~0 : 0;		// All bits on or off
  register size_t nf = nfull();		// Number of full bytes
  size_t nb          = nbytes();		// Total number of bytes in *vec_
  RWByte mask          = (1 << (npts_&7)) - 1;	// Mask to be used with partially full bytes

  // Check the full bytes:
  for (register size_t i=0; i<nf; i++)
    if (vec_[i] != val)
      return FALSE;
    
  // Check the last (partially full) byte, if any:
  return nf==nb ? TRUE : (val & mask) == (vec_[nf] & mask);
}

RWBitVec rwexport
operator!(const RWBitVec& u)
{
  size_t nb = u.nbytes();
  RWBitVec t(u.length());

  RWByte* dp       = t.vec_;
  const RWByte* up = u.vec_;

  while (nb--) *dp++ = ~(*up++);

  return t;
}

// Binary bitwise operator on two RWBitVecs.
RWBitVec rwexport
operator&(const RWBitVec& U, const RWBitVec& V)
{
  if(U.length() != V.length()) U.lengthErr(U.length(),V.length());
  RWBitVec temp(U);
  temp &= V;
  return temp;
}

// Binary bitwise operator on two RWBitVecs.
RWBitVec rwexport
operator^(const RWBitVec& U, const RWBitVec& V)
{
  if(U.length() != V.length()) U.lengthErr(U.length(),V.length());
  RWBitVec temp(U);
  temp ^= V;
  return temp;
}

// Binary bitwise operator on two RWBitVecs.
RWBitVec rwexport
operator|(const RWBitVec& U, const RWBitVec& V)
{
  if(U.length() != V.length()) U.lengthErr(U.length(),V.length());
  RWBitVec temp(U);
  temp |= V;
  return temp;
}

// Return a suitable hash value
unsigned
RWBitVec::hash() const
{
  size_t nf   = nfull();		// Number of full bytes
  size_t nb   = nbytes();		// Total number of bytes in *vec_
  RWByte mask = (1 << (npts_&7)) - 1;	// Mask to be used with partially full bytes
  unsigned h  = (unsigned)length();	// Hash value

  // Hash the full bytes:
  for (size_t i=0; i<nf; i++)
    h = mash(h, (unsigned)vec_[i]);

  // Hash the last (partially full) byte, if any:
  if (nb!=nf) h = mash(h, (unsigned)(vec_[i] & mask));
  return h;
}

RWBoolean
RWBitVec::isEqual(const RWBitVec& u) const
{
  if( npts_!= u.npts_ ) return FALSE;

  size_t nf   = nfull();		// Number of full bytes
  size_t nb   = nbytes();		// Total number of bytes in *vec_
  RWByte mask = (1 << (npts_&7)) - 1;	// Mask to be used with partially full bytes

  // Check the full bytes:
  for (size_t i=0; i<nf; i++)
    if (vec_[i] != u.vec_[i])
      return FALSE;
    
  // Check the last (partially full) byte, if any:
  return nf==nb ? TRUE : (vec_[i] & mask) == (u.vec_[nf] & mask);
}

void
RWBitVec::resize(size_t N)
{
  size_t oldnpts = npts_;	// Save old number of points
  size_t oldnb   = nbytes();	// Save old data length
  npts_ = N;			// Set new number of points
  size_t nb = nbytes();  	// Get new data length

  if( nb!=oldnb ){		// Get new data space if data length changed
    RWByte* newv = new RWByte[nb];
    // IBM xlC cannot overload rwmin() properly without casts:
    memcpy(newv, vec_, rwmin((unsigned)nb, (unsigned)oldnb));
    if( nb > oldnb ) memset(newv+oldnb, 0, nb-oldnb);
    delete vec_;
    vec_ = newv;
  }
  if( npts_ > oldnpts ){
    unsigned nRemainOld = oldnpts&7;	// The number of bits being retained
    if(nRemainOld){
      RWByte mask = (1 << nRemainOld) - 1;
      *(vec_+oldnb-1) &= mask;		// This will zero out the new bits
    }
  }
}

// Count of ones in the specified RWBitVec.
size_t rwexport
sum(const RWBitVec& w)
{
  size_t nb      = w.nbytes();
  const RWByte* dp = w.data();
  size_t tot = 0;

  while (nb--)
  {
    register RWByte val = *dp++;
    register unsigned jbits = nb ? 8 : ((w.length()-1) & 7) + 1;
    for( ; jbits--; val >>= 1 ){
      if( val & 1 ) tot++;
    }
  }
  return tot;
}

/*
 * Utility functions 
 */

void
RWBitVec::indexRangeErr(size_t i) const
{
  RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEX, (unsigned)i, (unsigned)length()-1)));
}

void
RWBitVec::lengthErr(size_t n1, size_t n2) const
{
  RWTHROW(RWInternalErr(RWMessage(RWTOOL_NEVECL, (unsigned)n1, (unsigned)n2)));
}
