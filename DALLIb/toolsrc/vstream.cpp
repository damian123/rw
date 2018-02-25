/*
 * Definitions for abstract base virtual streams class
 *
 * $Id: vstream.cpp,v 6.6 1994/07/18 20:51:00 jims Exp $
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
 ***************************************************************************
 *
 * $Log: vstream.cpp,v $
 * Revision 6.6  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.5  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.4  1994/06/16  04:33:02  vriezen
 * Added flush support.
 *
 * Revision 6.3  1994/06/16  03:31:02  vriezen
 * Added outlined virtual dtors
 *
 * Revision 6.2  1994/04/15  21:52:17  vriezen
 * Remove RW_DEFINE_HOME and add back in RWvios::dummy function
 *
 * Revision 6.1  1994/04/15  19:08:05  vriezen
 * Move all files to 6.1
 *
 * Revision 1.4  1994/03/23  20:41:38  vriezen
 * Add RW_DEFINE_HOME macros for otherwise homeless classes.
 *
 * Revision 1.3  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.2  1993/04/15  00:49:18  keffer
 * Added RWvios::dummy() to be used for virtual function definition
 * heuristics.
 *
 * Revision 1.1  1993/02/23  14:58:50  alv
 * Initial revision
 *
 */

#include "rw/vstream.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: vstream.cpp,v $ $Revision: 6.6 $ $Date: 1994/07/18 20:51:00 $");

RWios::~RWios() { ; }

RWvios::~RWvios() { ; }

RWvostream::~RWvostream() { ; }
RWvistream::~RWvistream() { ; }

RWvostream& flush(RWvostream& s) { return s.flush();}