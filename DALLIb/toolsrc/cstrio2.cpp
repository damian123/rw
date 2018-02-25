/*
 * Definitions for RWCString RWFile and vstream Input/Output functions
 *
 * $Id: cstrio2.cpp,v 6.5 1994/07/18 20:51:00 jims Exp $
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
 * These functions are included in their own object file to remove
 * dependency between RWCStrings and RWFile/RWvstreams
 *
 ***************************************************************************
 *
 * $Log: cstrio2.cpp,v $
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.3  1994/06/17  07:26:17  jims
 * Change RWCString::pref_ to data_ for easier debuggin
 *
 * Revision 6.2  1994/06/02  18:26:31  foote
 * Port to Symantec 7.0 on Macintosh
 *
 * Revision 6.1  1994/04/15  19:05:57  vriezen
 * Move all files to 6.1
 *
 * Revision 2.2  1993/11/18  22:11:38  jims
 * SaveOn and RestoreFrom now checks vstream version number
 *
 * Revision 2.1  1993/11/17  04:11:57  keffer
 * Split RWCString I/O functions
 *
 * Revision 2.0  1993/10/28  23:06:55  alv
 * moved RWFile and vstream I/O from cstrngio to here
 *
 */

#include "rw/cstring.h"
#include "rw/rwfile.h"
#include "rw/vstream.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: cstrio2.cpp,v $ $Revision: 6.5 $ $Date: 1994/07/18 20:51:00 $");

void
RWCString::restoreFrom(RWvistream& strm)
{
  size_t len;

  strm >> len;			// Get the string length & check for errors.
  if (strm.good())
  {
    clobber(len);

    RWASSERT(capacity() >= len);
    if (strm.version() != 0) {
      strm.getString(data_, len+1);
      pref()->nchars_ = len;
    } 
    else {
      strm.get(data_, len);
      data_[pref()->nchars_ = len] = '\0';
    }
  }
}

void
RWCString::restoreFrom(RWFile& file)
{
  size_t len;

  if(file.Read(len))		// Get the string length & check for errors.
  {
    clobber(len);

    RWASSERT(capacity() >= len);
    file.Read(data_, len); // Read the string.
    data_[pref()->nchars_ = len] = '\0';
  }
}

void
RWCString::saveOn(RWvostream& strm) const
{
  strm << length();
  if (strm.version() != 0)
    strm.putString(data(), length());  // output as characters
  else
    strm.put(data(), length());        // version 0: output as numbers
}

void
RWCString::saveOn(RWFile& file) const
{
  size_t len = length();

  // Store the number of characters, then the string itself,
  // without the terminating null:
  if( file.Write(len) )  file.Write(data(), len);
}

