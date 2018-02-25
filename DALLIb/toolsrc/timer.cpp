/*
 * RWTimer: measures elapsed time
 *
 * $Id: timer.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: timer.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:07:59  vriezen
 * Move all files to 6.1
 *
 * Revision 2.7  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.6  1993/01/25  21:59:54  keffer
 * Simplified absoluteTime() so that it now relies on clock()
 * for all machines.
 *
 * Revision 2.4  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 *    Rev 1.0   19 May 1992 15:30:42   KEFFER
 * Initial revision.
 */

#include "rw/timer.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: timer.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

#ifdef RW_NO_CLOCK

  // The function clock() is not declared in either <time.h>
  // (which is where it should be according to Standard C), 
  // or in <stdlib.h> (a common alternative).  Declare it
  // here.
  // This is a traditional representation for clock().
  // You may have to adjust CLOCKS_PER_SEC to match your own machine.

  extern "C" long clock();
  static const double CLOCKS_PER_SEC = 1e06;

#else

# include <time.h>
# include <stdlib.h>

#endif

RWTimer::RWTimer() :
  startTime_(0),
  stopTime_(0),
  isStopped_(TRUE)
{
}

double
RWTimer::elapsedTime() const
{
  return (isStopped_ ? stopTime_ : absoluteTime()) - startTime_;
}

void
RWTimer::reset()
{
  startTime_ = 0;
  stopTime_  = 0;
  isStopped_ = TRUE;
}

void
RWTimer::start()
{
  startTime_ = absoluteTime() - elapsedTime();
  isStopped_ = FALSE;
}

void
RWTimer::stop()
{
  stopTime_ = absoluteTime();
  isStopped_ = TRUE;
}

/************************************************
 *						*
 *		Private functions		*
 *						*
 ************************************************/

double
RWTimer::absoluteTime()
{
  return (double)clock() / CLOCKS_PER_SEC;
}
