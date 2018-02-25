/*
 * RWBinaryTree I/O
 *
 * $Id: bintrio.cpp,v 6.3 1994/07/19 01:15:10 jims Exp $
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
 * $Log: bintrio.cpp,v $
 * Revision 6.3  1994/07/19  01:15:10  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:04:59  vriezen
 * Move all files to 6.1
 *
 * Revision 2.6  1993/12/29  00:02:01  keffer
 * Changed rogue 'unsigned' to 'size_t'.
 *
 * Revision 2.5  1993/09/14  04:10:22  keffer
 * Ported to Symantec.  Does not accept "address of" functions.
 *
 * Revision 2.4  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.3  1993/07/09  18:14:07  randall
 * for port to Centerline made static member function argument a reference.
 *
 * Revision 2.2  1993/04/14  18:39:16  keffer
 * Moved declaration for RWTreeNode into the .cpp file.
 * Now does level saveGuts(), removing the need for a balance()
 * on restore.
 *
 * Revision 2.1  1992/11/15  22:07:11  keffer
 * Removed use of macro NL
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 * 
 *    Rev 1.2   28 Oct 1991 09:23:48   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.1   28 Jul 1991 12:38:12   keffer
 * No longer uses macro Const
 * 
 *    Rev 1.0   28 Jul 1991 08:38:42   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/bintree.h"
#include "rw/rwfile.h"
#include "rw/vstream.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: bintrio.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/19 01:15:10 $");

void
RWBinaryTree::saveGuts(RWvostream& os) const
{
  os << entries();
  if( os.good() ) { // cast away const to avoid warnings
#ifdef __SC__
    ((RWBinaryTree*)this)->levelApply( RWCollection::saveObjToStream, &os);
#else
    ((RWBinaryTree*)this)->levelApply(&RWCollection::saveObjToStream, &os);
#endif
  }
}
    
void
RWBinaryTree::saveGuts(RWFile& f) const
{
  size_t N = entries();
  f.Write(N);
  // cast away const to avoid const warnings
#ifdef __SC__
  ((RWBinaryTree*)this)->levelApply( RWCollection::saveObjToRWFile, &f);
#else
  ((RWBinaryTree*)this)->levelApply(&RWCollection::saveObjToRWFile, &f);
#endif
}

