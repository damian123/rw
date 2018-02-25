/*
 * Miscellaneous Tools.h++ functions
 *
 * $Id: toolmisc.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
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
 * No need to export functions such as rwPatternFact(), rwmemmove(),
 * etc, as these are used internally by the library and need not be
 * visible externally.
 *
 ***************************************************************************
 * $Log: toolmisc.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/05/13  16:48:26  vriezen
 * Removed RW versions of stricmp and strnicmp as they are no longer used, and were broken besides.
 *
 * Revision 6.1  1994/04/15  19:08:01  vriezen
 * Move all files to 6.1
 *
 * Revision 2.17  1994/01/04  22:35:08  jims
 * Remove incorrect assertion from bsearch
 *
 * Revision 2.16  1993/09/16  04:14:22  keffer
 * Changed function prototype for RWbsearch to satisfy CSet++.
 *
 * Revision 2.15  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.14  1993/09/03  00:00:03  keffer
 * Added global function rwToolsVersion().
 *
 * Revision 2.13  1993/08/03  18:27:39  dealys
 * Ported to MPW C++ 3.3
 *
 * Revision 2.12  1993/07/31  01:36:41  keffer
 * Added routines rwFirstFalse() and rwFirstTrue().
 *
 * Revision 2.11  1993/04/15  20:16:54  myersn
 * fix toupper to accommodate bug in Solaris.
 *
 * Revision 2.10  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.9  1993/03/24  23:49:27  keffer
 * Indexing operations now used unsigned
 *
 * Revision 2.8  1993/02/18  18:04:21  keffer
 * Removed the no longer needed rwPatternFact() function.
 *
 * Revision 2.7  1993/02/08  18:22:45  keffer
 * RWor, RWand, RWxor, RWbsearch now exported from DLL
 *
 * Revision 2.6  1993/02/08  18:07:20  keffer
 * RWbsearch now consistently presents candidates as first argument to
 * comparison function, keys as second argument.
 *
 * Revision 2.5  1993/02/07  23:07:30  keffer
 * Every string now contains the null string.
 *
 * Revision 2.4  1993/02/06  03:48:12  keffer
 * Changed name _defs.h to defmisc.h to avoid collision with Borland header file
 *
 * Revision 2.3  1993/01/27  19:17:20  keffer
 * RWor, RWxor, and RWand now always compiled.
 *
 * Revision 2.2  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.1  1992/11/05  23:26:38  keffer
 * Added definitions for stricmp and strnicmp for those compilers that don't have them.
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.5   27 May 1992 18:19:24   KEFFER
 * RWor() and friends now have formal parameter "size_t" instead of unsigned.
 * 
 *    Rev 1.4   05 Nov 1991 14:07:00   keffer
 * Removed RWXXXXErr macros --- now done by error package
 * 
 *    Rev 1.2   24 Sep 1991 18:51:30   keffer
 * msg scratch space now comes off the stack
 * 
 */

/*
 * Used for various hashing algorithms:
 */

#include "rw/tooldefs.h"
STARTWRAP
#include <ctype.h>
ENDWRAP
#include "defmisc.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: toolmisc.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

/*
 * Initialize the character mask.  You can think of it as an array
 * of unsigneds. The length of the array is the number of bytes in an
 * unsigned (i.e., sizeof(unsigned).).  Each element of an array, say
 * element "i", is a mask to retrieve the "i" bytes from the unsigned.
 *
 * Nothing is specified about the ordering of the bytes within the unsigned! 
 * Hence, this algorithm will work on big- and little-endian machines.  
 */

RWCharMask::RWCharMask()
{
  for (register size_t i=0; i<sizeof(unsigned); i++)
    for (register size_t j=0; j<sizeof(unsigned); j++)
      ch[sizeof(unsigned)*i+j] = j<i ? 0xff : 0;
}

#ifdef RW_NO_MEMMOVE

/*
 * This function is provided for those machines that do not
 * provide a memmove() function.
 */

extern "C" void* rwmemmove(void* dest, const void* src, size_t n)
{
  register const char* psrc  = (const char*)src;
  register       char* pdest = (char*)dest;
  register size_t N = n;

  if (psrc < pdest)
    while (N--) pdest[N] = psrc[N];
  else {
    register size_t i = 0;
    while (i<N) { pdest[i] = psrc[i]; i++; }
  }
  return dest;
}

#endif


/*
 * Used by various bit vector routines:
 */

void rwexport
RWor (RWByte* a, const RWByte* b, size_t N)
{ while(N--) *a++ |= *b++; }

void rwexport
RWxor(RWByte* a, const RWByte* b, size_t N)
{ while(N--) *a++ ^= *b++; }

void rwexport
RWand(RWByte* a, const RWByte* b, size_t N)
{ while(N--) *a++ &= *b++; }

size_t rwexport
rwFirstTrue(const RWByte* vec, size_t nbits)
{
  size_t Ntot  = (nbits + 7) >> 3; // Total # of bytes
  size_t Nfull = nbits >> 3;	   // Total that are full
  RWByte mask  = (1 << (nbits&7)) - 1; // Mask for (potentially) partially filled byte

  for (register size_t i=0; i<Ntot; i++)
  {
    RWByte b = vec[i];
    if (i==Nfull)
      b &= mask;

    if (b)		// Any bits set in this byte?
    {
      // Yes.  Find out which one, two at a time:
      for (register size_t j=0; j<4; j++)
      {
	if (b & 3) return (~b&1) + (j<<1) + (i<<3);
	b >>= 2;
      }
      RWASSERT(0);	// Should never reach here
    }
  }
  return RW_NPOS;
}

size_t rwexport
rwFirstFalse(const RWByte* vec, size_t nbits)
{
  size_t Ntot  = (nbits + 7) >> 3; // Total # of bytes
  size_t Nfull = nbits >> 3;	   // Total that are full
  RWByte mask  = (1 << (nbits&7)) - 1; // Mask for (potentially) partially filled byte

  for (register size_t i=0; i<Ntot; i++)
  {
    RWByte b = ~vec[i];
    if (i==Nfull)
      b &= mask;

    if (b)		// Any bits set in this byte?
    {
      // Yes.  Find out which one, two at a time:
      for (register size_t j=0; j<4; j++)
      {
	if (b & 3) return (~b&1) + (j<<1) + (i<<3);
	b >>= 2;
      }
      RWASSERT(0);	// Should never reach here
    }
  }
  return RW_NPOS;
}

/*
 * binary search routine
 */

extern "C" RWBoolean rwexport
RWbsearch(const void* key, const void* base, size_t nelem, size_t width, 
	  RWcompare compareFun, size_t& idx)
{
  idx = 0;
  if (nelem)
  {
    size_t top    = nelem - 1;
    size_t bottom = 0;
    int match;
    
    while(top>bottom)
    {
      idx = (top+bottom) >> 1;
      // Make sure first argument is candidate, second argument key:
      match = (*compareFun)((char*)base + idx*width, key);
      if (match==0)
	return TRUE;
      else if (match>0)
	top     = idx ? idx - 1 : 0u;
      else
	bottom  = idx + 1;
    }
    idx = bottom;
    return (*compareFun)((char*)base + idx*width, key)==0;
  }
  // Not found:
  return FALSE;
}

// Function to return the Tools.h++ version:
unsigned rwexport rwToolsVersion(){ return RWTOOLS; }

